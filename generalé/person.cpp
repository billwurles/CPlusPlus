#include "stdafx.h"
#include "person.h"

person::person(){}//defualt empty constructor

person::person(string firstName, string lastName, int age)
{
	//set values using setters and passing user supplied values
	setFirstName(firstName);
	setLastName(lastName);
	setAge(age);
}

//setters perform a very basic input validation
//if first name has at least 1 character, consider it a valid input
void person::setFirstName(string fname)
{
	if (fname.length() > 0)
		strFirstName = fname;
	else
	{
		cout << "Invalid input for first name\n";

	}
}

void person::setLastName(string lname)
{
	if (lname.length() > 0)
		strLastName = lname;
	else
	{
		cout << "Invalid input for last name\n";

	}
}

//if age is greater than zero, consider it a valid input
void person::setAge(int age)
{
	if (age > 0)
		intAge = age;
	else
	{
		cout << "Invalid input for age\n";

	}
}

person::~person()
{
}
