#include "Calendar.h"
#include <cstring>
#include <cstdlib>
using namespace std;

#define maxLineSize	80

bool
Calendar::roll(date& dt)
{
	if (dt.year() > 0)
	{
		dt += 1;
		while (isBusDay(dt) == 0)
		{
			dt += 1;
		}
		return SUCCESS;
	}
	return FAILURE;    //Return FAILURE when dt.year()=0
}

bool
Calendar::isBusDay(const date& dt)    
{
	if (dt.isWeekDay() == 1)
	{
		if (holidays.count(dt) == 0)
		{
			return true;
		}
	}
	return false;
}

bool
Calendar::addBusDays(date& dt, int count)
{
	//Return the count-th business day after dt.
	//Can cross month-end
	if (dt.year() > 0 && count > 0)
	{
		for (int i = 1; i <= count; i++)
		{
			Calendar::roll(dt);    //The roll function here is from Calendar class, not MMCalendar class,
		}                          //so that dt will be rolled forward no mater it's month-end or not
		return SUCCESS;
	}
	return FAILURE;    //Return FAILURE when dt.year()=0 or count<=0
}

bool
Calendar::addMonths(date& dt, int count)    
{
	if (dt.year() > 0 && count > 0)
	{
		dt.addMonths(count);
		if (isBusDay(dt) == 0)
		{
			roll(dt);    //When using the object of MMCalendar to call this function, MMCalendar::roll will be called,
		}                //so that month-end can be taken care of
		return SUCCESS;
	}
	return FAILURE;      //Return FAILURE when dt.year()=0 or count<=0
}

bool
Calendar::addYears(date& dt, int count)
{
	if (dt.year() > 0 && count > 0)
	{
		dt.addYears(count);
		if (isBusDay(dt) == 0)
		{
			roll(dt);    //When using the object of MMCalendar to call this function, MMCalendar::roll will be called,
		}                //so that month-end can be taken care of
		return SUCCESS;
	}
	return FAILURE;      //Return FAILURE when dt.year()=0 or count<=0
}

bool
Calendar::addHoliday(date& dt)
{
	if (dt.year() > 0)
	{
		holidays.insert(dt);
		return SUCCESS;
	}
	return FAILURE;    //Return FAILURE when dt.year()=0
}

bool
Calendar::removeHoliday(date& dt)
{
	if (dt.year() > 0)
	{
		holidays.erase(dt);
		return SUCCESS;
	}
	return FAILURE;    //Return FAILURE when dt.year()=0
}

MMCalendar::MMCalendar(string filename, string mkt) :market(mkt) {
	ifstream fin(filename.c_str());
	if (!fin)    //Examine if the file has been successfully opened
	{
		cerr << "error: unable to open input file: " << filename << endl;
	}
	int da;
	string mar;
	char line[256];
	while (fin.getline(line, 256))    //Read the lines in the file one by one
	{
		stringstream ss(line);
		ss >> da;          //da is the date of holiday
		if (ss.get() == ',')
		{
			ss >> mar;     //mar is the corresponding market of this holiday
		}
		if (mar == mkt)    //Check if the market is correct. If yes, add the new holiday into holidays
		{
			date dt(da / 10000, (da / 100) % 100, da % 100);
			addHoliday(dt);
		}
	}
}

bool
MMCalendar::roll(date& dt)
{
	if (dt.year() > 0)
	{
		//Check if dt is month-end. If yes, roll backward until business day 
		if (dt.day() == date::daysInMonth(dt.year(), dt.month()))
		{
			dt -= 1;
			while (isBusDay(dt) == 0)
			{
				dt -= 1;
			}
		}
		//If dt is not month-end, roll forward until business day
		//If there's no business day from dt to the end of this month, roll backward until business day
		else
		{
			dt += 1;
			while (isBusDay(dt) == 0 && dt.day() != date::daysInMonth(dt.year(), dt.month()))
			{
				dt += 1;
			}
			while (isBusDay(dt) == 0)
			{
				dt -= 1;
			}
		}
		return SUCCESS;
	}
	return FAILURE;    //Return FAILURE when dt.year()=0
}

date&
MMCalendar::nextIMMDay(date& dt) {
	int year = dt.year();
	int month = dt.month();
	int day = dt.day();
	//Deal with each case one by one
	if (month == 1 || month == 2)
	{
		//Fist, set dt to be the first day of March
		//Then, determine if the first day of March is before or after Wednesday(i.e., the relation between dayOfWeek and 3)
		//If it's before Wednesday, then the third Wednesday of March should be 3-dt.dayOfWeek()+14 days later
		//If it's after Wednesday, then the third Wednesday of March should be 10-dt.dayOfWeek()+14 days later
		dt.setYMD(year, 3, 1);
		if (dt.dayOfWeek() <= 3)
		{
			dt += (3 - dt.dayOfWeek()+14);
		}
		else if (dt.dayOfWeek() > 3)
		{
			dt += (10 - dt.dayOfWeek() + 14);
		}
	}
	else if (month == 3)
	{
		//Fist, determine if dt is before or after Wednesday(i.e., the relation between dayOfWeek and 3)
		//Then, determine which Wednesday is the Wednesday before dt(first, second, third or later).
		    //For dt that is before or after Wednesday, the ways of calculating the Wednesday before dt are different
	    //If the Wednesday before dt is the first or second Wednesday of March, calculate the third Wednesday of March
		//If the Wednesday before dt is the third Wednesday of March, calculate the third Wednesday of June
		if (dt.dayOfWeek() <= 3)
		{
			if ((day + 3 - dt.dayOfWeek() - 7) < 8)
			{
				dt += 3 - dt.dayOfWeek() + 7;
			}
			else if ((day + 3 - dt.dayOfWeek() - 7) < 15)
			{
				dt += 3 - dt.dayOfWeek();
			}
			else
			{
				dt.setYMD(year, 6, 1);
				if (dt.dayOfWeek() <= 3)
				{
					dt += (3 - dt.dayOfWeek() + 14);
				}
				else if (dt.dayOfWeek() > 3)
				{
					dt += (10 - dt.dayOfWeek() + 14);
				}
			}
		}
		else
		{
			if ((day + 3 - dt.dayOfWeek()) < 8)
			{
				dt += 3 - dt.dayOfWeek() + 14;
			}
			else if ((day + 3 - dt.dayOfWeek()) < 15)
			{
				dt += 3 - dt.dayOfWeek() + 7;
			}
			else
			{
				dt.setYMD(year, 6, 1);
				if (dt.dayOfWeek() <= 3)
				{
					dt += (3 - dt.dayOfWeek() + 14);
				}
				else if (dt.dayOfWeek() > 3)
				{
					dt += (10 - dt.dayOfWeek() + 14);
				}
			}
		}
	}
	else if (month == 4 || month == 5)
	{
		//The logic is simmilar to the case that month=1 or month=2
		dt.setYMD(year, 6, 1);
		if (dt.dayOfWeek() <= 3)
		{
			dt += (3 - dt.dayOfWeek() + 14);
		}
		else if (dt.dayOfWeek() > 3)
		{
			dt += (10 - dt.dayOfWeek() + 14);
		}
	}
	else if (month == 6)
	{
		//The logic is simmilar to the case that month=3
		if (dt.dayOfWeek() <= 3)
		{
			if ((day + 3 - dt.dayOfWeek() - 7) < 8)
			{
				dt += 3 - dt.dayOfWeek() + 7;
			}
			else if ((day + 3 - dt.dayOfWeek() - 7) < 15)
			{
				dt += 3 - dt.dayOfWeek();
			}
			else
			{
				dt.setYMD(year, 9, 1);
				if (dt.dayOfWeek() <= 3)
				{
					dt += (3 - dt.dayOfWeek() + 14);
				}
				else if (dt.dayOfWeek() > 3)
				{
					dt += (10 - dt.dayOfWeek() + 14);
				}
			}
		}
		else
		{
			if ((day + 3 - dt.dayOfWeek()) < 8)
			{
				dt += 3 - dt.dayOfWeek() + 14;
			}
			else if ((day + 3 - dt.dayOfWeek()) < 15)
			{
				dt += 3 - dt.dayOfWeek() + 7;
			}
			else
			{
				dt.setYMD(year, 9, 1);
				if (dt.dayOfWeek() <= 3)
				{
					dt += (3 - dt.dayOfWeek() + 14);
				}
				else if (dt.dayOfWeek() > 3)
				{
					dt += (10 - dt.dayOfWeek() + 14);
				}
			}
		}
	}
	else if (month == 7 || month == 8)
	{
	    //The logic is simmilar to the case that month=1 or month=2
		dt.setYMD(year, 9, 1);
		if (dt.dayOfWeek() <= 3)
		{
			dt += (3 - dt.dayOfWeek() + 14);
		}
		else if (dt.dayOfWeek() > 3)
		{
			dt += (10 - dt.dayOfWeek() + 14);
		}
    }
	else if (month == 9)
	{
	    //The logic is simmilar to the case that month=3
	    if (dt.dayOfWeek() <= 3)
		{
			if ((day + 3 - dt.dayOfWeek() - 7) < 8)
			{
				dt += 3 - dt.dayOfWeek() + 7;
			}
			else if ((day + 3 - dt.dayOfWeek() - 7) < 15)
			{
				dt += 3 - dt.dayOfWeek();
			}
			else
			{
				dt.setYMD(year, 12, 1);
				if (dt.dayOfWeek() <= 3)
				{
					dt += (3 - dt.dayOfWeek() + 14);
				}
				else if (dt.dayOfWeek() > 3)
				{
					dt += (10 - dt.dayOfWeek() + 14);
				}
			}
		}
		else
		{
			if ((day + 3 - dt.dayOfWeek()) < 8)
			{
				dt += 3 - dt.dayOfWeek() + 14;
			}
			else if ((day + 3 - dt.dayOfWeek()) < 15)
			{
				dt += 3 - dt.dayOfWeek() + 7;
			}
			else
			{
				dt.setYMD(year, 12, 1);
				if (dt.dayOfWeek() <= 3)
				{
					dt += (3 - dt.dayOfWeek() + 14);
				}
				else if (dt.dayOfWeek() > 3)
				{
					dt += (10 - dt.dayOfWeek() + 14);
				}
			}
		}
    }
	else if (month == 10 || month == 11)
	{
	    //The logic is simmilar to the case that month=1 or month=2
		dt.setYMD(year, 12, 1);
		if (dt.dayOfWeek() <= 3)
		{
			dt += (3 - dt.dayOfWeek() + 14);
		}
		else if (dt.dayOfWeek() > 3)
		{
			dt += (10 - dt.dayOfWeek() + 14);
		}
    }
	else if (month == 12)
	{
	    //The logic is simmilar to the case that month=3
	    if (dt.dayOfWeek() <= 3)
		{
			if ((day + 3 - dt.dayOfWeek() - 7) < 8)
			{
				dt += 3 - dt.dayOfWeek() + 7;
			}
			else if ((day + 3 - dt.dayOfWeek() - 7) < 15)
			{
				dt += 3 - dt.dayOfWeek();
			}
			else
			{
				dt.setYMD(year+1, 3, 1);
				if (dt.dayOfWeek() <= 3)
				{
					dt += (3 - dt.dayOfWeek() + 14);
				}
				else if (dt.dayOfWeek() > 3)
				{
					dt += (10 - dt.dayOfWeek() + 14);
				}
			}
		}
		else
		{
			if ((day + 3 - dt.dayOfWeek()) < 8)
			{
				dt += 3 - dt.dayOfWeek() + 14;
			}
			else if ((day + 3 - dt.dayOfWeek()) < 15)
			{
				dt += 3 - dt.dayOfWeek() + 7;
			}
			else
			{
				dt.setYMD(year+1, 3, 1);
				if (dt.dayOfWeek() <= 3)
				{
					dt += (3 - dt.dayOfWeek() + 14);
				}
				else if (dt.dayOfWeek() > 3)
				{
					dt += (10 - dt.dayOfWeek() + 14);
				}
			}
		}
    }
	if (isBusDay(dt) == 0)
	{
		roll(dt);
	}
	return dt;
}