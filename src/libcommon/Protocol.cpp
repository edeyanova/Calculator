/*
 * Protocol.cpp
 *
 *  Created on: Feb 16, 2016
 *      Author: visteon
 */

#include "Protocol.h"

Protocol::Protocol()
: operation(1)
, first(5)
, second(6)
, result(0)
, clientID("")
//, isLocked(LOCKED)
//, whoWrites(WHO_WRITES_CLIENT)
{
	isLocked = LOCKED;
	whoWrites = WHO_WRITES_CLIENT;
}

Protocol::~Protocol() {
	// TODO Auto-generated destructor stub
}

