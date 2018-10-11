////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @author     : Artur Ostrowski
//  @usage      : Singleton responsible for handling config file loading and reloading (in the future).
//  @version    : 1.0.0
//
////////////////////////////////////////
#pragma once

#include <map>
#include <sstream>

class ConfigLoader
{
public:
	int LoadConfigData(const std::string& InFileName, const std::string& InCategory, std::map<std::string, std::string>& OutResult);
	int LoadInputData(const std::string& InFileName, const std::string& InCategory, std::map<char, std::string>& OutResult);
	
	static inline ConfigLoader& GetInstance()
	{
		static ConfigLoader Instance;
		return Instance;
	}

	void Split(const char * InSentence, char InDelimiter, std::vector<std::string> & OutTokens)
	{
		std::stringstream SStream;
		SStream.str(InSentence);

		std::string Item;
		while (std::getline(SStream, Item, InDelimiter))
		{
			OutTokens.push_back(Item);
		}
	}

	std::string GetFileExtension(const char* aFilePath)
	{
		std::vector<std::string> OutTokens;
		Split(aFilePath, '.', OutTokens);
		if (OutTokens.size() == 2)
		{
			return OutTokens[1];
		}
		else
		{
			return "";
		}
	}

	// TODO : Implement
	void ReloadConfigData(); 
private:

	// TODO : Implement
	void LoadDataFromFile(const std::string& InFileName, const std::string& InCategory, std::map<std::string, std::string>& OutResult);
};