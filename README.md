# Data types used 
The linear algebra system uses a *single* dimensional array of "long long ints" which are an 8-byte version of int that can hold double the highest int data type can. "Column major" is a better way to store the elements since transpose goes through the columns of the matrix. 

The reason that I didn't use vectors is that we know ahead of time what the lengths of the double dimensional array are and guarantees that the elements are stored contiguously in memory. Vectors do not guarantee this in exchange for dynamically sized lists.

A templated version of this program could potentially take in any vector of vectors of any type (double, int, and so on). 

# Operations supported 
1) Multiplication
2) Transpose

# API Endpoints
Creating the matrix data type:
1) using the rows and cols and taking input through stdin or some other way OR
2) by passing in a vector of vectors. 

When creating the matrix, you can specify how many threads should be used in computing it's matrix or when doing the product. 

```
MatrixMap(size_t r, size_t c, unsigned int max_threads = 0);
MatrixMap(vector<vector<long long int>> vect, unsigned int max_threads = 0);
```

As seen in the header file, the public member functions of the class MatrixMap are the endpoints. The `max_threads` argument is optional and allows for many multiplications to happen simultaneously—the default use case to use all the threads both for transpose and multiplication. By default, the system uses a system call to determine the number of threads and spawn that many threads. 

```
void print_matrix();
void take_stdin_matrix();
static MatrixMap multiply(const MatrixMap &, const MatrixMap &);
static MatrixMap transpose(const MatrixMap &a);
```

These funcitons take in constant references in order to minimize the amount of copying that needs to go in the transpose and multiplciation. 

# Example usage
This is from the example.cpp file which has a entire usage of the multiplication and transpose API using vectors. 

Transpose:
```
vector<vector<long long int> > vect3{
{ 10, 0, 0 },
{ 0, 10, 0 }
};

MatrixMap d = MatrixMap(vect3);
MatrixMap trans = MatrixMap::transpose(d);
```
Multiplication:
```
vector<vector<long long int> > vect{
{ 5, 0, 0 },
{ 0, 5, 0 },
{ 0, 0, 5 }
};
MatrixMap a = MatrixMap(vect);

vector<vector<long long int> > vect2{
{ 10, 0, 0 },
{ 0, 10, 0 },
{ 0, 0, 10 }
};

MatrixMap b = MatrixMap(vect2);

MatrixMap c = MatrixMap::multiply(a, b);
```

# Running Instructions
There are 2 example clients to the API. `example` which uses vectors and `main` which uses stdin to create the matrixes and then also prints out how long the computation took. 

## Example
`make example` will build the example program which you can execute by running `./example`.

## Main
`make matrix` will build the stdin version

# Build Instructions
2 example programs are compiled and linked in this program. 
1) The main.cpp program which takes the matrixes in through stdin.
2) The example.cpp program which takes input as vectors.

Run the `make` command to create the matrix binary as well as the example binary

## Execution

1. Matrix - The command line arguments for this are explained below.

 1. Run the `matrix` binary and input the values that are being prompted. For transpose:
 ```
 ./matrix -t
 ```
 For multliplication:
 ```
 ./matrix -m
 ```
 2. Run the `matrix` binary and input the values that are being prompted. For Transpose:
 ```
 cat sample_input.txt | ./matrix -t
 ```
 ```
 cat sample_input.txt | ./matrix -m
 ```
2. Example - This binary is a lot easier to run.

```
./example
```


# Command-line options
The 'main' program takes in command line input to figure out how many numbers are going to be input through stdin. If you give `-t`, you only supply 1 matrix. If you give `-m`, you have to provide 2 matrices. You can also use the longhand `--transpose` and `--mult`.
```
-t --transpose **Prints the transpose of a matrix given through stdin**
-m --mult **Prints the multiplication of 2 matrices given through stdin**
-s --single-thread **Uses only one thread for the computation**
```

The example program takes no input and just produces a multiplication and transpose to enable devlopers to use the source code in other parts of their system. 

# Test Framework 

There are two types of tests, performance tests, and correctness tests. 

They are in python 3 for ease of verifying the fast solution and use a third party library *numpy* to verify correctness. They both use the matrix example program and input the values from the python script into the matrix process using a pipe. 

1. *correctness tests* try to call the multiply and trnaspose functions with various matrix lengths starting at 1 and ending at 91. It generates numbers from 0 to 90 for correctness testing.

2. *performance tests* call the transpose and multiply functions using single thread and then without that restriction to report how long the two versions took in microseconds and what the speed up was in times notion (1.5 x, 8.7 x and so on).

The output from this script is in test_it_output.txt, which ran on hummingbird, which is a 32 core compute machine that graduate students in my university have access to. The results were a speedup of around 8x, but its hard to say because it depends heavily on the number of elements. 



## Dependencies 
1) Pip

```
apt-get install pip3
```

2) Numpy - This can be installed globally for the user like this 
```
pip3 install numpy --user
``` 
Or inside of a virtual environment like this
```
virtualenv venv -p=python3
source venv/bin/activate
pip install numpy 
```

## Running 
```
python3 test_it.py
```

# Future Work

This algorithm doesn't use the cache optimally, it utilizes main memory in an order that is prone to cache evictions. For that reason, Strassen's algorithm is the best way to solve this problem. It is a recursive divide and conquer algorithm that would add even more speed up. In this algorithm, most of the values will be cache hits and, therefore, will be faster. 

Templates to accept matrix multiplication of other number data types like double and long double into the same algorithm.
