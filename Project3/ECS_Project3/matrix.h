#ifndef MATRIX_H
#define MATRIX_H

template<int W, int H>
struct Matrix{
   double m[W][H];

   //A*B=C sizeX(files de A), sizeY=columnes de B sizeY
   template<int sizeAX,int sizeAY, int sizeBY>
   void mult(const Matrix<sizeAX,sizeAY>& A,const Matrix<sizeAY,sizeBY>& B, Matrix<sizeAX,sizeBY>& C){//,const double & B,double & C){

	   for(int i=0;i<sizeAX;i++)
		   for(int j=0;j<sizeBY;j++)
			   for(int k=0;k<sizeAY;k++)
				   C.m[i][j]+=A.m[i][k]*B.m[k][j];
   }
};

#endif // MATRIX_H
