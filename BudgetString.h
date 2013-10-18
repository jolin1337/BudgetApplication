#ifndef BUDGETSTRING
#define BUDGETSTRING 

#include "Budget.h"

class BudgetString : public Budget {
public:
	BudgetString(std::string file);

	int error(std::string err);

	std::string dateFormat(std::string date);

	void printPeriodStr(std::string optarg);

	int addEventStr(std::string optarg);
	int removeEventStr(std::string optarg);
	int editEventStr(std::string optarg);

	int linkEventGroupStr(std::string optarg);
	int unlinkEventGroupStr(std::string optarg);

	int editGroupStr(std::string optarg);
private:
	std::string filename;
};

#endif