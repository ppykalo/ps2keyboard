/*
 * KeyCodes.h
 *
 *  Created on: Jan 3, 2019
 *      Author: pawell
 */

#ifndef KEYBOARD_KEYCODES_H_
#define KEYBOARD_KEYCODES_H_

#include <stdint.h>

typedef uint8_t VirtualKeyCode;

struct KeyState {
	VirtualKeyCode code;
	bool isPressed;
	bool isReleased;

	bool isValid() {
		return isPressed || isReleased;
	}

	static KeyState invalid() {
		return { 0x00, false, false };
	}
};

#define KEYBOARD_CODE_RESERVED           0x00
#define KEYBOARD_CODE_ERRORROLLOVER      0x01
#define KEYBOARD_CODE_POSTFAIL           0x02
#define KEYBOARD_CODE_ERRORUNDEFINED     0x03
#define KEYBOARD_CODE_A                  0x04
#define KEYBOARD_CODE_B                  0x05
#define KEYBOARD_CODE_C                  0x06
#define KEYBOARD_CODE_D                  0x07
#define KEYBOARD_CODE_E                  0x08
#define KEYBOARD_CODE_F                  0x09
#define KEYBOARD_CODE_G                  0x0A
#define KEYBOARD_CODE_H                  0x0B
#define KEYBOARD_CODE_I                  0x0C
#define KEYBOARD_CODE_J                  0x0D
#define KEYBOARD_CODE_K                  0x0E
#define KEYBOARD_CODE_L                  0x0F
#define KEYBOARD_CODE_M                  0x10
#define KEYBOARD_CODE_N                  0x11
#define KEYBOARD_CODE_O                  0x12
#define KEYBOARD_CODE_P                  0x13
#define KEYBOARD_CODE_Q                  0x14
#define KEYBOARD_CODE_R                  0x15
#define KEYBOARD_CODE_S                  0x16
#define KEYBOARD_CODE_T                  0x17
#define KEYBOARD_CODE_U                  0x18
#define KEYBOARD_CODE_V                  0x19
#define KEYBOARD_CODE_W                  0x1A
#define KEYBOARD_CODE_X                  0x1B
#define KEYBOARD_CODE_Y                  0x1C
#define KEYBOARD_CODE_Z                  0x1D
#define KEYBOARD_CODE_1                  0x1E
#define KEYBOARD_CODE_2                  0x1F
#define KEYBOARD_CODE_3                  0x20
#define KEYBOARD_CODE_4                  0x21
#define KEYBOARD_CODE_5                  0x22
#define KEYBOARD_CODE_6                  0x23
#define KEYBOARD_CODE_7                  0x24
#define KEYBOARD_CODE_8                  0x25
#define KEYBOARD_CODE_9                  0x26
#define KEYBOARD_CODE_0                  0x27
#define KEYBOARD_CODE_RETURN             0x28
#define KEYBOARD_CODE_ESCAPE             0x29
#define KEYBOARD_CODE_BACKSPACE          0x2A
#define KEYBOARD_CODE_TAB                0x2B
#define KEYBOARD_CODE_SPACEBAR           0x2C
#define KEYBOARD_CODE_MINUS              0x2D
#define KEYBOARD_CODE_EQUAL_PLUS         0x2E
#define KEYBOARD_CODE_SQUARE_OPEN        0x2F
#define KEYBOARD_CODE_SQUARE_CLOSE       0x30
#define KEYBOARD_CODE_BACKSLASH          0x31
#define KEYBOARD_CODE_HASH_TILDE         0x32
#define KEYBOARD_CODE_SEMICOLON          0x33
#define KEYBOARD_CODE_APOSTROPHE         0x34
#define KEYBOARD_CODE_GRAVE              0x35
#define KEYBOARD_CODE_COMMA              0x36
#define KEYBOARD_CODE_DOT                0x37
#define KEYBOARD_CODE_SLASH              0x38
#define KEYBOARD_CODE_CAPS               0x39
#define KEYBOARD_CODE_F1                 0x3A
#define KEYBOARD_CODE_F2                 0x3B
#define KEYBOARD_CODE_F3                 0x3C
#define KEYBOARD_CODE_F4                 0x3D
#define KEYBOARD_CODE_F5                 0x3E
#define KEYBOARD_CODE_F6                 0x3F
#define KEYBOARD_CODE_F7                 0x40
#define KEYBOARD_CODE_F8                 0x41
#define KEYBOARD_CODE_F9                 0x42
#define KEYBOARD_CODE_F10                0x43
#define KEYBOARD_CODE_F11                0x44
#define KEYBOARD_CODE_F12                0x45
#define KEYBOARD_CODE_PRINTSCREEN        0x46
#define KEYBOARD_CODE_SCROLL             0x47
#define KEYBOARD_CODE_PAUSE              0x48
#define KEYBOARD_CODE_INSERT             0x49
#define KEYBOARD_CODE_HOME               0x4A
#define KEYBOARD_CODE_PAGEUP             0x4B
#define KEYBOARD_CODE_DELETE             0x4C
#define KEYBOARD_CODE_END                0x4D
#define KEYBOARD_CODE_PAGEDOWN           0x4E
#define KEYBOARD_CODE_RIGHTARROW         0x4F
#define KEYBOARD_CODE_LEFTARROW          0x50
#define KEYBOARD_CODE_DOWNARROW          0x51
#define KEYBOARD_CODE_UPARROW            0x52
#define KEYBOARD_CODE_NUM_LOCK_CLEAR     0x53
#define KEYBOARD_CODE_NUM_SLASH          0x54
#define KEYBOARD_CODE_NUM_ASTERISK       0x55
#define KEYBOARD_CODE_NUM_MINUS          0x56
#define KEYBOARD_CODE_NUM_PLUS           0x57
#define KEYBOARD_CODE_NUM_ENTER          0x58
#define KEYBOARD_CODE_NUM_1              0x59
#define KEYBOARD_CODE_NUM_2              0x5A
#define KEYBOARD_CODE_NUM_3              0x5B
#define KEYBOARD_CODE_NUM_4              0x5C
#define KEYBOARD_CODE_NUM_5              0x5D
#define KEYBOARD_CODE_NUM_6              0x5E
#define KEYBOARD_CODE_NUM_7              0x5F
#define KEYBOARD_CODE_NUM_8              0x60
#define KEYBOARD_CODE_NUM_9              0x61
#define KEYBOARD_CODE_NUM_0              0x62
#define KEYBOARD_CODE_NUM_DOT            0x63
#define KEYBOARD_CODE_BACKSLASH_PIPE     0x64
#define KEYBOARD_CODE_APPLICATION        0x65
#define KEYBOARD_CODE_POWER              0x66
#define KEYBOARD_CODE_NUM_EQUAL          0x67
#define KEYBOARD_CODE_F13                0x68
#define KEYBOARD_CODE_F14                0x69
#define KEYBOARD_CODE_F15                0x6A
#define KEYBOARD_CODE_F16                0x6B
#define KEYBOARD_CODE_F17                0x6C
#define KEYBOARD_CODE_F18                0x6D
#define KEYBOARD_CODE_F19                0x6E
#define KEYBOARD_CODE_F20                0x6F
#define KEYBOARD_CODE_F21                0x70
#define KEYBOARD_CODE_F22                0x71
#define KEYBOARD_CODE_F23                0x72
#define KEYBOARD_CODE_F24                0x73
#define KEYBOARD_CODE_EXECUTE            0x74
#define KEYBOARD_CODE_HELP               0x75
#define KEYBOARD_CODE_MENU               0x76
#define KEYBOARD_CODE_SELECT             0x77
#define KEYBOARD_CODE_STOP               0x78
#define KEYBOARD_CODE_AGAIN              0x79
#define KEYBOARD_CODE_UNDO               0x7A
#define KEYBOARD_CODE_CUT                0x7B
#define KEYBOARD_CODE_COPY               0x7C
#define KEYBOARD_CODE_PASTE              0x7D
#define KEYBOARD_CODE_FIND               0x7E
#define KEYBOARD_CODE_MUTE               0x7F
#define KEYBOARD_CODE_VOLUME_UP          0x80
#define KEYBOARD_CODE_VOLUME_DOWN        0x81
#define KEYBOARD_CODE_CAPS_LOCK          0x82
#define KEYBOARD_CODE_NUM_LOCK           0x83
#define KEYBOARD_CODE_SCROLL_LOCK        0x84
#define KEYBOARD_CODE_NUM_COMMA          0x85
#define KEYBOARD_CODE_NUM_EQUAL_SIGN     0x86
#define KEYBOARD_CODE_INTERNATIONAL1     0x87
#define KEYBOARD_CODE_INTERNATIONAL2     0x88
#define KEYBOARD_CODE_INTERNATIONAL3     0x89
#define KEYBOARD_CODE_INTERNATIONAL4     0x8A
#define KEYBOARD_CODE_INTERNATIONAL5     0x8B
#define KEYBOARD_CODE_INTERNATIONAL6     0x8C
#define KEYBOARD_CODE_INTERNATIONAL7     0x8D
#define KEYBOARD_CODE_INTERNATIONAL8     0x8E
#define KEYBOARD_CODE_INTERNATIONAL9     0x8F
#define KEYBOARD_CODE_LANG1              0x90
#define KEYBOARD_CODE_LANG2              0x91
#define KEYBOARD_CODE_LANG3              0x92
#define KEYBOARD_CODE_LANG4              0x93
#define KEYBOARD_CODE_LANG5              0x94
#define KEYBOARD_CODE_LANG6              0x95
#define KEYBOARD_CODE_LANG7              0x96
#define KEYBOARD_CODE_LANG8              0x97
#define KEYBOARD_CODE_LANG9              0x98
#define KEYBOARD_CODE_ALTERNATE          0x99
#define KEYBOARD_CODE_SYSREQ             0x9A
#define KEYBOARD_CODE_CANCEL             0x9B
#define KEYBOARD_CODE_CLEAR              0x9C
#define KEYBOARD_CODE_PRIOR              0x9D
#define KEYBOARD_CODE_OTHER_RETURN       0x9E
#define KEYBOARD_CODE_SEPARATOR          0x9F
#define KEYBOARD_CODE_OUT                0xA0
#define KEYBOARD_CODE_OPER               0xA1
#define KEYBOARD_CODE_CLEAR_AGAIN        0xA2
#define KEYBOARD_CODE_CRSEL_PROPS        0xA3
#define KEYBOARD_CODE_EXSEL              0xA4
#define KEYBOARD_CODE_LEFTCONTROL        0xE0
#define KEYBOARD_CODE_LEFTSHIFT          0xE1
#define KEYBOARD_CODE_LEFTALT            0xE2
#define KEYBOARD_CODE_LEFTGUI            0xE3
#define KEYBOARD_CODE_RIGHTCONTROL       0xE4
#define KEYBOARD_CODE_RIGHTSHIFT         0xE5
#define KEYBOARD_CODE_RIGHTALT           0xE6
#define KEYBOARD_CODE_RIGHTGUI           0xE7

#endif /* KEYBOARD_KEYCODES_H_ */
