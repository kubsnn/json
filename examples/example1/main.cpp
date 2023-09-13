/*
	This example shows how to create a JSON object and print it
	to the standard output.
*/

#include <iostream>
#include "../../src/json.hpp"

using json_t = json::json;

int main()
{
    json_t json = json::dictionary();
	json["name"] = "John";
	json["age"] = 20;
	json["isMale"] = true;
	json["friends"] = json::array();
	json["friends"].add("Adam");
	json["friends"].add("Eva");
	

    std::cout << json << std::endl;		// alternatively:
										// std::cout << json.to_string() << std::endl;
	// Output:
	// { "name" : "John", "age" : 20, "isMale" : true, "friends" : [ "Adam", "Eva" ] }
}

