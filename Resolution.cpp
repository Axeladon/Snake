#include "Resolution.h"

void Resolution::SetResolution(m_resolution type_of_resolution)
{
	my_current_resolution = type_of_resolution;
	if (type_of_resolution == m_resolution::Full_HD)
	{
		actual_resol = "1920 x 1080";
		m_x = 1920;
		m_y = 1080;
	}
	else if (type_of_resolution == m_resolution::HD_Ready)
	{
		actual_resol = "1366 x 768";
		m_x = 1366;
		m_y = 768;
	}
}

void Resolution::SetResolution(int x, int y)
{
	std::string temp1, temp2;
	temp1 = std::to_string(x);
	temp2 = std::to_string(y);

	actual_resol = temp1 + " x " + temp2;
	m_x = x;
	m_y = y;
}

std::string Resolution::GetStringResolution() const
{
	return actual_resol;
}

sf::Vector2i Resolution::GetIntRecolution() const
{
	sf::Vector2i temp;
	temp.x = m_x;
	temp.y = m_y;
	return temp;
}

void Resolution::offset_left()
{
	if ((my_current_resolution - 1) >= 1)
		my_current_resolution -= 1;
	else
		my_current_resolution = count_of_max_resolution;

	SetResolution(m_resolution(my_current_resolution));
}

void Resolution::offset_right()
{
	if ((my_current_resolution + 1) <= count_of_max_resolution)
		my_current_resolution += 1;
	else
		my_current_resolution = 1;

	SetResolution(m_resolution(my_current_resolution));
}