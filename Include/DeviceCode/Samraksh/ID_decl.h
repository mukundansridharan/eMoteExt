#ifndef _DRIVERS_ID_DECL_H_
#define _DRIVERS_ID_DECL_H_ 1


/**
 * @file: ID_decl.h
 * @author: Michael Andrew McGrath
 * @copyright: The Samraksh Company, 2014
 * @brief: device-specific HAL functions for obtaining unique ID/serial number.
 * @detail: pal\configuration\reduc\ConfigHelper.cpp:GetHalSystemInfo() calls ID_GetSerialNumbers().
 */

unsigned int ID_GetSerialNumbers(struct OEM_SERIAL_NUMBERS& OemConfig );




#endif
