#ifndef __RESOURCE_MANAGER_HPP__
#define __RESOURCE_MANAGER_HPP__
#include <unordered_map>
#include "manager\Config.hpp"
#include "manager\GameConfig.hpp"
#include <string>
#include <vector>
#include "Config.hpp"

#define C_RED 6
#define RESOURCE(x) ResourceManager::getInstance()->getResource(x)
#define FRAMES(x) ResourceManager::getInstance()->getFrameNames(x)
#define STRING(x) ResourceManager::getInstance()->getString(x)
#define CONFIG(x) ResourceManager::getInstance()->getConfig(x)

class ResourceManager {
public:
	static ResourceManager * getInstance();
	const char * getResource(const std::string & key) const;
	const std::vector<std::string> & getFrameNames(const std::string & key) const;
	const char * getString(const std::string & key) const;
	double getConfig(const std::string & key) const;
	void init(const char * resources = "src/resource.xml",
		const char * animations = "src/animation.xml", 
		const char * configs = "src/config.xml",
		const char * strings = "src/string.xml");
private:
	ResourceManager() = default;
	std::unordered_map<std::string, std::string> m_Resources;
	std::unordered_map<std::string, std::vector<std::string>> m_FrameNames;
	std::unordered_map<std::string, double> m_Configs;
	std::unordered_map<std::string, std::string> m_Strings;
};
#endif