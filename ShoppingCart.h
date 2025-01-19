#pragma once
#include "ShoppingItem.h"
#include "list.h"

typedef struct
{
	LIST item_list;
}ShoppingCart;


void			initCart(ShoppingCart* pCart);
float			getTotalPrice(const ShoppingCart* pCart);
ShoppingItem*	getItemByBarcode(const ShoppingCart* pCart, const char* barcode);
int				addItemToCart(ShoppingCart* pCart, const char* barcode, float price, int count);
float			printShoppingCart(const ShoppingCart* pCart);
int				shoppingItemEqualsBarcode(const void* it, const void* barc);
int				compareItemBarcodes(const void* it1, const void* it2);

void			freeShoppingCart(ShoppingCart* pCart);