#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(void){
    printf("Cas prijezdu vlaku A:\n");
    int h1 = 0, m1 = 0;
    if (scanf("%d:%d", &h1, &m1) != 2 || h1 < 0 || h1 > 23 || m1 < 0 || m1 > 59) {
        printf("Nespravny vstup.\n");
        return 1;
    }
    if(h1 == 0 || h1 == 1) {
        h1 += 24;
    }
    int ArrivalA = h1 * 60 + m1;
    printf("Cas odjezdu vlaku A:\n");
    int h2 = 0, m2 = 0;
    if (scanf("%d:%d", &h2, &m2) != 2 || h2 < 0 || h2 > 23 || m2 < 0 || m2 > 59) {
        printf("Nespravny vstup.\n");
        return 1;
        }
        if(h2 == 0 || h2 == 1) {
            h2 += 24;
    }
    int DepartureA = h2 * 60 + m2;
    printf("Cas prijezdu vlaku B:\n");
    int h3 = 0, m3 = 0;
    if (scanf("%d:%d", &h3, &m3) != 2 || h3 < 0 || h3 > 23 || m3 < 0 || m3 > 59) {
        printf("Nespravny vstup.\n");
        return 1;
        }
    if(h3 == 0 || h3 == 1) {
        h3 += 24;
    }
    int ArrivalB = h3 * 60 + m3;
    printf("Cas odjezdu vlaku B:\n");
    int h4 = 0, m4 = 0;
    if (scanf("%d:%d", &h4, &m4) != 2 || h4 < 0 || h4 > 23 || m4 < 0 || m4 > 59) {
        printf("Nespravny vstup.\n");
        return 1;
    }
    if(h4 == 0 || h4 == 1) {
        h4 += 24;
    }
    int DepartureB = h4 * 60 + m4;
    printf("Cas prijezdu vlaku C:\n");
    int h5 = 0, m5 = 0;
    if (scanf("%d:%d", &h5, &m5) != 2 || h5 < 0 || h5 > 23 || m5 < 0 || m5 > 59) {
        printf("Nespravny vstup.\n");
        return 1;
    }
    if(h5 == 0 || h5 == 1) {
        h5 += 24;
    }
    int ArrivalC = h5 * 60 + m5;
    printf("Cas odjezdu vlaku C:\n");
    int h6 = 0, m6 = 0;
    if (scanf("%d:%d", &h6, &m6) != 2 || h6 < 0 || h6 > 23 || m6 < 0 || m6 > 59) {
        printf("Nespravny vstup.\n");
        return 1;
    }
    if(h6 == 0 || h6 == 1) {
        h6 += 24;
    }
    int DepartureC = h6 * 60 + m6;
    if((DepartureB-ArrivalA >= 5 && ArrivalB-ArrivalA <= 180) && (DepartureC-ArrivalA >= 5 && ArrivalC-ArrivalA <= 180)){
        printf("Z vlaku A lze prestoupit na vlaky B a C.\n");
    }else if(DepartureB-ArrivalA >= 5 && ArrivalB-ArrivalA <= 180)
        printf("Z vlaku A lze prestoupit na vlak B.\n");
    else if(DepartureC-ArrivalA >= 5 && ArrivalC-ArrivalB <= 180)
        printf("Z vlaku A lze prestoupit na vlak C.\n");
    else{
        printf("Z vlaku A nelze prestupovat.\n");
    }
    if((DepartureA-ArrivalB >= 5 && ArrivalA-ArrivalB <= 180) && (DepartureC-ArrivalB >= 5 && ArrivalC-ArrivalB <= 180)) {
        printf("Z vlaku B lze prestoupit na vlaky A a C.\n");
    }else if(DepartureA-ArrivalB >= 5 && ArrivalA-ArrivalB <= 180)
        printf("Z vlaku B lze prestoupit na vlak A.\n");
    else if(DepartureC-ArrivalB >= 5 && ArrivalC-ArrivalB <= 180)
        printf("Z vlaku B lze prestoupit na vlak C.\n");
    else{
        printf("Z vlaku B nelze prestupovat.\n");
    }
    if((DepartureA-ArrivalC >= 5 && ArrivalA-ArrivalC <= 180) && (DepartureB-ArrivalC >= 5 && ArrivalB-ArrivalC <= 180)) {
        printf("Z vlaku C lze prestoupit na vlaky A a B.\n");
    }else if(DepartureA-ArrivalC >= 5 && ArrivalA-ArrivalC <= 180)
        printf("Z vlaku C lze prestoupit na vlak A.\n");
    else if(DepartureB-ArrivalC >= 5 && ArrivalB-ArrivalC <= 180)
        printf("Z vlaku C lze prestoupit na vlak B.\n");
    else{
		printf("Z vlaku C nelze prestupovat.\n");
    }
	return 0;
}
