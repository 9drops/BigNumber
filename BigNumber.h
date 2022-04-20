#ifndef __BigNumber_H__
#define __BigNumber_H__

#include <string>
#include <ostream>

namespace zbz {

class BigNumber {
    public:
        static int MaxPointNum;
        BigNumber(std::string v) : value_(v) {}
        const std::string& getValue() const {
            return value_;
        }

    private:
        std::string value_;
    
};

std::string operator + (const BigNumber& a, const BigNumber& b);
std::string operator - (const BigNumber& a, const BigNumber& b);
std::string operator * (const BigNumber& a, const BigNumber& b);
std::string operator / (const BigNumber& a, const BigNumber& b);

std::ostream& operator << (std::ostream&, const BigNumber&);

}

#endif
