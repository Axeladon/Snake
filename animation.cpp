#include "animation.h"

void Animation::RunAnimation(sf::Sprite & sprite, int rectLeft, int rectTop, int rectWidth, int rectHeighr, int max_frame)
{
	m_CurFrame = m_CurFrame + m_SpeedAnimation;
	if (int(m_CurFrame) >= max_frame)
	{
		m_CurFrame = NULL;
	}
	sprite.setTextureRect(sf::IntRect(rectLeft + (rectWidth * int(m_CurFrame)), rectTop, rectWidth, rectHeighr)); //first w - size of rect
}