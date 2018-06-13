#include <stdio.h>
#include <string.h>
#include "xparameters.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "lwipopts.h"
#include "xparameters.h"
#include "xgpio.h"
#include "shared.h"
int khalid;
#ifdef __arm__
#include "xil_printf.h"
#include "FreeRTOS.h"
#include "task.h"
#endif

//#define SEND_BUFSIZE (2)
void print_ip();
int flag3, btn_check;
int delta_y_old=0;
void rx_data_thread()
{
				int n,delta_y;
				int RECV_BUF_SIZE = 2;
				char recv_buf[RECV_BUF_SIZE];

				//Buttons
				XGpio buttons;
				XGpio_Initialize(&buttons, XPAR_BUTTONS_DEVICE_ID);
				XGpio_SetDataDirection(&buttons, 1, 0xffffffff);


	struct ip_addr servaddr;
	int  sock;
	struct sockaddr_in serv_addr;
	//int btn_check ,flag3, flag4;




	/* Set the other zybo address */
	IP4_ADDR(&servaddr, 147, 83,  81, 229); // Ip of board 2 accordance with document in atenea


	memset((void*)&serv_addr, 0, sizeof serv_addr);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(1005);
	serv_addr.sin_addr.s_addr = servaddr.addr;


	while (1) {

		//xil_printf("INSIDE THE RECEIVER\r\n");
		if ((sock = lwip_socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			xil_printf("error creating socket\r\n");
			vTaskDelete(NULL);
			return;														   }

		print_ip("Now connected  ", &servaddr);
		//xil_printf("... ");

		if (lwip_connect(sock, (struct sockaddr *)&serv_addr, sizeof (serv_addr)) < 0)
		{
			xil_printf("error in connect\r\n");

		} else
		{
			print("Connected as a client \n\r");

			while (1) {
				if ((n = read(sock, recv_buf, RECV_BUF_SIZE)) < 0) {
					xil_printf("%s: error reading from socket %d, closing socket\r\n", __FUNCTION__, sock);
					break;
																   }

//START PROCESS
//Bar received

				delta_y=(int)recv_buf[0];

			//	xil_printf("Now Receiving Data [0] is: %d\r\n",delta_y);

				if(delta_y_old!=delta_y)
				{
					khalid=delta_y;
					//xil_printf("Khalid is: %d\r\n",khalid);
				}


				delta_y_old=delta_y;

//END PROCESS
					}


		xil_printf("Connection Failed \r\n Press Button 2 to Reconnect\r\n");

		}
				//close(sock);


				// Check the button to reconnect

				flag3=0;
				while (flag3==0){
					btn_check = XGpio_DiscreteRead(&buttons, 1);
					flag3 = (btn_check & 2);
								 }
				//
				while (flag3 == 2) {
					btn_check = XGpio_DiscreteRead(&buttons, 1);
					flag3 = (btn_check & 2);
									}

				xil_printf("Establishing Reconnection\r\n");

			vTaskDelete(NULL);
		xil_printf("Establishing Reconnection\r\n");
	}
	vTaskDelete(NULL);

}


