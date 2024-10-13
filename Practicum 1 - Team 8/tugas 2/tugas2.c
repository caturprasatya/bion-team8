#include <stdio.h>

void sort(int arr[], int n) {
    int i, j, temp;
    // Looping dari 0 hingga n-1
    for (i = 0; i < n-1; i++) {
        // Looping dari i+1 hingga n
        for (j = i+1; j < n; j++) {
            // Jika elemen berikut lebih kecil, tukar
            if (arr[j] < arr[i]) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

int main() {
    int arr[] = {64, 34, 25, 12, 22};
    int n = sizeof(arr)/sizeof(arr[0]);
    sort(arr, n);
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    return 0;
}