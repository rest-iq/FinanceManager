#pragma once
#include <iostream>
#include <vector>
#include "console.h"

using namespace std;

class Menu {
private:
	int activeOption = 0;
	vector<string> options;
	size_t getMaxItemSize() const {
		size_t max = 0;
		for (string item : options)
		{
			if (item.size() > max) {
				max = item.size();
			}
		}
		return max;
	}

public:
	Menu(const vector<string>& options) {
		this->options = options;
	}
	void drawFrame() {
		//отобразить рамку
		size_t widht = 40;
		size_t height = 12;
		for (size_t y = 0; y < height; y++)
		{
			for (size_t x = 0; x < widht; x++)
			{
				if (x == 0 || x == widht - 1 || y == 0 || y == height - 1) {
					SetCursorPosition(x, y);
					SetColor(ConsoleColor::WHITE, (ConsoleColor)rand() % 16);
					cout << ' ';
				}
			}
		}
		SetColor(WHITE, BLACK);
	}

	void drawOptions() {
		int startX = 3;
		int startY = 2;
		int max = getMaxItemSize();
		for (size_t i = 0; i < options.size(); i++)
		{
			SetCursorPosition(startX, startY + i);
			if (activeOption == i) {
				SetColor(RED, WHITE);
			}
			else {
				SetColor(WHITE, BLACK);
			}
			auto item = options[i];
			item.append(string(max - item.size(), ' '));
			cout << item;
		}
	}

	void down() {
		activeOption++;
		if (activeOption >= options.size()) {
			activeOption = 0;
		}
	}

	void up() {
		activeOption--;
		if (activeOption < 0) {
			activeOption = options.size() - 1;
		}
	}

	int getSelectedOption() const {
		return activeOption;
	}
	int run() {
		drawOptions();
		int key;
		while (true)
		{
			drawFrame();
			key = getKey();
			if (key == -1) {
				Sleep(90);
				continue;
			}
			switch (key)
			{
			case UP_ARROW:
				up();
				drawOptions();
				break;
			case DOWN_ARROW:
				down();
				drawOptions();
				break;
			case ENTER:
				system("cls");
				return  getSelectedOption();			
			default:
				break;
			}
			Sleep(10);
		}
	}
	
};

