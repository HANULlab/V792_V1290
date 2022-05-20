/*
Mother class for VME Modules.
*/
#include "CAENVMEV1718.h"
#ifndef VMEModule_h
#define VMEModule_h 1

enum V_ADDR{
	cvEventStored	  = 0x1020,// Event Stored ro,Number of Evt in buffer, D16
};

class CAENVMEModule{
	private: 
	public:
		int32_t handle;
		uint32_t addr;
		CAENVMEModule(){};
		CAENVMEModule(int32_t handle_, uint32_t addr_){
			handle=handle;addr=addr_;
		};
		void SetAddress(int32_t handle_, uint32_t addr_){
			handle=handle;addr=addr_;
		};
		uint32_t ReadOutputBuffer(){
			return CvRead32(handle,addr);
		};
		int ReadBLT(int size, uint32_t* buff){
			return CvReadBLT(handle,addr,size,buff);
		};
		uint16_t ReadStoredEvents(){
			return (CvRead16(handle, addr + cvEventStored))&0x1f;	
		};
};
#endif
