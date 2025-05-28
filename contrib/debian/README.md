
Debian
====================
This directory contains files used to package kitehcoind/kitehcoin-qt
for Debian-based Linux systems. If you compile kitehcoind/kitehcoin-qt yourself, there are some useful files here.

## kitehcoin: URI support ##


kitehcoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install kitehcoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your kitehcoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/kitehcoin128.png` to `/usr/share/pixmaps`

kitehcoin-qt.protocol (KDE)

