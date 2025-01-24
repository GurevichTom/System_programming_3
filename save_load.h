//
// Created by Tom on 24.01.2025.
//

#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H
#include <stdio.h>

#define BINARY_SAVE_FILE "SuperMarket.bin"
#define TEXT_SAVE_FILE "SuperMarket.txt"

struct SuperMarket;

int saveMarketNameAndProductsToBinary(const struct SuperMarket* pMarket);
void saveProducts(const struct SuperMarket* pMarket, FILE* fp);
int saveCustomersToTxt(const struct SuperMarket* pMarket);

int loadSuperMarket(struct SuperMarket* pMarket);
int loadMarketNameAndProductsFromBinary(struct SuperMarket* pMarket, FILE* fp);
int loadCustomersFromTxt(struct SuperMarket* pMarket, FILE* fp);

#endif //SAVE_LOAD_H
