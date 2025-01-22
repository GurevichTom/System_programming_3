#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Supermarket.h"
#include "Product.h"
#include "Customer.h"
#include "General.h"
#include "ShoppingCart.h"


const char* sortCriteriaStrings[NUM_SORT_CRITERIA] = {"Name", "Count", "Price"};
void* sortCriteriaComparators[NUM_SORT_CRITERIA] = {compareProductsPtrByName, compareProductsPtrByQuantity, compareProductsPtrByPrice};

int		initSuperMarket(SuperMarket* pMarket)
{
	initEmptySuperMarket(pMarket);
	if (loadSuperMarket(pMarket))
		return 1;

	pMarket->name = getStrExactLength("Enter market name");

	if (!pMarket->name)
		return 0;

	return 1;
}

void initEmptySuperMarket(SuperMarket* pMarket) {
	pMarket->productsArrSortedBy = NOT_SORTED;
	pMarket->customerCount = 0;
	pMarket->customerArr = NULL;
	pMarket->productCount = 0;
	pMarket->productArr = NULL;
	pMarket->name = NULL;
}

SortCriteria selectSoringCriteria() {
	printf("Base on what field do you want to sort?\n");
	int back;
	do {
		for (int i = 0; i < NUM_SORT_CRITERIA; i++) {
			printf("Enter %d for %s\n", i+1, sortCriteriaStrings[i]);
		}
		scanf("%d", &back);
	} while (back < 1 || back > NUM_SORT_CRITERIA);
	back--;
	return (SortCriteria) back;
}

int compareProductsPtrByName(const void* a, const void* b){
	const Product* p1 = *(const Product**) a;
	const Product* p2 = *(const Product**) b;
	return strcmp(p1->name, p2->name);
}

int compareProductsPtrByQuantity(const void* a, const void* b){
	const Product* p1 = *(const Product**) a;
	const Product* p2 = *(const Product**) b;
	return p1->count - p2->count;
}

int compareProductsPtrByPrice(const void* a, const void* b){
	const Product* p1 = *(const Product**) a;
	const Product* p2 = *(const Product**) b;
	return (int) (p1->price - p2->price);
}


void  sortProducts(SuperMarket* pMarket) {
	const SortCriteria criteria = selectSoringCriteria();
	qsort(pMarket->productArr, pMarket->productCount, sizeof(pMarket->productArr[0]), sortCriteriaComparators[criteria]);
	pMarket->productsArrSortedBy = criteria;
}


void findProduct(const SuperMarket* pMarket) {
	if (pMarket->productsArrSortedBy == NOT_SORTED) {
		printf("Search cannot be performed, array not sorted\n");
		return;
	}
	Product toSearch = {0};
	if (pMarket->productsArrSortedBy == SORT_BY_NAME) {
		const char* productNameToSearch = getStrExactLength("Enter product name");
		strcpy(toSearch.name, productNameToSearch);
	} else if (pMarket->productsArrSortedBy == SORT_BY_PRICE) {
		printf("Enter product price\n");
		scanf("%d", &toSearch.price);
	} else if (pMarket->productsArrSortedBy == SORT_BY_QUANTITY) {
		printf("Enter product count\n");
		scanf("%d", &toSearch.count);
	}
	Product* toSearchPtr = &toSearch;

	const Product** res = bsearch(&toSearchPtr, pMarket->productArr, pMarket->productCount,
		sizeof(Product**), sortCriteriaComparators[pMarket->productsArrSortedBy]);

	if (!res) {
		printf("Product not found\n");
		return;
	}

	printf("Product found\n");
	printProduct(*res);
}


void	printSuperMarket(const SuperMarket* pMarket)
{
	printf("Super Market Name: %s\t", pMarket->name);
	printf("\n");
	printAllProducts(pMarket);
	printf("\n");
	printAllCustomers(pMarket);
}

int		addProduct(SuperMarket* pMarket)
{
	char answer;
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = NULL;

	printf("\n");
	printf("Adding new product? y/Y: ");
	scanf("%c", &answer);
	getchar();

	if (toupper(answer) == 'Y')
	{
		if (!addNewProduct(pMarket))
		{
			free(pProd);
			return 0;
		}
		return 1;
	}
	else if (pMarket->productCount > 0)
	{
		printf("Do you want to increase the amount of an existing product? y/Y: ");
		scanf("%c", &answer);
		getchar();
		if (toupper(answer) == 'Y')
		{
			printAllProducts(pMarket);
			pProd = getProductFromUser(pMarket, barcode);
			if (pProd != NULL) //This barcode exist in stock
				updateProductCount(pProd);
		}
	}
	else
		return 0;
	
	return 1;
}

int		addNewProduct(SuperMarket* pMarket)
{

	Product** pTempArr = (Product**)realloc(pMarket->productArr, (pMarket->productCount + 1) * sizeof(Product*));
	if (!pTempArr)
	{
		freeProducts(pMarket);
		return 0;
	}
	pMarket->productArr = pTempArr;
	
	Product* pProd = (Product*)calloc(1, sizeof(Product));
	if (!pProd)
	{
		free(pMarket->productArr);
		return 0;
	}

	initProductNoBarcode(pProd);

	do
	{
		generateBarcode(pProd);
	} while (!isBarcodeUnique(pMarket, pProd->barcode)); //generate new barcode until it is unique
	
	pMarket->productArr[pMarket->productCount] = pProd;
	pMarket->productCount++;
	return 1;
}

int		isBarcodeUnique(const SuperMarket* pMarket, const char* barcode)
{
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (strcmp(pMarket->productArr[i]->barcode, barcode) == 0)
			return 0; // Barcode is not unique
	}
	return 1; // Barcode is unique
}

int isCustomerIdUnique(const SuperMarket* pMarket, const char* id)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (strcmp(pMarket->customerArr[i].id, id) == 0)
		{
			printf("ID %s is not unique\n", id);
			return 0; // ID is not unique
		}
	}
	return 1; // ID is unique
}

int		addCustomer(SuperMarket* pMarket)
{
	Customer cust = { 0 };

	do {
		freeCustomer(&cust);
		if (!initCustomer(&cust))
		{
			freeCustomer(&cust);
			return 0;
		}
	} while (!isCustomerIdUnique(pMarket, cust.id));

	if (isCustomerInMarket(pMarket, &cust))
	{
		printf("This customer already in market\n");
		free(cust.name);
		return 0;
	}

	pMarket->customerArr = (Customer*)safeRealloc(pMarket->customerArr, (pMarket->customerCount + 1) * sizeof(Customer));
	if (!pMarket->customerArr)
	{
		free(cust.name);
		return 0;
	}

	pMarket->customerArr[pMarket->customerCount] = cust;
	pMarket->customerCount++;
	return 1;
}

int		isCustomerInMarket(SuperMarket* pMarket, Customer* pCust)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (strcmp(pMarket->customerArr[i].name, pCust->name) == 0)
			return 1;
	}
	return 0;
}

int	doShopping(SuperMarket* pMarket)
{
	Customer* pCustomer = getCustomerShopPay(pMarket);
	if (!pCustomer)
		return 0;

	if (pCustomer->pCart == NULL)
	{
		pCustomer->pCart = (ShoppingCart*)malloc(sizeof(ShoppingCart));
		if (!pCustomer->pCart)
			return 0;
		initCart(pCustomer->pCart);
	}
	fillCart(pMarket, pCustomer->pCart);
	if (pCustomer->pCart->item_list.head.next == NULL) //did not buy any thing
	{
		free(pCustomer->pCart);
		pCustomer->pCart = NULL;
	}
	printf("---------- Shopping ended ----------\n");
	return 1;
}

Customer*	doPrintCart(SuperMarket* pMarket)
{
	Customer* pCustomer = getCustomerShopPay(pMarket);
	if (!pCustomer)
		return NULL;
	if (pCustomer->pCart == NULL)
	{
		printf("Customer cart is empty\n");
		return NULL;
	}
	printShoppingCartAndDisplayFinalBill(pCustomer);
	return pCustomer;
}

int	manageShoppingCart(SuperMarket* pMarket)
{
	Customer* pCustomer = doPrintCart(pMarket);
	char answer;

	if(!pCustomer)
		return 0;

	printf("Do you want to pay for the cart? y/Y, anything else to cancel shopping!\t");
	do {
		scanf("%c", &answer);
	} while (isspace(answer));

	getchar(); //clean the enter

	if (answer == 'y' || answer == 'Y')
		pay(pCustomer);
	else {
		clearCart(pMarket, pCustomer);
		cancelShopping(pCustomer);
	}
	return 1;

}

Customer*	getCustomerShopPay(SuperMarket* pMarket)
{
	if (pMarket->customerCount == 0)
	{
		printf("No customer listed to market\n");
		return NULL;
	}

	if (pMarket->productCount == 0)
	{
		printf("No products in market - cannot shop\n");
		return NULL;
	}

	Customer* pCustomer = getCustomerWhoShop(pMarket);
	if (!pCustomer)
	{
		printf("this customer is not listed\n");
		return NULL;
	}

	return pCustomer;
}

void	printAllProducts(const SuperMarket* pMarket)
{
	printf("There are %d products\n", pMarket->productCount);
	printf("%-20s %-10s\t", "Name", "Barcode");
	printf("%-20s %-10s %-20s %-15s\n", "Type", "Price", "Count In Stoke", "Expiry Date");
	printf("-------------------------------------------------------------------------------------------------\n");

	for (int i = 0; i < pMarket->productCount; i++)
		printProduct(pMarket->productArr[i]);
}

void	printAllCustomers(const SuperMarket* pMarket)
{
	printf("There are %d listed customers\n", pMarket->customerCount);
	for (int i = 0; i < pMarket->customerCount; i++)
		printCustomer(&pMarket->customerArr[i]);
}


Customer* getCustomerWhoShop(SuperMarket* pMarket)
{
	printAllCustomers(pMarket);
	char searchTerm[MAX_STR_LEN];
	getsStrFixSize(searchTerm, sizeof(searchTerm), "Who is shopping? Enter customer id\n");

	Customer* pCustomer = FindCustomerById(pMarket, searchTerm);
	
	return pCustomer;
}


void fillCart(SuperMarket* pMarket, ShoppingCart* pCart)
{
	printAllProducts(pMarket);
	char op;
	while (1)
	{
		printf("Do you want to shop for a product? y/Y, anything else to exit!!\t");
		do {
			scanf("%c", &op);
		} while (isspace(op));
		getchar(); //clean the enter
		if (op != 'y' && op != 'Y')
			break;
		int count;
		Product* pProd = getProductAndCount(pMarket, &count);
		if(pProd)
		{
			if (!addItemToCart(pCart, pProd->barcode, pProd->price, count))
			{
				printf("Error adding item\n");
				return;
			}
			pProd->count -= count; //item bought!!!
		}
	}
}

void clearCart(SuperMarket* pMarket, Customer* pCustomer)
{
	if (pCustomer->pCart == NULL)
		return;
	// for (int i = 0; i < pCustomer->pCart->count; i++)
	for (const NODE* tmp = pCustomer->pCart->item_list.head.next; tmp; tmp = tmp->next)
	{
		ShoppingItem* item = (ShoppingItem*) tmp->key;
		Product* pProd = getProductByBarcode(pMarket, item->barcode);
		if (pProd)
			pProd->count += item->count;
	}
}

Product* getProductAndCount(SuperMarket* pMarket, int* pCount)
{
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = getProductFromUser(pMarket, barcode);
	if (pProd == NULL)
	{
		printf("No such product\n");
		return NULL;
	} 
	
	if (pProd->count == 0)
	{
		printf("This product is out of stock\n");
		return NULL;
	}
	
	int count;
	do {
		printf("How many items do you want? max %d\n", pProd->count);
		scanf("%d", &count);
	} while (count <= 0 || count > pProd->count);
	*pCount = count;
	return pProd;
}

void	printProductByType(SuperMarket* pMarket)
{
	if (pMarket->productCount == 0)
	{
		printf("No products in market\n");
		return;
	}
	eProductType	type = getProductType();
	int count = 0;
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (pMarket->productArr[i]->type == type)
		{
			count++;
			printProduct(pMarket->productArr[i]);
		}
	}
	if (count == 0)
		printf("There are no product of type %s in market %s\n", getProductTypeStr(type), pMarket->name);
}

Product* getProductFromUser(SuperMarket* pMarket, char* barcode)
{
	getBarcodeCode(barcode);
	Product* pProd = getProductByBarcode(pMarket, barcode);
	if (!pProd)
	{
		printf("No such product barcode\n");
		return NULL;
	}
	return pProd;
}


int loadSuperMarket(SuperMarket* pMarket) {
	FILE* fp = fopen(BINARY_SAVE_FILE, "rb");
	if (!fp) {
		printf("Error open company file\n");
		return 0;
	}

	if (!loadMarketNameAndProductsFromBinary(pMarket, fp)) {
		printf("Error reading company file\n");
		fclose(fp);
		initEmptySuperMarket(pMarket);
		return 0;
	}

	fclose(fp);

	fp = fopen(TEXT_SAVE_FILE, "r");
	if (!fp) {
		printf("Error open customers file to write\n");
		freeProducts(pMarket);
		free(pMarket->name);
		initEmptySuperMarket(pMarket);
		return 0;
	}

	if (!loadCustomersFromTxt(pMarket, fp)) {
		printf("Error reading customers file\n");
		freeProducts(pMarket);
		free(pMarket->name);
		initEmptySuperMarket(pMarket);
		fclose(fp);
		return 0;
	}

	printf("Supermarket successfully loaded from files\n");
	fclose(fp);
	return 1;
}


int loadCustomersFromTxt(SuperMarket* pMarket, FILE* fp) {
	if (!fp)
		return 0;

	fscanf(fp, "%d", &pMarket->customerCount);
	fgetc(fp);
	pMarket->customerArr = (Customer*) malloc(sizeof(Customer) * pMarket->customerCount);
	if (!pMarket->customerArr) {
		return 0;
	}

	for (int i = 0; i < pMarket->customerCount; i++) {
		if (!loadCustomer(&pMarket->customerArr[i], fp)) {
			printf("Error loading customer\n");
			freeCustomers(pMarket);
			free(pMarket->customerArr);
			return 0;
		}
	}
	return 1;
}


int loadMarketNameAndProductsFromBinary(SuperMarket* pMarket, FILE* fp) {
	if (!fp) {
		return 0;
	}

	int len;
	if (fread(&len, sizeof(int), 1, fp) != 1)
		return 0;
	pMarket->name = (char*) malloc(len);
	if (!pMarket->name) {
		return 0;
	}
	if (fread(pMarket->name, sizeof(char), len, fp) != len) {
		return 0;
	}

	if (fread(&pMarket->productCount, sizeof(int), 1, fp) != 1) {
		free(pMarket->name);
		return 0;
	}

	pMarket->productArr = (Product**) malloc(sizeof(Product*) * pMarket->productCount);
	if (!pMarket->productArr) {
		free(pMarket->name);
		return 0;
	}

	for (int i = 0; i < pMarket->productCount; i++) {
		pMarket->productArr[i] = (Product*) malloc(sizeof(Product));
		if (!pMarket->productArr[i] || fread(pMarket->productArr[i], sizeof(Product), 1, fp) != 1) {
			freeProducts(pMarket);
			free(pMarket->name);
			return 0;
		}
	}

	return 1;

}


void finalizeSuperMarket(SuperMarket* pMarket) {
	saveMarketNameAndProductsToBinary(pMarket);
	saveCustomersToTxt(pMarket);
	freeMarket(pMarket);
}

int saveCustomersToTxt(const SuperMarket* pMarket) {
	FILE* fp = fopen(TEXT_SAVE_FILE, "w");
	if (!fp) {
		printf("Error opening save file\n");
		return 0;
	}
	fprintf(fp, "%d\n", pMarket->customerCount);
	for (int i = 0; i < pMarket->customerCount; i++) {
		saveCustomer(&pMarket->customerArr[i], fp);
	}
	fclose(fp);
	return 1;
}

int saveMarketNameAndProductsToBinary(const SuperMarket* pMarket) {
	FILE* file = fopen(BINARY_SAVE_FILE, "wb");
	if (!file) {
		printf("Error opening save file\n");
		return 0;
	}

	int len = (int) strlen(pMarket->name)+1;
	fwrite(&len, sizeof(int), 1, file);
	fwrite(pMarket->name, 1, len, file);
	saveProducts(pMarket, file);
	fclose(file);
	return 1;
}

void saveProducts(const SuperMarket* pMarket, FILE* fp) {
	fwrite(&pMarket->productCount, sizeof(int), 1, fp);
	for (int i = 0; i < pMarket->productCount; i++)
		fwrite(pMarket->productArr[i], sizeof(Product), 1, fp);
}

void	freeMarket(SuperMarket* pMarket)
{
	free(pMarket->name);
	freeProducts(pMarket);
	freeCustomers(pMarket);
}

void freeProducts(SuperMarket* pMarket)
{
	if (!pMarket->productArr)
		return;
	for (int i = 0; i < pMarket->productCount; i++) {
		if (pMarket->productArr[i] != NULL) {
			freeProduct(pMarket->productArr[i]);
			free(pMarket->productArr[i]);
		}
	}
	free(pMarket->productArr);
}

void freeCustomers(SuperMarket* pMarket)
{
	for (int i = 0; i < pMarket->customerCount; i++)
		freeCustomer(&pMarket->customerArr[i]);
	free(pMarket->customerArr);
}

void	getUniquBarcode(char* barcode, SuperMarket* pMarket)
{
	int cont = 1;
	while (cont)
	{
		getBarcodeCode(barcode);
		int index = getProductIndexByBarcode(pMarket, barcode);
		if (index == -1)
			cont = 0;
		else
			printf("This product already in market\n");
	}
}

int getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode)
{
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (isProduct(pMarket->productArr[i], barcode))
			return i;
	}
	return -1;
}

Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode)
{
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (isProduct(pMarket->productArr[i], barcode))
			return pMarket->productArr[i];
	}
	return NULL;

}


Customer* FindCustomerById(SuperMarket* pMarket, const char* id)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (isCustomerById(&pMarket->customerArr[i], id))
			return &pMarket->customerArr[i];
	}
	return  NULL;
}
