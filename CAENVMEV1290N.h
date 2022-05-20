/*!
	-----------------------------------------------------------------------------

	-----------------------------------------------------------------------------

	CAENVMEV1290N.h

	-----------------------------------------------------------------------------

Created: April 2022
Byungmin Kang, HANUL, korea Univerisity
Output buffer structure and memory map of V1290N
-----------------------------------------------------------------------------
*/
#ifndef __CAENVMEV1290_H
#define __CAENVMEV1290_H

#include <stdlib.h>
#include <stdint.h>
#include "CAENVMEModule.h"

// V1290N Output Buffer bit field
// output buffer, ro, D16
typedef struct _V1290N_OUTBUF_REG{
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
} V1290N_OUTBUF_REG;


class CAENVMEV1290N:public CAENVMEModule{
	public:
		CAENVMEV1290N(){};
		~CAENVMEV1290N(){};
		// OpCodes //
		int WriteMICRORegister(int nw, unsigned short * data);
		uint16_t ReadMICRORegister();
		
		// 		
		//TDC Settings//
		void SetTriggerMode(int mode); 
		//void ReadTDCMode();
		void SetWindowWidth( unsigned short width);
		void SetWindowOffset( unsigned short offset);
		void SetRejectionMargin( unsigned short margin);
		void SetExtraSearchWindow( unsigned short margin);	
		
//		void EnableAllChannels();
//		void SetDeadTime( int width);
		//Data Modes
		void SetMultiplicity( unsigned short mt);
		void SetDetectionEdge(int conf);
		void SetHeaderTrailer(bool flag);
		void SetErrorMark(bool flag);
		void SetTimeTagSubtraction(bool flag);
		void SetBLTNev(int nev);


		// Operations
		void Clear();
		void Reset();
		uint16_t ReadControlRegister();
		uint16_t GetStatRegister();
		uint16_t IsReady();
		uint32_t GetEventNumber();	
};





// TDC Window


uint16_t ReadTSubStatus(int32_t handle, uint32_t addr);
void ReadEdgeStatus(int32_t handle, uint32_t addr);
void ReadHTStatus(int32_t handle, uint32_t addr);
uint16_t ReadEventFIFOStatus(int32_t handle, uint32_t addr);
void EnableEventFIFO(int32_t handle, uint32_t addr);
uint16_t ReadStoredEvents(int32_t handle, uint32_t addr);
void SetStoredEvents(int32_t handle, uint32_t addr,uint16_t data);

// V1290N OPCODEs //
enum V1290N_OPCODE{
	cv1290TrgMatch = 0x0000,
	cv1290ContStor = 0x0100,
	cv1290ReadAcqMod = 0x0200,
	cv1290MICROReg = 0x102E,
	cv1290HandShake=0x1030,
	cv1290TDCWidth = 0x1000,
	cv1290TDCOffset = 0x01100,
	cv1290TDCExtra = 0x01200,
	cv1290TDCRejMargin = 0x01300,
	cv1290EnSubTrg = 0x1400,//Enables trigger time tag subtraction
	cv1290DisSubTrg = 0x1500,//Disables trigger time tag subtraciton;
	cv1290ReadSubTrg = 0x1600,//Disables trigger time tag subtraciton;
	cv1290SetEdge = 0x2200,
	cv1290ReadEdge = 0x2300,
	cv1290SetDeadTime = 0x2800,
	cv1290EnHeadTrail = 0x3000,
	cv1290DisHeadTrail = 0x3100,
	cv1290ReadHTStatus = 0x3200,
	cv1290SetMultiplicity = 0x3300,

	cv1290EnErrMrk = 0x3500,
	cv1290DisErrMrk = 0x3600,

	cv1290EnableCh = 0x4000,
	cv1290DisableCh = 0x4100,
	cv1290EnableAllCh = 0x4000,
};


// V1290N Address Mab structure
// Can be access via BASE_ADDRESS + OFFSET_ADDRESS
// all register contents are described as
// description, access type(read-only, read/write...), access mode
enum V1290N_ADDR{
	cv1290Buffer = 0x0000,
	cv1290ControlReg = 0x1000,
	cv1290StatReg		  = 0x1002,// Status Register, ro, D16
	cv1290IrLvl				= 0x100A,// Interrupt Level, r/w, D16
	cv1290IrVec        = 0x100C,// Interrupt Vector, r/w, D16
	cv1290AddrHigh		  = 0x1012,// ADER High, r/w, D16
	cv1290ModuleReset			= 0x1014,//  wo, D16
	cv1290Clear				= 0x1016,//   Software Clear, wo, D16
	cv1290SWTrigger			= 0x101A,// Software Trigger wo, D16
	cv1290EvtCnt			= 0x101C,// Event Counter ro, D16
	cv1290EvtStored	  = 0x1020,// Event Stored ro,Number of Evt in buffer, D16
	cv1290AlmostFullLevel	  = 0x1022,
	cv1290BLTEvtNumber	  = 0x1024,
	cv1290FIFOStat		= 0x103E,// Event FIFO Status Register, ro, D16


	cv1290IncEvt			  = 0x1028,// Increment Event, wo, D16
	cv1290Controller			  = 0x1030,// Increment Event, wo, D16
	cv1290IncOff			  = 0x102A,// Increment Offset, wo, D16
	cv1290LdTestReg		= 0x102C,// Load Test Register, r/w, D16
	cv1290FclrWindow	  = 0x102E,// Fast Clear Window, r/w, D16
	cv1290BitSet2			= 0x1032,// Bit Set 2, r/w, D16
	cv1290BitClr2			= 0x1034,// Bit Clear 2, wo, D16
	cv1290WmemTestAddr = 0x1036,// W Memory Test Address, wo, D16
	cv1290MemTestWrdH	= 0x1038,// Memory Test Word_High, wo, D16
	cv1290MemTestWrdL	= 0x103A,// Memory Test Word_Low, wo, D16
	cv1290CrateSel		  = 0x103C,// Crate Selection, r/w, D16
	cv1290EvtCntRst		= 0x1040,// Event Counter Reset, wo, D16
	cv1290Fsr				  = 0x1060,// Fulll Scale Range, r/w, D16
	cv1290RtestAddr		= 0x1064,// R Test Address, wo, D16
	cv1290SwComm			  = 0x1068,// SW Comm, wo, D16
	cv1290AAD					= 0x1070,// ADD, ro, D16
	cv1290BAD					= 0x1072,// BAD, ro, D16
	cv1290Thresh			  = 0x1080,// Thresholds, r/w, D16
	// ROM address map(all read-only)
	cv1290OuiMsb       = 0x8026,// Manufacturer Identifier
	cv1290Oui          = 0x802A,// Manufacturer Identifier
	cv1290OuiLsb       = 0x802E,// Manufacturer Identifier
	cv1290Version      = 0x4030,// Purchased Version of the Mod.V792
	cv1290BrdIdMsb     = 0x4034,// Board Identifier(V792)
	cv1290BrdId        = 0x4038,// Board Identifier(V792)
	cv1290BrdIdLsb     = 0x403C,// Board Identifier(V792)
	cv1290Revision     = 0x804E,// Hardware Revision Identifier
	cv1290SerialMsb    = 0x4080,// Serial Number
	cv1290SerialLsb    = 0x4084,// Serial Number
};

#endif // __CAENVMEV792_H

