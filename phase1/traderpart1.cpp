#include "receiver.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../headers/map.cpp"
#include "../headers/string_extra.cpp"
//hello
using namespace std;
int main() {
    CustomMap<std::string, int> stockMap;  //gonna store stock name and price
    CustomMap<std::string, int> buyMap;    //gonna store stock name and best (highest) rejected buy price 
    CustomMap<std::string, int> sellMap;   //gonna store stock name and best (lowest) rejected sell price 
    bool broken = false;
    string firsthalfline;
    Receiver rcv;
    
    sleep(5);

    while(true){
        std::string message = rcv.readIML();
    
        // Create a stringstream from the input string
        std::istringstream ss(message);
        std::string eachline;
        

        // Split the string using the delimiter '#'
        while (std::getline(ss, eachline, '#')) {
            if(broken){
                eachline = firsthalfline + eachline; //concatenate the broken line with the next line
                broken = false;
                //cout << "concatenated line" << eachline << endl;
            }

            if(eachline == "$") break;
            if(eachline == "") continue;

            char bs1 = eachline[eachline.size()-1];
            if(bs1 != 'b' && bs1 != 's'){             //broken line; must continue
                broken = true;
                firsthalfline = eachline;
                //cout << "broken line" << firsthalfline << endl;
                break;
            }

            int k = 0; //k is counter used to maintain which part of the string is being read (stock, price, b/s)
            std::string stock;
            std::string price;
            std::string bs;

            std::istringstream iss(eachline);
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

            if(stockMap.contains(stock) == false) {   //stock is not in the list
                stockMap.insert(stock, p);
                cout<< stock <<" "<<p<<" ";
                if (bs == "b") cout <<"s"<<endl;
                else cout<<"b"<<endl;
            }

            //THIS IS THE CASE WHERE STOCK IS ALREADY IN THE LIST.
            
            //here i am assuming p_i is the price of last stock order.
            else {
                int p_i = stockMap.get(stock);          //p_i is last traded price; sell above p_i, buy below p_i
                    //buy offer > p_i        
                //TK STARTS WRITING
                // let's check cancellation conditions
                if(bs == "b"){
                    if(!buyMap.contains(stock))
                        buyMap.insert(stock, p);
                    else if(p > buyMap.get(stock))
                        buyMap.set(stock, p);
                    else{
                        cout << "No trade" << endl;   //cancelled
                        continue;
                    }

                }
                else if(bs == "s"){
                    if(!sellMap.contains(stock))
                        sellMap.insert(stock, p);
                    else if(p < sellMap.get(stock))
                        sellMap.set(stock, p);
                    else{
                        cout << "No trade" << endl;   //cancelled
                        continue;
                    }
                }

                
                if(bs == "b" && p == sellMap.get(stock) && sellMap.contains(stock)){ //if buy offer is equal to best rejected sell price
                    cout<<"No trade"<<endl;
                    sellMap.remove(stock);
                    buyMap.remove(stock);
                    continue;
                }
                else if(bs == "s" && p == buyMap.get(stock) && buyMap.contains(stock)){ //if sell offer is equal to best rejected buy price
                    cout<<"No trade"<<endl;
                    sellMap.remove(stock);
                    buyMap.remove(stock);
                    continue;
                }

                //aight so trade hasnt been cancelled
                //cout << "Trade not yet cancelled   ";
                if(bs == "b"){
                    //trade is possible
                    if(p > p_i){
                        cout<<stock<<" "<<price<<" "<<"s"<<endl;
                        stockMap.set(stock, p);
                        buyMap.remove(stock);
                        continue;
                    }
                    else{ //trade is not possible, see if p > best rejected buy price
                        if(buyMap.get(stock) < p || buyMap.contains(stock) == false){
                            buyMap.set(stock, p);
                        }
                        cout<<"No trade"<<endl;
                        continue;
                    }
                }
                else if(bs == "s"){
                    //trade is possible
                    if(p < p_i){
                        cout<<stock<<" "<<price<<" "<<"b"<<endl;
                        stockMap.set(stock, p);
                        sellMap.remove(stock);
                        continue;
                    }
                    else{ //trade is not possible, see if p < best rejected sell price
                        if(sellMap.get(stock) > p || sellMap.contains(stock) == false){
                            sellMap.set(stock, p);
                        }
                        cout<<"No trade"<<endl;
                        continue;
                    }
                }
                
            }
                //TK ENDS WRITING



        }

        if(eachline == "$"){
            break;
        }

    }
}



//OJAS CODE : QUITE SIMILAR TO TK'S CODE, BUT DIDNT HAVE THE ENERGY TO LOOK FOR THE SUBTLETIES SO WROTE MY OWN AGAIN
/*if((p_i < p && bs=="b") && (p > sellMap.get(stock) || sellMap.contains(stock) == false) ){
                    stockMap.set(stock, p);

                    if(buyMap.get(stock) != p){
                        cout<<stock<<" "<<price<<" "<<"s"<<endl;
                    }

                    else{
                        buyMap.remove(stock);
                        cout<<"No trade"<<endl;
                    }
                }
                else if(p_i > p && bs=="s" && (p < buyMap.get(stock) || buyMap.contains(stock) == false) ){
                    stockMap.set(stock, p);

                    if(sellMap.get(stock) != p){
                        cout<<stock<<" "<<price<<" "<<"b"<<endl;
                    }
                    else{
                        sellMap.remove(stock);
                        cout<<"No trade"<<endl;
                    }

                }
                else {
                    if(p_i >= p && bs=="b"){
                        if(sellMap.get(stock) < p || sellMap.contains(stock) == false){
                            sellMap.set(stock,p);
                        }
                    }
                    else if(p_i <= p && bs=="s"){
                        if(buyMap.get(stock) > p || buyMap.contains(stock) == false){
                            buyMap.set(stock,p);
                        }
                    }
                    cout<<"No trade"<<endl;
                }
            }

            //cout << eachline << endl;*/
