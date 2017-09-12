#include <cstdlib>
#include <math.h>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include "city.h"
#define pi 3.14159265358979323846

using namespace std;

void getCities();
void displayCities(vector <city>);

int main()
{
	getCities();
	return 0;

}

void getCities() {
	//open file (read only)
	ifstream inputFile;
	inputFile.open("Cities.txt", ios::in);

	//verification of the file
	if (inputFile.fail()) {
		cout << "Error opening the file.\n";
		exit(EXIT_FAILURE);
	}
	else {
		string line;

		vector<city> arrOfCities;
		int counter = 0;
		while (getline(inputFile, line)) {
			//split the string by sphaces
			int spacePos[2];
			int posIndex = 0;
			counter++;

			for (int i = 0; i < line.length(); i++) {
				if (line[i] == ' ') {
					spacePos[posIndex] = i;
					posIndex++;
				}
			}

			std::string cityName = line.substr(0, spacePos[0]);
			double lat = atof((line.substr(spacePos[0] + 1, spacePos[1] - 1)).c_str());
			double longit = atof((line.substr(spacePos[1])).c_str());
			arrOfCities.push_back(city(cityName, lat, longit));
		}
		inputFile.close();

		displayCities(arrOfCities);
	}
}

void displayCities(vector<city> arrOfCities) {
	for (int i = 0; i < arrOfCities.size(); i++) {
		cout << "City: " << arrOfCities[i].getCity();
		cout << " Lat: " << arrOfCities[i].getLat();
		cout << " Long: " << arrOfCities[i].getLong() << endl;
	}
}


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function converts decimal degrees to radians             :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double deg2rad(double deg) {
	return (deg * pi / 180);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function converts radians to decimal degrees             :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double rad2deg(double rad) {
	return (rad * 180 / pi);
}

void distance(double lat1, double lon1, double lat2, double lon2, char unit) {
	double theta, dist;
	theta = lon1 - lon2;
	dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
	dist = acos(dist);
	dist = rad2deg(dist);
	dist = dist * 60 * 1.1515;

	cout << dist;
}
