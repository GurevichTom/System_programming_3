//
// Created by Tom on 18.01.2025.
//

#include "ClubMember.h"

#include <stdio.h>
#include <stdlib.h>

#include "General.h"


int initClubMember(Customer* pCustomer) {
    if (!initBaseCustomer(pCustomer))
        return 0;

    pCustomer->pDerived = (ClubMember*) malloc(sizeof(ClubMember));
    if (!pCustomer->pDerived) {
        freeCustomer(pCustomer);
        return 0;
    }

    ClubMember* the_club_member = (ClubMember*) pCustomer->pDerived;
    the_club_member->totalMonths = getPositiveInt("Please enter the total months of membership");
    initClubMemberVTable(pCustomer);

    return 1;
}


void initClubMemberVTable(Customer* pCustomer) {
    pCustomer->vTable.print = printClubMember;
    pCustomer->vTable.displayFinalBill = displayFinalBillToPayClubMember;
    pCustomer->vTable.save = clubMemberSaveMembership;
}


float calculateDiscount(const Customer* pCustomer) {
    const ClubMember* clubMember = (ClubMember*) pCustomer->pDerived;
    if (!clubMember)
        return 0;

    const int totalMonths = clubMember->totalMonths;
    const int totalYears = totalMonths / 12;

    if (totalYears < 2)
        return totalMonths * 0.1f;
    if (totalYears < 5)
        return 2.5f + 0.5f * totalYears;
    return 7.5f;
}

void displayFinalBillToPayClubMember(const float price, const Customer* pCustomer) {
    const float discountPercent = calculateDiscount(pCustomer);
    const float discountAmount = price * (discountPercent / 100.0f);
    const float finalPrice = price - discountAmount;
    // if (discountPercent > 0) {
    //     printf("Club discount: %.2f%% (%.2f off)\n", discountPercent, discountAmount);
    // }
    // printf("Final bill to pay: %.2f\n", finalPrice);
    printf("Total price for %s is %f, after discount of %f%%\n", pCustomer->name, finalPrice, discountPercent);
}

void printClubMember(const Customer* pCustomer) {
    printBaseCustomer(pCustomer);
    const ClubMember* clubMember = (ClubMember*) pCustomer->pDerived;
    if (clubMember)
        printf("Total months of membership: %d\n", clubMember->totalMonths);
}

void clubMemberSaveMembership(const Customer* pCustomer, FILE* fp) {
    const ClubMember* clubMember = (ClubMember*) pCustomer->pDerived;
    if (clubMember) {
        fprintf(fp, "1 %d\n", clubMember->totalMonths);
    }
}