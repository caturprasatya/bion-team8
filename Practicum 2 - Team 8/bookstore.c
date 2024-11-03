#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// ANSI Color codes untuk terminal
// Untuk memperindah output pada terminal, memberikan warna pada text output
#define RESET   "\x1B[0m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN    "\x1B[36m"
#define WHITE   "\x1B[37m"

#define MAX_BOOKS 100
#define MAX_STRING 100
#define MAX_CODE 5
#define CURRENCY "Rp"

typedef struct {
    char code[MAX_CODE];
    char name[MAX_STRING];
    char type[MAX_STRING];
    float price;
} Book;

typedef struct {
    char time[25];
    char codeBook[10];
    char nameBook[MAX_STRING];
    int quantity;
    float total;
} Transaction;

// Fungsi prototype
void clearScreen();
void printHeader(const char* title);
void printSeparator(char symbol);
void printMenu();
void waitForEnter();
char* formatCurrency(float amount);
int getNumberFromCode(const char* Code);
int validateInput(const char* prompt, int min, int max);

// Fungsi file handling
int readDataBook(Book *bookList);
void saveDataBook(const Book *bookList, int totalBooks);
void saveBookToFile(const Book *book);
void saveTransactionToFile(const Transaction* trans);
void getCurrentTime(char *time);

// Core functionality
void inputTransaction(Book *bookList, int totalBooks);
void addBook(Book *bookList, int *totalBooks);
// void view_history();

// Utilitas fungsi, membersihkan tampilan output
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Template header, untuk mengatasi redundant pada code
void printHeader(const char* title) {
    clearScreen();
    printSeparator('=');
    printf("%s%s%s\n", CYAN, title, RESET);
    printSeparator('=');
}

// Untuk memberikan batas pada menu atau tampilan output pada menu, kita sering menggunaka simbol "-", "=" dll.
// Karena proses itu cukup memakan waktu dan panjangnya harus sama, fungsi ini akan mempermudah kasus tersebut
void printSeparator(char symbol) {
    printf("%s", YELLOW);
    for(int i = 0; i < 12; i++) printf("%c", symbol);
    printf("%s\n", RESET);
}

// Pesan untuk kembali ke menu utama jika proses pada menu yang dipilih telah selesai dijalankan
void waitForEnter() {
    printf("\n%sTekan Enter untuk melanjutkan...%s", YELLOW, RESET);
    while(getchar() != '\n');
    getchar();
    clearScreen();
}

// Menuliskan nominal uang dengan mengikuti format mata uang Indonesia
char* formatCurrency(float amount) {
    static char formatted[32];
    sprintf(formatted, "%s %.2f", CURRENCY, amount);
    return formatted;
}

int validateInput(const char* prompt, int min, int max) {
    int input;
    char line[256];
    do {
        printf("%s", prompt);
        if (!fgets(line, sizeof(line), stdin)) {
            return min - 1;
        }
        if (sscanf(line, "%d", &input) != 1) {
            printf("%sInput tidak valid!%s\n", RED, RESET);
            continue;
        }
        if (input < min || input > max) {
            printf("%sInput harus antara %d dan %d!%s\n", RED, min, max, RESET);
            continue;
        }
        break;
    } while (1);
    return input;
}

// Fungsi untuk mendapatkan waktu saat ini
void getCurrentTime(char *times) {
    time_t now;
    struct tm *tm_info;

    time(&now);
    tm_info = localtime(&now);
    strftime(times, 25, "%Y-%m-%d %H:%M:%S", tm_info);
}

// Fungsi untuk mendapatkan nomor dari kode buku
int getNumberFromCode(const char* Code) {
    char number[4];
    strncpy(number, Code + 1, 3); // Mengambil 3 digit setelah 'B'
    number[3] = '\0';
    return atoi(number); // konversi string ke int, sekaligus juga mengembalikan nilai
}

// Fungsi untuk membuat kode buku baru
void createNewCode(const char* lastCode, char* newCode) {
    if (lastCode == NULL || lastCode[0] == '\0') {
        strncpy(newCode, "B001", MAX_CODE);
        return;
    }

    int number = getNumberFromCode(lastCode);
    sprintf(newCode, "B%03d", number + 1);
}

// Fungsi untuk membaca data buku dari file
int readDataBook(Book *bookList) {
    FILE *file = fopen("databuku.txt", "r");
    int totalBooks = 0;
    
    if (file == NULL) {
        printf("%sFile databuku.txt tidak ditemukan!%s\n", RED, RESET);
        return 0;
    }
    
    while (!feof(file) && totalBooks < MAX_BOOKS) {
        if (fscanf(file, "%[^,],%[^,],%[^,],%f\n", 
               bookList[totalBooks].code,
               bookList[totalBooks].name,
               bookList[totalBooks].type,
               &bookList[totalBooks].price) == 4) {
            totalBooks++;
        }
    }
    
    fclose(file);
    return totalBooks;
}

// Fungsi untuk menambahkan buku kedalam database(file databuku.txt)
void addBook(Book *bookList, int *totalBooks) {
    if (*totalBooks >= MAX_BOOKS) {
        printHeader("Penyimpanan Toko penuh!");
        return;
    }

    Book newBook;

    // Membuat kode buku otomatis
    if (totalBooks == 0) {
        createNewCode("", newBook.code);
    } else {
        createNewCode(bookList[*totalBooks-1].code, newBook.code);
    }

    printf("\n=== Input Data Buku ===\n");
    printf("Kode Buku: %s\n", newBook.code);

    // Input nama buku - pastikan buffer bersih
    while (getchar() != '\n'); // Membersihkan buffer dengan lebih aman
    printf("Nama Buku: ");
    if (fgets(newBook.name, sizeof(newBook.name), stdin) != NULL) {
        newBook.name[strcspn(newBook.name, "\n")] = 0;
    }
    // Input jenis buku
    printf("Jenis Buku: ");
    if (fgets(newBook.type, sizeof(newBook.type), stdin) != NULL) {
        newBook.type[strcspn(newBook.type, "\n")] = 0;
    }

    // Input harga buku
    do {
        printf("Harga Buku: ");
        if (scanf("%f", &newBook.price) != 1 || newBook.price <= 0) {
            printf("%sHarga tidak valid!%s\n", RED, RESET);
            while (getchar() != '\n');
            continue;
        }
        break;
    } while (1);

    // Menambahkan buku ke array
    bookList[*totalBooks] = newBook;
    (*totalBooks)++;

    // Simpan ke file
    saveBookToFile(&newBook);
    printf("Buku berhasil ditambahkan dengan kode: %s\n", newBook.code);
}

// Fungsi untuk menyimpan data buku ke file
void saveBookToFile(const Book* book) {
    FILE* file = fopen("databuku.txt", "a");
    if (file == NULL) {
        printf("Error: Tidak dapat membuka file!\n");
        return;
    }
    
    fprintf(file, "%s,%s,%s,%.2f\n", book->code, book->name, book->type, book->price);
    fclose(file);
}

// Core functionality implementation
void inputTransaction(Book *bookList, int totalBooks) {
    printHeader("INPUT TRANSAKSI");
    
    char codeBook[10];
    printf("%sMasukkan kode buku: %s", GREEN, RESET);
    scanf("%s", codeBook);
    getchar();  // Clear buffer
    
    for (int i = 0; i < totalBooks; i++) {
        if (strcmp(bookList[i].code, codeBook) == 0) {
            printf("\n%sDATA BUKU:%s\n", CYAN, RESET);
            printf("Nama: %s\n", bookList[i].name);
            printf("Harga: %s\n", formatCurrency(bookList[i].price));
            
            int quantity = validateInput("Masukkan jumlah buku: ", 1, 100);
            
            Transaction trans;

            getCurrentTime(trans.time);
            strcpy(trans.codeBook, bookList[i].code);
            strcpy(trans.nameBook, bookList[i].name);
            trans.quantity = quantity;
            trans.total = bookList[i].price * quantity;

            saveTransactionToFile(&trans);
            
            return;
        }
    }
    printf("%sBuku tidak ditemukan!%s\n", RED, RESET);
}

// Fungsi untuk menyimpan data history ke file
void saveTransactionToFile(const Transaction* trans) {
    FILE *file = fopen("history.txt", "a");
    if (file) {
        fprintf(file, "%s,%s,%s,%d,%.2f\n",
                trans->time, trans->codeBook, trans->nameBook,
                trans->quantity, trans->total);
        fclose(file);
        
        printf("\n%sTransaksi Berhasil!%s\n", GREEN, RESET);
        printf("Total pembayaran: %s%s%s\n", 
                GREEN, formatCurrency(trans->total), RESET);
    } else {
        printf("%sGagal menyimpan transaksi!%s\n", RED, RESET);
    }
}

void printMenu() {
    printf("\n%s===== APLIKASI TOKO BUKU =====%s\n", MAGENTA, RESET);
    printSeparator('-');
    printf("%s1.%s Input Buku\n", CYAN, RESET);
    printf("%s2.%s View History\n", CYAN, RESET);
    printf("%s3.%s View Buku\n", CYAN, RESET);
    printf("%s4.%s Delete History\n", CYAN, RESET);
    printf("%s5.%s Delete Buku\n", CYAN, RESET);
    printf("%s6.%s Exit\n", CYAN, RESET);
    printf("%s7.%s Input Transaksi\n", CYAN, RESET);
    printSeparator('-');
}

int main() {
    Book bookList[MAX_BOOKS];
    int totalBooks = readDataBook(bookList);
    int choice;
    
    do{
        printMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBook(bookList, &totalBooks); break;
            // case 2: view_history(); break;
            // Todo: Menambahkan menu yang belum ada
            // view_book;
            // delete_history;
            // delete_book;
            case 6:
                clearScreen();
                printHeader("TERIMA KASIH");
                printf("%sTerima kasih telah menggunakan aplikasi!%s\n\n", GREEN, RESET);
                return 0;
            // Menu 7 ini inisiatif dari kelompok kami, dikarenakan didalam soal tidak ada perintah untuk input trasaksi
            // memiliki kemungkinan tidak akan adanya update data transaksi terjadi.
            case 7: inputTransaction(bookList, totalBooks); break;
            default: 
                printf("Menu yang anda masukan tidak tersedia!!");
                break;
        }

        waitForEnter();
    } while (choice != 6);

    return 0;
}