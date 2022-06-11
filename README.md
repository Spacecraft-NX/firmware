# Spacecraft-NX

## What is this?
A fully opensource SX Core / Lite compatible firmware for chainloading into your own payloads.


### Usage
* Place a compatible payload, i.e. [hekate](https://github.com/CTCaer/hekate/releases), in your sdcard root folder under name `payload.bin`.
* After initial training (up to 5 minutes), modchip is ready and should boot quickly.
* Holding VOL+ during boot prevents the modchip from going to sleep. This subsequently allows interactions with [hwfly-toolbox](https://github.com/hwfly-nx/hwfly-toolbox) for additional modchip operations and diagnostics.
* When glitching is successful but an sdcard issue prevent loading of payload.bin, diagnostics are provided on the screen. From this screen, OFW can be booted by pressing VOL+ & VOL- simultaneously.
* Holding VOL+ and VOL- during power-on bypasses any payloads and boots into OFW. Buttons must be held until glitching completes.


### LED Diagnostics
After installation, the firmware will train itself. This can be recognized by the yellow pulsing LED pattern and can take up to 5 minutes.
When completed, subsequent boots will be much faster. Refer to the diagram below for a full overview of possible LED patterns, which may aid in diagnosis in case of non-working installations.

[![LED patterns](https://i.imgur.com/nHYnBfu.gif)


### Debug console
A debug console is available when the device is powered on by inserting a USB cable into your computer. Note that this will not work if the firmware is already running when you insert power, in that case the console must be turned off completely, and the USB cable re-inserted.
The device will enumerate as a USB CDC device and listed as a Serial COM port in your device manager. You can then use a tty program such as PuTTY on Windows to open a connection to this COM port. Commands available in this debug console can be retrieved by pressing 'h' for help.


### Updating
Updating can be done using one of 3 methods:

**Method a) - Using USB bootloader (preferred)**
 1. Fully power off the console.
 2. Insert USB cable. Take extreme caution not to insert incorrectly, this fully kills USB functionality on the chip.
 3. Run flash.bat. This updates both bootloader and firmware.
 4. Remove USB cable.
 5. Power on console.

**Method b) - Using hwfly-toolbox 
This method does not require opening device, but it cannot flash the bootloader.
LED patterns in bootloader will not correspond with documented ones.

 1. Obtain hwfly toolbox from https://github.com/hwfly-nx/hwfly-toolbox/releases
 2. Place hwfly_toolbox.bin in sdcard:/bootloader/payloads/.
 3. Boot switch into hekate, then run hwfly_toolbox.bin payload.
 4. Update SD loader from toolbox menu.
 5. Power off console from menu.
 7. Place firmware.bin and sdloader.enc from hwfly firmware into sdcard root. Do not do so earlier.
    If you have previously flashed a beta firmware, also create empty file .force_update in sdcard root.
 8. Put SD in switch and power on while holding VOL+ (verify: modchip green light keep pulsing while hekate loads).
 7. Run hwfly_toolbox from hekate again.
 9. Update firmware from menu.
 10. Reboot & done.

**Method c) - External programmer (j-link, st-link, gd-link)**
See https://www.sthetix.info/flashing-updating-unbricking-the-chip/ for details.
Supplied bootloader.bin should be flashed at 0x08000000.
Supplied payload.bin should be flashed at 0x08003000.
 
Note: neither method will allow unflashable chips to become flashed. Unflashable ones have a BGA FPGA IC on the board. Flashable ones use a QFN FPGA.
