#include <iostream>
#include <chrono>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "menu.h"
#include "category.h"
#include "transaction.h"
#include "TableView.h"
#include "MultiPageMenu.h"
#include "InputBox.h"
//Система "особисті фінанси"
//
//Рахунок.По рахунку є витрати та доходи.Баланс.Назва рахунку.
//Рахунків може бути декілька.
//Витрати. - категорія, опис, сума, дата.
//Дохід - категорія, опис, сума, дата.
//
//Створити рахунок.
//Вносити витрати.Видалити витрату.Змінити витрату(суму, дату та категорію та опис)
//Вносити дохід.Видалити дохід.Змінити дохід(суму, дату та категорію та опис)
//
//Вивод списку доходів та витрат по 30 на сторінку, з можливістю переходити по сторінках.
//
//Дані зберігаються в файл при закритті програми та читаються за файлу при завантаженні програми.
//
//Звіти.
//Звіт за останні 30 днів. (дата, дохід, витрати за кожен день)
//Звіт за місяць(загальний дохід, загальні витрати, баланс початку, баланс на кінець періоду)
//Список категорій з сумобю витрат по ним за місяць, від найбільшого до найменьшого.
//Звіт - 3 найдорожчі покупки(витрати)
//
//Можливість зберегти звіт в файл.
using namespace std;
//Звіт за місяць(загальний дохід, загальні витрати, баланс початку, баланс на кінець періоду)
class Report {
private:
	float totalIncome = 0;
	float totalOutcome = 0;
	float beginBalance = 0;
	float endBalance = 0;
public:
	float getTotalIncome() 
	{
		return totalIncome;
	}
	float getTotalOutcome()
	{
		return totalOutcome;
	}
	float getBeginBalance()
	{
		return beginBalance;
	}
	float getEndBalance()
	{
		return endBalance;
	}
	Report(){}
	Report(TransactionRepo& transactionRepo)
	{
		for ( auto t: transactionRepo.getTransactions())
		{
			if (t.getCategory()->getIsIncome()) {
				totalIncome += t.getAmount();
			}
			else {
				totalOutcome += t.getAmount();
			}
		}
		endBalance = totalIncome - totalOutcome;
	}

};
class MonthsReport:public Report {
protected:
	float totalIncome = 0;
	float totalOutcome = 0;
	float beginBalance = 0;
	float endBalance = 0;
public:
	float getTotalIncome()
	{
		return totalIncome;
	}
	float getTotalOutcome()
	{
		return totalOutcome;
	}
	float getBeginBalance()
	{
		return beginBalance;
	}
	float getEndBalance()
	{
		return endBalance;
	}
	MonthsReport(TransactionRepo& transactionRepo, chrono::time_point<chrono::system_clock> begin, chrono::time_point<chrono::system_clock> end)
	{
		//вирахувати баланс на початок періоду
		float beforePeriodIncome = 0;
		float beforePeriodOutcome = 0;
		for (auto t : transactionRepo.getTransactions())
		{
			if (t.getDataTime() <begin) {
				if (t.getCategory()->getIsIncome()) {
					beforePeriodIncome += t.getAmount();
				}
				else {
					beforePeriodOutcome += t.getAmount();
				}
			}

		}
		beginBalance = beforePeriodIncome - beforePeriodOutcome;

		// вирахувати баланас на кінець періодгну
		for (auto t : transactionRepo.getTransactions())
		{
			if (t.getDataTime() >= begin && t.getDataTime() <= end) {
				if (t.getCategory()->getIsIncome()) {
					totalIncome += t.getAmount();
				}
				else {
					totalOutcome += t.getAmount();
				}
			}

		}
		endBalance = beginBalance + (totalIncome - totalOutcome);
	}
};
Category createCategory(CategoryRepo& categoryRepo) 
{
	StringInputBox askDescription("Ask title", 30);
	string title= askDescription.getValue();
	InputBox <bool> askIsIncome("IsIncome ?", 5);
	bool IsIncome = askIsIncome.getValue();
	Category category(title, IsIncome);
	return category;
}
Transaction createTransaction(CategoryRepo& categoryRepo)
{
	double amount;
	string dateTime;
	string description;
	int categoryId;
	InputBox <float> askAmaunt("Enter Amount:", 7);
	amount = askAmaunt.getValue();
	StringInputBox askDate("Enter date", 12);
	dateTime = askDate.getValue();
	StringInputBox askDescription("Enter description", 30);
	description = askDescription.getValue();
	vector <string> categories;

	for (size_t i = 0; i < categoryRepo.getCategories().size(); i++)
	{
		auto t = categoryRepo.getCategories()[i];
		categories.push_back(t.getTitle());
	}
	MultiPageMenu catMenu(categories, 5);
	auto category = categoryRepo.getCategories()[catMenu.run()];
	categoryId = category.getId();

	Transaction transaction(amount, timePointFromString(dateTime, "%d.%m.%Y"), categoryRepo.getCategoryById(categoryId), description);
	return transaction;
}
void printHelpMenu() {
	SetCursorPosition(3, 21);
	cout << "F1 sort by date " ;
	SetCursorPosition(3, 22);
	cout << "F2 sort by category " ;
	SetCursorPosition(3, 23);
	cout << "F3 sort by amount ";
	SetCursorPosition(3, 24);
	cout << "F4 add category ";
	SetCursorPosition(3, 25);
	cout << "F5 add transaction ";
	SetCursorPosition(3, 25);
	cout << "F8 report ";
}
void printBalance(TransactionRepo& transactionRepo) {
	SetCursorPosition(90, 1);
	cout << "Income: " << transactionRepo.getTotalIncome();
	SetCursorPosition(90, 2);
	cout << "OutIncome: " << transactionRepo.getTotalOutIncome();
	SetCursorPosition(90, 3);
	cout << "Balance: " << transactionRepo.getTotalBalance();
	printHelpMenu();
}
void printMonthsReport(TransactionRepo& transactionRepo)
{
	string beginDate;
	string endDate;
	StringInputBox enterBeginDate("Enter BeginDate", 30);
	beginDate = enterBeginDate.getValue();
	StringInputBox enterEndDate("Enter EndDate", 30);
	endDate = enterEndDate.getValue();
	MonthsReport reportMonth(transactionRepo, timePointFromString(beginDate, "%d.%m.%Y"), timePointFromString(endDate, "%d.%m.%Y"));
	cout << "Report: " << endl;
	cout << "Total Income: " << reportMonth.getTotalIncome() << endl;
	cout << "Total Outcome: " << reportMonth.getTotalOutcome() << endl;
	cout << "Begin Balance: " << reportMonth.getBeginBalance() << endl;
	cout << "End Balance: " << reportMonth.getEndBalance() << endl;
	system("pause");
	system("cls");
}

int main()
{
	CategoryRepo categoryRepo("Categories.txt");
	TransactionRepo transactionRepo("Transactions.txt", &categoryRepo);

	//categoryRepo.addCategory("Tech", false);
	//categoryRepo.addCategory("Shop", true);
	//double amount, chrono::time_point<chrono::system_clock> dateTime, Category* category, string description = ""
	//Transaction transaction(100.0, timePointFromString("03.10.2022", "%d.%m.%Y"), categoryRepo.getCategoryById(1), "Hello!");
	//transactionRepo.addTransaction(transaction);
	//transactionRepo.save();
	//auto category = repository.getCategoryById(1);
	//cout << category->getId() << " " << category->getTitle() << " " << category->getIsIncome(); 
	transactionRepo.load();
	TransactionHelper helper(&transactionRepo);
	TableView table(transactionRepo.getColumns(), helper.sortByDate().getTransactionData());
	table.drawFrame();
	table.drawHeader();
	printBalance(transactionRepo);
	
	while (true)
	{
		table.drawContent();
		table.drawFrame();
		table.drawHeader();

		int key;
		while (true)
		{


			key = getKey();
			if (key == -1) {
				Sleep(10);
				continue;
			}
			switch (key)
			{
			case UP_ARROW:
				table.up();
				table.drawContent();
				break;
			case DOWN_ARROW:
				table.down();
				table.drawContent();
				break;
				/*case ENTER:
					system("cls");
					cout << menu.getSelectedOption();

					return 0;*/
			case F1_KEY:
				table.setRows(helper.sortByDate().getTransactionData());
				table.drawFrame();
				table.drawHeader();
				table.drawContent();
				printBalance(transactionRepo);
				break;
			case F2_KEY:
				table.setRows(helper.sortByCategory().getTransactionData());
				table.drawFrame();
				table.drawHeader();
				table.drawContent();
				printBalance(transactionRepo);
				break;
			case F3_KEY:
				table.setRows(helper.sortByAmount().getTransactionData());
				table.drawFrame();
				table.drawHeader();
				table.drawContent();
				printBalance(transactionRepo);
				break;
			case F4_KEY:
				categoryRepo.addCategory(createCategory(categoryRepo));
				transactionRepo.uppDateCategory();
				transactionRepo.save();				
				table.drawFrame();
				table.drawHeader();
				table.drawContent();
				printBalance(transactionRepo);
				break;
			case F5_KEY:
				transactionRepo.addTransaction(createTransaction(categoryRepo));
				transactionRepo.save();
				table.setRows(helper.sortByDate().getTransactionData());
				table.drawFrame();
				table.drawHeader();
				table.drawContent();
				printBalance(transactionRepo);
				break;
			case F8_KEY:
				printMonthsReport(transactionRepo);
				table.drawFrame();
				table.drawHeader();
				table.drawContent();
				printBalance(transactionRepo);
				break;
			default:
				break;
			}
			Sleep(10);
		}
	}
	/*Report report(transactionRepo);
	cout << "Report: " << endl;
	cout << "Total Income: " << report.getTotalIncome() << endl;
	cout << "Total Outcome: " << report.getTotalOutcome() << endl;
	cout << "Begin Balance: " << report.getBeginBalance() << endl;
	cout << "End Balance: " << report.getEndBalance() << endl;
	system("pause");
	MonthsReport reportMonth(transactionRepo, timePointFromString("01.10.2022", "%d.%m.%Y"),timePointFromString("31.10.2022", "%d.%m.%Y"));
	cout << "Report: " << endl;
	cout << "Total Income: " << reportMonth.getTotalIncome() << endl;
	cout << "Total Outcome: " << reportMonth.getTotalOutcome() << endl;
	cout << "Begin Balance: " << reportMonth.getBeginBalance() << endl;
	cout << "End Balance: " << reportMonth.getEndBalance() << endl;
	system("pause");*/
	do
	{
		Transaction t = createTransaction(categoryRepo);
		transactionRepo.addTransaction(t);
		char answer;
		cout << "Add new transaction?" << endl;
		cin >> answer;
		if (answer != 'y')
			break;

	} while (true);
	transactionRepo.save();
	vector<string> transactions;
	for (size_t i = 0; i < transactionRepo.getTransactions().size(); i++)
	{

		

		auto t = transactionRepo.getTransactions()[i];
		transactions.push_back(t.getDataTimeAsString() + " " + to_string(t.getId()) + " " + to_string(t.getAmount()) +
			" " + t.getCategory()->getTitle() + " " + t.getDescription());
		
	}
	MultiPageMenu menuTransaction(transactions, 20);
	menuTransaction.run();
	/*Transaction t(100.99, timePointFromString("29.09.2022", "%d.%m.%Y"), categoryRepo.getCategoryById(2), "!");
	transactionRepo.addTransaction(t);
	transactionRepo.save();*/
	/*auto transaction = transactionRepo.getTransactionsById(1);
	cout << transaction->getDataTimeAsString() << " " << transaction->getId() << " " << transaction->getAmount()
		<< " " << transaction->getCategory()->getTitle()<< " "<< transaction->getDescription()<<endl;*/
}

//cin.ignore(99999, '\n');