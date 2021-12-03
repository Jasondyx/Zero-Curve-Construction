#pragma once

#include "CurveInput.h"
#include "../Calendar/Calendar.h"
#include "../Calendar/date.h"

#include <map>
#include <utility>
#include <math.h>
#include <stdlib.h>

using std::map;
using std::pair;
using std::make_pair;

class Curve;
class SwapsCashFlow;
typedef long CurveIdType;
typedef double DiscountFactorType;
typedef map<CurveIdType, Curve*> CurvesMap;
typedef pair<julianDate, DiscountFactorType> KeyPoint;
typedef map<julianDate, DiscountFactorType> KeyPoints;
typedef vector<SwapsCashFlow> SwapsCashFlows;

class __declspec(dllexport) Curve {
private:
	int cashYB, futuresYB, swapsYB, nPerYear;
	date dt3M;

protected:
	KeyPoints m_keyPoints;
public:
	CurrencyType m_currency;
	date m_baseDate;
	int m_daysToSpot;
	CashInput* m_pCashInput;
	FuturesInput* m_pFuturesInput;
	SwapsInput* m_pSwapsInput;
	MMCalendar* m_pMMCal;

	Curve(char* ccy, long baseDt, int spot, MMCalendar* pMMCal,
		CashInput* cin, FuturesInput* fin, SwapsInput* sin);

	bool insertKeyPoint(date, DiscountFactorType);
	KeyPoints::const_iterator firstKeyPoint();
	KeyPoints::const_iterator endKeyPoint() { return m_keyPoints.end(); }
	KeyPoints::const_iterator lastKeyPoint(date dt);
	KeyPoint retrieveKeyPoint(KeyPoints::const_iterator);
	double interpolate(date);		// return 0 if unsuccessful

	void initProcess();
	void processCash();
	void processFutures();
	void processSwaps();

};

class __declspec(dllexport) SwapsCashFlow {
public:
	date	dt;		// fixed leg cash flow date
	double	CF;		// fixed leg cash flow
	double	DF;		// fixed leg cash flow X discount factor
};
