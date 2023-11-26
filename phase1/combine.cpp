#include "receiver.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../headers/map.cpp"
#include "../headers/string_extra.cpp"
#include <map>
#include "part1.cpp"
#include "part2.cpp"
#include "part3.cpp"
#include "../headers/map_new.cpp"
//hello
typedef long long ll;
using namespace std;



int main(int argc, char **argv) {
    //std::cout << "Hello, World!" << std::endl;
    //std::cout << argv[1]<< std::endl;
    
    if(strcmp(argv[1], "1") == 0){
        //std::cout << "Part 1" << std::endl;
        Part1::part1();
        std::cout << "Part1 done" << std::endl;
    }
        
    else if(strcmp(argv[1], "2") == 0)
        Part2::part2();
    else if(strcmp(argv[1], "3") == 0)
        Part3::part3();
}



