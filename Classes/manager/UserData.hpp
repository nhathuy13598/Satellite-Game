#ifndef __USER_DATA__
#define __USER_DATA__
#include "xml\pugixml.hpp"
#include "ResourceManager.hpp"

class UserData {
public:
	static const char * FILE_NAME;
	static UserData * getInstance();
	void init(const char * fileName = RESOURCE(R_R_userdata_xml));
	void reload();
	int getHighScore() const;
	float getAudio() const;
	void setHighScore(int highScore);
	void setAudio(float audio);
	void save();
private:
	pugi::xml_document m_Doc;
	int m_HighScore = 0;
	float m_Audio = 0;
};

#endif