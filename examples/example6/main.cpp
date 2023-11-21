/*
	This example shows usage of std::initializer_list constructor
*/
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
		"tcp-chat"
	],
	"stars" : 0,
	"forks" : null,
	"is_admin" : false
}
	
*/

