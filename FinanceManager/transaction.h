#pragma once
#include "category.h"
#include <algorithm>
//перетворює строку на хроно використовуючи формат
chrono::system_clock::time_point timePointFromString(const std::string& dateStr, const std::string& format)
{
	std::stringstream ss{ dateStr };
	std::chrono::system_clock::time_point tp;
	ss >> chrono::parse(format, tp);
	if (ss.fail()) {
		throw "Can`t parse date";
	}
	return tp;
}
//пертворює хроно на строку у форматі 04.10.2022
std::string timePointToString(const chrono::system_clock::time_point& tp)
{
	std::stringstream ss;
	ss << format("{:%d.%m.%Y}", tp);
	return ss.str();
}
class Transaction
{
private:
	double amount;
	chrono::time_point<chrono::system_clock> dateTime;
	Category* category;
	string description;
	int categoryId;
	int id;
public:

	Transaction()
	{

	}
	Transaction(double amount, chrono::time_point<chrono::system_clock> dateTime, Category* category, string description = "")
	{
		this->amount = amount;
		this->dateTime = dateTime;
		this->category = category;
		this->description = description;
	}
	int getId()
	{
		return id;
	}
	void setId(int id)
	{
		this->id = id;
	}
	double getAmount()
	{
		return amount;
	}

	string getAmountAsString() {
		stringstream ss;
		ss << fixed << setprecision(2) << ((category->getIsIncome() ? 1: -1) * amount);
		return ss.str();
	}

	int getCategoryId()
	{
		return categoryId;
	}
	string getDescription()
	{
		return description;
	}
	void setAmount(double amount)
	{
		this->amount = amount;
	}
	void setDateTime(chrono::time_point<chrono::system_clock> dateTime)
	{
		this->dateTime = dateTime;
	}
	void setCategory(Category* category)
	{
		this->categoryId = category->getId();
		this->category = category;
	}
	void setDescription(string description)
	{
		this->description = description;
	}
	chrono::time_point<chrono::system_clock>  getDataTime()
	{
		return dateTime;
	}
	string getDataTimeAsString()
	{
		return timePointToString(dateTime);
	}
	Category* getCategory()
	{
		return category;
	}
	friend ofstream& operator << (ofstream& out, Transaction& model)
	{
		out << model.id << endl;
		out << model.amount << endl;
		out << timePointToString(model.dateTime) << endl;
		out << model.description << endl;
		out << model.category->getId() << endl;
		return out;
	}
	friend ifstream& operator >> (ifstream& in, Transaction& model)
	{
		string temp;
		in >> model.id;
		in.ignore(99999, '\n');
		in >> model.amount;
		in.ignore(99999, '\n');
		getline(in, temp);
		model.dateTime = timePointFromString(temp, "%d.%m.%Y");
		getline(in, model.description);
		in >> model.categoryId;
		in.ignore(99999, '\n');
		return in;
	}

};
class TransactionRepo {
private:
	std::vector<Transaction> transactions;
	string file;
	int lastId = 0;
	CategoryRepo* categoryRepo;
public:
	TransactionRepo(string file, CategoryRepo* categoryRepo)
	{

		this->file = file;
		this->categoryRepo = categoryRepo;

	}
	float getTotalIncome() {
		float sum = 0.0;
		for (auto t : getTransactions())
		{		
				if (t.getCategory()->getIsIncome()) {
					sum += t.getAmount();
				}

		}
		return sum;
	}
	float getTotalOutIncome() {
		float sum = 0.0;
		for (auto t : getTransactions())
		{
			if (!t.getCategory()->getIsIncome()) {
				sum += t.getAmount();
			}

		}
		return sum;
	}
	float getTotalBalance() {
		return getTotalIncome() - getTotalOutIncome();
	}
	/*~TransactionRepo()
	{
		delete categoryRepo;
	}*/
	vector <pair<string, int>> getColumns()
	{
		vector <pair<string, int>> result({ {"Date", 10},{"Category",20},{"Description", 40},{"Amount", 10} });
			return result;
	}
	vector <vector<string>>getTransactionData() 
	{
		vector <vector<string>> result;
		for (size_t i = 0; i < transactions.size(); i++)
		{
			result.push_back(vector<string>({ transactions[i].getDataTimeAsString(), transactions[i].getCategory()->getTitle(), transactions[i].getDescription(),
				transactions[i].getAmountAsString() }));
		}
		return result;
	}
	void load()
	{
		categoryRepo->load();
		transactions.clear();
		ifstream in(file);
		int size;
		in >> size;
		in.ignore(99999, '\n');
		for (size_t i = 0; i < size; i++)
		{
			Transaction model;
			in >> model;
			if (lastId < model.getId())
			{
				lastId = model.getId();
			}

			model.setCategory(categoryRepo->getCategoryById(model.getCategoryId()));
			transactions.push_back(model);
		}
		in.close();
	}
	void uppDateCategory()
	{	
		for (size_t i = 0; i < transactions.size(); i++)
		{
			transactions[i].setCategory(categoryRepo->getCategoryById(transactions[i].getCategoryId()));
			
		}	
	}
	void save()
	{
		categoryRepo->save();
		ofstream out(file);
		out << transactions.size() << endl;
		for (size_t i = 0; i < transactions.size(); i++)
		{
			out << transactions[i];
		}
		out.close();
	}
	void addTransaction(Transaction transaction)
	{
		transaction.setId(++lastId);
		transactions.push_back(transaction);
	}

	Transaction* getTransactionsById(int id)
	{
		for (size_t i = 0; i < transactions.size(); i++)
		{
			if (transactions[i].getId() == id)
			{
				return &transactions[i];
			}
		}
		return nullptr;
	}

	const vector<Transaction>& getTransactions()
	{
		return transactions;
	}
	
};

bool byDate(Transaction a, Transaction b)
{
	return a.getDataTime() < b.getDataTime();
}

bool byCategory(Transaction a, Transaction b)
{
	return a.getCategory()->getTitle() < b.getCategory()->getTitle();
}

bool byAmount(Transaction a, Transaction b)
{
	return a.getAmount() < b.getAmount();
}

class TransactionHelper
{
private:
	vector <Transaction> transactions;
	TransactionRepo* transactionRepo;
	/*bool byDate(Transaction a, Transaction b)
	{
		return a.getDataTime() < b.getDataTime();
	}*/
public:
	TransactionHelper(TransactionRepo* transactionRepo)
	{
		this->transactionRepo = transactionRepo;
	}
	TransactionHelper & sortByDate() 
	{
		transactions = transactionRepo->getTransactions();
		sort(transactions.begin(), transactions.end(), byDate);
		return *this;
	}
	TransactionHelper& sortByCategory()
	{
		transactions = transactionRepo->getTransactions();
		sort(transactions.begin(), transactions.end(), byCategory);
		return *this;
	}
	TransactionHelper& sortByAmount()
	{
		transactions = transactionRepo->getTransactions();
		sort(transactions.begin(), transactions.end(), byAmount);
		return *this;
	}
	vector <vector<string>>getTransactionData()
	{
		vector <vector<string>> result;
		for (size_t i = 0; i < transactions.size(); i++)
		{
			result.push_back(vector<string>({ transactions[i].getDataTimeAsString(), transactions[i].getCategory()->getTitle(), transactions[i].getDescription(),
				transactions[i].getAmountAsString() }));
		}
		return result;
	}
};
