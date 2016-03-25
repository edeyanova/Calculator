#ifndef LIBCOMMON_LIBCOMMON_HPP_
#define LIBCOMMON_LIBCOMMON_HPP_

#include <iostream>
#include <sys/shm.h>
#include <unistd.h>
#include <syscall.h>
#include <sys/types.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <semaphore.h>

class Libcommon
{

public:

	enum WhoWritesEnum
	{
		SERVER = 0,
		CLIENT = 1
	};

	enum MemoryStatusEnum
	{
		EMPTY = 0,
		FULL = 1
	};

	struct CalculationValues
	{
		int operation;
		int first;
		int second;
		int result;
		char clientID[10];
		MemoryStatusEnum memoryStatus;
		WhoWritesEnum whoWrites;
		sem_t accessToMemory;

		CalculationValues();
		CalculationValues(CalculationValues &values);
	};

	Libcommon();
	virtual ~Libcommon();

	std::string createId();
	int getSizeOfId();

	int connectToMemory();
	int disconnectFromMemory();

	int deleteMemory();

//	void memoryWrite(CalculationValues * values);
	bool memoryWrite(CalculationValues * values);
	CalculationValues * memoryRead();
	void printMemory();
	void printCalculationValues(const Libcommon::CalculationValues & values);

	void getAccess(sem_t &sem);
	void releaseAccess(sem_t &sem);
	void setMemoryEmpty();

private:
	int sizeId;
	key_t key;
	int shmid;
	CalculationValues * sharedMemory;
	const int memorySize;

	bool access;
	int nsems;

	bool needInit;

	int createMemory();
	int destroyMemory();

	int attachSegment();
	int detachSegment();

	void initSem(sem_t &sem);
	void destroySem(sem_t &sem);
	int getValueSem(sem_t &sem);

};

#endif /* LIBCOMMON_LIBCOMMON_HPP_ */
