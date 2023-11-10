#include "receiver.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../headers/map.cpp"
#include "../headers/string_extra.cpp"
using namespace std;

struct order{
    CustomMap<std::string, int> stonks;
    int price;
    int bs;             //0 for buy, 1 for sell

};
int main() {

    vector <order> orderlist;

    Receiver rcv;
    sleep(5);
    std::string message = rcv.readIML();
    

    // Create a stringstream from the input string
    std::istringstream ss(message);

    std::vector<std::string> splitStrings;
    std::string token;

    // Split the string using the delimiter '#'
    while (std::getline(ss, token, '#')) {
        splitStrings.push_back(token);
    }

    // Now, splitStrings contains the individual substrings
    for (int i = 0; i< splitStrings.size(); i++) {
        int k = 0; //k is counter used to maintain which part of the string is being read (stock, price, b/s)
        std::string temp = splitStrings[i];

        if (temp == "$") break;

        //TK: I'm changing the spllitting function as this is more compact and scalable.

        std::istringstream iss(temp);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(iss, token, ' ')) {
            token = strip(token);
            tokens.push_back(token);
        }

        order o;
        
        int size = tokens.size();
        if(tokens[size-1] == "b") o.bs = 0;
        else o.bs = 1;

        o.price = stoi(tokens[size-2]);

        for(int j = 0; j<size-2; j+=2){
            o.stonks.insert(tokens[j], stoi(tokens[j+1]));
        }

        orderlist.push_back(o);

        for(int j = 0; j<orderlist.size(); j++){
            for(int k = 0; k<orderlist[j].stonks.getSize(); k++){
                cout<<orderlist[j].stonks.get(k)<<endl;
            }
        }
}
}