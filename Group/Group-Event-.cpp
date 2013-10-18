#include "Group-Event-.h"


Group<Event, ComparerEvent >::Group(std::string gname, ComparerEvent compare):gname(gname), compare(compare) {
}

void Group<Event, ComparerEvent >::name(std::string gname) {
	this->gname = gname;
}
std::string Group<Event, ComparerEvent >::name() const {
	return gname;
}
void Group<Event, ComparerEvent >::add(const Event &mem) {
	for (iterator it = gmems.begin(); it != gmems.end(); ++it)
		if(*it == &mem) return;
	gmems.push_back(&mem);
}
void Group<Event, ComparerEvent>::add(const Event *mem) {
	for (iterator it = gmems.begin(); it != gmems.end(); ++it)
		if(*it == mem) return;
	gmems.push_back(mem);
}
void Group<Event, ComparerEvent>::add(const Group<Event, ComparerEvent> &grp) {
	for (int i=0;i<grp.size();i++){
		const Event &mem = grp[i];
		bool exists = false;
		for (iterator it = gmems.begin(); it != gmems.end(); ++it)
			if(*it == &mem) exists = true;
		if(!exists)
			gmems.push_back(&mem);
	}
}
void Group<Event, ComparerEvent>::empty() {
	int i=0;
	for (iterator it = gmems.begin(); it != gmems.end(); ++it) {
		if(i == gmems.size()-1) {
			iterator tmp = it;
			tmp--;
			gmems.erase(it);
			it = tmp;
		}
		i++;
	}
}

void Group<Event, ComparerEvent >::remove(iterator it) {
	gmems.erase(it);
}
void Group<Event, ComparerEvent >::remove(const Event *mem) {
	for (iterator it = gmems.begin(); it != gmems.end(); ++it) {
		if(mem == *it) {
			iterator tmp = it;
			tmp--;
			gmems.erase(it);
			it = tmp;
		}
	}
}
const Event & Group<Event, ComparerEvent >::operator[](int i) const {
	return *gmems[i];
}
unsigned int Group<Event, ComparerEvent >::size() const {
	return gmems.size();
}

void Group<Event, ComparerEvent >::sortDate() {
	compare.date();
	std::sort(begin(), end(), compare);
}
void Group<Event, ComparerEvent >::sortMoney() {
	compare.money();
	std::sort(begin(), end(), compare);
}
Group<Event, ComparerEvent >::iterator Group<Event, ComparerEvent >::begin() {
	return gmems.begin();
}
Group<Event, ComparerEvent >::iterator Group<Event, ComparerEvent >::end() {
	return gmems.end();
}