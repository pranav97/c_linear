#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#define get_ind(r, c, num_rows) ((num_rows * c) + r)

#define UNSIGED_ONE 1U

#define MAX_THREAD 40

static int cpu_threads = 0;

typedef struct matrix {
    size_t num_rows;
    size_t num_cols;
    long long int *matrix;
} MatrixMap;

struct matrix_pair {
    MatrixMap *m;
    MatrixMap *m2;
    MatrixMap *targ;
};

struct row_col_dot {
    struct matrix_pair *mp;
    size_t row;
    size_t col;
};

struct m_help {
    const MatrixMap *a;
    const MatrixMap *b;
    MatrixMap *mult;
    size_t start;
    size_t end;
};

void multiply_single_thread(struct matrix_pair *mp);        
void multiply_row_and_col(const MatrixMap *a, size_t cur_row, const MatrixMap *b, size_t cur_col, MatrixMap *targ);
void print_matrix(const MatrixMap *a);
void take_stdin_matrix(MatrixMap *a);
void multiply(struct matrix_pair *mp);

void multiply(struct matrix_pair *mp);
void multiply_helper(void *mh);
void row_split_multiply(const MatrixMap *a, const MatrixMap *b, MatrixMap *targ);