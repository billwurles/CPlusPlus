#ifndef PERSON_H
#define PERSON_H

#include <iostream>
using namespace std;

class person
{
	private:
		string strFirstName;
		string strLastName;
		int intAge;

	public:
		person(); //defualt constructor
		person(string, string, int);
		~person();

		//setting values
		void setFirstName(string);
		void setLastName(string);
		void setAge(int);

		//get values
		string getFirstName() const { return strFirstName; }
		string getLastName() const { return strLastName; }
		int getAge() const { return intAge; }

};

#endif

