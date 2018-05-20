#include "camera.h"
#include <thread>
#include <iostream>

void Camera::FollowCharacter_DontGoBeyond(const sf::Window & window, sf::Vector2f position)
{
	sf::Vector2f pos = position;

	int left = 960; //размер игрового окна по диагонали = 1920 / 2 = 960
	int top = 540; //размер игрового окна по вертикали = 1080 / 2 = 540
	int right = 2304; //ширина карты = 25, размер спрайта карты 96х96 и того, максимальная длинна карты = 2304
	int down = 1920; //высота карты = 20, размер спрайта карты 96х96 и того, максимальная высота карты = 1920

	if (position.x < size_x / 2) //левый край
		pos.x = left; //(x) меняем, а (у) оставляем какой был
	if (position.y < size_y / 2) //верхний край
		pos.y = top; //(x) оставляем какой был, а (у) меняем
	if (position.x > right - size_x / 2) //правый край
		pos.x = right - size_x / 2; //(x) меняем, а (у) оставляем какой был
	if (position.y > down - size_y / 2) //нижний край
		pos.y = down - size_y / 2; //(x) оставляем какой был, а (у) меняем

	setCenter(pos.x, pos.y); //результирующие координаты для камеры
}

void Camera::ZoomIncrease()
{
	if (m_zoom < 180)
	{
		m_zoom += 10.0f;
		zoom(0.9302f);
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
	}
}

void Camera::ZoomDecrease()
{
	if (m_zoom > 30)
	{
		m_zoom -= 10.0f;
		zoom(1.075f);
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
	}
}

sf::Vector2i Camera::GetSize() const
{
	sf::Vector2i size_camera;
	size_camera.x = size_x;
	size_camera.y = size_y;
	return size_camera;
}

