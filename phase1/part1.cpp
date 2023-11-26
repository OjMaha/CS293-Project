// #include "receiver.h"
// #include <iostream>
// #include <string>
// #include <vector>
// #include <sstream>
//#include "../headers/map.cpp"
// #include "../headers/string_extra.cpp"
//hello
//using namespace std;
//#include "combine.cpp"


namespace Part1{
    void part1() {
        CustomMap<std::string, int> stockMap;  //gonna store stock name and price
        CustomMap<std::string, int> buyMap;    //gonna store stock name and best (highest) rejected buy price 
        CustomMap<std::string, int> sellMap;   //gonna store stock name and best (lowest) rejected sell price 
        bool broken = false;
        std::string firsthalfline;
        Receiver rcv;
        
        sleep(5);
        int chutiyapa = 0;

        while(true){
            std::string message = rcv.readIML();
        
            // Create a std::stringstream from the input std::string
            std::istringstream ss(message);
            std::string eachline;

            
            

            // Split the std::string using the delimiter '#'
            while (std::getline(ss, eachline, '#')) {
                if(broken){
                    eachline = firsthalfline + eachline; //concatenate the broken line with the next line
                    broken = false;
                    //std::cout << "concatenated line" << eachline << std::endl;
                }

                if(eachline == "$") return;
                if(eachline == ""){
                    chutiyapa++;
                    if(chutiyapa >= 10) return; //to avoid weird infinite loops
                }

                char bs1 = eachline[eachline.size()-1];
                if(bs1 != 'b' && bs1 != 's'){             //broken line; must continue
                    broken = true;
                    firsthalfline = eachline;
                    //std::cout << "broken line" << firsthalfline << std::endl;
                    break;
                }
                std::cout << std::endl;
                std::cout << eachline << std::endl;
                
                int k = 0; //k is counter used to maintain which part of the std::string is being read (stock, price, b/s)
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
                    std::cout<< stock <<" "<<p<<" ";
                    if (bs == "b") std::cout <<"s"<<std::endl;
                    else std::cout<<"b"<<std::endl;
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
                            std::cout << "No trade" << std::endl;   //cancelled
                            continue;
                        }

                    }
                    else if(bs == "s"){
                        if(!sellMap.contains(stock))
                            sellMap.insert(stock, p);
                        else if(p < sellMap.get(stock))
                            sellMap.set(stock, p);
                        else{
                            std::cout << "No trade" << std::endl;   //cancelled
                            continue;
                        }
                    }

                    
                    if(bs == "b" && p == sellMap.get(stock) && sellMap.contains(stock)){ //if buy offer is equal to best rejected sell price
                        std::cout<<"No trade"<<std::endl;
                        sellMap.remove(stock);
                        buyMap.remove(stock);
                        continue;
                    }
                    else if(bs == "s" && p == buyMap.get(stock) && buyMap.contains(stock)){ //if sell offer is equal to best rejected buy price
                        std::cout<<"No trade"<<std::endl;
                        sellMap.remove(stock);
                        buyMap.remove(stock);
                        continue;
                    }

                    //aight so trade hasnt been cancelled
                    //std::cout << "Trade not yet cancelled   ";
                    if(bs == "b"){
                        //trade is possible
                        if(p > p_i){
                            std::cout<<stock<<" "<<price<<" "<<"s"<<std::endl;
                            stockMap.set(stock, p);
                            buyMap.remove(stock);
                            continue;
                        }
                        else{ //trade is not possible, see if p > best rejected buy price
                            if(buyMap.get(stock) < p || buyMap.contains(stock) == false){
                                buyMap.set(stock, p);
                            }
                            std::cout<<"No trade"<<std::endl;
                            continue;
                        }
                    }
                    else if(bs == "s"){
                        //trade is possible
                        if(p < p_i){
                            std::cout<<stock<<" "<<price<<" "<<"b"<<std::endl;
                            stockMap.set(stock, p);
                            sellMap.remove(stock);
                            continue;
                        }
                        else{ //trade is not possible, see if p < best rejected sell price
                            if(sellMap.get(stock) > p || sellMap.contains(stock) == false){
                                sellMap.set(stock, p);
                            }
                            std::cout<<"No trade"<<std::endl;
                            continue;
                        }
                    }
                    
                }
                    //TK ENDS WRITING



            }

            if(eachline == "$"){
                return;
            }

        }
        return;
    }



}


//OJAS CODE : QUITE SIMILAR TO TK'S CODE, BUT DIDNT HAVE THE ENERGY TO LOOK FOR THE SUBTLETIES SO WROTE MY OWN AGAIN
/*if((p_i < p && bs=="b") && (p > sellMap.get(stock) || sellMap.contains(stock) == false) ){
                    stockMap.set(stock, p);

                    if(buyMap.get(stock) != p){
                        std::cout<<stock<<" "<<price<<" "<<"s"<<std::endl;
                    }

                    else{
                        buyMap.remove(stock);
                        std::cout<<"No trade"<<std::endl;
                    }
                }
                else if(p_i > p && bs=="s" && (p < buyMap.get(stock) || buyMap.contains(stock) == false) ){
                    stockMap.set(stock, p);

                    if(sellMap.get(stock) != p){
                        std::cout<<stock<<" "<<price<<" "<<"b"<<std::endl;
                    }
                    else{
                        sellMap.remove(stock);
                        std::cout<<"No trade"<<std::endl;
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
                    std::cout<<"No trade"<<std::endl;
                }
            }

            //std::cout << eachline << std::endl;*/
