#pragma once

#include <string>
#include <SFML\Graphics.hpp>

class Resolution
{
	int m_x;
	int m_y;
	std::string actual_resol;

	int my_current_resolution;
	int count_of_max_resolution;
public:
	Resolution();
	~Resolution();
	enum m_resolution { HD_Ready = 1, Full_HD };
	void SetResolution(m_resolution type_of_resolution);
	void SetResolution(int x, int y);
	std::string GetStringResolution() const;
	sf::Vector2i GetIntRecolution() const;
	void offset_left();
	void offset_right();
};

inline Resolution::Resolution()
{
	count_of_max_resolution = 2;
}

inline Resolution::~Resolution()
{
	//записать в файл
}