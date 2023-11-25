//use it to declare some functionality
#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

struct order
{
    string stonk;
    string broker;
    int price;
    int quantity;
	int issue;
    int expiry;
	int bs;				// 1 for buy, -1 for sell 		
};

void print_order(order o){
    cout << "issue time:"<<o.issue << " " <<"broker:"<< o.broker << " " <<"BS"<< o.bs << " " << o.stonk 
    << " " <<"price"<< o.price << " " << "quantity"<<o.quantity << " " <<"expiresat:"<< o.expiry << endl;
}

void print_all_orders(vector<order> orders){
    for(int i = 0; i<orders.size(); i++){
        print_order(orders[i]);
    }
}

    vector <order> buy_orders;
    vector <order> sell_orders;
	vector <order> all_orders;
	map <int, int> indices;	//all_orders indices corresponding to buy_orders and sell_orders
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