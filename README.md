# SimpleThreadedMatrix
Threaded matrix experiments

Problem: Assuming No BLAS/MKL/ATLAS/CUDA libraries exist, implement a high
performance matrix transpose and multiplication in C++11 using threads

Compile:
g++ -o testmatrix testmatrix.cpp -std=c++11 -pthread

for single threaded:
g++ -o testmatrix testmatrix.cpp -std=c++11

Run tests:
time ./testmatrix

Testing method:
The first test is against known result and the rest of tests use the rule that
for multiplication and transpose:
(A*B)^T = (B^T)*(A^T)  
above quation holds for matrices

POSSIBLE IMPROVEMENT NEEDS TO BE LOOKED IF IT WERE A RELEASABLE SW:
Inside C++ concurrency in Action classic book, there is a very good couple
page discussion in section 8.3.1 how to do very large matrix calculation. It suggests 
dividing to square regions instead of rows or columns which have not tried that may 
give some improvement. Here is argument:
"consider multiplying two matrices that have
1,000 rows and 1,000 columns. That’s 1 million elements. If you have 100 processors,
they can handle 10 rows each for a nice round 10,000 elements. However, to calculate
the results of those 10,000 elements, they need to access the entirety of the second
matrix (1 million elements) plus the 10,000 elements from the corresponding rows in
the first matrix, for a grand total of 1,010,000 elements. On the other hand, if they
each handle a block of 100 elements by 100 elements (which is still 10,000 elements
total), they need to access the values from 100 rows of the first matrix (100 x 1,000 =
100,000 elements) and 100 columns of the second matrix (another 100,000). This is
only 200,000 elements, which is a five-fold reduction in the number of elements read.
If you’re reading fewer elements, there’s less chance of a cache miss and the potential
for greater performance."
