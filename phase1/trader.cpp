#include "receiver.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
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
    

    
    return 0;
}
