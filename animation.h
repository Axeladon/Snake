#ifndef _ANIMATION_
#define _ANIMATION_
#include "SFML\Graphics.hpp"

class Animation
{
	float m_CurFrame; //need to normal animation work
	float m_SpeedAnimation;
	int m_dir;
public:
	Animation();
	enum direction { left, right, up, down }; //помощь при программировании (для выбора строн) используется в RunAnimation()
	void RunAnimation(sf::Sprite & sprite, int rectLeft, int rectTop, int rectWidth, int rectHeighr, int max_frame); //запускает анимацию
	void SetSpeedAnimation(float speed) { m_SpeedAnimation = speed; }
	void SetDirection(int direction) { m_dir = direction; } //указывает направление для анимации, в enum direction
	int GetDirection() const { return m_dir; }
};

inline Animation::Animation()
{
	m_CurFrame = 0;
	m_SpeedAnimation = 0.5;
	m_dir = 0;
}

#endif