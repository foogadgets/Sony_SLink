# Sony_SLink
[![license](https://img.shields.io/github/license/doocs/advanced-java?color=42b883&style=flat-square)](./LICENSE)

**Arduino SONY S-LINK/Control-A1 Protocol Library.**

  This code is based on the code written by Ircama, https://github.com/Ircama/Sony_SLink.git

  The code deviates in several points.
  1. The protocol implements the use of 2 pin communication. RX and TX. This in order to reuse the Sony HW implementation of the S-Link hardware interface.
  2. There is error detection
  3. There is support for writing full Table of Contents to a MiniDisc.
  4. There is support for receiving signals from a S-Link device.
  5. Timing parameters have been adjusted and signalling have been verified using Oscilloscope and Logic analyzer to get the timing right. A Sony MDS-JB940 QS was used as reference.


This library allows to drive Sony audio consumer devices connected through the
Sony S-Link bus-system.

The code verified on a ESP8266 microcontroller based module.

-------------------------------------------------------------------------------------------------------------------

# Installation
Create a new folder called "Sony_SLink" under the folder named "libraries" in your Arduino sketchbook folder.
Create the folder "libraries" in case it does not exist yet. Place all the files in the "Sony_SLink" folder.

# Usage
To use the library in your own sketch, select it from *Sketch > Import Library*.

# Example

Simplest example of using this library:

```c++
#define SLINK_INPIN 5 // S-Link Control - input pin
#define SLINK_OUTPIN 4 // S-Link Control - output pin

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

`slink.init(SLINK_INPIN, SLINK_OUTPIN)` - set-up S-Link pins

`slink.sendCommand(device, command)` - send a command to the device; check [Sony_SLink_Defines.h](https://github.com/foogadgets/Sony_SLink/blob/master/Sony_SLink_Defines.h) and [Sony_SLink_MD.h](https://github.com/foogadgets/Sony_SLink/blob/master/Sony_SLink_MD.h) for available commands

Examples:
```c++
slink.sendCommand(SLINK_DEVICE_MD, SLINK_CMD_MD_PLAY);
slink.sendCommand(SLINK_DEVICE_MD, SLINK_CMD_MD_REC_PAUSE);
slink.sendCommand(SLINK_DEVICE_MD, SLINK_CMD_MD_STOP);
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

# Notes

  This fork is heavily based on:
  - Ircama's code:
    https://github.com/Ircama/Sony_SLink.git

  Tested with a Sony MDS-JB940 QS MiniDisc Recorder.
  
