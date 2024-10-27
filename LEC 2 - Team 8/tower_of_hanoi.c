#include <stdio.h>
#include <stdlib.h>

#define MAX_DISKS 10

// Structure to represent the pole
typedef struct {
    int disks[MAX_DISKS];
    int top;
    char name;
} Tower;

// Function for adding disk from the pole
void push(Tower *tower, int disk) {
    if (tower->top < MAX_DISKS - 1) {
        tower->disks[++(tower->top)] = disk;
    }
}

// Function for removing disk from the pole
int pop(Tower *tower) {
    if (tower->top >= 0) {
        return tower->disks[(tower->top)--];
    }
    return -1;
}

// Function for moving disks between poles
void moveDisk(Tower *source, Tower *destination) {
    int disk = pop(source);
    push(destination, disk);
    printf("Pindahkan disk %d dari tiang %c ke tiang %c\n", disk, source->name, destination->name);
}

// Recursive functions to complete Towers of Hanoi
void hanoi(int n, Tower *source, Tower *auxiliary, Tower *destination) {
    if (n == 1) {
        moveDisk(source, destination);
        return;
    }
    
    hanoi(n - 1, source, destination, auxiliary);
    moveDisk(source, destination);
    hanoi(n - 1, auxiliary, source, destination);
}


// Function to print tower state
void printTower(Tower *tower) {
    if (tower == NULL) return;
    
    printf("Tower %c: ", tower->name);
    if (tower == NULL || tower->top == -1) {
        printf("empty");
    } else {
        for (int i = 0; i <= tower->top; i++) {
            printf("%d ", tower->disks[i]);
        }
    }
    printf("\n");
}

int main() {
    // Not declaring disks, by default the value will be 0
    Tower towers[3] = {
        {
            .name = 'A',
            .top = 2,
            .disks = {3, 2, 1}
        },
        {
            .name = 'B',
            .top = -1,
        },
        {
            .name = 'C',
            .top = -1,
        }
    };
    int numDisks = 3;

    printf("Penyelesaian Towers of Hanoi untuk %d disk:\n", numDisks);
    printf("Inisialisasi tower:\n");
    printTower(&towers[0]);
    printf("------------------------------------------\n");
    hanoi(numDisks, &towers[0], &towers[1], &towers[2]);  // Changed order to match algorithm
    printf("------------------------------------------\n");
    printf("Final Step:\n");
    printTower(&towers[2]);
    printf("Selesai! Semua disk telah dipindahkan dari tiang A ke tiang C.\n");

    return 0;
}