#include "BigNumber.h"
#include <iostream>
using namespace std;

int main() {
	string str1,str2;
	while(cin>>str1>>str2) {
		zbz::BigNumber a = str1;
		zbz::BigNumber b = str2;
		cout << a << " * " << b << " = " << a * b << endl;
		cout << a << " + " << b << " = " << a + b << endl; 
		cout << a << " - " << b << " = " << a - b << endl;
		cout << a << " / " << b << " = " << a / b << endl;
	}
     
	 return 0;
}



