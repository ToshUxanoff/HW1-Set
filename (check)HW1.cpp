#include "stdafx.h"
#include "TSet.h"

int main()
{
	SetClass<int> Test{ 3,2,1,5,4,8,6,9,0 };
	
	std::cout << "Begin Before erase:" << *Test.begin() << std::endl; //0
	Test.erase(0);
	
	std::cout << "Begin After erase:" << *Test.begin() << std::endl; //1
	std::cout << "Size before clear:" << Test.size() << std::endl; //8

	Test.clear();

	Test.insert(7);
	Test.insert(9);
	Test.insert(5);
	
	Test.find(9);

	std::cout << "Size after clear and insert:" <<Test.size() << std::endl; //3
	std::cout << "Begin: " << *Test.begin() << std::endl; //5
	std::cout << "Count (9) =  " << Test.count(9) << std::endl; //1

	SetClass<int> Test2{ 7,9,5 };
	std::cout <<"Lower bound(7): " << *Test2.lower_bound(7) << std::endl; //9
	std::cout << "Upper bound(7): " << *Test2.upper_bound(7) << std::endl; //7

	std::cout <<"Compare trees: " <<(Test2 == Test)<< std::endl; //1
	system("pause");
	return 0;
}
