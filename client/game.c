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
extern int khalid;
extern int local_bar;
extern int david;
extern int maria;
#ifdef __arm__
#include "xil_printf.h"
#include "FreeRTOS.h"
#include "task.h"
#endif

//#define SEND_BUFSIZE (2)
void print_ip();
int red=1;
int green=2;
int blue=4;
int black=0;
int colors;

int j;
int flag4 = 0;
int counter;
int k;

int y_min_1;
int y_min_2;


int btn_check; //Status all buttons
void write_colors();

void vn_game_thread()
{

		//*Definition of the local bar*/
//int i;
    	/* Pointer initialization
			 Check out your own XPAR ID symbol name declared in xparameters.h
			 The format is always XPAR_<NAME_IN_VIVADO>_DEVICE_ID
			 XGpio Initialization*/

		//XGpiovga,addr, buttons

			 XGpio vga; // the Xgpio used to link the vga (RGB)
			 XGpio mem_addr;
			 XGpio buttons;

		//Buttons
		XGpio_Initialize(&buttons, XPAR_BUTTONS_DEVICE_ID);
		XGpio_SetDataDirection(&buttons, 1, 0xffffffff);


		//VGA xgpio
			XGpio_Initialize(&vga,XPAR_RGB_DEVICE_ID);
			XGpio_SetDataDirection(&vga, 1, 0x00000000);

		//VGA xgpio
				XGpio_Initialize(&mem_addr,XPAR_ADDR_DEVICE_ID);
				XGpio_SetDataDirection(&mem_addr, 1, 0x00000000);


//START PROCESS
//Bar received
write_colors(black,0,255,0,255,vga,mem_addr);

while(1)
	{

			write_colors(black,0,120,0,160,vga,mem_addr);

			/*Definition Non Local Bar*/
				y_min_2=khalid*2; // khalid is controlled by player 2, y_min_2 update the position of bar 2

			/*Definition Local Bar*/
			flag4=XGpio_DiscreteRead(&buttons,1);  // flag4 read position of player 1

			//*Right button was pushed*/
					if (flag4==1)  // move down
					{
					vTaskDelay(3);

						if (counter>50)
							{counter=50;}
							else
							{counter++;}

					/*We refresh the screen*/
					write_colors(black,y_min_2,y_min_2+20,154,159,vga,mem_addr);
					write_colors(black,y_min_1,y_min_1+20,0,5,vga,mem_addr);
					/*We update the local bar*/
					y_min_1=counter*2;  // update position of player 1
					//y_max_1=12+counter*2;
					/*We print the screen*/
					write_colors(red,y_min_2,y_min_2+20,154,159,vga,mem_addr); // print bar 2
					write_colors(blue,y_min_1,y_min_1+20,0,5,vga,mem_addr);   // print bar 2
					vTaskDelay(5);
					flag4=0;
					}

		//*Left button was pushed*/

					if(flag4==8)  // move up
					{
						vTaskDelay(3);
						if (counter<0)
							{
								counter=0;
							}
							else
							{counter=counter-1;}
						/*We refresh the screen*/
						write_colors(black,y_min_2,y_min_2+20,154,159,vga,mem_addr);
						write_colors(black,y_min_1,y_min_1+20,0,5,vga,mem_addr);

						/*We update the local bar*/
						y_min_1=counter*2;
						//y_max_1=12+counter*2;
						/*We print the screen*/
						write_colors(red,y_min_2,y_min_2+20,154,159,vga,mem_addr);
						write_colors(blue,y_min_1,y_min_1+20,0,5,vga,mem_addr);
						vTaskDelay(5);
					flag4=0;
					}

					write_colors(black,y_min_2,y_min_2+20,154,159,vga,mem_addr);
					write_colors(black,y_min_1,y_min_1+20,0,5,vga,mem_addr);
					write_colors(red,y_min_2,y_min_2+20,154,159,vga,mem_addr);
					write_colors(blue,y_min_1,y_min_1+20,0,5,vga,mem_addr);
					vTaskDelay(5);


			/*Definition Ball*/

			write_colors(black,maria,maria+5,david,david+5,vga,mem_addr);
			write_colors(green,maria,maria+5,david,david+5,vga,mem_addr);
			vTaskDelay(5);

//END PROCESS
		}

}

//void write_colors(int color1,int color2,int color3,XGpio vga,XGpio addr)
void write_colors(int color,int y_min, int y_max,int x_min,int x_max, XGpio vga,XGpio mem_addr)
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

/*void write_colors(int color,int y_min,int y_max,int x_min,int x_max, XGpio vga, XGpio addr)
{
	int y=0;
	int x=0;


	for (y=0;y<120;y++)
	{
		for (x=0;x<160;x++)
		{
			if (y>y_min && y<y_max)
			{
				if(x>x_min && x<x_max)
				{
					XGpio_DiscreteWrite(&vga, 1, color);
					XGpio_DiscreteWrite(&addr, 1,y+256*x);
				}
				else
				{
					XGpio_DiscreteWrite(&vga, 1, 0);
					XGpio_DiscreteWrite(&addr, 1,y+256*x);
				}

			}else
			{
				XGpio_DiscreteWrite(&vga, 1, 0);
				XGpio_DiscreteWrite(&addr, 1,y+256*x);
			}

		}


	}


}
*/
