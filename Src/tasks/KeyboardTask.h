/*
 * KeyboardTask.h
 *
 *  Created on: 03.11.2018
 *      Author: papyk1
 */

#ifndef TASKS_KEYBOARDTASK_H_
#define TASKS_KEYBOARDTASK_H_

#include "RtosTask.h"

template<class keyboardPort>
class KeyboardTask: public RtosTask {
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
			receiveBuffer[receiveBufferIndex] = shiftInBuffer;
			receiveBufferIndex = (receiveBufferIndex + 1) % 40;
			receiveBuffer[receiveBufferIndex] = shiftInBuffer = shiftInTicksCount = 0;
			shiftInNotParity = false;
			++totalCodesReceived;
		}
	}

private:
	void setup() override {}
	void loop() override {
	 // TODO: process SCAN CODE -> ASCII code
		this->waitForByte();
	}

	uint8_t waitForByte() {
	 // wait for code in receiveBuffer
		while (receiveBuffer[processedBufferIndex] == 0x00) {
			sleep(1);
		}

		uint8_t receivedByte = receiveBuffer[processedBufferIndex];
		processedBufferIndex = (processedBufferIndex + 1) % 40;

		return receivedByte;
	}

	uint8_t shiftInBuffer = 0;
	uint8_t shiftInTicksCount = 0;
	bool shiftInNotParity = false;

	uint16_t shiftOutBuffer = 0;
	uint8_t shiftOutBitsCount = 0;

	uint8_t receiveBuffer[40] = {};
	uint8_t receiveBufferIndex = 0;

 // statistics:
	uint16_t ps2StartFaults = 0;
	uint16_t ps2StopFaults = 0;
	uint16_t ps2ParityFaults = 0;
	uint32_t totalCodesReceived = 0;

	uint8_t processedBufferIndex = 0;
};

#endif /* TASKS_KEYBOARDTASK_H_ */
