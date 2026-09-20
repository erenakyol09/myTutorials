# macOS Test Commands

Terminal commands for monitoring the board over USB CDC and verifying the Ethernet connection.

## 1. List serial ports

```bash
ls /dev/cu.* /dev/tty.*
```

When the board is connected to the USB OTG_FS port (CN13), a `/dev/cu.usbmodemXXXX` entry appears here.
Use `cu.*`, not `tty.*` (`cu.*` is the correct one for interactive sessions).

## 2. Verify the USB device

```bash
system_profiler SPUSBDataType | grep -B 2 -A 10 -i "STM\|Virtual\|usbmodem"
```

You should see two separate STM32 USB devices:

- **STM32 STLink** — CN1, the ST-LINK debugger/VCP (wired to USART3, unrelated to our code)
- A second device similar to **Virtual COM Port** — CN13, our USB Device CDC code

## 3. Open the USB CDC terminal

```bash
screen /dev/cu.usbmodemXXXX 115200
```

To exit: `Ctrl-A`, then `K`, then `y`.

## 4. Find the Mac's Ethernet interface

```bash
networksetup -listallhardwareports
```

Used to find the name of the USB-Ethernet adapter (in this project: "USB 10/100 LAN").

## 5. Assign a static IP to the Mac

```bash
sudo networksetup -setmanual "USB 10/100 LAN" 192.168.10.1 255.255.255.0
```

No gateway is needed because the board is connected with a direct cable.

Alternative (GUI): System Settings → Network → select the adapter → Details → TCP/IP →
Configure IPv4: Manually → IP `192.168.10.1`, Subnet Mask `255.255.255.0`, Router empty.

## 6. Verify the Ethernet connection

```bash
ping 192.168.10.2
```

The board's static IP is `192.168.10.2/24`. If it replies, the lwIP + ETH stack is working.

## 7. Install an MQTT broker (Mosquitto)

```bash
brew install mosquitto
```

Because this is a direct cable connection (no router/internet), the broker has to run on the Mac
and be reachable at `192.168.10.1` — a public broker is not reachable from the board.

## 8. Configure the broker

Append to `/opt/homebrew/etc/mosquitto/mosquitto.conf`:

```
listener 1883 0.0.0.0
allow_anonymous true
```

- `0.0.0.0`: listens on all interfaces (loopback + en7), so both `localhost` and `192.168.10.1` work.
- `allow_anonymous true`: Mosquitto 2.x defaults to `false`; required because the STM32 connects without credentials.

## 9. Start and verify the broker

```bash
brew services start mosquitto
brew services list | grep mosquitto
lsof -i :1883
```

The `lsof` output should contain a line like this (`ibm-mqisdp` is the name of port 1883 in `/etc/services`):

```
mosquitto  5805  eren.akyol  5u  IPv4  ...  TCP *:ibm-mqisdp (LISTEN)
```

## 10. Test the broker

**Two-terminal method** (regular concurrent pub/sub):

```bash
# Tab 1 — blocks, waits for messages
mosquitto_sub -h 192.168.10.1 -t test/topic -v
```

```bash
# Tab 2
mosquitto_pub -h 192.168.10.1 -t test/topic -m "hello"
```

**Single-terminal method** (retained message, independent of Ethernet):

```bash
mosquitto_pub -h localhost -t test/topic -m "hello" -r
mosquitto_sub -h localhost -t test/topic -v -C 1 -W 3
mosquitto_pub -h localhost -t test/topic -n -r
```

- `-r` (retain): the broker stores the last message on that topic; a client that subscribes later
  receives it immediately. This removes the need for pub and sub to run concurrently.
- `-C 1`: exit after receiving 1 message (otherwise it blocks forever).
- `-W 3`: exit with a timeout if no message arrives within 3 seconds.
- `-n -r`: a retained message with an empty payload clears the retained message on that topic (cleanup after testing).

Using `localhost` goes over loopback, so it works even with the Ethernet cable unplugged — this is
how to test the broker itself in isolation from network problems.

## Troubleshooting

**`mosquitto_sub` hangs without any output / cannot connect:**

```bash
lsof -i :1883
ifconfig en7 | grep -E "status|inet "
netstat -rn -f inet | grep 192.168.10
```

If the `lsof` output contains a line like the one below, the connection is not being established:

```
TCP 192.168.1.103:49946->192.168.10.1:ibm-mqisdp (SYN_SENT)
```

A source IP on the Wi-Fi interface (`192.168.1.x`) means macOS does not recognize `192.168.10.1` as a
local address. The usual cause: the Ethernet cable is unplugged → `en7` is `status: inactive` →
macOS does not apply the static IP → there is no route for `192.168.10.0/24` → traffic falls through
to the default route (Wi-Fi) and hangs in `SYN_SENT`.

Fix: plug the cable in and confirm that `ifconfig en7` shows `status: active` and `inet 192.168.10.1`.
If it does not, re-apply the static IP configuration (step 5).

## Expected USB CDC output

With `screen` open, when the Ethernet cable is plugged in (or the board is reset while it is already plugged in):

```
Link UP, IP: 192.168.10.2
```

When the cable is unplugged:

```
Link DOWN
```
