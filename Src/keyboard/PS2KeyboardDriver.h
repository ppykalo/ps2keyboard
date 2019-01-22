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
	void handleClockInterrupt();

	KeyState getKeyCode() override;
	bool setLedState(uint8_t led_number, bool on) override;

private:
	uint8_t peekReceivedByte(uint8_t byte_index);
	uint8_t getReceivedBytesCount();

	void receiveStartBit();
	void receiveDataBit();
	void receiveParityBit();
	void receiveStopBit();
	void storeReceivedByte(uint8_t value);

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

template<class keyboardPort>
inline void PS2KeyboardDriver<keyboardPort>::handleClockInterrupt()
{
 // TODO: handle shift DATA out
	if (shiftOutBitsCount > 0) {
		bool dataState = shiftOutBuffer & 0x01;
		shiftOutBuffer = shiftOutBuffer >> 1;
		keyboardPort::dataWrite(dataState);
		return;
	}

 // handle shift DATA in
	++shiftInTicksCount;

	if (shiftInTicksCount == 1)
		receiveStartBit();

	if (shiftInTicksCount >= 2 && shiftInTicksCount <= 9)
		receiveDataBit();

	if (shiftInTicksCount == 10)
		receiveParityBit();

	if (shiftInTicksCount == 11)
		receiveStopBit();
}

template<class keyboardPort>
inline void PS2KeyboardDriver<keyboardPort>::receiveStartBit()
{
	uint8_t startBit = keyboardPort::dataRead();

	if (startBit == true) { // not START bit - ignore
		++ps2StartFaults;
		return;
	}

}

template<class keyboardPort>
inline void PS2KeyboardDriver<keyboardPort>::receiveDataBit()
{
	uint8_t dataBit = keyboardPort::dataRead();

	shiftInBuffer = shiftInBuffer >> 1;
	if (dataBit == true) {
		shiftInBuffer |= (dataBit << 7);
		shiftInNotParity = !shiftInNotParity;
	}
}

template<class keyboardPort>
inline void PS2KeyboardDriver<keyboardPort>::receiveParityBit()
{
	uint8_t parityBit = keyboardPort::dataRead();

	if (shiftInNotParity == parityBit) {
		++ps2ParityFaults;
	}
}

template<class keyboardPort>
inline void PS2KeyboardDriver<keyboardPort>::receiveStopBit()
{
	uint8_t stopBit = keyboardPort::dataRead();

	if (stopBit == false) { // not a STOP bit - ignore
		++ps2StopFaults;
		return;
	}

	shiftInTicksCount = 0;
	shiftInNotParity = false;
	storeReceivedByte(shiftInBuffer);
	shiftInBuffer = 0;
	++totalCodesReceived;
}

template<class keyboardPort>
inline void PS2KeyboardDriver<keyboardPort>::storeReceivedByte(uint8_t value)
{
	uint8_t nextReceiveBufferIndex = (receiveBufferIndex + 1) % 40;
	if (nextReceiveBufferIndex == readBufferIndex) {
		++receiveOverflows;
		return;
	}
	receiveBuffer[receiveBufferIndex] = value;
	receiveBufferIndex = nextReceiveBufferIndex;
	receiveBuffer[receiveBufferIndex] = 0;
}

template<class keyboardPort>
inline KeyState PS2KeyboardDriver<keyboardPort>::getKeyCode()
{
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

template<class keyboardPort>
inline bool PS2KeyboardDriver<keyboardPort>::setLedState(uint8_t led_number, bool on)
{
	return false;
}

template<class keyboardPort>
inline uint8_t PS2KeyboardDriver<keyboardPort>::peekReceivedByte(uint8_t byte_index)
{
	if (byte_index >= getReceivedBytesCount() || byte_index >= receiveBuffer.size())
		return 0;
	else
		return receiveBuffer[readBufferIndex + byte_index];
}

template<class keyboardPort>
inline uint8_t PS2KeyboardDriver<keyboardPort>::getReceivedBytesCount()
{
	if (receiveBufferIndex >= readBufferIndex)
		return receiveBufferIndex - readBufferIndex;
	else
		return receiveBuffer.size() - readBufferIndex + receiveBufferIndex;
}

#endif /* KEYBOARD_PS2KEYBOARDDRIVER_H_ */
