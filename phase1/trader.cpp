#include "receiver.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;
int main() {

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

        std::string word = "";
    for (int j=0; j<temp.length(); j++) 
    {   
        if (temp[j] == ' ')
        {   
            if(k == 0) stock = word;
            else if(k == 1) price = word;
            k++;
            
            word = "";
        }
        else {
            word = word + temp[j];
        }

        
        
    }

    bs = temp[temp.length()-1];

    int p = stoi(price);

    //TILL HERE; I HAVE INPUTS AND HAVE STOCK(stock), B/S(bs) STORED AS STRING AND PRICE STORED AS INT(p).
    //NOW WE NEED TO PROCESS THIS DATA.

}

}
