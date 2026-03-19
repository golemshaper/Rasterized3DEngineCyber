#pragma once
#include <string>
#include <vector>
class GroupKeyValueDataExchange
{
public:

	struct Key {
		std::string keyName;
		std::string dataStr;
		int dataVal;
	};
	struct Group {
		std::string groupName;
		std::vector<Key> keys;
	};
	std::vector<Group> groups;
	void ParseFromString(const std::string str);
	std::string ToDataString();
};

