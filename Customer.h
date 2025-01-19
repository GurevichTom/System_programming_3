#pragma once
#include "ShoppingCart.h"
#include "CustomerVTable.h"

#define CUSTOMER_ID_LENGTH 9
#define NAMES_SEP " "
#define NAME_PARTS_SEP "- "


typedef struct Customer_
{
	char*			id;
	char*			name;
	ShoppingCart*	pCart;
	void* pDerived;
	CustomerVTable vTable;
}Customer;

int		initCustomer(Customer* pCustomer);
int		chooseIfClubMember();
int		initBaseCustomer(Customer* pCustomer);
void	getCustomerID(Customer* pCustomer);
void	printBaseCustomer(const Customer* pCustomer);
void	printCustomer(const Customer* pCustomer);
void	printShoppingCartAndDisplayFinalBill(const Customer* pCustomer);

int		isCustomerIdValid(const char* id);

int		isCustomerById(const Customer* pCust, const char* id);
//int		isCustomersNameValid(const char* name);
void	getNamePart(char* part, const char* msg);
void	upperLowerCustomerName(char* name);
//char*	combineFirstLast(char*  firstName, char* lastName);
char*	combineFirstLast(char** parts);
void	pay(Customer* pCustomer);
void	displayFinalBillToPayCustomer(float price, const Customer* pCustomer);
void	cancelShopping(Customer* pCustomer);

void	saveCustomer(const Customer* pCust, FILE* fp);
void	customerSaveMembership(const Customer* pCust, FILE* fp);
int		loadCustomer(Customer* pCust, FILE* fp);

void	freeCustomer(Customer* pCust);



