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

int devStatus=-1;

/*Подключение устройства*/
I openDeviceOperation(PT_DATA io_px_data);

/*ОТключнение устройства*/
I closeDeviceOperation(PT_DATA io_px_data);

/*Получение отпечатка*/
I getFingerCapture(PT_DATA io_px_data);

main(void)
{
	T_DATA l_x_data;

	devStatus=openDeviceOperation(&l_x_data);
	fprintf(stdout,"devStatus = %d\n",devStatus);

	getFingerCapture(&l_x_data);
	
	if(devStatus!=-1)
		{
			devStatus=closeDeviceOperation(&l_x_data);
		}

    return 0;
}

/*-------------------OPEN_DEVICE-----------------------*/

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
	
	return l_i_Ret;
}

/*----------------END_OPEN_DEVICE-----------------------*/

/*-----------------CLOSE_DEVICE-------------------------*/
I closeDeviceOperation(PT_DATA io_px_data)
{
	I	l_i_Ret;
	UL	g_ul_ledEventsID;

	l_i_Ret = io_px_data->m_x_device.UnregisterLEDEvent(g_ul_ledEventsID);
	l_i_Ret = io_px_data->m_x_device.UnregisterFFDCallback();
	l_i_Ret = io_px_data->m_x_device.CloseDevice();

	if (l_i_Ret == MORPHO_OK)
		{
			fprintf(stdout, "Successfully closed device connection\n");
			return -1;
		}
	else
		{
			fprintf(stdout, "Error %d while trying to close connection\n", l_i_Ret);
			return(l_i_Ret);
		}
}


/*----------------END_OPEN_DEVICE-----------------------*/

I getFingerCapture(PT_DATA io_px_data)
{
	I						l_i_Ret;
	UC						l_auc_X984Data[1024];
	T_MORPHO_TYPE_TEMPLATE	l_x_TypeTemplate=MORPHO_PK_COMP;
	//PC						o_pc_extension=".pkcomp";
	C_MORPHO_TemplateList	l_x_templates;
	UC						l_ul_nbFingers=1;
	char 					l_ac_Extensions[16];
	UL						l_ul_asyncEvents =	MORPHO_CALLBACK_DETECTQUALITY |
												MORPHO_CALLBACK_CODEQUALITY |
												MORPHO_CALLBACK_IMAGE_CMD |
												MORPHO_CALLBACK_COMMAND_CMD |
												MORPHO_CALLBACK_ENROLLMENT_CMD;
	T_MORPHO_COMPRESS_ALGO	l_x_compressAlgo = MORPHO_NO_COMPRESS;
	I						l_i_CompressRate=0;

	#ifdef ENABLE_DISPLAY
		InitScreen()
	#endif
	;

	l_x_templates.SetActiveFullImageRetrieving(true);

	l_i_Ret=io_px_data->m_x_device.Capture(
		0,
		0,
		0,
		(UC)l_ul_nbFingers,
		l_x_TypeTemplate,
		MORPHO_NO_PK_FVP,
		1,
		0,
		l_ul_asyncEvents,
		eventCallback,
		io_px_data,
		l_x_templates,
		MORPHO_RAW_TEMPLATE,
		0,
		l_auc_X984Data,
		0,
		MORPHO_DEFAULT_CODER,
		MORPHO_ENROLL_DETECT_MODE,
		0,
		NULL,
		l_x_compressAlgo,
		l_i_CompressRate
	);
	fprintf(stdout,"It's all bitches\n");
}


I eventCallback(
	PVOID						i_pv_context,
	T_MORPHO_CALLBACK_COMMAND	i_i_command,
	PVOID						i_pv_param)
{
PUC								l_puc_EnrollmentCmd;

if (i_i_command == MORPHO_CALLBACK_COMMAND_CMD)
{
	switch(*(PI)i_pv_param)
	{
	case MORPHO_MOVE_NO_FINGER:
		break;
	case MORPHO_REMOVE_FINGER:
		fprintf(stdout, "Remove finger\n");
		break;
	case MORPHO_LATENT:
		fprintf(stdout, "Latent detected\n");
		break;
	case MORPHO_MOVE_FINGER_UP:
		fprintf(stdout, "Move finger up\n");
		break;
	case MORPHO_MOVE_FINGER_DOWN:
		fprintf(stdout, "Move finger down\n");
		break;
	case MORPHO_MOVE_FINGER_LEFT:
		fprintf(stdout, "Move finger left\n");
		break;
	case MORPHO_MOVE_FINGER_RIGHT:
		fprintf(stdout, "Move finger right\n");
		break;
	case MORPHO_PRESS_FINGER_HARDER:
		fprintf(stdout, "Increase finger pressure\n");
		break;
	case MORPHO_FINGER_OK:
		fprintf(stdout, "Acquisition OK\n");
		break;
	case MORPHOERR_FFD:
		fprintf(stdout, "False finger detected\n");
		break;
	case MORPHOERR_MOIST_FINGER:
		fprintf(stdout, "Finger too moist\n");
		break;
	case MORPHO_FINGER_MISPLACED:
		fprintf(stdout, "Bad finger placement\n");
		break;
	case MORPHO_FINGER_DETECTED:
		break;
	case MORPHO_LIVE_OK:
		fprintf(stdout, "Finger detected, processing ...\n");
		break;
	default:
		break;
	}
}
else if (i_i_command == MORPHO_CALLBACK_ENROLLMENT_CMD)
{
	l_puc_EnrollmentCmd = (PUC)i_pv_param;

	switch (l_puc_EnrollmentCmd[2])
	{
	case 1:
		fprintf(stdout, "PLACE Finger #%d\nCAPTURE %d/%d\n",l_puc_EnrollmentCmd[0],l_puc_EnrollmentCmd[2],l_puc_EnrollmentCmd[3]);
		break;

	case 2:
		fprintf(stdout, "Finger #%d AGAIN\nCAPTURE %d/%d\n",l_puc_EnrollmentCmd[0],l_puc_EnrollmentCmd[2],l_puc_EnrollmentCmd[3]);
		break;

	case 3:
		fprintf(stdout, "Finger #%d AGAIN\nCAPTURE %d/%d\n",l_puc_EnrollmentCmd[0],l_puc_EnrollmentCmd[2],l_puc_EnrollmentCmd[3]);
		break;
	}
}
#ifdef ENABLE_DISPLAY
else if (i_i_command == MORPHO_CALLBACK_IMAGE_CMD)
{
	memcpy (&l_x_ImageStructure.m_x_ImageHeader,
		(T_MORPHO_IMAGE_HEADER *) i_pv_param,
		sizeof (T_MORPHO_IMAGE_HEADER));
	l_x_ImageStructure.m_puc_Image =
	(PUC) i_pv_param + sizeof (T_MORPHO_IMAGE_HEADER);

	Display_Image (l_x_ImageStructure.m_puc_Image,
		l_x_ImageStructure.m_x_ImageHeader.m_us_NbRow,
		l_x_ImageStructure.m_x_ImageHeader.m_us_NbCol,
		l_x_ImageStructure.m_x_ImageHeader.m_uc_NbBitsPerPixel);
	SDL_PollEvent (&event);
	if (event.type == SDL_QUIT)
		l_px_data->m_x_device.CancelLiveAcquisition();

}
#endif

return 0;
}

