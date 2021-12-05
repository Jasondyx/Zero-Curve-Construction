#include "Curve.h"
#include <cstring>
#include <assert.h>


Curve::Curve(char* ccy, long baseDt, int spot, MMCalendar* pMMCal_,
	CashInput* cin, FuturesInput* fin, SwapsInput* sin)
	: m_currency(ccy), m_baseDate(baseDt), m_daysToSpot(spot), m_pMMCal(pMMCal_),
	m_pCashInput(cin), m_pFuturesInput(fin), m_pSwapsInput(sin) {
	processCash();
	processFutures();
	processSwaps();
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


KeyPoints::const_iterator
Curve::lastKeyPoint(date dt) {
	KeyPoints::const_iterator iter = firstKeyPoint(), iter_old;
	while (iter != endKeyPoint() && (*iter).first <= dt)
	{
		iter_old = iter;
		iter++;
	}
	return iter_old;
}

double
Curve::interpolate(date dt) {
	KeyPoints::const_iterator lastKP = lastKeyPoint(dt), nextKP = next(lastKP);
	if ((*lastKP).first == dt)  // dt is a keypoint, return the DF directly
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
	// process ON rate
	assert((*iter).first == "ON");
	RateType ON_rate = (*iter).second;
	auto DateBase = date(m_baseDate / 10000, m_baseDate % 10000 / 100, m_baseDate % 100);
	auto DateON = date(DateBase);
	m_pMMCal->addBusDays(DateON, 1);
	auto DF_ON = 1.0 / (1.0 + ON_rate * (DateON - DateBase) / 360.0);
	insertKeyPoint(DateON, DF_ON);  // ON rate
	//process TN rate
	iter++;
	assert((*iter).first == "TN");
	RateType TN_rate = (*iter).second;
	auto DateTN = date(DateON);
	m_pMMCal->addBusDays(DateTN, 1);
	auto DF_Spot = DF_ON / (1 + TN_rate * (DateTN - DateON) / 360.0);
	insertKeyPoint(DateTN, DF_Spot);  // TN rate
	//process 3M rate
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
	// use the formula to solve the DF of first and second future date
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
		// iteratively use the future points to extend the keypoints list
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

double
Curve::solve_k(int m, double swapsRate)
{
	// solve the k value letting NPV=0
	double k1 = 0.9, k2 = 1, k3;
	auto temp1 = NPV(k1, m, swapsRate), temp2 = NPV(k2, m, swapsRate);
	assert(NPV(k1, m, swapsRate) < 0);
	assert(NPV(k2, m, swapsRate) > 0);
	while (k1 < k2)
	{
		k3 = (k1 + k2) / 2;
		if (NPV(k3, m, swapsRate) > 0)
			k2 = k3;
		else
			k1 = k3;
		if (NPV(k2, m, swapsRate) - NPV(k1, m, swapsRate) < 1e-8)
			break;
	}
	return k1;
}

double
Curve::NPV(double k, int m, double swapsRate)
{
	date dateBase = date(m_baseDate / 10000, m_baseDate % 10000 / 100, m_baseDate % 100);
	m_pMMCal->addBusDays(dateBase, m_daysToSpot);
	date date_t = date(dateBase), date_t_last = date(dateBase);
	date date_endKP = (date)(*(prev(endKeyPoint()))).first;
	DiscountFactorType DF_t;
	double NPV = 0;
	for (int i = 0; i <= m * 2; i++)
	{
		date_t = date(dateBase);
		if (i != 0) m_pMMCal->addMonths(date_t, 6 * i);
		// calculate DF_t
		if (date_t <= date_endKP)
		{
			DF_t = interpolate(date_t);
		}
		else
			DF_t = m_keyPoints[date_endKP] * pow(k, date_t - date_endKP);
		// calculate NPV_t
		if (i == 0)
			NPV -= 100 * DF_t;
		else
		{
			NPV += 100 * swapsRate * (date_t - date_t_last) / 365.0 * DF_t;
		}
		date_t_last = date_t;
	}
	NPV += 100 * DF_t;  // add the principle value at the end
	return NPV;
}


void
Curve::processSwaps() {
	date dateBase = date(m_baseDate / 10000, m_baseDate % 10000 / 100, m_baseDate % 100);
	m_pMMCal->addBusDays(dateBase, m_daysToSpot);
	date date_t, date_endKP;
	int matureYear;
	double k;
	DiscountFactorType DF_t;
	for (auto iter = m_pSwapsInput->m_swapsPoints.begin(); iter != m_pSwapsInput->m_swapsPoints.end(); iter++)
	{
		matureYear = std::stoi((*iter).first.substr(0, (*iter).first.find('Y')));  // get the mature year
		assert(m_pSwapsInput->m_swapsFreq == "Semi-Annually");
		
		k = solve_k(matureYear, (*iter).second);  // solve the factor k using bisection method
		for (int i = 1; i <= matureYear * 2; i++)
		{
			date_t = date(dateBase);
			m_pMMCal->addMonths(date_t, 6*i);
			date_endKP = (date) (*(prev(endKeyPoint()))).first;
			if (date_t <= date_endKP) continue;  // skip the date where previous keypoints have covered
			DF_t = m_keyPoints[date_endKP] * pow(k, date_t - date_endKP);
			insertKeyPoint(date_t, DF_t);  // assign value to keypoints
		}
	}
}
