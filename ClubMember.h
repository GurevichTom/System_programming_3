//
// Created by Tom on 18.01.2025.
//

#ifndef CLUBMEMBER_H
#define CLUBMEMBER_H
#include <stdio.h>

#include "Customer.h"

typedef struct {
    int totalMonths;
    Customer* pBase;
} ClubMember;

int initClubMember(Customer* pCustomer);
void printClubMember(const Customer* pCustomer);
float calculateDiscount(const Customer* pCustomer);
void displayFinalBillToPayClubMember(float price, const Customer* pCustomer);
void clubMemberSaveMembership(const Customer* pCustomer, FILE* fp);
void initClubMemberVTable(Customer* pCustomer);


#endif //CLUBMEMBER_H
