# Interacting with the Wavetable using `screen` on MacOS

1. Start by plugging in both the power supply and microcontroller on the Wavetable. The fan should start blowing air, the table should start spinning, and a green light should turn on on the microcontroller.
1. Open Terminal.app
1. Paste in the command `screen /dev/tty.usbmodem0000000000001 115200`, ensuring there are two spaces.
1. Press enter. There should be no text on the screen.

Now, you are ready to control your Wavetable! The valid commands are listed below.

| Command                | Description                                                |
|------------------------|------------------------------------------------------------|
| `set 3.2`  | Ramp up/down speed to the target RPM.                              |
| `fset 3.2` | Force set the speeed without interpolation.                        |
| `lset 3.2 10` | Set speed to `x` RPM with linear interpolation at `y` RPM per min. |
| `sina 0.1` | Set the amplitude of the sine wave (0 to disable).                 |
| `sinf 0.01` | Set the frequency of the sine wave, in Hz.                         |
| `info`         | Print software configuration information.                          |

The numbers above are just placeholders. Try setting different speeds! Most RPM values will be between `0.1` and `20`.

If the microcontroller says "unrecognized command", there may have been unexpected characters in the input. 

To stop the Wavetable, set the speed to zero with `set 0` and unplug it.
