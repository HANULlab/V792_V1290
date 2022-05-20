/*!
	-----------------------------------------------------------------------------

	-----------------------------------------------------------------------------

	CAENVMEV1290N.c

	-----------------------------------------------------------------------------

Created: April 2022
Byungmin Kang, HANUL, korea Univerisity
Functions for V1290N
-----------------------------------------------------------------------------
*/
#include "CAENVMEV1718.h"
#include "CAENVMEV1290N.h"
#include "unistd.h"
#include "tgmath.h"
#include <stdio.h>
#include <iostream>
#include <bitset>
unsigned short opcode[10];
using namespace std;

int CAENVMEV1290N::WriteMICRORegister(int nw, unsigned short* data){
	int timeout = 3000,timecnt=0;
	unsigned short hs;
	for(int i=0;i<nw;i++){
		do{
			hs = CvRead16(handle,addr+cv1290HandShake);
			timecnt++;
			usleep(1000);
			if(timecnt==timeout){ 
				printf("Warning:: Fail to write OPcode %d \n",data[i]);
				return 0;
			}
		}
		while(((hs&0x01)==0)&&(timecnt<timeout));
		CvWrite16(handle,addr+cv1290MICROReg,data[i]);
//		printf("OPcode %d Written %.4x, timecnt: %d \n",i,data[i],timecnt);
	}
	return 1;
}

uint16_t CAENVMEV1290N::ReadMICRORegister(){
	int timeout = 3000,timecnt=0;
	unsigned short hs;
		while(((hs&0x10)==0)&&(timecnt<timeout)){
			hs = CvRead16(handle,addr+cv1290HandShake);
			timecnt++;
			usleep(1000);
			if(timecnt==timeout){ 
				printf("Warning:: Fail to Read Register\n");
				return 0xffff;
			}
		}
		return CvRead16(handle,addr+cv1290MICROReg);
}



void CAENVMEV1290N::SetTriggerMode(int mode){
	opcode[0]=mode;
	if(!WriteMICRORegister(1,opcode)){
		std::cout<<"Waring: Failed to set Trigger mode. "<<std::endl;
		exit(1);
	};
}
void CAENVMEV1290N::SetWindowWidth( unsigned short width){
	opcode[0]=cv1290TDCWidth;
	opcode[1]=width;
	WriteMICRORegister(2,opcode);
}

void CAENVMEV1290N::SetWindowOffset( unsigned short offset){
	opcode[0]=cv1290TDCOffset;
	opcode[1]=offset;
	WriteMICRORegister(2,opcode);
}


void CAENVMEV1290N::SetExtraSearchWindow( unsigned short window){
	opcode[0]=cv1290TDCExtra;
	opcode[1]=window;
	WriteMICRORegister(2,opcode);
}
void CAENVMEV1290N::SetRejectionMargin( unsigned short margin){
	opcode[0]=cv1290TDCRejMargin;
	opcode[1]=margin;
	WriteMICRORegister(2,opcode);
}


void CAENVMEV1290N::SetMultiplicity(unsigned short mt){
	opcode[0]=cv1290SetMultiplicity;
	opcode[1]=0xFFFF&mt;
	WriteMICRORegister(2,opcode);
}

void CAENVMEV1290N::SetDetectionEdge(int conf){
	opcode[0]=cv1290SetEdge;
	switch(conf){
		case 0: {//Paired 
			opcode[1]=0x0000;
			break;
			}
		case 1: {//Leading 
			opcode[1]=0x0010;
			break;
			}
		case 2: {//Trailing
			opcode[1]=0x0001;
			break;
			}
		case 3: {//Leading And Trailing
			opcode[1]=0x0011;
			break;
			}
		default: {
			opcode[1]=0x0010;
			break;
			}
	}
	WriteMICRORegister(2,opcode);
}

void CAENVMEV1290N::SetHeaderTrailer(bool flag){
	if(flag){
		opcode[0]=cv1290EnHeadTrail;
	}
	else{
		opcode[0]=cv1290DisHeadTrail;
	}
	WriteMICRORegister(1,opcode);
}

void CAENVMEV1290N::SetErrorMark(bool flag){
	if(flag){
		opcode[0]=cv1290EnErrMrk;
	}
	else{
		opcode[0]=cv1290DisErrMrk;
	}
	WriteMICRORegister(1,opcode);

}

void CAENVMEV1290N::SetTimeTagSubtraction(bool flag){
	if(flag){
		opcode[0]=cv1290EnSubTrg;
	}
	else{
		opcode[0]=cv1290DisSubTrg;
	}
	WriteMICRORegister(1,opcode);
}

void CAENVMEV1290N::SetBLTNev(int nev){
	CvWrite16(handle, addr+cv1290BLTEvtNumber,nev);
}



void CAENVMEV1290N::Clear(){
	CvWrite16(handle,addr+cv1290Clear,0x0004);
}

void CAENVMEV1290N::Reset(){
	CvWrite16(handle,addr+cv1290ModuleReset,0x0004);
}

uint16_t CAENVMEV1290N::ReadControlRegister(){
	return (CvRead16(handle, addr + cv1290ControlReg));
}
uint16_t CAENVMEV1290N::GetStatRegister(){
	return (CvRead16(handle, addr + cv1290StatReg));
}
uint16_t CAENVMEV1290N::IsReady(){
	return (GetStatRegister())&0x01;
}





/*
void ReadTDCMode(int32_t handle, uint32_t addr){
	opcode[0]=cv1290ReadAcqMod;
//	int reg = CvWriteMICROReg(handle,addr,1,opcode);
//		return CvRead16(handle,addr+cv1290MICROReg);
//	std::bitset<16> bi(reg);
//	std::cout<<bi<<std::endl;
//	printf("TDC Mode reg : %.4X\n",reg);
//	return CvReadMICRORegister(handle, addr);
}
*/
/*
void SetDeadTime(int32_t handle, uint32_t addr, int deadtime){
	opcode[0]=cv1290SetDeadTime;
	if(deadtime<5){
		opcode[1]=0x0000;
	}
	else if(deadtime<10){
		opcode[1]=0x0001;
	}
	else if(deadtime<30){
		opcode[1]=0x0010;
	}
	else{
		opcode[1]=0x0011;
	}
	CvWriteMICROReg(handle,addr,2,opcode);
}
*/
uint32_t CAENVMEV1290N::GetEventNumber(){
	return CvRead32(handle, addr+cv1290EvtCnt);
}

uint16_t IsFIFOReady(int32_t handle, uint32_t addr){
	//  return (CvRead16(handle, addr + cv1290StatReg1))&0x0001;
	return (CvRead16(handle, addr + cv1290FIFOStat));
}

uint16_t ReadEventFIFOStatus(int32_t handle, uint32_t addr){
	return (CvRead16(handle, addr + cv1290FIFOStat));
}
void EnableEventFIFO(int32_t handle, uint32_t addr){
	uint16_t data= CvRead16(handle, addr + cv1290ControlReg);
	data = data | (0x0001 << 8 );
	//	uint16_t enfifo = 0x100;//100000000
	CvWrite16(handle, addr+cv1290ControlReg,data);
}
