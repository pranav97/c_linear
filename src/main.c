

#include "matrix.h"
#include <time.h>

// call this function to start a nanosecond-resolution timer
struct timespec timer_start(){
    struct timespec start_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    return start_time;
}

// call this function to end a timer, returning nanoseconds elapsed as a long
long timer_end(struct timespec start_time){
    struct timespec end_time;
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    long diffInNanos = (end_time.tv_sec - start_time.tv_sec) * (long)1e9 + (end_time.tv_nsec - start_time.tv_nsec);
    return (diffInNanos/1000);
}

#define TRANSPOSE 1
#define MULT 2

void print_help_message() {
    printf("-s     --single-thread (Optional) Tells the program to use no more than 1 thread.\n");
    printf("-h     --help          Print this message over here.\n");
}

bool parse_command_args(int argc, char * argv[]) {
    bool single_thread = false;

    for(int i = 0; i < argc; i++) {
        if((strcmp(argv[i], "--single-thread") == 0) || (strcmp(argv[i],"-s") == 0)) {
            single_thread = true;
        }
        if ((strcmp(argv[i], "--help") == 0) || (strcmp(argv[i],"-h") == 0)) {
            print_help_message();
            exit(1);
        }
    }
    return single_thread;
}

MatrixMap *take_input() { 
    size_t num_rows, num_cols;
    printf("Input the number of rows (m): \n");
    scanf("%lu", &num_rows);
    printf("Input the number of cols (m): \n");
    scanf("%lu", &num_cols);
    size_t num_elm = (num_cols * num_rows);
    printf("Enter %lu numbers: \n", num_rows * num_cols);
    MatrixMap *m = (MatrixMap*) malloc(sizeof(MatrixMap));
    m -> num_rows = num_rows;
    m -> num_cols = num_cols;
    long long int *matrix = malloc(num_elm * sizeof(long long int)); 
    m -> matrix = matrix;
    take_stdin_matrix(m);
    printf("You just entered: \n");
	print_matrix(m);
    return m;
}

int main(int argc, char *argv[]) {
    bool single_thread = false;
    single_thread = parse_command_args(argc, argv);
    MatrixMap *m = take_input();
    MatrixMap *m2 = take_input();
    if (m -> num_cols != m2 -> num_rows) {
        printf("Matrix multiplication not possible\n");
    }
    else {
        struct timespec s = timer_start();
        struct matrix_pair mp;
        mp.m = m;
        mp.m2 = m2;
        if (single_thread) {
            multiply_single_thread(&mp);
        }
        else {
            multiply(&mp);
        }
        MatrixMap *mup = mp.targ;
        long micros = timer_end(s);
        printf("Time taken by multiplication: %ld microseconds\n", micros);
        printf("Product is: \n");
        print_matrix(mup);
        free(mup -> matrix);
        free(mup);
    }
    free(m -> matrix);
    free(m);
    free(m2 -> matrix);
    free(m2);
    
    return 0;
}
