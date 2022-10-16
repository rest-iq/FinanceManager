#pragma once
#include "console.h"
class MultiPageMenu {
private:
	int activeOption = 0;
	int beginRow;
	int endRow;
	int rows;
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

	size_t getFrameWidth() const {

		return getMaxItemSize() + 6;

	}

	size_t getFrameHeight() const {
		return rows + 4;
	}
public:
	MultiPageMenu(const vector<string>& options, int rows) {
		this->options = options;
		this->rows = rows;
		beginRow = 0;
		endRow = rows;
		activeOption = 0;
	}
	void drawFrame() {
		size_t widht = getFrameWidth();
		size_t height = getFrameHeight();
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

		for (size_t i = beginRow; i < endRow; i++)
		{
			if (i >= options.size()) {
				SetCursorPosition(startX, startY + (i - beginRow));
				SetColor(WHITE, BLACK);
				cout << string(max, ' ');
				continue;
			}
			SetCursorPosition(startX, startY + (i - beginRow));
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
			beginRow = 0;
			endRow = rows;
		}

		if (activeOption >= endRow) {
			beginRow++;
			endRow++;
		}
	}
	void up() {
		activeOption--;
		if (activeOption < 0) {
			activeOption = options.size() - 1;
			beginRow = options.size() - rows;
			endRow = options.size();
		}
		if (activeOption < beginRow) {
			beginRow--;
			endRow--;
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