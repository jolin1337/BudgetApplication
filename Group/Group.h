#ifndef GROUP_H
#define GROUP_H 

#include "Comparer.h"
#include <string>
#include <vector>
#include <algorithm>

template<typename GMember, typename C=Comparer<GMember> > 
class Group {
public:
	Group(std::string gname="New Group", C compare=C());
	typedef typename std::vector<const GMember *>::iterator iterator;

	void name(std::string gname);
	std::string name() const;
	void add(const GMember &mem);
	void add(const GMember *mem);
	void add(const Group<GMember, C> &grp);
	void remove(const GMember &mem);
	const GMember & operator[](int i) const;

	unsigned int size() const;
	void sort();
	iterator begin() {
		return gmems.begin();
	}
	iterator end() {
		return gmems.end();
	}

	operator std::string() {
		return gname;
	}
	operator const char *() {
		return gname.c_str();
	}
private:
	// Group();

	std::string gname;						// name of this group
	std::vector<const GMember *> gmems;		// events connected to this group
	C compare;
};



/** Implemnentations below **/

template<typename GMember, typename C> 
Group<GMember, C>::Group(std::string gname, C compare):gname(gname), compare(compare) {
}

template<typename GMember, typename C> 
void Group<GMember, C>::name(std::string gname) {
	this->gname = gname;
}
template<typename GMember, typename C> 
std::string Group<GMember, C>::name() const{
	return gname;
}
template<typename GMember, typename C> 
void Group<GMember, C>::add(const GMember &mem) {
	for (iterator it = gmems.begin(); it != gmems.end(); ++it)
		if(*it == &mem) return;
	gmems.push_back(&mem);
}
template<typename GMember, typename C> 
void Group<GMember, C>::add(const GMember *mem) {
	for (iterator it = gmems.begin(); it != gmems.end(); ++it)
		if(*it == mem) return;
	gmems.push_back(mem);
}
template<typename GMember, typename C> 
void Group<GMember, C>::add(const Group<GMember, C> &grp) {
	for (iterator jt = grp.begin(); jt != grp.end(); ++jt){
		const GMember *mem = *jt;
		bool exists = false;
		for (iterator it = gmems.begin(); it != gmems.end(); ++it)
			if(*it == mem) exists = true;
		if(!exists)
			gmems.push_back(mem);
	}
}
template<typename GMember, typename C> 
void Group<GMember, C>::remove(const GMember &mem) {
	for (iterator it = gmems.begin(); it != gmems.end(); ++it) {
		if(&mem == *it)
			gmems.erase(it);
	}
}
template<typename GMember, typename C> 
const GMember & Group<GMember, C>::operator[](int i) const {
	return *gmems[i];
}

template<typename GMember, typename C> 
unsigned int Group<GMember, C>::size() const {
	return gmems.size();
}

template<typename GMember, typename C> 
void Group<GMember, C>::sort() {
	std::sort(begin(), end(), compare);
}

// template<typename GMember, typename C>
// Group<GMember, C>::iterator Group<GMember, C>::begin() {
// 	return gmems.begin();
// }
// template<typename GMember, typename C>
// Group<GMember, C>::iterator Group<GMember, C>::end() {
// 	return gmems.end();
// }
#endif