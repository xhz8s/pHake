#pragma once
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <Dwmapi.h>
#include <thread>
#include <vector>
#include "lib/pButton.hpp"
#include "lib/pItem.hpp"
#include "lib/pListView.hpp"
#include "lib/pMouse.hpp"
#include "lib/pNotification.hpp"
#pragma comment (lib, "Dwmapi.lib")

void setTransparent(HWND handle)
{
	MARGINS margins;
	margins.cxLeftWidth = -1;
	LONG cur_style = GetWindowLong(handle, GWL_EXSTYLE);

	DwmExtendFrameIntoClientArea(handle, &margins);
	SetWindowPos(handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetWindowLong(handle, GWL_EXSTYLE, cur_style | WS_EX_TRANSPARENT | WS_EX_LAYERED);
}

struct GameInfo
{
	HWND hwnd;
	RECT rect;
	LPCSTR game;
	sf::Vector2i pos;
	sf::Vector2i size;

	GameInfo() {}

	GameInfo(LPCSTR gameW)
	{
		game = gameW;
		hwnd = FindWindowA(0, gameW);
		GetWindowRect(hwnd, &rect);

		pos = this->getPos();
		size = this->getSize();
	}

	void update()
	{
		hwnd = FindWindowA(0, game);
		GetWindowRect(hwnd, &rect);
		pos = this->getPos();
		size = this->getSize();
	}

	sf::Vector2i getPos()
	{
		sf::Vector2i value;

		value.x = rect.left;
		value.y = rect.top;

		return value;
	}

	sf::Vector2i getSize()
	{
		sf::Vector2i value;

		value.x = rect.right - rect.left;
		value.y = rect.bottom - rect.top;

		return value;
	}
};

class pGui
{
private:
	sf::RenderWindow Window;
	sf::Font		 Font;
	GameInfo		 gameInfo;

public:
	pList			 entries;
	pMouse			 mouse;
	pNotification    notification;

	pGui(){}

	void create(LPCSTR Name)
	{
		gameInfo = GameInfo(Name);
		Font.loadFromFile("Settings/font.ttf");

		Window.create(sf::VideoMode(gameInfo.getSize().x - 1, gameInfo.getSize().y + 1), "pHake 3.1", sf::Style::None);
		Window.setFramerateLimit(60);
		setTransparent(Window.getSystemHandle());

		notification.create(&Window);
		mouse.create(&Window);
		entries.create(&Window);
		entries.setPostion(Window.getSize().x / 2, Window.getSize().y / 4);
	}

	void toggle()
	{
		entries.toggle();
		mouse.toggle();
	}

	void Loop()
	{
		while (Window.isOpen() && gameInfo.hwnd != 0)
		{
			gameInfo.update();
			Window.setPosition(gameInfo.pos);
	
			sf::Event event;
			while (Window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					Window.close();
			}
			entries.Loop();
			mouse.Loop();
	
			Window.clear(sf::Color::Color(0, 0, 0, 0));

			entries.draw();
			mouse.draw();
			notification.draw();

			Window.display();
		}
	}
};