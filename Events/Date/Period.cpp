#include "Period.h"


Period::Period(Date start, Date end):st(start), en(end) {}

void Period::start(Date start) {
	st = start;
}
void Period::end(Date end) {
	en = end;
}
Date Period::start() const {
	return st;
}
Date Period::end() const {
	return en;
}