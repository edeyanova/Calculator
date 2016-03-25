/*
 * Client.h
 *
 *  Created on: Feb 2, 2016
 *      Author: visteon
 */

#ifndef CLIENT_HPP_
#define CLIENT_HPP_
#include "../libcommon/Libcommon.hpp"

class Client
{
public:
	Client();
	virtual ~Client();

//	void writeCalculationValues(Libcommon::CalculationValues * newValues);
	bool writeCalculationValues(Libcommon::CalculationValues * newValues);
	Libcommon::CalculationValues * readCalculationValues();
	void printMemory();
	void printCalculationValues(const Libcommon::CalculationValues & values);


private:
	Libcommon * memory;
	std::string id;
};

#endif /* CLIENT_HPP_ */
