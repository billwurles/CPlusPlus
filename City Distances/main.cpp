#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void fileWrite();
void fileRead();

int main(){
    fileRead();
}

//this allows you to add the city to the list
void fileWrite(){
    ofstream theFile("cities.txt");

    cout << "Enter the city, latitude and longitude" << endl;

    string cityName;
    double lati;
    double longi;

    cout <<  "CityName: ";
    cin >> cityName;
    cout << "Latitude: ";
    cin >> lati;
    cout << "Longitude: ";
    cin >> longi;
    theFile << cityName << ' ' << lati << ' ' << longi << endl;
}

//void fileRead(){
//    ifstream file;
//    string line;
//    file.open("cities.txt");
//    while (getline (file, line))
//      cout << line << endl;
//}

void fileRead(){
vector <string> v;
string line;
ifstream fin("cities.txt");
while(getline(fin,line)){
    v.push_back(line);
}
}
