#include <iostream>
#include "BigNumber.h"
using namespace std;

namespace zbz {

//用'0'填充整数到指定长度
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

/*相同长度的无符号大整数相加*/
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

//无符号整数加
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

//无符号浮点数整数部分相加
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

//正数（支持小数点）加法
static string add_positive_number(string str1, string str2) {
	string float1, float2;
	string integer1 = str1, integer2 = str2;
	//获取整数部分
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

// 相同长度字符串，大减小
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

	//	去掉开头多余的'0'
	int i = 0;
	while (str1[i] == '0') ++i;
	if (i > 0) str1.erase(0, isFloat ? i - 1 : i);

	return str1;
}


//正数（支持小数点）减法
static string sub_positive_number(string str1, string str2) {
	string float1, float2;
	string integer1 = str1, integer2 = str2;
	//获取整数部分
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

	//将长度小的整数部分在高位填充‘0’
	if (integer1.size() > integer2.size()) {
		fillzero(integer2, integer1.size(), true);
	} else {
		fillzero(integer1, integer2.size(), true);
	}

	//将长度小的小数部分在低位填充‘0’
	if (float1.size() > float2.size()) {
		fillzero(float2, float1.size(), false);
	} else {
		fillzero(float1, float2.size(), false);
	}

	bool isNegative = false;
	str1 = integer1 + "." + float1;
	str2 = integer2 + "." + float2;

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

///数的减法，支持浮点、正负
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

///数的加法，支持浮点、正负
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

/// 大数乘法，支持正负小数
/// @param str1 乘数1
/// @param str2 乘数2
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

} // end of using namespace zbz

  
 int main() {
	 string str1,str2;
	 while(cin>>str1>>str2) {
         cout<< str1<<" * "<<str2<<" = " << zbz::mul_big_number(str1, str2) << endl;
         cout<< str1<<" + "<<str2<<" = " << zbz::add_big_number(str1, str2) << endl; 
         cout<< str1<<" - "<<str2<<" = " << zbz::sub_big_number(str1, str2) << endl;
	 }
     
	 return 0;
}
