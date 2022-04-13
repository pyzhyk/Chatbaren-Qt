# [Chatbaren-Qt](https://github.com/pyzhyk/Chatbaren-Qt)

[![License](https://img.shields.io/badge/license-GPL-yellow.svg)][license]
![Platform](https://img.shields.io/badge/platform-Linux-blue.svg)
[![Version](https://img.shields.io/badge/version-Qt-brightgreen.svg)][qt-version]

[license]: https://www.gnu.org/licenses/gpl.html
[qt-version]: https://github.com/pyzhyk/Chatbaren-Qt

## Secure, open-source, peer-to-peer, end-to-end encrypted messaging system with Qt for Linux.

## Getting Started

### Dependencies

- [Openssl](https://www.openssl.org/)
- [Tor](https://torproject.org/)
- [Ncat](https://nmap.org/ncat/)
- Shred

#### To install them, run:
```bash
apt install openssl tor ncat -y; apt install shred -y
```

### Setup

- Clone the repository:
```bash
git clone https://github.com/pyzhyk/Chatbaren-Qt
```
- Change directory:
```bash
cd Chatbaren-Qt
```
- Create a symlink to Chatbaren script:
```bash
ln -s /path/to/chatbaren-qt/chatbaren /usr/bin/chatbaren
```
- Create a symlink to Chatbaren-Qt program:
```bash
ln -s /path/to/chatbaren-qt/chatbaren-qt /usr/bin/chatbaren-qt
```
- Set Chatbaren-Qt folder location: edit `chatbaren` and set `chatbaren_prefix` to `/path/to/Chatbaren-Qt`
- Generate your keypair: 
```bash
chatbaren -g
```
- Start Tor hidden service:
```bash
chatbaren -o
```
- Run Chatbaren-Qt: 
```bash
chatbaren-qt
```
- Show your public key: `Key -> Show` or `Ctrl-K`
- Show your hostname: `Tor -> Address` or `Ctrl-A`
- To add your friend's public key: `Key -> Add` or `Ctrl-N`

It may take some time to start [Tor hidden service](https://www.torproject.org/docs/onion-services) for a first time.

[![Qt Screenshot](https://github.com/pyzhyk/Chatbaren-Qt/blob/master/Images/Screenshot-1.png)](https://github.com/pyzhyk/Chatbaren-Qt/blob/master/Images/Screenshot-1.png)

Happy chatting!


## Console Options

### Chat options
- `/NULL` - Hide your username.
- `/FULL` - Show your username.
- `/SUMS` - Display hashsums of your and recipient's public keys.

### Options:
- `-a` - Chat view mode + Server + Tor hidden service in background.
- `-c` address.onion - Type mode to recipient.
- `-o` - Start Tor hidden service.
- `-s` - Xterm chat view window + Server + Tor hidden service.
- `-g` - Wipe old and generate new keypair.
- `-n` - Wipe old and generate new tor address.
- `-k` - Show your public key.
- `-t` - Show your tor address.

### Developer options:
##### (Used for Qt Front-end)
- `-m address.onion msg username` - Send one message to recipient. Localhost sending disabled.
- `-d sums address.onion` - Get address.onion key hashes.
- `-d sums self` - Get your key hashes.
- `-d address` - Get your address.
- `-d prefix` - Get chatbaren_prefix.
- `-d pubkey` - Get your public key.
- `-d username` - Get your username.
- `-d serve` - Start server silently.

### Files and folders:
- `username.txt` - Your username that will be shown in chat window.
- `keys/` - Folder with your recipient's public keys and your keypair.
- `keys/self/self.pub` - Your public key. You may share it clearly to recipients.
- `keys/self/self.key` - Your private key. **DO NOT SHARE IT!**
- `tor/chatbaren` - Tor service configuration file.
- `tor/service/hostname` - Your Tor hidden service hostname. Your friend can chat with you only with this name.
- `tor/service/private_key` - Your Tor private key. **DO NOT SHARE IT!**

### Log files
- `tor/notices.log` - Tor service notices log.
- `tor/debug.log` - Tor service debug log. It is turned off by default.
