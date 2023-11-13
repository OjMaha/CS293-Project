#include "receiver.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../headers/map.cpp"
#include "../headers/string_extra.cpp"
using namespace std;

struct Order{
    CustomMap<std::string, int> stonks;
    int price = 0;
    char bs = 0;             //'b' for buy, 's' for sell
    bool validity = true; //true if order is valid, false if order is invalid
    vector <string> stonklist; ///maintain a list of stocks involved in an order
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
    cout << "hello1\n";
    bool cycle = true;
    for(int k = 0; k<upTillNow.stonklist.size(); k++){
        if(upTillNow.stonks.get(upTillNow.stonklist[k]) != 0){
            cycle = false;   //sum isnt yet 0
            //cout << "No cycle yet\n";
            break;
        }
    }
    cout << "hello2\n";
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
    cout << "hello3\n";
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
    if(ans1.possible && ans2.possible) { //both possible? return the one with max price
        if(ans1.price > ans2.price){
            return ans1;
        }
        else
            return ans2;
    }
    cout << "hello9\n";
    if(ans1.possible)
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
            o.stonks.insert(tokens[j], stoi(tokens[j+1]));// * (orderlist[j].bs == 'b' ? 1 : -1));
            o.stonklist.push_back(tokens[j]); 
        }
        cout << "check3\n";

        orderlist.push_back(o);

        cout << "check4\n";

        //Arbitrage Check!!

        Order upTillNow;
        vector <int> includedinorder;
        Combo cycle = CyclePossible(orderlist, orderlist.size(), upTillNow, includedinorder);
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