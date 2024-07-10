/*
 * Copyright (C) 2022 HiHope Open Source Organization .
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 *
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"
#include "hi_io.h"
#include "iot_gpio_ex.h"




#define GPIO3 3



void sendData(unsigned char add)
{
    IoTGpioInit(GPIO3); 
    IoSetFunc(GPIO3, IOT_IO_FUNC_GPIO_2_GPIO); 

    IoTGpioSetDir(GPIO3,IOT_GPIO_DIR_OUT);

    unsigned char i;
    IoTGpioSetOutputVal(GPIO3, IOT_GPIO_VALUE1);
    hi_udelay(1000);
    IoTGpioSetOutputVal(GPIO3, IOT_GPIO_VALUE0);
    hi_udelay(3200);
    for(i=0;i<8;i++)
    {
        IoTGpioSetOutputVal(GPIO3, IOT_GPIO_VALUE1);
        if(add & 0x01)
        {
            hi_udelay(600);
            IoTGpioSetOutputVal(GPIO3, IOT_GPIO_VALUE0);
            hi_udelay(200);
        }
        else
        {
            hi_udelay(200);
            IoTGpioSetOutputVal(GPIO3, IOT_GPIO_VALUE0);
            hi_udelay(600);
        }
        add >>=1;
    }
    IoTGpioSetOutputVal(GPIO3, IOT_GPIO_VALUE1);
}

