#include "CurveData.h"
#include <string>
#include <fstream>
#include <cstring>

using std::string;
using std::ifstream;
using std::ofstream;
using namespace std;

#define CurveDatalog	"C:\\temp\\CurveData.log"

const char* MONTH[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

bool
CurveData::load(const char* filename) {
	ifstream infile(filename);
	if (!infile) cerr << "error: unable to open inout file:" << filename << endl;
	string temp;
	infile >> temp;
	cout << temp << endl;

	return true;
}