# Sony_SLink

**Arduino SONY S-LINK/Control-A1 Protocol Library.**

  This code is based on the code written by (C) Ircama, https://github.com/Ircama/Sony_SLink.git

  The code deviates in several points.
  1. The protocol implements the use of 2 pin communication. RX and TX. This in order to reuse the Sony HW implementation of the S-Link hardware interface.
  2. There is error correction
  3. There is support for writing full Table of Contents to a MiniDisc.
  4. There is support for receiving signals from a S-Link device.
  5. Timing parameters have been adjusted and signalling have been verified using Oscilloscope and Logic analyzer to get the timing right. A Sony MDS-JB940 QS was used as reference.


This library allows to drive Sony audio consumer devices connected through the
Sony S-Link bus-system.

(C) Ircama, 2017, CC-BY-SA 4.0
https://creativecommons.org/licenses/by-sa/4.0/

The code is targeted to the ESP8266 microcontroller.

-------------------------------------------------------------------------------------------------------------------

# Installation
Create a new folder called "Sony_SLink" under the folder named "libraries" in your Arduino sketchbook folder.
Create the folder "libraries" in case it does not exist yet. Place all the files in the "Sony_SLink" folder.

# Usage
To use the library in your own sketch, select it from *Sketch > Import Library*.

# Example

Simplest example of using this library:

```c++
#define SLINK_INPIN 5 // S-Link Control-A1 pin
#define SLINK_OUTPIN 4 // S-Link Control-A1 pin

#include "Sony_SLink.h"
Slink slink;

void setup() {
  slink.init(SLINK_INPIN, SLINK_OUTPIN); // Set-up S-Link pin
  slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_POWER_ON);
}

void loop() {
}
```

Check the *examples* directory for further examples.

# Description of the functions

`slink.init(SLINK_INPIN, SLINK_OUTPIN)` - set-up S-Link pin

`slink.sendCommand(device, command)` - send a command to the device; check [Sony_SLink.h](https://github.com/Ircama/Sony_SLink/blob/master/Sony_SLink.h) for available commands

Examples:
```c++
slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_VOLUME_UP);
slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_VOLUME_DOWN);
slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_POWER_OFF);
slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_POWER_OFF);
slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_POWER_ON);
slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_SET_INPUT_CHAN, SLINK_CMDP_IN_TV);
slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_MUTE_ON);
slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_MUTE_OFF);
slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_SET_INPUT_CHAN, SLINK_CMDP_IN_TUNER);
slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_SET_INPUT_CHAN, SLINK_CMDP_IN_CD);
slink.sendCommand(SLINK_DEVICE_TUNER, SLINK_CMD_TUNER_PRESET_UP);
slink.sendCommand(SLINK_DEVICE_TUNER, SLINK_CMD_TUNER_PRESET_DOWN);
slink.sendCommand(SLINK_DEVICE_TUNER, SLINK_CMD_TUNER_PRESET_STATION, 1, 1);
```

`slink.inputMonitor` - monitor SLink input
Available options:
```c++
slink.inputMonitor(0); // measure timing of mark periods in microseconds (sync should be 2400, one about 1200, zero ab. 600)
slink.inputMonitor(1); // monitor bytes displaying binary and HEX format of each byte
slink.inputMonitor(2); // monitor bytes displaying HEX dump
slink.inputMonitor(0, true); // measure timing of idle periods (e.g., delimiter; all idle periods should be about 600 microseconds)
```

# Interfacing STR-DA1000ES

The STR-DA1000ES receiver (3V TTL, 112 AMP device ID) needs some amount of wait time between the volume up and down codes (20 and 21) to actually change the volume. From some experimentation, timing seems to be between 3ms and 1960ms. Any shorter or longer and the volume won't change.

Example:

```c++
void loop() {
slink.sendCommand(112, 20);
delay(3); //up to delay(1960);
}
```

# Notes

  Code heavily based on:
  - Ircama's code:
    https://github.com/Ircama/Sony_SLink.git
  - hajdbo's code:
    https://github.com/hajdbo/sony-jukebox-slink/blob/master/jukebox.pde
  - EdsterG's code:
    https://github.com/EdsterG/LANC_Library

  Reference documents:
  - Sony S-Link gateway document:
    http://ee.bradley.edu/projects/proj2001/sonyntwk/SLink.PDF

  - How SONY's S-LINK/CTRL-A(II) protocol works (waveform):
    http://boehmel.de/slink.htm

  - Codes for Sony STR-DA50ES receiver:
    http://www.hifi-remote.com/sony/Sony_dsp_192.htm

  - S-Link Parallel Device for Linux
    http://web.archive.org/web/20040722060841/http://www.undeadscientist.com/slink/index.html

  - Reverse Engineered Control-A1 codes (Using the Sony CDP-CX250) Written by BigDave (2/19/98)
    http://web.archive.org/web/19981202175957/http://www.cc.gatech.edu/people/home/bigdave/cdplayer/control-a1.txt

  - Working S-Link code - Background of the S-link protocol
    http://forums.parallax.com/discussion/70973/working-s-link-code-long

  - Slink Send and Receive Commands for Sony MiniDisc Recorders
    http://web.archive.org/web/20030419100153/http://web2.airmail.net/will/mdslink.txt

  - Jukebox Interface Guide
    http://web.archive.org/web/20030414200645/http://www.upl.cs.wisc.edu/~orn/jukebox/guide.html

  - Original Control-A1 document
    http://web.archive.org/web/20030414231523/http://www.upl.cs.wisc.edu/~orn/jukebox/controla1.html

  Tested with a Sony MDS-JB940 QS MiniDisc Recorder.
  
  This protocol is very slow: average 355 bps half duplex for a standard two-byte send transmission taking 45 millisecs (355=16/0,045).

  Feel free to share this source code, but include explicit mention to the author.
  Licensed under creative commons - see http://creativecommons.org/licenses/by-sa/4.0/
