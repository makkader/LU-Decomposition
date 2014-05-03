
/*
Author: Md Abdul Kader
University Of Texas at El Paso
*/
#include<omp.h>
#include<cmath>
#include<cstdio>
#include<iostream>

#include<string>
using namespace std;

class matrix{
	public:
	int row,col;
	double **mat;
	matrix(int r,int c){
		
		row=r;
		col=c;
		mat=new double*[row];
		for(int i=0;i<row;i++){
			mat[i] = new double[col];
		}
	}
	matrix(){row=col=0;mat=NULL;}
	void set(matrix super,int ri,int cj){
	
		for(int i=0;i<row;i++)
			for(int j=0;j<col;j++)
				mat[i][j]=super.mat[ri+i][cj+j];	
	}
	void set(double linear[]){
	
		for(int i=0;i<row;i++)
			for(int j=0;j<col;j++)
				mat[i][j]=linear[i*col+j];	
	}
	void mapSub(matrix LU99,int ri,int cj){
		
		for(int i=0;i<LU99.row;i++)
			for(int j=0;j<LU99.col;j++){
			
				mat[ri+i][cj+j]=LU99.mat[i][j];
			
			}
		
	}
	void show(){
	
		for(int i=0;i<row;i++){
			for(int j=0;j<col;j++)
				printf("%3lf ",mat[i][j]);	
			printf("\n");
		}
		printf("-----------\n");
	}

};

matrix multiply(matrix A, matrix B ){

	matrix C(A.row,B.col);
	int ROW=A.row;
	int COL=B.col;
	int INNER=A.col;
	
	
		#pragma omp parallel for	
		for (int row = 0; row < ROW; ++row) 
		{
			for (int col = 0; col < COL; ++col)
			{
				double sum = 0;
				for (int inner = 0; inner < INNER; ++inner)
				{
					sum += A.mat[row][inner] * B.mat[inner][col];
				}
				C.mat[row][col] = sum;
			}
		}
		
	
	
	return C;
}

matrix subtract(matrix A, matrix B ){

	matrix C(A.row,A.col);
	
	for(int i=0;i<A.row;i++)
		for(int j=0;j<A.col;j++)
			 C.mat[i][j]=A.mat[i][j]-B.mat[i][j];
	return C;
		
		
}

matrix combineMatrix(matrix A00,matrix A01,matrix A10,matrix A11){

	matrix M(A00.row+A10.row,A00.col+A01.col);

	M.mapSub(A00,0,0);
	M.mapSub(A01,0,A00.col);
	M.mapSub(A10,A00.row,0);
	M.mapSub(A11,A00.row,A00.col);
	
	return M;

}

/*
matrix solveUpper(matrix A, matrix U){
	if(A.row==0|| A.col==0){
		matrix zero(0,0);
		return zero;
	}
	else if(U.col==1){
	
		for(int i=0;i<A.row;i++)
			A.mat[i][0]/=U.mat[0][0];
		return A;
	}
	
	matrix L00(1,1);
	L00.mat[0][0]=A.mat[0][0]/U.mat[0][0];
	
	matrix A01(1,A.col-1),A10(A.row-1,1),A11(A.row-1,A.col-1);
	A01.set(A,0,1);
	A10.set(A,1,0);
	A11.set(A,1,1);
	
	matrix U01(1,U.col-1),U11(U.row-1,U.col-1);
	U01.set(U,0,1);
	U11.set(U,1,1);
	
	matrix L10(A10.row,A10.col);
	for(int i=0;i<L10.row;i++)
		L10.mat[i][0]=A10.mat[i][0]/U.mat[0][0];
		
	matrix L11,L01;
	if(A.row>64){
		#pragma omp task shared(L01)
   		{ 
   			matrix iA01=subtract(A01,multiply(L00,U01));
			L01=solveUpper(iA01,U11);

   		}
		#pragma omp task shared(L11)
   		{ 
   			matrix iA11=subtract(A11,multiply(L10,U01));
			L11=solveUpper(iA11,U11);
   		}

		#pragma omp taskwait
	}
	else {
			matrix iA01=subtract(A01,multiply(L00,U01));
			L01=solveUpper(iA01,U11);

			matrix iA11=subtract(A11,multiply(L10,U01));
			L11=solveUpper(iA11,U11);
	}
	
	return combineMatrix(L00,L01,L10,L11);
}
matrix solveLower(matrix A, matrix L){
	
	if(A.row==0 || A.col==0){
	
		matrix zero(0,0);
		return zero;
	}
	else if(L.row==1){
		return A;
	}
	
	
	matrix U00(1,1);
	U00.set(A,0,0);
	
	matrix U01(1,A.col-1);
	U01.set(A,0,1);
	
	matrix A10(A.row-1,1),A11(A.row-1,A.col-1);
	A10.set(A,1,0);
	A11.set(A,1,1);
	
	matrix L10(L.row-1,1),L11(L.row-1,L.col-1);
	L10.set(L,1,0);
	L11.set(L,1,1);
	
	matrix  U10,U11;
	if(A.row>64){
		#pragma omp task shared(U10)
   		{ 
   			matrix iA10=subtract(A10,multiply(L10,U00));
			U10=solveLower(iA10,L11);
   		}
   		#pragma omp task shared(U11)
   		{ 
   			matrix iA11=subtract(A11,multiply(L10,U01));
			 U11=solveLower(iA11,L11);
   		}
		#pragma omp taskwait
	}
	else {
			matrix iA10=subtract(A10,multiply(L10,U00));
			U10=solveLower(iA10,L11);
			matrix iA11=subtract(A11,multiply(L10,U01));
			U11=solveLower(iA11,L11);
	}
	
	
	
	return combineMatrix(U00,U01,U10,U11);
}
*/
matrix solveUpper1(matrix A, matrix U){
	if(A.row==0|| A.col==0){
		matrix zero(0,0);
		return zero;
	}
	else if(U.col==1){
	
		for(int i=0;i<A.row;i++)
			A.mat[i][0]/=U.mat[0][0];
		return A;
	}
	
	matrix L00(1,1);
	L00.mat[0][0]=A.mat[0][0]/U.mat[0][0];
	
	matrix A01(1,A.col-1),A10(A.row-1,1),A11(A.row-1,A.col-1);
	A01.set(A,0,1);
	A10.set(A,1,0);
	A11.set(A,1,1);
	
	matrix U01(1,U.col-1),U11(U.row-1,U.col-1);
	U01.set(U,0,1);
	U11.set(U,1,1);
	
	matrix L10(A10.row,A10.col);
	for(int i=0;i<L10.row;i++)
		L10.mat[i][0]=A10.mat[i][0]/U.mat[0][0];
		
	matrix iA01=subtract(A01,multiply(L00,U01));
	matrix L01=solveUpper1(iA01,U11);

	
	matrix iA11=subtract(A11,multiply(L10,U01));
	matrix L11=solveUpper1(iA11,U11);
	
	return combineMatrix(L00,L01,L10,L11);
}
matrix solveLower1(matrix A, matrix L){
	
	if(A.row==0 || A.col==0){
	
		matrix zero(0,0);
		return zero;
	}
	else if(L.row==1){
		return A;
	}
	
	
	matrix U00(1,1);
	U00.set(A,0,0);
	
	matrix U01(1,A.col-1);
	U01.set(A,0,1);
	
	matrix A10(A.row-1,1),A11(A.row-1,A.col-1);
	A10.set(A,1,0);
	A11.set(A,1,1);
	
	matrix L10(L.row-1,1),L11(L.row-1,L.col-1);
	L10.set(L,1,0);
	L11.set(L,1,1);
	
	matrix iA10=subtract(A10,multiply(L10,U00));
	matrix U10=solveLower1(iA10,L11);
	
	matrix iA11=subtract(A11,multiply(L10,U01));
	matrix U11=solveLower1(iA11,L11);
	
	return combineMatrix(U00,U01,U10,U11);
}
matrix solveLower(matrix A, matrix L){ //div/2 algo 
	
	if(A.row<4)
		return solveLower1(A,L);
	//printf("r %d, C=%d A\n",A.row,A.col);
	int r=A.row;
	int c=A.col;
	
	int r1=r/2; 
	int r2=r-r1;
	int c1=c/2;
	int c2=c-c1;
	
	matrix A00(r1,c1),A01(r1,c2),A10(r2,c1),A11(r2,c2);
	A00.set(A,0,0);
	A01.set(A,0,c1);
	A10.set(A,r1,0);
	A11.set(A,r1,c1);
	
	int cL1=L.col/2;
	int cL2=L.col-cL1; 
	matrix L00(r1,cL1),L10(r2,cL1),L11(r2,cL2);
	L00.set(L,0,0);
	L10.set(L,r1,0);
	L11.set(L,r1,cL1);
	
	matrix U00,U01,U10,U11,iA10,iA11;
	
	#pragma omp task shared(U00)
	U00=solveLower(A00,L00);
	#pragma omp task shared(U01)
	U01=solveLower(A01,L00);
	#pragma omp taskwait
	
	#pragma omp task shared(iA10)
	 iA10=subtract(A10,multiply(L10,U00));
	 #pragma omp task shared(iA11)
	 iA11=subtract(A11,multiply(L10,U01));
	 #pragma omp taskwait
	 
	 #pragma omp task shared(U10)
	 U10=solveLower(iA10,L11); 
	 #pragma omp task shared(U11)
 	 U11=solveLower(iA11,L11); 
	#pragma omp taskwait
	
	return combineMatrix(U00,U01,U10,U11);
}

matrix solveUpper(matrix A, matrix U){// div/2 algo 
	
	if(A.row<4)
		return solveUpper1(A,U);
	int r=A.row;
	int c=A.col;
	
	int r1=r/2; 
	int r2=r-r1;
	int c1=c/2;
	int c2=c-c1;
	
	matrix A00(r1,c1),A01(r1,c2),A10(r2,c1),A11(r2,c2);
	A00.set(A,0,0);
	A01.set(A,0,c1);
	A10.set(A,r1,0);
	A11.set(A,r1,c1);
	
	int rU1=U.row/2;
	int rU2=U.row-rU1;
	matrix U00(rU1,c1),U01(rU1,c2),U11(rU2,c2);
	U00.set(U,0,0);
	U01.set(U,0,c1);
	U11.set(U,rU1,c1);
	
	matrix L10,L01,L11,L00,iA01,iA11;
	#pragma omp task shared(L00)
	{L00=solveUpper(A00,U00);}
	#pragma omp task shared(L10)
	{L10=solveUpper(A10,U00);}
	#pragma omp taskwait
	
	#pragma omp task shared(iA01)
	{	iA01=subtract(A01,multiply(L00,U01));}
	#pragma omp task shared(iA11)
	{	iA11=subtract(A11,multiply(L10,U01));}
	#pragma omp taskwait
	
	#pragma omp task shared(L01)
	L01=solveUpper(iA01,U11); 
	#pragma omp task shared(L11)
	L11=solveUpper(iA11,U11); 
	#pragma omp taskwait
	
	return combineMatrix(L00,L01,L10,L11);
}
matrix makeL(matrix L00,matrix L10,matrix L11){

	matrix L(L00.row+L10.row,L00.col+L11.col);
	
	for(int i=0;i<L00.row;i++)
		for(int j=L00.col;j<L.col;j++)
			L.mat[i][j]=0;
			
	L.mapSub(L00,0,0);
	L.mapSub(L10,L00.row,0);
	L.mapSub(L11,L00.row,L00.col);
	
	return L;
}

matrix makeU(matrix U00,matrix U01,matrix U11){

	matrix U(U01.row+U11.row,U00.col+U01.col);
	
	for(int i=U00.row;i<U.row;i++)
		for(int j=0;j<U00.col;j++)
			U.mat[i][j]=0;

	U.mapSub(U00,0,0);
	U.mapSub(U01,0,U00.col);
	U.mapSub(U11,U00.row,U00.col);
	
	return U;
}
pair<matrix,matrix> LUDecompose(matrix A){


	if(A.row==1){
		
		matrix L(1,1);
		L.mat[0][0]=1;
		return pair<matrix,matrix>( L, A);
	}
	else if (A.col==1){
		double mx=A.mat[0][0];
		for(int i=1;i<A.row;i++)
			if(mx<A.mat[i][0])
				mx=A.mat[i][0];
		
		for(int i=0;i<A.row;i++)
			A.mat[i][0]/=mx;
			
		matrix U(1,1);
		U.mat[0][0]=mx;
		return pair<matrix,matrix>( A, U);		
		
	}
	
	int r=A.row;
	int c=A.col;
	
	int r1=r/2;
	int r2=r-r1;
	int c1=c/2;
	int c2=c-c1;
	matrix A00(r1,c1);
	A00.set(A,0,0);
	
	matrix A01(r1,c2);
	A01.set(A,0,c1);
	
	matrix A10(r2,c1);
	A10.set(A,r1,0);
	
	matrix A11(r2,c2);
	A11.set(A,r1,c1);
	
	
	
	pair<matrix,matrix> p=LUDecompose(A00);
	matrix L00=p.first;
	matrix U00=p.second;
	
	matrix U01,L10;
	if(A.row>4){
	
		#pragma omp task shared(U01)
   		{ 
   			U01=solveLower(A01,L00);
   		}
   		#pragma omp task shared(L10)
   		{ 
   			L10=solveUpper(A10,U00);
   		}
		#pragma omp taskwait
		
	}
	else {
	
		U01=solveLower(A01,L00);
		L10=solveUpper(A10,U00);
	}
	
	
	matrix iA11=subtract(A11,multiply(L10,U01));
	pair<matrix,matrix> q=LUDecompose(iA11);
	matrix L11=q.first;
	matrix U11=q.second;

	matrix L=makeL(L00,L10,L11);
	matrix U=makeU(U00,U01,U11);		
	
	return pair<matrix,matrix>(L,U);
	
}

string isEqual(matrix A,matrix B){//auxiliary method
	string False("False"),True("True");
	for(int i=0;i<A.row;i++)
		for(int j=0;j<A.col;j++)
			if(fabs(A.mat[i][j]-B.mat[i][j])>0.000001)
				return False;
	return True;
}
const int dim=3000;
void getMatrix(int dim,double A[]){
	char fn[100];
	sprintf(fn,"A%d.txt",dim);
	freopen(fn,"r",stdin);
	for(int i=0;i<dim;i++)
		for(int j=0;j<dim;j++)
			cin>>A[i*dim+j];

}
int main(){
	
	
	double ar[dim*dim];
	getMatrix(dim,ar);
	
	matrix AA(dim,dim);
	AA.set(ar);
	pair<matrix, matrix> p;
	
	omp_set_dynamic(1);
  	omp_set_num_threads(4);
	
	double start_t=omp_get_wtime();
	#pragma omp parallel shared(AA)
	{
		#pragma omp single
		{
			p=LUDecompose(AA);
		
		}
		
	}
	double elapse_t=omp_get_wtime()-start_t;
	
	printf("Omp Elapsed time: %lf\n",elapse_t);
	if(dim<=2000){
		printf("Verifying...\n");
		cout<<"Is it correct solution? "<<isEqual(AA,multiply(p.first,p.second))<<endl;
	}
	

return 0;
}