# Wavetable Pico

## TODO
- [ ] fix the rpm math (off by ~2x rn)
- [ ] add section about modifying code for custom stepper degrees and bearing sizes
- [x] add note about minicom not showing text as you type it
- [ ] test stderr and outputting a log of speed over time for later analysis
- [ ] test the pyserial module
- [ ] enable pressing a button to begin the next step
- [ ] arduino toolchain

## Usage

### Setup

See the [setup guide](./docs/installing.md).

### Interaction

If using MacOS, see the [interaction guide for MacOS](./docs/macos_with_screen.md).

To send commands, install `minicom` ([`brew` (MacOS)](https://brew.sh/)/`apt`/AUR).
Then, if using, Linux, run `sudo minicom -b 115200 -o -D /dev/ttyACM0`.

Or, if on Windows, see the [interaction guide for Windows](./docs/windows_with_PuTTY.md).

Commands can also be sent pragmatically using any serial communication program, such as the `pyserial` library.

#### Available Commands

| Command                | Description                                                |
|------------------------|------------------------------------------------------------|
| `set <float>`  | Ramp up/down speed to the target RPM.                              |
| `fset <float>` | Force set the speeed without interpolation.                        |
| `lset <x> <y>` | Set speed to `x` RPM with linear interpolation at `y` RPM per min. |
| `sina <float>` | Set the amplitude of the sine wave (0 to disable).                 |
| `sinf <float>` | Set the frequency of the sine wave, in Hz.                         |
| `info`         | Print software configuration information.                          |

Minicom does not display what you are typing as you type it, so just type the full command and press enter.

## Build

<details><summary>Steps required to customize and re-build the code</summary>

### CLI Toolchain Installation

#### MacOS

Install things until building works. You probably want
```
brew install --cask gcc-arm-embedded
brew install cmake
```

[gcc-arm-embedded trick source](https://gist.github.com/joegoggins/7763637).

#### Arch

Install `cmake`, and `gcc-arm-none-eabi-bin` from the AUR.

Or `raspberry-pico-sdk-git` apparently.


Clone recursively, or make sure to `git submodule update --init` in both this folder and in `pico-sdk`.

Then, just `make`, then load the `.uf2` from the `build` directory using one of the above Setup methods.

### Debug pico
Unplug, hold BOOTSEL, replug, release BOOTSEL. Then, `sudo picotool info -a`.

</details>

## Links
- [api documentation](https://raspberrypi.github.io/pico-sdk-doxygen/index.html)
- [pinout summary](https://microcontrollerslab.com/raspberry-pi-pico-pinout-features-programming-peripherals/)
- [detailed engineering analysis of the SDK](https://www.stereorocker.co.uk/2021/02/14/raspberry-pi-pico-displays-fonts-portability/)
- [tools for reading printf output via usb](https://www.raspberrypi.org/forums/viewtopic.php?t=302227)

