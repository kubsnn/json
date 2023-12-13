// example7.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include "../../include/json.hpp"
#include <sstream>
#include <fstream>
#include <string>

using jaszyk::json;


int main()
{
    std::string data = (std::stringstream() << std::ifstream("example.json").rdbuf()).str();

    try {
        json data1 = json::load(data);

        std::ofstream("example.out.json") << data1.get<std::string>();


        std::cout << data1.get<std::string>() << std::endl;
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    //std::cout << bytes << std::endl;
}

