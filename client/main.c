
#include <stdio.h>
#include "xparameters.h"
#include "lwipopts.h"
#include "platform_config.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/init.h"
#include "netif/xadapter.h"
#include "lwip/dhcp.h"
#include "config_apps.h"
#include "task.h"
#include "portmacro.h"
#include "xil_printf.h"
#include "xgpio.h"
extern int khalid;
int main_thread();
void print_echo_app_header();

//Server.c code --->
void tx_data_thread();

//Client.c code --->Managing connection with router
void rx_data_thread();

//Game.c code ---> gaming (Printing VGA)
void vn_game_thread();

//Ball.c code ---> ball movement
void ms_ball_thread();

//LocalBar.c code ---> local bar movement
//void local_bar_thread();


void print_ip(char *msg, struct ip_addr *ip)
{
    print(msg);
    xil_printf("%d.%d.%d.%d\r\n", ip4_addr1(ip), ip4_addr2(ip),
            ip4_addr3(ip), ip4_addr4(ip));
}

void print_settings(struct ip_addr *ip, struct ip_addr *mask, struct ip_addr *gw)
{

    print_ip("ZYBO VICTOR IP: ", ip);
    print_ip("NETMASK : ", mask);
    print_ip("GATEWAY : ", gw);
}


int main()
{
	//Start main thread
	sys_thread_new("main_thread", (void(*)(void*))main_thread, 0,
                THREAD_STACKSIZE,
                DEFAULT_THREAD_PRIO);
	vTaskStartScheduler();
    while(1); // infinite loop
    return 0;
}

struct netif server_netif;

//void network_thread(void *p)
void network_thread()
{
    struct netif *netif;
    struct ip_addr ipaddr, netmask, gw;

    /* We have to define different MAC address for both ZYBOS */

    unsigned char mac_ethernet_address[] = { 0x00, 0x16, 0x3e, 0x8a, 0x79, 0x38};

    netif = &server_netif;


    /* my ip address */
    IP4_ADDR(&ipaddr,  147, 83,  81, 229);

    IP4_ADDR(&netmask, 255, 255, 255,  0);
    /* second zybo ip address*/

   IP4_ADDR(&gw,      147, 83,  81, 1);


    /* print out IP settings of the board */
    print("\r\n\r\n");
    print("PING PONG-\r\n");
    print("DAVID-MARIA-KHALID-VICTOR-\r\n");
    /* Print all the Settings */
    print_settings(&ipaddr, &netmask, &gw);


    /* Add network interface to the netif_list, and set it as default */
    if (!xemac_add(netif, &ipaddr, &netmask, &gw, mac_ethernet_address, PLATFORM_EMAC_BASEADDR)) {
        xil_printf("Error adding N/W interface\r\n");
        return;
    }
    netif_set_default(netif);

    /* specify that the network if is up */
    netif_set_up(netif);

    /* start packet receive thread - required for lwIP operation */
    sys_thread_new("xemacif_input_thread", (void(*)(void*))xemacif_input_thread, netif,
            THREAD_STACKSIZE,
            DEFAULT_THREAD_PRIO);


    return;
}

int main_thread()
{
	/* lwip Initialization and then call sys_thread*/

	lwip_init();

	// Function to define the connection

	network_thread();

	 // Client Created and we obtained the non local bar
	    sys_thread_new("receiving_thread", rx_data_thread, 0,
	        THREAD_STACKSIZE,
	        DEFAULT_THREAD_PRIO);
/*
	// Local bar Created  (We generate the local bar movement)
	    sys_thread_new("localbar_thread", local_bar_thread, 0,
	        THREAD_STACKSIZE,
	        DEFAULT_THREAD_PRIO);
*/

	// Ball Created  (We generate the ball movement)
	    sys_thread_new("balling_thread", ms_ball_thread, 0,  // code in ball.c
	        THREAD_STACKSIZE,
	        DEFAULT_THREAD_PRIO);

	// GAme Created (We printed the VGA)
    sys_thread_new("gaming_thread", vn_game_thread, 0,  // code in game.c
        THREAD_STACKSIZE,
        DEFAULT_THREAD_PRIO);

    // Server Created
        sys_thread_new("sending_thread", tx_data_thread, 0, // code in server.c
        THREAD_STACKSIZE,
        DEFAULT_THREAD_PRIO);

    vTaskDelete(NULL);

    return 0;
}

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue or
	semaphore is created.  It is also called by various parts of the demo
	application.  If heap_1.c or heap_2.c are used, then the size of the heap
	available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains.*/
	xil_printf("Memory Allocation Error\r\n");
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* vApplicationStackOverflowHook() will only be called if
	configCHECK_FOR_STACK_OVERFLOW is set to either 1 or 2.  The handle and name
	of the offending task will be passed into the hook function via its
	parameters.  However, when a stack has overflowed, it is possible that the
	parameters will have been corrupted, in which case the pxCurrentTCB variable
	can be inspected directly.*/
	xil_printf("Stack Overflow in %s\r\n", pcTaskName);
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
void vApplicationSetupHardware( void )
{

}



