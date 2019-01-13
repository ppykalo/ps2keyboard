/*
 * PS2KeyboardDriver.h
 *
 *  Created on: Jan 3, 2019
 *      Author: pawell
 */

#ifndef KEYBOARD_PS2KEYBOARDDRIVER_H_
#define KEYBOARD_PS2KEYBOARDDRIVER_H_

#include "IKeyboardDriver.h"

#include <array>
#include <map>

typedef uint16_t PS2PhysicalKeyCode;
extern const std::map<PS2PhysicalKeyCode,VirtualKeyCode> PS2KeyboardDriver_PhysicalToVirtualKeyCodeMap;

template<class keyboardPort>
class PS2KeyboardDriver : public IKeyboardDriver {
public:
	inline void handleClockInterrupt() {
	 // TODO: handle shift DATA out
		if (shiftOutBitsCount > 0) {
			bool dataState = shiftOutBuffer & 0x01;
			shiftOutBuffer = shiftOutBuffer >> 1;
			keyboardPort::dataWrite(dataState);
			return;
		}

	 // handle shift DATA in
		++shiftInTicksCount;
		uint8_t dataLineState = keyboardPort::dataRead();

		if (shiftInTicksCount == 1 && dataLineState == true) { // not START bit - ignore
			++ps2StartFaults;
			return;
		}

		if (shiftInTicksCount == 11 && dataLineState == false) { // not a STOP bit - ignore
			++ps2StopFaults;
			return;
		}

		if (shiftInTicksCount >= 2 && shiftInTicksCount <= 9) {
			shiftInBuffer = shiftInBuffer >> 1;
			if (dataLineState == true) {
				shiftInBuffer |= (dataLineState << 7);
				shiftInNotParity = !shiftInNotParity;
			}
		}

		if (shiftInTicksCount == 10) {
			bool parityBit = dataLineState;
			if (shiftInNotParity == parityBit) {
				++ps2ParityFaults;
			}
		}

		if (shiftInTicksCount == 11) {
			shiftInTicksCount = 0;
			shiftInNotParity = false;
			uint8_t nextReceiveBufferIndex = (receiveBufferIndex + 1) % 40;
			if (nextReceiveBufferIndex == readBufferIndex) {
				++receiveOverflows;
				return;
			}
			receiveBuffer[receiveBufferIndex] = shiftInBuffer;
			receiveBufferIndex = nextReceiveBufferIndex;
			receiveBuffer[receiveBufferIndex] = shiftInBuffer = 0;
			++totalCodesReceived;
		}
	}

	KeyState getKeyCode() override {
		KeyState result;
		uint8_t bytesRead = 0;
		PS2PhysicalKeyCode physicalCode;
		uint8_t firstByte = peekReceivedByte(0);
		++bytesRead;

		switch (firstByte) {
		{
			case 0x00:
			 // no code - return invalid state
				return KeyState::invalid();
			}
			case 0xF0:
			{
			 // break code - physical code is the 2nd byte
				result.isReleased = true;
				physicalCode = peekReceivedByte(1);
				++bytesRead;
				if (physicalCode == 0x00)
					return KeyState::invalid();
				break;
			}
			case 0xE0:
			case 0xE1:
			{
			 // extended code - need to read at least one more
				physicalCode = firstByte;
				uint8_t extendedByte = peekReceivedByte(1);
				++bytesRead;
				switch (extendedByte) {
					case 0x00:
					{
					 // no code - return invalid state
						return KeyState::invalid();
					}
					case 0xF0:
					{
					 // break code - physical code is 1st and 3rd byte
						result.isReleased = true;
						extendedByte = peekReceivedByte(2);
						++bytesRead;
						if (extendedByte == 0x00)
							return KeyState::invalid();
						physicalCode = (physicalCode << 8) | extendedByte;
						break;
					}
					default:
					{
					 // regular code - physical code is 1st and 2nd byte
						result.isPressed = true;
						physicalCode = (physicalCode << 8) | extendedByte;
						break;
					}
				}
				break;
			}
			default:
			{
			 // regular code
				result.isPressed = true;
				physicalCode = firstByte;
				break;
			}
		}

	 // special case for Pause-Break key
		if (physicalCode == 0xE114) {
			if (result.isPressed) {
			 // key press
				if (peekReceivedByte(0+bytesRead) == 0x77) {
				 // OK - initial 0xE114 is followed by 0x77
					++bytesRead;
				} else {
				 // not enough data received yet
					return KeyState::invalid();
				}
			} else if (result.isReleased) {
			 // key release
				if (peekReceivedByte(0+bytesRead) == 0xF0 && peekReceivedByte(0+bytesRead+1) == 0x77) {
				 // OK - initial 0xE1F014 followed by 0xF077
					bytesRead += 2;
				} else {
				 // not enough data received yet
					return KeyState::invalid();
				}
			}
		}

	 // no matter if key can be found, at worst it will be ignored
		readBufferIndex = (readBufferIndex + bytesRead) % 40;

	 // find virtual key code
		auto itPhysicalToVirtualPair = PS2KeyboardDriver_PhysicalToVirtualKeyCodeMap.find(physicalCode);
		if (itPhysicalToVirtualPair != PS2KeyboardDriver_PhysicalToVirtualKeyCodeMap.end()) {
			result.code = itPhysicalToVirtualPair->second;
		} else {
			return KeyState::invalid();
		}

		return result;
	}

	bool setLedState(uint8_t led_number, bool on) override {
		return false;
	}

private:
	uint8_t peekReceivedByte(uint8_t byte_index) {
		if (byte_index >= getReceivedBytesCount() || byte_index >= receiveBuffer.size())
			return 0;
		else
			return receiveBuffer[readBufferIndex + byte_index];
	}

	uint8_t getReceivedBytesCount() {
		if (receiveBufferIndex >= readBufferIndex)
			return receiveBufferIndex - readBufferIndex;
		else
			return receiveBuffer.size() - readBufferIndex + receiveBufferIndex;
	}

	uint8_t shiftInBuffer = 0;
	uint8_t shiftInTicksCount = 0;
	bool shiftInNotParity = false;

	uint16_t shiftOutBuffer = 0;
	uint8_t shiftOutBitsCount = 0;

//	uint8_t receiveBuffer[40] = {};
	std::array<uint8_t, 40> receiveBuffer = {};
	uint8_t receiveBufferIndex = 0;

 // statistics:
	uint16_t ps2StartFaults = 0;
	uint16_t ps2StopFaults = 0;
	uint16_t ps2ParityFaults = 0;
	uint32_t receiveOverflows = 0;
	uint32_t totalCodesReceived = 0;

	uint8_t readBufferIndex = 0;
};


#endif /* KEYBOARD_PS2KEYBOARDDRIVER_H_ */
