#pragma once

#define SUCCESS		1
#define FAILURE		0
#define	BAD_JDATE	0
#define BAD_OPERATION	-99999
#define BASE_YEAR	1900		// Assume date before 1/1/1900 bad date
#define WEDNESDAY	3

typedef	int	julianDate;

class __declspec(dllexport) date {

protected:
	julianDate jDate;

public:
	//Constructors
	date(julianDate jDt = BAD_JDATE);	// if jDt<=0 jDate=0
	date(int year, int month, int day);	// check year, month, day is a valid date
	date(const date&);	// copy constructor

	//Destructor
	~date();

	//Assignment
	date& operator=(const date& dt);
	date& operator=(const julianDate& jDt);
	void setYMD(int year, int month, int day);	// if year, month, day is NOT a valid date
												// set jDate=0

	//Conversion
	operator julianDate() const { return jDate; }

	//Useful Info
	int month() const;	// return 0 if invalid date
	int day() const;	// return 0 if invalid date
	int year() const;	// return 0 if invalid date
	bool isLeapYear() const;// return false if invalid date
	int daysInYear() const;	// return 0 if invalid date
	int dayOfWeek() const;	// return 0 if invalid date
	bool isWeekDay() const; // return false if invalid date

	//Operations

	bool EOM();	// End of Month? return false if invalid date
	virtual void addMonths(int months); // no action if invalid date
										// take care of month end
										// no action if go before 1/1/1900
	virtual void addYears(int years);	// no action if invalid date
										// take care of leap year month end
										// no action if go before 1/1/1900

	// +/- days, ignore checking invalid date after increment or decrement
	void operator+=(int days);
	void operator-=(int days);
	date& operator++();		// pre-increment
	date operator++(int);	// post-increment
	date& operator--();		// pre-decrement
	date operator--(int);	// post-decrement

	int operator-(date& dt);	// return BAD_OPERATION if either date is invalid

	// return false if either date is invalid
	bool operator==(const date& dt);
	bool operator!=(const date& dt);
	bool operator<(const date& dt);
	bool operator>(const date& dt);
	bool operator<=(const date& dt);
	bool operator>=(const date& dt);

	//Static
	static bool isLeapYear(int year);	// return false if year<1900
	static int daysInYear(int year);	// return 0 if year<1900
	static int daysInMonth(int year, int month); // return 0 if invalid year,month
	static bool validYMD(int year, int month, int day); // return false if year>=1900
	static bool isWeekDay(const julianDate& j);	// return false if j<=0
	static julianDate YMD2julianDate(int year, int month, int day); // return 0 if invalid date
	static int julianDate2YMD(julianDate j, int* year, int* month, int* day); // return FAILURE if jDate<0

};
