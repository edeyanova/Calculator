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
	newValues->clientID[memory->getSizeOfId()]=0;
	memcpy(newValues->clientID,id.c_str(), memory->getSizeOfId()-1);

	newValues->whoWrites = Libcommon::CLIENT;

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
			memory->setMemoryEmpty();
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





