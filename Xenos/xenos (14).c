#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

 int gcd( int a,  int b) {

    long int gcd_a, gcd_b;
    if (a > b) {
        gcd_a = a;
        gcd_b = b;
    }

    else {
        gcd_a = b;
        gcd_b = a;
    }

    while(gcd_b > 0) {
        long int div = gcd_a / gcd_b;
        long int tmp = gcd_b;

        gcd_b = gcd_a - (div * gcd_b);
        gcd_a = tmp;
    }
    return gcd_a;
}

 int lcm( int a,  int b) {
    return (a*b) / gcd(a, b);
}

long int read_info( int* contact,  int* final) //функция
{
    *contact = 0;
    *final = 0;
    char info;

    for (;;) // начало цыкла
    {
        if(scanf("%c", &info) != 1)
            return -1;                                 // поиск ебучей палочки
        if (info == '|' ) {
            break;
        }
        if (info < 'a' || info > 'z')//ыые алфавит
        {
            return -1; // возврат чтобы не закончить программу раньше времени
        }
        long int reckon = 1; // геометрическая прогрессия
        for (char x = 'a'; x < info; x++) {
            reckon *= 2;
        }
        //printf("%ld\n",reckon);
        *contact += reckon;
    }

    for (;;) // начало цыкла
    {          
        if(scanf("%c", &info) != 1)
            return -1;                               // поиск ебучей палочки
        if (info == '\n')
            break;

        if (info < 'a' || info > 'z')//ыые алфавит
        {
            return -1; // возврат чтобы не закончить программу раньше времени
        }
        long int reckon = 1; // геометрическая прогрессия
        for (char x = 'a'; x < info; x++) {
            reckon *= 2;
        }
        *final += reckon;
    }

    return 0;
}


int main(void) {
    printf("Zpravy:\n");
    int contact1, final1;//урна ошибок
    if (read_info(&contact1, &final1) != 0) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    int contact2, final2;//урна ошибок
    if (read_info(&contact2, &final2) != 0) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    int fullmessage1 = contact1 + final1;
    int fullmessage2 = contact2 + final2;
    if(fullmessage1 == 0 || fullmessage2 == 0)
    {
        printf("Nespravny vstup.\n");
        return 1;
    }    
    int sinhronization1 = fullmessage1 + contact1;
    int sinhronization2 = fullmessage2 + contact2;
    if((contact1 == contact2) && (final1 == 0 && final2 == 0))
    {
        printf("Synchronizace za: %d\n", final1);
        return 0;
    }
    if ((contact1 == 0 && contact2 == 0) || (final1 == 0 && final2 == 0) ||
        (contact1 == 0 && final2 == 0) || (final1 == 0 && contact2 == 0) ||
         contact1 == contact2)
    {
        sinhronization1 = 0;
        if(contact1 == contact2)
             sinhronization1 = contact1;
        printf("Synchronizace za: %d\n", sinhronization1);
        return 0;
    }
    if (((sinhronization1 % 2 == 0 && fullmessage1 % 2 == 0 && sinhronization2 % 2 == 1 && fullmessage2 % 2 == 0) ||
       (sinhronization1 % 2 == 1 && fullmessage1 % 2 == 0 && sinhronization2 % 2 == 0 && fullmessage2 % 2 == 0) )){
        printf("Nelze dosahnout.\n");
        return 1;
    }
    if (sinhronization1 == sinhronization2 )
    {
        printf("Synchronizace za: %d\n", sinhronization1);
        return 0;
    }
    if(contact1 == sinhronization2)
    {
        printf("Synchronizace za: %d\n", contact1);
        return 0;
    }
    if(contact2 == sinhronization1)
    {
        printf("Synchronizace za: %d\n", contact2);
        return 0;
    }
    if(sinhronization1 < contact2)
    {
        int test = sinhronization1;
        while(test < contact2)
            test += fullmessage1;
        if(test == contact2)
        {
            printf("Synchronizace za: %d\n", contact2);
            return 0;
        }
    }
    if(sinhronization2 < contact1)
    {
        int test = sinhronization2;
        while(test < contact1)
            test += fullmessage2;
        if(test == contact1)
        {
            printf("Synchronizace za: %d\n", contact1);
            return 0;
        }
    }
     while(1)
    {
        if(sinhronization1 < 0 || sinhronization2 < 0)
        {
            printf("Nelze dosahnout.\n");
            return 1;
        }
        if (sinhronization1 == sinhronization2)
            break;
        if(sinhronization1 < sinhronization2)
            sinhronization1 += fullmessage1;
        else
            sinhronization2 += fullmessage2;
    }
    
    printf("Synchronizace za: %d\n", sinhronization1);
    return 0;
}