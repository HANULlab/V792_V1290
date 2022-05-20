/*!
        -----------------------------------------------------------------------------

        -----------------------------------------------------------------------------

        CAENVMEV1718.h

        -----------------------------------------------------------------------------

        Created: January 2021
        Hyunmin Yang, HANUL, korea Univerisity
        Wrapping of CAENVMElib for V1718 USB-VME bridge
        Only for Linux
				
				Modified: April 2022
				Byungmin Kang, HANUL, Korea Universty
				Functions for V1290 MT-TDC included.

        -----------------------------------------------------------------------------
*/
#ifndef __CAENVMEV1718_H
#define __CAENVMEV1718_H

#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include "CAENVMElib.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#ifdef LINUX

	
	
	/*
        CvInit
        -----------------------------------------------------------------------------
        Parameters:
                [in]  bdNum           : The slot index? of V1718 controller
                [out] handle          : The handle that identifies the device.
        -----------------------------------------------------------------------------
        Returns:
                CAENVNME_API containing the result of a command
        -----------------------------------------------------------------------------
        Description:
                Get the device handler of V1718 module.
*/

CAENVME_API CvInit(short bdNum, int32_t *handle);

/*
        CvClose
        -----------------------------------------------------------------------------
        Parameters:
                [in] handle           : The handle that identifies the device.
        -----------------------------------------------------------------------------
        Returns:
                CAENVNME_API containing the result of a command
        -----------------------------------------------------------------------------
        Description:
                Close the controller and free all allocated memory.
*/

CAENVME_API CvClose(int32_t handle);

/*
        CvRead32
        -----------------------------------------------------------------------------
        Parameters:
                [in]  Handle          : The handle that identifies the device.
                [in]  Addr            : Address to access a memory.
                [out] data            : 32 bit data to be read out
        -----------------------------------------------------------------------------
        Returns:
                CAENVNME_API containing the result of a command
        -----------------------------------------------------------------------------
        Description:
                Read data in D32 mode.
*/

uint32_t CvRead32(int32_t handle, uint32_t addr);

/*
        CvRead16
        -----------------------------------------------------------------------------
        Parameters:
                [in]  Handle          : The handle that identifies the device.
                [in]  Addr            : Address to access a memory.
                [out] data            : 32 bit data to be read out
        -----------------------------------------------------------------------------
        Returns:
                CAENVNME_API containing the result of a command
        -----------------------------------------------------------------------------
        Description:
                Read data in D16 mode.
*/

uint16_t CvRead16(int32_t handle, uint32_t addr);

/*
        CvRead32Swapped
        -----------------------------------------------------------------------------
        Parameters:
                [in]  Handle          : The handle that identifies the device.
                [in]  Addr            : Address to access a memory.
                [out] data            : 32 bit data to be read out
        -----------------------------------------------------------------------------
        Returns:
                CAENVNME_API containing the result of a command
        -----------------------------------------------------------------------------
        Description:
                Read data in D32 mode with endian reversed.
*/

uint32_t CvRead32Swapped(int32_t handle, uint32_t addr);

/*
        CvRead16Swapped
        -----------------------------------------------------------------------------
        Parameters:
                [in]  Handle          : The handle that identifies the device.
                [in]  Addr            : Address to access a memory.
                [out] data            : 32 bit data to be read out
        -----------------------------------------------------------------------------
        Returns:
                CAENVNME_API containing the result of a command
        -----------------------------------------------------------------------------
        Description:
                Read data in D16 mode with endian reversed.
*/

uint16_t CvRead16Swapped(int32_t handle, uint32_t addr);

/*
        CvWrite32
        -----------------------------------------------------------------------------
        Parameters:
                [in]  Handle          : The handle that identifies the device.
                [in]  Addr            : Address to access a memory.
                [out] data            : 16 bit data to be read out
        -----------------------------------------------------------------------------
        Returns:
                CAENVNME_API containing the result of a command
        -----------------------------------------------------------------------------
        Description:
                Wirte data by 4 bytes.
*/

CAENVME_API CvWrite32(int32_t handle, uint32_t addr, uint32_t data);

/*
        CvWrite16
        -----------------------------------------------------------------------------
        Parameters:
                [in]  Handle          : The handle that identifies the device.
                [in]  Data            : 32 bits data to be written
                [in]  Addr            : Address to access a memory.
        -----------------------------------------------------------------------------
        Returns:
                CAENVNME_API containing the result of a command
        -----------------------------------------------------------------------------
        Description:
                Write data by 2 bytes.
*/

CAENVME_API CvWrite16(int32_t handle, uint32_t addr, uint16_t data);

/*
        CvWrite32Swapped
        -----------------------------------------------------------------------------
        Parameters:
                [in]  Handle          : The handle that identifies the device.
                [in]  Addr            : Address to access a memory.
                [out] data            : 16 bit data to be read out
        -----------------------------------------------------------------------------
        Returns:
                CAENVNME_API containing the result of a command
        -----------------------------------------------------------------------------
        Description:
                Wirte data by 4 bytes with endian reversed.
*/

CAENVME_API CvWrite32Swapped(int32_t handle, uint32_t addr, uint32_t data);

/*
        CvWrite16Swapped
        -----------------------------------------------------------------------------
        Parameters:
                [in]  Handle          : The handle that identifies the device.
                [in]  Data            : 32 bits data to be written
                [in]  Addr            : Address to access a memory.
        -----------------------------------------------------------------------------
        Returns:
                CAENVNME_API containing the result of a command
        -----------------------------------------------------------------------------
        Description:
                Write data by 2 bytes with endian reversed.
*/

CAENVME_API CvWrite16Swapped(int32_t handle, uint32_t addr, uint16_t data);

int CvReadBLT(int32_t handle, uint32_t addr, int size, uint32_t* buff);

	/*
        CvStartPulser
        -----------------------------------------------------------------------------
        Parameters:
                [in]  Handle          : The handle that identifies the device.
                [in]  Pulser to start : Select pulseA or pulseB (CVPulserSelect enum)
                [in]  Period          : Pulse Period in an unit of 25 ns. 
                [in]  Width           : Pulse width in an unit of 25 ns.
        -----------------------------------------------------------------------------
        Returns: 
        -----------------------------------------------------------------------------
        Description:
                Configure a selected pulser and Start generating the pulse
                If this function has been called,
                the configured pulser can be started or stopped
                only by a software control.
*/

void CvStartPulser(int32_t handle, CVPulserSelect pulSel, uint8_t period, uint8_t width);

/*
        CvStopPulser
        -----------------------------------------------------------------------------
        Parameters:
                [in]  Handle          : The handle that identifies the device.
                [in]  Pulser to stop  : Select pulseA or pulseB (CVPulserSelect enum)
        -----------------------------------------------------------------------------
        Returns: 
        -----------------------------------------------------------------------------
        Description:
                Stop the pulse.
*/

void CvStopPulser(int32_t handle, CVPulserSelect pulSel);



//For V1290




#endif // LINUX

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __CAENVMEV1718_H

