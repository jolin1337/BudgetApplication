#include "Date.h"
#include <algorithm>
#include <ctime>
#include "format_error.h"

Date::Date(time_t time):timeStp(time) {}
Date::Date(std::string date, std::string format):timeStp(std::time(NULL)) {
	if(date != "now") 
		setDate(date, format);
}
Date &Date::time(time_t timeStp) {
	this->timeStp = timeStp;
	return *this;
}
time_t Date::time() const{
	return timeStp;
}
time_t &Date::time(){
	return timeStp;
}
Date &Date::setDate(std::string date, std::string format) {
	timeStp = convertToTime(date, format);
	return *this;
}
Date &Date::addDay() {
	timeStp +=  24*60*60;
	return *this;
}
Date &Date::addWeek() {
	timeStp += 24*60*60 * 7;
	return *this;
}
Date &Date::addMonth() {
	tm infoStart = *localtime( &timeStp );
	infoStart.tm_mon++;
	timeStp = mktime(&infoStart);
	return *this;
}
Date &Date::addYear() {
	tm infoStart = *localtime( &timeStp );
	infoStart.tm_year++;
	timeStp = mktime(&infoStart);
	return *this;
}
Date &Date::removeDay() {
	timeStp -=  24*60*60;
	return *this;
}
Date &Date::removeWeek() {
	timeStp -= 24*60*60 * 7;
	return *this;
}
Date &Date::removeMonth() {
	tm infoStart = *localtime( &timeStp );
	infoStart.tm_mon--;
	timeStp = mktime(&infoStart);
	return *this;
}
Date &Date::removeYear() {
	tm infoStart = *localtime( &timeStp );
	infoStart.tm_year--;
	timeStp = mktime(&infoStart);
	timeStp -= 24*60*60 * 366;
	return *this;
}

std::string Date::date(std::string format) const{
	// std::string endformat = format;
	std::transform(format.begin(), format.end(), format.begin(), tolower);
	char buffer[80];

    std::tm *timeinfo = std::localtime(&timeStp);

    // example: "%Y-%m-%d, %H:%M:%S"
    if(!std::strftime(buffer, 80, toStdFormat(format).c_str(), timeinfo))
    	throw format_error("Unable to recognize the format of the date");
    std::string res = buffer;
    return res;
}

Date::operator const char *() const{
	return date().c_str();
}
Date::operator time_t() const{
	return time();
}

time_t Date::convertToTime(std::string date, std::string format) {
    std::tm timeinfo = {0};
	if(!strptime(date.c_str(), toStdFormat(format).c_str(), &timeinfo))
    	throw format_error("Unable to recognize the format of the date");
	return mktime(&timeinfo);
}
std::string Date::convertToDate(time_t time) {

}
std::string Date::toStdFormat(std::string format) {
	std::transform(format.begin(), format.end(), format.begin(), tolower);

	std::string endformat = "";
	for (int i = 0; i < format.size(); i++) {
		int j = i;
		if(format[i] == 'y'){
			for (; format[j] == format[i] && j < format.size(); j++);
			if(j-i == 2)	// 2 chars
				endformat += "%y";
			else
				endformat += "%Y";
		}
		else if(format[i] == 'm'){
			for (; format[j] == format[i] && j < format.size(); j++);
			// if(j-i == 1)	// 2 chars
			endformat += "%m";
		}
		else if(format[i] == 'd') {
			for (; format[j] == format[i] && j < format.size(); j++);
			if(j-i == 3)	// 3 chars
				endformat += "%j";
			else
				endformat += "%d";
		}
		else if(format[i] == 'w') {
			for (; format[j] == format[i] && j < format.size(); j++);
			endformat += "%W";
		}
		else
			endformat += format[i];
		if(j != i)
			j--;
		i += j-i;
	}
	return endformat;
}

std::string Date::toDateFormat(std::string format) {
	// std::transform(format.begin(), format.end(), format.begin(), tolower);

	std::string endformat = "";
	for (int i = 0; i < format.size(); i++) {
		int j = i;
		if(format[i] == '%') {
			i++;
			if(format[i] == 'y'){
				endformat += "yy";
			}
			else if(format[i] == 'Y')
				endformat += "yyyy";
			else if(format[i] == 'm')
				endformat += "m";
			else if(format[i] == 'j')
				endformat += "ddd";
			else if(format[i] == 'd')
				endformat += "dd";
			else if(format[i] == 'W') {
				endformat += "w";
			}
			else
				endformat += format[i];
			if(j != i)
				j--;
			i += j-i;
		}
		else
			endformat += format[i];
	}
	return endformat;
}



bool operator <(const Date &l, const Date &r) {
	return (time_t)l < (time_t)r;
}