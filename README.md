# 3ds_hw_test
A (very) WIP hardware tester for the 3DS

## General Info
The overall, long-term goal of this project is to comprehensively test as
many of the various hardware components on Nintendo 3DS and 2DS systems as
possible, while also being able to boot and run on systems which are unable to
run almost any other software due to some critical hardware fault. Currently,
this tool only functions a simple memory tester, implementing some of the
tests from [Memtest86+](https://www.memtest.org/).

## To-Do List / Roadmap
In very approximate order of priority:
* Add an ARM11 payload
  - Remove reliance on whatever is loading the FIRM to initialize the screens
  - Do FCRAM testing from ARM11 if available; hopefully this will make testing
go much faster than it currently does with ARM9
  - The ARM9 payload (running from the TCMs) should still be able to function
independently if the ARM11 side somehow doesn't get loaded correctly or
otherwise dies, using the notification LED for status indication
* Make the memory testing functionality more complete
  - Implement the rest of the memory tests described
[here](https://www.memtest86.com/tech_individual-test-descr.html), save for
those which don't really have an equivalent on the 3DS's ARM processors
  - Further optimize, clean up, and comment the current testing code
  - Test the New3DS-exclusive extensions of FCRAM and ARM9 Internal Memory,
if the tester is running on a "new" model
* Totally revamp the UI, as the current one is dirt simple
  - Allow selecting specifc tests to run on specific memory regions
  - Allow selecting a number of passes to run
  - Display a nice-looking progess bar, estimated time remaining, current test
pattern, etc.
* Implement tests for other 3DS hardware components
* Probably some other stuff I have in mind but forgot

## How to Build This
Building requires `arm-none-eabi-gcc`, `arm-none-eabi-objcpy`, and
[firmtool](https://github.com/TuxSH/firmtool). With those installed, just
run `make`.

## License
You may use this under the terms of the GNU General Public License GPL v2 or
under the terms of any later revisions of the GPL. Refer to the provided
`license.txt` file for further information.

## Credits
* **MemTest86** and **Memtest86+** for providing very useful [conceptual
descriptions](https://www.memtest86.com/tech_memory-test.html) and [example
implementations](https://www.memtest.org/#downcode), respectively, of a proper
memory testing methodology
* **Normmatt**, for sdmmc.c / sdmmc.h and the basis for the draw code
* **Cha(N)**, **Kane49**, and all other FatFS contributors for FatFS
* All **[3dbrew.org](https://www.3dbrew.org/wiki/Main_Page) editors**
