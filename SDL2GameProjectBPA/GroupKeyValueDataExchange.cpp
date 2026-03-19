#include "GroupKeyValueDataExchange.h"

void GroupKeyValueDataExchange::ParseFromString(const std::string str)
{
	enum ParseState { GroupBuild, DataBuildKey, DataBuildValue };
	ParseState parseState = GroupBuild;
	std::string string_builder = "";
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '\n')
		{
			//ignore new lines
			continue;
		}
		if (str[i] == '\t')
		{
			//ignore tabs
			continue;
		}
		switch (parseState)
		{
			case GroupBuild:
				if (str[i] == '[')
				{
					string_builder.clear();
					continue;
				}
				if (str[i] == ']')
				{
					//BUILD NEW GROUP AND NAME IT, PUSH IT
					Group nGroup;
					nGroup.groupName = string_builder;
					groups.push_back(nGroup);
					string_builder.clear();
					parseState = DataBuildKey;
					continue;
				}
				string_builder += str[i];
				break;
			case DataBuildKey:
				if (str[i] == '[')
				{
					string_builder.clear();
					parseState = GroupBuild;
					continue;
				}
				if (str[i] == '=')
				{
					Key nKey;
					nKey.keyName = string_builder;
					groups[groups.size() - 1].keys.push_back(nKey);
					string_builder.clear();
					parseState = DataBuildValue;
					continue;
				}
				break;
			case DataBuildValue:
				if (str[i] == ';')
				{
					int group = groups.size() - 1;
					int key = groups[group].keys.size() - 1;
					groups[group].keys[key].dataStr = string_builder;
					string_builder = groups[group].keys[key].dataStr;
					groups[group].keys[key].dataVal = std::stoi(string_builder);
					//TODO: assign a int value if it's can be converted to an int.
					string_builder.clear();
					parseState = DataBuildKey;
					continue;
				}
				break;
		}
	}
}

std::string GroupKeyValueDataExchange::ToDataString()
{
	return std::string();
}
