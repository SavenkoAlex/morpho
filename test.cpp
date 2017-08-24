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
	UL			l_ul_index=0;
	PC			l_pc_deviceName;
	PC			l_pc_deviceProperties;

	
	l_i_Ret=io_px_data->m_x_device.InitUsbDevicesNameEnum(&l_ul_NbUsbDevice);
	fprintf(stdout,"returned %d\n",l_i_Ret);

	if(l_ul_NbUsbDevice)
	{
		fprintf(stdout,"Detected %u USB devices \n",l_ul_NbUsbDevice);
	}

	if ((I)l_ul_NbUsbDevice>1)
	{
		fprintf(stdout,"Find more that one device-%u please use one",l_ul_NbUsbDevice);

		for (l_ul_index;l_ul_index < l_ul_NbUsbDevice;l_ul_index++)
		{
			l_i_Ret = io_px_data->m_x_device.GetUsbDevicesNameEnum(l_ul_index,l_pc_deviceName, l_pc_deviceProperties);
			fprintf(stdout, "%u - %s - %s\n", l_ul_index, l_pc_deviceName, l_pc_deviceProperties);
		}
	}

	else
	{
		l_i_Ret=io_px_data->m_x_device.GetUsbDevicesNameEnum((I)l_ul_index,l_pc_deviceName,l_pc_deviceProperties);
		if(l_i_Ret != MORPHO_OK)
			fprintf(stdout,"Invalid device\n");
		else
		{
			fprintf(stdout, "%u - %s - %s\n", l_ul_index, l_pc_deviceName, l_pc_deviceProperties);
			
			l_i_Ret = io_px_data->m_x_device.OpenUsbDevice(l_pc_deviceName,0,NULL);	
			if(l_i_Ret == MORPHO_OK)
			{
				if (((strcmp(l_pc_deviceProperties, "CBM") == 0)) ||((strcmp(l_pc_deviceProperties, "Sagem MorphoSmart CBM") == 0)))
				{
					io_px_data->m_e_deviceType = DEVICE_CBM;
				}
				else if (strcmp(l_pc_deviceProperties, "CBI") == 0)
				{
					io_px_data->m_e_deviceType = DEVICE_CBI;
				}
				else if (strcmp(l_pc_deviceProperties, "MSI") == 0)
				{
					io_px_data->m_e_deviceType = DEVICE_MSI;
				}
				else if ( (strcmp(l_pc_deviceProperties, "MorphoSmart FINGER VP") == 0) || (strcmp(l_pc_deviceProperties, "MSO FVP") == 0) )
				{
					io_px_data->m_e_deviceType = DEVICE_FVP;
				}
				//TODO: map other devices properties to the correct type accordingly
				else
				{
					io_px_data->m_e_deviceType = DEVICE_UNKNOWN;
				}
			}
		}

	} 
}

