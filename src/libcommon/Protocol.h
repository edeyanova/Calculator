/*
 * Protocol.h
 *
 *  Created on: Feb 16, 2016
 *      Author: visteon
 */

#ifndef LIBCOMMON_PROTOCOL_H_
#define LIBCOMMON_PROTOCOL_H_
#include <string>

class Protocol {
public:
	Protocol();
	//	Protocol(Protocol &values);
	virtual ~Protocol();

	enum WhoWritesEnum {
		SERVER,
		WHO_WRITES_CLIENT
	};

	enum IsLockedEnum {
		LOCKED = 0,
		UNLOCKED = 1
	};


//	enum operation {sum, sub, mul, div};
//	OperationEnum operation;

	int operation;
	int first;
	int second;
	int result;
	std::string clientID;
	enum IsLockedEnum isLocked;
	WhoWritesEnum whoWrites;

};

#endif /* LIBCOMMON_PROTOCOL_H_ */
