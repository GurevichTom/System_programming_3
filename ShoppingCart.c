#include <stdio.h>
#include <string.h>
#include "ShoppingCart.h"


void	initCart(ShoppingCart* pCart)
{
	L_init(&pCart->item_list);
}

float	getTotalPrice(const ShoppingCart* pCart)
{
	printf("\n");
	float price = 0;
	for (const NODE* tmp = pCart->item_list.head.next; tmp; tmp = tmp->next) {
		const ShoppingItem* current_item = (ShoppingItem*) tmp->key;
		price += current_item->price * (float) current_item->count;
		printItem(current_item);
	}
	printf("\n");

	return price;
}

int		addItemToCart(ShoppingCart* pCart, const char* barcode, float price, int count)
{
	ShoppingItem* pItem = getItemByBarcode(pCart, barcode);
	if (!pItem) //new item
	{
		pItem = createItem(barcode, price, count);
		if (!pItem)
			return 0;
		// L_insert(&pCart->item_list.head, pItem);
		L_insertSorted(&pCart->item_list, pItem, compareItemBarcodes);
	}
	else {
		pItem->count += count;
	}
	return 1;
}

int		compareItemBarcodes(const void* it1, const void* it2) {
	const ShoppingItem* item1 = (ShoppingItem*) it1;
	const ShoppingItem* item2 = (ShoppingItem*) it2;
	return strcmp(item1->barcode, item2->barcode);
}

float	printShoppingCart(const ShoppingCart* pCart)
{
	const float price = getTotalPrice(pCart);
	// printf("Raw total: %.2f\n", price);
	return price;
}



ShoppingItem*	getItemByBarcode(const ShoppingCart* pCart, const char* barcode)
{
	const NODE* res = L_find(pCart->item_list.head.next, barcode, shoppingItemEqualsBarcode);

	if (!res)
		return NULL;

	return res->key;
}

int shoppingItemEqualsBarcode(const void* it, const void* barc) {
	const ShoppingItem* item = (ShoppingItem*) it;
	const char* barcode = (char*) barc;
	return strcmp(item->barcode, barcode) == 0;
}

void	freeShoppingCart(ShoppingCart* pCart)
{
	L_free(&pCart->item_list, NULL);
}