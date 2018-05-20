#ifndef _HERO_
#define _HERO_
#include "SFML\Graphics.hpp"
#include "animation.h"
#include <string>

class Hero : public sf::Image, public sf::Texture, public sf::Sprite, public Animation
{
private:
	sf::Image m_image;
	sf::Texture m_texture;

	float m_speed;
	float m_scale;
	sf::Vector2i head; //��������� �� ������ ����
	sf::Vector2i tail; //�� �����

	char n_bodypart_near_head;
	char n_bodypart_near_tail;

public:
	Hero();
	~Hero();
	void SetImageFromFile(const std::string & img_path); //������ ���� � �������� ��� ��������

	void SetTextureRect(int x, int y, int w, int h); //������ ������������� �� ���� ��������

	void SetSpeed(float speed) { m_speed = speed; }
	float GetSpeed() const { return m_speed; }
	void AddSpeed(float value) { m_speed -= value; }

	void SetHeadPointer(int x, int y);
	sf::Vector2i GetHeadPointer() const { return head; }
	void SetTailPointer(int x, int y);
	sf::Vector2i GetTailPointer() const { return tail; }

	void SetNumOfBodyPartNearHead(const char & num); //�������� ���������� ����� ������, ����� ��� CreateBodyPart()
	void SetNumOfBodyPartNearTail(const char & num); //�������� ���������� ����� ������, ����� ��� GetBodyPartNearTail()

	char CreateBodyPart(); //������� ���������� �����, ������� ����� ��� �������� ���� ���� (���������� �������������)
	char GenerateNumOfBodyPartToCheck(); //������� ���������� �����, ������� ����� ��� �������� (��� ���� ����� ���� ���� � ���������� �����)
	char GetBodyPartNearTail();
};

inline Hero::Hero()
{
	m_speed = 1;
}

inline Hero::~Hero()
{

}

#endif
