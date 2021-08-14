/*
  Arduino SONY S-LINK/Control-A1 Protocol Library

  https://github.com/Ircama/Sony_SLink.git

  (C) Ircama, 2017, CC-BY-SA 4.0
  https://creativecommons.org/licenses/by-sa/4.0/

  Feel free to share this source code, but include explicit mention to the author.
  Licensed under creative commons - see http://creativecommons.org/licenses/by-sa/3.0/
*/

#ifndef SLINK_MD_H
#define SLINK_MD_H
#if (ARDUINO >= 100)
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

// Ref. http://boehmel.de/slink.htm

// List of commands sent to MD-recorder
#define SLINK_CMD_MD_PLAY 0x0
#define SLINK_CMD_MD_STOP 0x1
#define SLINK_CMD_MD_PAUSE 0x2
#define SLINK_CMD_MD_TOGGLE_PAUSE 0x3
#define SLINK_CMD_MD_EJECT 0x4
#define SLINK_CMD_MD_REC_PAUSE 0x7
#define SLINK_CMD_MD_SKIP_FF 0x8
#define SLINK_CMD_MD_SKIP_REW 0x9
#define SLINK_CMD_MD_TREC 0xA
#define SLINK_CMD_MD_QUERY_STORE 0xE
#define SLINK_CMD_MD_SETUP_INFO 0xF
#define SLINK_CMD_MD_FWD_FAST 0x10
#define SLINK_CMD_MD_REW_FAST 0x11
#define SLINK_CMD_MD_FWD_SLOW 0x12
#define SLINK_CMD_MD_REW_SLOW 0x13
#define SLINK_CMD_MD_NORMAL 0x1F
#define SLINK_CMD_MD_DIS_RMCTRL 0x20
#define SLINK_CMD_MD_EN_RMCTRL 0x21
#define SLINK_CMD_MD_DEVICE_TYPE 0x22
#define SLINK_CMD_MD_AUTO_PAUSE_OFF 0x23
#define SLINK_CMD_MD_AUTO_PAUSE_ON 0x24
#define SLINK_CMD_MD_TIME_UPDATE_ON 0x25
#define SLINK_CMD_MD_TIME_UPDATE_OFF 0x26
#define SLINK_CMD_MD_POWER_ON 0x2E
#define SLINK_CMD_MD_POWER_OFF 0x2F
#define SLINK_CMD_MD_DIVIDE 0x32
#define SLINK_CMD_MD_MENU_YES 0x33
#define SLINK_CMD_MD_MENU_NO 0x34
#define SLINK_CMD_MD_UNDO 0x35
#define SLINK_CMD_MD_DEVICE_KEY_DIS 0x3E
#define SLINK_CMD_MD_DEVICE_KEY_EN 0x3F
#define SLINK_CMD_MD_ERASE_TRK 0x40 // 0x40 TT
#define SLINK_CMD_MD_MV_TRK 0x41	// 0x41 TT TT
#define SLINK_CMD_MD_COMB_PREV_TRK 0x42	// 0x42 TT
#define SLINK_CMD_MD_COMB_WITH_TRK 0x43	// 0x43 TT
#define SLINK_CMD_MD_QRY_DISC 0x44	// 0x44 DD
#define SLINK_CMD_MD_QRY_TRK 0x45	// 0x45 DD TT
#define SLINK_CMD_MD_SPLIT_ADJ 0x46	// 0x46 DD
#define SLINK_CMD_MD_PLAY_DIR_TRK 0x50	// 0x50 DD TT
#define SLINK_CMD_MD_PAUSE_DIR_TRK 0x51	// 0x51 DD TT
#define SLINK_CMD_MD_REMAIN_TIME_DSK 0x54	// 0x54 DD
#define SLINK_CMD_MD_QRY_DSK_NAME 0x58	// 0x58 DD 0x00
#define SLINK_CMD_MD_QRY_TRK_NAME 0x5A	// 0x5A TT 0x00
#define SLINK_CMD_MD_DEVICE_NAME 0x6A
#define SLINK_CMD_MD_REC_DATE 0x72	// 0x72 TT
#define SLINK_CMD_MD_COMB_A_B 0x76	// 0x76 DD TT
#define SLINK_CMD_MD_A_B_ERASE_A 0x78	// Set point A
#define SLINK_CMD_MD_A_B_ERASE_B 0x79	// Set point B
#define SLINK_CMD_MD_ADJ_A 0x7A	// Adjust point A
#define SLINK_CMD_MD_ADJ_B 0x7B	// Adjust point B
#define SLINK_CMD_MD_A_B_ERASE_ACC_A 0x7C	// Confirm point A
#define SLINK_CMD_MD_ACC_A_B_ERASE 0x7D	// Confirm erase
#define SLINK_CMD_MD_DISK_INFO 0x970100
#define SLINK_CMD_MD_WRITE_DISK_TXT 0x98	// 0x98 DD 00 00 [14d x CC]
#define SLINK_CMD_MD_WRITE_DISK_MORE 0x99	// 0x99 BB [16d x CC]
#define SLINK_CMD_MD_WRITE_TRK_TXT 0x9A	// 0x9A TT 00 00 [14d x CC]
#define SLINK_CMD_MD_WRITE_TRK_MORE 0x9B	// 0x9B BB [16d x CC]

// MD Receive Messages
#define SLINK_ST_MD_PLAY 0x0
#define SLINK_ST_MD_STOP 0x1
#define SLINK_ST_MD_PAUSE 0x2
#define SLINK_ST_MD_TOC_UPD 0x3
#define SLINK_ST_MD_REC_PLAY 0x4
#define SLINK_ST_MD_IGNORED 0x5
#define SLINK_ST_MD_SEEKING 0x6
#define SLINK_ST_MD_REC_PAUSE 0x7
#define SLINK_ST_MD_READY 0x8
#define SLINK_ST_MD_START_TRCK 0x9
#define SLINK_ST_MD_30S_TO_END 0xC
#define SLINK_ST_MD_UNAVAILABLE 0xE
#define SLINK_ST_MD_ERROR 0xF
#define SLINK_ST_MD_NO_MEMO 0x10
#define SLINK_ST_MD_NO_DEL_FILE 0x11
#define SLINK_ST_MD_NO_GROUP 0x12
#define SLINK_ST_MD_INV_DSC 0x14
#define SLINK_ST_MD_INV_DSC_TRK 0x15
#define SLINK_ST_MD_NO_DSK_NAME 0x16
#define SLINK_ST_MD_NO_TRK_NAME 0x17
#define SLINK_ST_MD_DOOR_OPEN 0x18
#define SLINK_ST_MD_NO_GROUP_CD 0x19
#define SLINK_ST_MD_NO_CD_TXT 0x1D
#define SLINK_ST_MD_WRITTEN_TO_MEM 0x1F
#define SLINK_ST_MD_PWR_ON 0x2E
#define SLINK_ST_MD_PWR_OFF 0x2F
#define SLINK_ST_MD_NOSE 0x31
#define SLINK_ST_MD_COMPLETE 0x33
#define SLINK_ST_MD_TRK_STAT 0x50
#define SLINK_ST_MD_TM_UPD_ST 0x51
#define SLINK_ST_MD_REM_TM_DSK 0x54
#define SLINK_ST_MD_DSK_TXT 0x58
#define SLINK_ST_MD_DSK_TXT_ADD 0x59
#define SLINK_ST_MD_TRK_TXT 0x5A
#define SLINK_ST_MD_TRX_TXT_ADD 0x5B
#define SLINK_ST_MD_DSK_INFO 0x60
#define SLINK_ST_MD_DSC_CAP 0x61
#define SLINK_ST_MD_TRK_INFO 0x62
#define SLINK_ST_MD_DEV_NAME 0x6A
#define SLINK_ST_MD_STATUS 0x70
#define SLINK_ST_MD_TOC_READ 0x71

#endif
