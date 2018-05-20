#ifndef _CAMERA_
#define _CAMERA_
#include "SFML\Graphics.hpp"

class Camera : public sf::View
{
	int size_x;
	int size_y;
	float m_zoom;
public:
	void FollowCharacter_DontGoBeyond(const sf::Window & window, sf::Vector2f position); //следит за персонажем и не выходит за границы
	Camera(int rect_x, int rect_y);
	void ZoomIncrease();
	void ZoomDecrease();
	sf::Vector2i GetSize() const;
};

inline Camera::Camera(int rect_x = 500, int rect_y = 500)
{
	m_zoom = 100;
	size_x = rect_x;
	size_y = rect_y;
	reset(sf::FloatRect(0, 0, (float)rect_x, (float)rect_y));
}
#endif