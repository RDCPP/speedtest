#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define ITER_TIME 100 // counting fastest type
#define TEST_TIME 100 // testing 10000*10000 times (plz use less than 32767)
#define NS_S 1000000000.0 // ns to s

// struct

typedef struct Benchmark_Type{
    char type_name[10];
    long type_time;
}BT;

// function

long ns_diff(struct timespec start, struct timespec end);
int compare(const void *first, const void *second);

void *test_int(void *);
void *test_short(void *);
void *test_long(void *);

const char *correct_ordinal(int);

void print_result(BT[3]);
void avg_result(BT[3]);

int main(void) {
    pthread_t threads[3];

    BT avg_bench[3] = {{"int",   0},
                       {"short", 0},
                       {"long",  0}};

    for (int i = 1; i <= ITER_TIME; i++) {

        const char *co = correct_ordinal(i);

        printf("----------%d%s test start----------\n",i,co);

        BT bench[3] = {{"int",   0},
                       {"short", 0},
                       {"long",  0}};

        pthread_create(&threads[0],NULL,test_int,&bench[0]);
        pthread_create(&threads[1],NULL,test_short,&bench[1]);
        pthread_create(&threads[2],NULL,test_long,&bench[2]);

        for (int j = 0; j < 3; j++) {
            pthread_join(threads[j],NULL);
            printf("%s test complete\n",bench[j].type_name);
        }

        avg_bench[0].type_time += bench[0].type_time;
        avg_bench[1].type_time += bench[1].type_time;
        avg_bench[2].type_time += bench[2].type_time;

        qsort(bench, 3, sizeof(BT), compare);

        print_result(bench);
    }

    printf("----------all test finished----------\n");

    avg_result(avg_bench);

    return 0;
}

long ns_diff(struct timespec start, struct timespec end){
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

void *test_int(void *param) {
    BT *bench = (BT *)param;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < TEST_TIME; i++) {
        for (int j = 0; j < 10000; j++) {
            for (int k = 0; k < 10000; k++) {
                int l = i + j + k;
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    bench->type_time = ns_diff(start, end);
    pthread_exit(param);
}

void *test_short(void *param) {
    BT *bench = (BT *)param;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (short i = 0; i < TEST_TIME; i++) {
        for (short j = 0; j < 10000; j++) {
            for (short k = 0; k < 10000; k++) {
                short l = i + j + k;
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    bench->type_time = ns_diff(start, end);
    pthread_exit(param);
}

void *test_long(void *param) {
    BT *bench = (BT *)param;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (long i = 0; i < TEST_TIME; i++) {
        for (long j = 0; j < 10000; j++) {
            for (long k = 0; k < 10000; k++) {
                long l = i + j + k;
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    bench->type_time = ns_diff(start, end);
    pthread_exit(param);
}

const char *correct_ordinal(int i){
    if (i%10 == 1 && i%100 != 11) return "st";
    else if (i%10 == 2 && i%100 != 12) return "nd";
    else if (i%10 == 3 && i%100 != 13) return "rd";
    else return "th";
}

void print_result(BT bench[3]){
    printf("\n<Result>\n");

    printf("1st : %s => %ld ns. (%.3lf s.)\n",bench[0].type_name,bench[0].type_time,bench[0].type_time/NS_S);
    printf("2nd : %s => %ld ns. (%.3lf s.)\n",bench[1].type_name,bench[1].type_time,bench[1].type_time/NS_S);
    printf("3rd : %s => %ld ns. (%.3lf s.)\n",bench[2].type_name,bench[2].type_time,bench[2].type_time/NS_S);
}

void avg_result(BT avg_bench[3]){
    avg_bench[0].type_time /= ITER_TIME;
    avg_bench[1].type_time /= ITER_TIME;
    avg_bench[2].type_time /= ITER_TIME;

    qsort(avg_bench, 3, sizeof(BT), compare);

    printf("\n[Average Result]\n");

    printf("1st : %s => %ld ns. (%.3lf s.)\n",avg_bench[0].type_name,avg_bench[0].type_time,avg_bench[0].type_time/NS_S);
    printf("2nd : %s => %ld ns. (%.3lf s.)\n",avg_bench[1].type_name,avg_bench[1].type_time,avg_bench[1].type_time/NS_S);
    printf("3rd : %s => %ld ns. (%.3lf s.)\n",avg_bench[2].type_name,avg_bench[2].type_time,avg_bench[2].type_time/NS_S);
}