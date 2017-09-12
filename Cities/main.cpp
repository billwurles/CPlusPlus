#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(){
    string x = "hello hello";
    string *y = &x;
    cout << y << "\n"; // prints memory address
    cout << *y << "\n"; // prints value of y
    
    *y += " world";
    
    cout << *y << "\n"; // prints value of y
    cout << x << "\n"; // prints value of x
    
}
