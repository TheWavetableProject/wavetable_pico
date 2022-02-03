# Installing the Software

Before using your wavetable, you need to flash the `.uf2` software file to the microcontroller. 

1. First, you'll have to wire up the electronics! See [the wiring guide](https://github.com/Exr0nProjects/wavetable_pico/blob/main/wiring.md).
2. Enter BOOTSEL mode, by pressing and holding the white `BOOTSEL` button on the front of the pico board
3. Plug the Raspberry Pico into a laptop using a micro-USB cable
4. Release the BOOTSEL button. On Mac and Windows laptops, the Raspberry Pico should show up as a flash drive under the name `RPI-RP2`.
5. Drag the most recent `.uf2` file from the [the `releases` folder](https://github.com/Exr0nProjects/wavetable_pico/tree/main/releases) into the media device. It should eject itself, and a green light should turn on. 

Congratulations, you are ready to use your wavetable! Learn how to interact with it on [MacOS](./macos_with_screen.md).

## Troubleshooting
- If you are on MacOS and an "unexpceted error" occurs, try unplugging and replugging the Pico. 

## Alternate Flashing Methods

Below are alternate methods to upload the `.uf2` to the Raspberry Pico (step 5 above). 

### Picotool

<details><summary>Loading executable using `picotool` command line tool</summary>
If this section is confusing, jump down to the manual installation method below. 

Install `picotool` using `apt` or from the AUR. (At time of writing, `brew` does not appear to have `picotool`) 

After plugging in the Raspberry Pico in BOOTSEL mode, load the binary:
```sh 
sudo picotool load -x path/to/wavetable_main.uf2
```
</details>


#### Manual 

<details><summary>Loading executable manually</summary>
If on Linux, manually mount the Rasperry Pico as a drive.

Copy the file onto the drive: `cp /path/to/wavetable_binary.uf2 /path/to/RPI-RP2`. Make sure you aren't already in the target directory (Raspberry Pico) when doing this. 
</details>


