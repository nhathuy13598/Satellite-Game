#include "ResourceManager.hpp"
#include "xml\pugixml.hpp"
#include "cocos2d.h"
#include <cstring>

USING_NS_CC;

const char * ResourceManager::getResource(const std::string & key) const
{
	return m_Resources.at(key).c_str();
}

const std::vector<std::string> & ResourceManager::getFrameNames(const std::string & key) const
{
	return m_FrameNames.at(key);
}

const char * ResourceManager::getString(const std::string & key) const
{
	return m_Strings.at(key).c_str();
}

double ResourceManager::getConfig(const std::string& key) const
{
	return m_Configs.at(key);
}

ResourceManager * ResourceManager::getInstance()
{
	static ResourceManager resManager;
	return &resManager;
}

void ResourceManager::init(const char * resPath, const char * frnPath, const char* configs, const char * strPath)
{
	//File utils
	FileUtils* file = FileUtils::getInstance();
	pugi::xml_document doc;
	//read resources
	doc.load_string(file->getStringFromFile(resPath).c_str());
	auto resources = doc.child("resource").children("r");
	for (auto resource : resources) {
		m_Resources[resource.attribute("id").as_string()] = resource.text().as_string();
	}
	//read animations
	doc.load_string(file->getStringFromFile(frnPath).c_str());
	auto anims = doc.child("resource").children("animation");
	for (auto anim : anims) {
		std::vector<std::string> &names = m_FrameNames[anim.attribute("id").as_string()] = std::vector<std::string>{};
		auto frames = anim.children("f");
		for (auto frame : frames) {
			names.push_back(frame.text().as_string());
		}
	}
	//read configs
	doc.load_string(file->getStringFromFile(configs).c_str());
	auto configValues = doc.child("resource").children("c");
	for (auto configValue : configValues) {
		m_Configs[configValue.attribute("id").as_string()] = configValue.text().as_double();
	}
	//read string
	doc.load_string(file->getStringFromFile(strPath).c_str());
	auto strings = doc.child("resource").children("string");
	for (auto string : strings) {
		m_Strings[string.attribute("id").as_string()] = string.text().as_string();
	}
}
