#include "CurveData.h"
#include <string>
#include <fstream>
#include <cstring>
using namespace std;

using std::string;
using std::ifstream;
using std::ofstream;

#define CurveDatalog	"C:\\temp\\CurveData.log"

const char* MONTH[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

//Constructor
CurveData::CurveData() {
	currency = new char[256];
	holidayCenter = new char[256];
	cash = new CashInput();
	futures = new FuturesInput();
	swaps = new SwapsInput();
}

bool
CurveData::load(const char* filename) {
	ifstream fin(filename);
	if (!fin)    //Examine if the file has been successfully opened.Return false if not
	{
		cerr << "error: unable to open input file: " << filename << endl;
		return false;
	}
	string line1, line2;

	//currency
	getline(fin, line1, ',');
	fin.getline(currency, 256);
    
	//baseDate
	int day = 0;
	int month = 0;
	int year = 0;
	getline(fin, line1, ',');
	fin >> day;
	if (fin.get() == '-')
	{
		getline(fin, line2, '-');
	}
	getline(fin, line1);
	year = 2000 + (line1[0] - 48) * 10 + line1[1] - 48;
	for (int i = 0; i < 12; i++)
	{
		if (line2 == MONTH[i])
		{
			month = i + 1;
			break;
		}
	}
	baseDate = year * 10000 + month * 100 + day;

	//daysToSpot
	getline(fin, line1, ',');
	getline(fin, line2);
	daysToSpot = atoi(line2.c_str());
	
	//cashBasis
	getline(fin, line1, ',');
	getline(fin, cash->m_cashBasis);

	//futuresBasis
	getline(fin, line1, ',');
	getline(fin, futures->m_futuresBasis);

	//swapsBasis
	getline(fin, line1, ',');
	getline(fin, swaps->m_swapsBasis);

	//swapsFreq
	getline(fin, line1, ',');
	getline(fin, swaps->m_swapsFreq);

	//holidayCenter
	getline(fin, line1, ',');
	fin.getline(holidayCenter, 256);

	//cashPoints, futuresPoints, swapsPoints
	CashMaturityType cmt;       //Used to record CashMaturityType 
	double rOrt;                //Used to record RateType of CashPoint or SwapsPoint, or FuturesPriceType of FuturesPoint
	FuturesMaturityType fmt;    //Used to record FuturesMaturityType
	SwapsMaturityType smt;      //Used to record SwapsMaturityType
	while (getline(fin, line1, ','))
	{
		if (line1 == "Cash")
		{
			getline(fin, cmt, ',');
			fin >> rOrt;
			getline(fin, line1);    //The purpose of this statement is to skip the "\n" in this line, since fin >> rOrt will stop before "\n" instead of skipping it
			CashPoint cp(cmt, rOrt);
			cash->m_cashPoints.push_back(cp);
			continue;
		}
		else if (line1 == "Futures")
		{
			fin >> day;
			if (fin.get() == '-')
			{
				getline(fin, line2, '-');
			}
			getline(fin, line1, ',');
			year = 2000 + (line1[0] - 48) * 10 + line1[1] - 48;
			for (int i = 0; i < 12; i++)
			{
				if (line2 == MONTH[i])
				{
					month = i + 1;
					break;
				}
			}
			fmt = year * 10000 + month * 100 + day;
			fin >> rOrt;
			getline(fin, line1);    //The purpose of this statement is to skip the "\n" in this line, since fin >> rOrt will stop before "\n" instead of skipping it
			FuturesPoint fp(fmt, rOrt);
			futures->m_futuresPoints.push_back(fp);
			continue;
		}
		else if (line1 == "Swaps")
		{
			getline(fin, smt, ',');
			fin >> rOrt;
			getline(fin, line1);    //The purpose of this statement is to skip the "\n" in this line, since fin >> rOrt will stop before "\n" instead of skipping it
			SwapsPoint sp(smt, rOrt);
			swaps->m_swapsPoints.push_back(sp);
		}
	}
	return true;
}