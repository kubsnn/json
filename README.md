# JSON Parser Project

This is a JSON parser project that aims to provide a lightweight and efficient JSON parsing library for C++ applications. The project is designed to enable developers to easily parse JSON data into native data structures and vice versa.

## Features

- Parse JSON data into native data structures.
- Serialize native data structures into JSON format.
- Support for handling various data types: strings, numbers, booleans, objects (dictionaries), arrays, and null values.
- Support for escape characters.
- Error handling for invalid JSON input.

## Examples

Inserting new data into a json

```cpp
#include <iostream>
#include "../../src/json.hpp"

using json = jaszyk::json;

int main()
{
    // easy inserting data into the json variable
    json j = json::object();
    j["name"] = "John";
    j["age"] = 20;
    j["isMale"] = true;
    j["friends"] = json::array();
    j["friends"].add("Adam");
    j["friends"].add("Eva");

    std::cout << j << std::endl;     // alternatively:
                                        // std::cout << json.to_string() << std::endl;
}
// Output:
// { "name" : "John", "age" : 20, "isMale" : true, "friends" : [ "Adam", "Eva" ] }
```

Using initializer list for creating json

```cpp
#include <iostream>
#include "../../src/json.hpp"

using json = jaszyk::json;

int main()
{
	json data = {
		{ "username", "kubsn" },
		{ "friends", 
			{
				{ "name", "krabbens" }
			}
		},
		{ "repos", json::array(
			{
				"json",
				"consolecolors",
				false,
				"tcp-chat"
			})
		},
		{ "stars", 0 },
		{ "forks", json::null() },
		{ "is_admin", false }
	};

	std::cout << data.dump(true) << std::endl;
}

/*
// OUTPUT:
{
	"username" : "kubsn",
	"friends" : {
		"name" : "krabbens"
	},
	"repos" : [
		"json",
		"consolecolors",
		false,
		"tcp-chat"
	],
	"stars" : 0,
	"forks" : null,
	"is_admin" : false
}
*/
```

Parsing json-string into a json

```cpp
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
    
    auto j = json::parse(json_to_parse);
    std::cout << j.to_pretty_string(2) << std::endl; 
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
```

Retrieving data from json object

```cpp
#include <iostream> 
#include "../../src/json.hpp"

using json = jaszyk::json;

int main()
{
    std::string_view json_to_parse = R"(
{
    "name" : "Jack",
    "age" : 20,
    "children" : [ "\\\\Alan", "\\\\Rose" ]
}
    )";
    auto j = json_t::parse(json_to_parse);
    
    std::cout << j["children"][0] << std::endl; // print in json style
    auto& first_child_ref = j["children"][0].get<std::string>();
    first_child_ref = "Jac\nk";
    
    const auto& children = j["children"].get<json::array>();
    for (const auto& child : children) {
        if (child.is<std::string>()) {
            std::cout << child.get<std::string>() << std::endl;
        }
    }
}

/*
// Output:
"\\\\Alan"
Jac
k
\\Rose
*/
```

## Requirements

- C++17 compiler
- Attached flatmap.hpp
- Currently tested on Visual Studio 2022 and GCC13

### Installation

Because it is a header only library, it can be added to any project with ease.
(just include a json.hpp header)
