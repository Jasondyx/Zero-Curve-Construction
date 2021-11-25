#include "date.h"

#define MAX_NUMYEARS 80   /* Max no of years for which the table is valid */

/* Julian dates are keyed off Jan 1 1900: 32874 = JAN 1 1990 */

static int yearList[MAX_NUMYEARS] =
{
	1990,1991,1992,1993,1994,1995,1996,1997,1998,1999,
	2000,2001,2002,2003,2004,2005,2006,2007,2008,2009,
	2010,2011,2012,2013,2014,2015,2016,2017,2018,2019,
	2020,2021,2022,2023,2024,2025,2026,2027,2028,2029,
	2030,2031,2032,2033,2034,2035,2036,2037,2038,2039,
	2040,2041,2042,2043,2044,2045,2046,2047,2048,2049,
	2050,2051,2052,2053,2054,2055,2056,2057,2058,2059,
	2060,2061,2062,2063,2064,2065,2066,2067,2068,2069
};

static julianDate jDateList[MAX_NUMYEARS] =
{
	32874,33239,33604,33970,34335,34700,35065,35431,35796,36161,
	36526,36892,37257,37622,37987,38353,38718,39083,39448,39814,
	40179,40544,40909,41275,41640,42005,42370,42736,43101,43466,
	43831,44197,44562,44927,45292,45658,46023,46388,46753,47119,
	47484,47849,48214,48580,48945,49310,49675,50041,50406,50771,
	51136,51502,51867,52232,52597,52963,53328,53693,54058,54424,
	54789,55154,55519,55885,56250,56615,56980,57346,57711,58076,
	58441,58807,59172,59537,59902,60268,60633,60998,61363,61729
};

/* The number of days in each month */
extern int dayInMonth[2][13] =
{
	{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

/* Months of the year */
extern const char* monthInYear[13] =
{
	"BAD MONTH",	/* BAD MONTH = 0 */
	"January",	"February",		"March",
	"April",	"May",			"June",
	"July",		"August",		"September",
	"October",	"November",		"December"
};



// Constructor
date::date(julianDate jDt)
{
	if (jDt <= 0)
		jDate = BAD_JDATE;
	else
		jDate = jDt;
}

date::date(int year, int month, int day)
{
	jDate = YMD2julianDate(year, month, day);
}

date::date(const date& dt)
{
	jDate = dt.jDate;
}

// destructor
date::~date()
{
}

// Assignment
date&
date::operator=(const date& dt)
{
	if (this == &dt)
		return *this;
	jDate = dt.jDate;
	return *this;
}

date&
date::operator=(const julianDate& jDt)
{
	jDate = jDt;
	return *this;
}

void
date::setYMD(int year, int month, int day)
{
	jDate = YMD2julianDate(year, month, day);
}

int
date::month() const
{
	if (jDate == BAD_JDATE)
		return 0;
	int y, m, d;
	if (julianDate2YMD(jDate, &y, &m, &d) != SUCCESS)
		return 0;
	return m;
}

int
date::day() const
{
	if (jDate == BAD_JDATE)
		return 0;
	int m, d, y;
	if (julianDate2YMD(jDate, &y, &m, &d) != SUCCESS)
		return 0;
	return d;
}

int
date::year() const
{
	if (jDate == BAD_JDATE)
		return 0;
	int m, d, y;
	if (julianDate2YMD(jDate, &y, &m, &d) != SUCCESS)
		return 0;
	return y;
}

bool
date::isLeapYear(int year)
{
	if (year < BASE_YEAR) return false;
	return (((year) % 4 == 0) && ((year) % 100 != 0) || ((year) % 400 == 0));
}

bool
date::isLeapYear() const
{
	return isLeapYear(this->year());
}

void
date::operator+=(int days)
{
	if (jDate != BAD_JDATE)
		jDate += days;
}

void
date::operator-=(int days)
{
	if (jDate != BAD_JDATE)
		jDate -= days;
}

date&
date::operator++()
{
	if (jDate != BAD_JDATE)
		++jDate;
	return *this;
}

date
date::operator++(int notUsed)	// parameter to indicate postfix ++
{
	date curDate = jDate;
	if (jDate != BAD_JDATE)
		++jDate;
	return curDate;
}

date&
date::operator--()
{
	if (jDate != BAD_JDATE && jDate != (BAD_JDATE + 1))
		--jDate;
	return *this;
}

date
date::operator--(int notUsed)	// parameter to indicate postfix --
{
	date curDate = jDate;
	if (jDate != BAD_JDATE && jDate != (BAD_JDATE + 1))
		--jDate;
	return curDate;
}

bool
date::operator==(const date& dt)
{
	if (jDate != BAD_JDATE && dt.jDate != BAD_JDATE)
		return this->jDate == dt.jDate;
	return false;
}

bool
date::operator!=(const date& dt)
{
	if (jDate != BAD_JDATE && dt.jDate != BAD_JDATE)
		return this->jDate != dt.jDate;
	return false;
}

bool
date::operator<(const date& dt)
{
	if (jDate != BAD_JDATE && dt.jDate != BAD_JDATE)
		return this->jDate < dt.jDate;
	return false;
}

bool
date::operator>(const date& dt)
{
	if (jDate != BAD_JDATE && dt.jDate != BAD_JDATE)
		return this->jDate > dt.jDate;
	return false;
}

bool
date::operator<=(const date& dt)
{
	if (jDate != BAD_JDATE && dt.jDate != BAD_JDATE)
		return this->jDate <= dt.jDate;
	return false;
}

bool
date::operator>=(const date& dt)
{
	if (jDate != BAD_JDATE && dt.jDate != BAD_JDATE)
		return this->jDate >= dt.jDate;
	return false;
}

bool
date::isWeekDay(const julianDate& j)
{
	if (j <= 0) return 0;
	int dayOfWeek = (int)(j - 1) % 7;			// 1 Jan 1900 is Sunday; dayOfWeek = 0  //1900.1.1应该是周一；但因为这里jDate是从2开始算的，所以结果是对的
	if (dayOfWeek == 0 || dayOfWeek == 6)	// Sunday=0 or Saturday=6
		return false;
	else
		return true;
}

julianDate
date::YMD2julianDate(int year, int month, int day)
/*-------------------------------------------------------------------
** DESCRIPTION:	Converts a month, day and year combination into a
**				julian date.  January 1, 1900 = julian date of 1.
**
** RETURNS:		The julian date if the conversion was successful
**				else BAD_JDATE.
**-----------------------------------------------------------------*/
{
	long	j;			/* julian date */
	int		startYear;	/* index into year array */

	/* check if valid date */
	if (!validYMD(year, month, day))
	{
		return BAD_JDATE;
	}

	/* check if starting year not less than lower bound */
	startYear = year - yearList[0];
	if (startYear >= 0)
	{
		if (startYear < MAX_NUMYEARS)
		{
			/* year within array bounds */
			j = jDateList[startYear];
		}
		else	/* days beyond upper limit */
		{
			/* use last array element as a base and move up */
			j = jDateList[MAX_NUMYEARS - 1];
			startYear = yearList[MAX_NUMYEARS - 1];
			while (startYear < year)
			{
				j += daysInYear(startYear);
				startYear++;
			}
		}
	}
	else	/* days below lower limit */
	{
		/* use first array element as base and move down */
		j = jDateList[0];
		startYear = yearList[0];
		while (startYear > year)
		{
			--startYear;
			j -= daysInYear(startYear);
		}
	}

	/* add on number of days for months and days */
	int leap = (isLeapYear(year) ? 1 : 0);
	for (int i = 1; i < month; i++)
		j += dayInMonth[leap][i];

	return j + day - 1;
}

int
date::julianDate2YMD(
	julianDate	date,		/* (I) date to convert */
	int* year,		/* (O) year number of date */
	int* month,		/* (O) month number of date */
	int* day)		/* (O) day number of date */
/*-------------------------------------------------------------------
** DESCRIPTION:	Converts a julian date into the month, day and year values.
**
** RETURNS:		SUCCESS or FAILURE if the date could not be converted.
**-----------------------------------------------------------------*/
{
	int		approxYrIdx;	/* approximate index in year table */
	long	dateStart;

	/* Check valid julian date */
	if (date < 0)
	{
		return FAILURE;
	}

	/* approximate year index */
	approxYrIdx = (int)(date - jDateList[0]) / 366;	/* 366 used to under approx. */

	/* check to make sure year is greater than or equal to lower limit */
	if ((approxYrIdx >= 0) && (date >= jDateList[0]))
	{
		if (approxYrIdx < MAX_NUMYEARS - 1)
		{
			/* check if index < max */
			int j;
			for (j = approxYrIdx; j < MAX_NUMYEARS; j++)
				if (jDateList[j] > date)
					break;

			/* date in previous array entry */
			j--;

			*year = yearList[j];
			date -= jDateList[j] - 1;
		}
		else
		{
			/* date beyond max year */
			*year = yearList[MAX_NUMYEARS - 1];
			dateStart = (long)(jDateList[MAX_NUMYEARS - 1] - 1);

			while (dateStart + daysInYear(*year) < date)
			{
				dateStart += daysInYear(*year);
				(*year)++;
			}

			date -= dateStart;
		}
	}
	else
	{
		/* date below min */
		--date; // fix Excel extra day (29 Feb) in 1900
		*year = BASE_YEAR;
		while (date > daysInYear(*year))
		{
			date -= daysInYear(*year);
			(*year)++;
		}
	}

	/* and now get the months and days */
	int leap = (isLeapYear(*year) ? 1 : 0);
	int i;
	for (i = 1; date > dayInMonth[leap][i]; i++)
		date -= dayInMonth[leap][i];
	*month = i;
	*day = date;

	return SUCCESS;
}

bool
date::validYMD(int year, int month, int day)
{
	int daysInMonth = dayInMonth[(isLeapYear(year) ? 1 : 0)][month];
	if (month > 0 && month <= 12 &&
		day > 0 && day <= daysInMonth &&
		year >= BASE_YEAR)
		return true;
	else
		return false;
}

bool
date::EOM()
{
	if (jDate == BAD_JDATE) return false;
	int leap = (isLeapYear() ? 1 : 0);
	return (dayInMonth[leap][month()] == day());
}

int
date::daysInYear(int year)
{
	if (year < BASE_YEAR) return 0;
	return (isLeapYear(year) ? 366 : 365);
}

int
date::daysInYear() const
{
	if (jDate == BAD_JDATE) return 0;
	return (isLeapYear() ? 366 : 365);
}
int
date::dayOfWeek() const
{
	if (jDate == BAD_JDATE) return 0;
	return (int)(jDate - 1) % 7;
}

bool
date::isWeekDay() const
{
	return isWeekDay(jDate);
}

void
date::addMonths(int mos)
{
	if (jDate != BAD_JDATE) {
		int year, month, day;
		bool monthEnd;

		monthEnd = EOM();
		julianDate2YMD(jDate, &year, &month, &day);
		/* add on months months */
		month += mos;
		/* adjust years if necessary */
		while (month > 12)
		{
			month -= 12;
			year++;
		}
		while (month < 1)
		{
			month += 12;
			year--;
		}

		if (year >= BASE_YEAR) {
			if (monthEnd						// original date is month end
				||
				day > daysInMonth(year, month))	// new month is "shorter"
				day = daysInMonth(year, month);

			jDate = YMD2julianDate(year, month, day);
		}
	}
}

void
date::addYears(int yrs)
{
	if (jDate != BAD_JDATE) {
		int year, month, day;
		bool monthEnd;

		monthEnd = EOM();
		julianDate2YMD(jDate, &year, &month, &day);

		/* add on years */
		year += yrs;
		if (year >= BASE_YEAR) {
			if (monthEnd						// original date is month end
				||
				day > daysInMonth(year, month))	// new month is "shorter"
				day = daysInMonth(year, month);

			jDate = YMD2julianDate(year, month, day);
		}

	}
}

int
date::daysInMonth(int year, int month) {
	if (!validYMD(year, month, 1)) return 0;
	if (isLeapYear(year))
		return dayInMonth[1][month];
	else
		return dayInMonth[0][month];
}

int
date::operator-(date& dt) {
	if (jDate != BAD_JDATE && (julianDate)dt != BAD_JDATE)
		return jDate - (julianDate)dt;
	else
		return BAD_OPERATION;
}