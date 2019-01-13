
#ifndef KEYBOARD_IKEYBOARDDRIVER_H_
#define KEYBOARD_IKEYBOARDDRIVER_H_

#include "KeyCodes.h"

class IKeyboardDriver
{
public:
	virtual KeyState getKeyCode() = 0;
	virtual bool setLedState(uint8_t led_number, bool on) = 0;
};

#endif /* KEYBOARD_IKEYBOARDDRIVER_H_ */
