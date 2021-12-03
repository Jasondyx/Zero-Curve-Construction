#include "Curve.h"
#include <cstring>
#include <assert.h>


Curve::Curve(char* ccy, long baseDt, int spot, MMCalendar* pMMCal_,
	CashInput* cin, FuturesInput* fin, SwapsInput* sin)
	: m_currency(ccy), m_baseDate(baseDt), m_daysToSpot(spot), m_pMMCal(pMMCal_),
	m_pCashInput(cin), m_pFuturesInput(fin), m_pSwapsInput(sin) {
	processCash();
	processFutures();
}

bool
Curve::insertKeyPoint(date dt, DiscountFactorType df) {
	
	 //m_keyPoints[dt] = df;
	auto res = m_keyPoints.insert(KeyPoint(dt, df));
	return res.second;
}

KeyPoints::const_iterator
Curve::firstKeyPoint() {
	return m_keyPoints.begin();
}

KeyPoint
Curve::retrieveKeyPoint(KeyPoints::const_iterator ki) {
	return *ki;
}

void
Curve::initProcess() {

}

KeyPoints::const_iterator
Curve::lastKeyPoint(date dt) {
	KeyPoints::const_iterator iter = firstKeyPoint(), iter_old;
	while ((*iter).first <= dt && iter != endKeyPoint())
	{
		iter_old = iter;
		iter++;
	}
	return iter_old;
}

double
Curve::interpolate(date dt) {
	KeyPoints::const_iterator lastKP = lastKeyPoint(dt), nextKP = next(lastKP);
	if ((*lastKP).first == dt)
		return (*lastKP).second;
	if (nextKP == endKeyPoint())
	{
		std::cerr << "Out of range of zero curve." << std::endl;
		return -1;
	}
	DiscountFactorType DF_curr = (*lastKP).second * pow((*nextKP).second / (*lastKP).second, double(julianDate(dt) - (*lastKP).first) / ((*nextKP).first - (*lastKP).first));
	return DF_curr;
}

void
Curve::processCash() {
	auto iter = m_pCashInput->m_cashPoints.begin();
	assert((*iter).first == "ON");
	RateType ON_rate = (*iter).second;
	auto DateBase = date(m_baseDate / 10000, m_baseDate % 10000 / 100, m_baseDate % 100);
	auto DateON = date(DateBase);
	m_pMMCal->addBusDays(DateON, 1);
	auto DF_ON = 1.0 / (1.0 + ON_rate * (DateON - DateBase) / 360.0);
	insertKeyPoint(DateON, DF_ON);  // ON rate

	iter++;
	assert((*iter).first == "TN");
	RateType TN_rate = (*iter).second;
	auto DateTN = date(DateON);
	m_pMMCal->addBusDays(DateTN, 1);
	auto DF_Spot = DF_ON / (1 + TN_rate * (DateTN - DateON) / 360.0);
	insertKeyPoint(DateTN, DF_Spot);  // TN rate

	iter++;
	assert((*iter).first == "3M");
	RateType M3_rate = (*iter).second;
	auto Date3M = date(DateTN);
	m_pMMCal->addMonths(Date3M, 3);
	auto DF_3M = DF_Spot / (1 + M3_rate * (Date3M - DateTN) / 360.0);
	insertKeyPoint(Date3M, DF_3M);  // 3M rate

}

void
Curve::processFutures() {
	DiscountFactorType DF_1Future, DF_2Future;
	auto DateBase = date(m_baseDate / 10000, m_baseDate % 10000 / 100, m_baseDate % 100);
	date Date3M = date(DateBase);
	m_pMMCal->addBusDays(Date3M, m_daysToSpot);
	m_pMMCal->addMonths(Date3M, 3);
	auto Price_1Future = m_pFuturesInput->m_futuresPoints[0].second, Price_2Future = m_pFuturesInput->m_futuresPoints[1].second;
	auto Date_1Future_l = m_pFuturesInput->m_futuresPoints[0].first, Date_2Future_l = m_pFuturesInput->m_futuresPoints[1].first;
	date Date_1Future = date(Date_1Future_l / 10000, Date_1Future_l % 10000 / 100, Date_1Future_l % 100);
	date Date_2Future = date(Date_2Future_l / 10000, Date_2Future_l % 10000 / 100, Date_2Future_l % 100);
	auto DF_ratio = 1 / (1 + ((100 - Price_1Future) / 100 * (Date_2Future - Date_1Future) / 360));

	DF_1Future = m_keyPoints[julianDate(Date3M)] / pow(DF_ratio, double(Date3M - Date_1Future) / (Date_2Future - Date_1Future));
	DF_2Future = DF_1Future * DF_ratio;
	insertKeyPoint(Date_1Future, DF_1Future);
	insertKeyPoint(Date_2Future, DF_2Future);

	for (auto iter = m_pFuturesInput->m_futuresPoints.begin(); iter != m_pFuturesInput->m_futuresPoints.end(); iter++)
	{
		auto currDate_l = (*iter).first;
		date currDate = date(currDate_l / 10000, currDate_l % 10000 / 100, currDate_l % 100);
		date nextDate = date(currDate);
		nextDate.addMonths(1);
		m_pMMCal->nextIMMDay(nextDate);

		auto currDF = m_keyPoints[currDate];
		auto nextDF = currDF / (1 + (100 - (*iter).second) / 100 * (nextDate - currDate) / 360);
		insertKeyPoint(nextDate, nextDF);

	}
}


void
Curve::processSwaps() {

}
