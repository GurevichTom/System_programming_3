//
// Created by Tom on 18.01.2025.
//

#ifndef CUSTOMERVTABLE_H
#define CUSTOMERVTABLE_H

struct Customer_;

typedef struct {
    void (*print) (const struct  Customer_*);
    void (*displayFinalBill) (float price, const struct  Customer_*);
    void (*save) (const struct Customer_*, FILE*);
} CustomerVTable;


#endif //CUSTOMERVTABLE_H
