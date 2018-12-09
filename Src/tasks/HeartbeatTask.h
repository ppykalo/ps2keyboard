/*
 * HeartbeatTask.h
 *
 *  Created on: 03.11.2018
 *      Author: papyk1
 */

#ifndef TASKS_HEARTBEATTASK_H_
#define TASKS_HEARTBEATTASK_H_

#include "RtosTask.h"

class HeartbeatTask: public RtosTask {
private:
	void setup() override;
	void loop() override;
};

#endif /* TASKS_HEARTBEATTASK_H_ */
