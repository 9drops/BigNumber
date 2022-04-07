#ifndef __BigNumber_H__
#define __BigNumber_H__

#include <string>

namespace zbz {

std::string add_big_number(std::string str1, std::string str2);
std::string sub_big_number(std::string str1, std::string str2);
std::string mul_big_number(std::string str1, std::string str2);
std::string div_big_number(std::string str1, std::string str2, int pointNum);

}

#endif
