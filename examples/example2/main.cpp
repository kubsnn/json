/*
	This example shows that parser is able accept to JSON string 
	that contains irregular whitespaces.
*/
#include <iostream>
#include "../../src/json.hpp"

using json = jaszyk::json;

int main()
{
    std::string_view json_to_parse = R"(
{"name" :     "John",
    "grades":{
    "Math" : 4,
    "Physics":[1,   "missing"]
},"huh" : null, "mhmm" :  {":D": ":("}}
	)";
    
    auto json = json::parse(json_to_parse);
    std::cout << json.to_pretty_string(2) << std::endl;
}

/*
// Output:
{
  "name" : "John",
  "grades" : {
    "Math" : 4,
    "Physics" : [
      1,
      "missing"
    ]
  },
  "huh" : null,
  "mhmm" : {
    ":D" : ":("
  }
}
*/
