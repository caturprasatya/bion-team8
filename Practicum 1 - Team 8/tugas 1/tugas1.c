#include <stdio.h>
#include <string.h>

// Define struct for storing item details
struct Media {
    char judul[50];
    int tahun_terbit;
    union {
        char buku[20];
        char majalah[20];
    }  tipe;
};
 
void tampilData(struct Media media, int isBook) {
    printf("\nTitle: %s\n", media.judul);
    printf("Year: %d\n", media.tahun_terbit);
    if (isBook) {
        printf("Tipe: Book\n");
        printf("Jenis Buku: %s\n", media.tipe.buku);
    } else {
        printf("Tipe: Majalah\n");
        printf("Jenis Majalah: %s\n", media.tipe.majalah);
    }
}

int main() {
    struct Media media;
    int n;
    
    printf("Masukkan Judul Buku: ");
    scanf(" %s", &media.judul);
    
    printf("Masukkan tahun_terbit buku:");
    scanf("%d", &media.tahun_terbit);

    printf("Masukkan jenis buku: (masukkan 1 untuk tipe buku dan 2 untuk tipe majalah) ");
    scanf("%d", &n);
    
    if (n == 1) {
        printf("Masukkan tipe buku: ");
        scanf(" %[^\n]", media.tipe.buku);
        tampilData(media, 1);
    } else if (n == 2) {
        printf("Masukkan tipe majalah: ");
        scanf(" %[^\n]", media.tipe.majalah);
        tampilData(media, 0);
    } else {
        printf("Invalid choice\n");
    }
    
    return 0;
}