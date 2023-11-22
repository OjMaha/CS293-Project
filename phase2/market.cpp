#include "market.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

//
struct executed_order{
    string buyer;
    string seller;
    string stonk;
    int price;
    int quantity;
};

void print_order(order o){
    cout << "issue time:"<<o.issue << " " <<"broker:"<< o.broker << " " <<"BS"<< o.bs << " " << o.stonk 
    << " " <<"price"<< o.price << " " << "quantity"<<o.quantity << " " <<"expiresat:"<< o.expiry << endl;
}

void print_executed_order(executed_order e){
    cout << "buyer:"<<e.buyer << " " <<"seller:"<< e.seller << " " <<"stonk:"<< e.stonk 
    << " " <<"price:"<< e.price << " " << "quantity:"<<e.quantity << endl;
}



market::market(int argc, char** argv)
{   
    // Open a file for reading
    std::ifstream inputFile("./my_input.txt");
    // Check if the file is open
    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
    }

    // Read and parse the file line by line
    std::string line;
    while (std::getline(inputFile, line, '\n')) {
        // Process each line (parse as needed)
        if(line == "TL") continue;
        if (line == "!@") break;

        vector<string> tokens;
        std::istringstream ss(line);
        while(std::getline(ss, line, ' ')) {
            tokens.push_back(line);
        }

        int start_time = stoi(tokens[0]);
        string broker = tokens[1];
        if(tokens[2] == "BUY") tokens[2] = "1";
        else tokens[2] = "-1";
        int bs = stoi(tokens[2]);
        string stonk = tokens[3];
        int price = stoi(tokens[4].substr(1)); //remove $ sign
        int quantity = stoi(tokens[5].substr(1)); //remove # sign
        int expires_after = stoi(tokens[6]);

        order o;

        o.issue = start_time;
        if(expires_after >= 0){
            o.expiry = start_time + expires_after;
        }
        else{
            o.expiry = -1;      // -1 for infinite expiry
        }

        o.broker = broker;
        o.bs = bs;
        o.stonk = stonk;
        o.price = price;
        o.quantity = quantity;

        if(bs == 1) {
            buy_orders.push_back(o);
            indices[all_num] = buy_num;
            buy_num++;
        }
        else{
            sell_orders.push_back(o);
            indices[all_num] = sell_num;
            sell_num++;
        }

        all_orders.push_back(o);
        all_num++;
    }

    // Close the file
    inputFile.close();
}

//note that buy_orders, sell_orders and all_orders are sorted by time

void market::start()
{  
    vector<executed_order> executed;

    for(int i=0;i<all_orders.size();i++){
        if(all_orders[i].bs == 1){
            for(int j = 0; j< sell_orders.size(); j++){
                order b = all_orders[i];
                order s = sell_orders[j];

                if(s.issue > b.issue) break;
                if(valid_orders(b,s)){
                    executed_order e;
                    e.buyer = b.broker;
                    e.seller = s.broker;
                    e.stonk = b.stonk;
                    e.price = s.price;
                    e.quantity = min(b.quantity, s.quantity);

                    if (e.quantity == 0) continue;

                    executed.push_back(e);

                    all_orders[i].quantity -= e.quantity;
                    sell_orders[j].quantity -= e.quantity;
                    buy_orders[indices[i]].quantity -= e.quantity;

                    print_executed_order(e);
                    if(b.quantity == 0) break;
                }
            }
        }

        else{
            for(int j = 0; j< buy_orders.size(); j++){
                order b = buy_orders[j];
                order s = all_orders[i];

                if(b.issue > s.issue) break;
                if(valid_orders(b,s)){
                    executed_order e;
                    e.buyer = b.broker;
                    e.seller = s.broker;
                    e.stonk = b.stonk;
                    e.price = b.price;
                    e.quantity = min(b.quantity, s.quantity);

                    if (e.quantity == 0) continue;
                    executed.push_back(e);

                    all_orders[i].quantity -= e.quantity;
                    buy_orders[j].quantity -= e.quantity;
                    sell_orders[indices[i]].quantity -= e.quantity;

                    print_executed_order(e);
                    if(s.quantity == 0) break;
                }
            }
        }
    }
}
