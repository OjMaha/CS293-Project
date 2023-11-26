#include "receiver.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../headers/map_new.cpp"
#include "../headers/string_extra.cpp"
#include "../headers/bst.cpp"

typedef long long ll;
using namespace std;

struct Order{
    //map<std::string, int> stonks;
    CustomMap<std::string, int> stonks2;
    int price = 0;
    char bs = 0;             //'b' for buy, 's' for sell
    bool validity = true; //true if order is valid, false if order is invalid
    vector <string> stonklist; //maintain a list of stocks involved in an order
    string order_string = ""; //maintain a string of order for easy comparison
};

struct Cancellations{
    int price = 0;
    int order_index = -1;
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
// void print_all_orders(vector <Order> &orderlist){
//     for(int i = 0; i<orderlist.size(); i++){
//         cout<<"order"<<i<<endl;
//         cout << orderlist[i].price << " " << orderlist[i].bs << " ";
//         for(auto it = orderlist[i].stonks.begin(); it != orderlist[i].stonks.end(); it++){
//             cout << it->first << " " << it->second << " ";
//         }
//         cout << endl;
//     }
// }

void print_all_possibilities(vector <string> &possibilities){
    for(int i = 0; i<possibilities.size(); i++){
        cout<<possibilities[i]<<endl;
    }
}

bool check_cancellations(vector <Order> &orderlist, Order &o, CustomMap<std::string, Cancellations> &buyMap, CustomMap<std::string, Cancellations> &sellMap, int n){
    if(o.bs == 'b'){ // buy order
        if(!buyMap.contains(o.order_string)){
            buyMap.insert(o.order_string, {o.price, n});
        }
        else if(o.price <= buyMap.get(o.order_string).price){ //price <= best rejected buy price
            o.validity = false;
            cout << "No trade" << endl;
            return true;
        }
        else{
            int i = buyMap.get(o.order_string).order_index;
            buyMap.set(o.order_string, {o.price, n});        //update best rejected buy price
            orderlist[i].price = o.price;                    //update best rejected buy price in orderlist
            return false;
        }
    }
    else{   //sell order, for sell order prices are negative, remember
        if(!sellMap.contains(o.order_string)){
            sellMap.insert(o.order_string, {o.price, n});
        }
        else if(o.price <= sellMap.get(o.order_string).price){ //price >= best rejected sell price
            o.validity = false;
            cout << "No trade" << endl;
            return true;
        }
        else{
            int i = sellMap.get(o.order_string).order_index;
            sellMap.set(o.order_string, {o.price, n});        //update best rejected sell price
            orderlist[i].price = o.price;                    //update best rejected sell price in orderlist
            return false;
        }
    }


    //now let's check for exact matches

    if(o.bs == 'b' && o.price == (-1 * sellMap.get(o.order_string).price)){  // exact match found
        cout << "No Trade" << endl;
        sellMap.remove(o.order_string);
        buyMap.remove(o.order_string);
        return true;
    }
    else if(o.bs == 's' && (-1*o.price) == buyMap.get(o.order_string).price){  // exact match found
        cout << "No Trade" << endl;
        sellMap.remove(o.order_string);
        buyMap.remove(o.order_string);
        return true;
    }

    orderlist.push_back(o); //order is valid, add it to orderlist

    //no more cancellation criteria, ig?
    return false;

}

int main(){
    vector <string> possibilities;
    generate_string(possibilities, 0);
    vector <Order> orderlist;
    int total_profit = 0;

    CustomMap<std::string, Cancellations> buyMap;    //gonna store stock name and best (highest) rejected buy price 
    CustomMap<std::string, Cancellations> sellMap;   //gonna store stock name and best (lowest) rejected sell price 

    Receiver rcv;
    string firsthalfline;
    bool broken = false;

    

    
    while(true){
        sleep(5);
        std::string message = rcv.readIML();
    
        // Create a stringstream from the input string
        std::istringstream ss(message);
        std::string eachline;
        
        

        // Split the string using the delimiter '#'
        while (std::getline(ss, eachline, '#')) {
            //cout << "Meow says " << eachline << endl;
            
            
            if(broken){
                eachline = firsthalfline + eachline; //concatenate the broken line with the next line
                broken = false;
                //cout << "concatenated line" << eachline << endl;
            }
            eachline = strip(eachline);
            //cout << "eachline: " << eachline << endl;

            if(eachline == "$"){
                cout<<total_profit<<endl;
                break;
            } 
            if(eachline == "") continue;

            char bs1 = eachline[eachline.size()-1];
            if(bs1 != 'b' && bs1 != 's'){             //broken line; must continue
                broken = true;
                firsthalfline = eachline;
                //cout << "broken line" << firsthalfline << endl;
                break;
            }

            //cout << i++ << "  " << eachline << endl;
            string temp = eachline;
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
                // o.stonks[tokens[j]]= (stoi(tokens[j+1]) * (o.bs == 'b' ? 1 : -1));// * (orderlist[j].bs == 'b' ? 1 : -1));
                o.stonks2.insert(tokens[j], stoi(tokens[j+1]) * (o.bs == 'b' ? 1 : -1));
                o.stonklist.push_back(tokens[j]); 
            }

            sort(o.stonklist.begin(), o.stonklist.end());
            for(int j = 0; j<o.stonklist.size(); j++){  //make a string of stonks for easy comparison.
                // only contains stockname quantity stockname quantity
                // o.order_string += o.stonklist[j] + " " + to_string(o.stonks[o.stonklist[j]]) + " ";
                o.order_string += o.stonklist[j] + " " + to_string(o.stonks2.get(o.stonklist[j])) + " ";
            }
            o.order_string = strip(o.order_string);

            //cout << "Yet Another trade " << o.order_string << endl;

            //Created an order
            //Let's check for cancellations

            //bool cancelled = false;
            bool cancelled = check_cancellations(orderlist, o, buyMap, sellMap, orderlist.size());
            //cout << "Checked for cancellations" << endl;
            if(cancelled) continue;

            //Order hasnt been cancelled; let's check for arbitrage
            //orderlist.push_back(o);
            

            //now arbitrage checker
            int n = orderlist.size();
            generate_string(possibilities, n);      //generate all possible combinations of orders

            //cout<<"PEHLE ";print_all_possibilities(possibilities);
            
            ll m = possibilities.size();

            

            int profit = 0;
            int possibility_number = -1;

            //cout << "Checking for arbitrage" << endl;

            for(int i = 0; i<m/2; i++){
                CustomMap<string, int> considered_stonks;     //maintain a list of stonks considered for arbitrage with qty
                int temp_profit = 0;
                bool flag = true;      //arbitrage checker

                //considering the ith possibility
                for(int j = 0; j<n; j++){
                    if(possibilities[i][j] == '1'){
                        //considering the jth order
                        // for(auto it = orderlist[j].stonks.begin(); it != orderlist[j].stonks.end(); it++){
                        //     //considering the kth stonk
                        //     if(considered_stonks.find(it->first) == considered_stonks.end()){
                        //         //if stonk not found in considered_stonks
                        //         considered_stonks[it->first] = it->second;
                        //     }
                        //     else{
                        //         //if stonk found in considered_stonks
                        //         considered_stonks[it->first] += it->second;
                        //     }
                        // }

                        vector<string> keys_to_iterate = orderlist[j].stonks2.getAllKeys();
                        for(int k = 0; k<keys_to_iterate.size(); k++){
                            //considering the kth stonk
                            if(considered_stonks.contains(keys_to_iterate[k])){ //if stonk found in considered_stonks
                                considered_stonks.set(keys_to_iterate[k], considered_stonks.get(keys_to_iterate[k]) + orderlist[j].stonks2.get(keys_to_iterate[k]));
                            }
                            else{  //if stonk not found in considered_stonks
                                considered_stonks.insert(keys_to_iterate[k], orderlist[j].stonks2.get(keys_to_iterate[k]));
                            }
                        }

                        temp_profit += orderlist[j].price;
                    }
                }

                //check if total qty of all stonks become zero

                // for(auto it = considered_stonks.begin(); it != considered_stonks.end(); it++){
                //     if(it->second != 0){
                //         flag = false;
                //         break;
                //     }
                // }

                vector <string> keys_for_considered_stonks = considered_stonks.getAllKeys();
                for(int j = 0; j<keys_for_considered_stonks.size(); j++){
                    if(considered_stonks.get(keys_for_considered_stonks[j]) != 0){
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
                for(int j = 0; j<n; j++){
                    if(possibilities[possibility_number][j] == '1'){
                        order_indices.push_back(j);
                    }
                }

                

                for(int j = n-1; j>=0; j--){        //reverse orderprint
                    if(possibilities[possibility_number][j] == '1'){
                        int multiplier = 1;
                        if(orderlist[j].bs == 's') multiplier = -1;
                        // for(auto it = orderlist[j].stonks.begin(); it != orderlist[j].stonks.end(); it++){
                        //         cout << it->first  << " " << it->second * multiplier << " ";
                        // }
                        vector<string> keys_to_iterate_print = orderlist[j].stonks2.getAllKeys();
                        for(int k = 0; k<keys_to_iterate_print.size(); k++){
                            cout << keys_to_iterate_print[k]  << " " << orderlist[j].stonks2.get(keys_to_iterate_print[k]) * multiplier << " ";
                        }

                        cout << orderlist[j].price << " ";
                        if(orderlist[j].bs == 'b') cout << "s";
                        else cout << "b";
                        cout << endl;
                    }

                }

                for(int k = 0; k<order_indices.size(); k++){       //never to be considered again
                    for(int l = 0; l<possibilities.size(); l++) possibilities[l][order_indices[k]] = '0';
                    Order ord = orderlist[order_indices[k]];

                    //if the order getting executed was the best order, delete that best order from database
                    if(ord.bs == 'b' && buyMap.get(ord.order_string).order_index == order_indices[k]) 
                        buyMap.remove(ord.order_string);
                    else if(ord.bs == 's' && sellMap.get(ord.order_string).order_index == order_indices[k]) 
                        buyMap.remove(ord.order_string);
                }

                    total_profit += profit;
            }

            else cout <<"No trade"<<endl;
            // cout<<"baadme: "; print_all_possibilities (possibilities);

        }
            
        if(eachline == "$"){
            //cout << "I'm Done here bro" << endl;            
            break;
        }
        
    }
        
        

}



//check if any order is getting cancelled
                /*for(int i = 0; i<order_indices.size()-1; i++){
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
                }*/
                //it makes canceled orders 0 in all possibilities