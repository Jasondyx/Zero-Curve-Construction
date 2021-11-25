#include "Calendar.h"
#include <cstring>
#include <cstdlib>
using namespace std;    //�¼ӵ�

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
	return FAILURE;    //这里什么情况return FAILURE啊??
}

bool
Calendar::isBusDay(const date& dt)    //�ж��Ƿ���business day
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
Calendar::addBusDays(date& dt, int count)    //��dt����ĵ�count��business day��ֵ��dt
{
	if (dt.year() > 0 && count > 0)
	{
		for (int i = 1; i <= count; i++)
		{
			Calendar::roll(dt);
		}
		return SUCCESS;
	}
	return FAILURE;    //�������Ҳ���Ҳµ�
}

bool
Calendar::addMonths(date& dt, int count)    
{
	if (dt.year() > 0 && count > 0)
	{
		dt.addMonths(count);
		if (isBusDay(dt) == 0)
		{
			roll(dt);
		}
		return SUCCESS;
	}
	return FAILURE;    //�������Ҳ���Ҳµ�
}

bool
Calendar::addYears(date& dt, int count)
{
	if (dt.year() > 0 && count > 0)
	{
		dt.addYears(count);
		if (isBusDay(dt) == 0)
		{
			roll(dt);
		}
		return SUCCESS;
	}
	return FAILURE;    //�������Ҳ���Ҳµ�
}

bool
Calendar::addHoliday(date& dt)
{
	if (dt.year() > 0)
	{
		holidays.insert(dt);
		return SUCCESS;
	}
	return FAILURE;    //�������Ҳ���Ҳµ�
}

bool
Calendar::removeHoliday(date& dt)
{
	if (dt.year() > 0)
	{
		holidays.erase(dt);
		return SUCCESS;
	}
	return FAILURE;    //�������Ҳ���Ҳµ�
}

MMCalendar::MMCalendar(string filename, string mkt) :market(mkt) {
	ifstream fin(filename.c_str());
	if (!fin)
	{
		cerr << "error: unable to open input file: " << filename << endl;    //���ڼ���Ƿ�ɹ����ļ���Ϊ����һ������include <iostream>����ʵҲ���Բ�Ҫ��һ��
	}
	int da;
	string mar;
	char line[256];
	while (fin.getline(line, 256))
	{
		stringstream ss(line);
		ss >> da;
		if (ss.get() == ',')
		{
			ss >> mar;
		}
		if (mar == mkt)
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
		if (dt.day() == date::daysInMonth(dt.year(), dt.month()))
		{
			dt -= 1;
			while (isBusDay(dt) == 0)
			{
				dt -= 1;
			}
		}
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
	return FAILURE;
}

date&
MMCalendar::nextIMMDay(date& dt) {
	int year = dt.year();
	int month = dt.month();
	int day = dt.day();
	if (month == 1 || month == 2)
	{
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