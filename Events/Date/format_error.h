#ifndef FORMATERROR
#define FORMATERROR 

#include <stdexcept>

class format_error : public std::runtime_error {
public:
	explicit format_error(const std::string &arg);
	const char * what() const throw();
};

#endif