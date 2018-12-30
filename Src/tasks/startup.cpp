/*
 * startup.cpp
 *
 *  Created on: 03.11.2018
 *      Author: papyk1
 */

#include "startup.h"

#include "main.h"

#include <stm32f1xx_hal.h>
#include <cmsis_os.h>

#include "KeyboardTask.h"
#include "HeartbeatTask.h"

class KeyboardPortIO {
public:
	inline static bool dataRead() {
		return HAL_GPIO_ReadPin(PS2_DATA_GPIO_Port, PS2_DATA_Pin);
	}

	inline static void dataWrite(bool state) {
		HAL_GPIO_WritePin(PS2_DATA_GPIO_Port, PS2_DATA_Pin, static_cast<GPIO_PinState>(state));
	}
};

KeyboardTask<KeyboardPortIO> keyboard;
HeartbeatTask heartbeat;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == PS2_CLK_Pin) {
		keyboard.handleClockInterrupt();
	}
}

void tasksStartup() {
	heartbeat.start("heartbeat", configMINIMAL_STACK_SIZE, 1);
	keyboard.start("keyboard", configMINIMAL_STACK_SIZE, configMAX_PRIORITIES-1);
}
