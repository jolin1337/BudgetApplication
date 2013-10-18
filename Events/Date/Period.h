#ifndef PERIOD_H
#define PERIOD_H 

#include "Date.h"

class Period {
public:
	Period(Date, Date);

	void start(Date);
	void end(Date);
	Date start() const;
	Date end() const;
private:
	Date st, en;
};

#endif