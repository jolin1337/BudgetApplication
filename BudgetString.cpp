#include "BudgetString.h"
#include <iostream>
#include <sstream>

BudgetString::BudgetString(std::string file):Budget(file), filename(file) {}

int BudgetString::error(std::string err) {
	std::cerr << err << std::endl;
	return 1;
}

std::string BudgetString::dateFormat(std::string date) {
	int sla = date.find("/");
	if(date.find("/") != std::string::npos){
		if(date.find("-") != std::string::npos)
			return "dd/mm-yyyy";
		else
			return "dd/mm/yy";
	}
	if(date.find("-") != std::string::npos)
		return "yyyy-mm-dd";
	return "";
}

void BudgetString::printPeriodStr(std::string optarg) {
	int spl = optarg.find(" ");
	if(spl == std::string::npos) return;
	std::string from = optarg.substr(0, spl),
				to = optarg.substr(spl);
	if(to.find_first_of("-/") != std::string::npos) {
		printPeriod(Period(Date(from, dateFormat(from)), Date(to, dateFormat(to))));
		return;
	}
	Date d(from, dateFormat(from));
	Date dd(d);
	std::transform(to.begin(), to.end(), to.begin(), ::tolower);
	if(to == "week") dd.addWeek();
	else if(to == "month") dd.addMonth();
	else if(to == "year") dd.addYear();
	else if(to == "day") dd.addDay();
	else {
		std::stringstream ss;
		ss << to;
		int len;
		ss >> len;
		char c = ss.get();
		while(c == ' ' && !ss.eof()) c = ss.get();
	// std::cout << to << "\n";
		if(c == 'd') 
			for (int i = 0; i < len; i++)
				dd.addDay();
		else if(c == 'w') 
			for (int i = 0; i < len; i++)
				dd.addWeek();
		else if(c == 'm') 
			for (int i = 0; i < len; i++)
				dd.addMonth();
		else if(c == 'y') 
			for (int i = 0; i < len; i++)
				dd.addYear();
		else return;
	}
	printPeriod(Period(d, dd));
	// std::cout << optarg << "\n";
}

int BudgetString::addEventStr(std::string optarg) {
	int spl = optarg.find(" ");
	if(spl == std::string::npos) {
		if(optarg.size() == 0)
			return 0;
		std::stringstream ss;
		ss << optarg;
		MONEY money;
		ss >> money;
		addEvent(Event(Date(), money));
		save("MyBudget.bas");
		return 0;
	}
	std::string strMoney = optarg.substr(0, spl);
	std::stringstream ssmoney;
	ssmoney << strMoney;
	MONEY money;
	ssmoney >> money;


	std::string dates = optarg.substr(spl+1);
	spl = dates.find(" ");
	if(spl == std::string::npos) {
		Date d = Date(dates, dateFormat(dates));
		std::cout << d.date() << "\n";
		addEvent(Event(d, money));
		save("MyBudget.bas");
		return 0;
	}
	std::string date = dates.substr(0, spl),
				inteval = dates.substr(spl+1);
	Date d(date, dateFormat(date));
	Date dd(d);
	std::transform(inteval.begin(), inteval.end(), inteval.begin(), ::tolower);
	if(inteval == "weelky") dd.addWeek();
	else if(inteval == "monthly") dd.addMonth();
	else if(inteval == "daily") dd.addDay();
	else {
		std::stringstream ss;
		ss << inteval;
		while(!ss.eof()) {
			int len;
			ss >> len;
			char c = ss.get();
			if(c == 'd') 
				for (int i = 0; i < len; i++)
					dd.addDay();
			else if(c == 'w') 
				for (int i = 0; i < len; i++)
					dd.addWeek();
			else if(c == 'm') 
				for (int i = 0; i < len; i++)
					dd.addMonth();
			else if(c == 'y') 
				for (int i = 0; i < len; i++)
					dd.addYear();
			else if(c == -1) break;
			else return 0;
			std::string line;
			getline(ss, line, ' ');
		}
	}
	if(d.time() == dd.time())
		addEvent(Event(d, money));
	else
		addEvent(REvent(d, dd, money));
	save("MyBudget.bas");
	return 0;
}
int BudgetString::removeEventStr(std::string optarg) {
	std::stringstream ss;
	ss << optarg;
	int i;
	std::list<int> toRemove;
	while(!ss.eof()) {
		if(!(ss >> i))
			return error("Invalid input");
		toRemove.push_back(i-1);
	}

	for (std::list<int>::iterator it = toRemove.begin(); it != toRemove.end(); ++it) {
		removeEvent(*it);
		for (std::list<int>::iterator jt = it; jt != toRemove.end(); ++jt) {
			if(*jt > *it)(*jt)--;
			else if(*jt == *it && jt != it){
				std::list<int>::iterator tmp = jt;
				tmp--;
				toRemove.erase(jt);
				jt = tmp;
			}
		}
		
	}
	save("MyBudget.bas");
	return 0;
}
int BudgetString::editEventStr(std::string optarg) {
	int spl = optarg.find(" ");
	if(spl == std::string::npos) return error("Invalid input");
	int moneyInt;		// this is what we want to decide the values of
	Date date, period;
	int i;
	int c;	// how many things to be changed
	std::stringstream ss;
	ss << optarg.substr(0, spl);
	if(!(ss >> i)) return error("Invalid input");

	i--;
	Event *e = getEvent(i);

	moneyInt = e->money();
	date = e->time();
	if(e->id() == REvent::ID)
		period = ((REvent *)e)->endTime();
	else period.time(0);

	std::string modifies = optarg.substr(spl+1);
	spl = modifies.find(" ");
	if(spl == std::string::npos); //c = 1;
	while(modifies.size()) {
		std::string nthMod = modifies.substr(0, spl);
		int idsign = nthMod.find_first_of("-/ymd");
		if(nthMod[idsign] == '-' && nthMod.find("-", idsign+1) == std::string::npos)
			idsign = std::string::npos;
		switch(nthMod[idsign]) {
			case '-':	// date
			case '/':
				date = Date(nthMod, dateFormat(nthMod));
				c++;
				break;
			case 'y': // period
			case 'm':
			case 'd':
				period = Date(date);
				ss.str();
				ss.clear();
				int v;
				ss << nthMod;
				ss >> v;
				switch(nthMod[idsign]) {
					case 'y':
						while(v--)
							period.addYear();
						break;
					case 'm':
						while(v--)
							period.addMonth();
						break;
					case 'd':
						while(v--)
							period.addDay();
						break;
				}
				c++;
				modifies = "";
				break;
			case std::string::npos: // money
				ss.str();
				ss.clear();
				ss << nthMod;
				ss >> moneyInt;
				c++;
				break;
		}
		if(modifies.empty()) break;
		if(spl == std::string::npos) break;
		modifies = modifies.substr(spl+1);
		spl = modifies.find(" ");
	}
	GroupNameCollection groups = getGroupsOf(e);
	removeEvent(i);
	Event *newevent;
	if(period.time() == 0 || period.time() == date.time()) {
		addEvent(Event(date, moneyInt));
		newevent = &events.back();
	}
	else {
		addEvent(REvent(date, period, moneyInt));
		newevent = &revents.back();
	}
	for (GroupNameCollection::iterator it = groups.begin(); it != groups.end(); ++it)
		linkEventToGroup(*it, newevent);
	save("MyBudget.bas");
	return 0;
}

int BudgetString::linkEventGroupStr(std::string optarg) {
	int spl = optarg.find(" ");
	if(spl == std::string::npos) return error("Invalid input");
	std::string groupName = optarg.substr(0, spl);
	std::stringstream ss;
	ss << optarg.substr(spl+1);
	int i;
	while(ss >> i)
		linkEventToGroup(groupName, i-1);

	save("MyBudget.bas");
	return 0;
}
int BudgetString::unlinkEventGroupStr(std::string optarg) {
	int spl = optarg.find(" ");
	if(spl == std::string::npos) return error("Invalid input");
	std::string groupName = optarg.substr(0, spl);
	std::stringstream ss;
	ss << optarg.substr(spl+1);

	int i;
	// std::list<int> toRemove;
	while(ss >> i) 
		unlinkEventToGroup(groupName, i-1);
	// 	toRemove.push_back(i-1);
	// for (std::list<int>::iterator it = toRemove.begin(); it != toRemove.end(); ++it) 
	// budget.printGroup("Shopping_Birsta_City");
	// budget.printEventList();

	save("MyBudget.bas");
	return 0;
}

int BudgetString::editGroupStr(std::string optarg) {
	int spl = optarg.find(" ");
	if(spl == std::string::npos) return error("Invalid input");
	std::string groupName = optarg.substr(0, spl);
	std::string newgroupname = optarg.substr(spl+1);
	
	Budget::GroupEvent *grp = getGroup(groupName);
	if(grp == 0) {
		std::stringstream ss;
		ss << "No group was found with the name '" << groupName << "'\n please check your spelling and try again";
		error(ss.str());
		return 0;
	}
	std::replace(newgroupname.begin(), newgroupname.end(), ' ', '_');
	grp->name(newgroupname);
	save("MyBudget.bas");
	return 0;
}