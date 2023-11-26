/*#include "receiver.h"
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

bool check_cancellations(vector <Order> &bigorderlist, vector <Order> &orderlist, Order &o, vector <vector <int>> &possibilities){
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
                            possibilities[l][0] = -1;        //set all higher values to 0
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
                                possibilities[l][0] = -1;        //set all higher values to -1
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
    if(o.quantity <= 100){
        orderlist.push_back(o);
        int n = orderlist.size();
        int qu = o.quantity;
        generate_string(possibilities, n, qu);
    }
    else{
        bigorderlist.pushback(o);
    }
    
    return false;
}






int main(){
    vector <vector<int>> possibilities;
    generate_string(possibilities, 0, 0);
    vector <Order> orderlist;
    vector <Order> bigorderlist;
    int total_profit = 0;
    bool broken = false;
    string firsthalfline = "";
    
    Receiver rcv;
    
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

            //TK: I'm changing the splitting function as this is more compact and scalable.
            string temp = eachline;
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
                o.stonks[tokens[j]]= (stoi(tokens[j+1]) * (o.bs == 'b' ? 1 : -1));// * (orderlist[j].bs == 'b' ? 1 : -1));
                o.stonklist.push_back(tokens[j]); 
            }

            sort(o.stonklist.begin(), o.stonklist.end());
            for(int j = 0; j<o.stonklist.size(); j++){
                o.order_string += o.stonklist[j] + " " + to_string(o.stonks[o.stonklist[j]]) + " ";
            }
            o.order_string += tokens[size-3];
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
                if(possibilities[i][0] == -1) continue; //if first element is -1, then it is not a valid possibility
                map<string, int> considered_stonks;     //maintain a list of stonks considered for arbitrage with qty
                int temp_profit = 0;
                bool flag = true;      //arbitrage checker

                //considering the ith possibility
                for(ll j = 0; j < n; j++){
                    if(possibilities[i][j] > 0){
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
                int r = 0;
                int index = -1;
                if(flag == false){ //not a perfect match
                    flag = true;
                    
                    for(int i = 0; i<bigorderlist.size(); i++){
                        Order o1 = bigorderlist[i];
                       
                        index = i;
                        for(auto it = considered_stonks.begin(); it != considered_stonks.end(); it++){
                            if(it->second == 0 && o1.stonks[it->first] == 0) continue;
                            if(it->second == 0 && o1.stonks[it->first] != 0) {
                                flag = false;
                                break;
                            }
                            int rem = o1.stonks[it->first] % it->second;
                            if(rem != 0){
                                flag = false;
                                break;
                            }
                            if(r == 0){
                                r = o1.stonks[it->first] / it->second;
                                if(r <= 0){
                                    flag = false;
                                    break;
                                }
                            }
                                
                            else if(r != o1.stonks[it->first] / it->second){
                                flag = false;
                                break;
                            }
                            
                        }
                    }
                    

                    

                }

                if(flag && r){
                    temp_profit += bigorderlist[index].price * r;
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
                int bigorder_indices;
                vector <int> bigorder_quantities;
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
                    
 /*                   cout << orderlist[order_indices[j]].order_string << " ";
                    //cout << orderlist[order_indices[j]].price << " ";
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
                            possibilities[l][0] = -1;                    //make it -1 if less than quantity
                    }     
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
    //cout<<total_profit<<endl;
    return 0;
}*/




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







#include "receiver.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../headers/map.cpp"
#include "../headers/string_extra.cpp"
#include "../headers/bst.cpp"
using namespace std;

struct Order{
    CustomMap<std::string, int> stonks;
    int price = 0;
    char bs = 0;             //'b' for buy, 's' for sell
    bool validity = true; //true if order is valid, false if order is invalid
    vector <string> stonklist; //maintain a list of stocks involved in an order
};

struct Combo{
    vector <int> comboOfOrders;
    int price = 0;
    bool possible = false;
};

//orderlist = our entire orderlist, i = evaluate uptil i orders, UpTillNow = summation of n to i+1 orders
// with their various possibilities, includedinorder is a list of indices of all the orders involved.

Combo CyclePossible(vector <Order> orderlist, int i, Order upTillNowOg, vector <int> includedinorderOg){
    Order upTillNow = upTillNowOg;
    vector <int> includedinorder = includedinorderOg;
    Combo ans1;
    cout << "hello1" << endl;
    bool cycle = true;
    for(int k = 0; k<upTillNow.stonklist.size(); k++){
        if(upTillNow.stonks.get(upTillNow.stonklist[k]) != 0){
            cycle = false;   //sum isnt yet 0
            //cout << "No cycle yet\n";
            break;
        }
    }
    cout << "hello2" << endl;
    // if(upTillNow.stonklist.size() == 3){
    //     cout << "Cycle Where?\n";
    //     cout << cycle << endl;
    //     cout << upTillNow.price << endl;
    //     cout << "Cycle Where??\n";
    // }
    if(cycle == true && upTillNow.stonklist.size() > 0 && upTillNow.price > 0){ // a cycle has been found
        ans1.possible = true;
        ans1.price = upTillNow.price;
        ans1.comboOfOrders = includedinorder;
        //cout << "A cycle has been found!! \n";
        return ans1;
    }
    cout << "hello3" << endl;
    //base cases of the recursion
    if(i < 0){
        // cout << "Reached the end; 1 out of 2^n possibilities evaluated and rejected\n";
        // for(int j = 0; j < includedinorder.size(); j++){
        //     cout << includedinorder[j] << "  ";
        // }
        // cout << "\n";
        return ans1; //by default, possible value is False;
    }
         
    


    //onto the main function
    
    cout << "hello4\n";


    ans1 = CyclePossible(orderlist, i-1, upTillNow, includedinorder); //can a cycle exist without this order?

    upTillNow = upTillNowOg;
    includedinorder = includedinorderOg;
    cout << "hello5\n";
    
    // add this stock to the upTillNow sum

    for(int k = 0; k < orderlist[i].stonklist.size(); k++){

        cout << "loopy1\n";
        cout << "Meow wants size  " << orderlist[i].stonklist.size() << endl;
        cout << "loopy1.4\n";
        std::string s = orderlist[i].stonklist[k];
        cout << "loopy1.5\n";
        int q = upTillNow.stonks.get(orderlist[i].stonklist[k]);       // q = stocks of that kind already in upTillNow
        cout << "loopy2\n";
        //a crude check whether this stock exists in upTillNow or not; can't be bothered with sorting and binary searching lmao
        if(q == 0){
            upTillNow.stonklist.push_back(orderlist[i].stonklist[k]);
            cout << "loopy3\n";
        }
        cout << "loopy4\n";

        q += orderlist[i].stonks.get(orderlist[i].stonklist[k]);        // q += stocks of a kind in the ith order
        cout << "loopy5\n";
        upTillNow.stonks.set(orderlist[i].stonklist[k], q);          // set this new value and update upTillNow
        cout << "loopy6\n";
    }
    cout << "hello6\n";
    upTillNow.price += orderlist[i].price;

    //WE NEED TO SOMEHOW CHECK IF ALL VALUES IN UPTILLNOW ARE 0. FOR THAT WE NEED THE STONKLIST VECTOR, BUT HOW TO INSERT IN IT EVERYTIME?
    //HOW DO I CHECK WHETHER THE STRING IS ALREADY INSIDE THE VECTOR OR NOT? OR SHOULD I RECKLESSLY INSERT EVERYTIME?? MEOW.
    // ALL I HAVE IS A CRUDE CHECK; SO SEE IF YOU CAN FIGURE OUT A BETTER WAY

    includedinorder.push_back(i); //add this stock to includedinorder


    

        
    cout << "hello7\n";
    Combo ans2 = CyclePossible(orderlist, i-1, upTillNow, includedinorder); //can a cycle exist with this order?
    cout << "hello8\n";
    if(ans1.possible && ans1.price > 0 && ans2.possible && ans2.price > 0) { //both possible? return the one with max price
        if(ans1.price > ans2.price){
            return ans1;
        }
        else
            return ans2;
    }
    cout << "hello9\n";
    if(ans1.possible && ans1.price > 0)
        return ans1;
    else
        return ans2;

    // we'll have to check the value of ans.possible everytime we check for a cycle
}


int main() {
    vector <Order> orderlist;
    //Receiver rcv;
    //sleep(5);
    std::string message = "X 1 Y -1 10 b#Z -1 Y 1 -15 b#Z 1 X -1 10 b#";
    
    

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
        cout << temp << endl;
        

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
        //cout << size << "   meow \n";
        if(tokens[size-1] == "b") o.bs = 'b';
        else o.bs = 's';

        o.price = stoi(tokens[size-2]);
        o.price *= o.bs == 'b' ? 1 : -1;   //take everything positive if buy, negative if sell

        //cout << "check2 \n";
        for(int j = 0; j<size-2; j+=2){
            //insert -ve quantities if selling
            o.stonks.insert(tokens[j], (stoi(tokens[j+1]) * (orderlist[j].bs == 'b' ? 1 : -1)));// * (orderlist[j].bs == 'b' ? 1 : -1));
            o.stonklist.push_back(tokens[j]); 
        }
        cout << "check3\n";

        orderlist.push_back(o);

        cout << "check4\n";

        //Arbitrage Check!!

        Order upTillNow;
        vector <int> includedinorder;
        includedinorder.push_back(orderlist.size()-1);
        Combo cycle = CyclePossible(orderlist, orderlist.size() - 1, upTillNow, includedinorder);
        cout << "Check5\n";
        cout << cycle.possible << endl;
        cout << cycle.price << endl;
        //cout << cycle.comboOfOrders[0] << endl;
        if(cycle.possible == false or cycle.price <= 0){
            cout << "No Trade\n";
            continue;
        }
        else{
            cout << "Let's Trade!!\n";
            for(int a = 0; a < cycle.comboOfOrders.size(); a++){
                int j = cycle.comboOfOrders[a]; 
                orderlist[j].validity = false;                       //make the order invalid now hehe
                for(int k = 0; k<orderlist[j].stonklist.size(); k++){                 
                    cout<<orderlist[j].stonklist[k] << " ";
                    cout<<orderlist[j].stonks.get(orderlist[j].stonklist[k]) * (orderlist[j].bs == 'b' ? 1 : -1) << " ";
                }
                cout << orderlist[j].price * (orderlist[j].bs == 'b' ? 1 : -1) << " " << orderlist[j].bs << "#" << endl;
            }
        }

        
    
    }



    //vector <Order> orderlist;
    /*Order o;
    o.stonklist.push_back("X");
    o.stonklist.push_back("Y");
    o.stonks.set("X", 1);
    o.stonks.set("Y", -1);
    o.price = 10;
    o.bs = 'b';
    orderlist.push_back(o);

    Order p;
    p.stonklist.push_back("Z");
    p.stonklist.push_back("Y");
    p.stonks.set("Z", -1);
    p.stonks.set("Y", 1);
    p.price = -15;
    p.bs = 'b';
    orderlist.push_back(p);

    Order q;
    q.stonklist.push_back("X");
    q.stonklist.push_back("Z");
    q.stonks.set("X", -1);
    q.stonks.set("Z", 1);
    q.price = 10;
    q.bs = 'b';
    orderlist.push_back(q);

    Order upTillNow;
    vector <int> includedinorder;

    Combo cycle = CyclePossible(orderlist, 2, upTillNow, includedinorder);
    cout << cycle.price << endl;
    cout << cycle.possible << endl;

    

    if(cycle.possible == false or cycle.price <= 0){
        cout << "No Trade\n";
        //continue;
    }
    else{
        cout << "Let's Trade!!\n";
        for(int a = 0; a < cycle.comboOfOrders.size(); a++){
            int j = cycle.comboOfOrders[a]; 
            orderlist[j].validity = false;                       //make the order invalid now hehe
            for(int k = 0; k<orderlist[j].stonklist.size(); k++){                 
                cout<<orderlist[j].stonklist[k] << " ";
                cout<<orderlist[j].stonks.get(orderlist[j].stonklist[k]) * (orderlist[j].bs == 'b' ? 1 : -1) << " ";
            }
            cout << orderlist[j].price * (orderlist[j].bs == 'b' ? 1 : -1) << " " << orderlist[j].bs << "#" << endl;
        }
    }*/

    


    /*cout << "bruh\n";

    vector <Order> orderlist;

    Receiver rcv;
    sleep(5);
    std::string message = rcv.readIML();
    cout << "bruh2\n";
    

    // Create a stringstream from the input string
    std::istringstream ss(message);

    std::vector<std::string> splitStrings;
    std::string token;

    // Split the string using the delimiter '#'
    while (std::getline(ss, token, '#')) {
        splitStrings.push_back(token);
    }
    cout << "bruh3\n";

    // Now, splitStrings contains the individual substrings
    for (int i = 0; i< splitStrings.size(); i++) {
        int k = 0; //k is counter used to maintain which part of the string is being read (stock, price, b/s)
        std::string temp = splitStrings[i];

        if (temp == "$") break;
        cout << temp << " bhow \n";

        //TK: I'm changing the spllitting function as this is more compact and scalable.

        std::istringstream iss(temp);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(iss, token, ' ')) {
            token = strip(token);
            tokens.push_back(token);
        }
        cout << "bruh4\n";
        Order o;

        cout << "check1\n";
        
        int size = tokens.size();
        if(size <= 1) break;    
        cout << size << "   meow \n";
        if(tokens[size-1] == "b") o.bs = 'b';
        else o.bs = 's';

        o.price = stoi(tokens[size-2]);
        o.price *= o.bs == 'b' ? 1 : -1;   //take everything positive if buy, negative if sell

        cout << "check2 \n";
        for(int j = 0; j<size-2; j+=2){
            //insert -ve quantities if selling
            o.stonks.insert(tokens[j], stoi(tokens[j+1]));// * (orderlist[j].bs == 'b' ? 1 : -1));
            o.stonklist.push_back(tokens[j]); 
        }
        cout << "check3\n";

        orderlist.push_back(o);

        //Arbitrage Check!!

        Order upTillNow;
        vector <int> includedinorder;
        Combo cycle = CyclePossible(orderlist, orderlist.size(), upTillNow, includedinorder);
        cout << cycle.possible << endl;
        cout << cycle.price << endl;
        //cout << cycle.comboOfOrders[0] << endl;
        if(cycle.possible == false or cycle.price <= 0){
            cout << "No Trade\n";
            continue;
        }
        else{
            cout << "Let's Trade!!\n";
            for(int a = 0; a < cycle.comboOfOrders.size(); a++){
                int j = cycle.comboOfOrders[a]; 
                orderlist[j].validity = false;                       //make the order invalid now hehe
                for(int k = 0; k<orderlist[j].stonklist.size(); k++){                 
                    cout<<orderlist[j].stonklist[k] << " ";
                    cout<<orderlist[j].stonks.get(orderlist[j].stonklist[k]) * (orderlist[j].bs == 'b' ? 1 : -1) << " ";
                }
                cout << orderlist[j].price * (orderlist[j].bs == 'b' ? 1 : -1) << " " << orderlist[j].bs << "#" << endl;
            }
        }

        
    
    }*/
    
}


// for(int j = 0; j<orderlist.size(); j++){
        //     //make everything back to input value by again multiplying by +-1 depending on b/s
        //     for(int k = 0; k<orderlist[j].stonklist.size(); k++){
        //         cout<<orderlist[j].stonklist[k] << " ";
        //         cout<<orderlist[j].stonks.get(orderlist[j].stonklist[k]) * (orderlist[j].bs == 'b' ? 1 : -1) << " ";
        //     }
        //     cout << orderlist[j].price * (orderlist[j].bs == 'b' ? 1 : -1) << " " << orderlist[j].bs << "#" << endl;
        // }
        // cout << "Check4\n";