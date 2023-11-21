/*
	This example shows usage of std::initializer_list constructor
*/
#include <iostream>
#include "../../include/json.hpp"

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

