// FilesAndObjectsYT.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include "person.h"

//prototypes
void GetData();
double CalculateAvgAge(vector <person>, int);
void displayOutput(vector <person>, double);

int _tmain(int argc, _TCHAR* argv[])
{
	GetData();
	return 0;
}

void GetData()
{
	//open file for read only
	//double \\ are needed to map the path correctly
	ifstream inputFile;
	inputFile.open("C:\\Temp\\Program.txt", ios::in);

	//verify the file opened
	if (inputFile.fail())
	{
		cout << "sfvsafsf Unable to open or process the file.\n";
	}
	else
	{
		string line;

		vector <person> arrOfNames;
		int counter = 0; //counts the number of elements in vector
		while (getline(inputFile, line))
		{
			//split the string by spaces
			//there are 2 spaces, so we can declare array with 2 indexes
			//that hoild the position of each space
			int spacePos[2];
			int posIndex = 0;
			counter++;

			for (int i = 0; i < line.length(); i++)
			{
				if (line[i] == ' ')
				{
					spacePos[posIndex] = i;
					posIndex++;
				}
			}

			//now we have position of the spaces, so we can delimit the line into 3 words
			string firstName = line.substr(0, spacePos[0]); //starts from zero, goes to first index in spacePos[]

			//last name starts from first space + 1, and the length is second space position - first space position minus 1
			string lastName = line.substr(spacePos[0] + 1, spacePos[1] - spacePos[0] - 1);
			int age = 10;
			arrOfNames.push_back(person(firstName, lastName, age));
		}

		inputFile.close();

		double avg = CalculateAvgAge(arrOfNames, counter);

		displayOutput(arrOfNames, avg);
	}
}

double CalculateAvgAge(vector<person> arrOfNames, int counter)
{
	double avg = 0;
	int total = 0;

	//loop through the vector with all the names and sum all the ages
	for (int i = 0; i < counter; i++)
		total += arrOfNames[i].getAge();

	avg = total * 1.0 / counter;
	return avg;
}

void displayOutput(vector<person> arrOfNames, double avg)
{
	//display all names in formatted output
	for (int i = 0; i < arrOfNames.size(); i++)
	{
		cout << "PERSON # " << i + 1 << endl;
		cout << "FIRST NAME: " << arrOfNames[i].getFirstName() << endl;
		cout << "LAST NAME: " << arrOfNames[i].getLastName() << endl;
		cout << "AGE: " << arrOfNames[i].getAge() << endl;
		cout << "--------------------------------------------------------\n";
	}

	cout << "\nAVERAGE AGE IS " << setprecision(2) << fixed << avg << " YEARS." << endl;
}

