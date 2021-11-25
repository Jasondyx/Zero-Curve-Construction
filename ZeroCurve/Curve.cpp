#include "Curve.h"
#include <cstring>


Curve::Curve(char* ccy, long baseDt, int spot, MMCalendar* pMMCal_,
	CashInput* cin, FuturesInput* fin, SwapsInput* sin)
	: m_currency(ccy), m_baseDate(baseDt), m_daysToSpot(spot), m_pMMCal(pMMCal_),
	m_pCashInput(cin), m_pFuturesInput(fin), m_pSwapsInput(sin) {

}

bool
Curve::insertKeyPoint(date dt, DiscountFactorType df) {
	return false;
}

KeyPoints::const_iterator
Curve::firstKeyPoint() {
		return m_keyPoints.end();
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

}

void
Curve::processFutures() {

}


void
Curve::processSwaps() {

}
