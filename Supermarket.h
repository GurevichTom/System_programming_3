#pragma once
#include "Customer.h"
#include "Product.h"
#include "ShoppingCart.h"
#include "save_load.h"

typedef enum {
	SORT_BY_NAME,
	SORT_BY_QUANTITY,
	SORT_BY_PRICE,
	NUM_SORT_CRITERIA, // Helper to count criteria
	NOT_SORTED
} SortCriteria;

extern const char* sortCriteriaStrings[NUM_SORT_CRITERIA];
extern void* sortCriteriaComparators[NUM_SORT_CRITERIA];

typedef struct SuperMarket
{
	char*		name;
	Customer*	customerArr;
	int			customerCount;
	Product**	productArr;
	int			productCount;
	SortCriteria productsArrSortedBy; // Current sorting criterion
}SuperMarket;


void		sortProducts(SuperMarket* pMarket);
int			compareProductsPtrByPrice(const void* a, const void* b);
int			compareProductsPtrByQuantity(const void* a, const void* b);
int			compareProductsPtrByName(const void* a, const void* b);
SortCriteria selectSoringCriteria();
void findProduct(const SuperMarket* pMarket);

int			initSuperMarket(SuperMarket* pMarket);
void		initEmptySuperMarket(SuperMarket* pMarket);
void		printSuperMarket(const SuperMarket* pMarket);
int			addProduct(SuperMarket* pMarket);
int			addNewProduct(SuperMarket* pMarket);
int			addCustomer(SuperMarket* pMarket);
int			isCustomerInMarket(SuperMarket* pMarket, Customer* pCust);
int			doShopping(SuperMarket* pMarket);
Customer*	doPrintCart(SuperMarket* pMarket);
int			manageShoppingCart(SuperMarket* pMarket);
Customer*	getCustomerShopPay(SuperMarket* pMarket);
void		fillCart(SuperMarket* pMarket, ShoppingCart* pCart);
void		clearCart(SuperMarket* pMarket, Customer* pCustomer);

void		printProductByType(SuperMarket* pMarket);
void		printAllProducts(const SuperMarket* pMarket);
void		printAllCustomers(const SuperMarket* pMarket);
int			isBarcodeUnique(const SuperMarket* pMarket, const char* barcode);
int 		isCustomerIdUnique(const SuperMarket* pMarket, const char* id);


int			getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode);
Product*	getProductByBarcode(SuperMarket* pMarket, const char* barcode);
Product*	getProductFromUser(SuperMarket* pMarket, char* barcode);
void		getUniquBarcode(char* barcode, SuperMarket* pMarket);
Customer*	FindCustomerById(SuperMarket* pMarket, const char* id);

Customer*	getCustomerWhoShop(SuperMarket* pMarket);
Product*	getProductAndCount(SuperMarket* pMarket, int* pCount);

void		freeMarket(SuperMarket* pMarket);
void		freeProducts(SuperMarket* pMarket);
void		freeCustomers(SuperMarket* pMarket);

void finalizeSuperMarket(SuperMarket* pMarket);

