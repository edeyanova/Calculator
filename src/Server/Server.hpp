#ifndef SERVER_HPP_
#define SERVER_HPP_
#include "../libcommon/Libcommon.hpp"
#include <queue>
#include <pthread.h>

void *taskRead(void * data);

void *taskSumSub(void * data);

void *taskMulDiv(void * data);

class Server
{
public:
	Server();
	virtual ~Server();

	void run();
	void stop();

	Libcommon::CalculationValues * read();
	bool write(Libcommon::CalculationValues * newValues);
	void printServer();

	void getSum(Libcommon::CalculationValues * data);
	void getSub(Libcommon::CalculationValues * data);

	void getMul(Libcommon::CalculationValues * data);
	void getDiv(Libcommon::CalculationValues *data);

	std::queue<Libcommon::CalculationValues*> operationPlusMin;
	std::queue<Libcommon::CalculationValues*> operationMulDiv;

	void insertDataForSumSub(Libcommon::CalculationValues * values);
	void deleteDataFromSumSub();

	void insertDataForMulDiv(Libcommon::CalculationValues * values);
	void deleteDataFromMulDiv();

	void distributeData(Libcommon::CalculationValues * values);

private:
	Libcommon * memory;
	pthread_t threadRead;
	pthread_t threadSumSub;
	pthread_t threadMulDiv;

};

#endif /* SERVER_HPP_ */
