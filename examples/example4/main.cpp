﻿/*
	This example shows how inproper brackets error message looks like.
*/
#include <iostream>
#include "../../include/json.hpp"

using json = jaszyk::json;

int main()
{
    std::string_view json_with_error = R"(
{
    "colors_shades" : {
        "whites" : [[ "snow", "ivory", "floral white", "white smoke" ],
        "yellows" : [ "cornsilk", "blanched almond", "yellow", "dark yellow" ],
        "oranges" : [ "orange", "dark orange", "coral" ],
    },
    "numbers" : [ 1, 2.2, 3, 4, 5, 6, 7, 8, 9.3, {} ]
}
)";
    try {
        auto json = json::parse(json_with_error);
	}
	catch (const json::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

/*
// Output:
Unexpected error at 19 character:
  Missing closing bracket!
>>>At line 4:
>>>        "whites" : [[ "snow", "ivory", "floral whit...
                      ^
                   ~~~~~~~
*/