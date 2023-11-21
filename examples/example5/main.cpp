/*
    In this example, you can see how to retrieve values 
	from a JSON object, including the ability to create 
	non-const references. Additionally, it demonstrates 
	the proper handling of escape characters.
*/

#include <iostream> 
#include "../../include/json.hpp"

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
	auto json = json::load(json_to_parse);
    
	std::cout << json["children"][0] << std::endl; // print in json style
	auto& first_child_ref = json["children"][0].get<std::string>();
    first_child_ref = "Jac\nk";
    

	const auto& children = json["children"].get<json::array>();
	for (const auto& child : children) {
		//if (std::holds_alternative<std::string>(child)) {			
		//	std::cout << std::get<std::string>(child) << std::endl;		
		//}
		// or
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