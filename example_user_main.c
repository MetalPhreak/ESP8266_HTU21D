/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_main.c
 *
 * Description: entry file of user application
 *
 * Modification history:
 *     2015/1/23, v1.0 create this file.
*******************************************************************************/

#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"


#include "driver/i2c.h"
#include "driver/htu21d.h"

//default tasks (3 priority levels)
#define TopTaskPrio        2
#define TopTaskQueueLen    1

#define ActionTaskPrio        1
#define ActionTaskQueueLen    8

#define IdleTaskPrio        0
#define IdleTaskQueueLen    1

static void TopTask(os_event_t *events);
static void ActionTask(os_event_t *events);
static void IdleTask(os_event_t *events);

os_event_t    TopTaskQueue[TopTaskQueueLen];
os_event_t    ActionTaskQueue[ActionTaskQueueLen];
os_event_t    IdleTaskQueue[IdleTaskQueueLen];

static void ICACHE_FLASH_ATTR TopTask(os_event_t *events){

}

static void ICACHE_FLASH_ATTR ActionTask(os_event_t *events){

}

static void ICACHE_FLASH_ATTR IdleTask(os_event_t *events){
    float temperature;
    float humidity;
    char tstring[16];
    char hstring[16];
    if(HTU21D_temperature(&temperature)) { //read temperature value into float var
        ftos(temperature, tstring); //convert float var into string and store for later use
    } else {
        os_sprintf(tstring, "Error!");
    }
    if(HTU21D_humidity(&humidity)) { //read humidity value into float var
        ftos(humidity, hstring); //convert float var into string and store for later use
    } else {
        os_sprintf(hstring, "Error!");
    }
    os_printf("Temp: %s Humidity: %s\n", tstring, hstring); //print values to serial terminal
	system_os_post(IdleTaskPrio, 0, 0); //add IdleTask back into queue
}

void ICACHE_FLASH_ATTR init_tasks(){
    os_delay_us(15000); //allow 15ms for sensor to come online from cold poweron
    HTU21D_softReset(); //probably not required but ensures sensor is running OK
    os_delay_us(15000); //allow 15ms for soft Reset to complete
    os_printf("Initialisation Complete. Starting Tasks\n");
    //add tasks
    system_os_task(TopTask, TopTaskPrio, TopTaskQueue, TopTaskQueueLen);
    system_os_task(ActionTask, ActionTaskPrio, ActionTaskQueue, ActionTaskQueueLen);
    system_os_task(IdleTask, IdleTaskPrio, IdleTaskQueue, IdleTaskQueueLen);
    //after init done, start idle task loop
    system_os_post(IdleTaskPrio, 0, 0);
}


void user_rf_pre_init(void)
{
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init(void)
{
    os_printf("SDK version:%s\n", system_get_sdk_version());

    i2c_init();

    wifi_set_opmode(STATION_MODE);
    system_init_done_cb(init_tasks); 

}


