#include <stdio.h>
#include <stdlib.h>

int getMedian(int arr[], int n) {
    // Sort the array
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }

    // Find the median
    if (n % 2 == 0) {
        return (arr[n/2] + arr[n/2-1]) / 2;
    } else {
        return arr[n/2];
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: %s <input file 1> <input file 2> ... <input file n> <output file>\n", argv[0]);
        return 1;
    }

    int n_rows, n_cols;
    FILE *fp = fopen(argv[1], "r");
    fscanf(fp, "P2\n%d %d\n255\n", &n_cols, &n_rows);
    fclose(fp);

    printf("File contains %d matrices with %d rows and %d columns.\n", argc-2, n_rows, n_cols);

    // Allocate memory for the input matrices
    int ***images = (int ***)malloc((argc-2) * sizeof(int **));
    for (int n = 0; n < argc-2; n++) {
        images[n] = (int **)malloc(n_rows * sizeof(int *));
        for (int r = 0; r < n_rows; r++) {
            images[n][r] = (int *)malloc(n_cols * sizeof(int));
        }
    }

    // Read the pixel values from the input matrices
    for (int n = 0; n < argc-2; n++) {
        FILE *fp = fopen(argv[n+1], "r");
        fscanf(fp, "P2\n%d %d\n255\n", &n_cols, &n_rows);
        for (int r = 0; r < n_rows; r++) {
            for (int c = 0; c < n_cols; c++) {
                int val;
                fscanf(fp, "%d", &val);
                images[n][r][c] = val;
            }
        }
        fclose(fp);
    }

    // Apply the median filter to the stack of matrices
    int **outputImage = (int **)malloc(n_rows * sizeof(int *));
    for (int r = 0; r < n_rows; r++) {
        outputImage[r] = (int *)malloc(n_cols * sizeof(int));
        for (int c = 0; c < n_cols; c++) {
            int *arr = (int *)malloc((argc-2) * sizeof(int));
            for (int n = 0; n < argc-2; n++) {
                arr[n] = images[n][r][c];
            }
            outputImage[r][c] = getMedian(arr, argc-2);
            free(arr);
        }
    }

    // Write the filtered output matrix to a file
    FILE *outfp = fopen(argv[argc-1], "w");
    fprintf(outfp, "P2\n%d %d\n255\n", n_cols, n_rows);
    for (int r = 0; r < n_rows; r++) {
        for (int c = 0; c < n_cols; c++) {
            fprintf(outfp, "%d ", outputImage[r][c]);
        }
        fprintf(outfp, "\n");
    }
    fclose(outfp);

    // Free allocated memory
    for (int n = 0; n < argc-2; n++) {
        for (int r = 0; r < n_rows; r++) {
            free(images[n][r]);
        }
        free(images[n]);
    }
    free(images);

    for (int r = 0; r < n_rows; r++) {
        free(outputImage[r]);
    }
    free(outputImage);

    return 0;
}