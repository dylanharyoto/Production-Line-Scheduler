#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int calculateDate(char *startYear, char *startMonth, char *startDay, char *endYear, char *endMonth, char *endDay)
{
    int startYearInt = (startYear[0] - '0') * 1000 + (startYear[1] - '0') * 100 + (startYear[2] - '0') * 10 + (startYear[3] - '0');
    int startMonthInt = (startMonth[0] - '0') * 10 + (startMonth[1] - '0');
    int startDayInt = (startDay[0] - '0') * 10 + (startDay[1] - '0');
    int endYearInt = (endYear[0] - '0') * 1000 + (endYear[1] - '0') * 100 + (endYear[2] - '0') * 10 + (endYear[3] - '0');
    int endMonthInt = (endMonth[0] - '0') * 10 + (endMonth[1] - '0');
    int endDayInt = (endDay[0] - '0') * 10 + (endDay[1] - '0');
    // decide the number of days between the two dates
    int daysInMonthNormal[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int daysInMonthLeap[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int days = 0;
    int i;
    for (i = startYearInt; i < endYearInt; i++)
    {
        if (i % 4 == 0)
        {
            days += 366;
        }
        else
        {
            days += 365;
        }
    }
    for (i = 0; i < startMonthInt - 1; i++)
    {
        if (startYearInt % 4 == 0)
        {
            days -= daysInMonthLeap[i];
        }
        else
        {
            days -= daysInMonthNormal[i];
        }
    }
    days -= startDayInt;
    for (i = 0; i < endMonthInt - 1; i++)
    {
        if (endYearInt % 4 == 0)
        {
            days += daysInMonthLeap[i];
        }
        else
        {
            days += daysInMonthNormal[i];
        }
    }
    days += endDayInt;
    return days + 1;
}

int compareDate(char *date1, char *date2)
{
    // The format of two dates is "YYYY-MM-DD"
    // This function is used to compare these two dates and return 1 if day1 is earlier than day2,

    int year1 = (date1[0] - '0') * 1000 + (date1[1] - '0') * 100 + (date1[2] - '0') * 10 + (date1[3] - '0');
    int year2 = (date2[0] - '0') * 1000 + (date2[1] - '0') * 100 + (date2[2] - '0') * 10 + (date2[3] - '0');
    int month1 = (date1[5] - '0') * 10 + (date1[6] - '0');
    int month2 = (date2[5] - '0') * 10 + (date2[6] - '0');
    int day1 = (date1[8] - '0') * 10 + (date1[9] - '0');
    int day2 = (date2[8] - '0') * 10 + (date2[9] - '0');
    int answer = 0;
    if (year1 < year2)
    {
        answer = 1;
    }
    else if (year1 == year2)
    {
        if (month1 < month2)
        {
            answer = 1;
        }
        else if (month1 == month2)
        {
            if (day1 < day2)
            {
                answer = 1;
            }
        }
    }
    return answer;
}

char *getDate(int plant_index, char *startYear, char *startMonth, char *startDay)
{
    // plant_index is the number of days from the start date
    int startYearInt = (startYear[0] - '0') * 1000 + (startYear[1] - '0') * 100 + (startYear[2] - '0') * 10 + (startYear[3] - '0');
    int startMonthInt = (startMonth[0] - '0') * 10 + (startMonth[1] - '0');
    int startDayInt = (startDay[0] - '0') * 10 + (startDay[1] - '0');
    int daysInMonthNormal[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int daysInMonthLeap[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int i;
    for (i = 0; i < plant_index; i++)
    {
        startDayInt++;
        if (startYearInt % 4 == 0)
        {
            if (startDayInt > daysInMonthLeap[startMonthInt - 1])
            {
                startDayInt = 1;
                startMonthInt++;
                if (startMonthInt > 12)
                {
                    startMonthInt = 1;
                    startYearInt++;
                }
            }
        }
        else
        {
            if (startDayInt > daysInMonthNormal[startMonthInt - 1])
            {
                startDayInt = 1;
                startMonthInt++;
                if (startMonthInt > 12)
                {
                    startMonthInt = 1;
                    startYearInt++;
                }
            }
        }
    }
    char *date = malloc(9);
    date[0] = startYearInt / 1000 + '0';
    date[1] = (startYearInt % 1000) / 100 + '0';
    date[2] = (startYearInt % 100) / 10 + '0';
    date[3] = startYearInt % 10 + '0';
    date[4] = '-';
    date[5] = startMonthInt / 10 + '0';
    date[6] = startMonthInt % 10 + '0';
    date[7] = '-';
    date[8] = startDayInt / 10 + '0';
    date[9] = startDayInt % 10 + '0';
    date[10] = '\0';
    return date;
}

int determineReject(int dateNum, int plantX_index, int plantY_index, int plantZ_index, int quantity, char *dueYear, char *dueMonth, char *dueDay, char *startYear, char *startMonth, char *startDay)
{
    // base case: quantity less than 0
    if (quantity <= 0)
    {
        return 0;
    }
    // base case: all the plants are full
    if (plantX_index >= dateNum && plantY_index >= dateNum && plantZ_index >= dateNum)
    {
        return 1;
    }
    char *date;
    char *dueDate = getDate(0, dueYear, dueMonth, dueDay);
    // Case I: only Plant X is available
    if (plantX_index < plantY_index && plantX_index < plantZ_index)
    {
        date = getDate(plantX_index, startYear, startMonth, startDay);
        if (compareDate(dueDate, date) == 1)
        {
            return 1;
        }
        else
        {
            return determineReject(dateNum, plantX_index + 1, plantY_index, plantZ_index, quantity - 300, dueYear, dueMonth, dueDay, startYear, startMonth, startDay);
        }
    }
    // Case II: only Plant Y is available
    if (plantY_index < plantX_index && plantY_index < plantZ_index)
    {
        date = getDate(plantY_index, startYear, startMonth, startDay);
        if (compareDate(dueDate, date) == 1)
        {
            return 1;
        }
        else
        {
            return determineReject(dateNum, plantX_index, plantY_index + 1, plantZ_index, quantity - 400, dueYear, dueMonth, dueDay, startYear, startMonth, startDay);
        }
    }
    // Case III: only Plant Z is available
    if (plantZ_index < plantX_index && plantZ_index < plantY_index)
    {
        date = getDate(plantZ_index, startYear, startMonth, startDay);
        if (compareDate(dueDate, date) == 1)
        {
            return 1;
        }
        else
        {
            return determineReject(dateNum, plantX_index, plantY_index, plantZ_index + 1, quantity - 500, dueYear, dueMonth, dueDay, startYear, startMonth, startDay);
        }
    }
    // Case IV: Plant X and Plant Y are available
    if (plantX_index == plantY_index && plantX_index < plantZ_index)
    {
        date = getDate(plantX_index, startYear, startMonth, startDay);
        if (compareDate(dueDate, date) == 1)
        {
            return 1;
        }
        else
        {
            if (quantity <= 300)
            {
                return determineReject(dateNum, plantX_index + 1, plantY_index, plantZ_index, quantity - 300, dueYear, dueMonth, dueDay, startYear, startMonth, startDay);
            }
            else
            {
                return determineReject(dateNum, plantX_index, plantY_index + 1, plantZ_index, quantity - 400, dueYear, dueMonth, dueDay, startYear, startMonth, startDay);
            }
        }
    }
    // Case V: Plant X and Plant Z are available
    if (plantX_index == plantZ_index && plantX_index < plantY_index)
    {
        date = getDate(plantX_index, startYear, startMonth, startDay);
        if (compareDate(dueDate, date) == 1)
        {
            return 1;
        }
        else
        {
            if (quantity <= 300)
            {
                return determineReject(dateNum, plantX_index + 1, plantY_index, plantZ_index, quantity - 300, dueYear, dueMonth, dueDay, startYear, startMonth, startDay);
            }
            else
            {
                return determineReject(dateNum, plantX_index, plantY_index, plantZ_index + 1, quantity - 500, dueYear, dueMonth, dueDay, startYear, startMonth, startDay);
            }
        }
    }
    // Case VI: Plant Y and Plant Z are available
    if (plantY_index == plantZ_index && plantY_index < plantX_index)
    {
        date = getDate(plantY_index, startYear, startMonth, startDay);
        if (compareDate(dueDate, date) == 1)
        {
            return 1;
        }
        else
        {
            if (quantity <= 400)
            {
                return determineReject(dateNum, plantX_index, plantY_index + 1, plantZ_index, quantity - 400, dueYear, dueMonth, dueDay, startYear, startMonth, startDay);
            }
            else
            {
                return determineReject(dateNum, plantX_index, plantY_index, plantZ_index + 1, quantity - 500, dueYear, dueMonth, dueDay, startYear, startMonth, startDay);
            }
        }
    }
    // Case VII: All the plants are available
    if (plantX_index == plantY_index && plantY_index == plantZ_index)
    {
        date = getDate(plantX_index, startYear, startMonth, startDay);
        if (compareDate(dueDate, date) == 1)
        {
            return 1;
        }
        else
        {
            if (quantity <= 300)
            {
                return determineReject(dateNum, plantX_index + 1, plantY_index, plantZ_index, quantity - 300, dueYear, dueMonth, dueDay, startYear, startMonth, startDay);
            }
            else if (quantity <= 400)
            {
                return determineReject(dateNum, plantX_index, plantY_index + 1, plantZ_index, quantity - 400, dueYear, dueMonth, dueDay, startYear, startMonth, startDay);
            }
            else
            {
                return determineReject(dateNum, plantX_index, plantY_index, plantZ_index + 1, quantity - 500, dueYear, dueMonth, dueDay, startYear, startMonth, startDay);
            }
        }
    }
    return -1; // some issue happened
}

void assignOneSlot(char *orderNum, char *quantityChar, char *productName, char *plant_schedule[], int plant_index, char *startYear, char *startMonth, char *startDay, char *dueYear, char *dueMonth, char *dueDay)
{
    // plant_schedule[plant_index] = malloc(strlen(orderNum) + strlen(quantityChar) + strlen(productName) + 3);
    char *date = getDate(plant_index, startYear, startMonth, startDay);
    char *dueDate = malloc(11);
    dueDate[0] = dueYear[0];
    dueDate[1] = dueYear[1];
    dueDate[2] = dueYear[2];
    dueDate[3] = dueYear[3];
    dueDate[4] = '-';
    dueDate[5] = dueMonth[0];
    dueDate[6] = dueMonth[1];
    dueDate[7] = '-';
    dueDate[8] = dueDay[0];
    dueDate[9] = dueDay[1];
    dueDate[10] = '\0';
    // plant_schedule[plant_index] = malloc(strlen(date) + strlen(orderNum) + strlen(quantityChar) + strlen(productName) + 4);
    plant_schedule[plant_index] = malloc(strlen(date) + strlen(orderNum) + strlen(quantityChar) + strlen(productName) + strlen(dueDate) + 4);
    strcpy(plant_schedule[plant_index], date);
    strcat(plant_schedule[plant_index], "|");
    strcat(plant_schedule[plant_index], orderNum);
    strcat(plant_schedule[plant_index], "|");
    strcat(plant_schedule[plant_index], quantityChar);
    strcat(plant_schedule[plant_index], "|");
    strcat(plant_schedule[plant_index], productName);
    strcat(plant_schedule[plant_index], "|");
    strcat(plant_schedule[plant_index], dueDate);
}

void assignOrderFCFS(char *orderNum, int quantity, char *quantityChar, char *productName, char *plantX_schedule[], char *plantY_schedule[], char *plantZ_schedule[], int dateNum, int *plantX_index, int *plantY_index, int *plantZ_index, char *startYear, char *startMonth, char *startDay, int *ifReject, char *dueYear, char *dueMonth, char *dueDay, int *plantX_quantity, int *plantY_quantity, int *plantZ_quantity)
{
    // base caseI: no more product to assign
    if (quantity <= 0)
        return;
    // base caseII: all the plants are full
    if ((*plantX_index) >= dateNum && (*plantY_index) >= dateNum && (*plantZ_index) >= dateNum)
    {
        *ifReject = 1;
        return;
    }
    // base case III: the order is rejected
    if (determineReject(dateNum, *plantX_index, *plantY_index, *plantZ_index, quantity, dueYear, dueMonth, dueDay, startYear, startMonth, startDay) == 1)
    {
        *ifReject = 1;
        return;
    }
    // Case I: Only Plant X is available
    if ((*plantX_index) < (*plantY_index) && (*plantX_index) < (*plantZ_index))
    {
        if (quantity <= 300)
        {
            // assign the order to plant A: plantX_schedule[plantX_index]= orderNum|quantity|productName
            assignOneSlot(orderNum, quantityChar, productName, plantX_schedule, *plantX_index, startYear, startMonth, startDay, dueYear, dueMonth, dueDay);
            (*plantX_index)++;
            *plantX_quantity += quantity;
        }
        else
        {
            // assign the order to plant A: plantX_schedule[plantX_index]= orderNum|300|productName
            assignOneSlot(orderNum, "300", productName, plantX_schedule, *plantX_index, startYear, startMonth, startDay, dueYear, dueMonth, dueDay);
            (*plantX_index)++;
            *plantX_quantity += 300;
        }
        // change the quantityChar
        char str[20];
        sprintf(str, "%d", quantity - 300);
        assignOrderFCFS(orderNum, quantity - 300, str, productName, plantX_schedule, plantY_schedule, plantZ_schedule, dateNum, plantX_index, plantY_index, plantZ_index, startYear, startMonth, startDay, ifReject, dueYear, dueMonth, dueDay, plantX_quantity, plantY_quantity, plantZ_quantity);
    }
    // Case II: Only Plant Y is available
    else if ((*plantY_index) < (*plantX_index) && (*plantY_index) < (*plantZ_index))
    {
        if (quantity <= 400)
        {
            // assign the order to plant B: plantY_schedule[plantY_index]= orderNum|quantity|productName
            assignOneSlot(orderNum, quantityChar, productName, plantY_schedule, *plantY_index, startYear, startMonth, startDay, dueYear, dueMonth, dueDay);
            (*plantY_index)++;
            *plantY_quantity += quantity;
        }
        else
        {
            // assign the order to plant B: plantY_schedule[plantY_index]= orderNum|400|productName
            assignOneSlot(orderNum, "400", productName, plantY_schedule, *plantY_index, startYear, startMonth, startDay, dueYear, dueMonth, dueDay);
            (*plantY_index)++;
            *plantY_quantity += 400;
        }
        char str[20];
        sprintf(str, "%d", quantity - 400);
        assignOrderFCFS(orderNum, quantity - 400, str, productName, plantX_schedule, plantY_schedule, plantZ_schedule, dateNum, plantX_index, plantY_index, plantZ_index, startYear, startMonth, startDay, ifReject, dueYear, dueMonth, dueDay, plantX_quantity, plantY_quantity, plantZ_quantity);
    }
    // Case III: Only Plant Z is available
    else if ((*plantZ_index) < (*plantX_index) && (*plantZ_index) < (*plantY_index))
    {
        if (quantity <= 500)
        {
            // assign the order to plant C: plantZ_schedule[plantZ_index]= orderNum|quantity|productName
            assignOneSlot(orderNum, quantityChar, productName, plantZ_schedule, *plantZ_index, startYear, startMonth, startDay, dueYear, dueMonth, dueDay);
            (*plantZ_index)++;
            *plantZ_quantity += quantity;
        }
        else
        {
            // assign the order to plant C: plantZ_schedule[plantZ_index]= orderNum|500|productName
            assignOneSlot(orderNum, "500", productName, plantZ_schedule, *plantZ_index, startYear, startMonth, startDay, dueYear, dueMonth, dueDay);
            (*plantZ_index)++;
            *plantZ_quantity += 500;
        }
        char str[20];
        sprintf(str, "%d", quantity - 500);
        assignOrderFCFS(orderNum, quantity - 500, str, productName, plantX_schedule, plantY_schedule, plantZ_schedule, dateNum, plantX_index, plantY_index, plantZ_index, startYear, startMonth, startDay, ifReject, dueYear, dueMonth, dueDay, plantX_quantity, plantY_quantity, plantZ_quantity);
    }
    // Case IV: Only Plant X is unavailable
    else if ((*plantX_index) > (*plantY_index) && (*plantY_index) == (*plantZ_index))
    {
        if (quantity <= 400)
        {
            // assign the order to plant B: plantY_schedule[plantY_index]= orderNum|quantity|productName
            assignOneSlot(orderNum, quantityChar, productName, plantY_schedule, *plantY_index, startYear, startMonth, startDay, dueYear, dueMonth, dueDay);
            (*plantY_index)++;
            *plantY_quantity += quantity;
            char str[20];
            sprintf(str, "%d", quantity - 400);
            assignOrderFCFS(orderNum, quantity - 400, str, productName, plantX_schedule, plantY_schedule, plantZ_schedule, dateNum, plantX_index, plantY_index, plantZ_index, startYear, startMonth, startDay, ifReject, dueYear, dueMonth, dueDay, plantX_quantity, plantY_quantity, plantZ_quantity);
        }
        else if (quantity <= 500)
        {
            // assign the order to plant C: plantZ_schedule[plantZ_index]= orderNum|quantity|productName
            assignOneSlot(orderNum, quantityChar, productName, plantZ_schedule, *plantZ_index, startYear, startMonth, startDay, dueYear, dueMonth, dueDay);
            (*plantZ_index)++;
            *plantZ_quantity += quantity;
            char str[20];
            sprintf(str, "%d", quantity - 500);
            assignOrderFCFS(orderNum, quantity - 500, str, productName, plantX_schedule, plantY_schedule, plantZ_schedule, dateNum, plantX_index, plantY_index, plantZ_index, startYear, startMonth, startDay, ifReject, dueYear, dueMonth, dueDay, plantX_quantity, plantY_quantity, plantZ_quantity);
        }
        else
        {
            // assign the order to plant C: plantZ_schedule[plantZ_index]= orderNum|500|productName
            assignOneSlot(orderNum, "500", productName, plantZ_schedule, *plantZ_index, startYear, startMonth, startDay, dueYear, dueMonth, dueDay);
            (*plantZ_index)++;
            *plantZ_quantity += 500;
            char str[20];
            sprintf(str, "%d", quantity - 500);
            assignOrderFCFS(orderNum, quantity - 500, str, productName, plantX_schedule, plantY_schedule, plantZ_schedule, dateNum, plantX_index, plantY_index, plantZ_index, startYear, startMonth, startDay, ifReject, dueYear, dueMonth, dueDay, plantX_quantity, plantY_quantity, plantZ_quantity);
        }
    }
    // Case V: Only Plant Y is unavailable
    else if ((*plantY_index) > (*plantX_index) && (*plantX_index) == (*plantZ_index))
    {
        if (quantity <= 300)
        {
            // assign the order to plant A: plantX_schedule[plantX_index]= orderNum|quantity|productName
            assignOneSlot(orderNum, quantityChar, productName, plantX_schedule, *plantX_index, startYear, startMonth, startDay, dueYear, dueMonth, dueDay);
            (*plantX_index)++;
            *plantX_quantity += quantity;
            char str[20];
            sprintf(str, "%d", quantity - 300);
            assignOrderFCFS(orderNum, quantity - 300, str, productName, plantX_schedule, plantY_schedule, plantZ_schedule, dateNum, plantX_index, plantY_index, plantZ_index, startYear, startMonth, startDay, ifReject, dueYear, dueMonth, dueDay, plantX_quantity, plantY_quantity, plantZ_quantity);
        }
        else if (quantity <= 500)
        {
            // assign the order to plant C: plantZ_schedule[plantZ_index]= orderNum|quantity|productName
            assignOneSlot(orderNum, quantityChar, productName, plantZ_schedule, *plantZ_index, startYear, startMonth, startDay, dueYear, dueMonth, dueDay);
            (*plantZ_index)++;
            *plantZ_quantity += quantity;
            char str[20];
            sprintf(str, "%d", quantity - 500);
            assignOrderFCFS(orderNum, quantity - 500, str, productName, plantX_schedule, plantY_schedule, plantZ_schedule, dateNum, plantX_index, plantY_index, plantZ_index, startYear, startMonth, startDay, ifReject, dueYear, dueMonth, dueDay, plantX_quantity, plantY_quantity, plantZ_quantity);
        }
        else
        {
            // assign the order to plant C: plantZ_schedule[plantZ_index]= orderNum|500|productName
            assignOneSlot(orderNum, "500", productName, plantZ_schedule, *plantZ_index, startYear, startMonth, startDay, dueYear, dueMonth, dueDay);
            (*plantZ_index)++;
            *plantZ_quantity += 500;
            char str[20];
            sprintf(str, "%d", quantity - 500);
            assignOrderFCFS(orderNum, quantity - 500, str, productName, plantX_schedule, plantY_schedule, plantZ_schedule, dateNum, plantX_index, plantY_index, plantZ_index, startYear, startMonth, startDay, ifReject, dueYear, dueMonth, dueDay, plantX_quantity, plantY_quantity, plantZ_quantity);
        }
    }
    // Case VI: Only Plant Z is unavailable
    else if ((*plantZ_index) > (*plantX_index) && (*plantX_index) == (*plantY_index))
    {
        if (quantity <= 300)
        {
            // assign the order to plant A: plantX_schedule[plantX_index]= orderNum|quantity|productName
            assignOneSlot(orderNum, quantityChar, productName, plantX_schedule, *plantX_index, startYear, startMonth, startDay, dueYear, dueMonth, dueDay);
            (*plantX_index)++;
            *plantX_quantity += quantity;
            char str[20];
            sprintf(str, "%d", quantity - 300);
            assignOrderFCFS(orderNum, quantity - 300, str, productName, plantX_schedule, plantY_schedule, plantZ_schedule, dateNum, plantX_index, plantY_index, plantZ_index, startYear, startMonth, startDay, ifReject, dueYear, dueMonth, dueDay, plantX_quantity, plantY_quantity, plantZ_quantity);
        }
        else if (quantity <= 400)
        {
            // assign the order to plant B: plantY_schedule[plantY_index]= orderNum|quantity|productName
            assignOneSlot(orderNum, quantityChar, productName, plantY_schedule, *plantY_index, startYear, startMonth, startDay, dueYear, dueMonth, dueDay);
            (*plantY_index)++;
            *plantY_quantity += quantity;
            char str[20];
            sprintf(str, "%d", quantity - 400);
            assignOrderFCFS(orderNum, quantity - 400, str, productName, plantX_schedule, plantY_schedule, plantZ_schedule, dateNum, plantX_index, plantY_index, plantZ_index, startYear, startMonth, startDay, ifReject, dueYear, dueMonth, dueDay, plantX_quantity, plantY_quantity, plantZ_quantity);
        }
        else
        {
            // assign the order to plant B: plantY_schedule[plantY_index]= orderNum|400|productName
            assignOneSlot(orderNum, "400", productName, plantY_schedule, *plantY_index, startYear, startMonth, startDay, dueYear, dueMonth, dueDay);
            (*plantY_index)++;
            *plantY_quantity += 400;
            char str[20];
            sprintf(str, "%d", quantity - 400);
            assignOrderFCFS(orderNum, quantity - 400, str, productName, plantX_schedule, plantY_schedule, plantZ_schedule, dateNum, plantX_index, plantY_index, plantZ_index, startYear, startMonth, startDay, ifReject, dueYear, dueMonth, dueDay, plantX_quantity, plantY_quantity, plantZ_quantity);
        }
    }
    // Case VII: All the plants are available
    else if ((*plantX_index) == (*plantY_index) && (*plantY_index) == (*plantZ_index))
    {
        if (quantity <= 300)
        {
            // assign the order to plant A: plantX_schedule[plantX_index]= orderNum|quantity|productName
            assignOneSlot(orderNum, quantityChar, productName, plantX_schedule, *plantX_index, startYear, startMonth, startDay, dueYear, dueMonth, dueDay);
            (*plantX_index)++;
            *plantX_quantity += quantity;
            char str[20];
            sprintf(str, "%d", quantity - 300);
            assignOrderFCFS(orderNum, quantity - 300, str, productName, plantX_schedule, plantY_schedule, plantZ_schedule, dateNum, plantX_index, plantY_index, plantZ_index, startYear, startMonth, startDay, ifReject, dueYear, dueMonth, dueDay, plantX_quantity, plantY_quantity, plantZ_quantity);
        }
        else if (quantity <= 400)
        {
            // assign the order to plant B: plantY_schedule[plantY_index]= orderNum|quantity|productName
            assignOneSlot(orderNum, quantityChar, productName, plantY_schedule, *plantY_index, startYear, startMonth, startDay, dueYear, dueMonth, dueDay);
            (*plantY_index)++;
            *plantY_quantity += quantity;
            char str[20];
            sprintf(str, "%d", quantity - 400);
            assignOrderFCFS(orderNum, quantity - 400, str, productName, plantX_schedule, plantY_schedule, plantZ_schedule, dateNum, plantX_index, plantY_index, plantZ_index, startYear, startMonth, startDay, ifReject, dueYear, dueMonth, dueDay, plantX_quantity, plantY_quantity, plantZ_quantity);
        }
        else if (quantity <= 500)
        {
            // assign the order to plant C: plantZ_schedule[plantZ_index]= orderNum|quantity|productName
            assignOneSlot(orderNum, quantityChar, productName, plantZ_schedule, *plantZ_index, startYear, startMonth, startDay, dueYear, dueMonth, dueDay);
            (*plantZ_index)++;
            *plantZ_quantity += quantity;
            char str[20];
            sprintf(str, "%d", quantity - 500);
            assignOrderFCFS(orderNum, quantity - 500, str, productName, plantX_schedule, plantY_schedule, plantZ_schedule, dateNum, plantX_index, plantY_index, plantZ_index, startYear, startMonth, startDay, ifReject, dueYear, dueMonth, dueDay, plantX_quantity, plantY_quantity, plantZ_quantity);
        }
        else
        {
            // assign the order to plant C: plantZ_schedule[plantZ_index]= orderNum|500|productName
            assignOneSlot(orderNum, "500", productName, plantZ_schedule, *plantZ_index, startYear, startMonth, startDay, dueYear, dueMonth, dueDay);
            (*plantZ_index)++;
            *plantZ_quantity += 500;
            char str[20];
            sprintf(str, "%d", quantity - 500);
            assignOrderFCFS(orderNum, quantity - 500, str, productName, plantX_schedule, plantY_schedule, plantZ_schedule, dateNum, plantX_index, plantY_index, plantZ_index, startYear, startMonth, startDay, ifReject, dueYear, dueMonth, dueDay, plantX_quantity, plantY_quantity, plantZ_quantity);
        }
    }
}

void assignOrderSJF(int orderAmount, char *orderNums[], int *quantities[], char *quantityChars[], char *productNames[], char *plantX_schedule[], char *plantY_schedule[], char *plantZ_schedule[], int dateNum, int *plantX_index, int *plantY_index, int *plantZ_index, char *startYear, char *startMonth, char *startDay, int *ifReject[], char *dueYear[], char *dueMonth[], char *dueDay[], int *plantX_quantity, int *plantY_quantity, int *plantZ_quantity)
{
    // base case: all the orders are assigned
    int i;
    int sum = 0;
    for (i = 0; i < orderAmount; i++)
    {
        sum += *quantities[i];
    }
    if (sum == 0)
    {
        return;
    }
    // compare the quantities of the orders and select the order with the smallest quantity which is larger than 0
    int minIndex = 0;
    int minQuantity = *quantities[0];
    while (minQuantity == 0)
    {
        minIndex++;
        minQuantity = *quantities[minIndex];
    }
    for (i = 1; i < orderAmount; i++)
    {
        if (*quantities[i] < minQuantity && *quantities[i] > 0)
        {
            minQuantity = *quantities[i];
            minIndex = i;
        }
    }
    // allocate the memory for ifReject[minIndex]
    ifReject[minIndex] = malloc(sizeof(int));
    *ifReject[minIndex] = 0;
    // assign the order with the smallest quantity to the plant
    assignOrderFCFS(orderNums[minIndex], minQuantity, quantityChars[minIndex], productNames[minIndex], plantX_schedule, plantY_schedule, plantZ_schedule, dateNum, plantX_index, plantY_index, plantZ_index, startYear, startMonth, startDay, ifReject[minIndex], dueYear[minIndex], dueMonth[minIndex], dueDay[minIndex], plantX_quantity, plantY_quantity, plantZ_quantity);
    // change the quantity of the assigned order to 0
    *quantities[minIndex] = 0;
    // assign the rest of the orders
    assignOrderSJF(orderAmount, orderNums, quantities, quantityChars, productNames, plantX_schedule, plantY_schedule, plantZ_schedule, dateNum, plantX_index, plantY_index, plantZ_index, startYear, startMonth, startDay, ifReject, dueYear, dueMonth, dueDay, plantX_quantity, plantY_quantity, plantZ_quantity);
}

void schedulePrinter(char *plantX_schedule[], char *plantY_schedule[], char *plantZ_schedule[], int plantX_index, int plantY_index, int plantZ_index, char *startYear, char *startMonth, char *startDay, char *endYear, char *endMonth, char *endDay, FILE *outputFile, int dateNum)
{
    // print 85 "-" to separate the schedules of different plants
    fprintf(outputFile, "-------------------------------------------------------------------------------------\n");
    fprintf(outputFile, "Plant_X (300 per day)\n");
    fprintf(outputFile, "%s-%s-%s to %s-%s-%s\n\n", startYear, startMonth, startDay, endYear, endMonth, endDay);
    fprintf(outputFile, "%15s%15s%15s%15s%15s\n", "Date", "Order Number", "Quantity", "Product Name", "Due Date");
    int i;
    for (i = 0; i < dateNum; i++)
    {
        if (i < plantX_index)
        {
            // divide the plantX_schedule[i] into date, orderNum, quantity, productName dueDate according to the delimiter "|"
            char tempSchedule[1000];
            strcpy(tempSchedule, plantX_schedule[i]);
            char *date = strtok(tempSchedule, "|");
            char *orderNum = strtok(NULL, "|");
            char *quantity = strtok(NULL, "|");
            char *productName = strtok(NULL, "|");
            char *dueDate = strtok(NULL, "|");
            fprintf(outputFile, "%15s%15s%15s%15s%15s\n", date, orderNum, quantity, productName, dueDate);
        }
        else
        {
            char *date = getDate(i, startYear, startMonth, startDay);
            fprintf(outputFile, "%15s%15s%15s%15s%15s\n", date, "NA", "NA", "NA", "NA");
        }
    }
    fprintf(outputFile, "-------------------------------------------------------------------------------------\n");
    fprintf(outputFile, "Plant_Y (400 per day)\n");
    fprintf(outputFile, "%s-%s-%s to %s-%s-%s\n\n", startYear, startMonth, startDay, endYear, endMonth, endDay);
    fprintf(outputFile, "%15s%15s%15s%15s%15s\n", "Date", "Order Number", "Quantity", "Product Name", "Due Date");
    for (i = 0; i < dateNum; i++)
    {
        if (i < plantY_index)
        {
            // divide the plantY_schedule[i] into date, orderNum, quantity, productName dueDate according to the delimiter "|"
            char tempSchedule[1000];
            strcpy(tempSchedule, plantY_schedule[i]);
            char *date = strtok(tempSchedule, "|");
            char *orderNum = strtok(NULL, "|");
            char *quantity = strtok(NULL, "|");
            char *productName = strtok(NULL, "|");
            char *dueDate = strtok(NULL, "|");
            fprintf(outputFile, "%15s%15s%15s%15s%15s\n", date, orderNum, quantity, productName, dueDate);
        }
        else
        {
            // Get the date
            char *date = getDate(i, startYear, startMonth, startDay);
            fprintf(outputFile, "%15s%15s%15s%15s%15s\n", date, "NA", "NA", "NA", "NA");
        }
    }
    fprintf(outputFile, "-------------------------------------------------------------------------------------\n");
    fprintf(outputFile, "Plant_Z (500 per day)\n");
    fprintf(outputFile, "%s-%s-%s to %s-%s-%s\n\n", startYear, startMonth, startDay, endYear, endMonth, endDay);
    fprintf(outputFile, "%15s%15s%15s%15s%15s\n", "Date", "Order Number", "Quantity", "Product Name", "Due Date");
    for (i = 0; i < dateNum; i++)
    {
        if (i < plantZ_index)
        {
            // divide the plantZ_schedule[i] into date, orderNum, quantity, productName dueDate according to the delimiter "|"
            char tempSchedule[1000];
            strcpy(tempSchedule, plantZ_schedule[i]);
            char *date = strtok(tempSchedule, "|");
            char *orderNum = strtok(NULL, "|");
            char *quantity = strtok(NULL, "|");
            char *productName = strtok(NULL, "|");
            char *dueDate = strtok(NULL, "|");
            fprintf(outputFile, "%15s%15s%15s%15s%15s\n", date, orderNum, quantity, productName, dueDate);
        }
        else
        {
            char *date = getDate(i, startYear, startMonth, startDay);
            fprintf(outputFile, "%15s%15s%15s%15s%15s\n", date, "NA", "NA", "NA", "NA");
        }
    }
}

void reportGenerator(char *plantX_schedule[], char *plantY_schedule[], char *plantZ_schedule[], int plantX_index, int plantY_index, int plantZ_index, char *rejectOrders[], int reject_index, char *acceptOrders[], int accept_index, FILE *outputFile, char *algorithm, int plantX_quantity, int plantY_quantity, int plantZ_quantity)
{
    fprintf(outputFile, "***PLS Schedule Analysis Report***\n\n");
    fprintf(outputFile, "Algorithm used: %s\n\n", algorithm);
    fprintf(outputFile, "There are %d Orders ACCEPTED. Details are as follows:\n\n", accept_index);
    fprintf(outputFile, "%15s%15s%15s%15s%15s%15s\n", "ORDER NUMBER", "START", "END", "DAYS", "QUANTITY", "PLANT");
    // print 75 "=" to separate the accepted orders and rejected orders
    fprintf(outputFile, "==========================================================================================\n");
    int i, j;
    for (i = 0; i < accept_index; i++)
    {
        char tempAcceptOrder[1000];
        strcpy(tempAcceptOrder, acceptOrders[i]);
        char *orderNum = strtok(tempAcceptOrder, "|");
        char startDate[11];
        char endDate[11];
        int days = 0;
        int amount = 0;
        for (j = 0; j < plantX_index; j++)
        {
            char tempSchedule1[1000];
            char tempSchedule2[1000];
            strcpy(tempSchedule1, plantX_schedule[j]);
            char *date = strtok(tempSchedule1, "|");
            char *orderNumInSchedule = strtok(NULL, "|");
            char *quantity = strtok(NULL, "|");
            // Get the quantityInt according to quantity
            int quantityInt = 0;
            int k;
            for (k = 0; k < strlen(quantity); k++)
            {
                quantityInt = quantityInt * 10 + quantity[k] - '0';
            }
            if (strcmp(orderNum, orderNumInSchedule) == 0)
            {
                // determine if the startYear has been set or not
                if (days == 0)
                {
                    strcpy(startDate, date);
                }
                days++;
                amount += quantityInt;
                // Get the next slot in the schedule
                if (j + 1 < plantX_index)
                {
                    strcpy(tempSchedule2, plantX_schedule[j + 1]);
                    char *nextDate = strtok(tempSchedule2, "|");
                    char *nextOrderNum = strtok(NULL, "|");
                    if (strcmp(orderNum, nextOrderNum) != 0)
                    {
                        strcpy(endDate, date);
                        break;
                    }
                }
                else
                {
                    strcpy(endDate, date);
                    break;
                }
            }
        }
        if (days != 0)
            fprintf(outputFile, "%15s%15s%15s%15d%15d%15s\n", orderNum, startDate, endDate, days, amount, "Plant_X");
        // Reset the variables
        days = 0;
        amount = 0;
        for (j = 0; j < plantY_index; j++)
        {
            char tempSchedule1[1000];
            char tempSchedule2[1000];
            strcpy(tempSchedule1, plantY_schedule[j]);
            char *date = strtok(tempSchedule1, "|");
            char *orderNumInSchedule = strtok(NULL, "|");
            char *quantity = strtok(NULL, "|");
            // Get the quantityInt according to quantity
            int quantityInt = 0;
            int k;
            for (k = 0; k < strlen(quantity); k++)
            {
                quantityInt = quantityInt * 10 + quantity[k] - '0';
            }
            if (strcmp(orderNum, orderNumInSchedule) == 0)
            {
                // determine if the startYear has been set or not
                if (days == 0)
                {
                    strcpy(startDate, date);
                }
                days++;
                amount += quantityInt;
                // Get the next slot in the schedule
                if (j + 1 < plantY_index)
                {
                    strcpy(tempSchedule2, plantY_schedule[j + 1]);
                    char *nextDate = strtok(tempSchedule2, "|");
                    char *nextOrderNum = strtok(NULL, "|");
                    if (strcmp(orderNum, nextOrderNum) != 0)
                    {
                        strcpy(endDate, date);
                        break;
                    }
                }
                else
                {
                    strcpy(endDate, date);
                    break;
                }
            }
        }
        if (days != 0)
            fprintf(outputFile, "%15s%15s%15s%15d%15d%15s\n", orderNum, startDate, endDate, days, amount, "Plant_Y");
        days = 0;
        amount = 0;
        for (j = 0; j < plantZ_index; j++)
        {
            char tempSchedule1[1000];
            char tempSchedule2[1000];
            strcpy(tempSchedule1, plantZ_schedule[j]);
            char *date = strtok(tempSchedule1, "|");
            char *orderNumInSchedule = strtok(NULL, "|");
            char *quantity = strtok(NULL, "|");
            // Get the quantityInt according to quantity
            int quantityInt = 0;
            int k;
            for (k = 0; k < strlen(quantity); k++)
            {
                quantityInt = quantityInt * 10 + quantity[k] - '0';
            }
            if (strcmp(orderNum, orderNumInSchedule) == 0)
            {
                // determine if the startYear has been set or not
                if (days == 0)
                {
                    strcpy(startDate, date);
                }
                days++;
                amount += quantityInt;
                // Get the next slot in the schedule
                if (j + 1 < plantZ_index)
                {
                    strcpy(tempSchedule2, plantZ_schedule[j + 1]);
                    char *nextDate = strtok(tempSchedule2, "|");
                    char *nextOrderNum = strtok(NULL, "|");
                    if (strcmp(orderNum, nextOrderNum) != 0)
                    {
                        strcpy(endDate, date);
                        break;
                    }
                }
                else
                {
                    strcpy(endDate, date);
                    break;
                }
            }
        }
        if (days != 0)
            fprintf(outputFile, "%15s%15s%15s%15d%15d%15s\n", orderNum, startDate, endDate, days, amount, "Plant_Z");
    }
    fprintf(outputFile, "\n             - End -\n\n");
    fprintf(outputFile, "==========================================================================================\n\n");
    fprintf(outputFile, "There are %d Orders REJECTED. Details are as follows:\n\n", reject_index);
    fprintf(outputFile, "%15s%15s%15s%15s\n", "ORDER NUMBER", "PRODUCT NAME", "DUE DATE", "QUANTITY");
    fprintf(outputFile, "==========================================================================================\n");
    for (i = 0; i < reject_index; i++)
    {
        char tempRejectOrder[1000];
        strcpy(tempRejectOrder, rejectOrders[i]);
        char *orderNum = strtok(tempRejectOrder, "|");
        char *productName = strtok(NULL, "|");
        char *dueDate = strtok(NULL, "|");
        char *quantity = strtok(NULL, "|");
        fprintf(outputFile, "%15s%15s%15s%15s\n", orderNum, productName, dueDate, quantity);
    }
    fprintf(outputFile, "\n             - End -\n\n");
    fprintf(outputFile, "==========================================================================================\n\n");
    fprintf(outputFile, "***PERFORMANCE\n");
    fprintf(outputFile, "Plant_X:\n");
    fprintf(outputFile, "         Number of days in use:%21d days\n", plantX_index);
    fprintf(outputFile, "         Total quantity produced:%15d (in total)\n", plantX_quantity);
    fprintf(outputFile, "         Utilization of the plant:%18.1f %%\n\n", (double)plantX_quantity / (plantX_index * 300) * 100);
    fprintf(outputFile, "Plant_Y:\n");
    fprintf(outputFile, "         Number of days in use:%21d days\n", plantY_index);
    fprintf(outputFile, "         Total quantity produced:%15d (in total)\n", plantY_quantity);
    fprintf(outputFile, "         Utilization of the plant:%18.1f %%\n\n", (double)plantY_quantity / (plantY_index * 400) * 100);
    fprintf(outputFile, "Plant_Z:\n");
    fprintf(outputFile, "         Number of days in use:%21d days\n", plantZ_index);
    fprintf(outputFile, "         Total quantity produced:%15d (in total)\n", plantZ_quantity);
    fprintf(outputFile, "         Utilization of the plant:%18.1f %%\n\n", (double)plantZ_quantity / (plantZ_index * 500) * 100);
    fprintf(outputFile, "Overall of utilization: %28.1f %%\n", (double)(plantX_quantity + plantY_quantity + plantZ_quantity) / ((plantX_index * 300) + (plantY_index * 400) + (plantZ_index * 500)) * 100);
}

void run()
{
    // Read char by char in a file
    printf("\n   ~~WELCOME TO PLS~~\n\n");
    char input[100]; // Assume that input is no morn than 99 characters long

    // create the processes and pipes
    int num_children = 2;
    int childid_array[2];
    int p2c_pipe[num_children][2]; // pipe: parent writes and child reads
    int c2p_pipe[num_children][2]; // pipe: child writes and parent reads

    int p = 0; // counter of the child processes

    for (p = 0; p < num_children; p++)
    {
        if (pipe(p2c_pipe[p]) < 0)
        {
            printf("Pipe creation error\n");
            exit(1); // parent write, child read
        }

        if (pipe(c2p_pipe[p]) < 0)
        {
            printf("Pipe creation error\n");
            exit(1); // parent read, child write
        }
    }

    int forkid = 0;
    for (p = 0; p < num_children; p++)
    {

        forkid = fork();

        if (forkid < 0)
        {
            printf("Error when creating the child processes!\n");
            return;
        }

        if (forkid > 0)
        {
            // parent process
            childid_array[p] = forkid;
            // printf("The child %d is created", forkid);
            close(p2c_pipe[p][0]); // close the parent read end for p2c pipe
            close(c2p_pipe[p][1]); // close the parent write end for c2p pipe
        }

        if (forkid == 0)
        {
            // child process
            // printf("I am child %d\n", getpid());
            close(p2c_pipe[p][1]); // close the child write end for p2c pipe
            close(c2p_pipe[p][0]); // close the child read end for c2p pipe

            char childRead[1000];
            // wait for the pre-processed period related information written by parent process
            int n = read(p2c_pipe[p][0], childRead, 50);
            childRead[n] = 0;

            // printf("Child %d read: %s\n", getpid(), childRead);

            int dateNum = 0;
            char startYear[5], startMonth[3], startDay[3], endYear[5], endMonth[3], endDay[3];
            sscanf(childRead, "%d %s %s %s %s %s %s", &dateNum, startYear, startMonth, startDay, endYear, endMonth, endDay);

            int j;
            char *plantX_schedule[dateNum];
            char *plantY_schedule[dateNum];
            char *plantZ_schedule[dateNum];
            char *rejectOrders[10000];
            char *acceptOrders[10000];
            char *orderNums[10000];
            int *quantities[10000];
            char *quantityChars[10000];
            char *productNames[10000];
            char *dueYears[10000];
            char *dueMonths[10000];
            char *dueDays[10000];
            int *ifRejects[10000];
            int plantX_index = 0;
            int plantY_index = 0;
            int plantZ_index = 0;
            int reject_index = 0;
            int accept_index = 0;
            int orderAmount = 0;
            int plantX_quantity = 0;
            int plantY_quantity = 0;
            int plantZ_quantity = 0;

            char algorithm[20];

            while (1)
            {

                int n = read(p2c_pipe[p][0], childRead, 100);
                childRead[n] = 0;
                // printf("Child %d read: %s\n", getpid(), childRead);

                // end the waiting with message "SystemExit"
                if (strcmp(childRead, "SystemExit") == 0)
                {

                    close(p2c_pipe[p][0]); // close the child read end for p2c pipe
                    close(c2p_pipe[p][1]); // close the child write end for c2p pipe

                    break;
                }

                else if (p == 0)
                {
                    // scheduler kernel process

                    if (strncmp(childRead, "EOF", 3) == 0)
                    {
                        // printf("EOF Received by the scheduler\n");
                        char reportFileName[50];

                        sscanf(childRead, "EOF | %s", reportFileName);

                        FILE *outputFile = fopen("output.txt", "w");
                        FILE *reportFile = fopen(reportFileName, "w");

                        // TEST the output functionality (not the module itself!)
                        if (strcmp(algorithm, "FCFS") == 0)
                        {
                            schedulePrinter(plantX_schedule, plantY_schedule, plantZ_schedule, plantX_index, plantY_index, plantZ_index, startYear, startMonth, startDay, endYear, endMonth, endDay, outputFile, dateNum);

                            write(c2p_pipe[0][1], "SchedulerDone", strlen("SchedulerDone"));
                            reportGenerator(plantX_schedule, plantY_schedule, plantZ_schedule, plantX_index, plantY_index, plantZ_index, rejectOrders, reject_index, acceptOrders, accept_index, reportFile, algorithm, plantX_quantity, plantY_quantity, plantZ_quantity);

                            for (j = 0; j < plantX_index; j++)
                            {
                                plantX_schedule[j] = NULL;
                            }
                            for (j = 0; j < plantY_index; j++)
                            {
                                plantY_schedule[j] = NULL;
                            }
                            for (j = 0; j < plantZ_index; j++)
                            {
                                plantZ_schedule[j] = NULL;
                            }
                            for (j = 0; j < reject_index; j++)
                            {
                                rejectOrders[j] = NULL;
                            }
                            for (j = 0; j < accept_index; j++)
                            {
                                acceptOrders[j] = NULL;
                            }
                        }

                        else if (strcmp(algorithm, "SJF") == 0)
                        {
                            // assignOrderSJF(orderAmount, orderNums, quantities, quantityChars, productNames, plantX_schedule, plantY_schedule, plantZ_schedule, dateNum, &plantX_index, &plantY_index, &plantZ_index, startYear, startMonth, startDay, ifRejects, dueYears, dueMonths, dueDays, &plantX_quantity, &plantY_quantity, &plantZ_quantity);
                            assignOrderSJF(orderAmount, orderNums, quantities, quantityChars, productNames, plantX_schedule, plantY_schedule, plantZ_schedule, dateNum, &plantX_index, &plantY_index, &plantZ_index, startYear, startMonth, startDay, ifRejects, dueYears, dueMonths, dueDays, &plantX_quantity, &plantY_quantity, &plantZ_quantity);
                            schedulePrinter(plantX_schedule, plantY_schedule, plantZ_schedule, plantX_index, plantY_index, plantZ_index, startYear, startMonth, startDay, endYear, endMonth, endDay, outputFile, dateNum);
                            write(c2p_pipe[0][1], "SchedulerDone", strlen("SchedulerDone"));

                            for (j = 0; j < orderAmount; j++)
                            {
                                if (*ifRejects[j] == 1)
                                {
                                    // rejectOrders[reject_index] = malloc(strlen(orderNums[j]) + 1);
                                    rejectOrders[reject_index] = malloc(strlen(orderNums[j]) + strlen(productNames[j]) + strlen(dueYears[j]) + strlen(dueMonths[j]) + strlen(dueDays[j]) + strlen(quantityChars[j]) + 6);
                                    strcpy(rejectOrders[reject_index], orderNums[j]);
                                    strcat(rejectOrders[reject_index], "|");
                                    strcat(rejectOrders[reject_index], productNames[j]);
                                    strcat(rejectOrders[reject_index], "|");
                                    strcat(rejectOrders[reject_index], dueYears[j]);
                                    strcat(rejectOrders[reject_index], "-");
                                    strcat(rejectOrders[reject_index], dueMonths[j]);
                                    strcat(rejectOrders[reject_index], "-");
                                    strcat(rejectOrders[reject_index], dueDays[j]);
                                    strcat(rejectOrders[reject_index], "|");
                                    strcat(rejectOrders[reject_index], quantityChars[j]);

                                    reject_index++;
                                }
                                else
                                {
                                    acceptOrders[accept_index] = malloc(strlen(orderNums[j]) + strlen(productNames[j]) + 2);
                                    strcpy(acceptOrders[accept_index], orderNums[j]);
                                    strcat(acceptOrders[accept_index], "|");
                                    strcat(acceptOrders[accept_index], productNames[j]);
                                    accept_index++;
                                }
                                ifRejects[j] = NULL;
                            }

                            reportGenerator(plantX_schedule, plantY_schedule, plantZ_schedule, plantX_index, plantY_index, plantZ_index, rejectOrders, reject_index, acceptOrders, accept_index, reportFile, algorithm, plantX_quantity, plantY_quantity, plantZ_quantity);

                            for (j = 0; j < plantX_index; j++)
                            {
                                plantX_schedule[j] = NULL;
                            }
                            for (j = 0; j < plantY_index; j++)
                            {
                                plantY_schedule[j] = NULL;
                            }
                            for (j = 0; j < plantZ_index; j++)
                            {
                                plantZ_schedule[j] = NULL;
                            }
                            for (j = 0; j < reject_index; j++)
                            {
                                rejectOrders[j] = NULL;
                            }
                            for (j = 0; j < accept_index; j++)
                            {
                                acceptOrders[j] = NULL;
                            }
                        }
                        fclose(outputFile);
                        fclose(reportFile);

                        // reset the plant time index

                        plantX_index = 0;
                        plantY_index = 0;
                        plantZ_index = 0;
                        reject_index = 0;
                        accept_index = 0;
                        orderAmount = 0;
                        plantX_quantity = 0;
                        plantY_quantity = 0;
                        plantZ_quantity = 0;

                        int n = read(p2c_pipe[0][0], childRead, 50);
                        childRead[n] = 0;
                    }

                    else if (strcmp(childRead, "FCFS") == 0)
                    {

                        write(c2p_pipe[0][1], "AlgorithmSet", strlen("AlgorithmSet"));
                        strcpy(algorithm, "FCFS");
                    }

                    else if (strcmp(childRead, "SJF") == 0)
                    {
                        write(c2p_pipe[0][1], "AlgorithmSet", strlen("AlgorithmSet"));
                        strcpy(algorithm, "SJF");
                    }

                    else if (strncmp(childRead, "FeedLine", 8) == 0)
                    {
                        // printf("Someone is feeding line!\n");
                        char checkLine[n + 1];

                        // skip the "FeedLine" keyword to get the data fed by parent process
                        strcpy(checkLine, childRead + 8);
                        // printf("I received %s from the parent\n", checkLine);

                        if (strcmp(algorithm, "FCFS") == 0)
                        {
                            // printf("Under FCFS received feeding line!\n");
                            //  FCFS scheduling: process the addORDER lines and do the scheduling later

                            char orderNum[5];
                            char dueYear[5];
                            char dueMonth[3];
                            char dueDay[3];
                            int quantity = 0;
                            char productName[10];
                            int ifReject = 0;
                            // divide the checkLine into 5 parts according to the space
                            int k = 0;
                            int l = 0;
                            int m = 0;
                            char *parts[5];
                            char part[100];
                            for (k = 0; k <= strlen(checkLine); k++)
                            {
                                if (checkLine[k] == ' ' || k == strlen(checkLine))
                                {
                                    part[l] = '\0';
                                    parts[m] = malloc(strlen(part) + 1);
                                    strcpy(parts[m], part);
                                    m++;
                                    l = 0;
                                }
                                else
                                {
                                    part[l] = checkLine[k];
                                    l++;
                                }
                            }
                            for (k = 0; k < strlen(parts[1]); k++)
                            {
                                orderNum[k] = parts[1][k];
                            }
                            orderNum[k] = '\0';
                            dueYear[0] = parts[2][0];
                            dueYear[1] = parts[2][1];
                            dueYear[2] = parts[2][2];
                            dueYear[3] = parts[2][3];
                            dueYear[4] = '\0';
                            dueMonth[0] = parts[2][5];
                            dueMonth[1] = parts[2][6];
                            dueMonth[2] = '\0';
                            dueDay[0] = parts[2][8];
                            dueDay[1] = parts[2][9];
                            dueDay[2] = '\0';
                            for (k = 0; k < strlen(parts[3]); k++)
                            {
                                quantity = quantity * 10 + (parts[3][k] - '0');
                            }
                            for (k = 0; k < strlen(parts[4]); k++)
                            {
                                productName[k] = parts[4][k];
                            }
                            productName[k] = '\0';

                            // the scheduler module works now
                            assignOrderFCFS(orderNum, quantity, parts[3], productName, plantX_schedule, plantY_schedule, plantZ_schedule, dateNum, &plantX_index, &plantY_index, &plantZ_index, startYear, startMonth, startDay, &ifReject, dueYear, dueMonth, dueDay, &plantX_quantity, &plantY_quantity, &plantZ_quantity);

                            // decide whether to reject the order
                            if (ifReject)
                            {
                                // rejectOrders[reject_index] = malloc(strlen(orderNum) + strlen(productName) + 2);
                                rejectOrders[reject_index] = malloc(strlen(orderNum) + strlen(productName) + strlen(dueYear) + strlen(dueMonth) + strlen(dueDay) + strlen(parts[3]) + 6);
                                strcpy(rejectOrders[reject_index], orderNum);
                                strcat(rejectOrders[reject_index], "|");
                                strcat(rejectOrders[reject_index], productName);
                                strcat(rejectOrders[reject_index], "|");
                                // Get the due date
                                strcat(rejectOrders[reject_index], dueYear);
                                strcat(rejectOrders[reject_index], "-");
                                strcat(rejectOrders[reject_index], dueMonth);
                                strcat(rejectOrders[reject_index], "-");
                                strcat(rejectOrders[reject_index], dueDay);
                                strcat(rejectOrders[reject_index], "|");
                                strcat(rejectOrders[reject_index], parts[3]);
                                reject_index++;
                            }
                            else
                            {
                                acceptOrders[accept_index] = malloc(strlen(orderNum) + strlen(productName) + 2);
                                strcpy(acceptOrders[accept_index], orderNum);
                                strcat(acceptOrders[accept_index], "|");
                                strcat(acceptOrders[accept_index], productName);
                                accept_index++;
                            }

                            // ask the parent to feed the next line
                            write(c2p_pipe[0][1], "NextLine", strlen("NextLine"));
                        }

                        else if (strcmp(algorithm, "SJF") == 0)
                        {
                            // SJF Algorithm
                            char orderNum[5];
                            char dueYear[5];
                            char dueMonth[3];
                            char dueDay[3];
                            int quantity = 0;
                            char productName[10];
                            orderAmount++;
                            // int ifReject = 0;
                            //  divide the checkLine into 5 parts according to the space
                            int k = 0;
                            int l = 0;
                            int m = 0;
                            char *parts[5];
                            char part[100];
                            for (k = 0; k <= strlen(checkLine); k++)
                            {
                                if (checkLine[k] == ' ' || k == strlen(checkLine))
                                {
                                    part[l] = '\0';
                                    parts[m] = malloc(strlen(part) + 1);
                                    strcpy(parts[m], part);
                                    m++;
                                    l = 0;
                                }
                                else
                                {
                                    part[l] = checkLine[k];
                                    l++;
                                }
                            }
                            for (k = 0; k < strlen(parts[1]); k++)
                            {
                                orderNum[k] = parts[1][k];
                            }
                            orderNum[k] = '\0';
                            dueYear[0] = parts[2][0];
                            dueYear[1] = parts[2][1];
                            dueYear[2] = parts[2][2];
                            dueYear[3] = parts[2][3];
                            dueYear[4] = '\0';
                            dueMonth[0] = parts[2][5];
                            dueMonth[1] = parts[2][6];
                            dueMonth[2] = '\0';
                            dueDay[0] = parts[2][8];
                            dueDay[1] = parts[2][9];
                            dueDay[2] = '\0';
                            for (k = 0; k < strlen(parts[3]); k++)
                            {
                                quantity = quantity * 10 + (parts[3][k] - '0');
                            }
                            for (k = 0; k < strlen(parts[4]); k++)
                            {
                                productName[k] = parts[4][k];
                            }
                            productName[k] = '\0';
                            orderNums[orderAmount - 1] = malloc(strlen(orderNum) + 1);
                            strcpy(orderNums[orderAmount - 1], orderNum);
                            quantities[orderAmount - 1] = malloc(sizeof(int));
                            *quantities[orderAmount - 1] = quantity;
                            quantityChars[orderAmount - 1] = malloc(strlen(parts[3]) + 1);
                            strcpy(quantityChars[orderAmount - 1], parts[3]);
                            productNames[orderAmount - 1] = malloc(strlen(productName) + 1);
                            strcpy(productNames[orderAmount - 1], productName);
                            dueYears[orderAmount - 1] = malloc(strlen(dueYear) + 1);
                            strcpy(dueYears[orderAmount - 1], dueYear);
                            dueMonths[orderAmount - 1] = malloc(strlen(dueMonth) + 1);
                            strcpy(dueMonths[orderAmount - 1], dueMonth);
                            dueDays[orderAmount - 1] = malloc(strlen(dueDay) + 1);
                            strcpy(dueDays[orderAmount - 1], dueDay);
                            // ask the parent to feed the next line
                            write(c2p_pipe[0][1], "NextLine", strlen("NextLine"));
                        }

                        else
                        {
                            // Algorithm name is not defined!
                            printf("Error: Algorithm name is not defined!");
                        }
                    }
                }
                else if (p == 1 && strcmp(childRead, "OutputBegins") == 0)
                {
                    // output module process

                    FILE *outputFile = fopen("output.txt", "r");
                    char line[100];
                    while (fgets(line, sizeof(line), outputFile) != NULL)
                    {
                        printf("%s", line);
                    }

                    // notify the parent that the output is done
                    fclose(outputFile);
                    write(c2p_pipe[1][1], "OutputDone", strlen("OutputDone"));
                }
            }
            exit(0);
        }
    }

    // Only the parent process can enter the following code area until the end of run()
    char parentRead[100];

    // Open the file to write and read the raw input
    FILE *file = fopen("input.txt", "w+");
    FILE *bonusInput = fopen("BonusInput.txt", "w");
    fclose(bonusInput);

    // open file to record the invalid order (error handling)
    FILE *invalidLog = fopen("invalid.txt", "w");

    if (file == NULL)
    {
        printf("Error: Cannot opening the input log file!\n");
        return;
    }

    // Period of the system can only be set ONCE
    int setPeriod = 0;
    int dateNum = 0;
    char startYear[5];
    char startMonth[3];
    char startDay[3];
    char endYear[5];
    char endMonth[3];
    char endDay[3];
    char startDate[15]; // the whole YYYY-MM-DD format
    char endDate[15];   // the whole YYYY-MM-DD format
                        // write users' input into the input.txt file
                        // end the system until the user inputs "exitPLS"

    while (1)
    {

        printf("Please enter:\n> ");
        fgets(input, sizeof(input), stdin);

        // if exitPLS command received, exit the PLS system
        if (strncmp(input, "exitPLS", 7) == 0)
        {
            break;
        }

        // if not legal commands, ignore and give feedback to the user
        if (!(strncmp(input, "addPERIOD", 9) == 0 || strncmp(input, "addORDER", 8) == 0 || strncmp(input, "addBATCH", 8) == 0 || strncmp(input, "runPLS FCFS", 11) == 0 || strncmp(input, "runPLS SJF", 10) == 0 || strncmp(input, "runPLS CHAI", 11) == 0))
        {
            printf("Error: Illegal Commands! Please double check the format.\n");
            continue;
        }
        else if ((strncmp(input, "addORDER", 8) == 0))
        {
            char orderNumber[15], dueDate[15], quantity[20], product_name[20];
            sscanf(input, "addORDER %s %s %s %s", orderNumber, dueDate, quantity, product_name);



            if ((compareDate(dueDate, startDate) == 1 || compareDate(endDate, dueDate) == 1))
            {
                // due date not falls in the period

                fprintf(invalidLog, "%s", input);
                printf("The order date is invalid, recorded in the invalid.txt\n");
                continue;
            }
            
            int isQuantity = 1;
            int quantityIndex = 0;
            for (quantityIndex = 0; quantityIndex < strlen(quantity); quantityIndex++) {
                if (quantity[quantityIndex] < '0' || quantity[quantityIndex] > '9'){
                    isQuantity = 0;
                    break;
                }
            }
            if (!isQuantity){
                // order quantity is invalid

                fprintf(invalidLog, "%s", input);
                printf("The order quantity is invalid, recorded in the invalid.txt\n");
                continue;

            }

            

            
        }

        // safely write to the input.txt and bonusInput.txt
        fprintf(file, "%s", input);

        bonusInput = fopen("BonusInput.txt", "a");
        fprintf(bonusInput, "%s", input);
        fclose(bonusInput);
        if (strncmp(input, "addPERIOD", 9) == 0)
        {
            if (setPeriod)
            {
                printf("Period of the system can only be set ONCE!");
                continue;
            }
            setPeriod = 1;
            sscanf(input + 10, "%4[^-]-%2[^-]-%s %4[^-]-%2[^-]-%s", startYear, startMonth, startDay, endYear, endMonth, endDay);
            dateNum = calculateDate(startYear, startMonth, startDay, endYear, endMonth, endDay);

            sscanf(input, "addPERIOD %s %s", startDate, endDate);

            char period_info[100];
            sprintf(period_info, "%d %s %s %s %s %s %s", dateNum, startYear, startMonth, startDay, endYear, endMonth, endDay);
            write(p2c_pipe[0][1], period_info, strlen(period_info));
            write(p2c_pipe[1][1], period_info, strlen(period_info));
        }

        else if (strncmp(input, "addBATCH", 8) == 0)
        {
            // (to be compatible with bonus) append batch content to the bonusInput.txt
            // read in a list of addORDER commands
            char batchFileName[50], batchFileLine[100];
            sscanf(input, "addBATCH %s", batchFileName);
            FILE *batchFile = fopen(batchFileName, "r");
            FILE *bonusInput = fopen("BonusInput.txt", "a");
            if (batchFile == NULL)
            {
                printf("Error: Cannot opening the file %s!\n", batchFileName);
                continue;
            }

            while (fgets(batchFileLine, sizeof(batchFileLine), batchFile) != NULL)
            {

                size_t len = strlen(batchFileLine);
                if (len <= 0)
                {
                    printf("Error: Empty line detected in the batch file.\n");
                    continue;
                }

                else if (strncmp(batchFileLine, "addORDER", 8) != 0)
                {
                    printf("Error: ONLY addORDER commands can be included in the batch file.\n");
                    continue;
                }

                else if (batchFileLine[len - 2] == 13)
                {
                    // remove the '\r' character
                    //printf("I catch the character %d", batchFileLine[len - 2]);
                    
                    batchFileLine[len - 2] = '\0';
                }

                else if (batchFileLine[len - 1] == '\n')
                {
                    // remove the '\n' character
                    // printf("I catch the character %d", batchFileLine[len - 1]);
                    
                    batchFileLine[len - 1] = '\0';
                }

                char orderNumber[15], dueDate[15], quantity[20], product_name[20];
                sscanf(batchFileLine, "addORDER %s %s %s %s", orderNumber, dueDate, quantity, product_name);

                if ((compareDate(dueDate, startDate) == 1 || compareDate(endDate, dueDate) == 1))
                {
                    // due date not falls in the period

                    //fprintf(invalidLog, "%s", batchFileLine);
                    printf("The order date %s is invalid, recorded in the invalid.txt\n", dueDate);
                    continue;
                }
                
                int isQuantity = 1;
                int quantityIndex = 0;
                for (quantityIndex = 0; quantityIndex < strlen(quantity); quantityIndex++) {
                    if (quantity[quantityIndex] < '0' || quantity[quantityIndex] > '9'){
                        isQuantity = 0;
                        break;
                    }
                }
                if (!isQuantity){
                    // order quantity is invalid

                    //fprintf(invalidLog, "%s", batchFileLine);
                    printf("The order quantity %s is invalid, recorded in the invalid.txt\n", quantity);
                    continue;

                }
                    fprintf(bonusInput, "%s\n", batchFileLine);
                }

            fclose(bonusInput);
            fclose(batchFile);
        }

        else if (strncmp(input, "runPLS CHAI", 11) == 0)
        {
            char reportFileName[100]; // Assume that each of them is 100 characters long
            sscanf(input + 7, "CHAI | printREPORT > %s", reportFileName);
            printf("The bonus is running\n");
            // close the input.txt file
            system("gcc -o Bonus Bonus.c");
            int status = system("./Bonus");

            // Check the status of the second file
            if (status == -1)
            {
                printf("Unable to run the bonus\n");
            }
            else
            {
                if (WIFEXITED(status))
                {
                    int exit_status = WEXITSTATUS(status);
                    printf("The bonus has exited with status: %d\n", exit_status);
                    // change the bonusFile.txt to the reportFileName using the system command
                    char command[100];
                    sprintf(command, "mv bonusFile.txt %s", reportFileName);
                    // if the reportFileName is empty, do not execute the command
                    if (strlen(reportFileName) > 0)
                    {
                        system(command);
                    }
                }
                else
                {
                    printf("The bonus terminated abnormally\n");
                }
            }
        }

        else if (strncmp(input, "runPLS", 6) == 0)
        {
            // start the scheduling kernel for FCFS and SJF

            char algorithm[100], reportFileName[100]; // Assume that each of them is 100 characters long
            sscanf(input + 7, "%s | printREPORT > %s", algorithm, reportFileName);

            // printf("The algorithm is %s\n", algorithm);
            // write algorithm name to the scheduler child process
            write(p2c_pipe[0][1], algorithm, strlen(algorithm));

            // wait for the child process to confirm the received algorithm
            int n = read(c2p_pipe[0][0], parentRead, 100);
            parentRead[n] = 0;

            // reset the cursor to the beginning of the file

            rewind(file);

            char c;
            char line[100];
            char *lines[100];
            int i = 0;
            int lineIndex = 0;

            // read in the lines of previous raw input
            while (fgets(line, sizeof(line), file) != NULL)
            {
                size_t len = strlen(line);
                if (len > 0 && line[len - 1] == '\n')
                {
                    line[len - 1] = '\0';
                }

                lines[lineIndex] = malloc(strlen(line) + 1);
                strcpy(lines[lineIndex], line);

                lineIndex++;
            }

            int j;
            for (j = 0; j < lineIndex; j++)
            {

                // process the lines of previous raw input
                char *checkLine = lines[j];
                if (strncmp(checkLine, "addPERIOD", 9) == 0 || strncmp(checkLine, "runPLS", 6) == 0)
                {
                    // omit the addPeriod and runPLS lines since the information has been extracted
                    continue;
                }

                else if (strncmp(checkLine, "addORDER", 8) == 0)
                {

                    char feedline[100] = "FeedLine";
                    strcat(feedline, checkLine);
                    write(p2c_pipe[0][1], feedline, strlen(feedline));

                    // printf("I am writing %s with %lu bytes\n", feedline, strlen(feedline));

                    // wait for the scheduler to process this line
                    n = read(c2p_pipe[0][0], parentRead, 100);
                    parentRead[n] = 0;
                    // printf("parent reads %s\n", parentRead);
                }

                else if (strncmp(checkLine, "addBATCH", 8) == 0)
                {

                    // read in a list of addORDER commands
                    char batchFileName[50], batchFileLine[100];
                    sscanf(checkLine, "addBATCH %s", batchFileName);

                    FILE *batchFile = fopen(batchFileName, "r");
                    if (batchFile == NULL)
                    {
                        printf("Error: Cannot opening the file %s!\n", batchFileName);
                        continue;
                    }

                    while (fgets(batchFileLine, sizeof(batchFileLine), batchFile) != NULL)
                    {
                        size_t len = strlen(batchFileLine);
                        if (len <= 0)
                        {
                            printf("Error: Empty line detected in the batch file.\n");
                            continue;
                        }

                        else if (strncmp(batchFileLine, "addORDER", 8) != 0)
                        {
                            printf("Error: ONLY addORDER commands can be included in the batch file.\n");
                            continue;
                        }

                        else if (batchFileLine[len - 2] == 13)
                        {
                            // remove the '\r' character
                            //printf("I catch the character %d", batchFileLine[len - 2]);
                            
                            batchFileLine[len - 2] = '\0';
                        }

                        else if (batchFileLine[len - 1] == '\n')
                        {
                            // remove the '\n' character
                            // printf("I catch the character %d", batchFileLine[len - 1]);
                            
                            batchFileLine[len - 1] = '\0';
                        }
                        
                   

                        char orderNumber[15], dueDate[15], quantity[20], product_name[20];
                        sscanf(batchFileLine, "addORDER %s %s %s %s", orderNumber, dueDate, quantity, product_name);


                        
                        if ((compareDate(dueDate, startDate) == 1 || compareDate(endDate, dueDate) == 1))
                        {
                            // due date not falls in the period

                            fprintf(invalidLog, "%s", batchFileLine);
                            printf("The order date %s is invalid, recorded in the invalid.txt\n", dueDate);
                            continue;
                        }
                        
                        int isQuantity = 1;
                        int quantityIndex = 0;
                        for (quantityIndex = 0; quantityIndex < strlen(quantity); quantityIndex++) {
                            if (quantity[quantityIndex] < '0' || quantity[quantityIndex] > '9'){
                                isQuantity = 0;
                                break;
                            }
                        }
                        if (!isQuantity){
                            // order quantity is invalid

                            fprintf(invalidLog, "%s", batchFileLine);
                            printf("The order quantity %s is invalid, recorded in the invalid.txt\n", quantity);
                            continue;

                        }


                        char feedline[100] = "FeedLine";
                        strcat(feedline, batchFileLine);
                        write(p2c_pipe[0][1], feedline, strlen(feedline));

                        n = read(c2p_pipe[0][0], parentRead, 100);
                        parentRead[n] = 0;
                    }

                    fclose(batchFile);
                }
            }

            char endOfOrder[100] = "EOF | ";
            strcat(endOfOrder, reportFileName);
            write(p2c_pipe[0][1], endOfOrder, strlen(endOfOrder));

            // wait for the scheduler to finish
            n = read(c2p_pipe[0][0], parentRead, 100);
            parentRead[n] = 0;
            // printf("parent reads %s\n", parentRead);
            write(p2c_pipe[1][1], "OutputBegins", strlen("OutputBegins"));

            // wait for the output to finish
            n = read(c2p_pipe[1][0], parentRead, 100);
            parentRead[n] = 0;
            // printf("parent reads %s\n", parentRead);

            write(p2c_pipe[0][1], "WakeupScheduler", strlen("WakeupScheduler"));
        }
    }
    printf("Bye-bye!\n");
    fclose(file);
    fclose(invalidLog);
    // system exits
    for (p = 0; p < num_children; p++)
    {
        write(p2c_pipe[p][1], "SystemExit", strlen("SystemExit"));
    }

    // wait for all child processes to collect them
    for (p = 0; p < num_children; p++)
    {
        int cid = wait(NULL);
        // close all the ends
        close(p2c_pipe[p][1]);
        close(c2p_pipe[p][0]);
        // printf("Parent: Child %d with pid %d collected\n", j, cid);
    }
}

int main()
{
    run();
    return 0;
}
