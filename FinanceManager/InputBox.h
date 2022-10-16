#pragma once
#include "console.h"
template <class T>
class InputBox {
protected:
	string prompt;
	size_t max;
	size_t getMaxItemSize() const {
		return max + prompt.size();
	}
	size_t getFrameWidth() const {
		return getMaxItemSize() + 6;
	}

	size_t getFrameHeight() const {
		return 5;
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
	void drawContent() {
		int startX = 2;
		int startY = 2;
		system("cls");
		drawFrame();
		SetCursorPosition(startX, startY);
		SetColor(WHITE, BLACK);
		cout << prompt << " ";
	}
public:
	InputBox(string prompt, int max) {
		this->prompt = prompt;
		this->max = max;
	}

	virtual T getValue(T* ignore = nullptr) {
		drawContent();
		T buffer;
		cin >> buffer;
		cin.ignore(99999, '\n');
		system("cls");
		return buffer;
	}

	/*virtual string getValue(string* ignore = nullptr) {
		drawContent();
		string buffer;
		getline(cin, buffer);
		system("cls");
		return buffer;
	}*/
};
class StringInputBox : public InputBox<string> {
public:
	StringInputBox(string prompt, int max) : InputBox<string>(prompt, max) {}

	virtual string getValue(string* ignore = nullptr) override {
		drawContent();
		string buffer;
		getline(cin, buffer);
		system("cls");
		return buffer;
	}
};
class PasswordInputBox : public InputBox<string> {
public:
	PasswordInputBox(string prompt, int max) : InputBox<string>(prompt, max) {}
	virtual string getValue(string* ignore = nullptr) override {
		drawContent();
		SetColor(WHITE, WHITE);
		string buffer;
		getline(cin, buffer);
		system("cls");
		SetColor(WHITE, BLACK);
		return buffer;
	}
};