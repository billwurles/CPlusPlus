#include "city.h"
#include <string>



city::city(std::string cityName, double latitude, double longitude)
{
    setCity(cityName);
    setLatitude(latitude);
    setLongitude(longitude);
}

void city::setCity(std::string nameOfCity)
{
	std::string strCity = nameOfCity;
}

void city::setLatitude(double latitude)
{
	double dLatitude = latitude;
}


void city::setLongitude(double longitude)
{
	double dLongitude = longitude;

}

