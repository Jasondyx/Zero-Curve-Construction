#include "pch.h"
#include "CppUnitTest.h"
#include "..\Calendar\Calendar.h"
#include "..\Calendar\date.h"
#include "..\ZeroCurve\CurveData.h"
#include "..\ZeroCurve\CurveInput.h"
#include "..\ZeroCurve\Curve.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	MMCalendar* pJpy;
	CurveData cd;
	Curve* pCurve;

	TEST_CLASS(CalendarUnitTest)
	{
	public:

		TEST_CLASS_INITIALIZE(LoadHolidays)
		{
			// test class initialization  code
			pJpy = new MMCalendar("D:\\资料\\学习\\科大\\quantitative C++\\Project\\Project_Calendar\\JPY_Holiday.txt", "JPY");
		}
		TEST_METHOD(Holiday20131104)
		{
			date d20131104(2013, 11, 4);
			Assert::IsFalse(pJpy->isBusDay(d20131104));
		}

		TEST_METHOD(MonthEnd) 
		{
			date d20130131(2013, 1, 31);
			date d(d20130131);
			pJpy->addMonths(d, 1);
			Assert::IsTrue(d == date(2013, 2, 28));
		}

		TEST_METHOD(ModifiedFollowing)
		{
			date d(2013, 2, 28);
			pJpy->addMonths(d, 1);
			Assert::IsTrue(d == date(2013, 3, 29));
		}

		TEST_METHOD(nextIMM)
		{
			date d(2013, 2, 28);
			pJpy->nextIMMDay(d);
			Assert::IsTrue(d == date(2013, 3, 21));
		}
		TEST_METHOD(nextIMM2)
		{
			date d(2015, 3, 12);
			pJpy->nextIMMDay(d);
			Assert::IsTrue(d == date(2015, 3, 18));
		}
	};

	TEST_CLASS(ZeroCurveUnitTest) {
	public:
		TEST_CLASS_INITIALIZE(ConstructCurve)
		{
			// test class initialization  code
			cd.load("C:\\temp\\curveData.txt");
			pCurve = new Curve(cd.currency, cd.baseDate, cd.daysToSpot, pJpy,
				cd.cash, cd.futures, cd.swaps);
		}

		TEST_METHOD(Cash3M)
		{
			date d(2007, 6, 12);
			DiscountFactorType df = pCurve->interpolate(d);
			Assert::AreEqual(0.998135175, df, 0.00000001);
		}

		TEST_METHOD(FirstSwap)
		{
			date d(2010, 3, 12);
			DiscountFactorType df = pCurve->interpolate(d);
			Assert::AreEqual(0.968739775, df, 0.00000001);
		}
	};
}
