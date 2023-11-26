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
    std::string eachline;

    // Create a stringstream from the input string
    std::istringstream ss(message);

    std::vector<std::string> splitStrings;
    std::string token;

    // Split the string using the delimiter '#'
    while (std::getline(ss, token, '#')) {
        splitStrings.push_back(token);
    }


    for (int i = 0; i< splitStrings.size(); i++){
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
        int pp = p;             //this is the value in BST
        if(bs == "s") pp*=-1;

        if(stockMap.contains(stock) == false) {
            stockMap.insert({stock, p});
            cout<< stock <<" "<<price<<" ";
            if (bs == "b") cout <<"s"<<endl;
            else cout<<"b"<<endl;
        }
        
        //THIS IS THE CASE WHERE STOCK IS ALREADY IN THE LIST.
        
        //first check for cancellations
        else if(rejected.contains(stock) == true && rejected[stock].search(pp) == true){
                rejected[stock].remove(pp);
                cout << "No Trade";
        }

        //not cancelled, so check for trade

        else if(stockMap[stock] < p && bs=="b"){
            stockMap[stock] = p;
            cout<<stock<<" "<<price<<" "<<"s"<<endl;
        }
        else if(stockMap[stock] > p && bs=="s"){
            stockMap[stock] = p;
            cout<<stock<<" "<<price<<" "<<"b"<<endl;
        }

        else {
            //rejected trade
            //add it to the rejected BST
            
            if(rejected.contains(stock) == false){
            
                rejected[stock] = BST(); 
                rejected[stock].insert(pp,0);
 
            }

            else {
                rejected[stock].insert(pp,0);
            }
            
            cout<<"No Trade"<<endl;

            
        }
            
    }
}

//     // Split the string using the delimiter '#'
//     while (std::getline(ss, eachline, '#')) {

//         if(eachline == "$") break;

//         int k = 0; //k is counter used to maintain which part of the string is being read (stock, price, b/s)
//         std::string stock;
//         std::string price;
//         std::string bs;

//         std::istringstream iss(eachline);
//         std::string token;
//         std::vector<std::string> tokens;

//         while (std::getline(iss, token, ' ')) {
//             token = strip(token);
//             tokens.push_back(token);
//         }
        
//         stock = tokens[0];
//         int p = stoi(tokens[1]);
//         price = tokens[1];
//         bs = tokens[2];

//         int pp = p;             //this is the value in BST
//         if(bs == "s") pp*=-1;
//         // cout<<"haibhainau";
//         if(stockMap.count(stock) == 0) {
//             stockMap.insert({stock, p});
//             cout<< stock <<" "<<price<<" ";
//             if (bs == "b") cout <<"s"<<endl;
//             else cout<<"b"<<endl;
//         }

//         //THIS IS THE CASE WHERE STOCK IS ALREADY IN THE LIST.
        
//         //check for cancellations
//         else if(rejected.contains(stock) == true){
//             //cout<<stock<<"rejected contains"<<endl;
//             BST temp = rejected[stock];
//             //cout<<rejected[stock].root->price<<endl;
//             // for (auto& pair : rejected) {
//             //         std::cout << "Key: " << pair.first << ",root value: ";
//             //         cout<<pair.second.root->price<<endl;
//             //     }
//             //cout<<"tryna";
//             if (temp.search(pp) == true){
//                 // cout<<"cancelled"<<endl;
//                 // cout<<"No Trade"<<endl;
//                 temp.remove(pp);
//                 rejected[stock] = temp;
//             }
//         }

//         //not cancelled, so check for trade
//         //here i am assuming p_i is the price of last stock order.
//         else {
//             //cout<<"not cancelled"<<endl;
//             int p_i = stockMap[stock];
//             if(p_i < p && bs=="b"){
//                 stockMap[stock] = p;
//                 cout<<stock<<" "<<price<<" "<<"s"<<endl;
//             }
//             else if(p_i > p && bs=="s"){
//                 stockMap[stock] = p;
//                 cout<<stock<<" "<<price<<" "<<"b"<<endl;
//             }

//             else {
//                 //rejected trade
//                 //add it to the rejected BST
                
//                 if(rejected.contains(stock) == false){
                
//                     rejected[stock] = BST(); 
//                     rejected[stock].insert(pp,0);
 
//                 }

//                 else {
//                     //cout<<"ghuso"<<endl;
//                     rejected[stock].insert(pp,0);
//                     //cout<<"insertin"<<pp<<endl;
//                 }
                
//                 cout<<"No Trade"<<endl;

                
//             }
//         }
        
//     // for(auto& pair : rejected){
//     //                 cout<<"Key: "<<pair.first<<", root value: ";
//     //                 cout<<pair.second.root->price<<endl;
//     //             }
        
//     }
// }

