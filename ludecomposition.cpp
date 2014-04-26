/*
Author: Md Abdul Kader
University Of Texas at El Paso
*/

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
	void set(vii super,int ri,int cj){//after creating matrix need to set values;
	
		for(int i=0;i<row;i++)
			for(int j=0;j<col;j++)
				mat[i][j]=super[ri+i][cj+j];	
	}
	void set(int super[][3],int ri,int cj){//after creating matrix need to set values;
	
		for(int i=0;i<row;i++)
			for(int j=0;j<col;j++)
				mat[i][j]=super[ri+i][cj+j];	
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

matrix solveLower(matrix A, matrix L){
	
	
	
	return U;
}

matrix solveUpper(matrix A, matrix U){
	
	
	
	return L;
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
pair<matrix,matrix> LUDecompose(A){


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
	
	
	
	pair<matrix,matrix> p=LUDecompose(A00);
	matrix L00=p.first;
	matrix U00=p.second;
	
	matrix U01=solveLower(A01,L00);
	matrix L10=solveUpper(A10,U00);
	
	matrix iA11=sumtract(A11,multiply(L10,U01));
	
	pair<matrix,matrix> q=LUDecompose(iA11);
	matrix L11=q.first;
	matrix U11=q.second;

	matrix L=makeL(L00,L10,L11);
	matrix U=makeU(U00,U01,U11);		
	
	return pair<matrix,matrix>(L,U);
	
}
int main(){
	
	
	int ar[3][3]={{1,-2,3},{2,-5,12},{0,2,-10}};
	matrix A(3,3);
	A.set(ar,0,0);
	A.show();
	
return 0;
}