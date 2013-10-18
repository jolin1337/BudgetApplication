#ifndef REPEATABLEEVENT_H
#define REPEATABLEEVENT_H 

#include "Date/Period.h"
#include "Event.h"

class REvent : public Event {
public:
	static const char ID;
	REvent(Date start=Date(), Date end=Date(), MONEY money=0);
	void endTime(Date time);
	Date endTime() const;

	char id() const;
private:
	Date end;
};

std::ostream &operator <<(std::ostream &o, const REvent &rhs);
std::istream &operator >>(std::istream &o, REvent &rhs);

#endif