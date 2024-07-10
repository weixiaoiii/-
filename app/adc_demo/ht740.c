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




#define GPIO3 0



void Car(void)
{
    IoTGpioInit(GPIO3); 
    IoSetFunc(GPIO3, IOT_IO_FUNC_GPIO_2_GPIO); 

    IoTGpioSetDir(GPIO3,IOT_GPIO_DIR_IN);

    unsigned char i;
    while(1)
    {
        if(IoTGpioGetInputVal(GPIO3, IOT_GPIO_VALUE1))
        {
            printf("have car\r\n");
        }
        else
        {
            printf("no car\r\n");
        }
        hi_udelay(1000);
    }
    
}

