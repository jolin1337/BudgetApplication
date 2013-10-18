#ifndef COMPARER_H
#define COMPARER_H


template<typename T>
class Comparer {
public:
	bool operator() (const T *one, const T *two);
};

template<typename T>
bool Comparer<T>::operator() (const T *one, const T *two) {
	return (*one) < (*two);
}

#endif