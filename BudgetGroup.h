#ifndef BUDGETGROUP
#define BUDGETGROUP 

#include "Budget.h"

class BudgetGroup : public Budget  {
public:
	BudgetGroup();
	BudgetGroup(std::string name);

private:
	std::string name;
	MONEY maximum;
};

#endif