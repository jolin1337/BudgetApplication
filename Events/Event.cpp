#include "Event.h"

const char Event::ID = 0;
Event::Event(Date date, MONEY money):timeStamp(date), cash(money) {
}
Event::Event():timeStamp(Date()), cash(0) {}

void Event::time(Date time) {
	timeStamp = time;
}
Date Event::time() const {
	return timeStamp;
}
void Event::money(MONEY m){
	cash = m;
}
MONEY Event::money() const {
	return cash;
}
char Event::id() const {
	return Event::ID;
}

bool operator <(const Event &lhs, const Event &rhs){ 
	return lhs.time() < rhs.time();
}
std::ostream &operator <<(std::ostream &o, const Event &rhs) {
	o << (time_t)rhs.time() << ";" << rhs.money();
	return o;
}

std::istream &operator >>(std::istream &i, Event &rhs) {
	MONEY cash;
	time_t t;
	i >> t;
	if(i.get() != ';') {
		i.putback(t);
		return i;
	}
	i >> cash;
	rhs.time(Date(t));
	rhs.money(cash);
	return i;
}