#ifndef GROUPEVENT_H
#define GROUPEVENT_H 

#include "Group.h"
#include "Events/Event.h"
#include "Events/ComparerEvent.h"

template<> 
class Group <Event, ComparerEvent > {
public:
	Group(std::string gname="New Group", ComparerEvent compare=ComparerEvent());
	typedef typename std::vector<const Event *>::iterator iterator;

	void name(std::string gname);
	std::string name() const;
	void add(const Event &mem);
	void add(const Event *mem);
	void add(const Group<Event, ComparerEvent> &grp);
	void empty();
	void remove(iterator it);
	void remove(const Event *mem);
	const Event & operator[](int i) const;
	unsigned int size() const;
	void sortMoney();
	void sortDate();
	iterator begin();
	iterator end();
private:
	// Group();

	std::string gname;						// name of this group
	std::vector<const Event *> gmems;		// events connected to this group
	ComparerEvent compare;
};

#endif