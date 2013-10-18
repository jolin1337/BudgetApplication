#include "format_error.h"

format_error::format_error(const std::string &arg):runtime_error(arg) {
}
const char * format_error::what() const throw() {
	return runtime_error::what();
}