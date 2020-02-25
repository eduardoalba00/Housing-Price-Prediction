#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))

double** multiply_matrix(double** mat1, double** mat2, int row1, int col1, int row2, int col2){
	double** res_mat = (double**)malloc(row1*sizeof(double*));
	for(int i = 0; i < row1; i++){
		res_mat[i]=(double*)malloc(col2*sizeof(double));
	}

	double num = 0;
	for(int i = 0; i < row1; i++){
		for(int j = 0; j < col2; j++){
			for(int k = 0; k < row2; k++){
				num = num + mat1[i][k] * mat2[k][j];
      		}
      	res_mat[i][j] = num;
      	num = 0;
		}
	}

	return res_mat;
}

/*void print_matrix(double** mat, int row1, int col2){
	for(int i = 0; i < row1; i++){
    	for(int j = 0; j < col2; j++){
      		printf ("%lf\t", mat[i][j]);
    	}
   	printf("\n");
	}
}
*/
void divide_row(double** matrix, int row, int col, double num){
	for(int i = 0; i < col; i++){
		matrix[row][i] = matrix[row][i]/num;
	}
}

void subtract_row(double** matrix, int row1, int row2, int col, double ratio){
	for(int i = 0; i < col; i++){
		matrix[row2][i] = matrix[row2][i] - (ratio * matrix[row1][i]);
	}
}

double** transpose(double** matrix, int row, int col){

	double** result = (double**)malloc(col*sizeof(double*));
	for(int i = 0; i < col; i++){
		result[i]=(double*)malloc(row*sizeof(double));
	}

	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			result[j][i] = matrix[i][j];
		}
	}
	return result;
}

double** inverse(double** matrix,int row, int col){

	double** res_mat = (double**)malloc(row*sizeof(double*));
	for(int i = 0; i < row; i++){
		res_mat[i]=(double*)malloc(col*sizeof(double));
	}

	//make result mat same as initial matrix
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			res_mat[i][j] = matrix[i][j];
		}
	}

	double** indentity = (double**)malloc(row*sizeof(double*));
	for(int i = 0; i < row; i++){
		indentity[i]=(double*)malloc(col*sizeof(double));
	}

	//create indentity matrix augmented
	for(int i = 0; i < row; i++){
		for(int j = 0; j < row; j++){
			if(j == i){
				indentity[i][j] = 1;
			} else{
				indentity[i][j] = 0;
			}

		}
	}

	double ratio;

	//Gaussian Elimination
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			if(i == j){
				if(res_mat[i][j] != 1){
					ratio = res_mat[i][j];
					divide_row(res_mat, i, col, ratio);
					divide_row(indentity, i, col, ratio);
				}
				for(int k = i+1; k < row; k++){
					if(res_mat[k][j] != 0){
						ratio = res_mat[k][j];
						subtract_row(res_mat, i, k, col, ratio);
						subtract_row(indentity, i, k, col, ratio);
					}
				}
				for(int k = i-1; k >= 0; k--){
					if(res_mat[k][j] != 0){
						ratio = res_mat[k][j];
						subtract_row(res_mat, i, k, col, ratio);
						subtract_row(indentity, i, k, col, ratio);
					}
				}
			}
		}
	}

	return indentity;
}

void solve_final(double** matrix, double** w, int row,int col){

	double num = 0;

	double** result=(double**)malloc(row*sizeof(double*));
	for(int i = 0; i < row; i++){
		result[i]=(double*)malloc(1*sizeof(double));
	}

	for(int i = 0; i < row; i++){
		num = w[0][0];
		for(int j = 0; j < col; j++){

			num += (matrix[i][j] * w[j+1][0]);
		}
		result[i][0] = num;
		printf("%0.0lf\n", result[i][0]);
	}
}

int main(int argc, char** argv)
{
	int num_attributes;
	int num_examples;
	int num_test;

	FILE *train = fopen(argv[1], "r");
	FILE *test = fopen(argv[2], "r");

	fscanf(train,"%d\n",&num_attributes);
	fscanf(train,"%d\n",&num_examples);
	fscanf(test,"%d\n",&num_test);

	// printf("Number of attributes (K) = %d\n", num_attributes);
	// printf("Number of examples (N) = %d\n", num_examples);
	// printf("Number of test data points (M) = %d\n", num_test);


	//TRAIN MATRIX
	double** train_mat = (double**)malloc(num_examples*sizeof(double*));
	for(int i = 0; i < num_examples; i++){
		train_mat[i] = (double*)malloc((num_attributes+1)*sizeof(double));
	}


	for(int row = 0; row < num_examples; row++){
		for(int col = 0; col < num_attributes+1; col++){
			fscanf(train,"%lf,", &train_mat[row][col]);
		}
	}

	// printf("\nTRAIN MATRIX\n");
	// print_matrix(train_mat, num_examples, num_attributes+1);


	//TEST MATRIX
	double** test_mat = (double**)malloc(num_test*sizeof(double*));
	for(int i = 0; i < num_test; i++){
		test_mat[i] = (double*)malloc((num_attributes)*sizeof(double));
	}

	for(int row = 0; row < num_test; row++){
		for(int col = 0; col < num_attributes; col++){
			fscanf(test,"%lf,", &test_mat[row][col]);
		}
	}

	// printf("\nTEST MATRIX\n");
	// print_matrix(test_mat, num_test, num_attributes);

	//X MATRIX
	double** x_matrix = (double**)malloc(num_examples*sizeof(double*));
	for(int i = 0; i < num_examples; i++){
		x_matrix[i] = (double*)malloc((num_attributes+1)*sizeof(double));
	}

	for(int row = 0; row < num_examples; row++){
		x_matrix[row][0] = 1;
	}

	for(int row = 0; row < num_examples; row++){
		for(int col = 1; col < num_attributes+1; col++){
			x_matrix[row][col] = train_mat[row][col-1];
		}
	}

	// printf("\nX MATRIX\n");
	// print_matrix(x_matrix, num_examples, num_attributes+1);

	//X TRANSPOSE MATRIX
	double** x_transpose = transpose(x_matrix, num_examples, num_attributes+1);

	// printf("\nX TRANSPOSE MATRIX\n");
	// print_matrix(x_transpose, num_attributes+1, num_examples);


	//Y MATRIX
	double** y_matrix=(double**)malloc(num_examples*sizeof(double*));
	for(int i = 0; i < num_examples; i++){
		y_matrix[i]=(double*)malloc(1*sizeof(double));
	}

	for(int row = 0; row < num_examples; row++){
		y_matrix[row][0] = train_mat[row][num_attributes];
	}

	// printf("\nY MATRIX\n");
	// print_matrix(y_matrix, num_examples, 1);

	//X^T*X
	double** xtranspose_xmatrix = multiply_matrix(x_transpose, x_matrix, num_attributes+1, num_examples, num_examples, num_attributes+1);
	//printf("\nX^T*X\n");
	//print_matrix(xtranspose_xmatrix, num_attributes+1, num_attributes+1);

	//X^T*Y
	double** xtranspose_ymatrix = multiply_matrix(x_transpose, y_matrix, num_attributes+1, num_examples, num_examples, 1);
	// printf("\nX^T*Y\n");
	// print_matrix(xtranspose_ymatrix, num_attributes+1, 1);

	double** xtx_inverse = inverse(xtranspose_xmatrix, num_attributes+1, num_attributes+1);
	// printf("\nXTX Inverse\n");
	// print_matrix(xtx_inverse, num_attributes+1, num_attributes+1);

	double** w = multiply_matrix(xtx_inverse, xtranspose_ymatrix, num_attributes+1, num_attributes+1, num_attributes+1, 1);
	// printf("\nW Matrix\n");
	// print_matrix(w, num_attributes+1, 1);

	double** weights = (double**)malloc(num_attributes*sizeof(double*));
	for(int i = 0; i < num_attributes; i++){
		weights[i]=(double*)malloc(1*sizeof(double));
	}

	for(int i = 0; i < num_attributes; i++){
		weights[i][0] = w[i+1][0];
	}

	// printf("\nREAL W Matrix\n");
	// print_matrix(weights, num_attributes, 1);

	// printf("\nFinal Matrix\n");
	solve_final(test_mat, w, num_test, num_attributes);

	return 0;
}


