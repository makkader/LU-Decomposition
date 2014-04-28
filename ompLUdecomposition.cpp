/*
h
Author: Md Abdul Kader
University Of Texas at El Paso
*/
#include<omp.h>
#include<cstdio>
#include<iostream>
#include<vector>
using namespace std;
typedef vector<double> vi;
typedef vector<vector<double> >vii;
class matrix{
	public:
	int row,col;
	vector<vi> mat;
	matrix(int r,int c){
		
		row=r;
		col=c;
		for(int i=0;i<r;i++){
			vi t(c);
			mat.push_back(t);
		}
		
	}
	matrix(){
		//printf("empty constructor\n");
	}
	matrix(const matrix &obj){
		
		//printf("copy constructor s\n");
		row=obj.row;
		col=obj.col;
		
		//mat=obj.mat;
		
		for(int i=0;i<row;i++){
			vi t(col);
			mat.push_back(t);
		}
		for(int i=0;i<row;i++)
			for(int j=0;j<col;j++)
				mat[i][j]=obj.mat[i][j];
				
		//printf("copy constructor id %d\n",omp_get_thread_num());
	}
	void set(vii super,int ri,int cj){//after creating matrix need to set values;
	
		for(int i=0;i<row;i++)
			for(int j=0;j<col;j++)
				mat[i][j]=super[ri+i][cj+j];	
	}
	void set(double linear[]){//after creating matrix need to set values;
	
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
	
	
	for (int row = 0; row != ROW; ++row) 
	{
 		for (int col = 0; col != COL; ++col)
		{
			double sum = 0;
			for (int inner = 0; inner != INNER; ++inner)
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
	A01.set(A.mat,0,1);
	A10.set(A.mat,1,0);
	A11.set(A.mat,1,1);
	
	matrix U01(1,U.col-1),U11(U.row-1,U.col-1);
	U01.set(U.mat,0,1);
	U11.set(U.mat,1,1);
	
	matrix L10(A10.row,A10.col);
	for(int i=0;i<L10.row;i++)
		L10.mat[i][0]=A10.mat[i][0]/U.mat[0][0];
		
	matrix iA01=subtract(A01,multiply(L00,U01));
	matrix L01=solveUpper(iA01,U11);

	
	matrix iA11=subtract(A11,multiply(L10,U01));
	matrix L11=solveUpper(iA11,U11);
	
	return combineMatrix(L00,L01,L10,L11);
}
void solveLower(matrix A, matrix L,matrix *rU){
	
	
	if(A.row==0 || A.col==0){
	
		matrix zero(0,0);
		*rU=zero;
		return ;
	}
	else //if(L.row==1)
	{
		*rU=A;
		return ;
	}
	
	
	matrix U00(1,1);
	U00.set(A.mat,0,0);
	
	matrix U01(1,A.col-1);
	U01.set(A.mat,0,1);
	
	matrix A10(A.row-1,1),A11(A.row-1,A.col-1);
	A10.set(A.mat,1,0);
	A11.set(A.mat,1,1);
	
	matrix L10(L.row-1,1),L11(L.row-1,L.col-1);
	L10.set(L.mat,1,0);
	L11.set(L.mat,1,1);
	
	matrix iA10=subtract(A10,multiply(L10,U00));
	matrix U10;
	solveLower(iA10,L11,&U10);
	
	matrix iA11=subtract(A11,multiply(L10,U01));
	matrix U11;
	solveLower(iA11,L11,&U11);
	
	matrix U=combineMatrix(U00,U01,U10,U11);
	*rU=U;
	//return U;
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
	A00.set(A.mat,0,0);
	
	matrix A01(r1,c2);
	A01.set(A.mat,0,c1);
	
	matrix A10(r2,c1);
	A10.set(A.mat,r1,0);
	
	matrix A11(r2,c2);
	A11.set(A.mat,r1,c1);
	
	
	matrix L,U;
	
	pair<matrix,matrix> p=LUDecompose(A00);
	matrix L00=p.first;
	matrix U00=p.second;
	matrix U01(L00.col,A01.col);
	matrix L10;
	
	solveLower(A01,L00,&U01);
	#pragma omp parallel sections 
	{	
  		#pragma omp section 
   		{ solveLower(A01,L00,&U01);}
   		#pragma omp section 
   		{ L10=solveUpper(A10,U00);}

	}
	
	matrix iA11=subtract(A11,multiply(L10,U01));
	
	pair<matrix,matrix> q=LUDecompose(iA11);
	matrix L11=q.first;
	matrix U11=q.second;

	L=makeL(L00,L10,L11);
	U=makeU(U00,U01,U11);		
	
	return pair<matrix,matrix>(L,U);
	
}
void getMatrix(int dim,double A[]){

	freopen("A.txt","r",stdin);
	for(int i=0;i<dim;i++)
		for(int j=0;j<dim;j++)
			cin>>A[i*dim+j];

}
int main(){
	
	const int dim=100;
	
	double ar[dim*dim];
	getMatrix(dim,ar);
	
	matrix AA(dim,dim);
	AA.set(ar);
	pair<matrix, matrix> p;
	
	omp_set_dynamic(0);
  	omp_set_num_threads(4);
	
	double start_t=omp_get_wtime();
	#pragma omp parallel shared(AA)
	{
		#pragma omp single
		{
			p=LUDecompose(AA);
		
			/*
			printf("T_id=%d\n",omp_get_thread_num());
		
			p.first.show();
			p.second.show();
			
			multiply(p.first,p.second).show();
			*/
		}
		
	}
	double elapse_t=omp_get_wtime()-start_t;
	
	printf("elapse time: %lf\n",elapse_t);

return 0;
}