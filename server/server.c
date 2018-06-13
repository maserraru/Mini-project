/*
This code correspond a Server:
1. Wait for clients (port ??)
2. If dip_switch toggle Send information from the value of the dip switches
3.Check the value of 1st dip
*/

#include <stdio.h>
#include <string.h>

#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwipopts.h"
#include "xparameters.h"
#include "config_apps.h"
#include "xil_printf.h"
#include "FreeRTOS.h"
#include "task.h"

#include "xgpio.h"
u16_t echo_port = 1005;

void print_echo_app_header()
{
    printf("%20s %6d %s\r\n", "echo server",
                        echo_port,
                        "$ telnet <board_ip> 1006");

}


/* Thread for the server */

void server_client(void *p)
{
	int sd = (int)p;
	int n, num_chars=0;
	int NUM_DATA = 20;
	int flag1;
	char counter;

	/*Variables used in irder to send information when dip switch 1  is toggled*/
	int dip_check;
	dip_check=0;
	/* Pointer initialization
	Check out your own XPAR ID symbol name declared in xparameters.h
	The format is always XPAR_<NAME_IN_VIVADO>_DEVICE_ID
	XGpio Initialization*/


	 XGpio buttons;  // read position bal

	//Buttons
		XGpio_Initialize(&buttons, XPAR_BUTTONS_DEVICE_ID);
		XGpio_SetDataDirection(&buttons, 1, 0xffffffff);
	while(1)
	{
		printf("\n estoy aqui :2\n");

		/*Right button was pushed*/
				flag1=XGpio_DiscreteRead(&buttons,1);
				if (flag1==1)

				{
					printf("\n estoy aqui :3\n");
					counter++;
					flag1=0;


						if (counter>60)
						{
							counter=60;
						}
				}
			/*Left button was pushed*/
				else if(flag1==8)
				//	((btn_check==8)&&(y_min>0))
				{

					counter=counter-1;
					flag1=0;


						if (counter<0)
						{
							counter=0;
						}
				}

    		/*send information from the switches*/
        if ((n = write(sd, counter, 20)) < 0)
            			{
            				xil_printf("%s: ERROR ",
            					__FUNCTION__, num_chars, n);
            				xil_printf("Closing socket ", sd);
            				break;
            			}
        xil_printf("\n el contador es: %d\n",counter);
	}

	/* End the connection*/
	close(sd);
	vTaskDelete(NULL);
}

void tx_data_thread()
{
	int socket, new_sd;
	struct sockaddr_in address, remote;
	int size;

	if ((socket = lwip_socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return;

	address.sin_family = AF_INET;
	address.sin_port = htons(echo_port);
	address.sin_addr.s_addr = INADDR_ANY;


	if (lwip_bind(socket, (struct sockaddr *)&address, sizeof (address)) < 0)
		return;

	lwip_listen(socket, 0);

	size = sizeof(remote);

	while (1)
	{

		printf("\n estoy aqui :1\n");
		// every new connection creates a processing thread
		if ((new_sd = lwip_accept(socket, (struct sockaddr *)&remote, (socklen_t *)&size)) > 0)
		{
			xil_printf("New Client thread\n\r");
			sys_thread_new("echos", server_client,
				(void*)new_sd,
				THREAD_STACKSIZE,
				DEFAULT_THREAD_PRIO);
		}
	}
}
