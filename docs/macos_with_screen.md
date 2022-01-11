# Interacting with the Wavetable using `screen` on MacOS

1. Start by plugging in both the power supply and microcontroller on the Wavetable. The fan should start blowing air, the table should start spinning, and a green light should turn on on the microcontroller.
1. Open Terminal.app
1. Paste in the command `screen /dev/tty.usbmodem0000000000001 115200`, ensuring there are two spaces.
1. Press enter. The screen should clear.

Now, you are ready to control your Wavetable! The valid commands are listed below.

| Command                | Description                                                |
|------------------------|------------------------------------------------------------|
| `set <float>`  | Ramp up/down speed to the target RPM.                              |
| `fset <float>` | Force set the speeed without interpolation.                        |
| `lset <x> <y>` | Set speed to `x` RPM with linear interpolation at `y` RPM per min. |
| `sina <float>` | Set the amplitude of the sine wave (0 to disable).                 |
| `sinf <float>` | Set the frequency of the sine wave, in Hz.                         |
| `info`         | Print software configuration information.                          |

