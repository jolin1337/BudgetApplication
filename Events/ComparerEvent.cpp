#include "ComparerEvent.h"

ComparerEvent::ComparerEvent() {
	sortDate = true;
}


bool ComparerEvent::operator() (const Event *one, const Event *two) {
	if(sortDate)
		return date(one, two);
	return money(one, two);
		// return (*one) < (*two);
}
bool ComparerEvent::date(const Event *one, const Event *two) {
	return one->time().time() < two->time().time();
}
bool ComparerEvent::money(const Event *one, const Event *two) {
	return one->money() < two->money();
}

void ComparerEvent::date() {
	sortDate = true;
}
void ComparerEvent::money() {
	sortDate = false;
}