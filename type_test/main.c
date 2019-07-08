#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TEST_TIME 100 // testing 10000*10000 times (plz use less than 32767)

long ns_diff(struct timespec start, struct timespec end);
int compare(const void *first, const void *second);

typedef struct Benchmark_Type{
    char type_name[10];
    long type_time;
}BT;

int main(void) {
    BT bench[3] = {{"int" , 0},
                  {"short", 0},
                  {"long",  0}};
    struct timespec int_start,int_end;
    struct timespec short_start,short_end;
    struct timespec long_start,long_end;
    clock_gettime(CLOCK_MONOTONIC,&int_start);
    for(int i = 0;i<TEST_TIME;i++){
        for(int j = 0;j<10000;j++){
            for(int k = 0;k<10000;k++){
                int l = i + j + k;
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC,&int_end);
    bench[0].type_time = ns_diff(int_start,int_end);
    clock_gettime(CLOCK_MONOTONIC,&short_start);
    for(short s = 0;s<TEST_TIME;s++){
        for(short t = 0;t<10000;t++){
            for(short u = 0;u<10000;u++){
                short v = s + t + u;
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC,&short_end);
    bench[1].type_time = ns_diff(short_start,short_end);
    clock_gettime(CLOCK_MONOTONIC,&long_start);
    for(long o = 0;o<TEST_TIME;o++){
        for(long p = 0;p<10000;p++){
            for(long q = 0;q<10000;q++){
                long r = o + p + q;
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC,&long_end);
    bench[2].type_time = ns_diff(long_start,long_end);
    qsort(bench,3, sizeof(BT),compare);
    printf("1st : %s (%ld ns.)\n",bench[0].type_name,bench[0].type_time);
    printf("2nd : %s (%ld ns.)\n",bench[1].type_name,bench[1].type_time);
    printf("3rd : %s (%ld ns.)\n",bench[2].type_name,bench[2].type_time);
    return 0;
}

long ns_diff(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp.tv_sec * 1000000000 + temp.tv_nsec;
}

int compare(const void *first, const void *second){
    BT *p1 = (BT *)first;
    BT *p2 = (BT *)second;

    if(p1->type_time > p2->type_time) return 1;
    else if (p1->type_time < p2->type_time) return -1;
    else return 0;
}