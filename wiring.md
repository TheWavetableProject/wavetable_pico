# Wiring your Wavetable

## Prep

To wire up your wavetable, you'll need:
- Materials
    - Stepper driver (TB6600)
    - Stepper motor (NEMA 17 flat)
    - Power supply (DC 12V 1-2A output)
    - Raspberry Pico 
    - Solid core wire (20-24 gauge)
    - 40mm Fan
- Tools
    - Saftey glasses
    - Soldering equipment (soldering iron, solder, fume hood)
    - Wire stripper
    - Wire cutter
    - Multimeter

## Wiring

It's time to wire up the electronics! Connect the components as in the diagram below.See the notes below the diagram and images for more details. 

**If using a 40mm fan, connect the wires to the 12v power supply terminals directly.**

![Wiring diagram](https://github.com/Exr0nProjects/wavetable_pico/blob/main/materials/wiring_diagram_streamlined.png?raw=true)

For example, your electronic circuit may look like this when complete. (The three wires going to the Pico are soldered on the back of the board).

![Completed wiring example](https://github.com/Exr0nProjects/wavetable_pico/blob/main/materials/wobbles_wiring_no_estop.png?raw=true)

Note that you'll need to insert two wires into the PUL+ terminal or the DIR+ terminal, to tie them both high.

**Check that the stepper motor is wired correctly**, by making sure that the pairs of wires going into each lettered terminal are a part of the same circuit. You can check by using the continuity test on your multimeter or by looking at the resistance: the resistance should be low between the 3rd and 4th wire down (A circuit), and for the 5th and 6th wire down (B circuit). If this isn't the case, swap the wires around until the connected wires are next to eachother. 

If your stepper is rotating in the wrong direction, swap one of the circuit pairs (A+ with A- or B+ with B-). Don't swap both, since that is the same as doing nothing.

**Be sure to turn on microstepping on your stepper controller**, or change the constants in the code and rebuild the `.uf2`, or deal with incorrect RPM calculations. To enable microstepping on the TB6600, flip dip switches SW1 through SW3 to the up position, such that they are pointing towards the side with lettering and away from the heatsink fins.

Also adjust the amerage limit. The final dip switch configuration should be: UP UP UP DOWN UP UP

![DIP Switches](https://github.com/Exr0nProjects/wavetable_pico/blob/main/materials/dip_switches.png?raw=true)

## Sanity Check

Even without loading any software onto your Pico, the stepper driver should be able to send power to the stepper. To check that this part of the circuit works, plug in the power supply and check that the stepper motor is harder to turn with your hand than before.

If this is working, it's time to load software onto your microcontroller! See the upload instructions [here](https://github.com/Exr0nProjects/wavetable_pico#setup).

