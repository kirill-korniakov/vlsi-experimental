/* 
* main.cpp
* this is a part of itlDragon source codes
* (C) 2005-2007 UNN, ITLab, Лебедев Илья 
* email: bkmzbmk@yandex.ru 
*/

#include "iostream"

using namespace std;
void Vmnk( double *index_1,double *index_2,
					double *values,
					int n,int m,
					double T, double R);

int main()
{
	int o,n=7,m=7;//временные названия
	double *index_1 = new double [n];
	double *index_2 = new double [m];
	double *values = new double [n*m];
	
	//инициализация из примера
	/*

	index_1 ("0.005, 0.01, 0.015, 0.02, 0.03, 0.04, 0.1");
	index_2 ("0.024, 0.072, 0.288, 0.672, 1.104, 1.584, 2.16");
	valuesues ( \
	"0.029688, 0.029575, 0.035366, 0.047994, 0.059677, 0.07074, 0.080771", \
	"0.039491, 0.039146, 0.044415, 0.057699, 0.070346, 0.082459, 0.09406", \
	"0.049123, 0.048946, 0.053685, 0.065729, 0.077742, 0.09028, 0.102926", \
	"0.05859, 0.058254, 0.062721, 0.074503, 0.085435, 0.098688, 0.11257", \
	"0.078092, 0.078202, 0.081425, 0.091854, 0.102095, 0.115162, 0.128974", \
	"0.097977, 0.097659, 0.100593, 0.110105, 0.120583, 0.131062, 0.143965", \
	"0.220596, 0.220451, 0.221192, 0.227688, 0.235268, 0.244518, 0.25432");

	*/

	index_1[0]= 0.005; 
	index_1[1]= 0.01; 
	index_1[2]= 0.015; 
	index_1[3]= 0.02; 
	index_1[4]= 0.03; 
	index_1[5]= 0.04; 
	index_1[6]= 0.1;

	index_2[0]= 0.024;
	index_2[1]= 0.072; 
	index_2[2]= 0.288; 
	index_2[3]= 0.672; 
	index_2[4]= 1.104; 
	index_2[5]= 1.584; 
	index_2[6]= 2.16;


	values[0]= 0.029688;
	values[1]= 0.029575;
	values[2]= 0.035366;
	values[3]= 0.047994;
	values[4]= 0.059677;
	values[5]= 0.07074;
	values[6]= 0.080771;
	values[7]= 0.039491;
	values[8]= 0.039146;
	values[9]= 0.044415;
	values[10]= 0.057699;
	values[11]= 0.070346;
	values[12]= 0.082459;
	values[13]= 0.09406;
	values[14]= 0.049123; 
	values[15]= 0.048946;
	values[16]= 0.053685;
	values[17]= 0.065729;
	values[18]= 0.077742;
	values[19]= 0.09028;
	values[20]= 0.102926;
	values[21]= 0.05859;
	values[22]= 0.058254;
	values[23]= 0.062721;
	values[24]= 0.074503;
	values[25]= 0.085435;
	values[26]= 0.098688;
	values[27]= 0.11257;
	values[28]= 0.078092; 
	values[29]= 0.078202;
	values[30]= 0.081425;
	values[31]= 0.091854;
	values[32]= 0.102095;
	values[33]= 0.115162;
	values[34]= 0.128974;
	values[35]= 0.097977;
	values[36]= 0.097659;
	values[37]= 0.100593;
	values[38]= 0.110105;
	values[39]= 0.120583;
	values[40]= 0.131062;
	values[41]= 0.143965;
	values[42]= 0.220596;
	values[43]= 0.220451;
	values[44]= 0.221192;
	values[45]= 0.227688;
	values[46]= 0.235268;
	values[47]= 0.244518;
	values[48]= 0.25432;

	double T = 0.0, R = 0.0;
	//формирование новой системы и её решение 
	Vmnk(index_1,index_2,values,n,m,T,R);


	delete [] index_1;
	delete [] index_2;
	delete [] values;
	cin>>o;
	return 0;
};

//вычисление методом наименьших квадратов
//уравнение dT=Tintrinsic + R*Cload + a * skew
//искомые переменные t, r, a;
void Vmnk(const double *index_1,
		  const double *index_2,
		  const double *values,
		  const int n, //length of index_1
		  const int m, //length of index_2
		  double& T,
		  double& R)
{
	const int dim = 3;//task dimention
	
	double *A = new double [n * m * dim];

	//иходная матрица A = [1,index_1,index_2] , (n*m строк)
	for(int i1 = 0 ; i1 < n ; i1++)
		for(int j1 = 0 ; j1 < m ; j1++)
		{
			A[(j1 + m * i1) * dim + 0] = 1;
			A[(j1 + m * i1) * dim + 1] = index_1[i1];
			A[(j1 + m * i1) * dim + 2] = index_2[j1];
		};

	double *M = new double [dim * dim];
	
	//M=0
	for(int i3 = 0 ; i3 < dim * dim ; i3++)
		M[i3] = 0.0;

	//M = transp(A) * A
	for (int i = 0 ; i < dim ; i++)
		for (int j = 0 ; j < dim ; j++)
			for (int k = 0 ; k < m * n ; k++)
				M[i * dim + j] += A[k * dim + i] * A[k * dim + j];
				
	double *Y = new double [dim];
	
	//Y = 0
	for(int i2 = 0; i2 < dim; i2++)
		Y[i2] = 0.0;

	//Y = transp(A) * values
	for(int h = 0 ; h < dim ; h++)
		for(int g = 0 ; g < m * n ; g++)
			Y[h] += A[g * dim + h] * values[g];

	//detM=||M||
	double detM = (M[0*3+1] * M[2*3+0] * M[1*3+2] -
				   M[0*3+1] * M[1*3+0] * M[2*3+2] +
				   M[1*3+0] * M[2*3+1] * M[0*3+2] +
				   M[1*3+1] * M[0*3+0] * M[2*3+2] -
				   M[2*3+0] * M[1*3+1] * M[0*3+2] -
				   M[2*3+1] * M[0*3+0] * M[1*3+2]);

	//calculate T and R
	T = (M[1*3+1] * Y[0] * M[2*3+2] - 
		 M[1*3+1] * M[0*3+2] * Y[2] +
		 M[0*3+2] * M[2*3+1] * Y[1] -
		 Y[0] * M[2*3+1] * M[1*3+2] +
		 M[0*3+1] * M[1*3+2] * Y[2] -
		 M[0*3+1] * Y[1] * M[2*3+2]) / detM;


	R = - (-Y[0] * M[2*3+0] * M[1*3+2] +
			Y[0] * M[1*3+0] * M[2*3+2] +
			M[0*3+0] * M[1*3+2] * Y[2] -
			M[0*3+0] * Y[1] * M[2*3+2] -
			M[0*3+2] * M[1*3+0] * Y[2] +
			M[0*3+2] * M[2*3+0] * Y[1]) / detM;
		
	//a = (-M[0*3+1] * M[1*3+0] * Y[2] - 
		//M[2*3+1] * M[0*3+0] * Y[1] + 
		//M[1*3+0] * M[2*3+1] * 
		//Y[0] +	M[0*3+1] * M[2*3+0] * 
		//Y[1] + M[1*3+1] * M[0*3+0] * Y[2] -
		//M[2*3+0] * M[1*3+1] * Y[0]) / detM	;

/*  возможно проверка нужна будет и после встраивания в наш инструмент, но надо будет подумать в каком виде

	//вывод в консоль нуждается в более точтом оформлении
	cout<<"\t t="<<t<<"\n\t r="<<r<<"\n\t a="<<a<<"\n\t";

	double proverka=0,max=0;
	for(int i = 0 ; i < n ; i++)
		//вычисление погрешности
		for(int j = 0 ; j < m ; j++)
		{
			proverka=
				A[j*3 + m * i*3 +0] * t+
				A[j*3 + m * i*3 + 1] * r+
				A[j*3 + m * i*3 + 2]* a - values[j + m * i];


			if( max<proverka) 
				max=proverka;
		};
	//cout<<"max pogreshnost="<<max;
*/
	delete [] Y;
	delete [] M;
	delete [] A;
}


