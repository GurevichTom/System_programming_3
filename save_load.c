//
// Created by Tom on 24.01.2025.
//

#include "save_load.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Customer.h"
#include "Supermarket.h"

int saveCustomersToTxt(const struct SuperMarket* pMarket) {
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

int saveMarketNameAndProductsToBinary(const struct SuperMarket* pMarket) {
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

void saveProducts(const struct SuperMarket* pMarket, FILE* fp) {
    fwrite(&pMarket->productCount, sizeof(int), 1, fp);
    for (int i = 0; i < pMarket->productCount; i++)
        fwrite(pMarket->productArr[i], sizeof(Product), 1, fp);
}



int loadSuperMarket(struct SuperMarket* pMarket) {
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


int loadCustomersFromTxt(struct SuperMarket* pMarket, FILE* fp) {
	if (!fp)
		return 0;

	if (fscanf(fp, "%d", &pMarket->customerCount) != 1)
		return 0;
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


int loadMarketNameAndProductsFromBinary(struct SuperMarket* pMarket, FILE* fp) {
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