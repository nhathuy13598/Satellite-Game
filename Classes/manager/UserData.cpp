#include "UserData.hpp"
#include <stdexcept>
#include "xml\pugixml.hpp"
#include "cocos2d.h"
#include <sstream>

USING_NS_CC;

const char * UserData::FILE_NAME = "userdata";

UserData * UserData::getInstance()
{
	static UserData data;
	return &data;
}

void UserData::init(const char * fileName)
{
	if (!fileName) {
		throw std::invalid_argument("UserData::init(const char * fileName): File name can't be null");
	}
	FileUtils* file = FileUtils::getInstance();
	std::string writeableFile = file->getWritablePath() + FILE_NAME;
	if (!file->isFileExist(writeableFile)) {
		m_Doc.load_string(file->getStringFromFile(fileName).c_str());
		m_Doc.save_file(writeableFile.c_str());
	}
	else {
		m_Doc.load_file(writeableFile.c_str());
	}
	pugi::xml_node root = m_Doc.child("data");
	m_HighScore = root.child("high-score").text().as_int();
	m_Audio = root.child("audio").text().as_float();
}

void UserData::reload()
{
	init(FILE_NAME);
}

int UserData::getHighScore() const
{
	return m_HighScore;
}

float UserData::getAudio() const
{
	return m_Audio;
}

void UserData::setHighScore(int highScore)
{
	if (highScore < 0) {
		throw std::invalid_argument("UserData::setHighScore(float highScore): score must be >= 0");
	}
	m_HighScore = highScore;
	pugi::xml_node root = m_Doc.child("data");
	root.child("high-score").text().set(highScore);
}

void UserData::setAudio(float audio)
{
	if (audio < 0 || audio > 1) {
		throw std::invalid_argument("UserData::setAudio(float audio): audio must be <= 1, >= 0");
	}
	m_Audio = audio;
	pugi::xml_node root = m_Doc.child("data");
	root.child("audio").text().set(audio);
}

void UserData::save()
{
	m_Doc.save_file((FileUtils::getInstance()->getWritablePath() + FILE_NAME).c_str());
}
