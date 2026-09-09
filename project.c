#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ZONES 6
#define COLS 9

void rankZones(float water[][COLS], char code[][10]);
void processWater(float water[][COLS], char code[][10], float *totalWater);
void searchZone(float water[][COLS], char code[][10]);

int main()
{
    char code[ZONES][10] =
    {
        "Z01", "Z02", "Z03", "Z04", "Z05", "Z06"
    };

    float water[ZONES][COLS] =
    {
        {4000, 3000,  2, 0, 0, 0, 0, 0, 0},
        {3500, 2500,  4, 1, 0, 0, 0, 0, 0},
        {4500, 2000,  8, 2, 0, 0, 0, 0, 0},
        {3800, 1800, 12, 3, 0, 0, 0, 0, 0},
        {2600, 1600,  5, 1, 0, 0, 0, 0, 0},
        {2000, 1500,  1, 0, 0, 0, 0, 0, 0}
    };

    float totalWater = 13500;

    rankZones(water, code);
    processWater(water, code, &totalWater);
    searchZone(water, code);

    return 0;
}


// sorting

void rankZones(float water[][COLS], char code[][10])
{
    int i, j, k;
    float temp;
    char tempCode[10];

    /* Calculate priority */
    for(i = 0; i < ZONES; i++)
    {
        water[i][4] = water[i][3] * 10;
    }

    /* Sort by priority - highest first */
    for(i = 0; i < ZONES - 1; i++)
    {
        k = i;

        for(j = i + 1; j < ZONES; j++)
        {
            if(water[j][4] > water[k][4])
            {
                k = j;
            }
        }

        if(k != i)
        {
            /* Swap complete row */
            for(j = 0; j < COLS; j++)
            {
                temp = water[i][j];
                water[i][j] = water[k][j];
                water[k][j] = temp;
            }

            /* Swap zone name */
            strcpy(tempCode, code[i]);
            strcpy(code[i], code[k]);
            strcpy(code[k], tempCode);
        }
    }

    printf("\n\n");
    printf("1. ZONE PRIORITY INFORMATION\n\n");

    printf("%-6s %-12s %-12s %-8s %-10s %-8s\n",
           "Zone", "Requested", "Minimum",
           "Loss%", "Waiting", "Priority");

    printf("\n");

    for(i = 0; i < ZONES; i++)
    {
        printf("%-6s %-12.2f %-12.2f %-8.1f %-10.0f %-8.0f\n",
               code[i],
               water[i][0],
               water[i][1],
               water[i][2],
               water[i][3],
               water[i][4]);
    }
}


//ALL WATER PROCESSING 

void processWater(float water[][COLS],
                  char code[][10],
                  float *totalWater)
{
    int i;
    float extra, lost;
    float totalRequested = 0;
    float totalAllocated = 0;
    float totalDelivered = 0;
    float totalShortage = 0;

// Minimum allocation

    printf("\n\n");
    printf("2. MINIMUM WATER ALLOCATION\n\n");

    for(i = 0; i < ZONES; i++)
    {
        if(water[i][1] <= *totalWater)
        {
            water[i][5] = water[i][1];
            *totalWater -= water[i][1];
        }
        else
        {
            water[i][5] = 0;
        }
    }

    printf("%-6s %-20s %-15s\n",
           "Zone", "Minimum Required", "Allocated");

    printf("\n");

    for(i = 0; i < ZONES; i++)
    {
        printf("%-6s %-20.2f %-15.2f\n",
               code[i],
               water[i][1],
               water[i][5]);
    }

    printf("\nRemaining Water: %.2f L\n", *totalWater);


    //Remaining 

    printf("\n\n");
    printf("3. Remaining WATER ALLOCATION\n\n");

    printf("%-6s %-12s %-12s %-15s\n",
           "Zone", "Requested", "Extra", "Total Allocated");

    printf("\n");

    for(i = 0; i < ZONES; i++)
    {
        extra = water[i][0] - water[i][1];

        if(extra <= *totalWater)
        {
            water[i][5] += extra;
            *totalWater -= extra;
        }
        else if(*totalWater > 0)
        {
            water[i][5] += *totalWater;
            *totalWater = 0;
        }
    }

    for(i = 0; i < ZONES; i++)
    {
        printf("%-6s %-12.2f %-12.2f %-15.2f\n",
               code[i],
               water[i][0],
               water[i][0] - water[i][1],
               water[i][5]);
    }


     //Loss & delivery 

    printf("\n\n");
    printf("4. WATER LOSS & DELIVERY\n\n");

    printf("%-6s %-15s %-15s %-15s\n",
           "Zone", "Allocated Water",
           "Loss %", "Delivered Water");

    printf("\n");

    for(i = 0; i < ZONES; i++)
    {
        lost = water[i][5] * water[i][2] / 100.0;

        water[i][6] = water[i][5] - lost;

        printf("%-6s %-15.2f %-15.2f %-15.2f\n",
               code[i],
               water[i][5],
               water[i][2],
               water[i][6]);
    }


    // final update

    printf("\n\n");
    printf("5. FINAL WATER STATUS\n\n");

    printf("%-6s %-15s %-15s %-15s\n",
           "Zone", "Requested",
           "Delivered", "Shortage");

    printf("\n");

    for(i = 0; i < ZONES; i++)
    {
        water[i][7] = water[i][0] - water[i][6];

        printf("%-6s %-15.2f %-15.2f %-15.2f\n",
               code[i],
               water[i][0],
               water[i][6],
               water[i][7]);
    }


    //total

    for(i = 0; i < ZONES; i++)
    {
        totalRequested += water[i][0];
        totalAllocated += water[i][5];
        totalDelivered += water[i][6];
        totalShortage += water[i][7];
    }

    printf("\n\n");
    printf("6. TOTAL SUMMARY\n\n");

    printf("Total Requested :%.1f\n", totalRequested);
    printf("Total Allocated :%.1f\n", totalAllocated);
    printf("Total Delivered :%.1f\n", totalDelivered);
    printf("Total Shortage  :%.1f\n", totalShortage);


    /* ---------- Zone condition ---------- */

    printf("\n\n");
    printf("7. FINAL ZONE CONDITION\n\n");

    printf("%-6s   %-15s\n", "Zone", "Condition");

    for(i = 0; i < ZONES; i++)
    {
        if(water[i][5] == 0)
        {
            water[i][8] = 1;
        }
        else if(water[i][6] >= water[i][0])
        {
            water[i][8] = 2;
        }
        else if(water[i][6] >= water[i][1])
        {
            water[i][8] = 3;
        }
        else
        {
            water[i][8] = 4;
        }

        if(water[i][8] == 1)
            printf("%-6s : %-15s\n", code[i], "Unserved");

        else if(water[i][8] == 2)
            printf("%-6s : %-15s\n", code[i], "Full");

        else if(water[i][8] == 3)
            printf("%-6s : %-15s\n",
                   code[i], "Minimum Satisfied");

        else
            printf("%-6s : %-15s\n",
                   code[i], "Below Satisfied");
    }
}


/* ================= SEARCH ================= */

void searchZone(float water[][COLS], char code[][10])
{
    char target[10];
    int i, found = 0;
    int highest = 0;

    printf("\n\n");
    printf("8. ZONE SEARCH\n\n");

    printf("Enter zone ID: ");
    scanf("%9s", target);

    /* Convert input to uppercase */
    for(i = 0; target[i] != '\0'; i++)
    {
        target[i] = toupper((unsigned char)target[i]);
    }

    /* Search zone */
    for(i = 0; i < ZONES; i++)
    {
        if(strcmp(target, code[i]) == 0)
        {
            printf("\nZone: %s\n", code[i]);
            printf("Requested: %.2f\n", water[i][0]);
            printf("Allocated: %.2f\n", water[i][5]);
            printf("Delivered: %.2f\n", water[i][6]);
            printf("Shortage: %.2f\n", water[i][7]);

            found = 1;
            break;
        }
    }

    if(found == 0)
    {
        printf("\nZone not found.\n");
    }


    // Highest shortage 

    for(i = 1; i < ZONES; i++)
    {
        if(water[i][7] > water[highest][7])
        {
            highest = i;
        }
    }

    printf("\n");
    printf("9. HIGHEST SHORTAGE\n\n");

    printf("Highest Shortage Zone: %s\n",
           code[highest]);

    printf("Highest Shortage: %.2f\n",
           water[highest][7]);
}