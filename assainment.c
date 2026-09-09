#include <stdio.h>

char names[6][20] = {
    "Oral Saline",
    "Paracetamol",
    "Insulin",
    "Amoxicillin",
    "Antacid",
    "Cetirizine"
};

void decoration() {
    printf("\n\n");
}

void idn_stock_condtion(int p[][8]) {
    printf("\nMedicine Stock Condition:\n");

    for (int i = 0; i < 6; i++) {

        if (p[i][1] > p[i][3] && p[i][4] <= 30)
            printf("Medicine%d (%s): Expiry Risk\n",
                   p[i][0], names[p[i][0]]);

        else if (p[i][1] > p[i][3] && p[i][4] > 30)
            printf("Medicine%d (%s): Sufficient Stock\n",
                   p[i][0], names[p[i][0]]);

        else if (p[i][1] <= p[i][3] &&
                 p[i][4] < 15 && p[i][5] == 3)
            printf("Medicine%d (%s): Critical Condition\n",
                   p[i][0], names[p[i][0]]);

        else if (p[i][1] <= p[i][3] && p[i][4] < 15)
            printf("Medicine%d (%s): Urgent Order\n",
                   p[i][0], names[p[i][0]]);

        else if (p[i][1] <= p[i][3] && p[i][4] >= 15)
            printf("Medicine%d (%s): Reorder Required\n",
                   p[i][0], names[p[i][0]]);
    }
}

int Points(int stock, int min_stock, int expiry,
           int essentiality, float coverage) {

    int points = 0;

    if (stock > min_stock)
        points += 0;
    else if (stock == min_stock)
        points += 1;
    else
        points += 2;

    if (essentiality == 3)
        points += 3;
    else if (essentiality == 2)
        points += 2;
    else
        points += 1;

    if (coverage <= 3)
        points += 2;
    else if (coverage <= 7)
        points += 1;

    if (expiry <= 15)
        points += 3;
    else if (expiry < 30)
        points += 2;

    return points;
}

void priority_level(int p[][8]) {

    printf("\nPriority Level (Highest to Lowest):\n");

    for (int i = 0; i < 6; i++) {

        p[i][6] = p[i][1] / p[i][2];

        p[i][7] = Points(
            p[i][1],
            p[i][3],
            p[i][4],
            p[i][5],
            p[i][6]
        );
    }

    for (int i = 0; i < 6; i++) {

        for (int j = i + 1; j < 6; j++) {

            if (p[j][7] > p[i][7]) {

                int temp[8];

                for (int k = 0; k < 8; k++) {
                    temp[k] = p[i][k];
                    p[i][k] = p[j][k];
                    p[j][k] = temp[k];
                }
            }
        }
    }

    for (int i = 0; i < 6; i++) {

        printf(
            "Priority Level %d = medCode%d (%s), Points = %d\n",
            i + 1,
            p[i][0],
            names[p[i][0]],
            p[i][7]
        );
    }
}

void shortage_expiry(int p[][8]) {

    printf("\nShortage and Expiry Analysis:\n");

    for (int i = 0; i < 6; i++) {

        if (p[i][1] > p[i][3] && p[i][4] <= 30)

            printf(
                "medCode%d (%s) = Expiry Risk\n",
                p[i][0], names[p[i][0]]
            );

        else if (p[i][1] > p[i][3] && p[i][4] > 30)

            printf(
                "medCode%d (%s) = No Shortage & No Expiry Risk\n",
                p[i][0], names[p[i][0]]
            );

        else if (p[i][1] <= p[i][3] && p[i][4] <= 30)

            printf(
                "medCode%d (%s) = Shortage & Expiry Risk\n",
                p[i][0], names[p[i][0]]
            );

        else

            printf(
                "medCode%d (%s) = Shortage Risk\n",
                p[i][0], names[p[i][0]]
            );
    }
}

int main() {

    int med[6][8] = {

        {0, 120, 35, 80, 45, 3},
        {1, 300, 40, 100, 120, 2},
        {2, 60, 12, 50, 30, 3},
        {3, 200, 25, 80, 20, 3},
        {4, 250, 18, 60, 15, 1},
        {5, 180, 15, 50, 90, 1}
    };

    decoration();

    printf("MEDICINE STOCK AND EXPIRY MANAGEMENT\n");

    decoration();

    printf("\nList of Medicine:\n");

    for (int i = 0; i < 6; i++)
        printf("medCode%d = %s\n", i, names[i]);


    printf("\nInitial Medicine Information:\n");

    printf(
        "Code\tMedicine Name\tStock\tDaily Req\tMin Stock\tExpiry\tEssentiality\n"
    );

    for (int i = 0; i < 6; i++) {

        printf(
            "%d\t%-15s\t%d\t%d\t\t%d\t\t%d\t%d\n",
            med[i][0],
            names[med[i][0]],
            med[i][1],
            med[i][2],
            med[i][3],
            med[i][4],
            med[i][5]
        );
    }

    idn_stock_condtion(med);

    priority_level(med);

    shortage_expiry(med);


    int search;

    printf("\nEnter medicine code to search (0-5): ");

    scanf("%d", &search);

    if (search >= 0 && search < 6) {

        printf("\nMedicine Information:\n");

        printf("Medicine Name: %s\n", names[search]);
        printf("Stock: %d\n", med[search][1]);
        printf("Daily Requirement: %d\n", med[search][2]);
        printf("Minimum Stock: %d\n", med[search][3]);
        printf("Expiry Days: %d\n", med[search][4]);
        printf("Essentiality: %d\n", med[search][5]);

    } else {

        printf("Medicine not found.\n");
    }


    int choice, amount;

    printf("\nDo you want to update stock?\n");
    printf("1. Add stock\n");
    printf("2. Issue stock\n");
    printf("3. No update\n");

    printf("Enter choice: ");
    scanf("%d", &choice);


    if (choice == 1) {

        printf("Enter quantity to add: ");
        scanf("%d", &amount);

        if (amount >= 0)
            med[search][1] += amount;

        else
            printf("Invalid quantity.\n");
    }

    else if (choice == 2) {

        printf("Enter quantity to issue: ");
        scanf("%d", &amount);

        if (amount >= 0 &&
            amount <= med[search][1])

            med[search][1] -= amount;

        else
            printf(
                "Invalid quantity. Stock cannot become negative.\n"
            );
    }


    if (search >= 0 && search < 6) {

        med[search][6] =
            med[search][1] / med[search][2];

        med[search][7] =
            Points(
                med[search][1],
                med[search][3],
                med[search][4],
                med[search][5],
                med[search][6]
            );

        printf("\nUpdated Medicine: %s\n",
               names[search]);

        printf("Updated Stock = %d\n",
               med[search][1]);

        printf("Updated Coverage = %d days\n",
               med[search][6]);

        printf("Updated Priority Points = %d\n",
               med[search][7]);
    }


    decoration();

    printf("Final Priority List:\n");

    priority_level(med);

    return 0;
}