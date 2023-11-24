#include "receiver.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../headers/map.cpp"
#include "../headers/string_extra.cpp"
//hello
using namespace std;
int main() {
    CustomMap<std::string, int> stockMap;  //gonna store stock name and price
    CustomMap<std::string, int> buyMap;    //gonna store stock name and lowest rejected buy price
    CustomMap<std::string, int> sellMap;   //gonna store stock name and highest rejected sell price
    std::string firsthalfline;
    bool broken = false;
    Receiver rcv;
    int i = 0;
    
    sleep(5);
    while(true){
        std::string message = rcv.readIML();
    
        // Create a stringstream from the input string
        std::istringstream ss(message);
        std::string eachline;
        

        // Split the string using the delimiter '#'
        while (std::getline(ss, eachline, '#')) {
            if(broken){
                eachline = firsthalfline + eachline; //concatenate the broken line with the next line
                broken = false;
                cout << "concatenated line" << eachline << endl;
            }

            if(eachline == "$") break;
            if(eachline == "") continue;

            char bs = eachline[eachline.size()-1];
            if(bs != 'b' && bs != 's'){             //broken line; must continue
                broken = true;
                firsthalfline = eachline;
                cout << "broken line" << firsthalfline << endl;
                break;
            }

            cout << i++ << "  " << eachline << endl;
            //cout << eachline << endl;
        }
        if(eachline == "$"){
            cout << "I'm Done here bro" << endl;
            break;
        }

    }
}