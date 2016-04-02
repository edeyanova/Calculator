#ifndef LIBCOMMON_PROTOCOL_H_
#define LIBCOMMON_PROTOCOL_H_
#include <string>

class Protocol {
public:
	Protocol();
	
	virtual ~Protocol();

	enum WhoWritesEnum {
		SERVER,
		WHO_WRITES_CLIENT
	};

	enum IsLockedEnum {
		LOCKED = 0,
		UNLOCKED = 1
	};

	int operation;
	int first;
	int second;
	int result;
	std::string clientID;
	enum IsLockedEnum isLocked;
	WhoWritesEnum whoWrites;

};

#endif /* LIBCOMMON_PROTOCOL_H_ */
