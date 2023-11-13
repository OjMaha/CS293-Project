#include "receiver.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include "../headers/string_extra.cpp"
#include "../headers/bst.cpp"

using namespace std;
int main() {
    map<std::string, int> stockMap;  //gonna store stock name and price
    map<std::string, BST> rejected;    //gonna store stock name and rejected trades in a bst (buy orders are positive, sell orders are negative)

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

        int pp = p;             //this is the value in BST
        if(bs == "s") pp*=-1;

         if(stockMap.count(stock) == 0) {
            stockMap.insert({stock, p});
            cout<< stock <<" "<<price<<" ";
            if (bs == "b") cout <<"s"<<endl;
            else cout<<"b"<<endl;
        }

        //THIS IS THE CASE WHERE STOCK IS ALREADY IN THE LIST.
        
        //check for cancellations
        else if(rejected.contains(stock) == true){
            BST temp = rejected[stock];
            cout<<"hai babaaa"<<endl;
            if (temp.search(pp) == true){
                cout<<"No trade"<<endl;
                temp.remove(pp);
                continue;
            }
        }

        //not cancelled, so check for trade
        //here i am assuming p_i is the price of last stock order.
        else {
            int p_i = stockMap[stock];
            if(p_i < p && bs=="b"){
                stockMap[stock] = p;
                cout<<stock<<" "<<price<<" "<<"s"<<endl;
            }
            else if(p_i > p && bs=="s"){
                stockMap[stock] = p;
                cout<<stock<<" "<<price<<" "<<"b"<<endl;
            }

            else {
                //rejected trade
                //add it to the rejected BST
                
                if(rejected.contains(stock) == false){

                    BST temp;
                    temp.insert(pp);
                    cout<<temp.search(pp)<<endl;
                    
                    rejected.insert({stock, temp});
                    cout <<"inserted"<<endl;

                }

                else {
                    BST temp = rejected[stock];
                    temp.insert(pp);
                    rejected[stock] = temp;
                }

                
                cout<<"No trade"<<endl;
            }
        }

        
    }
}

