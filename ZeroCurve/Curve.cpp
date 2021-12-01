#include "Curve.h"
#include <cstring>
#include <assert.h>


Curve::Curve(char* ccy, long baseDt, int spot, MMCalendar* pMMCal_,
	CashInput* cin, FuturesInput* fin, SwapsInput* sin)
	: m_currency(ccy), m_baseDate(baseDt), m_daysToSpot(spot), m_pMMCal(pMMCal_),
	m_pCashInput(cin), m_pFuturesInput(fin), m_pSwapsInput(sin) {

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

double
Curve::interpolate(date dt) {
	return 0.0;
}

void
Curve::processCash() {
	auto iter = m_pCashInput->m_cashPoints.begin();
	assert((*iter).first == "ON");
	RateType ON_rate = (*iter).second;
	auto DateON = date(m_baseDate);
	m_pMMCal->addBusDays(DateON, 1);
	auto DF_ON = 1 / (1 + ON_rate * (DateON - m_baseDate) / 360);
	insertKeyPoint(DateON, DF_ON);  // ON rate

	iter++;
	assert((*iter).first == "TN");
	RateType TN_rate = (*iter).second;
	auto DateTN = date(DateON);
	m_pMMCal->addBusDays(DateTN, 1);
	auto DF_Spot = DF_ON / (1 + TN_rate * (DateTN - DateON) / 360);
	insertKeyPoint(DateTN, DF_Spot);  // TN rate

	iter++;
	assert((*iter).first == "3M");
	RateType M3_rate = (*iter).second;
	auto Date3M = date(DateTN);
	m_pMMCal->addMonths(Date3M, 3);
	auto DF_3M = DF_Spot / (1 + M3_rate * (Date3M - DateTN) / 360);
	insertKeyPoint(Date3M, DF_3M);  // 3M rate

}

void
Curve::processFutures() {
	DiscountFactorType DF_1Future, DF_2Future;
	date Date3M = date(m_baseDate);
	m_pMMCal->addBusDays(Date3M, m_daysToSpot);
	m_pMMCal->addMonths(Date3M, 3);
	auto Price_1Future = m_pFuturesInput->m_futuresPoints[0].second, Price_2Future = m_pFuturesInput->m_futuresPoints[1].second;
	auto Date_1Future_l = m_pFuturesInput->m_futuresPoints[0].first, Date_2Future_l = m_pFuturesInput->m_futuresPoints[1].first;
	date Date_1Future = date(Date_1Future_l / 10000, Date_1Future_l % 10000 / 100, Date_1Future_l % 100);
	date Date_2Future = date(Date_2Future_l / 10000, Date_2Future_l % 10000 / 100, Date_2Future_l % 100);
	auto DF_ratio = 1 / (1 + ((100 - Price_1Future) / 100 * (Date_2Future - Date_1Future) / 360));
	DF_1Future = m_keyPoints[julianDate(Date3M)] / DF_ratio ^ ((Date3M - Date_1Future) / (Date_2Future - Date_1Future));
	DF_2Future = DF_1Future * DF_ratio;
	insertKeyPoint(Date_1Future, DF_1Future);
	insertKeyPoint(Date_2Future, DF_2Future);

	for (auto iter = m_pFuturesInput->m_futuresPoints.begin(); iter != m_pFuturesInput->m_futuresPoints.end(); iter++)
	{
		auto nextDate_l = (*(iter + 1)).first, currDate_l = (*iter).first;
		date nextDate = date(nextDate_l / 10000, nextDate_l % 10000 / 100, nextDate_l % 100);
		date currDate = date(currDate_l / 10000, currDate_l % 10000 / 100, currDate_l % 100);

		auto currDF = m_keyPoints[currDate];
		auto nextDF = currDF / (1 + (100 - (*iter).second) / 100 * (nextDate - currDate) / 360);
		insertKeyPoint(nextDate, nextDF);

	}
}


void
Curve::processSwaps() {

}
