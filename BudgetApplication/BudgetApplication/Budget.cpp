#include "Budget.h"
#include <iostream>
#include <fstream>
#include <string.h>

#include <stdexcept>
#include <sstream>
#include <algorithm>

Budget::Budget() {
}
Budget::Budget(std::string file) {
	load(file);
}

int Budget::save(std::string file) {
	std::ofstream sfile(file.c_str());
	sfile << "#BUDGETAPP|Johannes Linden|"; // A sign of the file to know what file type it is
	sfile << "'          '";
	// Save events
	for (std::list<Event>::iterator it = events.begin(); it != events.end(); ++it){
		sfile << it->id();  // sign so we know what type the structure is
		sfile << *it;
	}
	// Save repeated events
	for (std::list<REvent>::iterator it = revents.begin(); it != revents.end(); ++it){
		sfile << it->id();
		sfile << *it;
	}
	sfile << "GROUPS";
	// Save groups
	unsigned int eventsSize = events.size();
	for (std::list<GroupEvent >::iterator grpIt = groups.begin(); grpIt != groups.end(); ++grpIt) {
		sfile << grpIt->name() << '\0';
		for (GroupEvent::iterator eventIt = grpIt->begin(); eventIt != grpIt->end(); ++eventIt) {
			int index = 0;
			for (std::list<Event>::iterator kt = events.begin(); kt != events.end(); ++kt, index++) {
				if(*eventIt == &(*kt)) {
					sfile << ' ' << index;
					index = -1;
					// break;
				}
			}
			if(index != -1) {
				// int index = 0;
				for (std::list<REvent>::iterator kt = revents.begin(); kt != revents.end(); ++kt, index++) {
					if(*eventIt == &(*kt)) {
						sfile << ' ' << index;
						// break;
					}
				}
			}
		}sfile << ":";
	}
	sfile << "'          '";
}
int Budget::load(std::string file) {
	std::ifstream sfile(file.c_str());
	load(&sfile);
}
int Budget::load(std::istream *sfile) {
	if(!sfile->good()) return 1;
	events.clear();
	revents.clear();
	groups.clear();
	char sign[27];
	sfile->read(sign, 27);

	if(strncmp(sign, "#BUDGETAPP|Johannes Linden|", 27) != 0) return 1;
	sfile->read(sign, 18);
	std::vector<Event *> vecEvents;
	while(!sfile->eof()) {
		char id = sfile->get();

		if(id == Event::ID) {
			Event e;
			(*sfile) >> e;
			events.push_back(e);
			vecEvents.push_back(&events.back());
		}
		else if(id == REvent::ID) {
			REvent e;
			(*sfile) >> e;
			revents.push_back(e);
			vecEvents.push_back(&revents.back());
		}
		else if(id == 'G') {
			sign[0] = 'G';
			sfile->read(sign+1, 5);
			if(strncmp(sign, "GROUPS", 6) != 0) return 2; // no not a group something is wrong

			while(!sfile->eof()) {
				std::string name;
				std::getline(*sfile, name, '\0');
				if(sfile->eof()) return 0;
				GroupEvent *grp = getGroup(name);
				if(grp == 0) {
					groups.push_back(GroupEvent(name));
					grp = &groups.back();
				}
				int index;
				unsigned int vevs = vecEvents.size();
				char c;
				while((c = sfile->get()) != ':') {
					if(sfile->eof()) return 3;
					sfile->putback(c);
					(*sfile) >> index;

					if(index >= vevs)
						return 4;
					grp->add(*vecEvents[index]);
				}
			}
		}
	}
}

bool Budget::eventExist(Event *e) {
	for (std::list<Event>::iterator it = events.begin(); it != events.end(); ++it)
		if(e == &(*it))
			return true;
	for (std::list<REvent>::iterator it = revents.begin(); it != revents.end(); ++it)
		if(e == &(*it))
			return true;
	return false;
 }
void Budget::addEvent(Event e) {
	events.push_back(e);	
}
void Budget::addEvent(REvent e) {
	revents.push_back(e);	
}
void Budget::addEvent(Event *e) {
	if(eventExist(e)) return;
	if(e->id() == Event::ID) 
		events.push_back(*e);
	else if(e->id() == REvent::ID) 
		revents.push_back(*(REvent *)e);
}
void Budget::linkEventToGroup(std::string str, Event *e) {
	if(e == 0) return;

	if(!eventExist(e)) {
		if(e->id() == Event::ID) {
			events.push_back(*e);
			e = &events.back();
		}
		else if(e->id() == REvent::ID) {
			revents.push_back(*(REvent*)e);
			e = &revents.back();
		}
	}
	GroupEvent *grp = getGroup(str);
	if(grp == 0) {
		groups.push_back(GroupEvent(str));
		grp = &groups.back();
	}
	grp->add(e);
}
void Budget::linkEventToGroup(std::string str, int index) {
	unsigned int es = events.size();
	if(index >= es) {
		index -= es;
		int i=0;
		for (std::list<REvent>::iterator it = revents.begin(); it != revents.end(); ++it, i++)
			if(index == i) 
				linkEventToGroup(str, &(*it));
		return;
	}
	int i=0;
	for (std::list<Event>::iterator it = events.begin(); it != events.end(); ++it, i++)
		if(index == i) 
			linkEventToGroup(str, &(*it));
}
void Budget::unlinkEventToGroup(std::string str, Event *e) {
	if(e == 0) return;

	if(!eventExist(e))
		return;
	GroupEvent *grp = getGroup(str);
	if(grp == 0) return;
	grp->remove(e);
	if(grp->size() == 0) {
		for (std::list<GroupEvent>::iterator it = groups.begin(); it != groups.end(); ++it)
			if(&(*it) == grp) 
				groups.erase(it);
	}
	
}
void Budget::unlinkEventToGroup(std::string str, int index) {
	unsigned int es = events.size();
	int i=0;
	GroupEvent::iterator jt;
	GroupEvent *grp = getGroup(str);
	if(index >= es) {
		index -= es;
		for (std::list<REvent>::iterator it = revents.begin(); it != revents.end(); ++it, i++)
			if(index == i) {
				for (jt = grp->begin(); jt != grp->end(); ++jt)
					if(*jt == &(*it)){
						it = revents.end();
						break;// grp->remove(jt);
					}
				// unlinkEventToGroup(str, &(*it));
			}
		if(jt != grp->end())
			grp->remove(jt);
		return;
	}
	
	for (std::list<Event>::iterator it = events.begin(); it != events.end(); ++it, i++)
		if(index == i){
			for (jt = grp->begin(); jt != grp->end(); ++jt)
				if(*jt == &(*it)){
					it = events.end();
					break;// grp->remove(jt);
				}
		}
	if(jt != grp->end())
		grp->remove(jt);
}

void Budget::removeEventFromGroups(Event *e) {
	if(e == 0) return;
	for (std::list<GroupEvent>::iterator groupIt = groups.begin(); groupIt != groups.end(); ++groupIt) {
		groupIt->remove(e);
		if(groupIt->size() == 0) {
			std::list<GroupEvent>::iterator tmp = groupIt;
			tmp--;
			groups.erase(groupIt);
			groupIt = tmp;
		}
	}
}
void Budget::removeEvent(Event *e) {
	if(e == 0) return;

	for (std::list<Event>::iterator it = events.begin(); it != events.end(); ++it)
		if(e == &(*it)){
			removeEventFromGroups(e);
			events.erase(it);
			return;
		}
	for (std::list<REvent>::iterator it = revents.begin(); it != revents.end(); ++it)
		if(e == &(*it)) {
			removeEventFromGroups(e);
			revents.erase(it);
			return;
		}
}
void Budget::removeEvent(int index) {
	if(index < 0 || index > events.size()+revents.size()) return;
	int i=0;

	for (std::list<Event>::iterator seventIt = events.begin(); seventIt != events.end(); ++seventIt, i++)
		if(i == index){
			removeEventFromGroups(&(*seventIt));
			events.erase(seventIt);
			return;
		}

	for (std::list<REvent>::iterator seventIt = revents.begin(); seventIt != revents.end(); ++seventIt, i++)
		if(i == index) {
			removeEventFromGroups(&(*seventIt));
			revents.erase(seventIt);
			return;
		}
}
Event *Budget::getEvent(int index) {
	if(index < 0)
		index = events.size() + revents.size() - index;
	if(index < 0) return 0;
	unsigned int es = events.size();
	int i = 0;
	if(index< es) {
		for (std::list<Event>::iterator it = events.begin(); it != events.end(); ++it, i++){
			if(index == i)
				return &(*it);
		}
		throw std::domain_error("invalid index of event to remove");
	}
	index -= es;
	for (std::list<REvent>::iterator it = revents.begin(); it != revents.end(); ++it, i++) {
		if(index == i) 
			return &(*it);
	}
	throw std::domain_error("invalid index of event to remove");
}

Budget::GroupNameCollection Budget::getGroupsOf(const Event *e) {
	GroupNameCollection res;
	for (std::list<GroupEvent >::iterator it = groups.begin(); it != groups.end(); it++) {
		for (GroupEvent::iterator i = it->begin(); i != it->end(); ++i) {
			if(e == *i)
				res.push_back(it->name());
		}
	}
	return res;
}
Budget::GroupEvent *Budget::getGroup(std::string name) {
	std::replace(name.begin(), name.end(), ' ', '_');
	for (std::list<GroupEvent >::iterator it = groups.begin(); it != groups.end(); it++) {
		std::string gname = it->name();
		std::replace(gname.begin(), gname.end(), ' ', '_');
		if(name == gname) return &(*it);
	}
	return 0;
}


std::string getPeriod(Date startTime, Date endTime) {
	std::string period;

	tm infoEnd   = *localtime( &endTime.time() );
	tm infoStart = *localtime( &startTime.time() );
	infoEnd.tm_year -= infoStart.tm_year;
	infoEnd.tm_mon -= infoStart.tm_mon;
	infoEnd.tm_mday -= infoStart.tm_mday;
	if(infoEnd.tm_mon < 0) {
		infoEnd.tm_mon = 12 + infoEnd.tm_mon;
		infoEnd.tm_year = 0;
	}
	if(infoEnd.tm_mday < 0) {
		if((infoStart.tm_mon+1)%2)
			infoEnd.tm_mday = 30 + infoEnd.tm_mday;
		else
			infoEnd.tm_mday = 31 + infoEnd.tm_mday;
		infoEnd.tm_mon = 0;
	}
	std::stringstream ss;
	ss << infoEnd.tm_year << ' ' << infoEnd.tm_mon << ' ' << infoEnd.tm_mday;
	std::string y, m, d;
	getline(ss, y, ' ');
	getline(ss, m, ' ');
	getline(ss, d, ' ');
	if(d != "0")
		period += d + "d ";
	if(m != "0")
		period += m + "m ";
	if(y != "0")
		period += y + "y ";
	return period;
}
void Budget::printPeriod(Period period) {

	std::cout << "\n _Period: " << period.start().date("dd/mm-yyyy") << " " << period.end().date("dd/mm-yyyy");
	std::cout << "_______________________________________";

	std::cout << "\n| Ref: | Date:      | Next event:   | Period:      | Money:           |";
	std::cout << "\n|------|------------|---------------|--------------|------------------|\n";
	int index = 1;
	for (std::list<Event>::iterator jt = events.begin(); jt != events.end(); ++jt, index++) {
		time_t time = jt->time();
		if(!(time >= period.start().time() && time <= period.end().time())) continue;
		MONEY money = jt->money();
		std::string repeat = "Never     ";
	 	std::cout << "| " << index << (index<10?"   ":(index<100?"  ":(index<1000?" ":"\0"))) << " | " 
	 			  << jt->time().date("dd/mm-yyyy") << " | " 
	 			  << repeat << "    | " 
	 			  << "             | "
	 			  << money << " kr";
		const int lenMoneyField = 18;
		int lenMoney = 0;
		while(money >= 10){
			money /= 10;
			lenMoney++;
		}
		for (int i = 0; i < lenMoneyField-lenMoney-6; i++)
			std::cout << " ";
		std::cout << " |\n";
	}
	for (std::list<REvent>::iterator jt = revents.begin(); jt != revents.end(); ++jt, index++) {
		time_t time = jt->time();
		if(!(time >= period.start().time() && time <= period.end().time())) continue;
		MONEY money = jt->money();
		std::string repeat = "    Never    ";
		std::string period;
		if(jt->id() == REvent::ID) {
			Date end(((const REvent *)&(*jt))->endTime());
			repeat = end.date("dd/mm-yyyy");

			period = getPeriod(jt->time().time(), end.time());
		}
	 	std::cout << "| " << index << (index<10?"   ":(index<100?"  ":(index<1000?" ":"\0"))) << " | " 
	 			  << jt->time().date("dd/mm-yyyy") << " | " 
	 			  << repeat << "    | " 
	 			  << period;
	 	for (int i = 0; i < 12 - period.size(); i++)
	 		std::cout << " ";
	 	std::cout << " | "
	 			  << money << " kr";
		const int lenMoneyField = 18;
		int lenMoney = 0;
		while(money >= 10){
			money /= 10;
			lenMoney++;
		}
		for (int i = 0; i < lenMoneyField-lenMoney-6; i++)
			std::cout << " ";
		std::cout << " |\n";
	}
	std::cout << " ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾ \n";
}
void Budget::printGroups() {
	std::cout << "Group names:\n";
	for (std::list<GroupEvent>::iterator it = groups.begin(); it != groups.end(); ++it) {
		std::string name = it->name();
		std::replace(name.begin(), name.end(), '_', ' ');
		std::cout << "  - " << name << "\n";
	}
}
int moneyLength(int money) {

	int lenMoney = 0;
	if(money < 0) {
		lenMoney++;
		money *= -1;
	}
	while(money >= 10){
		money /= 10;
		lenMoney++;
	}
	return lenMoney;
}
void Budget::printGroup(std::string name) {
	GroupEvent *grp = getGroup(name);
	if(grp == 0) return;


	std::replace(name.begin(), name.end(), '_', ' ');
	std::cout << "\n _" << name;
	int s = name.size();
	for (int i = 0; i < 68-s; i++)
		std::cout << "_";

	std::cout << "\n| Ref: | Date:      | Next event:   | Period:      | Money:           |";
	std::cout << "\n|------|------------|---------------|--------------|------------------|\n";
	for (GroupEvent::iterator jt = grp->begin(); jt != grp->end(); ++jt) {
		MONEY money = (*jt)->money();
		std::string repeat = "   Never  ";
		int index = 0;
		std::string period = "            ";
		if((*jt)->id() == REvent::ID) {
			Date endTime = ((const REvent *)(*jt))->endTime();
			repeat = endTime.date("dd/mm-yyyy");
			index = events.size()+1;
			for (std::list<REvent>::iterator it = revents.begin(); it != revents.end(); ++it, index++)
				if(&(*it) == *jt) break;

			period = getPeriod((*jt)->time(), endTime);
		}
		else {
			index = 1;
			for (std::list<Event>::iterator it = events.begin(); it != events.end(); ++it, index++)
				if(&(*it) == *jt) break;
		}
	 	std::cout << "| " << index << (index<10?"   ":(index<100?"  ":(index<1000?" ":"\0"))) << " | " 
	 			  << (*jt)->time().date("dd/mm-yyyy") << " | " << repeat << "    | " 
	 			  << period;
	 	for (int i = 0; i < 12 - period.size(); i++)
	 		std::cout << " ";
	 	std::cout << " | "<< money << " kr";
		const int lenMoneyField = 18;
		int lenMoney = moneyLength(money);
		for (int i = 0; i < lenMoneyField-lenMoney-6; i++)
			std::cout << " ";
		std::cout << " |\n";
	}
	std::cout << " ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾ \n";
}
void Budget::printEventList() {
	if(events.size() == 0) return;
	std::cout << "\n _One time events______________________";
	std::cout << "\n| Ref: | Date:      | Money:           |";
	std::cout << "\n|------|------------|------------------|\n";
	int index = 1;
	for (std::list<Event>::iterator it = events.begin(); it != events.end(); ++it, index++) {
		MONEY money = it->money();
	 	std::cout << "| " << index << (index<10?"   ":(index<100?"  ":(index<1000?" ":"\0"))) << " | " 
				  << it->time().date("dd/mm-yyyy") << " | " 
				  << money << " kr";
		const int lenMoneyField = 18;
		int lenMoney = moneyLength(money);
		for (int i = 0; i < lenMoneyField-lenMoney-6; i++)
			std::cout << " ";
		std::cout << " |\n";
	}
	std::cout << " ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾ \n";
}
void Budget::printREventList() {
	if(revents.size() == 0) return;
	std::cout << "\n _All repeat events__________________________________________________";
	std::cout << "\n| Ref: | Start date:  | Next date: | Period       | Money:           |";
	std::cout << "\n|------|--------------|------------|--------------|------------------|\n";
	int index = events.size()+1;
	for (std::list<REvent>::iterator it = revents.begin(); it != revents.end(); ++it,index++) {
		MONEY money = it->money();
		std::string period = getPeriod(it->time(), it->endTime());
	 	std::cout << "| " << index << (index<10?"   ":(index<100?"  ":(index<1000?" ":"\0"))) << " | " 
				  << it->time().date("dd/mm-yyyy") << "   | ";

		std::cout << it->endTime().date("dd/mm-yyyy") << " | "
				  << period;
	 	for (int i = 0; i < 12 - period.size(); i++)
	 		std::cout << " ";
		std::cout << " | " << money << " kr";
		const int lenMoneyField = 18;
		int lenMoney = moneyLength(money);
		for (int i = 0; i < lenMoneyField-lenMoney-6; i++)
			std::cout << " ";
		std::cout << " |\n";
	}
	std::cout << " ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾ \n";
}