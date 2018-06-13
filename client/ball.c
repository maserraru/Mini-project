#include <stdio.h>
#include <stdlib.h>
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
extern int y_min_1;
extern int y_min_2;
#ifdef __arm__
#include "xil_printf.h"
#include "FreeRTOS.h"
#include "task.h"
#endif

//#define SEND_BUFSIZE (20)
int david; // will be y position sent to game.c
int maria; //will be x position sent to game.c


int j;
int aux_1;
int aux=0;
int * right();
int * left();
int aux_right[2];
int aux_left[2];


/*Definition of ball movement*/

void ms_ball_thread()
{
	int *y_pos;
	int *y_pos1;
	aux=0;

		while(1)
		{

			if(aux==0)
			{
					y_pos=right(5,0,2);
			        y_pos1=left(150,y_pos[0],y_pos[1]);
			        aux=y_pos1[1];
			}else
			{
				   if((y_pos1[0]>y_min_1) && (y_pos1[0]<y_min_1+20))
				   {
					   y_pos=right(5,y_pos1[0],y_pos1[1]);
				   }
				   else
				   {
					   	   y_pos=right(5,rand() % 100,2);//Generate random number to start
				   }

				   if((y_pos[0]>y_min_2) && (y_pos[0]<y_min_2+20))
				  	   {
					   	   y_pos1=left(150,y_pos[0],y_pos[1]);
				  	   }
				   else
				   {
				  		   y_pos1=left(150,rand() % 100,2);//Generate random number to start
				   }

					aux=y_pos1[1];
			}
		}
}

int * right(int x, int y, int aux_1)
{
				if(aux_1==2)
				{
					while(y<115 && x<150)
					{

							x++;
							y++;
							aux=1;
							/*Sending in global Variables*/
							david=x;
							maria=y;
							vTaskDelay(5);
					}
					while(y>0 && x<150)
					{
							x++;
							y--;
							aux=2;
							/*Sending in global Variables*/
							david=x;
							maria=y;
							vTaskDelay(5);

					}
					while(y<115 && x<150)
					{
						x++;
						y++;
						aux=3;
						/*Sending in global Variables*/
						david=x;
						maria=y;
						vTaskDelay(5);

					}
				}else
				{
					while(y>0 && x<150)
					{
					x++;
					y--;
					aux=1;
					/*Sending in global Variables*/
					david=x;
					maria=y;
					vTaskDelay(5);
					}
					while(y<115 && x<150)
					{

					x++;
					y++;
					aux=3;
					/*Sending in global Variables*/
					david=x;
					maria=y;
					vTaskDelay(5);

					}
					while(y>0 && x<150)
					{
					x++;
					y--;
					aux=2;
					/*Sending in global Variables*/
					david=x;
					maria=y;
					vTaskDelay(5);

					}
				}
				aux_right[0]=y;
				aux_right[1]=aux;
				return aux_right;
}

int * left(int x, int y, int aux_1)
{
	if(aux_1==2)
	{
	while(y>0 && x>5)
	{
		x--;
		y--;
		aux=1;
		/*Sending in global Variables*/
		david=x;
		maria=y;
		vTaskDelay(5);
	}

	while(y<115 && x>5)
	{
		x--;
		y++;
		aux=2;
		/*Sending in global Variables*/
		david=x;
		maria=y;
		vTaskDelay(5);
	}

	while(y>0 && x>5)
	{
		x--;
		y--;
		aux=3;
		/*Sending in global Variables*/
		david=x;
		maria=y;
		vTaskDelay(5);
	}
	}else
	{
		while(y<115 && x>5)
			{
				x--;
				y++;
				aux=1;
				/*Sending in global Variables*/
				david=x;
				maria=y;
				vTaskDelay(5);
			}

			while(y>0 && x>5)
			{
				x--;
				y--;
				aux=3;
				/*Sending in global Variables*/
				david=x;
				maria=y;
				vTaskDelay(5);
			}

			while(y<115 && x>5)
			{
				x--;
				y++;
				aux=2;
				/*Sending in global Variables*/
				david=x;
				maria=y;
				vTaskDelay(5);
			}
	}
	aux_left[0]=y;
	aux_left[1]=aux;
	return aux_left;
}

