#ifndef _MAP2_
#define _MAP2_
#include "SFML\Graphics.hpp"
#include "camera.h"
#include <string>

class Map : public sf::Image, public sf::Texture, public sf::Sprite //херовый класс, не соответствует ООП. П.С. методы тоже ГОВНО
{
	int height;
	int width;
	int resolution_x;
	int resolution_y;

	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f GetScaleFromCurrentResolution(int resolution_x, int resolution_y);
public:
	std::string m_map[20];
	Map();
	void LoadMapFromFile(const std::string & img_path);
	void DrawMap(sf::RenderWindow & window);
	sf::Vector2i GetSizeMap() const;
	sf::IntRect GetSizeRect() const; //возвращает размер спрайта
	void SetSizeResolution(int x, int y);
};

inline Map::Map() //11x19
{
	m_map[0] =  "                   ";
	m_map[1] =  " ################# ";
	m_map[2] =  " ################# ";
	m_map[3] =  " ################# ";
	m_map[4] =  " ################# ";
	m_map[5] =  " ################# ";
	m_map[6] =  " ################# ";
	m_map[7] =  " ################# ";
	m_map[8] =  " ################# ";
	m_map[9] =  " ################# ";
	m_map[10] = "                   ";

	width = m_map[0].size();
	height = 11;
}

#endif