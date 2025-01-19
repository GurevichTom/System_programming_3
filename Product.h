#pragma once
#include "Date.h"

typedef enum {eFruitVegtable, eFridge, eFrozen, eShelf, eNofProductType} eProductType;

#define NAME_LENGTH 20
#define BARCODE_LENGTH 7
#define MIN_BARCODE 10000
#define MAX_BARCODE 99999
#define PREFIX_LENGTH 2
#define BARCODE_DIGITS_LENGTH 5

typedef struct
{
	char			name[NAME_LENGTH+1];
	char			barcode[BARCODE_LENGTH + 1];
	eProductType	type;
	float			price;
	int				count;
	Date			expiryDate;
}Product;

void			initProduct(Product* pProduct);
void			initProductNoBarcode(Product* pProduct);
void			initProductName(Product* pProduct);
void			printProduct(const Product* pProduct);

void			generateBarcode(Product* pProd);
void			getBarcodeCode(char* code);
int				isProduct(const Product* pProduct,const char* barcode);
eProductType	getProductType();
const char*		getProductTypeStr(eProductType type);
const char*		getProductTypePrefix(eProductType type);

void			updateProductCount(Product* pProduct);
void			freeProduct(Product* pProduct);