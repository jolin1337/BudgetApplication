#include "REvent.h"

const char REvent::ID = Event::ID+1;

REvent::REvent(Date start, Date end, MONEY money):Event(start, money), end(end) {}


void REvent::endTime(Date time) {
	end = time;
}
Date REvent::endTime() const {
	return end;
}

char REvent::id() const {
	return REvent::ID;
}

std::ostream &operator <<(std::ostream &o, const REvent &rhs) {
	o << (time_t)rhs.time() << ":" << (time_t)rhs.endTime() << ";" << rhs.money();
}
std::istream &operator >>(std::istream &i, REvent &rhs) {

	MONEY cash;
	time_t st, et;
	i >> st;
	if(i.get() != ':') {
		i.putback(st);
		return i;
	}
	i >> et;
	if(i.get() != ';') {
		i.putback(st);
		return i;
	}
	i >> cash;
	rhs.time(Date(st));
	rhs.endTime(Date(et));
	rhs.money(cash);
	return i;
}