// #include "receiver.h"
// #include <iostream>
// #include <std::string>
// #include <vector>
// #include <sstream>
// #include "../headers/map.cpp"
// #include "../headers/std::string_extra.cpp"
// #include "../headers/bst.cpp"
// #include <map>

// typedef long long ll;
// using namespace std;

namespace Part3 {
struct Order{
    // map<std::string, int> stonks;
    CustomMap2<std::string, int> stonks2;
    int price = 0;
    char bs = 0;             //'b' for buy, 's' for sell
    int quantity = 0; //quantity of order remaining
    vector <string> stonklist; //maintain a list of stocks involved in an order
    string order_string = "";
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

void print_all_possibilities(vector <vector<int>> &possibilities){
    for(int i = 0; i<possibilities.size(); i++){
        for(int j = 0; j<possibilities[i].size(); j++){
            cout<<possibilities[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

bool check_cancellations(vector <Order> &orderlist, Order &o, vector <vector <int>> &possibilities){
    //cout << "Hello"<<endl;
    for(int i = 0; i < orderlist.size(); i++){
        if(orderlist[i].order_string == o.order_string){
            if(orderlist[i].bs == o.bs){       //combine 2 orders if both buy or both sell
                int q = orderlist[i].quantity; //old q
                orderlist[i].quantity += o.quantity;  //new q
                //cout << "Combine!!"<<endl;  
                int s = possibilities.size();
                for(int j = 0; j < s; j++){   //for all previous possibilities
                    if(possibilities[j][i] == q){
                        for(int k = q+1; k <= orderlist[i].quantity; k++){ //copy all q possibilities and add new possibilities
                            vector <int> a = possibilities[j];             
                            a[i] = k;                                      
                            possibilities.push_back(a);
                        }
                    }
                }
                //parse through possibiliities to make new possibilities
            }
            else{                              //cancel the orders if one buy and one sell
                if(orderlist[i].quantity >= o.quantity){
                    orderlist[i].quantity -= o.quantity;
                    //parse through possibiliities to make new possibilities
                    for(int l = 0; l<possibilities.size(); l++){
                        if(possibilities[l][i] > orderlist[i].quantity) 
                            possibilities[l][i] = -1;        //set all higher values to 0
                    }  
                    //cout << "Cancel and consume!!"<<endl;
                    return true;

                }
                else{
                    int q = orderlist[i].quantity; //old q
                    o.quantity = o.quantity - orderlist[i].quantity; //new q
                    orderlist[i] = o;
                    
                    //parse through possibilities to make new possibilities
                    if(q >= o.quantity){                                //possibilities already exist, just remove the higher values
                        for(int l = 0; l<possibilities.size(); l++){
                            if(possibilities[l][i] > orderlist[i].quantity) 
                                possibilities[l][i] = -1;        //set all higher values to -1
                        }  
                    }
                    else{                                               //possibilities don't exist, add new strings
                        for(int l = 0; l<possibilities.size(); l++){
                            if(possibilities[l][i] == q){
                                for(int k = q+1; k <= orderlist[i].quantity; k++){ //copy all q possibilities and add new possibilities
                                    vector <int> a = possibilities[l];             
                                    a[i] = k;                                      
                                    possibilities.push_back(a);
                                }
                            }
                        }  
                        
                    }
                }
                //cout << "Cancel!!"<<endl;
            }
            
            return false;
        }

    }
    //cout << "New order!!"<<endl;
    orderlist.push_back(o);
    int n = orderlist.size();
    int qu = o.quantity;
    generate_string(possibilities, n, qu);
    return false;
}






void part3(){
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
    for (int i = 0; i < splitStrings.size(); i++) {
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
        
        //o.order_string = temp.substr(0, temp.size() - 4);
        //o.order_string = strip(o.order_string);
        
    
        int size = tokens.size();
        if(size <= 1) break;    
        if(tokens[size-1] == "b") o.bs = 'b';
        else o.bs = 's';
        
        o.quantity = stoi(tokens[size-2]);
        o.price = stoi(tokens[size-3]);
        o.price *= o.bs == 'b' ? 1 : -1;   //take everything positive if buy, negative if sell

        for(int j = 0; j<size-3; j+=2){
            //insert -ve quantities if selling
            // o.stonks[tokens[j]]= (stoi(tokens[j+1]) * (o.bs == 'b' ? 1 : -1));// * (orderlist[j].bs == 'b' ? 1 : -1));
            o.stonks2.insert(tokens[j], (stoi(tokens[j+1]) * (o.bs == 'b' ? 1 : -1)));
            o.stonklist.push_back(tokens[j]); 
        }

        sort(o.stonklist.begin(), o.stonklist.end());
        // for(int j = 0; j<o.stonklist.size(); j++){
        //     o.order_string += o.stonklist[j] + " " + to_string(o.stonks[o.stonklist[j]]) + " ";
        // }
        for(int j = 0; j<o.stonklist.size(); j++){
            o.order_string += o.stonklist[j] + " " + to_string(o.stonks2.get(o.stonklist[j])) + " ";
        }
        o.order_string = strip(o.order_string);

        //check for cancellations or combinations
        bool cancelled = check_cancellations(orderlist, o, possibilities);
        

        //print_all_possibilities(possibilities);
        if(cancelled) continue; //here cancelled means that new order was eaten up by previous orders. So no need to check for arbitrage
        
    
        //orderlist.push_back(o); done in check_cancellations
        
        ll m = possibilities.size();
        int n = orderlist.size();

        

        int profit = 0;
        int possibility_number = -1;

        for(int i = 0; i<m; i++){
            CustomMap2<string, int> considered_stonks;     //maintain a list of stonks considered for arbitrage with qty
            int temp_profit = 0;
            bool flag = true;      //arbitrage checker

            //considering the ith possibility
            for(ll j = 0; j < n; j++){
                if(possibilities[i][j] > 0){
                    int quan = possibilities[i][j];

                    //considering the jth order
                    // for(auto it = orderlist[j].stonks.begin(); it != orderlist[j].stonks.end(); it++){
                    //     //considering the kth stonk
                    //     if(considered_stonks.find(it->first) == considered_stonks.end()){
                    //         //if stonk not found in considered_stonks
                    //         considered_stonks[it->first] = it->second * quan;
                    //     }
                    //     else{
                    //         //if stonk found in considered_stonks
                    //         considered_stonks[it->first] += it->second * quan;
                    //     }
                    // }

                    vector<string> keys_to_iterate = orderlist[j].stonks2.getAllKeys();
                    for(int k = 0; k<keys_to_iterate.size(); k++){
                        //considering the kth stonk
                        if(considered_stonks.contains(keys_to_iterate[k])){ //if stonk found in considered_stonks
                            considered_stonks.set(keys_to_iterate[k], considered_stonks.get(keys_to_iterate[k]) + quan * orderlist[j].stonks2.get(keys_to_iterate[k]));
                        }
                        else{  //if stonk not found in considered_stonks
                            considered_stonks.insert(keys_to_iterate[k], quan * orderlist[j].stonks2.get(keys_to_iterate[k]));
                        }
                    }

                    temp_profit += orderlist[j].price * quan;
                }
            }


            //check if total qty of all stonks become zero

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
            vector <int> order_quantities;
            for(int j = 0; j<n; j++){
                if(possibilities[possibility_number][j] != 0){
                    order_quantities.push_back(possibilities[possibility_number][j]);
                    order_indices.push_back(j);
                }
            }

            for(int j = order_indices.size()-1; j >= 0; j--){                      //reverse print
                /*int multiplier = 1;
                if(orderlist[order_indices[j]].bs == 's') multiplier = -1;
                for(auto it = orderlist[order_indices[j]].stonks.begin(); it != orderlist[order_indices[j]].stonks.end(); it++){
                        cout << it->first  << " " << it->second * multiplier << " ";
                }
                
                cout << orderlist[order_indices[j]].price * multiplier << " ";     //print price
                cout << order_quantities[j] << " ";                               //print quantities used*/
                
                cout << orderlist[order_indices[j]].order_string << " ";
                cout << order_quantities[j] << " ";
                orderlist[order_indices[j]].quantity -= order_quantities[j];      //update quantities
                if(orderlist[order_indices[j]].bs == 'b') cout << "s";
                else cout << "b";
                cout << endl;
            }



            for(int k = 0; k<order_indices.size(); k++){       //never to be considered again
                for(int l = 0; l<possibilities.size(); l++){
                    if(possibilities[l][order_indices[k]] > order_quantities[k]) 
                        possibilities[l][order_indices[k]] -= order_quantities[k]; //decrease all the quantities
                    else
                        possibilities[l][order_indices[k]] = -1;                    //make it -1 if less than quantity
                }     
            }

                total_profit += profit;
        }

        else cout <<"No trade"<<endl;
        // cout<<"baadme: "; print_all_possibilities (possibilities);

    }
    cout<<total_profit<<endl;
    return;

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
                    //std::cout<<"cancekl";                                                          //one buy other sell
                    for(int k = 0; k<possibilities.size(); k++) {
                        possibilities[k][order_indices[j]] = '0';
                        possibilities[k][order_indices[i]] = '0';
                    }
                }

                else flag = false;
            }
        }
        //it makes canceled orders 0 in all possibilities*/