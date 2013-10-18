#ifndef DATE_H
#define DATE_H 

#include <string>
#include <ctime>

class Date {
public:
	Date(time_t time);
	Date(std::string date="now", std::string format="yyyy-mm-dd");
	Date &time(time_t time);
	time_t &time();
	time_t time() const;
	Date &setDate(std::string date, std::string format="yyyy-mm-dd");
	Date &addDay();
	Date &addWeek();
	Date &addMonth();
	Date &addYear();
	Date &removeDay();
	Date &removeWeek();
	Date &removeMonth();
	Date &removeYear();
	/**
	 * Returns the time at the form of std::string time
	 * @param  time the format of time returned
	 * @return      the time described as a string
	 */
	std::string date(std::string format="yyyy-mm-dd") const; // 
	operator const char *() const;
	operator time_t() const;

	static time_t convertToTime(std::string date, std::string format="yyyy-mm-dd");
	static std::string convertToDate(time_t time);
	static std::string toStdFormat(std::string format);
	static std::string toDateFormat(std::string format);
private:
	time_t timeStp;		// time stamp
};

bool operator <(const Date &l, const Date &r);

#endif