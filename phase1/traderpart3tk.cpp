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
    int quantity = 0; //quantity of order remaining
    vector <string> stonklist; //maintain a list of stocks involved in an order
};

//document this pwease
void generate_string(vector <vector <int>> &possibilities, int size, int quantity){
    if(size == 0){
        return;
    }
    if(size == 1){
        for(int i = quantity; i>=0; i--){
            vector <int> a;
            a.push_back(i);
            possibilities.push_back(a);
        }
        return;
    }

    if(size == 2){
        int s = possibilities.size();
        for(int i = quantity-1; i>=0; i--){          //for all possible quantities q-1 to 0
            for(int j = 0; j < s; j++){   //for all previous possibilities
                vector <int> a = possibilities[j];
                a.push_back(i);
                possibilities.push_back(a);
            }
        }
        for(int j = 0; j < s; j++){                 //for quantity = q
            possibilities[j].push_back(quantity);
        }
        return;
    }
    else{
        int s = possibilities.size();
        for(int i = quantity-1; i>=0; i--){          //for all possible quantities q-1 to 0
            for(int j = 0; j < s; j++){   //for all previous possibilities
                vector <int> a = possibilities[j];
                a.push_back(i);
                possibilities.push_back(a);
            }
        }
        for(int j = 0; j < s; j++){                 //for quantity = q
            possibilities[j].push_back(quantity);
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

void print_all_possibilities(vector <vector<int>> &possibilities){
    for(int i = 0; i<possibilities.size(); i++){
        for(int j = 0; j<possibilities[i].size(); j++){
            cout<<possibilities[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

int main(){
    vector <vector<int>> possibilities;
    generate_string(possibilities, 0, 0);
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

        o.quantity = stoi(tokens[size-2]);
        o.price = stoi(tokens[size-3]);
        o.price *= o.bs == 'b' ? 1 : -1;   //take everything positive if buy, negative if sell

        for(int j = 0; j<size-3; j+=2){
            //insert -ve quantities if selling
            o.stonks[tokens[j]]= (stoi(tokens[j+1]) * (o.bs == 'b' ? 1 : -1));// * (orderlist[j].bs == 'b' ? 1 : -1));
            o.stonklist.push_back(tokens[j]); 
        }

        sort(o.stonklist.begin(), o.stonklist.end());
    
    orderlist.push_back(o);
    

    //now arbitrage checker
    int n = orderlist.size();
    int q = o.quantity;
    generate_string(possibilities, n, o.quantity);      //generate all possible combinations of orders

    //cout<<"PEHLE ";print_all_possibilities(possibilities);
    
    ll m = possibilities.size();

    

    int profit = 0;
    int possibility_number = -1;

    for(int i = 0; i<m; i++){
        map<string, int> considered_stonks;     //maintain a list of stonks considered for arbitrage with qty
        int temp_profit = 0;
        bool flag = true;      //arbitrage checker

        //considering the ith possibility
        for(ll j = 0; j < (n*q)/q+1; j++){
            if(possibilities[i][j] != 0){
                int quan = possibilities[i][j];

                //considering the jth order
                for(auto it = orderlist[j].stonks.begin(); it != orderlist[j].stonks.end(); it++){
                    //considering the kth stonk
                    if(considered_stonks.find(it->first) == considered_stonks.end()){
                        //if stonk not found in considered_stonks
                        considered_stonks[it->first] = it->second * quan;
                    }
                    else{
                        //if stonk found in considered_stonks
                        considered_stonks[it->first] += it->second * quan;
                    }
                }

                temp_profit += orderlist[j].price * quan;
            }
        }


        //check if total qty of all stonks become zero

        for(auto it = considered_stonks.begin(); it != considered_stonks.end(); it++){
            if(it->second != 0){
                flag = false;
                break;
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
        vector <int> order_quantities;
        for(int j = 0; j<n; j++){
            if(possibilities[possibility_number][j] != 0){
                order_quantities.push_back(possibilities[possibility_number][j]);
                order_indices.push_back(j);
            }
        }

        for(int j = order_indices.size()-1; j >= 0; j--){                      //reverse print
            int multiplier = 1;
            if(orderlist[order_indices[j]].bs == 's') multiplier = -1;
            for(auto it = orderlist[order_indices[j]].stonks.begin(); it != orderlist[order_indices[j]].stonks.end(); it++){
                    cout << it->first  << " " << it->second * multiplier << " ";
            }
            cout << order_quantities[j] << " ";                               //print quantities used
            orderlist[order_indices[j]].quantity -= order_quantities[j];      //update quantities
            cout << orderlist[order_indices[j]].price << " ";
            if(orderlist[order_indices[j]].bs == 'b') cout << "s";
            else cout << "b";
            cout << endl;
        }



        for(int k = 0; k<order_indices.size(); k++){       //never to be considered again
            for(int l = 0; l<possibilities.size(); l++){
                if(possibilities[l][order_indices[k]] > order_quantities[k]) 
                    possibilities[l][order_indices[k]] -= order_quantities[k]; //decrease all the quantities
                else
                    possibilities[l][order_indices[k]] = 0;                    //make it 0 if less than quantity
            }     
        }

            total_profit += profit;
    }

    else cout <<"No trade"<<endl;
    // cout<<"baadme: "; print_all_possibilities (possibilities);

    }
    cout<<total_profit<<endl;
}




/*//check if any order is getting cancelled
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
        //it makes canceled orders 0 in all possibilities*/