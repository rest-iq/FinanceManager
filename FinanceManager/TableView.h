#pragma once
#include "console.h"
class TableView {
private:
	const int WIDHT = 85;
	const int HEIGHT = 20;
	int activeRow;
	int beginRow;
	int endRow;

	size_t getFrameWidth() const {
		//TODO
		return WIDHT;

	}

	size_t getFrameHeight() const {
		return HEIGHT;
	}
	vector<pair<string, int>> columns;
	vector<vector<string>> rows;
public:
	TableView(vector<pair<string, int>> columns, vector<vector<string>> rows) {
		this->columns = columns;
		setRows(rows);
		
	}
	void setRows(vector<vector<string>> rows) {
		this->rows = rows;
		activeRow = 0;
		beginRow = 0;
		endRow = HEIGHT - 4;
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
					SetColor(ConsoleColor::WHITE, WHITE);
					//SetColor(ConsoleColor::WHITE, (ConsoleColor)rand() % 16);
					cout << ' ';
				}
			}
		}
		SetColor(WHITE, BLACK);
	}
	void drawHeader() {
		int startX = 1;
		int startY = 1;

		for (size_t j = 0; j < columns.size(); j++)
		{
			SetColor(WHITE, BLACK);
			SetCursorPosition(startX, startY);
			cout << columns[j].first;
			startX += columns[j].second;
			SetCursorPosition(startX, startY);
			SetColor(ConsoleColor::WHITE, WHITE);
			//SetColor(ConsoleColor::WHITE, (ConsoleColor)rand() % 16);
			cout << " ";
			SetColor(WHITE, BLACK);
			cout << " ";
			startX += 1;
		}
		startY += 1;
		startX = 1;
		SetCursorPosition(startX, startY);
		for (size_t i = 0; i < WIDHT - 2; i++)
		{
			SetColor(ConsoleColor::WHITE, WHITE);
			//SetColor(ConsoleColor::WHITE, (ConsoleColor)rand() % 16);
			cout << " ";
			SetColor(WHITE, BLACK);
		}
	}
	void drawContent() {

		int startX = 1;
		int startY = 3;

		for (size_t i = beginRow; i < endRow; i++)
		{
			if (i >= rows.size()) {

				for (size_t j = 0; j < columns.size(); j++) {
					SetCursorPosition(startX, startY + (i - beginRow));
					SetColor(WHITE, BLACK);
					auto item = string(columns[j].second, ' ');
					cout << item;
					startX += columns[j].second;
					SetCursorPosition(startX, startY + (i - beginRow));
					SetColor(ConsoleColor::WHITE, WHITE);
					//SetColor(ConsoleColor::WHITE, (ConsoleColor)rand() % 16);
					cout << " ";
					SetColor(WHITE, BLACK);
					startX += 1;
				}
				startX = 1;
				continue;
			}

			for (size_t j = 0; j < columns.size(); j++)
			{
				SetCursorPosition(startX, startY + (i - beginRow));
				if (activeRow == i) {
					SetColor(BLACK, YELLOW);
				}
				else {
					SetColor(WHITE, BLACK);
				}
				auto item = rows[i][j];
				if (item.size() > columns[j].second) {
					item = item.substr(0, columns[j].second - 3);
					item += "...";
				}
				item.append(string(columns[j].second - item.size(), ' '));
				cout << item;
				startX += columns[j].second;
				SetCursorPosition(startX, startY + (i - beginRow));
				SetColor(ConsoleColor::WHITE, WHITE);
				//SetColor(ConsoleColor::WHITE, (ConsoleColor)rand() % 16);
				cout << " ";
				SetColor(WHITE, BLACK);
				cout << " ";
				startX += 1;
			}
			startX = 1;
			//startY++;
		}
	}
	void down() {

		activeRow++;

		if (activeRow >= rows.size()) {
			activeRow--;
		}
		else {

			if (activeRow >= endRow) {
				beginRow++;
				endRow++;
			}
		}
	}
	void up() {
		activeRow--;
		if (activeRow < 0) {
			activeRow++;
		}
		else
		{
			if (activeRow < beginRow) {
				beginRow--;
				endRow--;
			}
		}
	}

};