/*
  Arduino SONY S-LINK/Control-A1 Protocol Library

  https://github.com/Ircama/Sony_SLink.git

  (C) Ircama, 2017, CC-BY-SA 4.0
  https://creativecommons.org/licenses/by-sa/4.0/

  Feel free to share this source code, but include explicit mention to the author.
  Licensed under creative commons - see http://creativecommons.org/licenses/by-sa/3.0/
*/

#ifndef SLINK_H
#define SLINK_H

#if (ARDUINO >= 100)
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif
#include "Sony_SLink_Defines.h"
#include "Sony_SLink_MD.h"

#define TOCBUFLEN 10
#define TOCSTRLEN 16

// S-LINK Protocol
#define SLINK_MARK_SYNC        2400 // microseconds
#define SLINK_MARK_ONE         1200 // microseconds
#define SLINK_MARK_ZERO         600 // microseconds
#define SLINK_MARK_DELIMITER    600 // microseconds
#define SLINK_MARK_RANGE       +1.2 // timing detection tolerance (2-2.88ms, 1-1.44ms, 500-720us)
#define SLINK_WORD_DELIMITER  45000 // padding microseconds after transmitting a command (45msecs should be the correct value...)
#define SLINK_LINE_READY       5000 // microseconds to wait for the line to become ready before transmitting a command
#define SLINK_LOOP_DELAY         25 // microseconds timer thick during a wait operation
#define SLINK_LOOP_TIMEOUT   500000 // microseconds total timeout while waiting

class Slink
{
  public:
    void init(int slinkInPin, int slinkOutPin);
    void sendCommand(unsigned int deviceId = 0x00, unsigned int commandId1 = 0x00, int commandId2 = -1, int commandId3 = -1);
    bool writeDiskTitle(char* diskTitle);
    bool writeTrackTitle(uint8_t trackNo, char* diskTitle);
    void inputMonitor(int type = 0, boolean idle = false, unsigned long uSecTimeout = 10000000UL, unsigned long serialSpeed = 115200UL);
    int  inPin();
    int  outPin();
  private:
    bool    _writeDiskTitle(uint8_t slinkCommand, uint8_t trackDiskNo, char* diskTitle);
    uint8_t _prepareTitleBuffer(char* title);
    char*   _recvAnswer(char* recvBuffer, size_t bufSize);
    void    _lineReady();
    void    _writeSync();
    void    _writeByte(byte value);
    char    _titleBuffer[TOCBUFLEN][TOCSTRLEN+1];
    int     _slinkInPin;
    int     _slinkOutPin;
};

#endif
