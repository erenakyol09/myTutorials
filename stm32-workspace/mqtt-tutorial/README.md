# mqtt-tutorial

MQTT over Ethernet on a Nucleo-F767ZI board. The firmware brings up the Ethernet link with a static
IP, connects to an MQTT broker running on the development machine, subscribes to a command topic and
publishes a periodic status message. All diagnostics are logged over a USB CDC virtual COM port.

## Stack

| Layer | Choice |
| --- | --- |
| RTOS | FreeRTOS (CMSIS-RTOS v2 API) |
| TCP/IP | lwIP with `NO_SYS=1` |
| MQTT client | lwIP built-in MQTT client (`Middlewares/Third_Party/LwIP/src/apps/mqtt/mqtt.c`) |
| PHY | LAN8742A over RMII |
| Logging | USB Device CDC (virtual COM port) on USB OTG_FS |

Because lwIP runs with `NO_SYS=1`, there is no lwIP thread and no socket API. A single FreeRTOS task
(`defaultTask` in `Core/Src/freertos.c`) polls the Ethernet input, drives the lwIP timers and runs the
MQTT client. **Every lwIP and MQTT call must happen from that task** — there is no core locking.

## Hardware setup

| Connection | Purpose |
| --- | --- |
| CN1 (ST-LINK USB) | Power, flashing and debugging |
| CN13 (USB OTG_FS) | USB CDC virtual COM port used for logging |
| RJ45 | Direct Ethernet cable to the development machine (no router) |

Both USB cables can stay connected at the same time.

## Network layout

| Node | Address |
| --- | --- |
| STM32 | `192.168.10.2/24` |
| Development machine | `192.168.10.1/24` |
| Gateway | none (direct cable) |
| Broker | `192.168.10.1:1883` |

The board's address is configured in STM32CubeMX under LWIP (DHCP disabled) and ends up in
`LWIP/App/lwip.c`. The broker address is defined in `App/Services/MqttService.c`.

## MQTT topics

| Topic | Direction | Payload |
| --- | --- | --- |
| `stm32/status` | board publishes every 5 s | `uptime=<seconds>s count=<n>` |
| `stm32/cmd` | board subscribes | any text, logged over USB CDC |

## Prerequisites

- STM32CubeMX (for `.ioc` edits and code regeneration)
- ARM GCC toolchain and CMake (Ninja recommended)
- ST-Link / OpenOCD or another flashing tool
- Mosquitto, for the broker and the `mosquitto_pub` / `mosquitto_sub` CLI tools

## Build

```bash
cmake --preset=default -B build/Debug
cmake --build build/Debug
```

Build artifacts are placed in `build/Debug/`. After cloning or moving the project, delete `build/`
first so the absolute paths in the CMake cache are regenerated.

## Flash

Flash the resulting ELF/BIN with ST-Link, OpenOCD or the STM32 VS Code extension. The repository does
not ship a flashing script.

## Regenerate code after `.ioc` changes

Edit `mqtt-tutorial.ioc` in STM32CubeMX and regenerate. Keep custom code inside
`/* USER CODE BEGIN */` ... `/* USER CODE END */` blocks; everything outside them is overwritten.

Two known regeneration pitfalls in this project:

- CubeMX resets the LWIP static IP to its own default (`192.168.0.2`). Re-check the LWIP IPv4 address
  field after regenerating.
- The `MX_USB_DEVICE_Init()` / `MX_LWIP_Init()` call order in `StartDefaultTask` lives outside the
  USER CODE blocks. USB init is deliberately placed first so the CDC log still comes up if Ethernet
  initialization fails; CubeMX may reorder it.

## Project layout

| Path | Contents |
| --- | --- |
| `App/Services/MqttService.c/.h` | MQTT client: connect, retry, subscribe, publish |
| `App/Application`, `App/Components`, `App/Interfaces` | C++ application layer (LED abstraction) |
| `Core/Src/freertos.c` | `defaultTask`: drives lwIP and the MQTT service |
| `Core/Src/main.c` | Entry point, clock and peripheral init |
| `LWIP/App/lwip.c` | lwIP init, static IP, link status callback (logs over CDC) |
| `LWIP/Target/ethernetif.c` | ETH HAL/MSP init, LAN8742 PHY glue |
| `USB_DEVICE/` | USB CDC virtual COM port |
| `Middlewares/Third_Party/` | FreeRTOS and lwIP sources |
| `Drivers/` | CMSIS, STM32 HAL, LAN8742 PHY driver |
| `cmake/stm32cubemx/` | CubeMX-generated CMake integration (regenerated) |
| `CMakeLists.txt` | User-owned source list; add new application files here |

## Test procedure (macOS)

### 1. Find and open the USB CDC port

```bash
ls /dev/cu.* /dev/tty.*
```

A `/dev/cu.usbmodemXXXX` entry appears when CN13 is connected. Use `cu.*`, not `tty.*`.

To confirm which device is which:

```bash
system_profiler SPUSBDataType | grep -B 2 -A 10 -i "STM\|Virtual\|usbmodem"
```

Two STM32 devices should be listed: **STM32 STLink** (CN1, unrelated to this firmware) and a
virtual COM port device (CN13, the CDC code). Open the CDC one:

```bash
screen /dev/cu.usbmodemXXXX 115200
```

To exit `screen`: `Ctrl-A`, then `K`, then `y`.

### 2. Give the development machine a static IP

```bash
networksetup -listallhardwareports
sudo networksetup -setmanual "USB 10/100 LAN" 192.168.10.1 255.255.255.0
```

Replace `USB 10/100 LAN` with the adapter name from the first command. GUI alternative:
System Settings → Network → select the adapter → Details → TCP/IP → Configure IPv4: Manually →
IP `192.168.10.1`, Subnet Mask `255.255.255.0`, Router empty.

### 3. Verify the Ethernet link

With `screen` open, plug in the Ethernet cable (or reset the board while it is plugged in):

```text
Link UP, IP: 192.168.10.2
```

Unplugging prints `Link DOWN`. Then verify IP connectivity:

```bash
ping 192.168.10.2
```

### 4. Set up the broker

```bash
brew install mosquitto
```

Append to `/opt/homebrew/etc/mosquitto/mosquitto.conf`:

```conf
listener 1883 0.0.0.0
allow_anonymous true
```

- `0.0.0.0` listens on all interfaces (loopback and the USB Ethernet adapter), so both `localhost`
  and `192.168.10.1` work.
- `allow_anonymous true` is required because the board connects without credentials; Mosquitto 2.x
  denies anonymous connections by default.

Start it and confirm it is listening:

```bash
brew services start mosquitto
lsof -i :1883
```

The output should contain `TCP *:ibm-mqisdp (LISTEN)` (`ibm-mqisdp` is the name of port 1883 in
`/etc/services`).

### 5. Test the broker by itself

Two-terminal method:

```bash
# Tab 1 - blocks, waits for messages
mosquitto_sub -h 192.168.10.1 -t test/topic -v
```

```bash
# Tab 2
mosquitto_pub -h 192.168.10.1 -t test/topic -m "hello"
```

Single-terminal method using a retained message, which also works with the Ethernet cable unplugged
when `-h localhost` is used:

```bash
mosquitto_pub -h localhost -t test/topic -m "hello" -r
mosquitto_sub -h localhost -t test/topic -v -C 1 -W 3
mosquitto_pub -h localhost -t test/topic -n -r
```

- `-r` (retain): the broker stores the last message on the topic and delivers it immediately to any
  client that subscribes later, so publisher and subscriber do not have to run concurrently.
- `-C 1`: exit after one message. `-W 3`: exit after a 3 second timeout.
- `-n -r`: an empty retained message clears the retained message (cleanup after testing).

### 6. Test the MQTT client end to end

Flash the firmware and watch the CDC terminal. Expected sequence:

```text
Link UP, IP: 192.168.10.2
MQTT connected to 192.168.10.1
MQTT subscribed to stm32/cmd
```

Watch the board's status messages (one every 5 seconds):

```bash
mosquitto_sub -h 192.168.10.1 -t stm32/status -v
```

```text
stm32/status uptime=25s count=3
```

Send a command to the board:

```bash
mosquitto_pub -h 192.168.10.1 -t stm32/cmd -m "hello"
```

The CDC terminal prints:

```text
MQTT rx [stm32/cmd] hello
```

## Troubleshooting

### `mosquitto_sub` hangs with no output

```bash
lsof -i :1883
ifconfig en7 | grep -E "status|inet "
netstat -rn -f inet | grep 192.168.10
```

A line such as `TCP 192.168.1.103:49946->192.168.10.1:ibm-mqisdp (SYN_SENT)` means the connection is
not being established: the source address belongs to the Wi-Fi interface, so macOS does not consider
`192.168.10.1` a local address. The usual cause is an unplugged Ethernet cable — the adapter goes
`status: inactive`, macOS drops the manual IP and the `192.168.10.0/24` route, and the traffic falls
through to the default route.

Fix: plug the cable in and confirm `ifconfig en7` shows `status: active` and `inet 192.168.10.1`.
Re-apply the static IP if it does not.

### No USB CDC device appears

Check that the cable is in CN13 (USB OTG_FS), not only CN1 (ST-LINK), and that the current firmware
is flashed. If the log stays silent while the device does enumerate, the task is most likely stuck
before its loop — `MX_USB_DEVICE_Init()` runs before `MX_LWIP_Init()` specifically so that CDC output
survives an Ethernet initialization failure.

### Verifying the USB CDC path alone

`Core/Src/freertos.c` contains a heartbeat that prints an incrementing counter once per second,
independent of Ethernet. Enable it by setting:

```c
#define USB_CDC_HEARTBEAT_ENABLED 1
```

## Coding guidelines

See `RULES.md` for the full standard. Summary:

- English-only comments, ASCII characters only
- 4-space indentation, camelCase for functions and variables, UPPER_CASE for macros
- Header guards in all headers, `static` for file-local symbols
- Doxygen-style comments for public functions
- Custom code only inside `USER CODE` blocks in CubeMX-generated files
- Use HAL APIs rather than direct register access

## License

See the LICENSE file in the repository root, if present. Otherwise the code is provided AS-IS.
