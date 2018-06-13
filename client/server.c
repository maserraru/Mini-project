

#include <stdio.h>
#include <string.h>
#include <time.h>

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
//int condition(int flag1, int counter);
void print_echo_app_header()
{
    printf("%20s %6d %s\r\n", "echo server",
                        echo_port,
                        "$ telnet <board_ip> 1006");

}


/* thread spawned for each connection */



void server_client(void *p)
{
	int sd = (int)p;
	int NUM_DATA = 2;
	char send_buf[NUM_DATA];
	int  n, num_chars=0;
	//int switch_check;
	int flag1;
	int counter=0;

	// Pointer initialization
	// Check out your own XPAR ID symbol name declared in xparameters.h
	// The format is always XPAR_<NAME_IN_VIVADO>_DEVICE_ID
	//XGpio Initialization

	XGpio buttons;


	        XGpio_Initialize(&buttons, XPAR_BUTTONS_DEVICE_ID);
			XGpio_SetDataDirection(&buttons, 1, 0xffffffff);
	//xil_printf("A new client connected\n\r");

	while(1)
	{

		/*Right button was pushed*/
				flag1=XGpio_DiscreteRead(&buttons,1);
				vTaskDelay(3);
		//		counter=condition(int flag1, int counter);
				if (flag1==1)
				{

					vTaskDelay(3);
						if (counter>40)
						{counter=50;}
						else
						{counter++;}
				flag1=0;
				}
			/*Left button was pushed*/
				else if(flag1==8)
					//for(i=0;i<60;i++){};
				//	((btn_check==8)&&(y_min>0))
				{
					vTaskDelay(3);
						if (counter<10)
						{counter=1;}
						else{counter=counter-1;}
				flag1=0;
				}
				 //xil_printf("\n el contador es: %d\n",counter);
				send_buf[0] = (char)counter;
				send_buf[1] ='\O';

				// fin de la funcion

    		/*send information from the switches*/
        if ((n = write(sd, send_buf, 2)) < 0)
            			{
            				xil_printf("%s: ERROR ",
            					__FUNCTION__, num_chars, n);
            				xil_printf("Closing socket ", sd);
            				break;
            			}


	}

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
/*
int condition(int flag1, int counter){

	if (flag1==1)

					{
						counter++;
						flag1=0;

							if (counter>60)
							{
								counter=60;
							}
					}
				//Left button was pushed
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

	return counter;
}  */
