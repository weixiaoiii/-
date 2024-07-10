/*
 * Copyright (C) 2022 HiHope Open Source Organization .
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http:// www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 *
 * limitations under the License.
 */

#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <stdlib.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio_ex.h"
#include "hi_time.h"
#include "iot_gpio.h"
#include "iot_adc.h"
#include "iot_errno.h"
#include "iot_i2c.h"
#include "hi_io.h"
#include "robot_sg90.h"
#include "iot_pwm.h"
#include "hi_pwm.h"
#include "robot_hcsr04.h"
#include "app_demo_aht20.h"
#include "JQ8900.h"
#include "hi_time.h"



#define NUM 1
#define STACK_SIZE 4096
#define AHT20_BAUDRATE (400 * 1000)
#define AHT20_I2C_IDX 0
#define HCSRTASK_STAK_SIZE    (1024*10)

static void ADCSWTask(int *arg)
{
    (void)arg;
    unsigned int ret = 0;
    unsigned short data = 0;
    float value = 0.0;


    while (NUM) {
        // 光敏电阻对应的是ADC channel 4，
        ret = AdcRead(IOT_ADC_CHANNEL_4, &data, IOT_ADC_EQU_MODEL_4, IOT_ADC_CUR_BAIS_DEFAULT, 0xff);
        if (ret != IOT_SUCCESS) {
            printf("shuiwei Read Fail \r\n");
            return;
        } else {
            /* vlt * 1.8 * 4 / 4096.0 为将码字转换为电压 */
            value = (float)data * (1.8) * 4 /4096;
            printf("shuiwei Read value is %.3f \r\n", value);
            if(value>1.5)
            {
                sendData(0x0a);    
                sendData(0x02);    
                sendData(0x0b);   
                hi_udelay(1500000);
            }
           
        }
        TaskMsleep(100); /* 20:sleep 20ms */
    }
}

static void ADCYWTask(int *arg)
{
    (void)arg;
    unsigned int ret = 0;
    unsigned short data = 0;
    float value = 0.0;

    while (NUM) {
        // 光敏电阻对应的是ADC channel 4，A
        ret = AdcRead(IOT_ADC_CHANNEL_5, &data, IOT_ADC_EQU_MODEL_4, IOT_ADC_CUR_BAIS_DEFAULT, 0xff);
        if (ret != IOT_SUCCESS) {
            printf("yanwu Read Fail \r\n");
            return;
        } else {
            /* vlt * 1.8 * 4 / 4096.0 为将码字转换为电压 */
            value = (float)data * (1.8) * 4 /4096;
            printf("yanwu Read value is %.3f \r\n", value);
        }
        if(value>1.4)
        {
            sendData(0x0a);    
            sendData(0x01);    
            sendData(0x0b); 
            hi_udelay(1500000);
        }
        TaskMsleep(100); /* 20:sleep 20ms */
    }
}

void HCSRTask(void* parame)
{
    (void)parame;
    printf("start test hcsr04\r\n");
    unsigned int time = 200;
    while (1) {
        float distance = GetDistance();
        printf("distance is %f\r\n", distance);
        osDelay(time);
        if(distance>20&&distance<60)
        {
            engine_turn_left();
        }
        else  if(distance<=20)
        {
            engine_turn_right();
            sendData(0x0a);    //清空数字
	        sendData(0x04);    //曲目号
	        sendData(0x0b); 
            hi_udelay(1500000);
        }
        else 
        {
            engine_turn_right();
        }

        TaskMsleep(50);
    }
}

static void AHT20Task(int *arg)
{
    (void)arg;
    unsigned int ret = 0;
    unsigned short data = 0;
    float value = 0.0;


    while (NUM) {
        GetAht20SensorData();
        value=GetAhtSensorValue(1);
       // printf("AHT_wendu is %f\r\n", value);
       hi_udelay(200);
        value=GetAhtSensorValue(2);
       // printf("AHT_shudu is %f\r\n", value);
    }
    TaskMsleep(100); /* 20:sleep 20ms */
}


static void HCSRDemo(void)
{
    osThreadAttr_t attr;

    attr.name = "HCSRTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = HCSRTASK_STAK_SIZE;
    attr.priority = osPriorityNormal+1;

    if (osThreadNew(HCSRTask, NULL, &attr) == NULL) {
        printf("[HCSRDemo] Falied to create HCSRTask!\n");
    }
}

static void ADCSWDemo(void)
{
    osThreadAttr_t attr;

    attr.name = "ADCSWTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = STACK_SIZE;
    attr.priority = osPriorityNormal;

    if (osThreadNew(ADCSWTask, NULL, &attr) == NULL) {
        printf("[ADCSWDemo] Failed to create ADCSWTask!\n");
    }
}

static void ADCYWDemo(void)
{
    osThreadAttr_t attr;

    attr.name = "ADCYWTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = STACK_SIZE;
    attr.priority = osPriorityNormal;

    if (osThreadNew(ADCYWTask, NULL, &attr) == NULL) {
        printf("[ADCYWDemo] Failed to create ADCYWTask!\n");
    }
}
static void AHT20Demo(void)
{
    osThreadAttr_t attr;

    attr.name = "AHT20Task";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = STACK_SIZE;
    attr.priority = osPriorityNormal-1;

    if (osThreadNew(AHT20Task, NULL, &attr) == NULL) {
        printf("[AHT20Demo] Failed to create AHT20Task!\n");
    }
}

APP_FEATURE_INIT(ADCSWDemo);
APP_FEATURE_INIT(ADCYWDemo);
APP_FEATURE_INIT(HCSRDemo);
APP_FEATURE_INIT(AHT20Demo);