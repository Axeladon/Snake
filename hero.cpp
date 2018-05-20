#include "hero.h"
#include <iostream>

void Hero::SetImageFromFile(const std::string & img_path)
{
	m_image.loadFromFile("images//" + img_path);
	m_texture.loadFromImage(m_image);
	setTexture(m_texture);
}

void Hero::SetTextureRect(int x, int y, int w, int h)
{
	setTextureRect(sf::IntRect(x, y, w, h));
}

void Hero::SetHeadPointer(int x, int y)
{
	head.x = y;
	head.y = x;
}

void Hero::SetTailPointer(int x, int y)
{
	tail.x = y;
	tail.y = x;
}

void Hero::SetNumOfBodyPartNearHead(const char & num)
{ 
	n_bodypart_near_head = num;
}

void Hero::SetNumOfBodyPartNearTail(const char & num)
{
	n_bodypart_near_tail = num;
}

char Hero::GetBodyPartNearTail()
{
	return n_bodypart_near_tail;
}


char Hero::GenerateNumOfBodyPartToCheck()
{
	char digits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '!', '@', '#', '$', '^', '&', '*', '(', ')', '_', '+', '=', '-', '}', '{', '[', ']', ':',
		';', '"', '<', '>', '?', '/', '\\', '\'', '|', '.', ',', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z',
		'x', 'c', 'v', 'b', 'n', 'm', 'Q', 'W', 'E', 'R', 'Y', 'U', 'I', 'O', 'P', 'S', 'D', 'F', 'G', 'J', 'K', 'L' };

	int max = sizeof(digits) - 1;
	char temp = n_bodypart_near_tail;

	for (int i = max; i > 0; i--)
	{
		if (n_bodypart_near_tail == digits[i] && n_bodypart_near_tail != digits[0])
		{
			n_bodypart_near_tail = digits[i - 1];
			return temp;
		}
		else if (n_bodypart_near_tail == digits[0])
		{
			n_bodypart_near_tail = digits[max];
			return temp;
		}
	}
}

char Hero::CreateBodyPart()
{
	char digits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '!', '@', '#', '$', '^', '&', '*', '(', ')', '_', '+', '=', '-', '}', '{', '[', ']', ':', 
		';', '"', '<', '>', '?', '/', '\\', '\'', '|', '.', ',', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z', 
		'x', 'c', 'v', 'b', 'n', 'm', 'Q', 'W', 'E', 'R', 'Y', 'U', 'I', 'O', 'P', 'S', 'D', 'F', 'G', 'J', 'K', 'L' };

	int max = sizeof(digits) - 1;

	for (int i = max; i > 0; i--)
	{
		if (n_bodypart_near_head == digits[i] && n_bodypart_near_head != digits[0])
		{
			n_bodypart_near_head = digits[i - 1];
			return digits[i - 1];
		}
		else if (n_bodypart_near_head == digits[0])
		{
			n_bodypart_near_head = digits[max];
			return digits[max];
		}
	}
}