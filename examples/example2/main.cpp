/*
	This example shows that parser is able accept to JSON string 
	that contains irregular whitespaces.
*/
#include <iostream>
#include "../../src/json.hpp"

using json_t = json::json;

int main()
{
    std::string_view json_to_parse = R"(
{"name" :     "John",
    "grades":{
    "Math" : [ 5 , 4  , 5 ],
    "Physics":[1,   "missing",2]
},"huh" : null, "mhmm" : [[[],{}], {":D": ":("}]
	)";
    
    auto json = json_t::parse(json_to_parse);
    std::cout << json.to_pretty_string(2) << std::endl;
}

/*
// Output:
{
  "name" : "John",
  "grades" : {
    "Math" : [
      5,
      4,
      5
    ],
    "Physics" : [
      1,
      "missing",
      2
    ]
  },
  "huh" : null,
  "mhmm" : [
    [
      [],
      {}
    ],
    {
      ":D" : ":("
    }
  ]
}
*/
