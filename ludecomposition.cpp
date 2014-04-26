/*
Author: Md Abdul Kader
University Of Texas at El Paso
*/

#include<iostream>
#include<vector>
using namespace std;
typedef vector<int> vi;
typedef vector<vector<int> >vii;
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
	void show(){
	
		for(int i=0;i<row;i++){
			for(int j=0;j<col;j++)
				printf("%3d ",mat[i][j]);	
			printf("\n");
		}
	}

};


int main(){
	
	
	int ar[3][3]={{1,-2,3},{2,-5,12},{0,2,-10}};
	matrix A(3,3);
	A.set(ar,0,0);
	A.show();

return 0;
}