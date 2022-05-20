/*!
        -----------------------------------------------------------------------------

        -----------------------------------------------------------------------------

        CAENVMEV792N.h

        -----------------------------------------------------------------------------

        Created: January 2021
        Hyunmin Yang, HANUL, korea Univerisity
        Output buffer structure and memory map of V792N
        -----------------------------------------------------------------------------
*/
#ifndef __CAENVMEV792_H
#define __CAENVMEV792_H

#include <stdlib.h>
#include <stdint.h>
#include "CAENVMEModule.h"
// V792N Output Buffer bit field
// output buffer, ro, D16
typedef struct _V792N_OUTBUF_REG{
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
} V792N_OUTBUF_REG;



class CAENVMEV792N:public CAENVMEModule{
	public:
		CAENVMEV792N(){};
		~CAENVMEV792N(){};
		void SetPedestal(uint16_t ped);
		void EventReset();
		void Write16(uint32_t regaddr, uint16_t data);
		void Write32(uint32_t regaddr, uint32_t data);
		uint16_t Read16(uint32_t regaddr);
		uint16_t GetStatRegister();
		uint32_t Read32(uint32_t regaddr);
};
// V792N Address Mab structure
// Can be access via BASE_ADDRESS + OFFSET_ADDRESS
// all register contents are described as
// description, access type(read-only, read/write...), access mode
enum V792N_ADDR{
  cv792FirmRev      = 0x1000, // Firmware Revision, ro, D16
  cv792GeoAddr      = 0x1002, // Geo Address, r/w, D16
  cv792McstAddr     = 0x1004, // MCST/CBLT address, r/w, D16
  cv792BitSet1      = 0x1006, // Bit Set 1, r/w, D16
  cv792BitClr1      = 0x1008, // Bit Clear 1, r/w, D16
  cv792IrLvl				= 0x100A, // Interrupt Level, r/w, D16
  cv792IrVec        = 0x100C, // Interrupt Vector, r/w, D16
  cv792StatReg1		  = 0x100E, // Status Register 1, ro, D16
  cv792CtlReg1			= 0x1010, // Control Register, r/w, D16
  cv792AddrHigh		  = 0x1012, // ADER High, r/w, D16
  cv792AddrLow			= 0x1014, // ADER Low, r/w, D16
  cv792SsRst				= 0x1016, // Single Shot Reset, wo, D16
  cv792McstCtl			= 0x101A, // MCST/CBLT Ctl, r/w, D16
  cv792EvtTrigReg	  = 0x1020, // Event Trigger Register, D16
  cv792StatReg2		  = 0x1022, // Status Register, ro, D16
  cv792EvtCntL			= 0x1024, // Event Counter Low, ro, D16
  cv792EvtCntH			= 0x1026, // Event Counter High, ro, D16
  cv792IncEvt			  = 0x1028, // Increment Event, wo, D16
  cv792IncOff			  = 0x102A, // Increment Offset, wo, D16
  cv792LdTestReg		= 0x102C, // Load Test Register, r/w, D16
  cv792FclrWindow	  = 0x102E, // Fast Clear Window, r/w, D16
  cv792BitSet2			= 0x1032, // Bit Set 2, r/w, D16
  cv792BitClr2			= 0x1034, // Bit Clear 2, wo, D16
  cv792WmemTestAddr = 0x1036, // W Memory Test Address, wo, D16
  cv792MemTestWrdH	= 0x1038, // Memory Test Word_High, wo, D16
  cv792MemTestWrdL	= 0x103A, // Memory Test Word_Low, wo, D16
  cv792CrateSel		  = 0x103C, // Crate Selection, r/w, D16
  cv792TestEvtWr		= 0x103E, // Test Event Write, wo, D16
  cv792EvtCntRst		= 0x1040, // Event Counter Reset, wo, D16
  cv792Iped				  = 0x1060, // Iped, r/w, D16
  cv792RtestAddr		= 0x1064, // R Test Address, wo, D16
  cv792SwComm			  = 0x1068, // SW Comm, wo, D16
  cv792AAD					= 0x1070, // ADD, ro, D16
  cv792BAD					= 0x1072, // BAD, ro, D16
  cv792Thresh			  = 0x1080, // Thresholds, r/w, D16
  // ROM address map(all read-only)
  cv792OuiMsb      = 0x8026,  // Manufacturer Identifier
  cv792Oui         = 0x802A,  // Manufacturer Identifier
  cv792OuiLsb      = 0x802E,  // Manufacturer Identifier
  cv792Version     = 0x8032,  // Purchased Version of the Mod.V792
  cv792BrdIdMsb    = 0x8036,  // Board Identifier(V792)
  cv792BrdId       = 0x803A,  // Board Identifier(V792)
  cv792BrdIdLsb    = 0x803E,  // Board Identifier(V792)
  cv792Revision    = 0x804E,  // Hardware Revision Identifier
  cv792SerialMsb   = 0x8F02,  // Serial Number
  cv792SerialLsb   = 0x8F06,  // Serial Number
};

#endif // __CAENVMEV792_H

