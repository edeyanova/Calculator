#include "Libcommon.hpp"
#include <iostream>
#include <syscall.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <sstream>
#include <semaphore.h>
#include <stdexcept>
#include <errno.h>

Libcommon::CalculationValues::CalculationValues()
: operation(1)
, first(5)
, second(6)
, result(0)
{
	memoryStatus = Libcommon::EMPTY;
	whoWrites = Libcommon::CLIENT;
}

Libcommon::Libcommon()
: sizeId(10)
, key(56)
, shmid(0)
, memorySize(sizeof(CalculationValues))
, sharedMemory(NULL)
, access(true)
, nsems(1)
, needInit(true)
{
}

Libcommon::~Libcommon()
{
}

std::string Libcommon::createId()
{
	std::string id;

	pid_t pid = getpid();
	pthread_t tid = pthread_self();

	std::stringstream s;
	s<<pid<<tid;
	id = s.str();
	return id;
}

int Libcommon::getSizeOfId()
{
	return sizeId;
}

int Libcommon::connectToMemory()
{
	createMemory();
	attachSegment();
//	std::cout<<"Attach returns "<<attachSegment()<<std::endl;
}

int Libcommon::disconnectFromMemory()
{
	detachSegment();
}

int Libcommon::deleteMemory()
{
	destroyMemory();
}

void Libcommon::printMemory()
{
	std::cout<<"---------Print Memory---------"<<std::endl;
	std::cout<<"Operation: "<<sharedMemory->operation<<std::endl;
	std::cout<<"First: "<<sharedMemory->first<<std::endl;
	std::cout<<"Second: "<<sharedMemory->second<<std::endl;
	std::cout<<"Result: "<<sharedMemory->result<<std::endl;
	std::cout<<"clientID: "<<sharedMemory->clientID<<std::endl;
	std::cout<<"Memory Status-> Empty - 0, FULL - 1: "<<sharedMemory->memoryStatus<<std::endl;
	std::cout<<"WhoWrites-> Server - 0, Client - 1: "<<sharedMemory->whoWrites<<std::endl;
}

void Libcommon::printCalculationValues(const Libcommon::CalculationValues & values)
{
	std::cout<<"---------Print Calculation values---------"<<std::endl;
	std::cout<<"Operation: "<<values.operation<<std::endl;
	std::cout<<"First: "<<values.first<<std::endl;
	std::cout<<"Second: "<<values.second<<std::endl;
	std::cout<<"Result: "<<values.result<<std::endl;
	std::cout<<"clientID: "<<values.clientID<<std::endl;
	std::cout<<"Memory status-> Empty - 0, Full - 1: "<<values.memoryStatus<<std::endl;
	std::cout<<"WhoWrites-> Server - 0, Client - 1: "<<values.whoWrites<<std::endl;
}

bool Libcommon::memoryWrite(Libcommon::CalculationValues * values)
{
	bool result = false;
	getAccess(sharedMemory->accessToMemory);

	if (sharedMemory->memoryStatus == Libcommon::EMPTY)
	{
		std::cout<<"**************************memoryWrite empty "<<std::endl;

		sharedMemory->operation = values->operation;
		sharedMemory->first = values->first;
		sharedMemory->second = values->second;
		sharedMemory->result = values->result;
		sharedMemory->whoWrites = values->whoWrites;

		for(int i = 0; i < sizeId-1; i++)
		{
			sharedMemory->clientID[i] = values->clientID[i];
		}
		sharedMemory->clientID[sizeId];

		sharedMemory->memoryStatus = Libcommon::FULL;

		result = true;
	}
	releaseAccess(sharedMemory->accessToMemory);

	return result;
}

Libcommon::CalculationValues * Libcommon::memoryRead()
{
	getAccess(sharedMemory->accessToMemory);

	Libcommon::CalculationValues * tempRead = NULL;

	if (sharedMemory->memoryStatus == Libcommon::FULL)
	{
		tempRead = new CalculationValues();

		tempRead->operation = sharedMemory->operation;

		tempRead->first = sharedMemory->first;
		tempRead->second = sharedMemory->second;

		for (int i = 0; i < sizeId - 1; i++)
		{
			tempRead->clientID[i] = sharedMemory->clientID[i];
		}
		tempRead->clientID[sizeId];
		tempRead->memoryStatus = sharedMemory->memoryStatus;
		tempRead->whoWrites = sharedMemory->whoWrites;
	}

	releaseAccess(sharedMemory->accessToMemory);

	return tempRead;
}

void Libcommon::initSem(sem_t &sem)
{
	if (sem_init(&sem, 1, 1) == -1)
	{
		std::cout<<"sem init failed "<<std::endl;
	}
	std::cout<<"sem init "<<std::endl;
}

void Libcommon::destroySem(sem_t &sem)
{
	sem_destroy(&sem);
}

int Libcommon::getValueSem(sem_t &sem)
{
	if (sem_getvalue(&sem, &nsems) == -1)
	{
		std::cout<<"sem_getValue failed "<<std::endl;
	}
	return nsems;
}

void Libcommon::getAccess(sem_t &sem)
{
	sem_wait(&sem);
}

void Libcommon::releaseAccess(sem_t &sem)
{
	sem_post(&sem);
}

void Libcommon::setMemoryEmpty()
{
	sharedMemory->memoryStatus = Libcommon::EMPTY;
}

int Libcommon::createMemory()
{
	if ((shmid = shmget(key, memorySize, IPC_CREAT | IPC_EXCL | 0666)) < 0)
	{
		std::cout<< "shmget err shmem exist shmid" << shmid<< std::endl;
		needInit = false;

		if ((shmid = shmget(key, memorySize, IPC_CREAT | 0666)) < 0)
		{
			std::cout<< "shmget err can not get" << shmid << std::endl;
			return 1;
		}
	}
	return 0;
}

int Libcommon::destroyMemory()
{
	if ((shmctl(shmid, IPC_RMID, 0)) == -1)
	{
		std::cout<<"shmremove err"<< std::endl;
		return 1;
	}
	return 0;
}

int Libcommon::attachSegment()
{
	std::cout<<"attach"<< std::endl;
	if ((sharedMemory = (CalculationValues *) shmat(shmid, 0, 0)) == (CalculationValues *) - 1)
	{
		std::cout<<"shmat error" << strerror(errno) << std::endl;
		return 1;
	}

	if (needInit == true)
	{
		setMemoryEmpty();

		initSem(sharedMemory->accessToMemory);
		std::cout<<"needInit "<< std::endl;
	}

	return 0;
}

int Libcommon::detachSegment()
{
	if (shmdt(sharedMemory) == -1)
	{
		std::cerr<<"shmdt err"<< std::endl;
		return 1;
	}
		return 0;
}
