#ifndef CITY_H
#define	CITY_H
#include <iostream>
#include <string>


class city {
private:
	std::string strCity;
	double dLatitude;
	double dLongitude;

public:
    city(std::string, double, double);
    //set values
    void setCity(std::string);
    void setLatitude(double);
    void setLongitude(double);

    //get values
    std::string getCity() const {
        return strCity;
    }
    double getLat() const {
        return dLatitude;
    }
    double getLong() const {
        return dLongitude;
    }


};

#endif	/* CITY_H */

