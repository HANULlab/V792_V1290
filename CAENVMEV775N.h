/*!
        -----------------------------------------------------------------------------

        -----------------------------------------------------------------------------

        CAENVMEV775N.h

        -----------------------------------------------------------------------------

        Created: January 2021
        Hyunmin Yang, HANUL, korea Univerisity
        Output buffer structure and memory map of V775N
        -----------------------------------------------------------------------------
*/
#ifndef __CAENVMEV775_H
#define __CAENVMEV775_H

#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>

// V775N Output Buffer bit field
// output buffer, ro, D16
typedef struct _V775N_OUTBUF_REG{
    union {
        struct {
            uint32_t data         : 12;
            uint32_t overFlow     : 1;
            uint32_t underThresh  : 1;
            uint32_t nothing1     : 3;
            uint32_t channel      : 4;
            uint32_t nothing2     : 3;
            uint32_t type         : 3;
            uint32_t geo          : 5;
        };
        uint32_t buffer;
    };
} V775N_OUTBUF_REG;

// V775N Address Mab structure
// Can be access via BASE_ADDRESS + OFFSET_ADDRESS
// all register contents are described as
// description, access type(read-only, read/write...), access mode
enum V775N_ADDR{
  cv775FirmRev      = 0x1000,// Firmware Revision, ro, D16
  cv775GeoAddr      = 0x1002,// Geo Address, r/w, D16
  cv775McstAddr     = 0x1004,// MCST/CBLT address, r/w, D16
  cv775BitSet1      = 0x1006,// Bit Set 1, r/w, D16
  cv775BitClr1      = 0x1008,// Bit Clear 1, r/w, D16
  cv775IrLvl				= 0x100A,// Interrupt Level, r/w, D16
  cv775IrVec        = 0x100C,// Interrupt Vector, r/w, D16
  cv775StatReg1		  = 0x100E,// Status Register 1, ro, D16
  cv775CtlReg1			= 0x1010,// Control Register, r/w, D16
  cv775AddrHigh		  = 0x1012,// ADER High, r/w, D16
  cv775AddrLow			= 0x1014,// ADER Low, r/w, D16
  cv775SsRst				= 0x1016,// Single Shot Reset, wo, D16
  cv775McstCtl			= 0x101A,// MCST/CBLT Ctl, r/w, D16
  cv775EvtTrigReg	  = 0x1020,// Event Trigger Register, D16
  cv775StatReg2		  = 0x1022,// Status Register, ro, D16
  cv775EvtCntL			= 0x1024,// Event Counter Low, ro, D16
  cv775EvtCntH			= 0x1026,// Event Counter High, ro, D16
  cv775IncEvt			  = 0x1028,// Increment Event, wo, D16
  cv775IncOff			  = 0x102A,// Increment Offset, wo, D16
  cv775LdTestReg		= 0x102C,// Load Test Register, r/w, D16
  cv775FclrWindow	  = 0x102E,// Fast Clear Window, r/w, D16
  cv775BitSet2			= 0x1032,// Bit Set 2, r/w, D16
  cv775BitClr2			= 0x1034,// Bit Clear 2, wo, D16
  cv775WmemTestAddr = 0x1036,// W Memory Test Address, wo, D16
  cv775MemTestWrdH	= 0x1038,// Memory Test Word_High, wo, D16
  cv775MemTestWrdL	= 0x103A,// Memory Test Word_Low, wo, D16
  cv775CrateSel		  = 0x103C,// Crate Selection, r/w, D16
  cv775TestEvtWr		= 0x103E,// Test Event Write, wo, D16
  cv775EvtCntRst		= 0x1040,// Event Counter Reset, wo, D16
  cv775Fsr				  = 0x1060,// Fulll Scale Range, r/w, D16
  cv775RtestAddr		= 0x1064,// R Test Address, wo, D16
  cv775SwComm			  = 0x1068,// SW Comm, wo, D16
  cv775AAD					= 0x1070,// ADD, ro, D16
  cv775BAD					= 0x1072,// BAD, ro, D16
  cv775Thresh			  = 0x1080,// Thresholds, r/w, D16
  // ROM address map(all read-only)
  cv775OuiMsb       = 0x8026,// Manufacturer Identifier
  cv775Oui          = 0x802A,// Manufacturer Identifier
  cv775OuiLsb       = 0x802E,// Manufacturer Identifier
  cv775Version      = 0x8032,// Purchased Version of the Mod.V792
  cv775BrdIdMsb     = 0x8036,// Board Identifier(V792)
  cv775BrdId        = 0x803A,// Board Identifier(V792)
  cv775BrdIdLsb     = 0x803E,// Board Identifier(V792)
  cv775Revision     = 0x804E,// Hardware Revision Identifier
  cv775SerialMsb    = 0x8F02,// Serial Number
  cv775SerialLsb    = 0x8F06,// Serial Number
};

#endif // __CAENVMEV792_H

