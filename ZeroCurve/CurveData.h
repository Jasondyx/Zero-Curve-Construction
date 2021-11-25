#pragma once

#include "Curve.h"
#define maxLineSize	256

class __declspec(dllexport) CurveData {
public:
	char*	currency;
	long	baseDate;
	short	daysToSpot;
	char*	holidayCenter;
	CashInput* cash;
	FuturesInput* futures;
	SwapsInput* swaps;
public:
	bool load(const char* filename);
};