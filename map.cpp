#include "map.h"

void Map::LoadMapFromFile(const std::string & img_path)
{
	image.loadFromFile("images//" + img_path);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
}

void Map::SetSizeResolution(int x, int y)
{
	resolution_x = x;
	resolution_y = y;
}

sf::Vector2f Map::GetScaleFromCurrentResolution(int resolution_x, int resolution_y)
{
	float coefficient_x = resolution_x / 1920.0f;
	float coefficient_y = resolution_y / 1080.0f;
	coefficient_x = 1 - coefficient_x;
	coefficient_y = 1 - coefficient_y;

	sf::Vector2f temp;
	if (coefficient_x == 1 || coefficient_y == 1)
	{
		temp.x = 0;
		temp.y = 0;
	}
	else
	{
		temp.x = coefficient_x;
		temp.y = coefficient_y;
	}
	return temp;
}

void Map::DrawMap(sf::RenderWindow & window)
{
	sf::Vector2f scale_from_resolution = GetScaleFromCurrentResolution(resolution_x, resolution_y);

	bool flag = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (m_map[i][j] == '#')
			{
				sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
				sprite.setScale(1 - scale_from_resolution.x, 1 - scale_from_resolution.y);
				sprite.setPosition(j * int(100.0f - scale_from_resolution.x * 100), i * int(100.0f - scale_from_resolution.y * 100)); //задает каждому квадрату свою позицию, иначе вся карта нарисуется в одном квадрате и будет только один квадрат
				if (flag == 1)
				{
					sprite.setColor(sf::Color(239, 228, 176));
					flag = 0;
				}
				else
				{
					sprite.setColor(sf::Color(167, 214, 56));
					flag = 1;
				}
				window.draw(sprite); //рисование карты
			}
		}
	}
}

sf::Vector2i Map::GetSizeMap() const
{
	sf::Vector2i size_map;
	size_map.x = m_map[0].length();
	size_map.y = sizeof(m_map) / sizeof(m_map[0]);

	return size_map;
}

sf::IntRect Map::GetSizeRect() const
{
	sf::IntRect size = sprite.getTextureRect(); //тут косяк, должно вернуть 96
	return size;
}