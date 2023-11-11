#include "receiver.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../headers/map.cpp"
#include "../headers/string_extra.cpp"
using namespace std;

struct Order{
    CustomMap<std::string, int> stonks;
    int price = 0;
    char bs = 0;             //'b' for buy, 's' for sell
    bool validity = true; //true if order is valid, false if order is invalid
    vector <string> stonklist; ///maintain a list of stocks involved in an order

};
int main() {

    vector <Order> orderlist;

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

        Order o;
        
        int size = tokens.size();
        if(tokens[size-1] == "b") o.bs = 'b';
        else o.bs = 's';

        o.price = stoi(tokens[size-2]);

        for(int j = 0; j<size-2; j+=2){
            o.stonks.insert(tokens[j], stoi(tokens[j+1]));
            o.stonklist.push_back(tokens[j]); 
        }

        orderlist.push_back(o);

        for(int j = 0; j<orderlist.size(); j++){
            for(int k = 0; k<orderlist[j].stonklist.size(); k++){
                cout<<orderlist[j].stonklist[k] << " ";
                cout<<orderlist[j].stonks.get(orderlist[j].stonklist[k]) << " ";
            }
            cout << orderlist[j].price << " " << orderlist[j].bs << "#" << endl;
        }
}
}