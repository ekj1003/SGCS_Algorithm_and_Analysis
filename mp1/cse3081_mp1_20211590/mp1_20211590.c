#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
 
int rows, cols;

int fun1(int **arr);
int fun2(int **arr);
int fun3(int **arr);


int main(int argc, char* argv[]) {

    FILE *input = NULL;
    input = fopen(argv[1], "r");
    if (argc != 3) {
        printf("Usage: %s <rows> <cols> <algorithm>\n", argv[0]);
        return 1;
    }


    int i, j, max;
    fscanf(input, "%d %d", &rows, &cols);

    int **arr = (int **)malloc(rows * sizeof(int *));
    for (i = 0; i < rows; i++){
        arr[i] = (int *)malloc(cols * sizeof(int));
    }

    for (i = 0; i < rows; i++){
        for (j = 0; j < cols; j++){
            fscanf(input, "%d", &arr[i][j]);
        }
    }

    fclose(input);

    int algo = *argv[2];
    clock_t start, end;
    long time;
    
    start = clock();
    if(algo == '1') max = fun1(arr);
    else if(algo == '2') max = fun2(arr);
    else if(algo == '3') max = fun3(arr);
    end = clock();
    time = end - start;


    char res[20];
    sprintf(res, "result_%s", argv[1]);
    FILE *output = fopen(res, "w");
    fprintf(output,"%s\n", argv[1]);
    fprintf(output,"%s\n", argv[2]);
    fprintf(output,"%d\n%d\n", rows, cols);
    fprintf(output,"%d\n", max);
    fprintf(output,"%ld\n", time);

}


int fun1(int **arr) {
    int currentSum, maxSum;
    int maxNegative, is_all_negative;
    int i, j, k, l, x, y;

    maxSum = arr[0][0];
    maxNegative = INT_MIN;
    is_all_negative = 1;

    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            for (k = i; k < rows; ++k) {
                for (int l = j; l < cols; ++l) {
                    currentSum = 0;
                    for (x = i; x <= k; ++x) {
                        for (y = j; y <= l; ++y) {
                            currentSum += arr[x][y];

                            if (arr[x][y] > maxNegative) {
                                maxNegative = arr[x][y];
                            }
                        }
                    }

                    if (currentSum > maxSum) maxSum = currentSum;
                    if (currentSum >= 0)is_all_negative = 0;
                }
            }
        }
    }

    if (is_all_negative) return maxNegative;
    else return maxSum;

}


int fun2(int **arr){
    int currentSum, maxSum;
    int maxNegative, is_all_negative;
    int i, j, k, l;
    int colSum[cols];

    maxSum = arr[0][0];
    maxNegative = INT_MIN;
    is_all_negative = 1;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            for (int k = 0; k < cols; ++k) {
                colSum[k] = 0;
            }

            for (int k = i; k < rows; ++k) {
                currentSum = 0;
                for (int l = j; l < cols; ++l) {
                    colSum[l] += arr[k][l];
                    currentSum += colSum[l];
                    if (currentSum > maxSum) maxSum = currentSum;
                    if (arr[k][l] > maxNegative) maxNegative = arr[k][l];
                    if (arr[k][l] >= 0) is_all_negative = 0; 
                    
                }
            }
        }
    }

    if (is_all_negative) return maxNegative;
    else return maxSum;
    
}


int fun3(int **arr) {
    int maxSum, currentSum;
    int maxNegative, is_all_negative;
    int i, j, k;

    maxSum = INT_MIN;
    maxNegative = INT_MIN;
    is_all_negative = 1;

    for (i = 0; i < cols; i++) {
        int rowSum[rows];
        for (j = 0; j < rows; j++) {
            rowSum[j] = 0;
        }

        for (j = i; j < cols; j++) {
            for (k = 0; k < rows; k++) {
                rowSum[k] += arr[k][j];
            }

            currentSum = 0;
            for (k = 0; k < rows; k++) {
                currentSum += rowSum[k];
                if (currentSum > maxSum) maxSum = currentSum;
                if (currentSum < 0) currentSum = 0;
                if (arr[k][j] > maxNegative) maxNegative = arr[k][j];
                if (arr[k][j] >= 0) is_all_negative = 0;

            }
        }
    }

    if (is_all_negative) return maxNegative;
    else return maxSum;
}
