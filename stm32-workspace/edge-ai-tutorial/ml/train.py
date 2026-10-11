"""Train the gesture classifier from accelerometer recordings.

Reads the CSV files saved by tools/accel_viewer.html, cuts them into fixed-length
windows, trains a small 1D convolutional network and exports it as a TensorFlow
Lite model for X-CUBE-AI.

Each recording is a file named <label>.csv or <label>_<timestamp>.csv with the
header x_mg,y_mg,z_mg and one sample per line at 100 Hz. Several recordings may
share a label.

Usage:
    python ml/train.py --data motion-folder --out ml/models
"""

import argparse
import re
import sys
from pathlib import Path

import numpy as np

SAMPLE_RATE_HZ = 100
AXIS_COUNT = 3
# Samples are divided by this so the network sees values of roughly -1..1.
# The firmware must apply the same scale before running the model.
SCALE_MILLI_G = 4000.0
VALIDATION_FRACTION = 0.2
CSV_HEADER = "x_mg,y_mg,z_mg"
# Suffix the viewer appends to the label when it saves a recording
TIMESTAMP_SUFFIX = re.compile(r"_\d{8}T\d{6}$")
RANDOM_SEED = 42


def label_from_path(path):
    """Extract the gesture label from a recording file name.

    Args:
        path (Path): Recording file named <label>.csv or <label>_<timestamp>.csv.

    Returns:
        str: The label, which may itself contain underscores.

    Raises:
        ValueError: If nothing is left of the name once the timestamp is removed.
    """
    label = TIMESTAMP_SUFFIX.sub("", path.stem)
    if not label:
        raise ValueError(f"{path.name}: expected a name like <label>.csv or <label>_<timestamp>.csv")
    return label


def load_recording(path):
    """Load one recording as an array of raw samples.

    Args:
        path (Path): CSV file with the header x_mg,y_mg,z_mg.

    Returns:
        numpy.ndarray: Samples in milli-g, shape (sample_count, 3), dtype float32.

    Raises:
        ValueError: If the header or the column count is not the expected one.
    """
    with path.open(encoding="utf-8") as handle:
        header = handle.readline().strip()
        if header != CSV_HEADER:
            raise ValueError(f"{path.name}: expected header '{CSV_HEADER}', found '{header}'")
        samples = np.loadtxt(handle, delimiter=",", dtype=np.float32, ndmin=2)

    if samples.shape[1] != AXIS_COUNT:
        raise ValueError(f"{path.name}: expected {AXIS_COUNT} columns, found {samples.shape[1]}")
    return samples


def make_windows(samples, window, stride):
    """Cut a recording into overlapping fixed-length windows.

    Args:
        samples (numpy.ndarray): Samples of shape (sample_count, 3).
        window (int): Number of samples per window.
        stride (int): Number of samples between the starts of consecutive windows.

    Returns:
        numpy.ndarray: Windows of shape (window_count, window, 3). Empty along the
            first axis if the recording is shorter than one window.

    Raises:
        ValueError: If window or stride is not positive.
    """
    if window <= 0 or stride <= 0:
        raise ValueError("window and stride must be positive")

    starts = range(0, len(samples) - window + 1, stride)
    if len(starts) == 0:
        return np.empty((0, window, samples.shape[1]), dtype=samples.dtype)
    return np.stack([samples[start:start + window] for start in starts])


def build_dataset(data_dir, window, stride):
    """Load every recording and split it into training and validation windows.

    The last part of each recording is held out for validation. Splitting by time
    rather than shuffling windows keeps overlapping windows from landing on both
    sides of the split, which would make the validation score look better than it is.

    Args:
        data_dir (Path): Directory holding the recordings.
        window (int): Number of samples per window.
        stride (int): Number of samples between consecutive windows.

    Returns:
        tuple: (train_x, train_y, validation_x, validation_y, labels). The x arrays
            have shape (count, window, 3) and are scaled by SCALE_MILLI_G, the y
            arrays hold indices into labels, a sorted list of label names.

    Raises:
        ValueError: If there are no recordings, fewer than two labels, or a label
            ends up without training or validation windows.
    """
    paths = sorted(data_dir.glob("*.csv"))
    if not paths:
        raise ValueError(f"no .csv recordings found in {data_dir}")

    labels = sorted({label_from_path(path) for path in paths})
    if len(labels) < 2:
        raise ValueError(f"need at least two labels to train a classifier, found {labels}")

    parts = {"train": ([], []), "validation": ([], [])}
    for path in paths:
        label_index = labels.index(label_from_path(path))
        samples = load_recording(path) / SCALE_MILLI_G
        split = int(len(samples) * (1.0 - VALIDATION_FRACTION))

        for name, chunk in (("train", samples[:split]), ("validation", samples[split:])):
            windows = make_windows(chunk, window, stride)
            parts[name][0].append(windows)
            parts[name][1].append(np.full(len(windows), label_index, dtype=np.int64))

        print(f"  {path.name}: {len(samples) / SAMPLE_RATE_HZ:.1f} s")

    train_x = np.concatenate(parts["train"][0])
    train_y = np.concatenate(parts["train"][1])
    validation_x = np.concatenate(parts["validation"][0])
    validation_y = np.concatenate(parts["validation"][1])

    for name, targets in (("training", train_y), ("validation", validation_y)):
        counts = np.bincount(targets, minlength=len(labels))
        for label, count in zip(labels, counts):
            if count == 0:
                raise ValueError(f"label '{label}' has no {name} windows, record more of it")

    return train_x, train_y, validation_x, validation_y, labels


def build_model(window, class_count):
    """Create the gesture classifier.

    The layers are limited to ones X-CUBE-AI converts directly, and the network is
    small enough to run in well under a millisecond on a Cortex-M7.

    Args:
        window (int): Number of samples per input window.
        class_count (int): Number of gesture labels.

    Returns:
        keras.Model: Compiled model taking (window, 3) inputs and returning one
            probability per label.
    """
    import tensorflow as tf

    model = tf.keras.Sequential([
        tf.keras.layers.Input(shape=(window, AXIS_COUNT)),
        tf.keras.layers.Conv1D(8, 5, activation="relu"),
        tf.keras.layers.MaxPooling1D(2),
        tf.keras.layers.Conv1D(16, 5, activation="relu"),
        tf.keras.layers.GlobalAveragePooling1D(),
        tf.keras.layers.Dense(class_count, activation="softmax"),
    ])
    model.compile(optimizer="adam", loss="sparse_categorical_crossentropy", metrics=["accuracy"])
    return model


def confusion_matrix(targets, predictions, class_count):
    """Count how often each true label was predicted as each label.

    Args:
        targets (numpy.ndarray): True label indices.
        predictions (numpy.ndarray): Predicted label indices, same length.
        class_count (int): Number of labels.

    Returns:
        numpy.ndarray: Matrix of shape (class_count, class_count), rows are true
            labels and columns are predicted labels.
    """
    matrix = np.zeros((class_count, class_count), dtype=np.int64)
    np.add.at(matrix, (targets, predictions), 1)
    return matrix


def print_confusion_matrix(matrix, labels):
    """Print a confusion matrix with label names.

    Args:
        matrix (numpy.ndarray): Output of confusion_matrix().
        labels (list): Label names in index order.

    Returns:
        None
    """
    width = max(len(label) for label in labels) + 2
    print(" " * width + "".join(label.rjust(width) for label in labels) + "   <- predicted")
    for label, row in zip(labels, matrix):
        print(label.ljust(width) + "".join(str(count).rjust(width) for count in row))


def export_tflite(model, labels, out_dir):
    """Write the model as TensorFlow Lite and the label order next to it.

    Args:
        model (keras.Model): Trained model.
        labels (list): Label names in output index order.
        out_dir (Path): Directory to write into, created if missing.

    Returns:
        Path: Path of the written .tflite file.
    """
    import tensorflow as tf

    out_dir.mkdir(parents=True, exist_ok=True)
    model_path = out_dir / "gesture_model.tflite"
    model_path.write_bytes(tf.lite.TFLiteConverter.from_keras_model(model).convert())
    (out_dir / "labels.txt").write_text("\n".join(labels) + "\n", encoding="utf-8")
    return model_path


def parse_arguments(argv):
    """Parse the command line.

    Args:
        argv (list): Arguments without the program name.

    Returns:
        argparse.Namespace: Parsed options.
    """
    parser = argparse.ArgumentParser(description="Train the accelerometer gesture classifier.")
    parser.add_argument("--data", type=Path, default=Path("motion-folder"), help="directory with the CSV recordings")
    parser.add_argument("--out", type=Path, default=Path("ml/models"), help="directory for the exported model")
    parser.add_argument("--window", type=int, default=SAMPLE_RATE_HZ, help="samples per window (100 = 1 s)")
    parser.add_argument("--stride", type=int, default=20, help="samples between consecutive windows")
    parser.add_argument("--epochs", type=int, default=300, help="training epochs")
    return parser.parse_args(argv)


def main(argv):
    """Train and export the model.

    Args:
        argv (list): Command line arguments without the program name.

    Returns:
        int: Process exit code, 0 on success.
    """
    options = parse_arguments(argv)

    print(f"Loading recordings from {options.data}")
    try:
        train_x, train_y, validation_x, validation_y, labels = build_dataset(
            options.data, options.window, options.stride)
    except ValueError as error:
        print(f"error: {error}", file=sys.stderr)
        return 1

    print(f"Labels: {', '.join(labels)}")
    print(f"Windows: {len(train_x)} training, {len(validation_x)} validation")

    import tensorflow as tf

    tf.keras.utils.set_random_seed(RANDOM_SEED)
    model = build_model(options.window, len(labels))
    model.summary()
    model.fit(train_x, train_y, validation_data=(validation_x, validation_y),
              epochs=options.epochs, batch_size=32, verbose=2)

    predictions = np.argmax(model.predict(validation_x, verbose=0), axis=1)
    print(f"\nValidation accuracy: {np.mean(predictions == validation_y):.3f}")
    print_confusion_matrix(confusion_matrix(validation_y, predictions, len(labels)), labels)

    model_path = export_tflite(model, labels, options.out)
    print(f"\nSaved {model_path} ({model_path.stat().st_size} bytes) and labels.txt")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
