/*!
	-----------------------------------------------------------------------------

	-----------------------------------------------------------------------------

	daq.c

	-----------------------------------------------------------------------------

Created: January 2021
Hyunmin Yang, HANUL, korea Univerisity
	Simple DAQ program of V792N controlled by V1718 USB-Bridge interface.
Modified: May 2022
Byungmin Kang, HANUL, korea university
	V1290 TDC control is available
	data is read by block to accelate readout.

-----------------------------------------------------------------------------
*/
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include "CAENVMEV1718.h"
#include "CAENVMEV792N.h"
#include "CAENVMEV1290N.h"
#include "CAENVMElib.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <bitset>
#include <vector>
#include <chrono>
/* #include "CAENVMEoslib.h"
#include "CAENVMEtypes.h
*/
using namespace std;
using namespace chrono;
// global variables for a controller and modules
int32_t ctlHdl;                    // controller handler
short ctlIdx = 0;                  // controller slot index???

int min_int(int a,int b){
	if(a>b){
		return b;
	}
	else{
		return a;
	}
}

// definition of base addresses
const uint32_t qdcAddr1 = 0x20000000;    // base address of first QDC
const uint32_t tdcAddr1 = 0x11000000;    // base address of first TDC
const uint16_t BLTAddress = 0xAA;
// channel number
const uint8_t nQdcCh1 = 16; 
const uint8_t nTdcCh1 = 16; 

CVPulserSelect pulser = cvPulserB;
uint8_t Pulse_period = 0xff;
uint8_t Pulse_width  = 0xff; 

const int timeOut = 0;             // zero if no time out limit(in ms)  
struct timeval tStart, tStop; 
bool Force_flag = true;

// to deal with ctrl + c signal
// When ctrl + c is pressed to kill the process, IntHandler gets called
// instead of killing the process. 
bool isQuit = false;
void IntHandler(int sig)
{
	printf("Quiting the program...\n");
	isQuit = true;
}


void InitModules();
bool WaitModules();
void ClearModules();
void ResetTDC();
void PrintSummary(struct timeval tStart, struct timeval tStop, int nEvt);
int nEvt = 10000;
const int nv1290=10;
const int nv792=10;
CAENVMEV1290N v1290[nv1290];
CAENVMEV792N v792[nv792];


int main(int argc, char **argv)
{

	switch(argc){
		case 1:
			{
				cout<<"Usage: ./daq Filename Nevents"<<endl;
				exit(1);
				break;
			}
		case 2:
			{
				cout<<"Nevents set to 10000"<<endl;
				break;
			}
		case 3:
			{
				nEvt = atoi(argv[2]);
				break;
			}
		defalut:
			{
				cout<<"Too much arguments; others will be neglected"<<endl;
				break;
			}
	}


	// CvClose must be called before the end of the program!!!
	// interrupt signal(ctrl + c)
	signal(SIGINT, IntHandler);
	// initiating V1718
	if (CvInit(ctlIdx, &ctlHdl) != cvSuccess)
		exit(0);
	// Initializing modules
	v792[0].SetAddress(ctlHdl,qdcAddr1);
	v1290[0].SetAddress(ctlHdl,tdcAddr1);
	InitModules();
	printf("Modules Initialized\n"); 
	// acqusition loop
	// gettimeofday(&tStart, NULL);
	ClearModules();
	//	ResetTDC();
	int32_t qdc1[16];
	int32_t tdc1[16];
	uint32_t qdcBuf1;
	uint32_t tdcBuf1;
	const int QDCBlockSize= 128;//int 32 casted by char. Actual number of int32 blocks should be divided by 4.
	uint32_t QDCBlock[QDCBlockSize/4];//Block will be casted to uchar, but this should be uint32_t, for data interpretation.

	const int TDCBlockSize= 1024;
//	const int TDCBlockSize= 2048;
//	const int TDCBlockSize= 4096;
	uint32_t TDCBlock[TDCBlockSize/4];

	FILE* fp;
	std::string filename = argv[1];
	string dir = "./dats/";
	filename=dir+filename;
	fp=fopen(filename.data(),"wb+");
	std::cout<<"File Created : "<<filename<<std::endl;
	FILE* fp_time;
	filename=dir+"_timing";
	fp_time=fopen(filename.data(),"wb+");
	int32_t fheader = 0xffffffff;
	int32_t ffooter = 0xfffffffe;

	printf("********************************************************************************************\n");
	printf("************************************ Start of DAQ loop *************************************\n");
	printf("********************************************************************************************\n");
	int32_t QDCevt_id=0,TDCevt_id=0;
	int nerr = 0; 

//	CvWrite16(ctlHdl,ctlHdl+0x0C,0xf801);
	for(int32_t i = 0;i < nEvt;i++)//loop
	{
		if(isQuit)
			break;
		while(i==0){
			ClearModules();
			uint16_t tdc_clear=(uint16_t)v1290[0].ReadStoredEvents();
			if(tdc_clear==0){
				std::cout<<"Cleared "<<std::endl;
				break;
			}
			else{
				std::cout<<"Clearing..."<<std::endl;
				std::cout<<"TDC nev = "<<tdc_clear<<std::endl;
			}	
			if(isQuit){
				return false;
			} 
		}

		if(!WaitModules())
		{
			printf("WaitModules Failure!\n");
			break;
		}
		system_clock::time_point start_time = system_clock::now(); 
		// Output Register controll to trigger veto logic

		// QDC1 module data
		for(int j = 0;j < nQdcCh1;j++)
		{
			qdc1[j] = -9999;
		}
		for(int j = 0;j < nTdcCh1;j++)
		{
			tdc1[j] = -9999;
		}
		
//		CvStartPulser(ctlHdl,pulser,Pulse_period,Pulse_width);
	

		int nbq = v792[0].ReadBLT(QDCBlockSize,QDCBlock);//Block Level Transfer
		system_clock::time_point QDCread_time = system_clock::now(); 
		int nbt = v1290[0].ReadBLT(TDCBlockSize,TDCBlock);
		system_clock::time_point TDCread_time = system_clock::now(); 
//		CvStopPulser(ctlHdl,pulser);
//		cout<<"QDC nb : " <<nb<<endl;
		for(int j = 0;j < nQdcCh1 +2;j++) // header + 16 channels + EOB
		{
			qdcBuf1 = QDCBlock[j];
			switch((qdcBuf1>>24) & 0x07) // check output buffer type
			{
				case 0x04: // EOB endofblock
					{
						QDCevt_id = (qdcBuf1&0xFFFFFF);
					}
					break;
				case 0x02: // header
					{
						//						std::cout<<"Header"<<std::endl;
						break;
					}
				case 0x00: // valid data
					{
						int chan = (qdcBuf1 >> 17)&0xf; 
						qdc1[chan] = (int32_t)(qdcBuf1&0xfff); 
//					std::cout<<"QDC Hit on CH : "<<chan<<" Value: "<<qdc1[chan]<<std::endl;
						break;
					}
				case 0x06: // filler
					{
						//						std::cout<<"Filler"<<std::endl;
						break;
					}
				default:
					{
						printf("Warning, an invalid QDC datum has been detected!\n");
					}
			}
		}
		//		std::cout<<"ADC Read"<<std::endl;
		bool TDC_events = true;
		//	for(int j = 0;j < 15;j++) // 16 channels 
		int j=0;
		int bunch_id = 0;
		while(TDC_events&&j<255)
		{
//			for(int k=0;k<32;k++){
		//		std::bitset<32> x(TDCBlock[k]);
//			}
			tdcBuf1=TDCBlock[j];	
			j++;
			switch(((tdcBuf1>>27)&(0x1f))  )// check output buffer type
			{
				case 0x08: //Global Header
					{
						//						std::cout<<"GH"<<std::endl;
						TDCevt_id = ((tdcBuf1>>5)&0x3fffff);
						break;
					}
				case 0x01: // Header
					{
						//						std::cout<<"TDC Header"<<std::endl;
						//						TDCevt_id = ((tdcBuf1>>12)&0xfff);
						bunch_id = (tdcBuf1&0xfff);
						//						std::cout<<"Bunch ID : "<<bunch_id<<std::endl;
						break;
					}
				case 0x03: // Trailer
					{
						//						int evt_id = ((tdcBuf1>>12)&0xfff);
						int wdcnt = (tdcBuf1&0xfff);
						break;
					}
				case 0x00: // valid data
					{
						int chan = (int)((tdcBuf1 >> 21)&0xf);
						//	int tdc = (int32_t)(tdcBuf1&0x1fffff) - bunch_id;	
						int tdc = (int32_t)(tdcBuf1&0x1fffff);	
						if(tdc1[chan]==-9999){
							tdc1[chan]=tdc;
			//										std::cout<<"TDC Hit on CH : "<<chan<<" Value: "<<tdc1[chan]<<std::endl;
						}
						else{
														std::cout<<"TDC MT Hit on CH : "<<chan<<std::endl;
						}
						break;
					}
				case 0x04: // Error
					{
						int err_flag = (tdcBuf1&0x3fff);
						std::bitset<15> eb(err_flag);
						std::cout<<"TDC Error : "<<eb<<std::endl;
						break;
					}
				case 0x11: //ETTT 
					{
						unsigned int ETTT = (tdcBuf1&0x7ffffff);
						break;
					}
				case 0x10: //Output Buf Trailer 
					{
						unsigned short wdcnt = ((tdcBuf1>>5)&0xffff);
						TDC_events = false;
						break;
					}
				case 0x18: //dummy 
					{
						TDC_events = false;
						std::cout<<"Dummy buf"<<std::endl;
						break;
					}
				default:
					{
						//						printf("Warning, an invalid TDC datum has been detected!\n");
					}
					if(j==254){
						std::cout<<"Warning: TDC loop reached 255"<<std::endl;
					}
			}//Switch
		}//while(TDC_events)
		int nev = i;
		if(TDCevt_id!=QDCevt_id){
				nerr++;	
//				cout<<"Warning! TDCevt : "<<TDCevt_id<<" QDCevt : "<<QDCevt_id<<endl;
//				usleep(1);
				ClearModules();
				i--;
				continue;
		}
		if(i%100==0){
			std::cout<<i<<" th evt"<<std::endl;
			std::cout<<"TDC Evt: "<<TDCevt_id<<std::endl;
			std::cout<<"QDC Evt: "<<QDCevt_id<<std::endl;
			std::cout<<"MissMatch : "<<nerr<<std::endl;
		}
		system_clock::time_point end_time = system_clock::now(); 
		fwrite(&fheader,sizeof (int32_t),1, fp);
		fwrite(&fheader,sizeof (int32_t),1, fp_time);
		fwrite(&i,sizeof (int32_t),1, fp);
		int32_t QDCHeader = 0xFFFFFFF0;
		fwrite(&QDCHeader,sizeof (int32_t),1, fp);
		fwrite(&QDCevt_id,sizeof (int32_t),1, fp);
		for(int32_t nch=0;nch<16;nch++){
			fwrite(&qdc1[nch],sizeof (int32_t),1, fp);
		}
		int32_t TDCHeader = 0xFFFFFFF1;
		fwrite(&TDCHeader,sizeof (int32_t),1, fp);
		fwrite(&TDCevt_id,sizeof (int32_t),1, fp);
		for(int32_t nch=0;nch<16;nch++){
			fwrite(&tdc1[nch],sizeof (int32_t),1, fp);
		}
		fwrite(&ffooter,sizeof (int32_t),1, fp);
		fwrite(&ffooter,sizeof (int32_t),1, fp_time);
		system_clock::time_point write_time = system_clock::now();
		nanoseconds qdcreading = QDCread_time-start_time;
		nanoseconds tdcreading = TDCread_time-start_time;
		nanoseconds ending = end_time-start_time;
		nanoseconds writing = write_time-start_time;
		uint32_t qt = qdcreading.count();
		uint32_t tt= tdcreading.count();
		uint32_t et= ending.count();
		uint32_t wt= writing.count();
		fwrite(&qt,sizeof (int32_t),1, fp_time);
		fwrite(&tt,sizeof (int32_t),1, fp_time);
		fwrite(&et,sizeof (int32_t),1, fp_time);
		fwrite(&wt,sizeof (int32_t),1, fp_time);
/*
		cout<<"Elapsed_Time"<<endl;
		cout<<"ReadingQDC : " <<qt<<" ns"<<endl;
		cout<<"ReadingTDC : " <<tt<<" ns"<<endl;
		cout<<"Ending : " <<et<<" ns"<<endl;
		cout<<"Writing : " <<wt<<" ns"<<endl;
	*/
		//ClearModules();
		//			std::cout<<"Time Difference : "<<(double)(tdc1[2]-tdc1[3])*0.025<<std::endl;
		//			std::cout<<"QDC : "<<(double)(qdc1[2])<<std::endl;
		//		std::cout<<"End Of Event "<<i<<" QDCID: "<< QDCevt_id<<" TDCID: "<<TDCevt_id <<std::endl;
	}
	int32_t feof = 0xfffffffd;
	fwrite(&feof,sizeof (int32_t),1, fp);
	fwrite(&feof,sizeof (int32_t),1, fp_time);
	std::cout<<"MissMatch : "<<nerr<<std::endl;
	printf("********************************************************************************************\n");
	printf("************************************ End of DAQ loop ***************************************\n");
	printf("************************************ Summary         ***************************************\n");
	//  gettimeofday(&tStop, NULL);
	// PrintSummary(tStart, tStop, nEvt);
	printf("********************************************************************************************\n");
	// closing V1718
	ClearModules();
	if (CvClose(ctlHdl) != cvSuccess)
		exit(0);
	fclose(fp);
	fclose(fp_time);
	std::cout<<"File Closed : "<<filename<<std::endl;
	return 0;
}
//inni
void InitModules()
{
	// TDC initialization
	unsigned short TriggerOffset = -9;//In units of 25 ns.
	unsigned short TriggerWindow = 9;//In units of 25 ns.
	//				unsigned short code[2];
	//				code[0]=cv1290TrgMatch;
	std::cout<<"Setting TDC Mode"<<std::endl;
	v1290[0].SetTriggerMode(cv1290TrgMatch);
//	v1290[0].SetTriggerMode(cv1290ContStor);
	v1290[0].SetWindowWidth(TriggerWindow);
	v1290[0].SetWindowOffset(TriggerOffset);
	v1290[0].SetRejectionMargin((unsigned short)0x27);
	v1290[0].SetExtraSearchWindow((unsigned short)0x01);
	v1290[0].SetTimeTagSubtraction(true);	
	unsigned short mt = 6;//0->0, 1->1... 4->8...	8->128, 9->no limit
	v1290[0].SetMultiplicity(mt);	
	v1290[0].SetBLTNev(1);	
	
	// QDC initialization
	v792[0].Write16(cv792BitSet1,0x0080);
	v792[0].Write16(cv792BitClr1,0x0080);
	v792[0].SetPedestal(0xD7);
	v792[0].Write16(cv792BitSet2,0x001C);
	v792[0].Write16(cv792CtlReg1,0x0004);
	std::cout<<"v792 Initialized"<<std::endl;
}

bool WaitModules()//gate
{
	struct timeval tPrev, tNew;
	uint16_t adcstat = 0;
	bool adcready = false;

	bool adcbusy =false;
	bool adctrg ;
	uint16_t tdcready = 0;
	while(true)
	{
		if(isQuit){
			ResetTDC();
			return false;
		} 
		adcstat = v792[0].GetStatRegister();
		adctrg = adcstat&0x0001;
		adcbusy = (adcstat>>2)&0x0001;
//		adcready = (!adcbusy && adctrg);	
		adcready = (adctrg&&!adcbusy);	
		tdcready = (v1290[0].IsReady());

		if(v1290[0].ReadStoredEvents()>31){
		}
//		cout<<"V792 Evts: "<<v1290[0].ReadStoredEvents()<<endl;

		if(adcbusy&&tdcready!=0x0001){
		}
		//adcready = (!adcbusy && adctrg);	
	
		if(adcready==0x0001&&tdcready==0x0001&&Force_flag)
			return true;
		else if(adcready!=0x0001&&tdcready==0x0001){
		}
	} 
}

void ClearModules()//clr
{
	v1290[0].Clear();
	CvWrite16(ctlHdl, qdcAddr1 + cv792BitSet2, 0x001C);
	usleep(1);
	CvWrite16(ctlHdl, qdcAddr1 + cv792BitClr2, 0x8004);
	v792[0].EventReset();
}
void ResetTDC(){
	v1290[0].Clear();
	v1290[0].Reset();
}


void PrintSummary(struct timeval tStart, struct timeval tStop, int nEvt)
{
	int msec = (int)(tStop.tv_usec - tStart.tv_usec)/1000;
	if(msec < 0)
		msec += 1000;
	int sec = tStop.tv_sec - tStart.tv_sec;
	if(tStop.tv_usec < tStart.tv_usec)
		sec--;
	int min = (int)(sec/60)%60;
	int h = (int)(sec/3600)%24;
	int day = (int)sec/(60*60*24);
	long long tDiffMs = 1000*sec + msec/1000;
	sec = sec%60; 
	printf("Elapsed Time : %02dD %02dH %02dM %02dS %03dmS\n", day, h, min, sec, msec);
	if(tDiffMs != 0)
	{
		double tRate = (double)1000.*nEvt/tDiffMs;
		if(tRate > 100)
			printf("Trigger Rate : %3.3f kHz\n", tRate/1000);
		else
			printf("Trigger Rate : %2.2f Hz\n", tRate);
	}
}
