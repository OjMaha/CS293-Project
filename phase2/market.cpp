#include "market.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <queue>
#include "pq_buy.cpp"
#include "pq_sell.cpp"
using namespace std;


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
    cout<< e.buyer<<" purchased "<< e.quantity << " share of "<< e.stonk << " from "<< e.seller << " for $"<< e.price << "/share"<< endl;
}

void print_matches(vector<executed_order> matches){
    for(int i = 0; i<matches.size(); i++){
        print_executed_order(matches[i]);
    }
}

void print_all_orders(vector<order> orders){
    for(int i = 0; i<orders.size(); i++){
        print_order(orders[i]);
    }
}
void print_buy_pq(priority_queue<order, vector<order>, buy_order_compare> pq){
    while(pq.size() > 0){
        print_order(pq.top());
        pq.pop();
    }
}

void print_sell_pq(priority_queue<order, vector<order>, sell_order_compare> pq){
    while(pq.size() > 0){
        print_order(pq.top());
        pq.pop();
    }
}


market::market(int argc, char** argv)
{       
    // Open a file for reading
    std::ifstream inputFile("output.txt");
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
        
        int sz = tokens.size();
        int start_time = stoi(tokens[0]);
        string broker = tokens[1];
        if(tokens[2] == "BUY") tokens[2] = "1";
        else tokens[2] = "-1";
        int bs = stoi(tokens[2]);
        int price = stoi(tokens[sz-3].substr(1)); //remove $ sign
        int quantity = stoi(tokens[sz-2].substr(1)); //remove # sign
        int expires_after = stoi(tokens[sz-1]);
        //process lincomb as a string
        string stonk;
        for(int i = 3; i<sz-3; i++){
            stonk += (tokens[i] + " ");
        }   

        std::string stock_data_string = stonk;

        std::map<std::string, int> stock_map;

        std::istringstream iss(stock_data_string);

        string token_stonk;
        vector<string> tokens_stonk;

        // Read stock name and quantity pairs from the stringstream waayon
        while (iss >> token_stonk) {
            tokens_stonk.push_back(token_stonk);
        }

        // Process the stock name and quantity pairs
        for (int i = 0; i < tokens_stonk.size(); i++) {
            string stock_name = tokens_stonk[i];

            if(i+1 == tokens_stonk.size()){    //if last token
                stock_map[stock_name] = 1;      //default quantity is 1
                break;
            }

            if(tokens_stonk[i+1][0] - '0' <=9 && tokens_stonk[i+1][0] - '0' >=0){   //if next token is a number
                int stock_quantity = stoi(tokens_stonk[i + 1]);
                stock_map[stock_name] = stock_quantity;
                i++;
            }

            else{
                stock_map[stock_name] = 1;      //default quantity is 1
            }
        }

        stonk = "";
        for(auto it = stock_map.begin(); it != stock_map.end(); it++){
            stonk += (it->first + " ");
            if(it->second != 1){
                stonk += (to_string(it->second) + " ");
            }
        }

        stonk.pop_back();   //remove last space

        order o;            //initialise order      

        o.issue = start_time;
        if(expires_after >= 0){
            o.expiry = start_time + expires_after;
        }
        else{
            o.expiry = -1;      /// -1 for infinite expiry
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

    // for(int i = 0; i<all_orders.size(); i++){
    //     print_order(all_orders[i]);
    // }
    // cout<<endl;
    // for(int i = 0; i<buy_orders.size(); i++){
    //     print_order(buy_orders[i]);
    // }
    // cout<<endl;
    // for(int i = 0; i<sell_orders.size(); i++){
    //     print_order(sell_orders[i]);
    // }
    

}

///note thaat buy_orders, sell_orders and all_orders are sorted by time

void market::start()
{  
    vector<executed_order> executed;
    map<string, priority_queue<order, vector<order>, buy_order_compare>> buy_map; //stonk name, priority queue of buy orders
    map<string, priority_queue<order, vector <order>, sell_order_compare>> sell_map; //stonk name, priority queue of sell orders

    for(int i=0;i<all_orders.size();i++){
        if(all_orders[i].bs == 1){          //buy order hai

            if(sell_map.find(all_orders[i].stonk) == sell_map.end()){   //if no sell orders for this stonk to match with
                buy_map[all_orders[i].stonk].push(all_orders[i]);
            }
        
            else{//if there are sell orders for this stonk

                while(sell_map[all_orders[i].stonk].size() > 0 && 
                (sell_map[all_orders[i].stonk].top().expiry < all_orders[i].issue && sell_map[all_orders[i].stonk].top().expiry != -1)){
                    sell_map[all_orders[i].stonk].pop();  //remove all expired top sell orders
                    
                }

                while(sell_map[all_orders[i].stonk].size() > 0 && sell_map[all_orders[i].stonk].top().price <= all_orders[i].price){    //matching hai
                    order s = sell_map[all_orders[i].stonk].top();
                    int qty = min(all_orders[i].quantity, s.quantity);
                    all_orders[i].quantity -= qty;
                    s.quantity -= qty;

                    if(s.quantity == 0) {
                        sell_map[all_orders[i].stonk].pop();    //if sell order is completely executed, remove it
                        while(sell_map[all_orders[i].stonk].size() > 0 && 
                            (sell_map[all_orders[i].stonk].top().expiry < all_orders[i].issue && sell_map[all_orders[i].stonk].top().expiry != -1)){
                                sell_map[all_orders[i].stonk].pop();  //make sure the order at top of queue is not expired
                    
                        }
                    }
                    //move on to next sell order
                    else {
                        sell_map[all_orders[i].stonk].pop();    //else update the quantity of the sell order (essentiallybuy order fully executed)
                        if(s.quantity > 0) sell_map[all_orders[i].stonk].push(s); //if sell order is not completely executed, push it back into the queue
                    }
                    buy_orders[indices[i]].quantity -= qty;
                
                    executed_order final_order;
                    final_order.buyer = all_orders[i].broker;
                    final_order.seller = s.broker;
                    final_order.stonk = s.stonk;
                    final_order.price = s.price;
                    final_order.quantity = qty;

                    if(final_order.quantity != 0) {
                        print_executed_order(final_order);
                        executed.push_back(final_order);
                    }         

                    if(all_orders[i].quantity == 0) break;  //if buy order is completely executed, move on to next buy order

                }

                if(all_orders[i].quantity > 0){ //if buy order is not completely executed, push it into the queue
                    buy_map[all_orders[i].stonk].push(all_orders[i]);
                }
            }


        }

        else{                 //sell order hai
            if(buy_map.find(all_orders[i].stonk) == buy_map.end()){
                    sell_map[all_orders[i].stonk].push(all_orders[i]);
                }

            else{

                while(buy_map[all_orders[i].stonk].size() > 0 &&
                (buy_map[all_orders[i].stonk].top().expiry < all_orders[i].issue && buy_map[all_orders[i].stonk].top().expiry != -1)){
                    buy_map[all_orders[i].stonk].pop();  //remove all expired buy orders
                }

                while(buy_map[all_orders[i].stonk].size() > 0 && buy_map[all_orders[i].stonk].top().price >= all_orders[i].price){
                    order b = buy_map[all_orders[i].stonk].top();
                    int qty = min(all_orders[i].quantity, b.quantity);
                    all_orders[i].quantity -= qty;
                    b.quantity -= qty;
                    sell_orders[indices[i]].quantity -= qty;

                    if(b.quantity == 0) {
                        buy_map[all_orders[i].stonk].pop(); //if buy order is completely executed, remove it
                        while(buy_map[all_orders[i].stonk].size() > 0 && 
                            (buy_map[all_orders[i].stonk].top().expiry < all_orders[i].issue && buy_map[all_orders[i].stonk].top().expiry != -1)){
                                buy_map[all_orders[i].stonk].pop();  //make sure the order at top of queue is not expired
                    
                        }
                    }
                    //move on to next buy order

                    else {
                        buy_map[all_orders[i].stonk].pop();    //else update the quantity of the buy order (essentially sell order fully executed)
                        if(b.quantity > 0) buy_map[all_orders[i].stonk].push(b); //if buy order is not completely executed, push it back into the queue
                    }
                
                    executed_order final_order;
                    final_order.buyer = b.broker;
                    final_order.seller = all_orders[i].broker;
                    final_order.stonk = b.stonk;
                    final_order.price = b.price;
                    final_order.quantity = qty;

                    if(final_order.quantity != 0) {
                        print_executed_order(final_order);
                        executed.push_back(final_order);
                    }

                    if(all_orders[i].quantity == 0) break;  //if sell order is completely executed, move on to next sell order

                }

                if(all_orders[i].quantity > 0){ //partial sell order execution
                    sell_map[all_orders[i].stonk].push(all_orders[i]);
                }
            }
            
        }
    }   

    //NOW PROCESS END OF DAY OUTPUT hehe
    int total_money_transferred = 0;
    int num_shares_traded = 0;
    map<string, vector<int>> broker_stats;  //broker name, [num shares bought, num shares sold, total money]
    for(int i = 0; i<executed.size(); i++){
        total_money_transferred += executed[i].price * executed[i].quantity;
        num_shares_traded += executed[i].quantity;
        
        if(broker_stats.find(executed[i].buyer) == broker_stats.end()){
            broker_stats[executed[i].buyer] = {executed[i].quantity, 0, -1*executed[i].price*executed[i].quantity};
        }
        else{
            broker_stats[executed[i].buyer][0] += executed[i].quantity;
            broker_stats[executed[i].buyer][2] -= executed[i].price*executed[i].quantity;
        }

        if(broker_stats.find(executed[i].seller) == broker_stats.end()){
            broker_stats[executed[i].seller] = {0, executed[i].quantity, executed[i].price*executed[i].quantity};
        }
        else{
            broker_stats[executed[i].seller][1] += executed[i].quantity;
            broker_stats[executed[i].seller][2] += executed[i].price*executed[i].quantity;
        }
    }

    cout<<endl;
    
    cout<<"---End of Day---"<<endl;
    cout<<"Total Amount of Money Transferred: $"<<total_money_transferred<<endl;
    cout<<"Number of Completed Trades: "<<executed.size()<<endl;
    cout<<"Number of Shares Traded: "<<num_shares_traded<<endl;
    
    for(auto it = broker_stats.begin(); it != broker_stats.end(); it++){
        cout<<it->first<<" bought "<<it->second[0]<<" and sold "<<it->second[1]<<" for a net transfer of $"<<it->second[2]<<endl;
    }
}

