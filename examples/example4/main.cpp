/*
	This example shows how inproper brackets error message looks like.
*/
#include <iostream>
#include "../../src/json.hpp"

using json_t = json::json;

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
        auto json = json_t::parse(json_with_error);
	}
	catch (const std::runtime_error& e) {
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