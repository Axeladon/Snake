#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>

sf::Vector2f GetPercentScaleFromCurrentResolution(int resolution_x, int resolution_y)
{
	float coefficient_x = resolution_x / 1920.0f;
	float coefficient_y = resolution_y / 1080.0f;

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

/*
coefficient_x = 1 - coefficient_x;
coefficient_y = 1 - coefficient_y;
*/