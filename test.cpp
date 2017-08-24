// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <sys/time.h>
//#include <pthread.h>
//#include <iostream>
//#include <fstream>
//#include <dlfcn.h>
//#include <vector>
//#include <getopt.h>
#include <MORPHO_Types.h>
#include "MORPHO_Interface.h"
#include "sample_sdk.h"

static UC g_uc_ConnectionType = 0;

I openDeviceOperation(PT_DATA io_px_data);

main(void)
{
	T_DATA l_x_data;
	openDeviceOperation(&l_x_data);
    return 0;
}

I openDeviceOperation(PT_DATA io_px_data)
{
	I			l_i_Ret;
	UL 			l_ul_NbUsbDevice;
	UL			l_ul_index;
	PC			l_pc_deviceName;
	PC			l_pc_deviceProperties;

	
	l_i_Ret=io_px_data->m_x_device.InitUsbDevicesNameEnum(&l_ul_NbUsbDevice);
	fprintf(stdout,"returned %d\n",l_i_Ret);

	if(l_ul_NbUsbDevice)
	{
		fprintf(stdout,"Detected %u USB devices :\n",l_ul_NbUsbDevice);
	}

	for (l_ul_index=0;l_ul_index < l_ul_NbUsbDevice;l_ul_index++)
	{
		l_i_Ret = io_px_data->m_x_device.GetUsbDevicesNameEnum(l_ul_index,l_pc_deviceName, l_pc_deviceProperties);
		fprintf(stdout, "%u - %s - %s\n", l_ul_index, l_pc_deviceName, l_pc_deviceProperties);
	}
}

