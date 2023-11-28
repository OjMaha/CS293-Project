// trader_tk.h
#ifndef TRADER_TK_H
#define TRADER_TK_H


#include <string>

int reader(int time);
int trader(std::string *message);
void* userThread(void* arg);
void* userTrader(void* arg);

#endif // TRADER_TK_H
