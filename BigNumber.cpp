#include <cstdlib>
#include "BigNumber.h"
using namespace std;

namespace zbz {

	const string ErrorStr = "error";
	const int SPACE = 32;

/// Fill string to special size with '0'
static int fillzero(string& str, int size, bool inPrefix) {
	if (str.size() > size) {
		return -1;
	}

	int nZero = size - str.size();
	if (inPrefix) {
		str.insert(0, nZero, '0');
	} else {
		if (str.size() > 0)
            str.insert(str.size() - 1, nZero, '0');
        else
            str.insert(0, nZero, '0');
	}

	return 0;
}

/// Comparison of two number strings
static bool numge(const string& str1, const string& str2) {
	return str1.size() > str2.size() || (str1.size() == str2.size() && str1 >= str2);
}

static size_t strtrim(const string& str) {
	size_t i = 0;
	//trim space
	while (str[i] == 32 && i < str.size()) ++i;

	//trim 0
	i = 0;
	while (str[i] == '0' && str[i + 1] != '.' && i < str.size() - 1) ++i;

	return i;
}

/// Addition of positive integer strings
static string add_same_size_positive_number(string str1, string str2, bool& highCarry) {
    int i;
   	reverse(str1.begin(), str1.end());
   	reverse(str2.begin(), str2.end());
   	string res = str1;
   	res.push_back('0');
   	int carry = 0;
   	for (i = 0; i < str2.size(); ++i) {
    	int tmp = res[i] - '0' + str2[i] - '0' + carry;
    	int v = tmp % 10;
    	carry = tmp / 10;
    	res[i] = v + '0';
   	}

   	if (carry > 0) {
   		res[i] += carry;
   		highCarry = true;
   	} else if (res.back() == '0') {
   		res.erase(res.size() - 1, 1);
   		highCarry = false;
   	}

   	reverse(res.begin(), res.end());
   	return res;
}

/// Addition of positive integer strings
static string add_unsigned_number(string str1, string str2) {
	bool highCarry;
	int size1 = str1.size();
	int size2 = str2.size();
	if (size1 < size2) {
		fillzero(str1, size2, true) ;
	} else {
		fillzero(str2, size1, true) ;
	}


	return add_same_size_positive_number(str1, str2, highCarry);
}

/// Addition of integer in positive float number strings
static string add_positive_number_of_float(string str1, string str2, bool& highCarry) {
	int size1 = str1.size();
	int size2 = str2.size();
	if (size1 < size2) {
		fillzero(str1, size2, false) ;
	} else {
		fillzero(str2, size1, false) ;
	}

	return add_same_size_positive_number(str1, str2, highCarry);
}

/// Addition of positive (float) big number
static string add_positive_number(string str1, string str2) {
	string float1, float2;
	string integer1 = str1, integer2 = str2;
	size_t found1 = str1.find('.');
	size_t found2 = str2.find('.');
	if (found1 != std::string::npos) {
		integer1 = str1.substr(0, found1);
		float1 = str1.substr(found1 + 1, str1.size() - found1);
	}

	if (found2 != std::string::npos) {
		integer2 = str2.substr(0, found2);
		float2 = str2.substr(found2 + 1, str2.size() - found2);
	}

	bool floatHighCarry;
	string floatRes;
	string integerRes = add_unsigned_number(integer1, integer2);
	if (float1.size() > 0 || float2.size() > 0) {
		floatRes = add_positive_number_of_float(float1, float2, floatHighCarry);
	}

	if (floatHighCarry) {
		integerRes = add_unsigned_number(integerRes, "1");
	}
	
	if (floatRes.size() == 0) {
		return integerRes;
	}

	return integerRes + "." + floatRes;
}

/// Subtraction of same size positive integer strings, big subtract small
static string sub_same_size_positive_number_great_small(string str1, string str2) {
    bool isFloat = false;
	reverse(str1.begin(), str1.end());
	reverse(str2.begin(), str2.end());

	for (int i = 0; i < str1.size(); ++i) {
		if (str1[i] == '.' && str2[i] == '.') {
            isFloat = true;
			continue;
		}

		if (str1[i] < str2[i]) {
			int j = i;
			while (str1[++j] == '0') {
				str1[j] = '9';
			}

            if (str1[j] == '.') str1[j+1] -= 1;
            else str1[j] -= 1;
			str1[i] += 10;
		}

		str1[i] = '0' + str1[i] - str2[i];
	}

	reverse(str1.begin(), str1.end());

	//	trim '0'
	int i = 0;
	while (str1[i] == '0' && i < str1.size()) ++i;
	if (i > 0) str1.erase(0, (str1.size() == i || isFloat) ? i - 1 : i);

	return str1;
}


/// Addition of big positive float numbers
static string sub_positive_number(string str1, string str2) {
	string float1, float2;
	string integer1 = str1, integer2 = str2;
	// get integer string
	size_t found1 = str1.find('.');
	size_t found2 = str2.find('.');
	if (found1 != std::string::npos) {
		integer1 = str1.substr(0, found1);
		float1 = str1.substr(found1 + 1, str1.size() - found1);
	}

	if (found2 != std::string::npos) {
		integer2 = str2.substr(0, found2);
		float2 = str2.substr(found2 + 1, str2.size() - found2);
	}

	// fille '0' in the front of integer number string which size is samll
	if (integer1.size() > integer2.size()) {
		fillzero(integer2, integer1.size(), true);
	} else {
		fillzero(integer1, integer2.size(), true);
	}

	// fille '0' in the back of integer number float string which size is samll
	if (float1.size() > float2.size()) {
		fillzero(float2, float1.size(), false);
	} else {
		fillzero(float1, float2.size(), false);
	}

	bool isNegative = false;
	if (float1.size() > 0) 
		str1 = integer1 + "." + float1;
	else 
		str1 = integer1;
	if (float2.size() > 0) 
		str2 = integer2 + "." + float2;
	else
		str2 = integer2;

	if (str1 < str2) {
		isNegative = true;
		swap(str1, str2);
	} 

	string subRes = sub_same_size_positive_number_great_small(str1, str2);
	if (isNegative) {
		subRes.insert(0, 1, '-');
	}

	return subRes;
}

/// Subtraction of big numbers, support negative float number
string sub_big_number(string str1, string str2) {
	if (str1[0] != '-' && str2[0] == '-') { //+ -
		str2 = str2.substr(1, str2.size() - 1);
		return add_positive_number(str1, str2);
	} else if (str1[0] == '-' && str2[0] != '-') {// - +
		str1 = str1.substr(1, str1.size() - 1);
		return "-" + add_positive_number(str1, str2);
	} else if (str1[0] != '-' && str2[0] != '-') {//+ +
		if (str1[0] == '+') {
			str1 = str1.substr(1, str1.size() - 1);
		}

		if (str2[0] == '+') {
			str2 = str2.substr(1, str2.size() - 1);
		}

		return sub_positive_number(str1, str2);
	} else { //- -
		str1 = str1.substr(1, str1.size() - 1);
		str2 = str2.substr(1, str2.size() - 1);
		return sub_positive_number(str2, str1);
	}

}

/// Addition of big number ,support negative float number
string add_big_number(string str1, string str2) {
	if (str1[0] != '-' && str2[0] == '-') { //+ -
		str2 = str2.substr(1, str2.size() - 1);
		return sub_positive_number(str1, str2);
	} else if (str1[0] == '-' && str2[0] != '-') {// - +
		str1 = str1.substr(1, str1.size() - 1);
		return sub_positive_number(str2, str1);
	} else if (str1[0] != '-' && str2[0] != '-') {// + +
		if (str1[0] == '+') {
			str1 = str1.substr(1, str1.size() - 1);
		}

		if (str2[0] == '+') {
			str2 = str2.substr(1, str2.size() - 1);
		}
		return add_positive_number(str1, str2);
	} else { // - -
		str1 = str1.substr(1, str1.size() - 1);
		str2 = str2.substr(1, str2.size() - 1);
		return "-" + add_positive_number(str1, str2);
	}

}

///  Multiplication of big number, support negative float number
/// @param str1 multiplier one
/// @param str2 multiplier two
string mul_big_number(string str1, string str2) {
    bool isNeg = false;
    if (str1[0] == '+' || str1[0] == '-') {
        if (str1[0] == '-') {
            isNeg = true;
        }

        str1 = str1.substr(1, str1.size() - 1);
    }

    if (str2[0] == '+' || str2[0] == '-') {
        if (str2[0] == '-') {
            isNeg = false;
        }

        str2 = str2.substr(1, str2.size() - 1);
    }

    int floatCount1 = 0, floatCount2 = 0;
    size_t found1 = str1.find(".");
    size_t found2 = str2.find(".");
    if (found1 != std::string::npos) {
        floatCount1 = str1.size() - 1 - found1;
        str1.erase(found1, 1);
    }

    if (found2 != std::string::npos) {
        floatCount2 = str2.size() - 1 - found2;
        str2.erase(found2, 1);
    }

    int size1 = str1.size(), size2 = str2.size();
    string str(size1 + size2, '0');
    for (int i = size2 - 1; i >= 0; --i) {
        int mulflag = 0, addflag = 0;
        for (int j = size1 - 1; j >= 0; --j) {
            int temp1 = (str2[i] - '0') * (str1[j] - '0') + mulflag;
            mulflag = temp1 / 10;
            temp1 = temp1 % 10;
            int temp2 = str[i+j+1] - '0' + temp1 + addflag;
            str[i+j+1] = temp2 % 10 + '0';
            addflag = temp2 / 10;
        }
        
        str[i] += mulflag + addflag;
    }
         
    if (str[0]=='0') str=str.substr(1,str.size());
    if (isNeg) str.insert(0, "-");
    if (floatCount1 + floatCount2 > 0) {
        str.insert(str.size() - (floatCount1 + floatCount2), 1, '.' );
    }
    
    return str;
}

/// Division of big positive number
/// !warning:it is very slow
/// @param dividend dividend
/// @param divisor divisor
/// @param pointNum max float number
static string div_big_positive_integer_number(string dividend, string divisor, int pointNum) {
	string integerStr, floatStr;
	int floatNum = 0;
	uint64_t nInteger = 0;
	uint64_t nFloat = 0;

	while (numge(dividend, divisor)) {
		++nInteger;
		dividend = sub_big_number(dividend, divisor);
		if (dividend == "0") { // Divisible
			return to_string(nInteger);
		}
	}

	string floatRes;
	while (floatNum < pointNum) {
		nFloat = 0;
		dividend += '0';
		++floatNum;
		while (numge(dividend, divisor)) {
			dividend = sub_big_number(dividend, divisor);
			++nFloat;
		}

		floatRes.push_back('0' + nFloat);
		if (dividend == "0") { // Divisible
			break;
		}
	}

	if (nFloat == 0) 
		return to_string(nInteger);
	else 
		return to_string(nInteger) + '.' + floatRes;
}

/// A function to perform division of large numbers
static string div_big_positive_integer_number_small_divisor(string number, int64_t divisor) {
    // As result can be very large store it in string
    string ans;
 
    // Find prefix of number that is larger
    // than divisor.
    int idx = 0;
    int64_t size = number.size();
    int64_t temp = number[idx] - '0';
    while (temp < divisor && idx < size - 1)
        temp = temp * 10 + (number[++idx] - '0');
 
    // Repeatedly divide divisor with temp. After
    // every division, update temp to include one
    // more digit.
    int64_t leave;
    while (number.size() > idx) {
        // Store result in answer i.e. temp / divisor
        ans += (temp / divisor) + '0';
        leave = temp % divisor;
 
        // Take next digit of number
        temp = (temp % divisor) * 10 + number[++idx] - '0';
    }
 
    //point number
    double fLeave = 1.0 * leave / divisor;
    if (fLeave != 0) 
        ans += to_string(fLeave);
    // If divisor is greater than number
    if (ans.length() == 0)
        return "0";
 
    // else return ans
    return ans;
}

/// Big numbrer divsion，support negative and float number
/// @param dividend dividend
/// @param divisor divisor
/// @param pointNum max float number
string div_big_number(string dividend, string divisor, int pointNum) {
	bool isNegative = false;
	if (dividend[0] != '-' && divisor[0] == '-') {
		isNegative = true;
		divisor = divisor.substr(1, divisor.size() - 1);
	} else if (dividend[0] == '-' && divisor[0] != '-') {
		isNegative = true;
		dividend = dividend.substr(1, dividend.size() - 1);
	}

	uint64_t floatNum1 = 0, floatNum2 = 0;
	size_t found1 = dividend.find('.');
	size_t found2 = divisor.find('.');
	if (found1 != std::string::npos) {
		floatNum1 = dividend.size() - 1 - found1;
		dividend.erase(found1, 1);
	}

	if (found2 != std::string::npos) {
		floatNum2 = divisor.size() - 1 - found2;
		divisor.erase(found2, 1);
	}

	//test divided zero exception
	size_t start = strtrim(divisor);
	divisor = divisor.substr(start, divisor.size() - start);
	if (divisor == "0")
		return ErrorStr;

	start = strtrim(dividend);
	dividend = dividend.substr(start, dividend.size() - start);

	if (floatNum1 < floatNum2) {
		dividend.append(floatNum2 - floatNum1, '0');
	} else if (floatNum1 > floatNum2) {
		divisor.append(floatNum1 - floatNum2, '0');
	}

	string res;
	if (divisor.size() < 64 - 1) {
		int64_t iDivisor = atoll(divisor.c_str());
		res = div_big_positive_integer_number_small_divisor(dividend, iDivisor);
	} else {
		res = div_big_positive_integer_number(dividend, divisor, pointNum);
	}
	 
	return isNegative ? "-" + res : res;

}


} // end of using namespace zbz
