#ifndef BUDGET_H
#define BUDGET_H 

#include <list>
#include <vector>

#include "Events/Event.h"
#include "Events/REvent.h"
#include "Group/Group-Event-.h"


class Budget {
public:
	typedef Group<Event, ComparerEvent> GroupEvent;
	typedef std::vector<std::string> GroupNameCollection;
	Budget();
	Budget(std::string file);
	GroupEvent *getGroup(std::string name);
	GroupNameCollection getGroupsOf(const Event *e);

 	bool eventExist(Event *e);
	void addEvent(Event *e);
	void addEvent(Event e);
	void addEvent(REvent e);
	void linkEventToGroup(std::string str, Event *e);
	void linkEventToGroup(std::string str, int index);
	void unlinkEventToGroup(std::string str, Event *e);
	void unlinkEventToGroup(std::string str, int index);

	void removeEventFromGroups(Event *e);
	void removeEvent(int index);
	void removeEvent(Event *e);
	
	Event *getEvent(int index);

	// Print events
	void printGroup(std::string name);
	void printGroups();
	void printEventList();
	void printREventList();
	void printPeriod(Period period);

	int save(std::string file);
	int load(std::string file);
	int load(std::istream *sfile);
protected:
	std::list<GroupEvent> groups;
	std::list<Event> events;
	std::list<REvent> revents;
};

#endif