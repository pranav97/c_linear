#include "matrix.h"
#include <pthread.h> 


// void copy_elems(const MatrixMap &a, MatrixMap &targ, size_t start_ind, size_t end_ind) {
//     for (size_t j = start_ind; j < end_ind; j++) {
//         targ.matrix[start_ind] = a.matrix[start_ind];
//         start_ind ++;
//     }
// }

void print_matrix(const MatrixMap *a) {
    size_t row, col;
    if (a == NULL) {
        printf("fuccccc");
        exit(1);
    }
    for (row = 0; row < a -> num_rows; row++) {
        for (col = 0; col < a -> num_cols; col++) {
            printf("%lld \t", a -> matrix[get_ind(row, col, a -> num_rows)]);
        }
        printf("\n");
    }
}

void take_stdin_matrix(MatrixMap *a) {
    for (size_t cur_row = 0; cur_row < a -> num_rows; cur_row++) {
        for (size_t cur_col = 0; cur_col < a -> num_cols; cur_col++) {
            long long int cur_num;
            scanf("%llu", &cur_num);
            a -> matrix[get_ind(cur_row, cur_col, a -> num_rows)] = cur_num;
        }
    }
}

void multiply_single_thread(struct matrix_pair *mp) {
    MatrixMap *a = mp -> m;
    MatrixMap *b = mp -> m2;

    mp -> targ  = (MatrixMap*) malloc(sizeof(MatrixMap));
    mp -> targ -> num_rows = a -> num_rows;
    mp -> targ -> num_cols = b -> num_cols;
    
    size_t num_elm = mp -> targ -> num_rows  * mp -> targ -> num_cols;
    long long int *matrix = malloc(num_elm * sizeof(long long int)); 
    mp -> targ -> matrix = matrix;
    size_t last_row = mp ->  targ -> num_rows, cur_row, cur_col;
    for (cur_row = 0; cur_row < last_row; cur_row++) {
        for (cur_col = 0; cur_col < mp -> targ -> num_cols; cur_col++) {
            multiply_row_and_col(mp -> m, cur_row, mp -> m2, cur_col, mp -> targ);
        }
    } 
    
}


void multiply_row_and_col(const MatrixMap *a, size_t cur_row, const MatrixMap *b, size_t cur_col, MatrixMap *targ) {
    long long int val = 0;
    size_t cc = get_ind(0, cur_col, b ->num_rows);
    size_t last_ind = get_ind(cur_row, a ->num_cols-1, a ->num_rows); 
    for (size_t cur_ind    =       get_ind(cur_row, 0, a ->num_rows); 
            cur_ind     <=      last_ind;
            cur_ind     +=      a ->num_rows
        ) {
        val += a ->matrix[cur_ind] * b ->matrix[cc];
        cc++;
    }
    targ ->matrix[get_ind(cur_row, cur_col, targ ->num_rows)] = val;
}




void *multiply_helper(void *mh) {
    size_t cur_row, cur_col;
    struct m_help *mult_helper = mh;
    for (cur_row = mult_helper -> start;
            cur_row < mult_helper -> end; 
            cur_row++
        ) {
        for (cur_col = 0;
            cur_col < mult_helper -> mult -> num_cols; 
            cur_col++
        ) {
            multiply_row_and_col(mult_helper -> a, cur_row, mult_helper -> b, cur_col, mult_helper -> mult);
        }
    }
    pthread_exit(NULL);

}

void row_split_multiply(const MatrixMap *a, const MatrixMap *b, MatrixMap *targ) {
    size_t last_row = a ->num_rows, rows_per_core, start = 0, end = 0;
    int numofcpus = sysconf(_SC_NPROCESSORS_ONLN); // Get the number of logical CPUs.
    pthread_t all_threads[MAX_THREAD];
    int running = 0;

    if (numofcpus <= 0) {
        printf("There is only 1 core on this machine");
        numofcpus = 1;
    }
    // else {
    //     printf("There is only %d cores on this machine\n", numofcpus);
    // }
    
    rows_per_core =  last_row / numofcpus;
    if (last_row % numofcpus != 0) {
        rows_per_core += 1;
    }   

    
    for(end = start + rows_per_core; end <= last_row; end = end + rows_per_core) {
        struct m_help help;
        help.a = a;
        help.b = b;
        help.mult = targ;
        help.start = start;
        help.end = end;

        pthread_create(&all_threads[running], NULL, multiply_helper, &help);
        running++;
        multiply_helper(&help); 
        start += rows_per_core;
    }
    if (end != last_row) {
        struct m_help help;
        help.a = a;
        help.b = b;
        help.mult = targ;
        help.start = start;
        help.end = last_row;
        multiply_helper(&help); 
    }
    for (int i = 0; i < running; i++) {
        pthread_join(all_threads[i], NULL); 

    }

}

void multiply(struct matrix_pair *mp) {
    MatrixMap *a = mp -> m;
    MatrixMap *b = mp -> m2;

    mp -> targ  = (MatrixMap*) malloc(sizeof(MatrixMap));
    mp -> targ -> num_rows = a -> num_rows;
    mp -> targ -> num_cols = b -> num_cols;
    
    size_t num_elm = mp -> targ -> num_rows  * mp -> targ -> num_cols;
    long long int *matrix = malloc(num_elm * sizeof(long long int)); 
    mp -> targ -> matrix = matrix;  
    row_split_multiply(mp -> m, mp -> m2, mp -> targ);
}
