#include "receiver.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../headers/map.cpp"
#include "../headers/string_extra.cpp"
using namespace std;
int main() {
    CustomMap<std::string, int> stockMap;  //gonna store stock name and price
    Receiver rcv;
    
    sleep(5);
    std::string message = rcv.readIML();
    
    // Create a stringstream from the input string
    std::istringstream ss(message);

    std::string eachline;

    // Split the string using the delimiter '#'
    while (std::getline(ss, eachline, '#')) {

        if(eachline == "$") break;

        int k = 0; //k is counter used to maintain which part of the string is being read (stock, price, b/s)
        std::string stock;
        std::string price;
        std::string bs;

        std::istringstream iss(eachline);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(iss, token, ' ')) {
            token = strip(token);
            tokens.push_back(token);
        }
        
        stock = tokens[0];
        int p = stoi(tokens[1]);
        price = tokens[1];
        bs = tokens[2];

         if(stockMap.contains(stock) == false) {
            stockMap.insert(stock, p);
            cout<< stock <<" "<<p<<" ";
            if (bs == "b") cout <<"s"<<endl;
            else cout<<"b"<<endl;
        }

        //THIS IS THE CASE WHERE STOCK IS ALREADY IN THE LIST.
        
        //here i am assuming p_i is the price of last stock order.
        else {
            int p_i = stockMap.get(stock);
            if(p_i < p && bs=="b"){
                stockMap.set(stock, p);
                cout<<stock<<" "<<price<<" "<<"s"<<endl;
            }
            else if(p_i > p && bs=="s"){
                stockMap.set(stock, p);
                cout<<stock<<" "<<price<<" "<<"b"<<endl;
            }
            else cout<<"No trade"<<endl;
        }

        
    }
}

