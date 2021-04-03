# Raspberry Pico Test

## Usage

Copy a `.uf2` file from the `releases` folder to your Raspberry Pi Pico to get running.

To send commands, install `minicom` (`brew`/`apt`/`AUR`) and run `$ sudo minicom -b 115200 -o -D /dev/ttyACM0`.

### Available Commands

| Command       | Description         |
|---------------|---------------------|
| `set <float>` | Set the target RPM. |

Minicom does not display what you are typing as you type it, so just type the full command and press enter.

### Indicators

The Pico's onboard LED will flash when processing a command and stay solid when ready for the next one.

## CLI Toolchain Installation

### MacOS

Make sure to clone the [pico-sdk](https://github.com/raspberrypi/pico-sdk) into the parent folder of this one.

Then, install things until building works. Probably need atleast
```
brew install --cask gcc-arm-embedded
brew install cmake
```

[gcc-arm-embedded trick source](https://gist.github.com/joegoggins/7763637).

### Arch

Install `cmake`, and `gcc-arm-none-eabi-bin` from the AUR.

Or `raspberry-pico-sdk-git` apparently.

## Build

Just `make`.

## Debug pico
Unplug, hold BOOTSEL, replug. Then, `sudo picotool info -a`.

## Links
- [api documentation](https://raspberrypi.github.io/pico-sdk-doxygen/index.html)
- [pinout summary](https://microcontrollerslab.com/raspberry-pi-pico-pinout-features-programming-peripherals/)
- [detailed engineering analysis of the SDK](https://www.stereorocker.co.uk/2021/02/14/raspberry-pi-pico-displays-fonts-portability/)
- [tools for reading printf output via usb](https://www.raspberrypi.org/forums/viewtopic.php?t=302227)

<details> <summary>Pastebin</summary>

pls mount /dev/sdb1 ~/vol/rpi && pls cp build/my_proj.uf2 ~/vol/rpi && pls umount ~/vol/rpi

</details>
