#include <stdio.h>
#include <string.h>
#include "xparameters.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "lwipopts.h"
#include "xparameters.h"
#include "xgpio.h"

#ifdef __arm__
#include "xil_printf.h"
#include "FreeRTOS.h"
#include "task.h"
#endif

#define SEND_BUFSIZE (20)
int red=1;
int green=2;
int blue=4;
int yel=3;
int colors;
int col;
int columns;
int columnas;
int filas;
int j;
int rows;
int num_but_left; //#Button left was pressed
int num_but_right;//#Button right was pressed
int btn_check; //Status all buttons
void write_colors();
int flag1 = 0;
int flag4 = 0;
int aux1;
int counter;
 //extern int x_min=0, x_max, y_min,y_max;



//static char send_buf[SEND_BUFSIZE];

/* Client Thread*/

void rx_data_thread()
{
	//int num_but_left=0;
	//int num_but_right=0;
	//struct ip_addr servaddr;
	//int  sock;
	//struct sockaddr_in serv_addr;
	//int btn_check ,flag3, flag4;


	/* Pointer initialization
	 Check out your own XPAR ID symbol name declared in xparameters.h
	 The format is always XPAR_<NAME_IN_VIVADO>_DEVICE_ID
	 XGpio Initialization*/

//XGpio buttons,vga,addr
	 XGpio buttons;
	 XGpio vga; // the Xgpio used to link the vga (RGB)
	 XGpio mem_addr;

//Buttons
	XGpio_Initialize(&buttons, XPAR_BUTTONS_DEVICE_ID);
	XGpio_SetDataDirection(&buttons, 1, 0xffffffff);

//VGA xgpio
	XGpio_Initialize(&vga,XPAR_RGB_DEVICE_ID);
	XGpio_SetDataDirection(&vga, 1, 0x00000000);

//VGA xgpio
		XGpio_Initialize(&mem_addr,XPAR_ADDR_DEVICE_ID);
		XGpio_SetDataDirection(&mem_addr, 1, 0x00000000);

	/* Set the other zybo address */
	
	//IP4_ADDR(&servaddr, 192, 168,  0, 3); // IP slim
	//IP4_ADDR(&servaddr, 169, 254,  234, 94);//Hanne

	//memset((void*)&serv_addr, 0, sizeof serv_addr);
	//serv_addr.sin_family = AF_INET;
	//serv_addr.sin_port = htons(1005);
	//serv_addr.sin_addr.s_addr = servaddr.addr;
		int x_min=0;
		int x_max=5;
		int y_min=0;
		int y_max=12;

	while (1) {

	/*Right button was pushed*/
		flag4=XGpio_DiscreteRead(&buttons,1);
		if ((flag4==1)&&(y_max<120))

		{

			counter++;
			flag4=0;
			y_min=y_min+counter*2;
			y_max=y_max+counter*2;

				if (counter>60)
				{
					counter=60;
				}
		}
	/*Left button was pushed*/
		else if((flag4==8)&&(y_min>0))
		//	((btn_check==8)&&(y_min>0))
		{

			counter=counter-1;
			flag4=0;
			y_min=y_min+counter*2;
			y_max=y_max+counter*2;

				if (counter<0)
				{
					counter=0;
				}
		}

		/*Definition local Bar*/

		write_colors(0,0,20,0,255,vga,mem_addr);//refresh screen
		write_colors(blue,x_min,x_max,y_min,y_max,vga,mem_addr);
		for(j=0;j<1000000;j++){}

			}// while loop
} // thread





//void write_colors(int color1,int color2,int color3,XGpio vga,XGpio addr)
void write_colors(int color,int x_min,int x_max,int y_min,int y_max,XGpio vga,XGpio mem_addr)
{


	int y=0;
	int x=0;


	for (y=y_min;y<y_max;y++)
	{

		for (x=x_min;x<x_max;x++)
		{
			XGpio_DiscreteWrite(&vga, 1, color);
			XGpio_DiscreteWrite(&mem_addr, 1,y+256*x);

		}


	}
}


