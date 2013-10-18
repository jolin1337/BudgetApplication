#ifndef EVENT_H
#define EVENT_H 

#include "Date/Date.h"
#include <string>
#include <iostream>

typedef double MONEY;
class Event {
public:
	static const char ID;
	Event();
	Event(Date date, MONEY money);

	void time(Date time);
	Date time() const;
	void money(MONEY m);
	MONEY money() const;

	virtual char id() const;

private:
	Date timeStamp;	// time when this event was added
	MONEY cash;	// money spent/gaind under this event
	//std::string comment; 	// a short description of the evet (not nessesary)?
};


bool operator <(const Event &lhs, const Event &rhs);
std::ostream &operator <<(std::ostream &o, const Event &rhs);
std::istream &operator >>(std::istream &o, Event &rhs);

#endif