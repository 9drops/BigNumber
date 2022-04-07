#include "BigNumber.h"
#include <iostream>
using namespace std;

int main() {
	 string str1,str2;
	 while(cin>>str1>>str2) {
        //  cout << str1 << " * " << str2 << " = " << zbz::mul_big_number(str1, str2) << endl;
        //  cout << str1 << " + " << str2 << " = " << zbz::add_big_number(str1, str2) << endl; 
        //  cout << str1 << " - " << str2 << " = " << zbz::sub_big_number(str1, str2) << endl;
		 cout << str1 << " / " << str2 << " = " << zbz::div_big_number(str1, str2, 6) << endl;
	 }
     
	 return 0;
}



