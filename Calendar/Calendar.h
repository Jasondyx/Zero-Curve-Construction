#pragma once

#include "date.h"
#include <set>
#include <string>
#include <fstream>
#include <ios>
#include <iostream>    //新加的
#include <sstream>     //新加的

using std::set;
using std::string;
using std::ifstream;
using std::ios;

class __declspec(dllexport) Calendar {
protected:
	set<date>	holidays;
public:
	Calendar() {};
	virtual bool roll(date&);	// roll forward until business day
	bool isBusDay(const date&);
	bool addBusDays(date&, int);
	bool addMonths(date&, int);	// take care of month-end; roll to business day
	bool addYears(date&, int);	// take care of month-end; roll to business day
	bool addHoliday(date&);
	bool removeHoliday(date&);
};

class __declspec(dllexport) MMCalendar : public Calendar {
private:
	string	market;
public:
	MMCalendar(string file, string mkt);	// load market holidays from "file"
	bool roll(date&);	// while not month-end, roll forward until business day
						//           otherwise, roll backward until business day
	date& nextIMMDay(date&);	// Next third Wednesday of March, June, September and December
								// If holiday, roll forward
};
