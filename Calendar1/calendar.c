#ifndef __PROGTEST__
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
typedef struct

{
    int m_TotalDays;
    int m_WorkDays;
} TResult;
#endif /* __PROGTEST__ */

bool isHoliday( int m, int d )
{
    if((d == 1 && m == 1) || (d == 1 && m == 5) || (d == 8  && m == 5) || (d == 5 && m == 7) || (d == 6 && m == 7) || (d == 28 && m == 9) || (d == 28 && m == 10) || (d == 17 && m == 11) || (d == 24 && m == 12) || (d == 25 && m == 12) || (d == 26 && m == 12))
        return 1;
    return 0; // Workday
}

// vysokosniy god ili net
bool leapornot(int y) {
    if (y % 400 == 0) {
        return true;
    } else if (y % 100 == 0)
    {
        return false;
    } else if (y % 4 == 0) {
        return true;
    } else {
        return false;
    }
}

bool isWorkDay ( int y, int m, int d )
{
    int daysInMonth[13];
    daysInMonth[0] = 0;
    daysInMonth[1] = 31;
    if (leapornot(y)) {
        daysInMonth[2] = 29;
    } else {
        daysInMonth[2] = 28;
    }
    daysInMonth[3] = 31;
    daysInMonth[4] = 30;
    daysInMonth[5] = 31;
    daysInMonth[6] = 30;
    daysInMonth[7] = 31;
    daysInMonth[8] = 31;
    daysInMonth[9] = 30;
    daysInMonth[10] = 31;
    daysInMonth[11] = 30;
    daysInMonth[12] = 31;

    if (y<2000 || m<1 || m>12 || d<1 || d>daysInMonth[m])
        return false;


    if (isHoliday(m, d))
        return false;

    int m_code[] = {0, 13, 14, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}; // хуйня что возвращает день недели от 1 до 7

    int month_code = m_code[m];
    if ( month_code == 13 || month_code == 14)
        y -= 1;
    int a = (d+((13*(month_code+1))/5) + y + (y/4) - (y/100) + (y/400)) % 7;

    if (a > 1) {
        return true;
    }

    return false;
}

TResult countDays ( int y1, int m1, int d1,
                    int y2, int m2, int d2 )
{
    int daysIn1Month[13];
    daysIn1Month[0] = 0;
    daysIn1Month[1] = 31;
    if (leapornot(y1)) {
        daysIn1Month[2] = 29;
    } else {
        daysIn1Month[2] = 28;
    }
    daysIn1Month[3] = 31;
    daysIn1Month[4] = 30;
    daysIn1Month[5] = 31;
    daysIn1Month[6] = 30;
    daysIn1Month[7] = 31;
    daysIn1Month[8] = 31;
    daysIn1Month[9] = 30;
    daysIn1Month[10] = 31;
    daysIn1Month[11] = 30;
    daysIn1Month[12] = 31;

    int daysIn2Month[13];
    daysIn2Month[0] = 0;
    daysIn2Month[1] = 31;
    if (leapornot(y2)) {
        daysIn2Month[2] = 29;
    } else {
        daysIn2Month[2] = 28;
    }
    daysIn2Month[3] = 31;
    daysIn2Month[4] = 30;
    daysIn2Month[5] = 31;
    daysIn2Month[6] = 30;
    daysIn2Month[7] = 31;
    daysIn2Month[8] = 31;
    daysIn2Month[9] = 30;
    daysIn2Month[10] = 31;
    daysIn2Month[11] = 30;
    daysIn2Month[12] = 31;

    if (d1<1 || d1>daysIn1Month[m1] || d2<1 || d2>daysIn2Month[m2] || m1<1 || m2<1 || m1>12 || m2>12 || y1<2000 || y2<2000 || y1>y2 || (y1==y2 && m1>m2) || (y1==y2 && m1==m2 && d1>d2)){
        TResult errorResult;
        errorResult.m_TotalDays = -1;
        errorResult.m_WorkDays = -1;
        return errorResult;
    }


    int totalDays = 0;
    int workDays = 0;
    for (int y = y1; y <= y2; ++y)
    {

        int monthbegin;
        if (y == y1) {
            monthbegin = m1;
        } else {
            monthbegin = 1;
        }
        int monthfinish;
        if (y == y2) {
            monthfinish = m2;
        } else {
            monthfinish = 12;
        }

        int daysInMonth[13];
        daysInMonth[0] = 0;
        daysInMonth[1] = 31;
        if (leapornot(y)) {
            daysInMonth[2] = 29;
        } else {
            daysInMonth[2] = 28;
        }
        daysInMonth[3] = 31;
        daysInMonth[4] = 30;
        daysInMonth[5] = 31;
        daysInMonth[6] = 30;
        daysInMonth[7] = 31;
        daysInMonth[8] = 31;
        daysInMonth[9] = 30;
        daysInMonth[10] = 31;
        daysInMonth[11] = 30;
        daysInMonth[12] = 31;

        for (int m = monthbegin; m <= monthfinish; ++m) {
            int daybegin = (y == y1 && m == m1) ? d1 : 1; // Start day: d1 if first month of first year, else 1
            int dayfinish = (y == y2 && m == m2) ? d2 : daysInMonth[m]; // End day: d2 if last month of last year, else last day of current month

            for (int d = daybegin; d <= dayfinish; ++d)
            {
                totalDays++;
                if (isWorkDay(y, m, d))
                {
                    workDays++;
                }
            }
        }

    }
    TResult result;
    result.m_TotalDays = totalDays;
    result.m_WorkDays = workDays;
    printf("TotalDays: %d\n\n", result.m_TotalDays); // это потом надо будет убрать, просто для того чтобы сравнить
    printf("WorkDays: %d\n\n", result.m_WorkDays); // результаты с ассертами
    return result;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
    TResult r;


    assert ( isWorkDay ( 2023, 10, 9 ) );

    assert ( ! isWorkDay ( 2023, 11, 11 ) );

    assert ( ! isWorkDay ( 2023, 11, 17 ) );

    assert ( ! isWorkDay ( 2023, 11, 31 ) );

    assert ( ! isWorkDay ( 2023,  2, 29 ) );

    assert ( ! isWorkDay ( 2004,  2, 29 ) );

    assert ( isWorkDay ( 2008,  2, 29 ) );

    assert ( ! isWorkDay ( 2001,  2, 29 ) );

    assert ( ! isWorkDay ( 1996,  1,  2 ) );

    r = countDays ( 2023, 11,  1,
                    2023, 11, 30 );
    assert ( r . m_TotalDays == 30 );
    assert ( r . m_WorkDays == 21 );

    r = countDays ( 2023, 11,  1,
                    2023, 11, 17 );
    assert ( r . m_TotalDays == 17 );
    assert ( r . m_WorkDays == 12 );

    r = countDays ( 2023, 11,  1,
                    2023, 11,  1 );
    assert ( r . m_TotalDays == 1 );
    assert ( r . m_WorkDays == 1 );

    r = countDays ( 2023, 11, 17,
                    2023, 11, 17 );
    assert ( r . m_TotalDays == 1 );
    assert ( r . m_WorkDays == 0 );

    r = countDays ( 2023,  1,  1,
                    2023, 12, 31 );
    assert ( r . m_TotalDays == 365 );
    assert ( r . m_WorkDays == 252 );

    r = countDays ( 2024,  1,  1,
                    2024, 12, 31 );
    assert ( r . m_TotalDays == 366 );
    assert ( r . m_WorkDays == 254 );

    r = countDays ( 2000,  1,  1,
                    2023, 12, 31 );
    assert ( r . m_TotalDays == 8766 );
    assert ( r . m_WorkDays == 6072 );

    r = countDays ( 2001,  2,  3,
                    2023,  7, 18 );
    assert ( r . m_TotalDays == 8201 );
    assert ( r . m_WorkDays == 5682 );

    r = countDays ( 2021,  3, 31,
                    2023, 11, 12 );
    assert ( r . m_TotalDays == 957 );
    assert ( r . m_WorkDays == 666 );

    r = countDays ( 2001,  1,  1,
                    2000,  1,  1 );
    assert ( r . m_TotalDays == -1 );
    assert ( r . m_WorkDays == -1 );

    r = countDays ( 2001,  1,  1,
                    2023,  2, 29 );
    assert ( r . m_TotalDays == -1 );
    assert ( r . m_WorkDays == -1 );

    r = countDays (4004, 2, 29, 4008, 2, 29);
    printf("%d\n", r.m_TotalDays);
    assert(r.m_TotalDays == 1462);
    assert(r.m_WorkDays == 1015);

    r = countDays (4800, 2, 29, 4808, 2, 29);

    assert(r.m_TotalDays == 2923);
    assert(r.m_WorkDays == 2025);

    r = countDays (7200, 2, 29, 7216, 2, 29);

    assert(r.m_TotalDays == 5845);
    assert(r.m_WorkDays == 4048);

    r = countDays (8000, 2, 29, 8032, 2, 29);

    assert(r.m_TotalDays == 11689);
    assert(r.m_WorkDays == 8095);

    r = countDays (11200, 2, 29, 11264, 2, 29);

    assert(r.m_TotalDays == 23377);
    assert(r.m_WorkDays == 16195);

    r = countDays (14400, 2, 29, 14528, 2, 29);

    assert(r.m_TotalDays == 46752);
    assert(r.m_WorkDays == 32387);

    r = countDays (15200, 2, 29, 15204, 2, 29);

    assert(r.m_TotalDays == 1462);
    assert(r.m_WorkDays == 1010);

    r = countDays (16000, 2, 29, 17008, 2, 29);

    assert(r.m_TotalDays == 368165);
    assert(r.m_WorkDays == 255052);

    // начало второй части проверок (заканчиваются на високосные года)

    r = countDays(4207, 7, 14, 7004, 2, 29);

    assert ( r . m_TotalDays == 1021449 );
    assert ( r . m_WorkDays == 707627 );

    r = countDays(5555, 1, 1, 8888, 2, 29);

    assert ( r . m_TotalDays == 1217414 );
    assert ( r . m_WorkDays == 843381 );

    r = countDays(6321, 12, 25, 12000, 2, 29);

    assert ( r . m_TotalDays == 2073914 );
    assert ( r . m_WorkDays == 1436736 );

    r = countDays(7809, 4, 9, 14004, 2, 29);

    assert ( r . m_TotalDays == 2262639 );
    assert ( r . m_WorkDays == 1567473 );

    // начало третьей части проверок (начинаются на високосные года)

    r = countDays(4208, 2, 29, 7010, 7, 14);
    assert ( r . m_TotalDays == 1023546 );
    assert ( r . m_WorkDays == 709078 );

    r = countDays(5604, 2, 29, 8902, 12, 3);
    assert ( r . m_TotalDays == 1204848 );
    assert ( r . m_WorkDays == 834682 );

    r = countDays(6400, 2, 29, 12110, 5, 23);
    assert ( r . m_TotalDays == 2085619 );
    assert ( r . m_WorkDays == 1444848 );

    r = countDays(7804, 2, 29, 14022, 9, 11);
    assert ( r . m_TotalDays == 2271274 );
    assert ( r . m_WorkDays == 1573459 );

    // начало четвертой части проверок (високосные года)

    r = countDays(2404, 2, 29, 8404, 2, 29);

    assert ( r . m_TotalDays == 2191456 );
    assert ( r . m_WorkDays == 1518165 );

    r = countDays(2800, 2, 29, 9800, 2, 29);

    assert ( r . m_TotalDays == -1 );
    assert ( r . m_WorkDays == -1 );

    r = countDays(3208, 2, 29, 9208, 2, 29);

    assert ( r . m_TotalDays == 2191456 );
    assert ( r . m_WorkDays == 1518166 );

    r = countDays(3604, 2, 29, 8604, 2, 29);

    assert ( r . m_TotalDays == 1826213 );
    assert ( r . m_WorkDays == 1265143 );

    r = countDays(4000, 2, 29, 9000, 2, 29);

    assert ( r . m_TotalDays == -1 );
    assert ( r . m_WorkDays == -1 );

    r = countDays(4404, 2, 29, 7404, 2, 29);

    assert ( r . m_TotalDays == 1095728 );
    assert ( r . m_WorkDays == 759088 );

    r = countDays(4808, 2, 29, 8808, 2, 29);

    assert ( r . m_TotalDays == 1460971 );
    assert ( r . m_WorkDays == 1012111 );

    r = countDays(5200, 2, 29, 8200, 2, 29);

    assert ( r . m_TotalDays == -1 );
    assert ( r . m_WorkDays == -1 );

    r = countDays(5604, 2, 29, 7604, 2, 29);

    assert ( r . m_TotalDays == 730486 );
    assert ( r . m_WorkDays == 506055 );

    r = countDays(6008, 2, 29, 8008, 2, 29);

    assert ( r . m_TotalDays == 730486 );
    assert ( r . m_WorkDays == 506056 );

    // начало пятой части проверок (рандомные даты)

    r = countDays(5123, 3, 15, 16150, 8, 12);

    assert ( r . m_TotalDays == 4027681 );
    assert ( r . m_WorkDays == 2790241 );

    r = countDays(8000, 11, 20, 18034, 5, 25);

    assert ( r . m_TotalDays == 3664665 );
    assert ( r . m_WorkDays == 2538757 );

    r = countDays(7012, 7, 9, 17250, 2, 1);

    assert ( r . m_TotalDays == 3739196 );
    assert ( r . m_WorkDays == 2590386 );

    r = countDays(6520, 4, 4, 17634, 12, 30);

    assert ( r . m_TotalDays == 4059576 );
    assert ( r . m_WorkDays == 2812333 );

    r = countDays(6098, 10, 10, 17123, 3, 15);

    assert ( r . m_TotalDays == 4026589 );
    assert ( r . m_WorkDays == 2789480 );

    // начало шестой части проверок (одинаковые даты)

    r = countDays ( 8000, 2,  29,
                    8000, 2, 29 );

    assert(r.m_TotalDays == 1);
    assert(r.m_WorkDays == 1);

    r = countDays ( 6000, 2,  29,
                    6000, 2, 29 );

    assert(r.m_TotalDays == 1);
    assert(r.m_WorkDays == 1);

    r = countDays ( 4200, 2,  29,
                    4200, 2, 29 );

    assert(r.m_TotalDays == -1);
    assert(r.m_WorkDays == -1);

    r = countDays ( 40000, 2,  29,
                    40000, 2, 29 );

    assert(r.m_TotalDays == 1);
    assert(r.m_WorkDays == 1);

    r = countDays ( 4000, 2,  29,
                    4000, 2, 29 );

    assert(r.m_TotalDays == 1);
    assert(r.m_WorkDays == 1);

    r = countDays ( 4004, 2,  29,
                    4004, 2, 29 );

    assert(r.m_TotalDays == 1);
    assert(r.m_WorkDays == 0);

    r = countDays ( 2000, 3,  2,
                    2000, 3, 2 );

    assert(r.m_TotalDays == 1);
    assert(r.m_WorkDays == 1);

    r = countDays ( 2873, 4, 14,
                    2873, 4, 14 );


    assert(r.m_TotalDays == 1);
    assert(r.m_WorkDays == 1);

    r = countDays ( 5632, 7, 29,
                    5632, 7, 29 );

    assert(r.m_TotalDays == 1);
    assert(r.m_WorkDays == 1);


    r = countDays ( 8120, 2, 6,
                    8120, 2, 6 );

    assert(r.m_TotalDays == 1);
    assert(r.m_WorkDays == 1);

    r = countDays ( 4778, 11, 11,
                    4778, 11, 11 );

    assert(r.m_TotalDays == 1);
    assert(r.m_WorkDays == 0);


    r = countDays ( 9354, 3, 23,
                    9354, 3, 23 );

    assert(r.m_TotalDays == 1);
    assert(r.m_WorkDays == 0);
    return EXIT_SUCCESS;
}
#endif
