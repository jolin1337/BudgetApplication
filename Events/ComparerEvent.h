#ifndef COMPAREREVENT_H
#define COMPAREREVENT_H

#include "Event.h"

class ComparerEvent {
public:
	ComparerEvent();
	bool operator() (const Event *one, const Event *two);
	void date();
	void money();
private:
	bool date(const Event *one, const Event *two);
	bool money(const Event *one, const Event *two);
	bool sortDate;
};

#endif