#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Client.hpp"
#include "../libcommon/Libcommon.hpp"
#include <semaphore.h>
#include <pthread.h>
#include <queue>

sem_t sem;
std::queue<Libcommon::CalculationValues*> queueData;

void addData(Libcommon::CalculationValues * values)
{
	std::cout<<"addData in a queue "<<std::endl;
	queueData.push(values);
}

void deleteData()
{
	queueData.pop();
}

bool menu(Libcommon::CalculationValues * values)
{
	int oper;
	int firstNumber;
	int secondNumber;
	int res;

	std::cout<<" ----------Menu---------- " << std::endl;
	std::cout<<"1 - Sum"<<std::endl;
	std::cout<<"2 - Subtraction"<<std::endl;
	std::cout<<"3 - Multiplication"<<std::endl;
	std::cout<<"4 - Division"<<std::endl;
	std::cout<<"5 - Exit"<<std::endl;

	std::cout<<"Enter operation: "<<std::endl;
	std::cin>>oper;

	std::cout<<"Enter first number: "<<std::endl;
	std::cin>>firstNumber;
	std::cout<<"Enter second number: "<<std::endl;
	std::cin>>secondNumber;

	switch (oper)
	{
		case 1 :
		{
			int sum = firstNumber + secondNumber;
			res = sum;
			values->operation = oper;
			values->first = firstNumber;
			values->second = secondNumber;
			values->result = res;
			addData(values);
			sem_wait(&sem);
			break;
		}
		case 2 :
		{

			int sub;
			if (firstNumber > secondNumber)
			{
				sub = firstNumber - secondNumber;
			}
			else {
				sub = secondNumber - firstNumber;
			}
			res = sub;
			values->operation = oper;
			values->first = firstNumber;
			values->second = secondNumber;
			values->result = res;
			addData(values);
			break;
		}
		case 3 :
		{
			int mul;
			mul = firstNumber * secondNumber;
			res = mul;
			values->operation = oper;
			values->first = firstNumber;
			values->second = secondNumber;
			values->result = res;
			addData(values);
			break;
		}
		case 4 :
		{
			int div;
			if (firstNumber > secondNumber)
			{
				div = firstNumber / secondNumber;
			}
			else
			{
				div = secondNumber / firstNumber;
			}
			res = div;
			values->operation = oper;
			values->first = firstNumber;
			values->second = secondNumber;
			values->result = res;
			addData(values);
			break;
		}
		default:
		{
			std::cout<<"Invalid selection."<<std::endl;
			return false;
			break;
		}
	}
	return true;
}

void * threadReadHandler(void * data)
{
	Client * client = (Client*)data;

	while (1)
	{
		std::cout<<"Read------------------------Handler "<<std::endl;
		Libcommon::CalculationValues * values = client->readCalculationValues();
		if(NULL != values)
		{
			client->printCalculationValues(*values);

			if(values->operation == 1)
			{
				sem_post(&sem);
			}
		}
		sleep(5);
	}
	return NULL;
}


void * threadWriteHandler(void * data)
{
	Client * client = (Client*)data;
	Libcommon::CalculationValues * values = NULL;

	while (1)
	{

		std::cout<<"-------------Write-------------Handler "<<std::endl;
		if (! queueData.empty())
		{
			values = (Libcommon::CalculationValues *)queueData.front();

			client->writeCalculationValues(values);

			client->printMemory();

			deleteData();
		}
		sleep(5);
	}

	return values;
}

void runClient()
{
	Client * client = new Client();
	Libcommon::CalculationValues * values = new Libcommon::CalculationValues();
	pthread_t threadRead;
	pthread_t threadWrite;

	pthread_create(&threadWrite, NULL, threadWriteHandler, client);
	pthread_create(&threadRead, NULL, threadReadHandler, client);

	do
	{
		bool retVal = menu(values);
		if(retVal == false)
			return;
		std::cout<<std::endl;
	}while(1);

	pthread_join(threadWrite, NULL);
	pthread_join(threadRead, NULL);

	delete values;
	delete client;
}

int main()
{
	if (sem_init(&sem, 0, 0) == -1)
	{
		std::cout<<"sem init failed "<<std::endl;
	}

	runClient();

	return 0;
}
