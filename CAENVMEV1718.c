/*!
        -----------------------------------------------------------------------------

        -----------------------------------------------------------------------------

        CAENVMEV1718.c

        -----------------------------------------------------------------------------

        Created: January 2021
        Hyunmin Yang, HANUL, korea Univerisity
        Wrapping of CAENVMElib for V1718 USB-VME bridge
        Only for Linux
        -----------------------------------------------------------------------------
*/

#include "CAENVMEV1718.h"

void PrintErrInfo(const char *errLocation, const char *errFunc,
  const char *msg, CAENVME_API err)
{
  printf("In %s, an error occurred when calling %s.\n", errLocation, errFunc);
  if(msg != NULL)
    printf("%s\n", msg);
  printf("Error Type : %s\n", CAENVME_DecodeError(err));
}



CAENVME_API CvInit(short bdNum, int32_t *handle)
{
  CAENVME_API ret = CAENVME_Init(cvV1718, 0, bdNum, handle);
  if (ret != cvSuccess)
    PrintErrInfo("CvInit", "CAENVME_Init", "Failed to open V1718.", ret);
  else
  {
    ret = CAENVME_SystemReset(*handle);
    if (ret != cvSuccess)
      PrintErrInfo("CvReset", "CAENVME_SystemReset", "Failed to reset V1718.", ret);
  	else
			printf("Initialized\n");
	}
  return ret;
}

CAENVME_API CvClose(int32_t handle)
{
  CAENVME_API ret = CAENVME_End(handle);
  if (ret != cvSuccess)
    PrintErrInfo("CvClose", "CAENVME_End", "Failed to close V1718.", ret);;
  return ret;
}

uint32_t CvRead32(int32_t handle, uint32_t Addr)
{
  uint32_t data = 0x00000000;
  CAENVME_API ret = CAENVME_ReadCycle(handle, Addr, &data, cvA32_U_DATA, cvD32);
;;
//char* FWRel;
//	CAENVME_BoardFWRelease(handle,FWRel);
//	printf("%s\n",FWRel);
	if (ret != cvSuccess)
  {
    char msg[256];
    sprintf(msg, "Failed to read the register at address 0x%08X.", (uint32_t)(Addr));
    PrintErrInfo("CvRead32", "CAENVME_ReadCycle", msg, ret);
  }
  return data;
}

uint16_t CvRead16(int32_t handle, uint32_t Addr)
{
  uint16_t data = 0x0000;
  CAENVME_API ret = CAENVME_ReadCycle(handle, Addr, &data, cvA32_U_DATA, cvD16);
  if (ret != cvSuccess)
  {
    char msg[256];
    sprintf(msg, "Failed to read the register at address 0x%08X.", (uint32_t)(Addr));
    PrintErrInfo("CvRead16", "CAENVME_ReadCycle", msg, ret);
  	printf("Error Code: %d\n",ret);
	}
  return data;
}

int CvReadBLT(int32_t handle, uint32_t addr, int size, uint32_t* buff){
  int count;
	CAENVME_API ret = CAENVME_BLTReadCycle(handle, addr, (unsigned char*)buff, size, cvA32_U_MBLT, cvD64, &count);
  if (ret != cvSuccess)
  {
    char msg[256];
    sprintf(msg, "Failed to read the register at address 0x%08X.", (uint32_t)(addr));
    PrintErrInfo("CvReadBLT", "CAENVME_ReadCycle", msg, ret);
  }
  return count;
}

uint32_t CvRead32Swapped(int32_t handle, uint32_t Addr)
{
  uint32_t data = 0x00000000;
  CAENVME_API ret = CAENVME_ReadCycle(handle, Addr, &data, cvA32_U_DATA, cvD32_swapped);
  if (ret != cvSuccess)
  {
    char msg[256];
    sprintf(msg, "Failed to read the register at address 0x%08X.", (uint32_t)(Addr));
    PrintErrInfo("CvRead32Swapped", "CAENVME_ReadCycle", msg, ret);
  }
  return data;
}

uint16_t CvRead16Swapped(int32_t handle, uint32_t Addr)
{
  uint16_t data = 0x0000;
  CAENVME_API ret = CAENVME_ReadCycle(handle, Addr, &data, cvA32_U_DATA, cvD16_swapped);
  if (ret != cvSuccess)
  {
    char msg[256];
    sprintf(msg, "Failed to read the register at address 0x%08X.", (uint32_t)(Addr));
    PrintErrInfo("CvRead16Swapped", "CAENVME_ReadCycle", msg, ret);
  }
  return data;
}

CAENVME_API CvWrite32(int32_t handle, uint32_t Addr, uint32_t data)
{
  CAENVME_API ret = CAENVME_WriteCycle(handle, Addr, &data, cvA32_U_DATA, cvD32);
  if (ret != cvSuccess)
  {
    char msg[256];
    sprintf(msg, "Failed to write the register at address 0x%08X.", (uint32_t)(Addr));
    PrintErrInfo("CvWrite32", "CAENVME_WriteCycle", msg, ret);
  }
  return ret;
}

CAENVME_API CvWrite16(int32_t handle, uint32_t Addr, uint16_t data)
{
  CAENVME_API ret = CAENVME_WriteCycle(handle, Addr, &data, cvA32_U_DATA, cvD16);
  if (ret != cvSuccess)
  {
    char msg[256];
    sprintf(msg, "Failed to write the register at address 0x%08X.", (uint32_t)(Addr));
    PrintErrInfo("CvWrite16", "CAENVME_WriteCycle", msg, ret);
  }
  return ret;
}

CAENVME_API CvWrite32Swap(int32_t handle, uint32_t Addr, uint32_t data)
{
  CAENVME_API ret = CAENVME_WriteCycle(handle, Addr, &data, cvA32_U_DATA, cvD32_swapped);
  if (ret != cvSuccess)
  {
    char msg[256];
    sprintf(msg, "Failed to write the register at address 0x%08X.", (uint32_t)(Addr));
    PrintErrInfo("CvWrite32Swap", "CAENVME_WriteCycle", msg, ret);
  }
  return ret;
}

CAENVME_API CvWrite16Swap(int32_t handle, uint32_t Addr, uint16_t data)
{
  CAENVME_API ret = CAENVME_WriteCycle(handle, Addr, &data, cvA32_U_DATA, cvD16_swapped);
  if (ret != cvSuccess)
  {
    char msg[256];
    sprintf(msg, "Failed to write the register at address 0x%08X.", (uint32_t)(Addr));
    PrintErrInfo("CvWrite16Swap", "CAENVME_WriteCycle", msg, ret);
  }
  return ret;
}






void CvStartPulser(int32_t handle, CVPulserSelect pulSel, uint8_t period, uint8_t width)
{
  CAENVME_API ret =
    CAENVME_SetPulserConf(handle, pulSel, period, width, cvUnit25ns, 1, cvManualSW, cvManualSW);
  if(ret != cvSuccess)
    PrintErrInfo("CvStartPulser", "CAENVME_SetPulserConf", NULL, ret);
  ret = CAENVME_StartPulser(handle, pulSel);
  if(ret != cvSuccess)
    PrintErrInfo("CvStartPulser", "CAENVME_StartPulser", NULL, ret);
}

void CvStopPulser(int32_t handle, CVPulserSelect pulSel)
{
  CAENVME_API ret = CAENVME_StopPulser(handle, pulSel);
  if(ret != cvSuccess)
    PrintErrInfo("CvStopPulser", "CAENVME_StopPulser", NULL, ret);
}



