# Spacecraft-NX

## What is this?
A fully opensource SX Core / Lite compatible firmware that lets you play a Space Invaders inspired mini-game and load any payload you like.

## Does it allow me to do piracy?
No, it doesn't. You can't even boot into Horizon OS using Spacecraft-NX by itself.

## How can i boot Horizon OS then?
To do that you need an firmware like [Atmosphère](https://github.com/Atmosphere-NX) and a bootloader like [hekate](https://github.com/CTCaer/hekate) or fusée.

## Can i run [insert payload name here]?
If the payload is updated to support patched erista or/and mariko chips: yes

If the payload is only RCM compatible: you need to ask the developer for update.

## I love this! How can i install it?
For now you can't flash Spacecraft-NX without pluging in your modchip into your pc.
* Plug in your USB dongle into the modchip.
* Plug in your micro-usb cable into the dongle.
* Plug in the other end of the cable to your Windows PC. (Linux support is work in progress. Feel free to send in a PR for OS X)
* Download the [latest release](https://github.com/Spacecraft-NX/firmware/releases).
* Unpack the archive
* Double click the flash.bat
* Wait till it says `!!!DONE!!!`
* Copy the payload into the root of the sdcard and rename it to `payload.bin`
* On the first boot up your chip will enter into a training session, blinking yellow. This may take up to 30 minutes. (Recommended to plug in a charger)
* After the initial training the chip will blink magenta and turn green upon successful boot up.

## HELP! I got `Spacecraft-NX DFU not found!` message. 
Don't worry. Windows still installing the drivers.

Calm down and try again.
