#include <SFML/Graphics.hpp>
#include "hero.h"
#include "map.h"
#include "animation.h"
#include <iostream>
#include <string>
#include <time.h>
#include <chrono>
#include <thread>
#include "Resolution.h"
#include <fstream>
#include "Main.h"


#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#define MAX_WIDTH 16
#define MAX_HEIGHT 8

Resolution resol; //����� ����������

bool LoadMenu();
bool LoadGame();
void SettingsGame(sf::RenderWindow &);
void SetAllToZero(); //�������� ��� ���������� ����������
void LoadPause(sf::RenderWindow & this_window);
bool LoadControls(sf::RenderWindow & menu);

void keyboard(Hero & hero, std::string * snake_map);
void moving(Hero & hero, std::string * snake_map, char ** snake_body);
void draw_snake(sf::RenderWindow & window, Hero & hero, std::string * snake_map, char ** snake_body);
void apple_generation(std::string * snake_map);
void addScoreAndSpeed(Hero & hero); //���� ����� ������, ��������� ���� � �������� ��������
bool GameOver(sf::RenderWindow & window);
void readFromFile();
void writeToFile(std::string str);

bool width = false, height = true, update = true; //������ �� �������� � ��������������� �������
int my_score = 0;
int dir; //��� ���, ���������� ������ �� ����, �������� ������ ��� ���������� ��� ������������
bool m_exit = false;
bool l = false; bool r = true; bool down = false; bool up = false; //���������� ������� Moving

struct Apple
{
	int x;
	int y;
}apple;

int main()
{
	readFromFile(); //������ �� �����, ����� ����� ���� ���������� �������
	if (!LoadMenu())
		return 0;
}

bool LoadMenu()
{
	sf::RenderWindow menu(sf::VideoMode(400, 500), "Snake", sf::Style::Close);

	sf::Image icon;
	icon.loadFromFile("images\\bmp_snake.bmp");
	menu.setIcon(32, 32, icon.getPixelsPtr());

	sf::Image img_snake;
	img_snake.loadFromFile("images\\img_snake.png");
	img_snake.createMaskFromColor(sf::Color(153, 217, 234));
	sf::Texture tex_snake;
	tex_snake.loadFromImage(img_snake);
	sf::Sprite spr_snake;
	spr_snake.setTexture(tex_snake);
	spr_snake.setPosition(155, 50);
	spr_snake.setScale(0.43f, 0.43f);

	sf::Font main_font;
	main_font.loadFromFile("fonts\\kirsty rg.ttf");
	sf::Text text_one;
	text_one.setFont(main_font);
	text_one.setString("start");
	text_one.setPosition(175, 230);
	text_one.setCharacterSize(24);

	sf::Text text_two;
	text_two = text_one;
	text_two.setString("options");
	text_two.setPosition(164, 280);

	sf::Text text_three;
	text_three = text_one;
	text_three.setString("controls");
	text_three.setPosition(157, 330);

	sf::Text text_four;
	text_four = text_one;
	text_four.setString("quit");
	text_four.setPosition(182, 380);

	sf::Font nickname_font;
	nickname_font.loadFromFile("fonts\\TRIBAL__.ttf");
	sf::Text nickname_text;
	nickname_text.setFont(nickname_font);
	nickname_text.setString("Axeladon");
	nickname_text.setPosition(285, 480);
	nickname_text.setCharacterSize(16);
	nickname_text.setColor(sf::Color::Red);

	while (menu.isOpen())
	{
		sf::Event event;
		while (menu.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				menu.close();
		}
		text_one.setFillColor(sf::Color::White); //��������� ���� �� ���������
		text_two.setFillColor(sf::Color::White); //��������� ���� �� ���������
		text_three.setFillColor(sf::Color::White); //��������� ���� �� ���������
		text_four.setFillColor(sf::Color::White); //��������� ���� �� ���������

		if (sf::IntRect(170, 233, 64, 21).contains(sf::Mouse::getPosition(menu)))
		{
			text_one.setFillColor(sf::Color(27, 105, 3));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				menu.close(); //��������� ���� ����
				LoadGame(); //��������� ���� ����
			}
		}
		else if (sf::IntRect(165, 283, 80, 21).contains(sf::Mouse::getPosition(menu)))
		{
			text_two.setFillColor(sf::Color(27, 105, 3));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				menu.setVisible(0);
				std::this_thread::sleep_for(std::chrono::milliseconds(400));
				SettingsGame(menu);
			}
		}
		else if (sf::IntRect(168, 333, 75, 21).contains(sf::Mouse::getPosition(menu)))
		{
			text_three.setFillColor(sf::Color(27, 105, 3));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				menu.setVisible(0);
				std::this_thread::sleep_for(std::chrono::milliseconds(400));
				LoadControls(menu);
			}
		}
		else if (sf::IntRect(181, 383, 46, 21).contains(sf::Mouse::getPosition(menu)))
		{
			text_four.setFillColor(sf::Color(27, 105, 3));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				return 0;
			}
		}

		menu.clear(sf::Color(117, 154, 105));
		menu.draw(spr_snake);
		menu.draw(text_one);
		menu.draw(text_two);
		menu.draw(text_three);
		menu.draw(text_four);
		menu.draw(nickname_text);
		menu.display();
	}
	return 0;
}

bool LoadGame()
{
	SetAllToZero(); //�������� ����������

	srand((unsigned int)time(NULL)); //������ ������

	float my_timer = 0;

	sf::Vector2i resolution_window; //�������� ���������� ������
	resolution_window = resol.GetIntRecolution();

	sf::RenderWindow window(sf::VideoMode(resolution_window.x, resolution_window.y), "Snake", sf::Style::Close);
	window.setFramerateLimit(60);
	window.setMouseCursorVisible(false);
	window.setPosition(sf::Vector2i(-10, 0));

	sf::Image icon;
	icon.loadFromFile("images\\bmp_snake.bmp");
	window.setIcon(32, 32, icon.getPixelsPtr());

	//��������� �������� ��� ������� ���������� ������
	sf::Vector2f scale_from_resolution = GetPercentScaleFromCurrentResolution(resolution_window.x, resolution_window.y);
	if (scale_from_resolution.x == 0)//�������, ��� ���������� ������ � snake.setScale(1.48f * scale_from_resolution.x, 1.48f * scale_from_resolution.y);
	{
		scale_from_resolution.x = 1;
		scale_from_resolution.y = 1;
	}

	Hero snake; //main character
	snake.SetImageFromFile("snake.png");
	snake.setOrigin(10, 10);
	snake.setScale(1.56f * scale_from_resolution.x, 1.56f * scale_from_resolution.y);
	snake.SetDirection(snake.direction::right); //����������� ��� �������� �� ������
	snake.SetSpeed(0.4f);

	std::string snake_map[9] =
	{
		"                 ",
		"                 ",
		"                 ",
		"                 ",
		"     T321H       ",
		"                 ",
		"                 ",
		"                 ",
		"                 ",
	};
	snake.SetHeadPointer(5 - 1, 10 - 1);
	snake.SetTailPointer(5 - 1, 6 - 1); //������ ���������� ���������� ������� ���� ����

	snake.SetNumOfBodyPartNearHead('1'); //������ �������� ����� ����� #
	snake.SetNumOfBodyPartNearTail('3');

	//��������� ������ ��� ��������� ���� ���� (����...��� �������� ����� �����)
	char* snake_body[9];
	for (int i = 0; i < 9; i++) {
		snake_body[i] = new char[17];
	}

	for (int i = 0; i < 9; i++) //�������������
		for (int j = 0; j < 17; j++)
			snake_body[i][j] = ' ';
	snake_body[4][6] = '-';
	snake_body[4][7] = '-';
	snake_body[4][8] = '-';
	//����� �������

	apple_generation(snake_map); //���������� ������ ������

								 //----------------------------text--------------------------------------
	sf::Font font;
	font.loadFromFile("fonts\\Counterproductive.ttf");

	sf::Text str_score("Score: ", font, int(120 * scale_from_resolution.x));
	str_score.setFillColor(sf::Color::Yellow);
	str_score.setPosition(280 * scale_from_resolution.x, -30);//������ ������� ������

	sf::Text n_score(" ", font, int(120 * scale_from_resolution.x));
	n_score.setPosition(615 * scale_from_resolution.x, -25);
	n_score.setFillColor(sf::Color::Yellow);

	sf::Text str_time("Time: ", font, int(120 * scale_from_resolution.x));
	str_time.setPosition(1300 * scale_from_resolution.x, -25);

	sf::Text n_time(" ", font, int(120 * scale_from_resolution.x));
	n_time.setPosition(1600 * scale_from_resolution.x, -25);

	//---------------------------map----------------------
	Map map;
	map.LoadMapFromFile("rect 100x100.png");
	map.SetSizeResolution(resolution_window.x, resolution_window.y);

	sf::Clock clock1; //�������� ������� ����, ��� ������� �������� (��������) ����
	sf::Time cheak_clock; //�������� ������� �����, ��� ��������� ����� � ���������� �������� (����� ��� �������� �������� ����)

	sf::Clock clock2; //���������� ������� ���� ��� ������ ���������� �������
	sf::Time timer;

	dir = snake.GetDirection(); //������������� �����������

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		keyboard(snake, snake_map);
		LoadPause(window);

		//��������� �����
		timer = clock2.getElapsedTime(); //���������� ��������� ����� � ���������� ������� Time
		if (timer >= sf::seconds(1)) //���� ���� ������� ��� ������ ��...
		{
			my_timer += 1; //+1 ������� �������
			clock2.restart(); //�������� ��������� �����
		}

		//�������� ����
		if ((cheak_clock = clock1.getElapsedTime()) >= sf::seconds(snake.GetSpeed()))
		{
			moving(snake, snake_map, snake_body);
			clock1.restart();
			dir = snake.GetDirection();
			update = true; //���� �� �������������, �� ����� ������� �����������
		}

		if (GameOver(window)) //�������, ������� �������� �� ��, ��� ����� ����� ������������
		{
			//return 0;
		}

		window.clear(sf::Color(71, 117, 45));

		map.DrawMap(window);

		draw_snake(window, snake, snake_map, snake_body);

		window.draw(str_score); //��������� score
		n_score.setString(std::to_string(my_score)); //������ ������ � ������� ����
		window.draw(n_score); //�������� ������� ����

		window.draw(str_time); //�������� ����� Time:
		n_time.setString(std::to_string(int(my_timer))); //������ ����� � ����������
		window.draw(n_time); //�������� ����� Timr

		window.display();
	}
	return 0;
}

void SettingsGame(sf::RenderWindow & menu)
{
	sf::RenderWindow settings(sf::VideoMode(700, 400), " ", sf::Style::None);

	sf::Font main_font; //font
	main_font.loadFromFile("fonts\\kirsty rg.ttf");

	sf::Text text_one; //text
	text_one.setFont(main_font);
	text_one.setString("game settings");
	text_one.setPosition(255, 80);
	text_one.setCharacterSize(32);
	text_one.setFillColor(sf::Color::Black);

	sf::Text text_exit; //text
	text_exit.setFont(main_font);
	text_exit.setString("back");
	text_exit.setPosition(317, 310);
	text_exit.setCharacterSize(28);
	text_exit.setFillColor(sf::Color::Black);

	sf::Text text_format; //����� ��� ������ ���������� ������
	text_format.setFont(main_font);
	text_format.setString(resol.GetStringResolution()); //��������� ���������� ����������
	text_format.setPosition(280, 200);
	text_format.setCharacterSize(24);
	text_format.setFillColor(sf::Color::White);

	sf::Image image;
	image.loadFromFile("images\\settings_lines.png");
	sf::Texture texture;
	texture.loadFromImage(image);
	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setPosition(125, 200);
	sprite.setTextureRect(sf::IntRect(0, 0, 450, 28));

	sf::Image main_img;
	main_img.loadFromFile("images\\Controls.jpg");
	main_img.createMaskFromColor(sf::Color(153, 217, 234));
	sf::Texture main_texture;
	main_texture.loadFromImage(main_img);
	sf::Sprite main_sprite;
	main_sprite.setTexture(main_texture);

	while (settings.isOpen())
	{
		sf::Event event;
		while (settings.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				settings.close();
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
				LoadMenu();
			}
		}

		if (sf::IntRect(321, 309, 55, 26).contains(sf::Mouse::getPosition(settings)))
		{
			text_exit.setFillColor(sf::Color(27, 105, 3));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				settings.close(); //��������� ���� ���������
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
				menu.setVisible(1);
			}
		}
		else if (sf::IntRect(125, 200, 26, 27).contains(sf::Mouse::getPosition(settings)))
		{
			sprite.setTextureRect(sf::IntRect(0, 29, 450, 28));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				resol.offset_left();
				writeToFile(resol.GetStringResolution());
				text_format.setString(resol.GetStringResolution()); //��������� ���������� ����������
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}
		}
		else if (sf::IntRect(550, 200, 26, 27).contains(sf::Mouse::getPosition(settings)))
		{
			sprite.setTextureRect(sf::IntRect(0, 58, 450, 28));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				resol.offset_right();
				writeToFile(resol.GetStringResolution());
				text_format.setString(resol.GetStringResolution()); //��������� ���������� ����������
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}
		}
		else //default
		{
			text_exit.setFillColor(sf::Color::Black);
			sprite.setTextureRect(sf::IntRect(0, 0, 450, 28));
		}


		settings.clear();
		settings.draw(main_sprite);
		settings.draw(sprite);
		settings.draw(text_one);
		settings.draw(text_exit);
		settings.draw(text_format);
		settings.display();
	}
}

bool LoadControls(sf::RenderWindow & menu)
{
	sf::RenderWindow control(sf::VideoMode(700, 400), " ", sf::Style::None);

	sf::Font main_font; //font
	main_font.loadFromFile("fonts\\kirsty rg.ttf");

	sf::Text main_text;
	main_text.setFont(main_font);
	main_text.setString("game controls");
	main_text.setPosition(255, 50);
	main_text.setCharacterSize(32);
	main_text.setFillColor(sf::Color::Black);

	sf::Image main_img;
	main_img.loadFromFile("images\\Controls1.jpg");
	main_img.createMaskFromColor(sf::Color(153, 217, 234));
	sf::Texture main_texture;
	main_texture.loadFromImage(main_img);
	sf::Sprite main_sprite;
	main_sprite.setTexture(main_texture);

	sf::Text text_exit; //text
	text_exit.setFont(main_font);
	text_exit.setString("back");
	text_exit.setPosition(330, 310);
	text_exit.setCharacterSize(28);
	text_exit.setFillColor(sf::Color::Black);

	while (control.isOpen())
	{
		sf::Event event;
		while (control.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				control.close();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				LoadMenu();
			}
		}

		if (sf::IntRect(334, 320, 55, 26).contains(sf::Mouse::getPosition(control)))
		{
			text_exit.setFillColor(sf::Color(27, 105, 3));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				control.close(); //��������� ���� ���������
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
				menu.setVisible(1);
			}
		}
		else //default
		{
			text_exit.setFillColor(sf::Color::Black);

		}
		control.clear();
		control.draw(main_sprite);
		control.draw(main_text);
		control.draw(text_exit);
		control.display();
	}
}

void LoadPause(sf::RenderWindow & this_window)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		this_window.setMouseCursorVisible(true);
		//��������� �������� ��� ������� ���������� ������
		sf::Vector2i resolution_window; //�������� ���������� ������
		resolution_window = resol.GetIntRecolution();
		sf::Vector2f scale_from_resolution = GetPercentScaleFromCurrentResolution(resolution_window.x, resolution_window.y);
		float scale_coeff;
		if (scale_from_resolution.x == 0)
			scale_coeff = 1;
		else
			scale_coeff = scale_from_resolution.x;

		sf::Image img;
		img.loadFromFile("images\\pause.png");
		sf::Texture tex;
		tex.loadFromImage(img);
		sf::Sprite sprite;
		sprite.setTexture(tex);

		//��������� ������� ���� (��� ������� �� ���� ���������), ����� ���������� � ������
		sf::Vector2u win_size = this_window.getSize();
		sf::Vector2u image_size = tex.getSize();
		int x = (win_size.x - image_size.x) / 2;
		int y = (win_size.y - image_size.y) / 2;
		sprite.setPosition(float(x), float(y));

		//����� + ������� � ��� ��� � ���� �������
		sf::Font main_font;
		main_font.loadFromFile("fonts\\kirsty rg.ttf");

		sf::Text text_one;
		text_one.setFont(main_font);
		text_one.setString("resume");
		text_one.setFillColor(sf::Color::Red); //���� ������ �� ���������
		text_one.setPosition(870 * scale_coeff, 400 * scale_coeff);
		text_one.setCharacterSize(int(60 * scale_coeff));

		sf::Text text_two;
		text_two.setFont(main_font);
		text_two.setString("back to main menu");
		text_two.setFillColor(sf::Color::Blue);
		text_two.setPosition(750 * scale_coeff, 500 * scale_coeff);
		text_two.setCharacterSize(int(60 * scale_coeff));

		sf::Text text_three;
		text_three.setFont(main_font);
		text_three.setString("exit");
		text_three.setFillColor(sf::Color::Blue);
		text_three.setPosition(915 * scale_coeff, 600 * scale_coeff);
		text_three.setCharacterSize(int(60 * scale_coeff));

		enum PauseButtons { Resume = 1, Menu, Exit };
		int pause_button = PauseButtons::Resume; //������. �� ��������� ������� ������ (����������)
		int was_update = PauseButtons::Resume; //���������� ��� ��������, ����� �� �������� ��� ��� �� �������

		this_window.draw(sprite);
		this_window.draw(text_one);
		this_window.draw(text_two);
		this_window.draw(text_three);
		this_window.display(); // ��������� �� ���������

		while (1)
		{
			if (sf::IntRect(int(870 * scale_coeff), int(420 * scale_coeff), int(193 * scale_coeff), int(40 * scale_coeff)).contains(sf::Mouse::getPosition(this_window)))
			{
				pause_button = PauseButtons::Resume;
			}
			else if (sf::IntRect(int(750 * scale_coeff), int(520 * scale_coeff), int(710 * scale_coeff), int(40 * scale_coeff)).contains(sf::Mouse::getPosition(this_window)))
			{
				pause_button = PauseButtons::Menu;
			}
			else if (sf::IntRect(int(916 * scale_coeff), int(620 * scale_coeff), int(105 * scale_coeff), int(40 * scale_coeff)).contains(sf::Mouse::getPosition(this_window)))
			{
				pause_button = PauseButtons::Exit;
			}

			if (was_update != pause_button) //���� ���-�� ����������
			{
				if (pause_button == PauseButtons::Resume)
				{
					text_one.setFillColor(sf::Color::Red);
					text_two.setFillColor(sf::Color::Blue);
					text_three.setFillColor(sf::Color::Blue);
				}
				else if (pause_button == PauseButtons::Menu)
				{
					text_one.setFillColor(sf::Color::Blue);
					text_two.setFillColor(sf::Color::Red);
					text_three.setFillColor(sf::Color::Blue);
				}
				else if (pause_button == PauseButtons::Exit)
				{
					text_one.setFillColor(sf::Color::Blue);
					text_two.setFillColor(sf::Color::Blue);
					text_three.setFillColor(sf::Color::Red);
				}
				was_update = pause_button; //���������, ��� ��� ��������� � ����� ����� ���������� ����������
				
				this_window.draw(sprite);
				this_window.draw(text_one);
				this_window.draw(text_two);
				this_window.draw(text_three);
				this_window.display(); //��������� ����� ���������
			}

			//��� ���������� �������� �� ���������� (������������ � ����)
			if (pause_button == PauseButtons::Resume && (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
			{
				pause_button = PauseButtons::Menu;
				std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(200));
			}
			else if (pause_button == PauseButtons::Resume && (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)))
			{
				pause_button = PauseButtons::Exit;
				std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(200));
			}
			else if (pause_button == PauseButtons::Menu && (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)))
			{
				pause_button = PauseButtons::Resume;
				std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(200));
			}
			else if (pause_button == PauseButtons::Menu && (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
			{
				pause_button = PauseButtons::Exit;
				std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(200));
			}
			else if (pause_button == PauseButtons::Exit && (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)))
			{
				pause_button = PauseButtons::Menu;
				std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(200));
			}
			else if (pause_button == PauseButtons::Exit && (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
			{
				pause_button = PauseButtons::Resume;
				std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(200));
			}

			//��� ��������� ������...Enter or LM
			if (pause_button == PauseButtons::Resume && (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) 
				|| sf::Mouse::isButtonPressed(sf::Mouse::Left))) //if select first key and pressed Enter or LM
			{
				this_window.setMouseCursorVisible(false);
				return;
			}
			else if (pause_button == PauseButtons::Menu && (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)
				|| sf::Mouse::isButtonPressed(sf::Mouse::Left))) //if select second key and pressed Enter or LM
			{
				this_window.close();
				LoadMenu();
				return;
			}
			else if (pause_button == PauseButtons::Exit && (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)
				|| sf::Mouse::isButtonPressed(sf::Mouse::Left))) //if select second key and pressed Enter or LM
			{
				this_window.close();
				return;
			}
		}

	}
}

void SetAllToZero()
{
	width = false, height = true, update = true; //������ �� �������� � ��������������� �������
	my_score = 0;
	//dir = 3;
	m_exit = false;
	l = false; r = true; down = false; up = false;
}

void readFromFile()
{
	std::string line;

	std::string temp;
	std::ifstream myfile("system_files\\resolution.txt");
	if (myfile.is_open())
	{
		while (getline(myfile, temp))
		{
			line = temp; //�������� ���������� � ����� � ����, �.�. ������ � ����� ������� (�� ���� ������)
		}
		myfile.close();
	}

	int digits = line.find_first_of("x"); //����� ������ ��� (�� ����� ������������ ������ � ������)
	if (digits <= int(line.size())) //���� ���e��� "�"
	{
		std::string m_before;
		for (int i = 0; i < digits; i++) //�������� ����� �� ������ �� ����
		{
			m_before += line[i];
		}
		std::string m_after;
		for (int i = digits + 1; i < int(line.length()); i++) //�������� ����� �� ������ ����� ����
		{
			m_after += line[i];
		}
		resol.SetResolution(atoi(m_before.c_str()), atoi(m_after.c_str())); //��������������� ������� � ����
	}
}

void writeToFile(std::string str)
{
	std::ofstream myfile;
	myfile.open("system_files\\resolution.txt");
	myfile << str;
	myfile.close();
}

void keyboard(Hero & hero, std::string * snake_map)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		if (width && update)
		{
			hero.SetDirection(hero.direction::right);
			height = true;
			width = false;
			update = false;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		if (width && update)
		{
			hero.SetDirection(hero.direction::left);
			height = true;
			width = false;
			update = false;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if (height && update)
		{
			hero.SetDirection(hero.direction::up);
			width = true;
			height = false;
			update = false;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		if (height && update)
		{
			hero.SetDirection(hero.direction::down);
			width = true;
			height = false;
			update = false;
		}
	}
}

void moving(Hero & hero, std::string * snake_map, char ** snake_body)
{
	sf::Vector2i head = hero.GetHeadPointer(); //��������� ��������� ������ ����
	sf::Vector2i tail = hero.GetTailPointer(); //��������� ��������� ���� ����

	bool ifate = true;
	if (hero.GetDirection() == hero.direction::up)
	{
		if (head.y > 0) //���� �� ���� �����
		{
			if (snake_map[head.y - 1][head.x] == ' ' || snake_map[head.y - 1][head.x] == 'A') //��������, ���� "������" ������ ��� � �������
			{
				if (snake_map[head.y - 1][head.x] == 'A') //���� ���� ������
				{
					apple_generation(snake_map); //���������� ����� ������
					addScoreAndSpeed(hero); //���� ������ �����, ������ �������� 
					ifate = false;
				}
				snake_map[head.y - 1][head.x] = 'H'; //������ ����� ���������
				hero.SetHeadPointer(head.y - 1, head.x); //���� ��������� ����� ��������� ������
			}
			else
				m_exit = true;
		}
		else
		{
			if (snake_map[MAX_HEIGHT][head.x] == ' ' || snake_map[MAX_HEIGHT][head.x] == 'A') //�������� �� ��, ����� ���� �� ������ ���� �� ����
			{
				if (snake_map[MAX_HEIGHT][head.x] == 'A') //��������, ���� "������" ������ ��� � �������
				{
					apple_generation(snake_map); //���������� ����� ������
					addScoreAndSpeed(hero); //���� ������ �����, ������ �������� 
				}
				snake_map[MAX_HEIGHT][head.x] = 'H'; //������ ����� ���������
				hero.SetHeadPointer(MAX_HEIGHT, head.x); //�������� ���������� ������ ���������
			}
			else
				m_exit = true;
		}

		//����, ��� ��� � ����� ��� ��������� ���� ��������� ����
		if (l || r) //���� ���� ��������� � ���� ��� �����, ��:
		{
			up = true;
			if (r) //���� � �����
			{
				snake_body[head.y][head.x] = '3'; //������ ��������� ������ -> �����
				r = false;
			}
			else if (l) //���� � ����
			{
				snake_body[head.y][head.x] = '1'; //������ ��������� ����� -> �����
				l = false;
			}
		}
		else
			snake_body[head.y][head.x] = '|'; //� ������� ���� ������ ������������ ���� �� ����� ������
	}
	else if (hero.GetDirection() == hero.direction::down)
	{
		if (head.y < MAX_HEIGHT)
		{
			if (snake_map[head.y + 1][head.x] == ' ' || snake_map[head.y + 1][head.x] == 'A') //�������� �� ��, ����� ���� �� ������ ���� �� ����
			{
				if (snake_map[head.y + 1][head.x] == 'A') //��������, ���� "������" ������ ��� � �������
				{
					apple_generation(snake_map); //���������� ����� ������
					addScoreAndSpeed(hero); //���� ������ �����, ������ �������� 
					ifate = false;
				}
				snake_map[head.y + 1][head.x] = 'H'; //������ ����� ���������
				hero.SetHeadPointer(head.y + 1, head.x);
			}
			else
				m_exit = true;
		}
		else
		{
			if (snake_map[0][head.x] == ' ' || snake_map[0][head.x] == 'A') //�������� �� ��, ����� ���� �� ������ ���� �� ����
			{
				if (snake_map[0][head.x] == 'A') //��������, ���� "������" ������ ��� � �������
				{
					apple_generation(snake_map); //���������� ����� ������
					addScoreAndSpeed(hero); //���� ������ �����, ������ �������� 
				}
				snake_map[0][head.x] = 'H'; //������ ����� ���������
				hero.SetHeadPointer(0, head.x);
			}
			else
				m_exit = true;
		}

		if (l || r) //���� ���� ��������� � ���� ��� �����, ��:
		{
			down = true;
			if (r) //���� � �����
			{
				snake_body[head.y][head.x] = '9'; //������ ��������� ������ -> �����
				r = false;
			}
			else if (l) //���� � ����
			{
				snake_body[head.y][head.x] = '7'; //������ ��������� ����� -> �����
				l = false;
			}
		}
		else
			snake_body[head.y][head.x] = '|'; //� ������� ���� ������ ������������ ���� �� ����� ������
	}
	else if (hero.GetDirection() == hero.direction::right)
	{
		if (head.x < MAX_WIDTH) //���� �� ���� �����
		{
			if (snake_map[head.y][head.x + 1] == ' ' || snake_map[head.y][head.x + 1] == 'A') //�������� �� ��, ����� ���� �� ������ ���� �� ����
			{
				if (snake_map[head.y][head.x + 1] == 'A') //��������, ���� "������" ������ ��� � �������
				{
					apple_generation(snake_map); //���������� ����� ������
					addScoreAndSpeed(hero); //���� ������ �����, ������ �������� 
					ifate = false; //����� ������ � ����� �� ������ ���������, �.�. ���������� ����� ���� ����
				}
				snake_map[head.y][head.x + 1] = 'H'; //������ ����� ��������� ������
				hero.SetHeadPointer(head.y, head.x + 1); //���� ��������� ����� ��������� ������
			}
			else
				m_exit = true;
		}
		else
		{
			if (snake_map[head.y][0] == ' ' || snake_map[head.y][0] == 'A') //�������� �� ��, ����� ���� �� ������ ���� �� ����
			{
				if (snake_map[head.y][0] == 'A') //��������, ���� "������" ������ ��� � �������
				{
					apple_generation(snake_map); //���������� ����� ������
					addScoreAndSpeed(hero); //���� ������ �����, ������ �������� 
				}
				snake_map[head.y][0] = 'H'; //������ ����� ���������
				hero.SetHeadPointer(head.y, 0); //������ ��������� �� ����� ���������
			}
			else
				m_exit = true;
		}

		if (up || down) //���� ���� ��������� � ���� ��� ���, ��:
		{
			r = true;
			if (up) //���� � �����
			{
				snake_body[head.y][head.x] = '7'; //������ ��������� ������ -> �����
				up = false;
			}
			else if (down) //���� � ����
			{
				snake_body[head.y][head.x] = '1'; //������ ��������� ����� -> �����
				down = false;
			}
		}
		else
			snake_body[head.y][head.x] = '-'; //� ������� ���� ������ ������������ ���� �� ����� ������
	}
	else if (hero.GetDirection() == hero.direction::left)
	{
		if (head.x > 0)
		{
			if (snake_map[head.y][head.x - 1] == ' ' || snake_map[head.y][head.x - 1] == 'A') //�������� �� ��, ����� ���� �� ������ ���� �� ����
			{
				if (snake_map[head.y][head.x - 1] == 'A') //��������, ���� "������" ������ ��� � �������
				{
					apple_generation(snake_map); //���������� ����� ������
					addScoreAndSpeed(hero); //���� ������ �����, ������ �������� 
					ifate = false;
				}
				snake_map[head.y][head.x - 1] = 'H'; //������ ����� ���������
				hero.SetHeadPointer(head.y, head.x - 1);
			}
			else
				m_exit = true;
		}
		else
		{
			if (snake_map[head.y][MAX_WIDTH] == ' ' || snake_map[head.y][MAX_WIDTH] == 'A') //�������� �� ��, ����� ���� �� ������ ���� �� ����
			{
				if (snake_map[head.y][MAX_WIDTH] == 'A') //��������, ���� "������" ������ ��� � �������
				{
					apple_generation(snake_map); //���������� ����� ������
					addScoreAndSpeed(hero); //���� ������ �����, ������ �������� 
				}
				snake_map[head.y][MAX_WIDTH] = 'H'; //������ ����� ���������
				hero.SetHeadPointer(head.y, MAX_WIDTH);
			}
			else
				m_exit = true;
		}

		if (up || down) //���� ���� ��������� � ���� ��� �����, ��:
		{
			l = true;
			if (up) //���� � �����
			{
				snake_body[head.y][head.x] = '9'; //������ ��������� ������ -> �����
				up = false;
			}
			else if (down) //���� � ����
			{
				snake_body[head.y][head.x] = '3'; //������ ��������� ����� -> �����
				down = false;
			}
		}
		else
			snake_body[head.y][head.x] = '-'; //� ������� ���� ������ ������������ ���� �� ����� ������
	}
	snake_map[head.y][head.x] = hero.CreateBodyPart(); //������ ������ ��������� ������ � �� ��� ����� �������� ���� ����

													   //������������ ������
	if (ifate)
	{
		char number_of_body = hero.GenerateNumOfBodyPartToCheck();
		if (tail.y > 0 && tail.x > 0 && tail.y < MAX_HEIGHT && tail.x < MAX_WIDTH) //���� ����� �� ������ ����
		{
			if (snake_map[tail.y][tail.x + 1] == number_of_body) //���� ���� ������ ������
			{
				snake_map[tail.y][tail.x + 1] = 'T'; //�������� �������������� ������ �� �����
				hero.SetTailPointer(tail.y, tail.x + 1); //�������� ��������� �� �����
			}
			else if (snake_map[tail.y][tail.x - 1] == number_of_body) //���� ���� ������ �����
			{
				snake_map[tail.y][tail.x - 1] = 'T'; //�������� �������������� ������ �� �����
				hero.SetTailPointer(tail.y, tail.x - 1); //�������� ��������� �� �����
			}
			else if (snake_map[tail.y - 1][tail.x] == number_of_body) //���� ���� ������ �����
			{
				snake_map[tail.y - 1][tail.x] = 'T'; //�������� �������������� ������ �� �����
				hero.SetTailPointer(tail.y - 1, tail.x); //�������� ��������� �� �����
			}
			else if (snake_map[tail.y + 1][tail.x] == number_of_body) //���� ���� ������ ����
			{
				snake_map[tail.y + 1][tail.x] = 'T'; //�������� �������������� ������ �� �����
				hero.SetTailPointer(tail.y + 1, tail.x); //�������� ��������� �� �����
			}
		}
		else
		{
			if (tail.y == 0 && tail.x == MAX_WIDTH && snake_map[tail.y][0] == number_of_body) //���� ����� ������ �������� ������� ���� � ���� ������ ������
			{
				snake_map[tail.y][0] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(tail.y, 0); //�������� ��������� �� �����
			}
			else if (tail.y == 0 && tail.x == MAX_WIDTH && snake_map[MAX_HEIGHT][tail.x] == number_of_body) //���� ����� ������ �������� ������� ���� � ���� ������ �����
			{
				snake_map[MAX_HEIGHT][tail.x] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(MAX_HEIGHT, tail.x); //�������� ��������� �� �����
			}
			else if (tail.y == 0 && tail.x == 0 && snake_map[MAX_HEIGHT][0] == number_of_body) //���� ����� ������ �������� ������ ���� � ���� ������ �����
			{
				snake_map[MAX_HEIGHT][0] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(MAX_HEIGHT, 0); //�������� ��������� �� �����
			}
			else if (tail.y == 0 && tail.x == 0 && snake_map[tail.y + 1][tail.x] == number_of_body) //���� ����� ������ �������� ������ ���� � ���� ������ ����
			{
				snake_map[tail.y + 1][tail.x] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(tail.y + 1, tail.x); //�������� ��������� �� �����
			}
			else if (tail.y == 0 && tail.x == 0 && snake_map[tail.y][MAX_WIDTH] == number_of_body) //���� ����� ������ �������� ������ ���� � ���� ������ �����
			{
				snake_map[tail.y][MAX_WIDTH] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(tail.y, MAX_WIDTH); //�������� ��������� �� �����
			}
			else if (tail.y == MAX_HEIGHT && tail.x == MAX_WIDTH && snake_map[tail.y][tail.x - 1] == number_of_body) //���� ����� ������ ������� ������� ���� � ���� ������ �����
			{
				snake_map[tail.y][tail.x - 1] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(tail.y, tail.x - 1); //�������� ��������� �� �����
			}
			else if (tail.y == MAX_HEIGHT && tail.x == MAX_WIDTH && snake_map[0][tail.x] == number_of_body) //���� ����� ������ ������� ������� ���� � ���� ������ ����
			{
				snake_map[0][tail.x] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(0, tail.x); //�������� ��������� �� �����
			}
			else if (tail.y == MAX_HEIGHT && tail.x == 0 && snake_map[0][tail.x] == number_of_body) //���� ����� ������ ������� ������ ���� � ���� ������ ����
			{
				snake_map[0][tail.x] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(0, tail.x); //�������� ��������� �� �����
			}
			else if (tail.y == 0 && snake_map[tail.y][tail.x + 1] == number_of_body) //���� ����� ������ �������� ���� � ���� ������ ������
			{
				snake_map[tail.y][tail.x + 1] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(tail.y, tail.x + 1); //�������� ��������� �� �����
			}
			else if (tail.y == 0 && snake_map[tail.y + 1][tail.x] == number_of_body) //���� ����� ������ �������� ���� � ���� ������ ����
			{
				snake_map[tail.y + 1][tail.x] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(tail.y + 1, tail.x); //�������� ��������� �� �����
			}
			else if (tail.y == 0 && snake_map[tail.y][tail.x - 1] == number_of_body) //���� ����� ������ �������� ���� � ���� ������ �����
			{
				snake_map[tail.y][tail.x - 1] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(tail.y, tail.x - 1); //�������� ��������� �� �����
			}
			else if (tail.x == MAX_WIDTH && snake_map[tail.y][0] == number_of_body) //���� ����� ������ ������� ���� � ���� ������ ������
			{
				snake_map[tail.y][0] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(tail.y, 0); //�������� ��������� �� �����
			}
			else if (tail.x == MAX_WIDTH && snake_map[tail.y - 1][tail.x] == number_of_body) //���� ����� ������ ������� ���� � ���� ������ �����
			{
				snake_map[tail.y - 1][tail.x] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(tail.y - 1, tail.x); //�������� ��������� �� �����
			}
			else if (tail.x == MAX_WIDTH && snake_map[tail.y + 1][tail.x] == number_of_body) //���� ����� ������ ������� ���� � ���� ������ ����
			{
				snake_map[tail.y + 1][tail.x] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(tail.y + 1, tail.x); //�������� ��������� �� �����
			}
			else if (tail.x == MAX_WIDTH && snake_map[tail.y][tail.x - 1] == number_of_body) //���� ����� ������ ������� ���� � ���� ������ � ����
			{
				snake_map[tail.y][tail.x - 1] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(tail.y, tail.x - 1); //�������� ��������� �� �����
			}
			else if (tail.x == 0 && snake_map[tail.y][tail.x + 1] == number_of_body) //���� ����� ������ ������ ���� � ���� ������ � �����
			{
				snake_map[tail.y][1] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(tail.y, 1); //�������� ��������� �� �����
			}
			else if (tail.x == 0 && snake_map[tail.y][MAX_WIDTH] == number_of_body) //���� ����� ������ ������ ���� � ���� ������ � ����
			{
				snake_map[tail.y][MAX_WIDTH] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(tail.y, MAX_WIDTH); //�������� ��������� �� �����
			}
			else if (tail.x == 0 && snake_map[tail.y - 1][tail.x] == number_of_body) //���� ����� ������ ������ ���� � ���� ������ �����
			{
				snake_map[tail.y - 1][tail.x] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(tail.y - 1, tail.x); //�������� ��������� �� �����
			}
			else if (tail.x == 0 && snake_map[tail.y + 1][tail.x] == number_of_body) //���� ����� ������ ������ ���� � ���� ������ ����
			{
				snake_map[tail.y + 1][tail.x] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(tail.y + 1, tail.x); //�������� ��������� �� �����
			}
			else if (snake_map[11 - 3][tail.x] == number_of_body) //���� ���� ������ �����
			{
				snake_map[11 - 3][tail.x] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(11 - 3, tail.x); //�������� ��������� �� �����
			}
			else if (tail.y == MAX_HEIGHT && snake_map[0][tail.x] == number_of_body) //���� ����� ������ ������� ���� � ���� ������ ����
			{
				snake_map[0][tail.x] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(0, tail.x); //�������� ��������� �� �����
			}
			else if (tail.y == MAX_HEIGHT && snake_map[tail.y - 1][tail.x] == number_of_body) //���� ����� ������ ������� ���� � ���� ������ �����
			{
				snake_map[tail.y - 1][tail.x] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(tail.y - 1, tail.x); //�������� ��������� �� �����
			}
			else if (tail.y == MAX_HEIGHT && snake_map[tail.y][tail.x - 1] == number_of_body) //���� ����� ������ ������� ���� � ���� ������ �����
			{
				snake_map[tail.y][tail.x - 1] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(tail.y, tail.x - 1); //�������� ��������� �� �����
			}
			else if (tail.y == MAX_HEIGHT && snake_map[tail.y][tail.x + 1] == number_of_body) //���� ����� ������ ������� ���� � ���� ������ ������
			{
				snake_map[tail.y][tail.x + 1] = 'T'; //�������� ������ �� �����
				hero.SetTailPointer(tail.y, tail.x + 1); //�������� ��������� �� �����
			}
		}
		snake_map[tail.y][tail.x] = ' '; //������� �����
		snake_body[tail.y][tail.x] = ' '; //� ������� ���� ������� ����, ������� ���� �� ����� ������ (���, �� ������ � ��� ��������)------------------------------------------------------------------------------------------
	}
}

void draw_snake(sf::RenderWindow & window, Hero & hero, std::string * snake_map, char ** snake_body)
{
	sf::Vector2i resolution_window; //�������� ���������� ������
	resolution_window = resol.GetIntRecolution();
	sf::Vector2f scale_from_resolution = GetPercentScaleFromCurrentResolution(resolution_window.x, resolution_window.y);
	if (scale_from_resolution.x == 0) //�������
	{
		scale_from_resolution.x = 1;
		scale_from_resolution.y = 1;
	}

	for (int i = 0; i <= MAX_HEIGHT; i++)
	{
		for (int j = 0; j <= MAX_WIDTH; j++)
		{
			if (snake_map[i][j] == 'H') //���� ������
			{
				if (dir == (int)hero.direction::up)
				{
					hero.setTextureRect(sf::IntRect(64 * 3, 64 * 0, 64, 64));
				}
				else if (dir == (int)hero.direction::down)
				{
					hero.setTextureRect(sf::IntRect(64 * 4, 64 * 1, 64, 64));
				}
				else if (dir == (int)hero.direction::right)
				{
					hero.setTextureRect(sf::IntRect(64 * 4, 64 * 0, 64, 64));
				}
				else if (dir == (int)hero.direction::left)
				{
					hero.setTextureRect(sf::IntRect(64 * 3, 64 * 1, 64, 64));
				}
				hero.setPosition(float(int(118 * scale_from_resolution.x) + j * int(100 * scale_from_resolution.x)), float(int(118 * scale_from_resolution.y) + i * int(100 * scale_from_resolution.y)));
				//118 - ������ �� ����, � 100 - �� ������� ��������
				window.draw(hero); //���������� ������ ����
			}
			else if (snake_map[i][j] == 'T') //���� �����
			{
				sf::Vector2i tail = hero.GetTailPointer(); //��������� ��������� ���� ����
				char number_of_body = hero.GetBodyPartNearTail();
				if (tail.y > 0 && tail.x > 0 && tail.y < MAX_HEIGHT && tail.x < MAX_WIDTH) //���� ����� ���� ��������� � ������ (�� ������������� �� ����� �����)
				{
					if (snake_map[tail.y][tail.x + 1] == number_of_body) //���� ���� ������ ������
					{
						hero.setTextureRect(sf::IntRect(64 * 4, 64 * 2, 64, 64));
					}
					else if (snake_map[tail.y][tail.x - 1] == number_of_body) //���� ���� ������ �����
					{
						hero.setTextureRect(sf::IntRect(64 * 3, 64 * 3, 64, 64));
					}
					else if (snake_map[tail.y - 1][tail.x] == number_of_body) //���� ���� ������ �����
					{
						hero.setTextureRect(sf::IntRect(64 * 3, 64 * 2, 64, 64));
					}
					else if (snake_map[tail.y + 1][tail.x] == number_of_body) //���� ���� ������ ����
					{
						hero.setTextureRect(sf::IntRect(64 * 4, 64 * 3, 64, 64));
					}
				}
				else // ���� ����� ������ ����
				{
					if (tail.y == 0 && tail.x == MAX_WIDTH && snake_map[tail.y][0] == number_of_body) //���� ����� ������ �������� ������� ���� � ���� ������ ������
						hero.setTextureRect(sf::IntRect(64 * 4, 64 * 2, 64, 64));
					else if (tail.y == 0 && tail.x == MAX_WIDTH && snake_map[MAX_HEIGHT][tail.x] == number_of_body) //���� ����� ������ �������� ������� ���� � ���� ������ �����
						hero.setTextureRect(sf::IntRect(64 * 4, 64 * 2, 64, 64));
					else if (tail.y == 0 && tail.x == 0 && snake_map[MAX_HEIGHT][0] == number_of_body) //���� ����� ������ �������� ������ ���� � ���� ������ �����
						hero.setTextureRect(sf::IntRect(64 * 3, 64 * 2, 64, 64));
					else if (tail.y == 0 && tail.x == 0 && snake_map[tail.y + 1][tail.x] == number_of_body) //���� ����� ������ �������� ������ ���� � ���� ������ ����
						hero.setTextureRect(sf::IntRect(64 * 4, 64 * 3, 64, 64));
					else if (tail.y == 0 && tail.x == 0 && snake_map[tail.y][MAX_WIDTH] == number_of_body) //���� ����� ������ �������� ������ ���� � ���� ������ �����
						hero.setTextureRect(sf::IntRect(64 * 3, 64 * 3, 64, 64));
					else if (tail.y == MAX_HEIGHT && tail.x == MAX_WIDTH && snake_map[tail.y][tail.x - 1] == number_of_body) //���� ����� ������ ������� ������� ���� � ���� ������ �����
						hero.setTextureRect(sf::IntRect(64 * 3, 64 * 3, 64, 64));
					else if (tail.y == MAX_HEIGHT && tail.x == MAX_WIDTH && snake_map[0][tail.x] == number_of_body) //���� ����� ������ ������� ������� ���� � ���� ������ ����
						hero.setTextureRect(sf::IntRect(64 * 4, 64 * 3, 64, 64));
					else if (tail.y == MAX_HEIGHT && tail.x == 0 && snake_map[0][tail.x] == number_of_body) //���� ����� ������ ������� ������ ���� � ���� ������ ����
						hero.setTextureRect(sf::IntRect(64 * 4, 64 * 3, 64, 64));
					else if (tail.y == 0 && snake_map[tail.y][tail.x + 1] == number_of_body) //���� ����� ������ �������� ���� � ���� ������ ������
						hero.setTextureRect(sf::IntRect(64 * 4, 64 * 2, 64, 64));
					else if (tail.y == 0 && snake_map[tail.y + 1][tail.x] == number_of_body) //���� ����� ������ �������� ���� � ���� ������ ����
						hero.setTextureRect(sf::IntRect(64 * 4, 64 * 3, 64, 64));
					else if (tail.y == 0 && snake_map[tail.y][tail.x - 1] == number_of_body) //���� ����� ������ �������� ���� � ���� ������ �����
						hero.setTextureRect(sf::IntRect(64 * 3, 64 * 3, 64, 64));
					else if (tail.x == MAX_WIDTH && snake_map[tail.y][0] == number_of_body) //���� ����� ������ ������� ���� � ���� ������ ������
						hero.setTextureRect(sf::IntRect(64 * 4, 64 * 2, 64, 64));
					else if (tail.x == MAX_WIDTH && snake_map[tail.y - 1][tail.x] == number_of_body) //���� ����� ������ ������� ���� � ���� ������ �����
						hero.setTextureRect(sf::IntRect(64 * 3, 64 * 2, 64, 64));
					else if (tail.x == MAX_WIDTH && snake_map[tail.y + 1][tail.x] == number_of_body) //���� ����� ������ ������� ���� � ���� ������ ����
						hero.setTextureRect(sf::IntRect(64 * 4, 64 * 3, 64, 64));
					else if (tail.x == MAX_WIDTH && snake_map[tail.y][tail.x - 1] == number_of_body) //���� ����� ������ ������� ���� � ���� ������ � ����
						hero.setTextureRect(sf::IntRect(64 * 3, 64 * 3, 64, 64));
					else if (tail.x == 0 && snake_map[tail.y][tail.x + 1] == number_of_body) //���� ����� ������ ������ ���� � ���� ������ � �����
						hero.setTextureRect(sf::IntRect(64 * 4, 64 * 2, 64, 64));
					else if (tail.x == 0 && snake_map[tail.y][MAX_WIDTH] == number_of_body) //���� ����� ������ ������ ���� � ���� ������ � ����
						hero.setTextureRect(sf::IntRect(64 * 3, 64 * 3, 64, 64));
					else if (tail.x == 0 && snake_map[tail.y - 1][tail.x] == number_of_body) //���� ����� ������ ������ ���� � ���� ������ �����
						hero.setTextureRect(sf::IntRect(64 * 3, 64 * 2, 64, 64));
					else if (tail.x == 0 && snake_map[tail.y + 1][tail.x] == number_of_body) //���� ����� ������ ������ ���� � ���� ������ ����
						hero.setTextureRect(sf::IntRect(64 * 4, 64 * 3, 64, 64));
					else if (snake_map[11 - 3][tail.x] == number_of_body) //���� ���� ������ �����
						hero.setTextureRect(sf::IntRect(64 * 3, 64 * 2, 64, 64));
					else if (tail.y == MAX_HEIGHT && snake_map[0][tail.x] == number_of_body) //���� ����� ������ ������� ���� � ���� ������ ����
						hero.setTextureRect(sf::IntRect(64 * 4, 64 * 3, 64, 64));
					else if (tail.y == MAX_HEIGHT && snake_map[tail.y - 1][tail.x] == number_of_body) //���� ����� ������ ������� ���� � ���� ������ �����
						hero.setTextureRect(sf::IntRect(64 * 3, 64 * 2, 64, 64));
					else if (tail.y == MAX_HEIGHT && snake_map[tail.y][tail.x - 1] == number_of_body) //���� ����� ������ ������� ���� � ���� ������ �����
						hero.setTextureRect(sf::IntRect(64 * 3, 64 * 3, 64, 64));
					else if (tail.y == MAX_HEIGHT && snake_map[tail.y][tail.x + 1] == number_of_body) //���� ����� ������ ������� ���� � ���� ������ ������
						hero.setTextureRect(sf::IntRect(64 * 4, 64 * 2, 64, 64));
				}
				hero.setPosition(float(int(118 * scale_from_resolution.x) + j * int(100 * scale_from_resolution.x)), float(int(118 * scale_from_resolution.y) + i * int(100 * scale_from_resolution.y)));
				window.draw(hero);
			}
			else if (snake_map[i][j] == 'A') //���� ������
			{
				hero.setTextureRect(sf::IntRect(64 * 0, 64 * 3, 64, 64));
				hero.setPosition(float(int(118 * scale_from_resolution.x) + j * int(100 * scale_from_resolution.x)), float(int(118 * scale_from_resolution.y) + i * int(100 * scale_from_resolution.y)));
				window.draw(hero);
			}
			else if (snake_map[i][j] != ' ')
			{
				if (snake_body[i][j] == '-')
					hero.setTextureRect(sf::IntRect(64 * 1, 64 * 0, 64, 64));
				else if (snake_body[i][j] == '|')
					hero.setTextureRect(sf::IntRect(64 * 2, 64 * 1, 64, 64));
				else if (snake_body[i][j] == '7')
					hero.setTextureRect(sf::IntRect(64 * 0, 64 * 0, 64, 64));
				else if (snake_body[i][j] == '9')
					hero.setTextureRect(sf::IntRect(64 * 2, 64 * 0, 64, 64));
				else if (snake_body[i][j] == '3')
					hero.setTextureRect(sf::IntRect(64 * 2, 64 * 2, 64, 64));
				else if (snake_body[i][j] == '1')
					hero.setTextureRect(sf::IntRect(64 * 0, 64 * 1, 64, 64));
				hero.setPosition(float(int(118 * scale_from_resolution.x) + j * int(100 * scale_from_resolution.x)), float(int(118 * scale_from_resolution.y) + i * int(100 * scale_from_resolution.y)));
				window.draw(hero);
			}
		}
	}
}

void apple_generation(std::string * snake_map)
{
	int apple_coord_x = 1 + rand() % MAX_WIDTH; //����� - ������ �����
	int apple_coord_y = 1 + rand() % MAX_HEIGHT;

	if (snake_map[apple_coord_y][apple_coord_x] == ' ')
	{
		apple.x = apple_coord_x; //��������� �� ������
		apple.y = apple_coord_y;
		snake_map[apple_coord_y][apple_coord_x] = 'A'; //�������� ������ ������ � ������ ��� ���������� ���������
	}
	else
	{
		apple_generation(snake_map);
	}
}

void addScoreAndSpeed(Hero & hero)
{
	float snake_speed = 0.035f; //��� �������� ��������� �������� �������� � ����
	my_score += 10; //��������� ����
	if ((my_score % 100) == 0)
	{
		hero.AddSpeed(snake_speed);
	}
}

bool GameOver(sf::RenderWindow & window)
{
	if (m_exit)
	{
		window.setMouseCursorVisible(true);
		//window = sf::RenderWindow(sf::VideoMode(resolution_window.x, resolution_window.y), "Snake", sf::Style::Fullscreen));
		sf::Vector2i resolution_window;
		resolution_window = resol.GetIntRecolution();

		sf::Font exit_font;
		exit_font.loadFromFile("fonts\\GrandHotel-Regular.otf");

		sf::Text exit_text(" ", exit_font, 200);
		exit_text.setString("The game is over!");
		exit_text.setFillColor(sf::Color::Blue);
		exit_text.setPosition(float(resolution_window.x / 2), float(resolution_window.y / 2));//������ ������� ������
		exit_text.setOrigin(550, 150);
		window.draw(exit_text);
		window.display();

		while (1)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			{
				window.close();
				LoadMenu();
				return 1;
			}
		}
	}
}

