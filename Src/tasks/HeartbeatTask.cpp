/*
 * HeartbeatTask.cpp
 *
 *  Created on: 03.11.2018
 *      Author: papyk1
 */

#include "HeartbeatTask.h"
#include "main.h"
#include <stm32f1xx_hal.h>

void HeartbeatTask::setup() {
	HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_SET);
}

void HeartbeatTask::loop() {
	HAL_GPIO_TogglePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin);
	sleep(150);
	HAL_GPIO_TogglePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin);

	sleep(200);

	HAL_GPIO_TogglePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin);
	sleep(150);
	HAL_GPIO_TogglePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin);

	sleep(1000);
}
