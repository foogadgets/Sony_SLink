/*

  Arduino SONY S-LINK/Control-A1 Protocol Library

  https://github.com/foogadgets/Sony_SLink.git

  This code is based on the code written by (C) Ircama
  The original code is licenced under 2017, CC-BY-SA 4.0
  https://creativecommons.org/licenses/by-sa/4.0/
  and so is this code.

  The code deviates in several points.
  1. The protocol implements the use of 2 pin communication. RX and TX. This in order to reuse the Sony HW implementation of the S-Link hardware interface.
  2. There is error correction
  3. There is support for writing full Table of Contents to a MiniDisc.
  4. There is support for receiving signals from a S-Link device.
  5. Timing parameters have been adjusted and signalling have been verified using Oscilloscope and Logic analyzer to get the timing right. A Sony MDS-JB940 QS was used as reference.

*/

#include "Sony_SLink.h"


/* Initialize protocol */

void
Slink::init(int slinkInPin, int slinkOutPin){
  this->_slinkInPin=slinkInPin;
  this->_slinkOutPin=slinkOutPin;
  pinMode(this->_slinkInPin, INPUT); // Define pin mode INPUT for slinkPin.
  pinMode(this->_slinkOutPin, OUTPUT); // Define pin mode OUTPUT for slinkPin.
  digitalWrite(this->_slinkOutPin, LOW); // release the default HIGH state

  for (uint8_t i=0; i<TOCBUFLEN; i++) {
    memset(this->_titleBuffer[i], 0, TOCSTRLEN+1);
  }
}


void
Slink::sendCommand(unsigned int deviceId, unsigned int commandId1, int commandId2, int commandId3){
  unsigned long Start;

  _lineReady();          // Check line availability to write a frame
  Start = micros();
  _writeSync();          // Beginning of new transmission
  _writeByte(deviceId);  // Select device (one byte)
  _writeByte(commandId1); // Send the actual Command-Code (one byte)
  if (commandId2>=0)
    _writeByte(commandId2); // Send the actual Command-Code (one byte)
  if (commandId3>=0)
    _writeByte(commandId3); // Send an additional Command-Code (one byte)

  do { // The command sequence must be padded (by 5V) at the end to make it 45 millisec. long
    delayMicroseconds(SLINK_LOOP_DELAY);
    if (digitalRead(this->_slinkInPin)==LOW)
      break; // break padding if the other device starts transmitting meanwhile
  } //  (but the padding rule is apparently not very strict)
  while (micros()-Start < SLINK_WORD_DELIMITER);
}



bool
Slink::writeTrackTitle(uint8_t trackNo, char* diskTitle) {
/*
9A TT 00 00 [14d x CC]	write track text	if CC<14d, fill with 00 (ok, answer 1F)
9B BB [16d x CC]	write track more text	BB>1 (block), if CC<16d, fill with 00 (ok, answer 1F)
*/

  uint8_t segments = _prepareTitleBuffer(diskTitle);

  if(!_writeDiskTitle(0x9a, trackNo, this->_titleBuffer[0])) { return false; }
  if (segments > 1) {
    for (uint8_t i=1; i<segments; i++) {
      if(!_writeDiskTitle(0x9b, i+1, this->_titleBuffer[i])) { return false; }
    }
  }
  return true;
}


bool
Slink::writeDiskTitle(char* diskTitle) {
/*
98 DD 00 00 [14d x CC]	write disc text	if CC<14d, fill with 00 (ok, answer 1F)
99 BB [16d x CC]	write disc more text	BB>1 (block), if CC<16d, fill with 00 (ok, answer 1F)
*/

  uint8_t segments = _prepareTitleBuffer(diskTitle);

  if(!_writeDiskTitle(0x98, 0x01, this->_titleBuffer[0])) { return false; }
  if (segments > 1) {
    for (uint8_t i=1; i<segments; i++) {
      if(!_writeDiskTitle(0x99, (i+1), this->_titleBuffer[i])) { return false; }
    }
  }
  return true;
}


uint8_t
Slink::_prepareTitleBuffer(char* title) {
  uint16_t titleLen = 2 + strlen(title);
  uint8_t segments = 1 + (titleLen / (TOCSTRLEN+0.0001));
// Init all buffer to \0
  for (uint8_t i=0; i<TOCBUFLEN; i++) {
    memset(this->_titleBuffer[i], 0, TOCSTRLEN+1);
  }

// Write first title buffer, start with 0x00, 0x00 (already in buffer after init)
  strncpy(&this->_titleBuffer[0][2], title, (TOCSTRLEN-2));

  if (segments > 1) {
    for (uint8_t i=1; i<segments; i++) {
      strncpy(this->_titleBuffer[i], &title[i*TOCSTRLEN-2], TOCSTRLEN);
    }
  }

  return segments;
}


bool
Slink::_writeDiskTitle(uint8_t slinkCommand, uint8_t trackDiskNo, char* diskTitle) {
  char recvBuffer[2];
  uint8_t retries = 0;

  do {
    _lineReady();         // Check line availability to write a frame
    _writeSync();         // Beginning of new transmission
    _writeByte(0xb0);		// Select device (one byte)
    _writeByte(slinkCommand);		// Send the actual Command-Code (one byte)
    _writeByte(trackDiskNo);		// Send the actual Command-Code (one byte)
    for (uint8_t i=0;i<TOCSTRLEN; i++) {
      _writeByte(diskTitle[i]);         // Send the actual Command-Code (one byte)
    }

    delayMicroseconds(2*SLINK_LINE_READY);

    if (0 == strncmp("\xb8", _recvAnswer(recvBuffer, 2), 1)) {
      if (0 == strncmp("\x1f", &recvBuffer[1], 1)) {
        delayMicroseconds(2*SLINK_LINE_READY);
	return true;
      }
    }

    Serial.println("Error. Resending data");

    retries++;
    delayMicroseconds(2*SLINK_LINE_READY);

  } while( retries < 3 );

  return false;
}


char*
Slink::_recvAnswer(char* recvBuffer, size_t bufSize) {
  unsigned long pulseDuration = 0;
  unsigned long Start = 0;
  uint8_t state = 0;
  uint8_t count = 0; // tracks what bit we are currently working with
  uint8_t byteBuffer = 0;
  uint8_t bytesRecv = 0;
 
  memset(recvBuffer, 0, bufSize);

  Start = micros();

  do {
    pulseDuration = pulseIn(this->_slinkInPin, LOW, 55000UL); // timeout to 40ms
    if (pulseDuration == 0) { // no sending ongoing within Timeout
      continue;
    }
    else {  // We found a pulse and will analyze.
      switch( state ) {
        case 0:
          if ((pulseDuration > (SLINK_MARK_SYNC / SLINK_MARK_RANGE)) && (pulseDuration < SLINK_MARK_SYNC * SLINK_MARK_RANGE)) {
            byteBuffer = 0;
            count = 0;
            state = 1;
          }
	      break;
        case 1:
          if ((pulseDuration > SLINK_MARK_ONE / SLINK_MARK_RANGE) && (pulseDuration < SLINK_MARK_ONE * SLINK_MARK_RANGE)) {
            byteBuffer |= 128>>count;
          }

          if (count++ == 7) {
            strncpy(&recvBuffer[bytesRecv++],(char*)&byteBuffer, 1);
            byteBuffer = 0;
            count = 0;
            state = 1;
            if ( bytesRecv==bufSize )
              state = 2;
          }
          break;
        case 2:
          break;
      }  // else
    }    // else Pulse detected within 5ms
  }      // do
  while (micros()-Start < 60000UL && state !=2 );
  return &recvBuffer[0];
}


/* Check line availability before transmitting or receiving

 * S-Link has no bus arbitration logic. Messages can
 * be sent at any time by any participant. Bus
 * collision may occur. Because there is not much
 * traffic, this happens extremly rare. Checking the
 * receiver state immediately before starting a new
 * transmission prevents from collisions in nearly all cases.
 */

void
Slink::_lineReady(){
  unsigned long Start = micros();
  unsigned long beginTimeout = Start;

  //Search for a 5ms=5000uSec gap (at least), being HIGH continuosly.

  do {
    delayMicroseconds(SLINK_LOOP_DELAY);
    if (digitalRead(this->_slinkInPin)==LOW)
      Start = micros(); // reset the loop each time traffic is detected
  }
  while ( ((micros()-Start) < SLINK_LINE_READY) && ((micros()-beginTimeout) < SLINK_LOOP_TIMEOUT) );
}

/*
 * Synchronisation:
 * holding the line low for SLINK_MARK_SYNC uS indicates the beginning of new transmission
 */
void
Slink::_writeSync(){
  digitalWrite(this->_slinkOutPin, HIGH); // start sync (line low)
  delayMicroseconds(SLINK_MARK_SYNC); // keep line low for SLINK_MARK_SYNC uS
  digitalWrite(this->_slinkOutPin, LOW); // release the default HIGH state
  delayMicroseconds(SLINK_MARK_DELIMITER); // Delimiter (between Sync, Ones and Zeros, the line must be released (return to high level) for SLINK_MARK_DELIMITER uS)
}

//-------------------------------------------------------------------------------------

void
Slink::_writeByte(byte value){
/*
 * Zero bit: holding the line low for SLINK_MARK_ZERO uS indicates a 0
 * One bit: holding the line low for SLINK_MARK_ONE uS indicates a 1
 * Delimiter between Sync, Ones and Zeros, the line must be released (return to high level) for SLINK_MARK_DELIMITER uS
 */

  for (int i=7; i>=0; i--) {
    digitalWrite(this->_slinkOutPin, HIGH);
    if (value & 1<<i) {
      delayMicroseconds(SLINK_MARK_ONE);
      }
    else {
      delayMicroseconds(SLINK_MARK_ZERO);
      }
    digitalWrite(this->_slinkOutPin, LOW);
    delayMicroseconds(SLINK_MARK_DELIMITER);
  }
}

//-------------------------------------------------------------------------------------

int
Slink::outPin(){
  return this->_slinkOutPin;
}

int
Slink::inPin(){
  return this->_slinkInPin;
}

//-------------------------------------------------------------------------------------

// Read, dump and debug S-Link protocol
// This function is for debugging purpose and is valid for ATMEL devices provided with serial interface,
// which is used to output the monitoring information related to S-Link timings and data, read from the input port.
#if !defined (__AVR_ATtiny85__)
void
Slink::inputMonitor(int type, // 0=measure timing, 1=decode and dump binary & hex, 2=decode and dump hex
                         boolean idle, // false(default)=measure mark (=data), true=measure idle timing (=delimiters)
                         // for S-Link protocol, marks can return sync (2400usec), zero (600usec), one (1200usec)
                         // idle=true can onewLiney be used with type=0 and timing should always return 600 usec (delimiter)
                         unsigned long uSecTimeout, // monitoring timeout; defaults to 10 seconds
                         // information read from S-Link is buffered until timeout and then dumped to the serial port
                         unsigned long serialSpeed // baud rate of the serial port (defaults to 115.2kbps)
                         ){
  unsigned long value = 0;
  unsigned long Start = micros();
  int newLine=0;
  int count=0;
  int byte=0;
  String buffer = "";
  
  Serial.begin(serialSpeed);
  Serial.println("Start monitor");
    do {
      value = pulseIn(this->_slinkInPin, idle, 5000UL); // timeout to 5 milliseconds=5000 uSec
      if (value==0) {
        if (newLine==0)
          buffer += String("\n");
        newLine=1;
        count=0;
        byte=0;
      }
      else {
        switch(type) {
         case 0: // timing
            buffer += String(",");
            buffer += String(value);
            break;
         case 1: // binary - HEX
         case 2: // HEX
            if ((value > (SLINK_MARK_SYNC / SLINK_MARK_RANGE)) && (value < SLINK_MARK_SYNC * SLINK_MARK_RANGE)) {
              buffer += String("\n");
              buffer += String("START,");
              count=0;
              byte=0;
            }
            else {
              if ((value > SLINK_MARK_ONE / SLINK_MARK_RANGE) && (value < SLINK_MARK_ONE * SLINK_MARK_RANGE)) {
                byte |= 128>>count;
                
                if (type == 1)
                  buffer += String("1,");
              }
              if ((value > SLINK_MARK_ZERO / SLINK_MARK_RANGE) && (value < SLINK_MARK_ZERO * SLINK_MARK_RANGE))
                if (type == 1)
                  buffer += String("0,");
              
              if (count++ == 7) {
                if (type == 1)
                  buffer += String(" = ");
                buffer += String(byte, HEX) + String(",");
                if (type == 1)
                  buffer += String("  ");
                count=0;
                byte=0;
              }
            }
            break;
        }
        newLine=0;
      } // else
    } // do
    while (micros()-Start < uSecTimeout);
  Serial.println(buffer);
  Serial.println("End monitor");
}
#endif
