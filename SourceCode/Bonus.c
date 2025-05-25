#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// define the constants
const int X = 300;
const int Y = 400;
const int Z = 500;
const int D = 1200;

const int shit2zero[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

struct Order
{
    int ContainedOrder; // the number of contained orders
    char *OrderName;    // the name of the order
    char *ProductName;  // the name of the product
    int *DDL;           // the deadline of the order
    int *Quantity;      // the quantity of the order

    int Remainder;   // the remainder of the order
    int Days;        // the number of days to finish the order
    int LatestStart; // the latest start date of the order

    char ***Schedule; // [[Prodect for X, Product for Y, Product for Z], [X, Y, Z], [X, Y, Z], ...]
    int days_X;
    int days_Y;
    int days_Z;
    int startDay_X;
    int startDay_Y;
    int startDay_Z;
    int output_X;
    int output_Y;
    int output_Z;
};

int calculateDate(char *Year, char *Month, char *Day);
struct Order *initOrder();
void printOrder(struct Order *order);
void parseFile(char *fileName);
void CHAISchedule();
int checkRejectOrNot(struct Order *Bullshit, int *checkPlant, int TempDays);
struct Order *CHAICombine(struct Order **orders);
void run();

int i, j, k, l, m, n, o;

int ScheduleStartDay = 0;
int ScheduleEndDay = 0;
struct Order *Orders;
int OrderCount = 0;
int Plant_XUseDays = 0;
int Plant_XRealUsage = 0;
int Plant_XPerfectUsage = 0;
int Plant_YUseDays = 0;
int Plant_YRealUsage = 0;
int Plant_YPerfectUsage = 0;
int Plant_ZUseDays = 0;
int Plant_ZRealUsage = 0;
int Plant_ZPerfectUsage = 0;

struct Order *initOrder()
{
    struct Order *order = (struct Order *)malloc(sizeof(struct Order));
    order->ContainedOrder = 1;
    order->OrderName = (char *)malloc(sizeof(char));
    order->ProductName = (char *)malloc(sizeof(char));
    order->DDL = (int *)malloc(sizeof(int));
    order->Quantity = (int *)malloc(sizeof(int));
    order->Schedule = NULL;

    order->ContainedOrder = 0;
    order->OrderName[0] = '\0';
    order->ProductName[0] = '\0';
    order->DDL[0] = 0;
    order->Quantity[0] = 0;
    order->Remainder = 0;
    order->Days = 0;
    order->days_X = 0;
    order->days_Y = 0;
    order->days_Z = 0;
    order->startDay_X = 0;
    order->startDay_Y = 0;
    order->startDay_Z = 0;
    order->output_X = 0;
    order->output_Y = 0;
    order->output_Z = 0;
    // latest start of empty order is MAX of int
    order->LatestStart = 2147483647;
    return order;
}

void printOrder(struct Order *order)
{
    int i, j;
    printf("*** ");
    printf("ContainedOrder: %d  ", order->ContainedOrder);
    printf("OrderName: %s  ", order->OrderName);
    printf("ProductName: %s  ", order->ProductName);
    printf("DDL: ");
    for (i = 0; i < order->ContainedOrder; i++)
    {
        printf("%d ", order->DDL[i]);
    }
    printf("Quantity: ");
    for (i = 0; i < order->ContainedOrder; i++)
    {
        printf("%d ", order->Quantity[i]);
    }
    printf("Remainder: %d  ", order->Remainder);
    printf("Days: %d  ", order->Days);
    printf("LatestStart: %d  ", order->LatestStart);

    printf("Schedule: ");
    if (order->Schedule != NULL)
    {
        for (i = 0; i < order->Days; i++)
        {
            printf("\n[");
            for (j = 0; j < 3; j++)
            {
                printf("%s ", order->Schedule[i][j]);
            }
            printf("]");
        }
    }
    else
    {
        printf("NULL");
    }

    printf("days_X: %d  ", order->days_X);
    printf("days_Y: %d  ", order->days_Y);
    printf("days_Z: %d  ", order->days_Z);
    printf("startDay_X: %d  ", order->startDay_X);
    printf("startDay_Y: %d  ", order->startDay_Y);
    printf("startDay_Z: %d  ", order->startDay_Z);
    printf("output_X: %d  ", order->output_X);
    printf("output_Y: %d  ", order->output_Y);
    printf("output_Z: %d  ", order->output_Z);

    printf("***\n");
}

// sparse the file
void parseFile(char *fileName)
{
    FILE *file = fopen(fileName, "r");

    if (file == NULL)
    {
        // printf("Error opening file!\n");
        return;
    }
    char line[1000];
    char *lines[1000];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *token, *Year, *Month, *Day;
        // check if the line start with "addRERIOD" or "addORDER"
        if (strncmp(line, "addPERIOD", 9) == 0)
        {
            // //printf("this is a period\n");
            // parse the line
            strtok(line, " ");               // remove the first token
            char *Start = strtok(NULL, " "); // get the start date
            char *End = strtok(NULL, " ");   // get the end date

            // get the start
            Year = strtok(Start, "-");
            Month = strtok(NULL, "-");
            Day = strtok(NULL, "-");
            ScheduleStartDay = calculateDate(Year, Month, Day);

            // get the end
            Year = strtok(End, "-");
            Month = strtok(NULL, "-");
            Day = strtok(NULL, "-");
            ScheduleEndDay = calculateDate(Year, Month, Day);
            // //printf("111 ScheduleStartDay: %d\n", ScheduleStartDay);
        }
        else if (strncmp(line, "addORDER", 8) == 0)
        {
            // //printf("this is an order\n");
            // parse the line
            line[strlen(line) - 1] = '\0';         // remove the new line character
            strtok(line, " ");                     // remove the first token
            char *OrderName = strtok(NULL, " ");   // get the order name
            char *DDL = strtok(NULL, " ");         // get the DDL
            char *Quantity = strtok(NULL, " ");    // get the quantity
            char *ProductName = strtok(NULL, " "); // get the product name

            // create a new order
            struct Order *newOrder = initOrder();
            newOrder->ContainedOrder = 1;
            // get the order strings
            strcpy(newOrder->OrderName, OrderName);
            strcpy(newOrder->ProductName, ProductName);

            // get the DDL
            Year = strtok(DDL, "-");
            Month = strtok(NULL, "-");
            Day = strtok(NULL, "-");
            newOrder->DDL[0] = calculateDate(Year, Month, Day);

            // get the quantity
            newOrder->Quantity[0] = atoi(Quantity);
            newOrder->Remainder = newOrder->Quantity[0] % D;
            newOrder->Days = newOrder->Quantity[0] / D;

            // calculate the latest start
            newOrder->LatestStart = newOrder->DDL[0] - newOrder->Days + (newOrder->Remainder == 0 ? 1 : 0);

            // printOrder(newOrder);

            // add the order to the list
            Orders = (struct Order *)realloc(Orders, (OrderCount + 1) * sizeof(struct Order));
            Orders[OrderCount] = *newOrder;
            OrderCount++;
        }
    }
    fclose(file);
}

int calculateDate(char *Year, char *Month, char *Day)
{
    // INPUT FORMAT: YYYY-MM-DD
    // OUTPUT: days from 0-0-0
    int Y = (Year[0] - '0') * 1000 + (Year[1] - '0') * 100 + (Year[2] - '0') * 10 + (Year[3] - '0');
    int M = (Month[0] - '0') * 10 + (Month[1] - '0');
    int D = (Day[0] - '0') * 10 + (Day[1] - '0');
    // decide the number of days between the two datesw
    int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int days = 0;
    int i;
    for (i = 0; i < Y; i++)
    {
        if ((i % 4 == 0 && i % 100 != 0) || i % 400 == 0)
        {
            days += 366;
        }
        else
        {
            days += 365;
        }
    }
    for (i = 0; i < M - 1; i++)
    {
        if ((Y % 4 == 0 && Y % 100 != 0) || Y % 400 == 0)
        {
            days += daysInMonth[i];
            if (i == 1)
            {
                days++;
            }
        }
        else
        {
            days += daysInMonth[i];
        }
    }
    days += D;
    // //printf("Year: %d Month: %d Day: %d\n", Y, M, D);
    // //printf("Days: %d\n", days);
    // char output[11];
    // calculateBackDate(days, output);
    // //printf("Output: %s\n", output);
    return days;
}

void calculateBackDate(int days, char *output)
{
    int year = 0, month = 1, day = 1;
    int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    while (days > 365)
    {
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        {
            if (days > 366)
            {
                days -= 366;
            }
            else
            {
                break;
            }
        }
        else
        {
            days -= 365;
        }
        year++;
    }

    while (days > 0)
    {
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        {
            daysInMonth[1] = 29;
        }
        else
        {
            daysInMonth[1] = 28;
        }

        if (days > daysInMonth[month - 1])
        {
            days -= daysInMonth[month - 1];
            month++;
        }
        else
        {
            day = days;
            break;
        }
    }
    sprintf(output, "%04d-%02d-%02d", year, month, day);
}

void generateBonusReport(struct Order Accepted[], int AcceptedCount, struct Order Rejected[], int RejectedCount, int Plant_XUseDays, int Plant_YUseDays, int Plant_ZUseDays, int Plant_XRealUsage, int Plant_YRealUsage, int Plant_ZRealUsage, int Plant_XPerfectUsage, int Plant_YPerfectUsage, int Plant_ZPerfectUsage)
{
    FILE *bonusFile = fopen("bonusFile.txt", "w");
    if (bonusFile == NULL)
    {
        // printf("Failed to open the file\n");
        return;
    }
    int RealAcceptedCount = 0;
    for (i = 0; i < AcceptedCount; i++)
    {
        RealAcceptedCount += Accepted[i].ContainedOrder;
    }
    fprintf(bonusFile, "***PLS Schedule Analysis Report***\n\n");
    fprintf(bonusFile, "Algorithm used: CHAI Algorithm\n\n");
    fprintf(bonusFile, "There are %d Orders ACCEPTED. Details are as follows:\n\n", RealAcceptedCount);
    fprintf(bonusFile, "%15s%15s%15s%15s%15s%15s\n", "ORDER NUMBER", "START", "END", "DAYS", "QUANTITY", "PLANT");
    fprintf(bonusFile, "==========================================================================================\n");
    // print the accepted orders
    // printf("AcceptedCount: %d\n", AcceptedCount);
    // printf("\nplant_X: %d/%d\n", Plant_XRealUsage, Plant_XPerfectUsage);
    // printf("plant_Y: %d/%d\n", Plant_YRealUsage, Plant_YPerfectUsage);
    // printf("plant_Z: %d/%d\n", Plant_ZRealUsage, Plant_ZPerfectUsage);

    for (i = 0; i < AcceptedCount; i++)
    {
        char *OrderName[3]; // split the order name with space
        char *token = strtok(Accepted[i].OrderName, " ");
        for (j = 0; j < Accepted[i].ContainedOrder; j++)
        {
            OrderName[j] = token;
            token = strtok(NULL, " ");
        }

        for (j = 0; j < Accepted[i].ContainedOrder; j++)
        {
            // printOrder(&Accepted[i]);
            if (Accepted[i].Remainder == 0)
            {
                // this is a perfect order
                // printOrder(&Accepted[i]);
                int startDay_X = Accepted[i].startDay_X;
                int startDay_Y = Accepted[i].startDay_Y;
                int startDay_Z = Accepted[i].startDay_Z;
                Accepted[i].days_X = 0;
                Accepted[i].days_Y = 0;
                Accepted[i].days_Z = 0;

                // printf("\nPerfect Order: %s\n", Accepted[i].OrderName);
                //  printf("Order Name: %s: startDay_X: %d, startDay_Y: %d, startDay_Z: %d\n", OrderName[j], startDay_X, Accepted[i].startDay_Y, Accepted[i].startDay_Z);
                for (k = 0; k < Accepted[i].Days; k++)
                { // check plant X
                    if (strcmp(OrderName[j], Accepted[i].Schedule[k][0]) == 0)
                    {                              // check whether the order is in this schedule day
                        Accepted[i].days_X += 1;   // increase the days
                        Accepted[i].output_X += X; // increase the output
                        Plant_XRealUsage += X;
                        Plant_XPerfectUsage += X;
                        Plant_XUseDays += 1;

                        // printf("Accepted[i].output_X: %d\n", Accepted[i].output_X);
                    }
                    else
                    {
                        // print if the order days x is greater than 0
                        if (Accepted[i].days_X > 0)
                        {
                            char realStartDate[11];
                            char realEndDate[11];
                            calculateBackDate(startDay_X, realStartDate);
                            calculateBackDate(startDay_X + Accepted[i].days_X - 1, realEndDate);
                            // printf("Ordername: %s\n", OrderName[j]);
                            // printf("realStartDate: %s\n", realStartDate);
                            // printf("realEndDate: %s\n", realEndDate);
                            fprintf(bonusFile, "%15s%15s%15s%15d%15d%15s\n", OrderName[j], realStartDate, realEndDate, Accepted[i].days_X, Accepted[i].output_X, "Plant_X");

                            startDay_X += Accepted[i].days_X;
                            Accepted[i].days_X = 0;
                            Accepted[i].output_X = 0;
                        }

                        startDay_X += 1;
                    }
                }
                if (Accepted[i].days_X > 0)
                {
                    char realStartDate[11];
                    char realEndDate[11];
                    calculateBackDate(startDay_X, realStartDate);
                    calculateBackDate(startDay_X + Accepted[i].days_X - 1, realEndDate);
                    // printf("Ordername: %s\n", OrderName[j]);
                    // printf("realStartDate: %s\n", realStartDate);
                    // printf("realEndDate: %s\n", realEndDate);
                    fprintf(bonusFile, "%15s%15s%15s%15d%15d%15s\n", OrderName[j], realStartDate, realEndDate, Accepted[i].days_X, Accepted[i].output_X, "Plant_X");
                }

                // check plant Y
                // printf("Ordername: %s plant Y\n", OrderName[j]);
                for (k = 0; k < Accepted[i].Days; k++)
                {
                    // printf("k: %d; startDay_Y: %d; Accepted[i].days_Y: %d\n", k, startDay_Y, Accepted[i].days_Y);
                    if (strcmp(OrderName[j], Accepted[i].Schedule[k][1]) == 0)
                    {
                        // printf("has\n");
                        Accepted[i].days_Y += 1;
                        Accepted[i].output_Y += Y;
                        Plant_YRealUsage += Y;
                        Plant_YPerfectUsage += Y;
                        Plant_YUseDays += 1;
                    }
                    else
                    {
                        // printf("no\n");
                        if (Accepted[i].days_Y > 0)
                        {
                            // printf("111Order Name: %s: startDay_Y: %d\n", OrderName[j], startDay_Y);
                            // printf("Accepted[i].days_Y: %d\n", Accepted[i].days_Y);
                            char realStartDate[11];
                            char realEndDate[11];
                            calculateBackDate(startDay_Y, realStartDate);
                            calculateBackDate(startDay_Y + Accepted[i].days_Y - 1, realEndDate);
                            // printf("Ordername: %s\n", OrderName[j]);
                            // printf("realStartDate: %s\n", realStartDate);
                            // printf("realEndDate: %s\n", realEndDate);
                            fprintf(bonusFile, "%15s%15s%15s%15d%15d%15s\n", OrderName[j], realStartDate, realEndDate, Accepted[i].days_Y, Accepted[i].output_Y, "Plant_Y");

                            startDay_Y += Accepted[i].days_Y;
                            Accepted[i].days_Y = 0;
                            Accepted[i].output_Y = 0;
                        }

                        startDay_Y += 1;
                    }
                }

                if (Accepted[i].days_Y > 0)
                {
                    // printf("222Order Name: %s: startDay_Y: %d\n", OrderName[j], startDay_Y);
                    // printf("Accepted[i].days_Y: %d\n", Accepted[i].days_Y);
                    char realStartDate[11];
                    char realEndDate[11];
                    calculateBackDate(startDay_Y, realStartDate);
                    calculateBackDate(startDay_Y + Accepted[i].days_Y - 1, realEndDate);
                    // printf("Ordername: %s\n", OrderName[j]);
                    // printf("realStartDate: %s\n", realStartDate);
                    // printf("realEndDate: %s\n", realEndDate);
                    fprintf(bonusFile, "%15s%15s%15s%15d%15d%15s\n", OrderName[j], realStartDate, realEndDate, Accepted[i].days_Y, Accepted[i].output_Y, "Plant_Y");
                }

                // check plant Z
                for (k = 0; k < Accepted[i].Days; k++)
                {
                    if (strcmp(OrderName[j], Accepted[i].Schedule[k][2]) == 0)
                    {
                        Accepted[i].days_Z += 1;
                        Accepted[i].output_Z += Z;
                        Plant_ZRealUsage += Z;
                        Plant_ZPerfectUsage += Z;
                        Plant_ZUseDays += 1;
                    }
                    else
                    {
                        if (Accepted[i].days_Z > 0)
                        {
                            char realStartDate[11];
                            char realEndDate[11];
                            calculateBackDate(startDay_Z, realStartDate);
                            calculateBackDate(startDay_Z + Accepted[i].days_Z - 1, realEndDate);
                            // printf("Ordername: %s\n", OrderName[j]);
                            // printf("realStartDate: %s\n", realStartDate);
                            // printf("realEndDate: %s\n", realEndDate);
                            fprintf(bonusFile, "%15s%15s%15s%15d%15d%15s\n", OrderName[j], realStartDate, realEndDate, Accepted[i].days_Z, Accepted[i].output_Z, "Plant_Z");

                            startDay_Z += Accepted[i].days_Z;
                            Accepted[i].days_Z = 0;
                            Accepted[i].output_Z = 0;
                        }

                        startDay_Z += 1;
                    }
                }

                if (Accepted[i].days_Z > 0)
                {
                    char realStartDate[11];
                    char realEndDate[11];
                    calculateBackDate(startDay_Z, realStartDate);
                    calculateBackDate(startDay_Z + Accepted[i].days_Z - 1, realEndDate);
                    // printf("Ordername: %s\n", OrderName[j]);
                    // printf("realStartDate: %s\n", realStartDate);
                    // printf("realEndDate: %s\n", realEndDate);
                    fprintf(bonusFile, "%15s%15s%15s%15d%15d%15s\n", OrderName[j], realStartDate, realEndDate, Accepted[i].days_Z, Accepted[i].output_Z, "Plant_Z");
                }
            }
            else
            {
                // this is a bullshit order
                if (Accepted[i].days_X > 0)
                { // check whether this order used Plant_X
                    char realStartDate[11];
                    char realEndDate[11];
                    calculateBackDate(Accepted[i].startDay_X, realStartDate);
                    calculateBackDate(Accepted[i].startDay_X + Accepted[i].days_X - 1, realEndDate);
                    fprintf(bonusFile, "%15s%15s%15s%15d%15d%15s\n", Accepted[i].OrderName, realStartDate, realEndDate, Accepted[i].days_X, Accepted[i].output_X, "Plant_X");
                }
                if (Accepted[i].days_Y > 0)
                { // check whether this order used Plant_Y
                    char realStartDate[11];
                    char realEndDate[11];
                    calculateBackDate(Accepted[i].startDay_Y, realStartDate);
                    calculateBackDate(Accepted[i].startDay_Y + Accepted[i].days_Y - 1, realEndDate);
                    fprintf(bonusFile, "%15s%15s%15s%15d%15d%15s\n", Accepted[i].OrderName, realStartDate, realEndDate, Accepted[i].days_Y, Accepted[i].output_Y, "Plant_Y");
                }
                if (Accepted[i].days_Z > 0)
                {
                    char realStartDate[11];
                    char realEndDate[11];
                    calculateBackDate(Accepted[i].startDay_Z, realStartDate);
                    calculateBackDate(Accepted[i].startDay_Z + Accepted[i].days_Z - 1, realEndDate);
                    fprintf(bonusFile, "%15s%15s%15s%15d%15d%15s\n", Accepted[i].OrderName, realStartDate, realEndDate, Accepted[i].days_Z, Accepted[i].output_Z, "Plant_Z");
                }
            }
        }
    }
    fprintf(bonusFile, "\n             - End -\n\n");
    fprintf(bonusFile, "==========================================================================================\n\n");
    fprintf(bonusFile, "There are %d Orders REJECTED. Details are as follows:\n\n", RejectedCount);
    fprintf(bonusFile, "%15s%15s%15s%15s\n", "ORDER NUMBER", "PRODUCT NAME", "DUE DATE", "QUANTITY");
    fprintf(bonusFile, "==========================================================================================\n");
    // print the rejected orders
    for (i = 0; i < RejectedCount; i++)
    {
        fprintf(bonusFile, "%15s%15s%15d%15d\n", Rejected[i].OrderName, Rejected[i].ProductName, *(Rejected[i].DDL), *(Rejected[i].Quantity));
    }
    fprintf(bonusFile, "\n             - End -\n\n");
    fprintf(bonusFile, "==========================================================================================\n\n");
    fprintf(bonusFile, "***PERFORMANCE\n");
    fprintf(bonusFile, "Plant_X:\n");
    fprintf(bonusFile, "         Number of days in use:%21d days\n", Plant_XUseDays);
    fprintf(bonusFile, "         Total quantity produced:%15d (in total)\n", Plant_XRealUsage);
    fprintf(bonusFile, "         Utilization of the plant:%18.1f %%\n\n", (double)Plant_XRealUsage / Plant_XPerfectUsage * 100);
    fprintf(bonusFile, "Plant_Y:\n");
    fprintf(bonusFile, "         Number of days in use:%21d days\n", Plant_YUseDays);
    fprintf(bonusFile, "         Total quantity produced:%15d (in total)\n", Plant_YRealUsage);
    fprintf(bonusFile, "         Utilization of the plant:%18.1f %%\n\n", (double)Plant_YRealUsage / Plant_YPerfectUsage * 100);
    fprintf(bonusFile, "Plant_Z:\n");
    fprintf(bonusFile, "         Number of days in use:%21d days\n", Plant_ZUseDays);
    fprintf(bonusFile, "         Total quantity produced:%15d (in total)\n", Plant_ZRealUsage);
    fprintf(bonusFile, "         Utilization of the plant:%18.1f %%\n\n", (double)Plant_ZRealUsage / Plant_ZPerfectUsage * 100);
    fprintf(bonusFile, "Overall of utilization: %28.1f %%\n", (double)(Plant_XRealUsage + Plant_YRealUsage + Plant_ZRealUsage) / (Plant_XPerfectUsage + Plant_YPerfectUsage + Plant_ZPerfectUsage) * 100);
    fclose(bonusFile);
}

void printSchedule(struct Order *order, int ScheduleStartDayCount)
{
    // printOrder(order);
    int SumOfQuantity = 0;
    for (j = 0; j < order->ContainedOrder; j++)
    {
        SumOfQuantity += order->Quantity[j];
    }

    // //printf("%-s %-13s %-13s %-5d %-9d %-s\n", order->OrderName, RealStartDate, RealEndDate, order->Days, SumOfQuantity, "Plant_XYZ");
    // get the order name by parsing the order name with space
    char **parsedOrderName = (char **)malloc(order->ContainedOrder * sizeof(char *));
    char **parsedProductName = (char **)malloc(order->ContainedOrder * sizeof(char *));
    char *token = strtok(order->OrderName, " ");

    for (j = 0; j < order->ContainedOrder; j++)
    {
        parsedOrderName[j] = token;
        token = strtok(NULL, " ");
        // //printf("parsedOrderName: %s\n", parsedOrderName[j]);
    }

    token = strtok(order->ProductName, " ");
    for (j = 0; j < order->ContainedOrder; j++)
    {
        parsedProductName[j] = token;
        token = strtok(NULL, " ");
        // //printf("parsedProductName: %s\n", parsedProductName[j]);
    }

    for (j = 0; j < order->Days; j++)
    {
        char RealStartDate[11];
        char RealEndDate[11];
        calculateBackDate(ScheduleStartDayCount + j, RealStartDate);
        calculateBackDate(ScheduleStartDayCount + j + 1, RealEndDate);
        // select the order based on the order name in the schedule
        for (l = 0; l < order->ContainedOrder; l++)
        {
            // //printf("parsedOrderName: %s\n", parsedOrderName[l]);
            // //printf("order->Schedule[j][0]: %s\n", order->Schedule[j][0]);
            if (strcmp(parsedOrderName[l], order->Schedule[j][0]) == 0)
            {
                Plant_XPerfectUsage += 300;
                Plant_XRealUsage += X;
                // printf("%-10s %-13s %-13s %-5d %-9d %-s\n", parsedOrderName[l], RealStartDate, RealEndDate, 1, X, "Plant_X");
                break;
            }
        }
        for (l = 0; l < order->ContainedOrder; l++)
        {
            if (strcmp(parsedOrderName[l], order->Schedule[j][1]) == 0)
            {
                Plant_YPerfectUsage += 400;
                Plant_YRealUsage += Y;
                // printf("%-10s %-13s %-13s %-5d %-9d %-s\n", parsedOrderName[l], RealStartDate, RealEndDate, 1, Y, "Plant_Y");
                break;
            }
        }
        for (l = 0; l < order->ContainedOrder; l++)
        {
            if (strcmp(parsedOrderName[l], order->Schedule[j][2]) == 0)
            {
                Plant_ZPerfectUsage += 500;
                Plant_ZRealUsage += Z;
                // printf("%-10s %-13s %-13s %-5d %-9d %-s\n", parsedOrderName[l], RealStartDate, RealEndDate, 1, Z, "Plant_Z");
                break;
            }
        }
    }
}

void DYLANDecision(struct Order *Zeros, struct Order *Bullshits, int ZeroCount, int BullshitCount)
{
    struct Order Accepted[OrderCount]; // remainder = 0, CHAI Wenchang call it perfect order
    struct Order Rejected[OrderCount]; // remainder != 0, CHAI Wenchang call it bullshit order
    int AcceptedCount = 0;
    int RejectedCount = 0;
    int ScheduleStartDayCount = ScheduleStartDay;
    // printf("***PLS Schedule Analysis Report***\n");
    // printf("Algorithm used: CHAI\n");
    // printf("ORDER NUMBER   START     END     DAYS       QUANTITY         PLANT\n");
    // printf("===========================================================================\n");
    for (i = 0; i < ZeroCount; i++) // deal with perfect order
    {
        if (Zeros[i].LatestStart < ScheduleStartDayCount || Zeros[i].LatestStart + Zeros[i].Days - 1 > ScheduleEndDay)
        {
            char *OrderName[3]; // split the order name with space
            char *token = strtok(Zeros[i].OrderName, " ");
            for (j = 0; j < Zeros[i].ContainedOrder; j++)
            {
                OrderName[j] = token;
                token = strtok(NULL, " ");
            }

            for (j = 0; j < Zeros[i].ContainedOrder; j++)
            {
                for (k = 0; k < BullshitCount; k++)
                {
                    if (strcmp(OrderName[j], Bullshits[k].OrderName) == 0)
                    {
                        // pick up the order that last start date is greater than the current start date
                        if (Bullshits[k].LatestStart >= ScheduleStartDayCount)
                        {
                            Bullshits[k].ContainedOrder = 1;
                            // //printf("restored: %s\n", Bullshits[k].OrderName);
                        }
                        else
                        {
                            Rejected[RejectedCount] = Bullshits[k];
                            RejectedCount++;
                            // //printf("Rejected: %s\n", Bullshits[k].OrderName);
                        }

                        break;
                    }
                }
            }
        }
        else
        {
            Accepted[AcceptedCount] = Zeros[i];
            Accepted[AcceptedCount].startDay_X = Accepted[AcceptedCount].startDay_Y = Accepted[AcceptedCount].startDay_Z = ScheduleStartDayCount;
            // printf("ScheduleStartDayCount: %d\n", ScheduleStartDayCount);
            //  printSchedule(&Accepted[AcceptedCount], ScheduleStartDayCount);
            //  printOrder(&Accepted[AcceptedCount]);
            AcceptedCount++;
            ScheduleStartDayCount += Zeros[i].Days;
        }
    }
    // printf("===========================================================================\n\n");
    int BullshitStartDayCount = ScheduleStartDayCount;
    // sort the bullshit order according to the latest start day in ascending order
    for (i = 0; i < BullshitCount; i++)
    {
        for (j = i + 1; j < BullshitCount; j++)
        {
            if (Bullshits[i].LatestStart > Bullshits[j].LatestStart)
            {
                struct Order temp = Bullshits[i];
                Bullshits[i] = Bullshits[j];
                Bullshits[j] = temp;
            }
        }
    }

    // array to store which plants can be used
    int checkPlant[] = {0, 0, 0};
    // deal with the bullshit order with fcfs
    for (i = 0; i < BullshitCount; i++)
    {
        if (Bullshits[i].ContainedOrder == 0)
            continue; // skip the dead order
        int days = 0;

        struct Order *BullshitCPY = (struct Order *)malloc(sizeof(struct Order));
        BullshitCPY->OrderName = (char *)malloc(sizeof(char));
        BullshitCPY->ProductName = (char *)malloc(sizeof(char));
        BullshitCPY->DDL = (int *)malloc(sizeof(int));
        BullshitCPY->Quantity = (int *)malloc(sizeof(int));
        BullshitCPY->Schedule = NULL;
        BullshitCPY->ContainedOrder = Bullshits[i].ContainedOrder;
        strcpy(BullshitCPY->OrderName, Bullshits[i].OrderName);
        strcpy(BullshitCPY->ProductName, Bullshits[i].ProductName);
        for (j = 0; j < Bullshits[i].ContainedOrder; j++)
        {
            BullshitCPY->DDL[j] = Bullshits[i].DDL[j];
            BullshitCPY->Quantity[j] = Bullshits[i].Quantity[j];
        }
        BullshitCPY->Remainder = Bullshits[i].Remainder;
        BullshitCPY->Days = Bullshits[i].Days;
        BullshitCPY->LatestStart = Bullshits[i].LatestStart;

        int checkPlantCPY[] = {checkPlant[0], checkPlant[1], checkPlant[2]};
        int BullshitStartDayCountCPY = BullshitStartDayCount;
        if (Bullshits[i].LatestStart < BullshitStartDayCount || checkRejectOrNot(BullshitCPY, checkPlantCPY, BullshitStartDayCountCPY) == 0 || Bullshits[i].LatestStart + Bullshits[i].Days - 1 > ScheduleEndDay)
        {
            Rejected[RejectedCount] = Bullshits[i];
            RejectedCount++;
        }
        else
        {
            Accepted[AcceptedCount] = Bullshits[i];
            AcceptedCount++;
            while (Bullshits[i].Quantity > 0)
            {
                // determine whether the order needs to be rejected during fcfs
                // //printf("checkPlant: %d %d %d\n", checkPlant[0], checkPlant[1], checkPlant[2]);
                // determine the plant to be used
                if (checkPlant[2] == 0 && (*(Bullshits[i].Quantity) > 400 || (checkPlant[1] == 1 && checkPlant[0] == 1))) // if the quantity is more than 400, use Plant_Z; or Plant_Y and Plant_X are both used
                {
                    // check whether the order gonna be finished here
                    if (*(Bullshits[i].Quantity) <= 500)
                    {
                        Plant_ZUseDays++;
                        if (Bullshits[i].days_Z == 0)
                        {
                            Bullshits[i].startDay_Z = BullshitStartDayCount;
                        }
                        Bullshits[i].days_Z += 1;
                        Bullshits[i].output_Z += *(Bullshits[i].Quantity);
                        char RealStartDate[11];
                        char RealEndDate[11];
                        calculateBackDate(BullshitStartDayCount, RealStartDate);
                        calculateBackDate(BullshitStartDayCount + 1, RealEndDate);
                        Plant_ZRealUsage += *(Bullshits[i].Quantity);
                        Plant_ZPerfectUsage += 500;
                        // printf("%-10s %-13s %-13s %-5d %-9d %-s\n", Bullshits[i].OrderName, RealStartDate, RealEndDate, 1, *(Bullshits[i].Quantity), "Plant_Z");
                        *(Bullshits[i].Quantity) = 0;
                        checkPlant[2] = 1;
                        // check whether the date needs to be increased
                        if (checkPlant[1] == 1 && checkPlant[0] == 1 && checkPlant[2] == 1)
                        {
                            BullshitStartDayCount++;
                            checkPlant[0] = 0;
                            checkPlant[1] = 0;
                            checkPlant[2] = 0;
                        }
                        // //printf("Break\n");
                        break;
                    }
                    else
                    {
                        Plant_ZUseDays++;
                        if (Bullshits[i].days_Z == 0)
                        {
                            Bullshits[i].startDay_Z = BullshitStartDayCount;
                        }
                        Bullshits[i].days_Z += 1;
                        Bullshits[i].output_Z += 500;
                        char RealStartDate[11];
                        char RealEndDate[11];
                        calculateBackDate(BullshitStartDayCount, RealStartDate);
                        calculateBackDate(BullshitStartDayCount + 1, RealEndDate);
                        Plant_ZRealUsage += 500;
                        Plant_ZPerfectUsage += 500;
                        // printf("%-10s %-13s %-13s %-5d %-9s %-s\n", Bullshits[i].OrderName, RealStartDate, RealEndDate, 1, "500", "Plant_Z");
                        *(Bullshits[i].Quantity) -= 500;
                        days++;
                        checkPlant[2] = 1;
                        if (checkPlant[1] == 1 && checkPlant[0] == 1 && checkPlant[2] == 1)
                        {
                            BullshitStartDayCount++;
                            checkPlant[0] = 0;
                            checkPlant[1] = 0;
                            checkPlant[2] = 0;
                        }
                    }
                }
                // //printf("Quantity: %d\n", *(Bullshits[i].Quantity));
                else if (checkPlant[1] == 0 && (*(Bullshits[i].Quantity) > 300 || checkPlant[0] == 1)) // if the quantity is more than 300, use Plant_Y; or Plant_X and Plant_Z are both used
                {
                    if (*(Bullshits[i].Quantity) <= 400)
                    {
                        Plant_YUseDays++;
                        if (Bullshits[i].days_Y == 0)
                        {
                            Bullshits[i].startDay_Y = BullshitStartDayCount;
                        }
                        Bullshits[i].days_Y += 1;
                        Bullshits[i].output_Y += *(Bullshits[i].Quantity);
                        char RealStartDate[11];
                        char RealEndDate[11];
                        calculateBackDate(BullshitStartDayCount, RealStartDate);
                        calculateBackDate(BullshitStartDayCount + 1, RealEndDate);
                        Plant_YRealUsage += *(Bullshits[i].Quantity);
                        Plant_YPerfectUsage += 400;
                        // printf("%-10s %-13s %-13s %-5d %-9d %-s\n", Bullshits[i].OrderName, RealStartDate, RealEndDate, 1, *(Bullshits[i].Quantity), "Plant_Y");
                        *(Bullshits[i].Quantity) = 0;
                        checkPlant[1] = 1;
                        if (checkPlant[1] == 1 && checkPlant[0] == 1 && checkPlant[2] == 1)
                        {
                            BullshitStartDayCount++;
                            checkPlant[0] = 0;
                            checkPlant[1] = 0;
                            checkPlant[2] = 0;
                        }
                        // //printf("Break\n");
                        break;
                    }
                    else
                    {
                        Plant_YUseDays++;
                        if (Bullshits[i].days_Y == 0)
                        {
                            Bullshits[i].startDay_Y = BullshitStartDayCount;
                        }
                        Bullshits[i].days_Y += 1;
                        Bullshits[i].output_Y += 400;
                        char RealStartDate[11];
                        char RealEndDate[11];
                        calculateBackDate(BullshitStartDayCount, RealStartDate);
                        calculateBackDate(BullshitStartDayCount + 1, RealEndDate);
                        Plant_YRealUsage += 400;
                        Plant_YPerfectUsage += 400;
                        // printf("%-10s %-13s %-13s %-5d %-9s %-s\n", Bullshits[i].OrderName, RealStartDate, RealEndDate, 1, "400", "Plant_Y");
                        *(Bullshits[i].Quantity) -= 400;
                        days++;
                        checkPlant[1] = 1;
                        if (checkPlant[1] == 1 && checkPlant[0] == 1 && checkPlant[2] == 1)
                        {
                            BullshitStartDayCount++;
                            checkPlant[0] = 0;
                            checkPlant[1] = 0;
                            checkPlant[2] = 0;
                        }
                    }
                }
                else if (checkPlant[0] == 0 && *(Bullshits[i].Quantity) > 0) // if the quantity is more than 0, use Plant_X;
                {
                    if (*(Bullshits[i].Quantity) <= 300)
                    {
                        Plant_XUseDays++;
                        if (Bullshits[i].days_X == 0)
                        {
                            Bullshits[i].startDay_X = BullshitStartDayCount;
                        }
                        Bullshits[i].days_X += 1;
                        Bullshits[i].output_X += *(Bullshits[i].Quantity);
                        char RealStartDate[11];
                        char RealEndDate[11];
                        calculateBackDate(BullshitStartDayCount, RealStartDate);
                        calculateBackDate(BullshitStartDayCount + 1, RealEndDate);
                        Plant_XRealUsage += *(Bullshits[i].Quantity);
                        Plant_XPerfectUsage += 300;
                        // printf("%-10s %-13s %-13s %-5d %-9d %-s\n", Bullshits[i].OrderName, RealStartDate, RealEndDate, 1, *(Bullshits[i].Quantity), "Plant_X");
                        *(Bullshits[i].Quantity) = 0;
                        checkPlant[0] = 1;
                        if (checkPlant[1] == 1 && checkPlant[0] == 1 && checkPlant[2] == 1)
                        {
                            BullshitStartDayCount++;
                            checkPlant[0] = 0;
                            checkPlant[1] = 0;
                            checkPlant[2] = 0;
                        }
                        // //printf("Break\n");
                        break;
                    }
                    else
                    {
                        Plant_XUseDays++;
                        if (Bullshits[i].days_X == 0)
                        {
                            Bullshits[i].startDay_X = BullshitStartDayCount;
                        }
                        Bullshits[i].days_X += 1;
                        Bullshits[i].output_X += 300;
                        char RealStartDate[11];
                        char RealEndDate[11];
                        calculateBackDate(BullshitStartDayCount, RealStartDate);
                        calculateBackDate(BullshitStartDayCount + 1, RealEndDate);
                        Plant_XRealUsage += 300;
                        Plant_XPerfectUsage += 300;
                        // printf("%-10s %-13s %-13s %-5d %-9s %-s\n", Bullshits[i].OrderName, RealStartDate, RealEndDate, 1, "300", "Plant_X");
                        *(Bullshits[i].Quantity) -= 300;
                        days++;
                        checkPlant[0] = 1;
                        if (checkPlant[1] == 1 && checkPlant[0] == 1 && checkPlant[2] == 1)
                        {
                            BullshitStartDayCount++;
                            checkPlant[0] = 0;
                            checkPlant[1] = 0;
                            checkPlant[2] = 0;
                        }
                    }
                }
            }
            Accepted[AcceptedCount - 1].startDay_X = Bullshits[i].startDay_X;
            Accepted[AcceptedCount - 1].startDay_Y = Bullshits[i].startDay_Y;
            Accepted[AcceptedCount - 1].startDay_Z = Bullshits[i].startDay_Z;
            Accepted[AcceptedCount - 1].days_X = Bullshits[i].days_X;
            Accepted[AcceptedCount - 1].days_Y = Bullshits[i].days_Y;
            Accepted[AcceptedCount - 1].days_Z = Bullshits[i].days_Z;
            Accepted[AcceptedCount - 1].output_X = Bullshits[i].output_X;
            Accepted[AcceptedCount - 1].output_Y = Bullshits[i].output_Y;
            Accepted[AcceptedCount - 1].output_Z = Bullshits[i].output_Z;
        }
    }

    // printf("There are %d Orders REJECTED. \n", RejectedCount);
    for (i = 0; i < RejectedCount; i++)
    {
        // printf("Rejected: %s\n", Rejected[i].OrderName);
    }
    // printf("***PERFORMANCE\n\n");
    // printf("Plant_X:");
    //  calculate the utilization of Plant_X
    // printf(" %.2f%%\n", (float)Plant_XRealUsage / Plant_XPerfectUsage * 100);
    // printf("Plant_Y:");
    //  calculate the utilization of Plant_Y
    // printf(" %.2f%%\n", (float)Plant_YRealUsage / Plant_YPerfectUsage * 100);
    // printf("Plant_Z:");
    //  calculate the utilization of Plant_Z
    // printf(" %.2f%%\n", (float)Plant_ZRealUsage / Plant_ZPerfectUsage * 100);
    //  calculate the overall utilization
    // printf("Overall:");
    // printf(" %.2f%%\n", (float)(Plant_XRealUsage + Plant_YRealUsage + Plant_ZRealUsage) / (Plant_XPerfectUsage + Plant_YPerfectUsage + Plant_ZPerfectUsage) * 100);

    // printf("Call the generateBonusReport\n");
    generateBonusReport(Accepted, AcceptedCount, Rejected, RejectedCount, Plant_XUseDays, Plant_YUseDays, Plant_ZUseDays, Plant_XRealUsage, Plant_YRealUsage, Plant_ZRealUsage, Plant_XPerfectUsage, Plant_YPerfectUsage, Plant_ZPerfectUsage);
}

void CHAISchedule()
{
    struct Order *Zeros = (struct Order *)malloc(sizeof(struct Order));     // remainder = 0, CHAI Wenchang call it perfect order
    struct Order *BullShits = (struct Order *)malloc(sizeof(struct Order)); // remainder != 0, CHAI Wenchang call it bullshit order
    int ZeroCount = 0;
    int BullShitCount = 0;

    char *OrderName; // cache the product name

    // find the perfect order
    for (i = 0; i < OrderCount; i++)
    {
        if (Orders[i].Remainder == 0)
        {
            OrderName = Orders[i].OrderName;
            Orders[i].Schedule = (char ***)malloc(Orders[i].Days * sizeof(char **)); // create the schedule
            for (j = 0; j < Orders[i].Days; j++)
            {
                Orders[i].Schedule[j] = (char **)malloc(3 * sizeof(char *));
                Orders[i].Schedule[j][0] = Orders[i].Schedule[j][1] = Orders[i].Schedule[j][2] = OrderName;
            }
            Zeros = (struct Order *)realloc(Zeros, (ZeroCount + 1) * sizeof(struct Order));
            Zeros[ZeroCount] = Orders[i];
            ZeroCount++;
        }
        else
        {
            BullShits = (struct Order *)realloc(BullShits, (BullShitCount + 1) * sizeof(struct Order));
            BullShits[BullShitCount] = Orders[i];
            BullShitCount++;
        }
    }

    // sort the perfect order
    for (i = 0; i < ZeroCount; i++)
    {
        for (j = i + 1; j < ZeroCount; j++)
        {
            if (Zeros[i].LatestStart > Zeros[j].LatestStart)
            {
                struct Order temp = Zeros[i];
                Zeros[i] = Zeros[j];
                Zeros[j] = temp;
            }
        }
    }
    // sort the bullshit order
    for (i = 0; i < BullShitCount; i++)
    {
        for (j = i + 1; j < BullShitCount; j++)
        {
            if (BullShits[i].LatestStart > BullShits[j].LatestStart)
            {
                struct Order temp = BullShits[i];
                BullShits[i] = BullShits[j];
                BullShits[j] = temp;
            }
        }
    }

    // // print the perfect order and bullshit order for debug
    // //printf("\nPerfect Orders:\n");
    // for(i=0; i<ZeroCount; i++){
    //     printOrder(&Zeros[i]);
    // }
    // //printf("\nBullshit Orders:\n");
    // for(i=0; i<BullShitCount; i++){
    //     printOrder(&BullShits[i]);
    // }

    // combine bullshit orders to be perfect orders
    // CHAI will combine the orders to make the remainder to be 0
    // here we only combine to make 1 more day as breif example:
    // 300 + 400 + 500 = 1200
    // 300 + 900 = 1200
    // 400 + 800 = 1200
    // 500 + 700 = 1200
    struct Order *newZeros; // new perfect orders
    struct Order **orders = (struct Order **)malloc(3 * sizeof(struct Order *));
    int newBullShitCount = BullShitCount;

    for (i = 0; i < BullShitCount; i++)
    {
        if (BullShits[i].ContainedOrder == 0)
            continue; // skip the dead order
        if (BullShits[i].Remainder == X)
        {
            // //printf("X: %s\n", BullShits[i].OrderName);
            for (j = 0; j < BullShitCount; j++)
            {
                if (BullShits[j].ContainedOrder == 0)
                    continue; // skip the dead order
                if (BullShits[j].Remainder == Y + Z)
                {
                    orders[0] = &BullShits[i];
                    orders[1] = &BullShits[j];
                    orders[2] = initOrder(); // create an empty order

                    // sort the orders
                    for (k = 0; k < 3; k++)
                    {
                        for (l = k + 1; l < 3; l++)
                        {
                            if (orders[k]->Days < orders[l]->Days)
                            {
                                struct Order *temp = orders[k];
                                orders[k] = orders[l];
                                orders[l] = temp;
                            }
                        }
                    }

                    newZeros = CHAICombine(orders);

                    if (newZeros == NULL)
                    {
                        // //printf("Cannot combine %s and %s\n", BullShits[i].OrderName, BullShits[j].OrderName);
                        continue;
                    }

                    // fill the schedule for the earliest start day of orders
                    newZeros->Schedule[0][0] = BullShits[i].OrderName; // X
                    newZeros->Schedule[0][1] = BullShits[j].OrderName; // Y
                    newZeros->Schedule[0][2] = BullShits[j].OrderName; // Z

                    // fill the schedule for the rest of the days of the orders
                    k = 1;
                    for (l = 0; l < 3; l++)
                    {
                        for (m = 0; m < orders[l]->Days; m++, k++)
                        {
                            for (n = 0; n < 3; n++)
                            {
                                newZeros->Schedule[k][n] = orders[l]->OrderName;
                            }
                        }
                    }

                    // remove the combined orders from the list
                    BullShits[i].ContainedOrder = 0;
                    BullShits[j].ContainedOrder = 0;
                    newBullShitCount -= 2;

                    // add the new order to the list
                    Zeros = (struct Order *)realloc(Zeros, (ZeroCount + 1) * sizeof(struct Order));
                    Zeros[ZeroCount] = *newZeros;
                    ZeroCount++;
                    break;
                }
                else if (BullShits[j].Remainder == Y)
                {
                    for (k = 0; k < BullShitCount; k++)
                    {
                        if (BullShits[k].ContainedOrder == 0)
                            continue; // skip the dead order
                        if (BullShits[k].Remainder == Z)
                        {
                            orders[0] = &BullShits[i];
                            orders[1] = &BullShits[j];
                            orders[2] = &BullShits[k];

                            // sort the orders
                            for (l = 0; l < 3; l++)
                            {
                                for (m = l + 1; m < 3; m++)
                                {
                                    if (orders[l]->Days < orders[m]->Days)
                                    {
                                        struct Order *temp = orders[l];
                                        orders[l] = orders[m];
                                        orders[m] = temp;
                                    }
                                }
                            }

                            newZeros = CHAICombine(orders);

                            if (newZeros == NULL)
                            {
                                // //printf("Cannot combine %s, %s and %s\n", BullShits[i].OrderName, BullShits[j].OrderName, BullShits[k].OrderName);
                                continue;
                            }

                            // fill the schedule for the earliest start day of orders
                            newZeros->Schedule[0][0] = BullShits[i].OrderName; // X
                            newZeros->Schedule[0][1] = BullShits[j].OrderName; // Y
                            newZeros->Schedule[0][2] = BullShits[k].OrderName; // Z

                            // fill the schedule for the rest of the days of the orders
                            l = 1;
                            for (m = 0; m < 3; m++)
                            {
                                for (n = 0; n < orders[m]->Days; n++, l++)
                                {
                                    for (o = 0; o < 3; o++)
                                    {
                                        newZeros->Schedule[l][o] = orders[m]->OrderName;
                                    }
                                }
                            }

                            // remove the combined orders from the list
                            BullShits[i].ContainedOrder = 0;
                            BullShits[j].ContainedOrder = 0;
                            BullShits[k].ContainedOrder = 0;
                            newBullShitCount -= 3;

                            // add the new order to the list
                            Zeros = (struct Order *)realloc(Zeros, (ZeroCount + 1) * sizeof(struct Order));
                            Zeros[ZeroCount] = *newZeros;
                            ZeroCount++;
                            break;
                        }
                    }
                }
            }
        }
        else if (BullShits[i].Remainder == Y)
        {
            // //printf("Y: %s\n", BullShits[i].OrderName);
            for (j = 0; j < BullShitCount; j++)
            {
                if (BullShits[j].ContainedOrder == 0)
                    continue; // skip the dead order
                if (BullShits[j].Remainder == X + Z)
                {
                    orders[0] = &BullShits[i];
                    orders[1] = &BullShits[j];
                    orders[2] = initOrder(); // create an empty order

                    // sort the orders
                    for (k = 0; k < 3; k++)
                    {
                        for (l = k + 1; l < 3; l++)
                        {
                            if (orders[k]->Days < orders[l]->Days)
                            {
                                struct Order *temp = orders[k];
                                orders[k] = orders[l];
                                orders[l] = temp;
                            }
                        }
                    }

                    newZeros = CHAICombine(orders);

                    if (newZeros == NULL)
                    {
                        // //printf("Cannot combine %s and %s\n", BullShits[i].OrderName, BullShits[j].OrderName);
                        continue;
                    }

                    // fill the schedule for the earliest start day of orders
                    newZeros->Schedule[0][0] = BullShits[j].OrderName; // X
                    newZeros->Schedule[0][1] = BullShits[i].OrderName; // Y
                    newZeros->Schedule[0][2] = BullShits[j].OrderName; // Z

                    // fill the schedule for the rest of the days of the orders
                    k = 1;
                    for (l = 0; l < 3; l++)
                    {
                        for (m = 0; m < orders[l]->Days; m++, k++)
                        {
                            for (n = 0; n < 3; n++)
                            {
                                newZeros->Schedule[k][n] = orders[l]->OrderName;
                            }
                        }
                    }

                    // remove the combined orders from the list
                    BullShits[i].ContainedOrder = 0;
                    BullShits[j].ContainedOrder = 0;
                    newBullShitCount -= 2;

                    // add the new order to the list
                    Zeros = (struct Order *)realloc(Zeros, (ZeroCount + 1) * sizeof(struct Order));
                    Zeros[ZeroCount] = *newZeros;
                    ZeroCount++;
                    break;
                }
            }
        }
        else if (BullShits[i].Remainder == Z)
        {
            // //printf("Z: %s\n", BullShits[i].OrderName);
            for (j = 0; j < BullShitCount; j++)
            {
                if (BullShits[j].ContainedOrder == 0)
                    continue; // skip the dead order
                if (BullShits[j].Remainder == X + Y)
                {
                    orders[0] = &BullShits[i];
                    orders[1] = &BullShits[j];
                    orders[2] = initOrder(); // create an empty order

                    // sort the orders
                    for (k = 0; k < 3; k++)
                    {
                        for (l = k + 1; l < 3; l++)
                        {
                            if (orders[k]->Days < orders[l]->Days)
                            {
                                struct Order *temp = orders[k];
                                orders[k] = orders[l];
                                orders[l] = temp;
                            }
                        }
                    }

                    newZeros = CHAICombine(orders);

                    if (newZeros == NULL)
                    {
                        // //printf("Cannot combine %s and %s\n", BullShits[i].OrderName, BullShits[j].OrderName);
                        continue;
                    }

                    // fill the schedule for the earliest start day of orders
                    newZeros->Schedule[0][0] = BullShits[j].OrderName; // X
                    newZeros->Schedule[0][1] = BullShits[j].OrderName; // Y
                    newZeros->Schedule[0][2] = BullShits[i].OrderName; // Z

                    // fill the schedule for the rest of the days of the orders
                    k = 1;
                    for (l = 0; l < 3; l++)
                    {
                        for (m = 0; m < orders[l]->Days; m++, k++)
                        {
                            for (n = 0; n < 3; n++)
                            {
                                newZeros->Schedule[k][n] = orders[l]->OrderName;
                            }
                        }
                    }

                    // remove the combined orders from the list
                    BullShits[i].ContainedOrder = 0;
                    BullShits[j].ContainedOrder = 0;
                    newBullShitCount -= 2;

                    // add the new order to the list
                    Zeros = (struct Order *)realloc(Zeros, (ZeroCount + 1) * sizeof(struct Order));
                    Zeros[ZeroCount] = *newZeros;
                    ZeroCount++;
                    break;
                }
            }
        }
    }

    // sort the perfect order
    for (i = 0; i < ZeroCount; i++)
    {
        for (j = i + 1; j < ZeroCount; j++)
        {
            if (Zeros[i].Days > Zeros[j].Days || (Zeros[i].Days == Zeros[j].Days && Zeros[i].LatestStart > Zeros[j].LatestStart) || (Zeros[i].Days == Zeros[j].Days && Zeros[i].LatestStart == Zeros[j].LatestStart && Zeros[i].ContainedOrder > Zeros[j].ContainedOrder))
            {
                struct Order temp = Zeros[i];
                Zeros[i] = Zeros[j];
                Zeros[j] = temp;
            }
        }
    }
    for (i = 0; i < BullShitCount; i++)
    {
        for (j = i + 1; j < BullShitCount; j++)
        {
            if (BullShits[i].Days > BullShits[j].Days || (BullShits[i].Days == BullShits[j].Days && BullShits[i].LatestStart > BullShits[j].LatestStart) || (BullShits[i].Days == BullShits[j].Days && BullShits[i].LatestStart == BullShits[j].LatestStart && BullShits[i].ContainedOrder > BullShits[j].ContainedOrder))
            {
                struct Order temp = BullShits[i];
                BullShits[i] = BullShits[j];
                BullShits[j] = temp;
            }
        }
    }
    // print the perfect order and bullshit order for debug
    // //printf("\nPerfect Orders:\n");
    // for (i = 0; i < ZeroCount; i++)
    // {
    //     printOrder(&Zeros[i]);
    // }
    // //printf("\nBullshit Orders:\n");
    // for (i = 0; i < BullShitCount; i++)
    // { // skip the dead order
    //     if (BullShits[i].ContainedOrder == 0)
    //         continue;
    //     printOrder(&BullShits[i]);
    // }
    // printf("Here\n");
    // printf("ScheduleStartDay: %d\n", ScheduleStartDay);
    DYLANDecision(Zeros, BullShits, ZeroCount, BullShitCount);
    // start here
}
struct Order *CHAICombine(struct Order **orders)
{ // combine three orders
    struct Order *combinedOrder = initOrder();
    struct Order *order1;
    struct Order *order2;
    struct Order *order3;
    int i, j;

    order1 = orders[0];
    order2 = orders[1];
    order3 = orders[2];

    // combine the orders
    combinedOrder->ContainedOrder = order1->ContainedOrder + order2->ContainedOrder + order3->ContainedOrder;
    sprintf(combinedOrder->OrderName, "%s %s %s", order1->OrderName, order2->OrderName, order3->OrderName);
    sprintf(combinedOrder->ProductName, "%s %s %s", order1->ProductName, order2->ProductName, order3->ProductName);
    combinedOrder->DDL = (int *)malloc(combinedOrder->ContainedOrder * sizeof(int));
    combinedOrder->Quantity = (int *)malloc(combinedOrder->ContainedOrder * sizeof(int));

    combinedOrder->DDL[0] = order1->DDL[0];
    combinedOrder->Quantity[0] = order1->Quantity[0];

    combinedOrder->DDL[1] = order2->DDL[0];
    combinedOrder->Quantity[1] = order2->Quantity[0];

    combinedOrder->DDL[2] = order3->DDL[0];
    combinedOrder->Quantity[2] = order3->Quantity[0];

    combinedOrder->Remainder = order1->Remainder + order2->Remainder + order3->Remainder;
    combinedOrder->Days = order1->Days + order2->Days + order3->Days + combinedOrder->Remainder / D; // add the days for the remainder
    combinedOrder->Remainder = combinedOrder->Remainder % D;                                         // update the remainder

    // calculate the latest start
    // //printf("Order1: %s Order2: %s Order3: %s\n", order1->OrderName, order2->OrderName, order3->OrderName);
    // //printf("DDL1: %d DDL2: %d DDL3: %d\n", order1->DDL[0], order2->DDL[0], order3->DDL[0]);
    // //printf("Days1: %d Days2: %d Days3: %d\n", order1->Days, order2->Days, order3->Days);
    // //printf("LatestStart1: %d LatestStart2: %d LatestStart3: %d\n", order1->LatestStart, order2->LatestStart, order3->LatestStart);
    combinedOrder->LatestStart = (order1->LatestStart - order2->Days) < order2->LatestStart ? (order1->LatestStart - order2->Days) : order2->LatestStart;               // update the latest start
    combinedOrder->LatestStart = (combinedOrder->LatestStart - order3->Days) < order3->LatestStart ? (combinedOrder->LatestStart - order3->Days) : order3->LatestStart; // update the latest start
    // //printf("LatestStart: %d\n", combinedOrder->LatestStart);
    // check if the latest start is earlier than the start day
    if (combinedOrder->LatestStart < ScheduleStartDay)
    {
        // //printf("Cannot combine %s, %s and %s\n", order1->OrderName, order2->OrderName, order3->OrderName);
        return NULL;
    }

    combinedOrder->Schedule = (char ***)malloc(combinedOrder->Days * sizeof(char **)); // create the schedule
    for (i = 0; i < combinedOrder->Days; i++)
    {
        combinedOrder->Schedule[i] = (char **)malloc(3 * sizeof(char *));
    }

    return combinedOrder;
}

int checkRejectOrNot(struct Order *Bullshit, int *checkPlant, int BullshitStartDayCount)
{
    while (Bullshit->Quantity > 0)
    {
        // determine whether the order needs to be rejected during fcfs
        // //printf("checkPlant: %d %d %d\n", checkPlant[0], checkPlant[1], checkPlant[2]);
        // determine the plant to be used
        if (checkPlant[2] == 0 && (*(Bullshit->Quantity) > 400 || (checkPlant[1] == 1 && checkPlant[0] == 1))) // if the quantity is more than 400, use Plant_Z; or Plant_Y and Plant_X are both used
        {
            // check whether the order gonna be finished here
            if (*(Bullshit->Quantity) <= 500)
            {
                *(Bullshit->Quantity) = 0;
                checkPlant[2] = 1;
                // check whether the date needs to be increased
                if (checkPlant[1] == 1 && checkPlant[0] == 1 && checkPlant[2] == 1)
                {
                    // printf("BullshitStartDayCount: %d\n", BullshitStartDayCount);
                    BullshitStartDayCount++;
                    checkPlant[0] = 0;
                    checkPlant[1] = 0;
                    checkPlant[2] = 0;
                }
                // //printf("Break\n");
                break;
            }
            else
            {
                *(Bullshit->Quantity) -= 500;
                checkPlant[2] = 1;
                if (checkPlant[1] == 1 && checkPlant[0] == 1 && checkPlant[2] == 1)
                {
                    // printf("BullshitStartDayCount: %d\n", BullshitStartDayCount);
                    BullshitStartDayCount++;
                    checkPlant[0] = 0;
                    checkPlant[1] = 0;
                    checkPlant[2] = 0;
                }
            }
        }
        // //printf("Quantity: %d\n", *(Bullshit->Quantity));
        else if (checkPlant[1] == 0 && (*(Bullshit->Quantity) > 300 || checkPlant[0] == 1)) // if the quantity is more than 300, use Plant_Y; or Plant_X and Plant_Z are both used
        {
            if (*(Bullshit->Quantity) <= 400)
            {
                *(Bullshit->Quantity) = 0;
                checkPlant[1] = 1;
                if (checkPlant[1] == 1 && checkPlant[0] == 1 && checkPlant[2] == 1)
                {
                    // printf("BullshitStartDayCount: %d\n", BullshitStartDayCount);
                    BullshitStartDayCount++;
                    checkPlant[0] = 0;
                    checkPlant[1] = 0;
                    checkPlant[2] = 0;
                }
                // //printf("Break\n");
                break;
            }
            else
            {
                *(Bullshit->Quantity) -= 400;
                checkPlant[1] = 1;
                if (checkPlant[1] == 1 && checkPlant[0] == 1 && checkPlant[2] == 1)
                {
                    // printf("BullshitStartDayCount: %d\n", BullshitStartDayCount);
                    BullshitStartDayCount++;
                    checkPlant[0] = 0;
                    checkPlant[1] = 0;
                    checkPlant[2] = 0;
                }
            }
        }
        else if (checkPlant[0] == 0 && *(Bullshit->Quantity) > 0) // if the quantity is more than 0, use Plant_X;
        {
            if (*(Bullshit->Quantity) <= 300)
            {
                *(Bullshit->Quantity) = 0;
                checkPlant[0] = 1;
                if (checkPlant[1] == 1 && checkPlant[0] == 1 && checkPlant[2] == 1)
                {
                    // printf("BullshitStartDayCount: %d\n", BullshitStartDayCount);
                    BullshitStartDayCount++;
                    checkPlant[0] = 0;
                    checkPlant[1] = 0;
                    checkPlant[2] = 0;
                }
                // //printf("Break\n");
                break;
            }
            else
            {
                *(Bullshit->Quantity) -= 300;
                checkPlant[0] = 1;
                if (checkPlant[1] == 1 && checkPlant[0] == 1 && checkPlant[2] == 1)
                {
                    // printf("BullshitStartDayCount: %d\n", BullshitStartDayCount);
                    BullshitStartDayCount++;
                    checkPlant[0] = 0;
                    checkPlant[1] = 0;
                    checkPlant[2] = 0;
                }
            }
        }
    }
    // printf("BullshitStartDayCount: %d\n", BullshitStartDayCount);
    // printf("Bullshit->DDL: %d\n", *(Bullshit->DDL));
    //  check if the order's ddl is earlier than the start day
    if (*(Bullshit->DDL) < BullshitStartDayCount || BullshitStartDayCount + 1 > ScheduleEndDay)
    {
        // printf("Rejected: %s\n", Bullshit->OrderName);
        return 0;
    }
    // printf("Accepted: %s\n", Bullshit->OrderName);
    return 1;
}

void run()
{
    // Read the input file to orders
    parseFile("BonusInput.txt");

    // printf("ScheduleStartDay: %d\n", ScheduleStartDay);
    // printf("ScheduleEndDay: %d\n", ScheduleEndDay);
    //  print the orders
    //  for (i = 0; i < OrderCount; i++)
    //  {
    //      printOrder(&Orders[i]);
    //  }

    // CHAI Schedule
    CHAISchedule();
}

int main()
{
    // get time before running the program
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    run();

    // get time after running the program
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    // print the time used
    // printf("Time used: %f\n", cpu_time_used);

    return 0;
}
