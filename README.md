Author: MD ABDUL KADER
University of Texas at EL Paso
Email: mkader@miners.utep.edu

Copyright: You can use it for any purpose except as a project work for CS5334/4390 (Spring	2014) in University of Texas at El Paso. But you can use it for testing and comparing with your own implementation. 


Taksed Based LU-Decomposition
=============================
File structure:
-------------
	1. ludecomposition.cpp
		->This is the sequential implementation of LU decomposition.
	2. OMP-LUDecomposition.cpp
		-> This is the code after adding OpenMP directives to the sequential implementation. It uses 4 threads. 
	3. cilkLUDecomposition.cpp
		-> This is the cilk version of LU decomposition. 
		
Compile:
---------
	I used intel c++ compiler without any optimization. 
	To compile any file use the command 
	icpc -openmp -o <filename>.out <filename>.cpp 
Run:
-----
	./<filename>.out

Testing:
--------
	I have added a 3000x3000 matrix in A3000.txt file. After downloading the code if you compile and run it will decompose that 3000x3000 matrix. 
	To change the size of the matrix you just need to change the variable const int 'dim'.   
 Note:
------
	The verification steps will not be performed if dimension size is greater than 2000x2000. 
	

