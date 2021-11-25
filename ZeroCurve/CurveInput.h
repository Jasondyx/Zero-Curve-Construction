#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;
using std::pair;

typedef double RateType;
typedef string CurrencyType;
typedef double FuturesPriceType;
typedef string CashMaturityType;
typedef long FuturesMaturityType;
typedef string SwapsMaturityType;
typedef string SwapsFreqType;
typedef string CashBasis;
typedef string FuturesBasis;
typedef string SwapsBasis;
typedef pair<CashMaturityType, RateType> CashPoint;
typedef pair<FuturesMaturityType, FuturesPriceType> FuturesPoint;
typedef pair<SwapsMaturityType, RateType> SwapsPoint;
typedef vector<CashPoint> CashPoints;
typedef vector<FuturesPoint> FuturesPoints;
typedef vector<SwapsPoint> SwapsPoints;

class __declspec(dllexport) CashInput {
public:
	CashBasis m_cashBasis;
	CashPoints m_cashPoints;
};

class __declspec(dllexport) FuturesInput {
public:
	FuturesBasis m_futuresBasis;
	FuturesPoints m_futuresPoints;
};

class __declspec(dllexport) SwapsInput {
public:
	SwapsBasis m_swapsBasis;
	SwapsFreqType m_swapsFreq;
	SwapsPoints m_swapsPoints;
};
