#pragma once
#include <iostream>
#include <chrono>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;
class Category
{
private:
	int id;
	string title;
	bool isIncome;

public:
	Category(string title, bool isIncome)
	{
		this->title = title;
		this->isIncome = isIncome;
	}
	Category() {}
	int getId()
	{
		return id;
	}
	string getTitle()
	{
		return title;
	}
	bool getIsIncome()
	{
		return isIncome;
	}
	void setId(int id)
	{
		this->id = id;
	}
	friend ofstream& operator << (ofstream& out, Category& model)
	{
		out << model.id << endl;
		out << model.title << endl;
		out << model.isIncome << endl;
		return out;
	}
	friend ifstream& operator >> (ifstream& in, Category& model)
	{
		in >> model.id;
		in.ignore(99999, '\n');
		getline(in, model.title);
		in >> model.isIncome;
		in.ignore(99999, '\n');
		return in;
	}
};
class CategoryRepo
{
private:
	std::vector<Category> categories;
	string file;
	int lastId = 0;
public:
	CategoryRepo(string file)
	{
		this->file = file;

	}
	void load()
	{
		categories.clear();
		ifstream in(file);
		int size;
		in >> size;
		in.ignore(99999, '\n');
		for (size_t i = 0; i < size; i++)
		{
			Category model;
			in >> model;
			if (lastId < model.getId())
			{
				lastId = model.getId();
			}
			categories.push_back(model);
		}
		in.close();
	}
	void save()
	{
		ofstream out(file);
		out << categories.size() << endl;
		for (size_t i = 0; i < categories.size(); i++)
		{
			out << categories[i];
		}
		out.close();
	}
	void addCategory(string title, bool isIncome)
	{
		Category category(title, isIncome);
		category.setId(++lastId);
		categories.push_back(category);
	}
	void addCategory( Category category)
	{		
		category.setId(++lastId);
		categories.push_back(category);
	}
	Category* getCategoryById(int id)
	{
		for (size_t i = 0; i < categories.size(); i++)
		{
			if (categories[i].getId() == id)
			{
				return &categories[i];
			}
		}
		return nullptr;
	}
	const vector<Category>& getCategories()
	{
		return categories;
	}

};
