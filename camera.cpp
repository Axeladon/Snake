#include "camera.h"
#include <thread>
#include <iostream>

void Camera::FollowCharacter_DontGoBeyond(const sf::Window & window, sf::Vector2f position)
{
	sf::Vector2f pos = position;

	int left = 960; //������ �������� ���� �� ��������� = 1920 / 2 = 960
	int top = 540; //������ �������� ���� �� ��������� = 1080 / 2 = 540
	int right = 2304; //������ ����� = 25, ������ ������� ����� 96�96 � ����, ������������ ������ ����� = 2304
	int down = 1920; //������ ����� = 20, ������ ������� ����� 96�96 � ����, ������������ ������ ����� = 1920

	if (position.x < size_x / 2) //����� ����
		pos.x = left; //(x) ������, � (�) ��������� ����� ���
	if (position.y < size_y / 2) //������� ����
		pos.y = top; //(x) ��������� ����� ���, � (�) ������
	if (position.x > right - size_x / 2) //������ ����
		pos.x = right - size_x / 2; //(x) ������, � (�) ��������� ����� ���
	if (position.y > down - size_y / 2) //������ ����
		pos.y = down - size_y / 2; //(x) ��������� ����� ���, � (�) ������

	setCenter(pos.x, pos.y); //�������������� ���������� ��� ������
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

