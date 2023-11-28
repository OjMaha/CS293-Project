#ifndef MARKET_H
#define MARKET_H
#include <iostream>
#include <string>
#include <vector>
#include "headers/map_new.cpp"
using namespace std;
    
struct order
{
    string stonk;
    string broker;
    int price;
    int quantity;
	int issue;
    int expiry;
	int bs;				/// 1 for buy, -1 for sell 		
};

struct sell_order_compare {
    bool operator()(const order& lhs, const order& rhs) const {
        // Compare prices in ascending order
        if (lhs.price != rhs.price) {
            return lhs.price > rhs.price;
        }

        if(lhs.expiry == -1 && rhs.expiry == -1) return lhs.broker > rhs.broker;
        if(lhs.expiry == -1) return true;
        if(rhs.expiry == -1) return false;

        // If prices are equal, compare expiry times in ascending order
        if (lhs.expiry != rhs.expiry) {
            return lhs.expiry > rhs.expiry;
        }

        // If both price and expiry time are equal, compare broker names alphabetically
        return lhs.broker > rhs.broker;
    }
};

struct buy_order_compare {
    bool operator()(const order& lhs, const order& rhs) const {
        // Compare prices in descending order
        if (lhs.price != rhs.price) {
            return lhs.price < rhs.price;
        }

        if(lhs.expiry == -1 && rhs.expiry == -1) return lhs.broker > rhs.broker;
        if(lhs.expiry == -1) return true;
        if(rhs.expiry == -1) return false;
        
        // If prices are equal, compare expiry times in ascending order
        if (lhs.expiry != rhs.expiry) {
            return lhs.expiry > rhs.expiry;
        }

        // If both price and expiry time are equal, compare broker names alphabetically
        return lhs.broker > rhs.broker;
    }
};

class market
{
public:
	market(int argc, char** argv);
	void start();
	vector <order> buy_orders;
    vector <order> sell_orders;
	vector <order> all_orders;
	CustomMap2 <int, int> indices;	//all_orders indices corresponding to buy_orders and sell_orders
	int all_num = 0;
	int buy_num = 0;
	int sell_num = 0;
	
	bool valid_orders(order later, order first){		//same stock, both active and price match
		if(later.stonk != first.stonk) return false;
		if(later.bs == 1 && later.price < first.price) return false;
		if(later.bs == -1 && later.price > first.price) return false;
		if(later.issue > first.expiry && first.expiry != -1) return false;
		if(later.quantity == 0 || first.quantity == 0) return false;		
		return true;
	}

	int get_index(vector <order> all_orders, order o){
		for(int i = 0; i < all_orders.size(); i++){
			if(all_orders[i].stonk == o.stonk && all_orders[i].broker == o.broker && 
			all_orders[i].price == o.price && all_orders[i].quantity == o.quantity && all_orders[i].issue == o.issue && all_orders[i].expiry == o.expiry && all_orders[i].bs == o.bs) return i;
		}
		return -1;
	}

private:
};
#endif
