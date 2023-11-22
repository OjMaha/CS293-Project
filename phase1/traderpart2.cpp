#include "receiver.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../headers/map.cpp"
#include "../headers/string_extra.cpp"
#include "../headers/bst.cpp"
#include <map>

typedef long long ll;
using namespace std;

struct Order{
    map<std::string, int> stonks;
    int price = 0;
    char bs = 0;             //'b' for buy, 's' for sell
    bool validity = true; //true if order is valid, false if order is invalid
    vector <string> stonklist; //maintain a list of stocks involved in an order
};

//document this pwease
void generate_string(vector <string> &possibilities, int size){
    if(size == 0){
        return;
    }
    if(size == 1){
        possibilities.push_back("1");
        possibilities.push_back("0");
        return;
    }

    if(size == 2){
        possibilities.push_back(possibilities[0] + "0");
        possibilities[0] = possibilities[0] + "1";
        possibilities.push_back(possibilities[1] + "0");
        possibilities[1] = possibilities[1] + "1";
        return;
    }
    else{
        ll total = possibilities.size();
        for(ll i = 0; i< total; i++){
            possibilities.push_back(possibilities[i]+ "0");
            possibilities[i] = possibilities[i] + "1";
        }
    }
}

//debuging functions
void print_all_orders(vector <Order> &orderlist){
    for(int i = 0; i<orderlist.size(); i++){
        cout<<"order"<<i<<endl;
        cout << orderlist[i].price << " " << orderlist[i].bs << " ";
        for(auto it = orderlist[i].stonks.begin(); it != orderlist[i].stonks.end(); it++){
            cout << it->first << " " << it->second << " ";
        }
        cout << endl;
    }
}

void print_all_possibilities(vector <string> &possibilities){
    for(int i = 0; i<possibilities.size(); i++){
        cout<<possibilities[i]<<endl;
    }
}

int main(){
    vector <string> possibilities;
    generate_string(possibilities, 0);
    vector <Order> orderlist;
    int total_profit = 0;

    Receiver rcv;
    
    sleep(5);
    std::string message = rcv.readIML();

    // std::string message = "X 1 Y -1 10 b#Z -1 Y 1 -15 b#Z 1 X -1 10 b#";
    
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

        if (temp == "$" or temp == "") break;        

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
        if(size <= 1) break;    
        if(tokens[size-1] == "b") o.bs = 'b';
        else o.bs = 's';

        o.price = stoi(tokens[size-2]);
        o.price *= o.bs == 'b' ? 1 : -1;   //take everything positive if buy, negative if sell

        for(int j = 0; j<size-2; j+=2){
            //insert -ve quantities if selling
            o.stonks[tokens[j]]= (stoi(tokens[j+1]) * (o.bs == 'b' ? 1 : -1));// * (orderlist[j].bs == 'b' ? 1 : -1));
            o.stonklist.push_back(tokens[j]); 
        }

        sort(o.stonklist.begin(), o.stonklist.end());
    
    orderlist.push_back(o);
    

    //now arbitrage checker
    int n = orderlist.size();
    generate_string(possibilities, n);      //generate all possible combinations of orders

    //cout<<"PEHLE ";print_all_possibilities(possibilities);
    
    int m = possibilities.size();

    

    int profit = 0;
    int possibility_number = -1;

    for(int i = 0; i<m/2; i++){
        map<string, int> considered_stonks;     //maintain a list of stonks considered for arbitrage with qty
        int temp_profit = 0;
        bool flag = true;      //arbitrage checker

        //considering the ith possibility
        for(int j = 0; j<n; j++){
            if(possibilities[i][j] == '1'){
                //considering the jth order
                for(auto it = orderlist[j].stonks.begin(); it != orderlist[j].stonks.end(); it++){
                    //considering the kth stonk
                    if(considered_stonks.find(it->first) == considered_stonks.end()){
                        //if stonk not found in considered_stonks
                        considered_stonks[it->first] = it->second;
                    }
                    else{
                        //if stonk found in considered_stonks
                        considered_stonks[it->first] += it->second;
                    }
                }

                temp_profit += orderlist[j].price;
            }
        }

        //check if total qty of all stonks become zero

        for(auto it = considered_stonks.begin(); it != considered_stonks.end(); it++){
            if(it->second != 0){
                flag = false;
            }
        }
        //cout<<"flag: "<<flag<<endl;


        //now check if profit ho raha hai

        if(flag && temp_profit > profit){
            //arbitrage found
            //check if it is better than other arbitrage possibilities

            profit = temp_profit;
            possibility_number = i;
        }

    }

    //checked all possibilities, now print the best one
    if(possibility_number != -1 && profit > 0){

        //store the order indices in a vector
        vector <int> order_indices;
        for(int j = 0; j<n; j++){
            if(possibilities[possibility_number][j] == '1'){
                order_indices.push_back(j);
            }
        }

        //check if any order is getting cancelled
        for(int i = 0; i<order_indices.size()-1; i++){
            for (int j = i+1; j<order_indices.size(); j++){
                bool flag = false;

                if (orderlist[order_indices[i]].stonklist.size() == orderlist[order_indices[j]].stonklist.size() 
                && std::equal(orderlist[order_indices[i]].stonklist.begin(), orderlist[order_indices[i]].stonklist.end(), 
                orderlist[order_indices[j]].stonklist.begin())) flag = true;

                if(orderlist[order_indices[i]].price == orderlist[order_indices[j]].price *(-1) 
                && flag && orderlist[order_indices[i]].bs != orderlist[order_indices[j]].bs){   //one should be negation of other,
                    //cout<<"cancekl";                                                          //one buy other sell
                    for(int k = 0; k<possibilities.size(); k++) {
                        possibilities[k][order_indices[j]] = '0';
                        possibilities[k][order_indices[i]] = '0';
                    }
                }

                else flag = false;
            }
        }
        //it makes canceled orders 0 in all possibilities

        for(int j = n-1; j>=0; j--){        //reverse orderprint
            if(possibilities[possibility_number][j] == '1'){
                int multiplier = 1;
                if(orderlist[j].bs == 's') multiplier = -1;
                for(auto it = orderlist[j].stonks.begin(); it != orderlist[j].stonks.end(); it++){
                        cout << it->first  << " " << it->second * multiplier << " ";
                }

                cout << orderlist[j].price << " ";
                if(orderlist[j].bs == 'b') cout << "s";
                else cout << "b";
                cout << endl;
            }

        }

        for(int k = 0; k<order_indices.size(); k++){       //never to be considered again
            for(int l = 0; l<possibilities.size(); l++) possibilities[l][order_indices[k]] = '0';       
        }

            total_profit += profit;
    }

    else cout <<"No trade"<<endl;
    // cout<<"baadme: "; print_all_possibilities (possibilities);

    }
    cout<<total_profit<<endl;
}