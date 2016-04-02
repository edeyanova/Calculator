#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "Server.hpp"
#include <pthread.h>
#include "../libcommon/Libcommon.hpp"

void *taskRead(void * data)
{
	Server *server =(Server*)data;

	while (1)
	{
		Libcommon::CalculationValues * values = server->read();
		if (values != NULL)
		{
			std::cout<<"before distributeData values "<<values->first<<std::endl;
			std::cout<<"before distributeData values "<<values->second<<std::endl;

			server->distributeData(values);
		}
		sleep(5);
	}
}

void *taskSumSub(void * data)
{
	Server * server = (Server*)data;

	while (1)
	{
		if(! server->operationPlusMin.empty())
		{
			Libcommon::CalculationValues * values = (Libcommon::CalculationValues *)server->operationPlusMin.front();

			if(values->operation == 1)
				server->getSum(values);
			else
				server->getSub(values);
		}
		sleep(5);
	}
	return NULL;
}

void *taskMulDiv(void * data)
{
	Server * server = (Server*)data;
	while (1)
	{
		if(! server->operationMulDiv.empty())
		{
			Libcommon::CalculationValues * values = (Libcommon::CalculationValues *)server->operationMulDiv.front();

			if(values->operation == 3)
				server->getMul(values);

			if(values->operation == 4)
				server->getDiv(values);
		}
		sleep(5);
	}
	return NULL;
}

Server::Server()
: memory(NULL)
{
}

Server::~Server()
{
	if(NULL != memory)
		delete memory;
}

void Server::run()
{
	std::cout<<std::endl;
	memory = new Libcommon();
	memory->connectToMemory();

	Libcommon::CalculationValues * values;

	pthread_create(&threadRead, NULL, taskRead, this);
	pthread_create(&threadSumSub, NULL, taskSumSub, this);
	pthread_create(&threadMulDiv, NULL, taskMulDiv, this);

	pthread_join(threadRead, NULL);
	pthread_join(threadSumSub, NULL);
	pthread_join(threadMulDiv, NULL);
}

void Server::stop()
{
	memory->disconnectFromMemory();
	memory->deleteMemory();
}

void Server::distributeData(Libcommon::CalculationValues * values)
{

	if (values->operation == 1 || values->operation == 2)
	{
		insertDataForSumSub(values);
	}
	else if (values->operation == 3 || values->operation == 4)
	{
		insertDataForMulDiv(values);
	}
}

Libcommon::CalculationValues * Server::read()
{
	Libcommon::CalculationValues * values = NULL;
	values =  memory->memoryRead();

	if (values != NULL)
	{

		if (values->whoWrites == Libcommon::CLIENT && values->memoryStatus == Libcommon::FULL)
		{
			memory->setMemoryEmpty();
		}
		else
		{
			delete values;
			values = NULL;
		}
	}
	std::cout<<"Values in server read: "<<values<<std::endl;
	return values;
}

bool Server::write(Libcommon::CalculationValues * newValues)
{
	std::string s = memory->createId();

	newValues->whoWrites = Libcommon::SERVER;

	bool result;
	do
	{
		result = memory->memoryWrite(newValues);
	}
	while(result == true);

	return true;
}

void Server::printServer()
{
	memory->printMemory();
}

void Server::getSum(Libcommon::CalculationValues * data)
{
	data->result = data->first + data->second;
	std::cout<<"Printing the result from sum operation... "<<std::endl;
	std::cout <<"sum result " <<  data->result << std::endl;

	write(data);
}

void Server::getSub(Libcommon::CalculationValues * data)
{
	data->result = data->first - data->second;
	std::cout<<"Printing the result from subtraction operation... "<<std::endl;
	std::cout <<"sub result " << data->result<<std::endl;

	write(data);
}

void Server::getMul(Libcommon::CalculationValues * data)
{
	data->result = data->first * data->second;
	std::cout<<"Printing the result from multiplication operation... "<<std::endl;
	std::cout <<"mul result " << data->result << std::endl;

	write(data);
}

void Server::getDiv(Libcommon::CalculationValues * data)
{
	data->result = data->first / data->second;
	std::cout<<"Printing the result from division operation... "<<std::endl;
	std::cout<<"div result " << data->result<<std::endl;

	write(data);
}

void Server::insertDataForSumSub(Libcommon::CalculationValues * values)
{
	operationPlusMin.push(values);
}

void Server::deleteDataFromSumSub()
{
	operationPlusMin.pop();
}

void Server::insertDataForMulDiv(Libcommon::CalculationValues * values)
{
	operationMulDiv.push(values);
}

void Server::deleteDataFromMulDiv()
{
	operationMulDiv.pop();
}
