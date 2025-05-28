# Kitehcoin Snap Packaging

Commands for building and uploading a Kitehcoin Core Snap to the Snap Store. Anyone on amd64 (x86_64), arm64 (aarch64), or i386 (i686) should be able to build it themselves with these instructions. This would pull the official Kitehcoin binaries from the releases page, verify them, and install them on a user's machine.

## Building Locally
```
sudo apt install snapd
sudo snap install --classic snapcraft
sudo snapcraft
```

### Installing Locally
```
snap install \*.snap --devmode
```

### To Upload to the Snap Store
```
snapcraft login
snapcraft register kitehcoin-core
snapcraft upload \*.snap
sudo snap install kitehcoin-core
```

### Usage
```
kitehcoin-unofficial.cli # for kitehcoin-cli
kitehcoin-unofficial.d # for kitehcoind
kitehcoin-unofficial.qt # for kitehcoin-qt
kitehcoin-unofficial.test # for test_kitehcoin
kitehcoin-unofficial.tx # for kitehcoin-tx
```

### Uninstalling
```
sudo snap remove kitehcoin-unofficial
```