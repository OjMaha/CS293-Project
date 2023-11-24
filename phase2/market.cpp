#include "market.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;


struct executed_order{
    string buyer;
    string seller;
    string stonk;
    int price;
    int quantity;
    int index;
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

    for(int i=0;i<all_orders.size();i++){
        if(all_orders[i].bs == 1){          //buy order hai
            vector <executed_order> possible_matches;
            for(int j = 0; j< sell_orders.size(); j++){
                order b = all_orders[i];
                order s = sell_orders[j];

                if(s.issue > b.issue) break;
                else if(s.issue == b.issue && get_index(all_orders, s) > i) break; //if s order came after b order, then break
                else
                if(valid_orders(b,s) && b.price >= s.price){
            
                    executed_order e;
                    e.buyer = b.broker;
                    e.seller = s.broker;
                    e.stonk = b.stonk;
                    e.price = s.price;
                    e.quantity = min(b.quantity, s.quantity);
                    e.index = j;

                    if (e.quantity == 0) continue;
                    else possible_matches.push_back(e);
                }
            }

            ///sort possible matches by price (ascending), then quantity(descending), then timestamp(ascending), then stonk alphabetically(ascending)
            for(int i = 0; i<possible_matches.size(); i++){
                for(int j = i+1; j<possible_matches.size(); j++){
                    if(possible_matches[i].price > possible_matches[j].price){
                        swap(possible_matches[i], possible_matches[j]);
                    }
                    else if(possible_matches[i].price == possible_matches[j].price){
                        if(possible_matches[i].quantity < possible_matches[j].quantity){
                            swap(possible_matches[i], possible_matches[j]);
                        }
                        else if(possible_matches[i].quantity == possible_matches[j].quantity){
                            if(possible_matches[i].buyer > possible_matches[j].buyer){
                                swap(possible_matches[i], possible_matches[j]);
                            }
                            else if(possible_matches[i].buyer == possible_matches[j].buyer){
                                if(possible_matches[i].seller > possible_matches[j].seller){
                                    swap(possible_matches[i], possible_matches[j]);
                                }
                                else if(possible_matches[i].seller == possible_matches[j].seller){
                                    if(possible_matches[i].stonk > possible_matches[j].stonk){
                                        swap(possible_matches[i], possible_matches[j]);
                                    }
                                } 
                            }
                        }
                    }
                }
            }

            // print_matches(possible_matches);
            // cout<<endl;
            //now execute the matches
            for(int m = 0; m<possible_matches.size(); m++){
                int j = possible_matches[m].index;

                int qty = min(all_orders[i].quantity, possible_matches[m].quantity);
                all_orders[i].quantity -= qty;
                sell_orders[j].quantity -= qty;
                buy_orders[indices[i]].quantity -= qty;
                // cout<<"updatebuy";print_all_orders(buy_orders);cout<<endl;
                // cout<<"updatesell";print_all_orders(sell_orders);cout<<endl;
                // cout<<"updateall";print_all_orders(all_orders);cout<<endl;

                executed_order final_order;
                final_order.buyer = possible_matches[m].buyer;
                final_order.seller = possible_matches[m].seller;
                final_order.stonk = possible_matches[m].stonk;
                final_order.price = possible_matches[m].price;
                final_order.quantity = qty;
                executed.push_back(final_order);
                print_executed_order(final_order);

                if(all_orders[i].quantity == 0) break;  //if buy order is completely executed, move on to next buy order

            }
            
        }

        else{               ///sell order hai
            vector <executed_order> possible_matches;
            for(int j = 0; j< buy_orders.size(); j++){
                order b = buy_orders[j];
                order s = all_orders[i];

                if(b.issue > s.issue) break;
                else if(b.issue == s.issue && get_index(all_orders, b) > i) break; //if b order came after s order, then break
                if(valid_orders(s,b) && b.price >= s.price){
                    
                    executed_order e;
                    e.buyer = b.broker;
                    e.seller = s.broker;
                    e.stonk = b.stonk;
                    e.price = b.price;
                    e.index = j;
                    e.quantity = min(b.quantity, s.quantity);

                    if (e.quantity == 0) continue;

                    else possible_matches.push_back(e);  
                }

            }

            //sort possible matches by price (descending), then quantity(descending), then timestamp(ascending), then stonk alphabetically(ascending)
            for(int i = 0; i<possible_matches.size(); i++){
                for(int j = i+1; j<possible_matches.size(); j++){
                    if(possible_matches[i].price < possible_matches[j].price){
                        swap(possible_matches[i], possible_matches[j]);
                    }
                    else if(possible_matches[i].price == possible_matches[j].price){
                        if(possible_matches[i].quantity < possible_matches[j].quantity){
                            swap(possible_matches[i], possible_matches[j]);
                        }
                        else if(possible_matches[i].quantity == possible_matches[j].quantity){
                            if(possible_matches[i].buyer > possible_matches[j].buyer){
                                swap(possible_matches[i], possible_matches[j]);
                            }
                            else if(possible_matches[i].buyer == possible_matches[j].buyer){
                                if(possible_matches[i].seller > possible_matches[j].seller){
                                    swap(possible_matches[i], possible_matches[j]);
                                }
                                else if(possible_matches[i].seller == possible_matches[j].seller){
                                    if(possible_matches[i].stonk > possible_matches[j].stonk){
                                        swap(possible_matches[i], possible_matches[j]);
                                    }
                                } 
                            }
                        }
                    }
                }
            }

            // print_matches(possible_matches);
            // cout<<endl;

            //now execute the matches
            for(int m = 0; m<possible_matches.size(); m++){
                int j = possible_matches[m].index;

                int qty = min(all_orders[i].quantity, possible_matches[m].quantity);

                all_orders[i].quantity -= qty;
                buy_orders[j].quantity -= qty;
                sell_orders[indices[i]].quantity -= qty;

                executed_order final_order;
                final_order.buyer = possible_matches[m].buyer;
                final_order.seller = possible_matches[m].seller;
                final_order.stonk = possible_matches[m].stonk;
                final_order.price = possible_matches[m].price;
                final_order.quantity = qty;
                executed.push_back(final_order);
                print_executed_order(final_order);

                if(all_orders[i].quantity == 0) break;  ///ifsell order is completely executed, move on to next buy order

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

    // cout<<endl;
    // print_all_orders(all_orders);
    // cout<<endl;
    // print_all_orders(buy_orders);
    // cout<<endl;
    // print_all_orders(sell_orders);
    // cout<<endl;
}
