#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#define get_ind(r, c, num_rows) ((num_rows * c) + r)

#define UNSIGED_ONE 1U

#define MAX_THREAD 100

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



// static void copy_elems(const MatrixMap &a, MatrixMap &targ, size_t start_ind, size_t end_ind);        

void multiply_row_and_col(const MatrixMap *a, size_t cur_row, const MatrixMap *b, size_t cur_col, MatrixMap *targ);
// static void multiply_helper(const MatrixMap &a, const MatrixMap &b, MatrixMap &mult, size_t start, size_t end);
void multiply_single_thread(struct matrix_pair *mp);        
// static void row_split_multiply(const MatrixMap &a, const MatrixMap &b, MatrixMap &targ);

void print_matrix(const MatrixMap *a);
void take_stdin_matrix(MatrixMap *a);
// static void multiply(const MatrixMap &, const MatrixMap &, MatrixMap *);
void multiply(struct matrix_pair *mp);
