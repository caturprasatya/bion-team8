#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// ANSI Color codes untuk terminal
// Untuk memperindah output pada terminal, memberikan warna pada text output
#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"

#define MAX_BOOKS 100
#define MAX_STRING 100
#define MAX_CODE 5
#define CURRENCY "Rp"

typedef struct
{
    char code[MAX_CODE];
    char name[MAX_STRING];
    char type[MAX_STRING];
    float price;
} Book;

typedef struct
{
    char time[25];
    char codeBook[10];
    char nameBook[MAX_STRING];
    int quantity;
    float total;
} Transaction;

// Fungsi prototype
void clearScreen();
void printHeader(const char *title);
void printSeparator(char symbol);
void printMenu();
void waitForEnter();
char *formatCurrency(float amount);
int getNumberFromCode(const char *Code);
int validateInput(const char *prompt, int min, int max);

// Fungsi file handling
int readDataBook(Book *bookList);
void saveDataBook(const Book *bookList, int totalBooks);
void saveBookToFile(const Book *book);
void saveTransactionToFile(const Transaction *trans);
void getCurrentTime(char *time);
void viewHistory(void);
void viewBooks(const Book *bookList, int totalBooks);
void deleteHistory(void);
void addBook(Book *bookList, int *totalBooks);
void inputTransaction(Book *bookList, int totalBooks);

/*
 * Fungsi clearScreen:
 * Membersihkan output pada layar konsol sehingga memberikan tampilan yang lebih rapi.
 *
 * Parameter:
 *   Tidak ada
 *
 * Return:
 *   Tidak ada
 */
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/*
 * Fungsi printHeader:
 * Mencetak header dengan format yang konsisten dan menghindari redundant pada kode
 *
 * Parameter:
 *   title - String judul yang akan ditampilkan
 *
 * Return:
 *   Tidak ada
 */
void printHeader(const char *title)
{
    clearScreen();
    printSeparator('=');
    printf("%s%s%s\n", CYAN, title, RESET);
    printSeparator('=');
}

/*
 * Fungsi printSeparator:
 * Untuk memberikan batas pada menu atau tampilan output dengan menggunakan simbol, penggunaan simbol seperti "-", "=",  dll.
 * Menggunakan warna kuning untuk pembatas dan mencetak sebanyak 12 karakter.
 *
 * Parameter:
 *   symbol - Karakter yang digunakan sebagai pembatas
 *
 * Return:
 *   Tidak ada
 */
void printSeparator(char symbol)
{
    printf("%s", YELLOW);
    for (int i = 0; i < 20; i++)
        printf("%c", symbol);
    printf("%s\n", RESET);
}

/*
 * Fungsi waitForEnter:
 * Fungsi yang akan memberi tahukan perintah untuk melanjutkan program(perpindah untuk memuat menu utama)
 *
 * Parameter:
 *   Tidak ada
 *
 * Return:
 *   Tidak ada
 */
void waitForEnter()
{
    printf("\n%sTekan Enter untuk melanjutkan...%s", YELLOW, RESET);
    while (getchar() != '\n')
        ;
    getchar();
    clearScreen();
}

// Menuliskan nominal uang dengan mengikuti format mata uang Indonesia
char *formatCurrency(float amount)
{
    static char formatted[32];
    sprintf(formatted, "%s %.2f", CURRENCY, amount);
    return formatted;
}

/*
 * Fungsi validateInput:
 * Karena dalam program ini terdapat berbagai macam input, dan agar program bisa berjalan dengan baik
 * tentu diperlukan validasi apakah data tersebut sesuai dengan kebutuhan pada program
 * Parameter:
 *   prompt - Pesan yang memperlihatkan input.
 *   min  - Minimum nilai yang ditentukan.
 *   max  - Maximum nilai yang ditentukan.
 *
 * Return:
 *   Nilai yang diinputkan.
 */
int validateInput(const char *prompt, int min, int max)
{
    int input;
    char line[256];
    do
    {
        printf("%s", prompt);
        if (!fgets(line, sizeof(line), stdin))
        {
            return min - 1;
        }
        if (sscanf(line, "%d", &input) != 1)
        {
            printf("%sInput tidak valid!%s\n", RED, RESET);
            continue;
        }
        if (input < min || input > max)
        {
            printf("%sInput harus antara %d dan %d!%s\n", RED, min, max, RESET);
            continue;
        }
        break;
    } while (1);
    return input;
}

/*
 * Fungsi waitForEnter:
 * Fungsi untuk mendapatkan waktu saat ini
 *
 * Parameter:
 *   times - pointer ke array char untuk menyimpan string waktu
 *   (harus memiliki ukuran minimal 25 karakter)
 *
 * Return:
 *   Tidak ada
 */
void getCurrentTime(char *times)
{
    time_t now;
    struct tm *tm_info;

    time(&now);
    tm_info = localtime(&now);
    strftime(times, 25, "%Y-%m-%d %H:%M:%S", tm_info);
}

/*
 * Fungsi getNumberFromCode:
 * Mengekstrak dan mengkonversi bagian numerik dari kode buku.
 * Parameter:
 *   code - pointer ke string kode buku yang akan diekstrak
 *
 * Return:
 *   Integer hasil konversi dari bagian numerik kode
 */
int getNumberFromCode(const char *code)
{
    char number[4];
    strncpy(number, code + 1, 3); // Mengambil 3 digit setelah 'B'
    number[3] = '\0';
    return atoi(number); // konversi string ke int, sekaligus juga mengembalikan nilai
}

/*
 * Fungsi createNewCode:
 * Membuat kode buku baru berdasarkan kode terakhir.
 *
 * Parameter:
 *   lastCode - pointer ke string kode terakhir yang ada
 *   newCode - pointer ke buffer untuk menyimpan kode baru
 *
 * Return:
 *   Tidak ada
 */
void createNewCode(const char *lastCode, char *newCode)
{
    // Kondisi apabila lastcode bertipe null atau array char kosong, akan membuat kode "B001"
    if (lastCode == NULL || lastCode[0] == '\0')
    {
        strncpy(newCode, "B001", MAX_CODE);
        return;
    }

    int number = getNumberFromCode(lastCode);
    // Increament 1, setiap menambah data baru
    sprintf(newCode, "B%03d", number + 1);
}

/*
 * Fungsi readDataBook:
 * Membaca data buku dari file databuku.txt.
 *
 * Parameter:
 *   bookList - pointer ke array struct Book untuk menyimpan data
 *
 * Return:
 *   Jumlah buku yang berhasil dibaca dari file
 */
int readDataBook(Book *bookList)
{
    FILE *file = fopen("databuku.txt", "r");
    int totalBooks = 0;

    if (file == NULL)
    {
        printf("%sFile databuku.txt tidak ditemukan!%s\n", RED, RESET);
        return 0;
    }

    while (!feof(file) && totalBooks < MAX_BOOKS)
    {
        if (fscanf(file, "%[^,],%[^,],%[^,],%f\n",
                   bookList[totalBooks].code,
                   bookList[totalBooks].name,
                   bookList[totalBooks].type,
                   &bookList[totalBooks].price) == 4)
        {
            totalBooks++;
        }
    }

    fclose(file);
    return totalBooks;
}


/*
 * Fungsi viewBook:
 * Menampilkan data buku dari file databuku.txt.
 *
 * Parameter:
 *   bookList - pointer ke array struct Book
 *   totalBooks - pointer ke integer jumlah total buku
 *
 * Return:
 *   Tidak ada
 */
void viewBooks(const Book *bookList, int totalBooks) {
    printHeader("DAFTAR BUKU");
    
    if (totalBooks == 0) {
        printf("%sTidak ada buku dalam database%s\n", RED, RESET);
        return;
    }

    printf("\n%-8s %-30s %-20s %s\n", "Kode", "Nama", "Tipe", "Harga");
    printSeparator('-');

    for (int i = 0; i < totalBooks; i++) {
        printf("%-8s %-30s %-20s %s\n",
               bookList[i].code,
               bookList[i].name,
               bookList[i].type,
               formatCurrency(bookList[i].price));
    }
}

/*
 * Fungsi addBook:
 * Menambahkan data buku baru ke dalam sistem dan file(database).
 * Parameter:
 *   bookList - pointer ke array struct Book
 *   totalBooks - pointer ke integer jumlah total buku
 *
 * Return:
 *   Tidak ada
 */
void addBook(Book *bookList, int *totalBooks)
{
    // Kondisi untuk mengetahui batas maksimal buku
    if (*totalBooks >= MAX_BOOKS)
    {
        printHeader("Penyimpanan Toko penuh!");
        return;
    }

    Book newBook;

    // Membuat kode buku otomatis
    if (totalBooks == 0)
    {
        createNewCode("", newBook.code);
    }
    else
    {
        createNewCode(bookList[*totalBooks - 1].code, newBook.code);
    }

    printf("\n=== Input Data Buku ===\n");
    printf("Kode Buku: %s\n", newBook.code);

    // Input nama buku - pastikan buffer bersih
    while (getchar() != '\n'); // Membersihkan buffer dengan lebih aman
    printf("Nama Buku: ");
    if (fgets(newBook.name, sizeof(newBook.name), stdin) != NULL)
    {
        newBook.name[strcspn(newBook.name, "\n")] = 0;
    }
    // Input jenis buku
    printf("Jenis Buku: ");
    if (fgets(newBook.type, sizeof(newBook.type), stdin) != NULL)
    {
        newBook.type[strcspn(newBook.type, "\n")] = 0;
    }

    // Input harga buku
    do
    {
        printf("Harga Buku: ");
        if (scanf("%f", &newBook.price) != 1 || newBook.price <= 0)
        {
            printf("%sHarga tidak valid!%s\n", RED, RESET);
            while (getchar() != '\n')
                ;
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

/*
 * Fungsi saveBookToFile:
 * Menyimpan data buku spesific ke dalam file databuku.txt.
 *
 * Parameter:
 *   book - pointer ke struct Book yang akan disimpan
 *
 * Return:
 *   Tidak ada
 */
void saveBookToFile(const Book *book)
{
    // Dalam membaca file databuku, disini menggunakan mode a (Append) dalam kasus ini ingin menambahkan data pada line terakhir
    FILE *file = fopen("databuku.txt", "a");
    if (file == NULL)
    {
        printf("Error: Tidak dapat membuka file!\n");
        return;
    }

    fprintf(file, "%s,%s,%s,%.2f\n", book->code, book->name, book->type, book->price);
    fclose(file);
}

/*
 * Fungsi deleteBook:
 * Menghapus buku dari sistem dan memperbarui file databuku.txt
 * 
 * Parameter:
 *   bookList - pointer ke array struct Book
 *   totalBooks - pointer ke integer jumlah total buku
 * 
 * Return:
 *   Tidak ada
 */
void deleteBook(Book *bookList, int *totalBooks) {
    printHeader("HAPUS BUKU");
    
    if (*totalBooks == 0) {
        printf("%sTidak ada buku dalam database!%s\n", RED, RESET);
        return;
    }

    // Menampilkan daftar buku yang tersedia
    viewBooks(bookList, *totalBooks);

    char codeBook[MAX_CODE];
    printf("\n%sMasukkan kode buku yang akan dihapus: %s", GREEN, RESET);
    scanf("%s", codeBook);
    getchar(); // Clear buffer

    // Mencari buku yang akan dihapus
    int indexToDelete = -1;
    for (int i = 0; i < *totalBooks; i++) {
        if (strcmp(bookList[i].code, codeBook) == 0) {
            indexToDelete = i;
            break;
        }
    }

    if (indexToDelete == -1) {
        printf("%sBuku dengan kode %s tidak ditemukan!%s\n", RED, codeBook, RESET);
        return;
    }

    // Konfirmasi penghapusan
    printf("\nAnda akan menghapus buku:\n");
    printf("Kode  : %s\n", bookList[indexToDelete].code);
    printf("Nama  : %s\n", bookList[indexToDelete].name);
    printf("Jenis : %s\n", bookList[indexToDelete].type);
    printf("Harga : %s\n", formatCurrency(bookList[indexToDelete].price));

    printf("\n%sApakah anda yakin ingin menghapus buku ini? (y/n): %s", YELLOW, RESET);
    char confirmation;
    scanf("%c", &confirmation);
    getchar(); // Clear buffer

    if (tolower(confirmation) != 'y') {
        printf("%sPenghapusan dibatalkan%s\n", YELLOW, RESET);
        return;
    }

    // Menggeser semua elemen setelah index yang dihapus
    for (int i = indexToDelete; i < *totalBooks - 1; i++) {
        bookList[i] = bookList[i + 1];
    }
    (*totalBooks)--;

    // Memperbarui file databuku.txt
    FILE *file = fopen("databuku.txt", "w");
    if (file == NULL) {
        printf("%sError: Tidak dapat membuka file!%s\n", RED, RESET);
        return;
    }

    // Menulis ulang semua data ke file
    for (int i = 0; i < *totalBooks; i++) {
        fprintf(file, "%s,%s,%s,%.2f\n",
                bookList[i].code,
                bookList[i].name,
                bookList[i].type,
                bookList[i].price);
    }
    fclose(file);

    printf("%sBuku berhasil dihapus!%s\n", GREEN, RESET);
}

/*
 * Fungsi viewHistory:
 * Menampilkan history Transaksi.
 * Parameter:
 *   Tidak ada
 * Return:
 *   Tidak ada
 */
 void viewHistory()
{
    FILE *file = fopen("history.txt", "r");
    if (file == NULL)
    {
        printf("%sBelum terdapat transaksi!  (file history.txt not found)%s\n", RED, RESET);
        return;
    }

    char line[256];
    printf("\n%s===== RIWAYAT TRANSAKSI =====%s\n", CYAN, RESET);
    printSeparator('-');
    printf("%-20s %-10s %-30s %-10s %-10s\n", "Waktu", "Kode Buku", "Nama Buku", "Jumlah", "Total");
    printSeparator('-');

    while (fgets(line, sizeof(line), file))
    {
        char time[25], codeBook[10], nameBook[MAX_STRING];
        int quantity;
        float total;

        // sscanf untuk memiliah data dari line
        sscanf(line, "%[^,],%[^,],%[^,],%d,%f", time, codeBook, nameBook, &quantity, &total);

        // mnampilkan data yang telah dipilah
        printf("%-20s %-10s %-30s %-10d %-10s\n", time, codeBook, nameBook, quantity, formatCurrency(total));
    }

    fclose(file);
    printSeparator('-');
}

/*
 * Fungsi deleteHistory:
 * Menghapus history Transaksi berdasarkan index.
 * Parameter:
 *   Tidak ada
 * Return:
 *   Tidak ada
 */void deleteHistory()
{
    FILE *file = fopen("history.txt", "r");
    if (file == NULL)
    {
        printf("%sBelum terdapat transaksi!  (file history.txt not found)%s\n", RED, RESET);
        return;
    }

    // membaca semua history ke dalam array
    char lines[100][256]; // bisa disesuaikan sesuai kebutuhan
    int count = 0;

    while (fgets(lines[count], sizeof(lines[count]), file) && count < 100)
    {
        count++;
    }
    fclose(file);

    // untuk menampilkan history
    printf("\n%s===== RIWAYAT TRANSAKSI =====%s\n", CYAN, RESET);
    printSeparator('-');
    printf("%-3s %-20s %-10s %-30s %-10s %-10s\n", "No", "Waktu", "Kode Buku", "Nama Buku", "Jumlah", "Total");
    printSeparator('-');

    for (int i = 0; i < count; i++)
    {
        char time[25], codeBook[10], nameBook[MAX_STRING];
        int quantity;
        float total;

        sscanf(lines[i], "%[^,],%[^,],%[^,],%d,%f", time, codeBook, nameBook, &quantity, &total);
        printf("%-3d %-20s %-10s %-30s %-10d %-10s\n", i + 1, time, codeBook, nameBook, quantity, formatCurrency(total));
    }

    // Meminta input index untuk dihapus
    char input[10];
    int index = 0;
    while (1)
    {
        printf("Nomor index yang dihapus: ");
        fgets(input, sizeof(input), stdin);

        // Cek apakah input kosong
        if (input[0] == '\n')
        {
            printf("%sInput tidak valid!%s\n", RED, RESET);
            continue;
        }

        // mengonversi input string menjadi integer
        index = atoi(input);

        // validasi input
        if (index >= 1 && index <= count)
        {
            break; // Input valid
        }
        else
        {
            printf("%sInput tidak valid!%s\n", RED, RESET);
        }
    }

    // menghapus data sesuai index yang dipilih
    file = fopen("history.txt", "w"); // Membuka file untuk menulis ulang
    for (int i = 0; i < count; i++)
    {
        if (i != index - 1)
        { // jika bukan index yang dihapus
            fprintf(file, "%s", lines[i]);
        }
    }
    fclose(file);

    printf("%sData berhasil dihapus%s\n", GREEN, RESET);
}

/*
 * Fungsi inputTransaction:
 * Memproses transaksi pembelian buku.
 * Mencari buku berdasarkan kode dan menghitung total pembelian.
 *
 * Parameter:
 *   bookList - pointer ke array struct Book
 *   totalBooks - jumlah total buku dalam sistem
 *
 * Return:
 *   Tidak ada
 */
void inputTransaction(Book *bookList, int totalBooks)
{
    printHeader("INPUT TRANSAKSI");

    char codeBook[10];
    printf("%sMasukkan kode buku: %s", GREEN, RESET);
    scanf("%s", codeBook);
    getchar(); // Clear buffer

    for (int i = 0; i < totalBooks; i++)
    {
        if (strcmp(bookList[i].code, codeBook) == 0)
        {
            printf("\n%sDATA BUKU:%s\n", CYAN, RESET);
            printf("Nama: %s\n", bookList[i].name);
            printf("Harga: %s\n", formatCurrency(bookList[i].price));

            // Input jumlah buku, dan validasi input diantar batas yang ditentukan
            int quantity = validateInput("Masukkan jumlah buku: ", 1, 100);

            Transaction trans;

            // Untuk mendapatkan waktu saat ini
            getCurrentTime(trans.time);
            strcpy(trans.codeBook, bookList[i].code);
            strcpy(trans.nameBook, bookList[i].name);
            trans.quantity = quantity;
            trans.total = bookList[i].price * quantity;

            // Menyimpan transaksi kedalam file
            saveTransactionToFile(&trans);

            return;
        }
    }

    printf("%sBuku tidak ditemukan!%s\n", RED, RESET);
}

/*
 * Fungsi saveTransactionToFile:
 * Menyimpan data transaksi ke dalam file history.txt.
 *
 * Parameter:
 *   trans - pointer ke struct Transaction yang akan disimpan
 *
 * Return:
 *   Tidak ada
 */
void saveTransactionToFile(const Transaction *trans)
{
    // Dalam membaca file history, disini menggunakan mode a (Append) dalam kasus ini ingin menambahkan data pada line terakhir
    FILE *file = fopen("history.txt", "a");
    if (file)
    {
        fprintf(file, "%s,%s,%s,%d,%.2f\n",
                trans->time, trans->codeBook, trans->nameBook,
                trans->quantity, trans->total);
        fclose(file);

        printf("\n%sTransaksi Berhasil!%s\n", GREEN, RESET);
        printf("Total pembayaran: %s%s%s\n",
               GREEN, formatCurrency(trans->total), RESET);
    }
    else
    {
        printf("%sGagal menyimpan transaksi!%s\n", RED, RESET);
    }
}

/*
 * Fungsi printMenu:
 * Mempermudah dalam dalam membaca dan maintenance kode
 *
 * Parameter:
 *   trans - pointer ke struct Transaction yang akan disimpan
 *
 * Return:
 *   Tidak ada
 */
void printMenu()
{
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

int main()
{
    Book bookList[MAX_BOOKS];
    int totalBooks = readDataBook(bookList);
    int choice;

    do
    {
        printMenu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addBook(bookList, &totalBooks);
            break;
        case 2:
            viewHistory();
            break;
        case 3:
            viewBooks(bookList, totalBooks);
            break;
        case 4:
            deleteHistory();
            break;
        case 5:
            deleteBook(bookList, &totalBooks);
            break;
        case 6:
            clearScreen();
            printHeader("TERIMA KASIH");
            printf("%sTerima kasih telah menggunakan aplikasi!%s\n\n", GREEN, RESET);
            return 0;
        // Menu 7 ini inisiatif dari kelompok kami, dikarenakan didalam soal tidak ada perintah untuk input transaksi
        // Terdapat kemungkinan tidak akan adanya update data transaksi terjadi.
        case 7:
            inputTransaction(bookList, totalBooks);
            break;
        default:
            printf("Menu yang anda masukan tidak tersedia!!");
            break;
        }

        waitForEnter();
    } while (choice != 6);

    return 0;
}