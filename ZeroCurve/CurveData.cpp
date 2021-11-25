#include "CurveData.h"
#include <string>
#include <fstream>
#include <cstring>

using std::string;
using std::ifstream;
using std::ofstream;

#define CurveDatalog	"C:\\temp\\CurveData.log"

const char* MONTH[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

bool
CurveData::load(const char* filename) {
	return true;
}