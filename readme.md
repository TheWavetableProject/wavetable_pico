# Raspberry Pico Test

## Usage

### Setup

To enter BOOTSEL mode, press and hold the `BOOTSEL` button on the front of the pico board. Then, plug in the USB, and release the `BOOTSEL` button. 

Then, use either `picotool` or the manual method to install the UF2.

#### Picotool

Install `picotool` using `apt` or from the AUR. (At time of writing, `brew` does not appear to have `picotool`) 

After plugging in the Raspberry Pico in BOOTSEL mode, load the binary:
```sh 
sudo picotool load -x path/to/wavetable_main.uf2
```

#### Manual 

If on Linux, manually mount the Rasperry Pico as a drive.

Copy a `.uf2` file from the `releases` folder to your Raspberry Pi Pico to get running.

### Interaction

To send commands, install `minicom` (`brew`/`apt`/`AUR`).

Then, if using, Linux, run `sudo minicom -b 115200 -o -D /dev/ttyACM0`.

Or, if using MacOS, run `sudo minicom -b 115200 -o -D /dev/tty.usbmodem0000000000001`.

Or, if on Windows, [try using PuTTY](https://stackoverflow.com/questions/66223686/raspberry-pi-pico-usb-debbuging-on-windows).

Commands can also be sent pragmatically using any serial communication program, such as the `pyserial` library.

### Available Commands

| Command        | Description                                 |
|----------------|---------------------------------------------|
| `set <float>`  | Ramp up/down speed to the target RPM.       |
| `fset <float>` | Force set the speeed without interpolation. |
| `info`         | Print software configuration information.   |

Minicom does not display what you are typing as you type it, so just type the full command and press enter.

### Indicators

The Pico's onboard LED will flash when processing a command and stay solid when ready for the next one.

## CLI Toolchain Installation

### MacOS

Then, install things until building works. You probably want
```
brew install --cask gcc-arm-embedded
brew install cmake
```

[gcc-arm-embedded trick source](https://gist.github.com/joegoggins/7763637).

### Arch

Install `cmake`, and `gcc-arm-none-eabi-bin` from the AUR.

Or `raspberry-pico-sdk-git` apparently.

## Build

Clone recursively, or make sure to `git submodule update --init` in both this folder and in `pico-sdk`.

Then, just `make`, then load the `.uf2` from the `build` directory using one of the above Setup methods.

## Debug pico
Unplug, hold BOOTSEL, replug, release BOOTSEL. Then, `sudo picotool info -a`.

## Links
- [api documentation](https://raspberrypi.github.io/pico-sdk-doxygen/index.html)
- [pinout summary](https://microcontrollerslab.com/raspberry-pi-pico-pinout-features-programming-peripherals/)
- [detailed engineering analysis of the SDK](https://www.stereorocker.co.uk/2021/02/14/raspberry-pi-pico-displays-fonts-portability/)
- [tools for reading printf output via usb](https://www.raspberrypi.org/forums/viewtopic.php?t=302227)

