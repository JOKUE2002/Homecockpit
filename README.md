# Homecockpit

## What is it?
The homecockpit is a piece of hardware, that emulates a cockpit for a given computergame.
In this case we're emulating a "universal" cockpit in M$ FSX:SE.
"Universal" means, that it is adapted, so that it can be used on any aircraft type.

## What is the main concept?
The main concept is to have a big frame with cutouts, where panels are _slotted_. 
The frame features:
* space for 7 panels (6x 20x10cm + 1x 10x20cm), 
* a cutout for a monitor to show more complex instruments (PFD, GPS, ...)
* a cutout to fit the saitek yoke
* a powerbar
* an Arduino MEGA as the "brain"

## Which panels are already finished?
* Basic RadioPanel
  * 2 frequency displays + 1 knob (to set s/b freqency)
  * Hardware done (100%)
  * Software still WIP (w/ main firmare)
* AutopilotPanel Part 1
  * CRS, HDG, SPD displays + knobs
  * hardware done (100%)
  * software still WIP (w/ main firmare), but a beta has already been written and tested
* AutopilotPanel Part 2
  * ALT, V/S displays + knobs
  * hardware done (100%)
  * same software as AutopilotPanel Part 1
* Advanced RadioPanel
  * SQUAWK display + setting buttons; com/nav selector/switch buttons
  * hardware work in progress (50% done)
  * software not yet started, but easy (0%)

## Which panels are still planned?
* Landing gear lever
  * lever that controlls the landing gear, with indicators (green/red/off)
* simulatorcommands
  * buttons to command fsx (pause, slew mode, kneeboard, ...)
* Electrical Panel
  * Lights, APU, Generators, ...

## What's included?
In the "_pcb_" folder, there are the EAGLE **.sch**ematic/**.b**oa**rd** files for the custom PCBs we designed and ordered to make things easyer.
In the "_firmware_" folder there are nearly all iterations of the firmaware, including the current version ("_FSX_3.0_").
In the "_parts_" folder there is a list of all the parts, that have been used to build the cockpit.

## How much is done is it?
If we had to guess this would be like one third done. There are still ~~a few~~ a lot of the planed panels missing. Also the main frame is still a work in progress.

### Individual statuses
* Frame: 70%
* Firmware: 40%
* Panels: 40%
* whole project: 30%

## Who's contributing?
Currently there are two main contributers:
Jonas (Hardware & Software specialist)
Cornelius (Flightsim/flying & design specialist)

## How can you contact us / help us?
Just create an issue. We'll answer on it as soon as possible.
