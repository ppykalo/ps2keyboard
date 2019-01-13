/*
 * KeyboardTask.h
 *
 *  Created on: 03.11.2018
 *      Author: papyk1
 */

#ifndef TASKS_KEYBOARDTASK_H_
#define TASKS_KEYBOARDTASK_H_

#include "RtosTask.h"
#include "keyboard/IKeyboardDriver.h"

class KeyboardTask: public RtosTask {
public:
	KeyboardTask(IKeyboardDriver* keyboardDriver) : _keyboardDriver(keyboardDriver) {}
private:
	void setup() override {}
	void loop() override {
	 // TODO: process SCAN CODE -> ASCII code
		KeyState state;
		do {
			state = _keyboardDriver->getKeyCode();
			if (state.isPressed) {
				++keysPressed;
			} else if (state.isReleased) {
				++keysReleased;
			}
		} while(state.isValid());

		sleep(1);
	}

	IKeyboardDriver* _keyboardDriver;
	uint32_t keysPressed  = 0;
	uint32_t keysReleased = 0;
};

#endif /* TASKS_KEYBOARDTASK_H_ */
