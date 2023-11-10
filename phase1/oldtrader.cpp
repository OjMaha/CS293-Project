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

    std::vector<std::string> splitStrings;
    std::string token;

    // Split the string using the delimiter '#'
    while (std::getline(ss, token, '#')) {
        splitStrings.push_back(token);
    }

    // Now, splitStrings contains the individual substrings
    for (int i = 0; i< splitStrings.size(); i++) {
        int k = 0; //k is counter used to maintain which part of the string is being read (stock, price, b/s)
        std::string stock;
        std::string price;
        std::string bs;

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
        
        stock = tokens[0];
        int p = stoi(tokens[1]);
        price = tokens[1];
        bs = tokens[2];



        // std::string word = "";
        // for (int j=0; j<temp.length(); j++) 
        // {   
        //     if (temp[j] == ' ')
        //     {   
        //         if(k == 0) stock = word;
        //         else if(k == 1) price = word;
        //         k++;
                
        //         word = "";
        //     }
        //     else {
        //         word = word + temp[j];
        //     }
        // }

        // bs = temp[temp.length()-1];

        // int p = stoi(price);

        //TILL HERE; I HAVE INPUTS AND HAVE STOCK(stock), B/S(bs) STORED AS STRING AND PRICE STORED AS INT(p).
        //NOW WE NEED TO PROCESS THIS DATA.

        //THIS IS THE CASE WHERE NEW PREV UNDECLARED STOCK COMES IN. (ADD CONDITION FOR STOCK NOT BEING IN THE LIST)

        //cout << stock << " " << p << " " << bs;
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
