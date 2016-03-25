/*
 * Client.cpp
 *
 *  Created on: Feb 2, 2016
 *      Author: visteon
 */

#include "Client.hpp"
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "../libcommon/Libcommon.hpp"

Client::Client()
: memory(new Libcommon())
{
	memory->connectToMemory();
	id = memory->createId();
}

Client::~Client()
{

}

bool Client::writeCalculationValues(Libcommon::CalculationValues * newValues)
{
//	std::cout<<"inside client writeCalculationValues"<<std::endl;
	newValues->clientID[memory->getSizeOfId()]=0;
	memcpy(newValues->clientID,id.c_str(), memory->getSizeOfId()-1);

	newValues->whoWrites = Libcommon::CLIENT;
//	newValues->memoryStatus = Libcommon::FULL;

//	std::cout<<"Before memoryWrite in client"<<std::endl;
	std::cout<<"Memory status: "<<newValues->memoryStatus<<std::endl;
	std::cout<<"Who writes: "<<newValues->whoWrites<<std::endl;

	bool result;
	do
	{
		result = memory->memoryWrite(newValues);
	}
	while(result == false);

	return true;
}

Libcommon::CalculationValues * Client::readCalculationValues()
{
	Libcommon::CalculationValues * values = NULL;

	values = memory->memoryRead();

	if (values != NULL)
	{
		if (values->clientID == id && values->whoWrites == Libcommon::SERVER)
		{
//			std::cout<<"Before setMemoryEmpty in Client"<<std::endl;
			memory->setMemoryEmpty();
//			std::cout<<"Memory status "<<values->memoryStatus<<std::endl;
		}
		else
		{
			delete values;
			values = NULL;
		}
	}
	return values;
}

void Client::printMemory()
{
	memory->printMemory();
}

void Client::printCalculationValues(const Libcommon::CalculationValues & values)
{
	memory->printCalculationValues(values);
}





