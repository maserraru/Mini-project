#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "xparameters.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "lwipopts.h"
#include "xparameters.h"
#include "xgpio.h"
#include "shared.h"
int local_bar;
#ifdef __arm__
#include "xil_printf.h"
#include "FreeRTOS.h"
#include "task.h"
#endif

//#define SEND_BUFSIZE (2)

//int flag4 = 0;
int counter;
int k;
void local_bar_thread()
{

	/* Pointer initialization
			 Check out your own XPAR ID symbol name declared in xparameters.h
			 The format is always XPAR_<NAME_IN_VIVADO>_DEVICE_ID
			 XGpio Initialization*/

		//XGpio buttons,vga,addr
	//		 XGpio buttons;
/*

		//Buttons
			XGpio_Initialize(&buttons, XPAR_BUTTONS_DEVICE_ID);
			XGpio_SetDataDirection(&buttons, 1, 0xffffffff);

*/

//START PROCESS
//Bar
/*
while(1)
	{


		flag4=XGpio_DiscreteRead(&buttons,1);
		if (flag4==1)
		{
			vTaskDelay(10);
			if (counter>50)
				{counter=50;}
				else
				{counter++;}

		flag4=0;
		}

*/			//*Left button was pushed*/
/*
		if(flag4==8)

		{
			vTaskDelay(10);
			if (counter<0)
				{
					counter=0;
				}
				else
				{counter=counter-1;}

		flag4=0;
		}

		local_bar=counter;

		}


*/


}






