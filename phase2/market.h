#ifndef MARKET_H
#define MARKET_H
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

class market
{
public:
	market(int argc, char** argv);
	void start();
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

private:
};
#endif
