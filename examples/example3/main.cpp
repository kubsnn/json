/*
    This example shows how typical error messages look like
	when parsing fails. The error message contains the line
	number, the character number,  the line where the error
	occured and a pointer to  the character that caused the
    exception.
*/
#include <iostream>
#include "../../src/json.hpp"

using json_t = json::json;

int main()
{
    std::string_view json_to_parse = R"({
  "name": "JSON",
    "description": "A JSON file designed to test JSON parser.",
  "nested_object": {
    "key1": "value1",
    "key2": ["value2", "value3"],
    "key3": {
      "sub_key1": 42,
      "sub_key2": true,
      "sub_key3": null
    }
  },
  "boolean_values": [true, false, true, true, false],
  "numbers_with_error": [3.1415whooops, 2.71828183, 1.61803399],
  "escape_characters": "This is a newline character: \nAnd this is a tab character: \t",
  "empty_object": {},
  "empty_array": []
})";
    
    try {
        auto json = json_t::parse(json_to_parse);
        std::cout << json.to_pretty_string(2) << std::endl;
	}
	catch (const std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}
}

/*
// Output:
Unexpected error at 31 character:
  Expected ','
>>>At line 14:
>>>  "numbers_with_error": [3.1415whooops, 2.71828183, 1.61803399],
                                  ^
                               ~~~~~~~
*/