# Dinosaur Run CE

A clone of Google Chrome's dinosaur game (from the no internet screen) for the TI-84 Plus CE graphing calculator.

### Installation:
Send DINO.8xp to the calculator using TI-Connect CE (for Windows and Mac) or TiLP (for Linux).
Also send [the C libraries](https://github.com/CE-Programming/libraries/releases) to the calculator.
To run, select DINO from the prgm menu and press enter.

On older calculator OSes, you may need to open the catalog and select the Asm( token before running the program, so that the text `Asm(prgmDINO` is displayed.
To avoid having to do this, either update your OS or use a shell such as [Cesium](https://github.com/mateoconlechuga/cesium/releases). 

### Controls:
UP to jump, DOWN to duck / fall, enter/2nd to restart, clear to exit.

### Building from source:
The most up-to-date version of the source code may be found [on GitHub](https://github.com/commandblockguy/dino-run-ce).
To build, you will need to install the [CE toolchain](https://github.com/CE-Programming/toolchain/releases).
Run `make` in this directory to produce a binary in `bin/`.

###USB:
Please note that USB support is still experimental and not recommended.
To build with USB support, use `make usb` or set USE_USB to 1 in `config.h`.
You will also need to send the [USB libraries](https://github.com/CE-Programming/toolchain/tree/usbdrvce).
A (probably outdated) prebuilt version can be found [here](http://commandblockguy.xyz/downloads/usblibs.8xg).
You will also need a USB Mini B OTG adapter to use most devices.
Dino Run is compatible with HID boot devices (up/left click to jump, down/right click to jump) and the Steam controller (for rumble sounds).

### Contact:
Questions / comments?

* [Cemetech topic](https://www.cemetech.net/forum/viewtopic.php?p=283380)
* Discord: [https://discord.gg/DZbmraw](https://discord.gg/DZbmraw) (commandblockguy#8263)
* IRC: #cemetech on EFnet
* Email: [commandblockguy1@gmail.com](mailto:commandblockguy1@gmail.com)
* [GitHub issues](https://github.com/commandblockguy/dino-run-ce/issues)