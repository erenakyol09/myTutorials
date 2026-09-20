# macOS Test Commands

Terminal komutları: board'u USB CDC üzerinden izlemek ve Ethernet bağlantısını doğrulamak için.

## 1. Seri portları listele

```bash
ls /dev/cu.* /dev/tty.*
```

Board USB OTG_FS (CN13) portuna bağlıyken burada `/dev/cu.usbmodemXXXX` görünür.
`cu.*` kullan, `tty.*` değil (interaktif oturumlar için doğru olan).

## 2. USB cihazını doğrula

```bash
system_profiler SPUSBDataType | grep -B 2 -A 10 -i "STM\|Virtual\|usbmodem"
```

İki ayrı STM32 USB cihazı görmelisin:
- **STM32 STLink** — CN1, ST-LINK debugger/VCP (USART3'e bağlı, bizim kodumuzla alakası yok)
- **Virtual COM Port** benzeri ikinci cihaz — CN13, bizim USB Device CDC kodumuz

## 3. USB CDC terminalini aç

```bash
screen /dev/cu.usbmodemXXXX 115200
```

Çıkmak için: `Ctrl-A` sonra `K`, ardından `y`.

## 4. Mac'in Ethernet arayüzünü bul

```bash
networksetup -listallhardwareports
```

USB-Ethernet adaptörünün adını bulmak için (bu projede: "USB 10/100 LAN").

## 5. Mac'e statik IP ver

```bash
sudo networksetup -setmanual "USB 10/100 LAN" 192.168.10.1 255.255.255.0
```

Board ile doğrudan kablo bağlantısı olduğu için gateway gerekmiyor.

Alternatif (GUI): System Settings → Network → adaptörü seç → Details → TCP/IP →
Configure IPv4: Manually → IP `192.168.10.1`, Subnet Mask `255.255.255.0`, Router boş.

## 6. Ethernet bağlantısını doğrula

```bash
ping 192.168.10.2
```

Board'un statik IP'si `192.168.10.2/24`. Cevap alırsan lwIP + ETH stack'i çalışıyor demektir.

## Beklenen USB CDC çıktısı

`screen` açıkken Ethernet kablosunu takınca (veya zaten takılıyken board'u reset'lerken):

```
Link UP, IP: 192.168.10.2
```

Kabloyu çekince:

```
Link DOWN
```
