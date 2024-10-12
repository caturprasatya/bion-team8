#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_EMPLOYEE 100
#define MAX_NIP 12

enum WorkingClass { D3, D2, D1 };

typedef struct {
    char nip[MAX_NIP];
    char name[30];
    char address[100];
    char phoneNumber[15];
    char position[50];
    enum WorkingClass workingClass;
    int basicSalary;
} Employee;

int totalEmployee = 5;

Employee employees[MAX_EMPLOYEE] = {
    {
        .nip = "NIP001",
        .name = "John Doe",
        .address = "123 Main St, City",
        .phoneNumber = "081234567890",
        .position = "Manager",
        .workingClass = D1,
        .basicSalary = 2500000
    },
    {
        .nip = "NIP002",
        .name = "Jane Smith",
        .address = "456 Elm St, Town",
        .phoneNumber = "082345678901",
        .position = "Supervisor",
        .workingClass = D2,
        .basicSalary = 2000000
    },
    {
        .nip = "NIP003",
        .name = "Bob Johnson",
        .address = "789 Oak St, Village",
        .phoneNumber = "083456789012",
        .position = "Staff",
        .workingClass = D3,
        .basicSalary = 1500000
    },
    {
        .nip = "NIP004",
        .name = "Alice Brown",
        .address = "101 Pine St, County",
        .phoneNumber = "084567890123",
        .position = "Analyst",
        .workingClass = D2,
        .basicSalary = 2000000
    },
    {
        .nip = "NIP005",
        .name = "Charlie Wilson",
        .address = "202 Maple St, District",
        .phoneNumber = "085678901234",
        .position = "Engineer",
        .workingClass = D1,
        .basicSalary = 2500000
    }
};

void viewEmployeeProfile(const char* nipEmployee) {
    for (int i = 0; i < totalEmployee; i++) {
        if (strcmp(employees[i].nip, nipEmployee) == 0) {
            printf("----------------------------------------------------\n");
            printf("Nama Karyawan: %s\n", employees[i].name);
            printf("NIP Karyawan: %s\n", employees[i].nip);
            printf("Alamat Karyawan: %s\n", employees[i].address);
            printf("Nomer Telepon Karyawan: %s\n", employees[i].phoneNumber);
            printf("Jabatan Karyawan: %s\n", employees[i].position);
            printf("Gaji Karyawan: Rp %d\n", employees[i].basicSalary);
            printf("----------------------------------------------------\n");
            return;
        }
    }
    printf("NIP karyawan tidak ditemukan\n");
}

void calculateOvertimePay(const char* nip, enum WorkingClass workingClass, int totalOvertime) {
    int overtimePerHour;
    switch (workingClass){
        case D1:
            overtimePerHour = 10000;
            break;
        case D2:
            overtimePerHour = 5000;
            break;
        case D3:
            overtimePerHour = 2500;
            break;
        default:
            printf("Input data tidak valid!!!");
    }
    
    
    for (int i = 0; i < totalEmployee; i++) {
        if (strcmp(employees[i].nip, nip) == 0) {
            int totalSalary = employees[i].basicSalary + (totalOvertime * overtimePerHour);
            printf("\nHasil Perhitungan Gaji:\n");
            printf("NIP: %s\n", employees[i].nip);
            printf("Golongan: ");
            switch (employees[i].workingClass) {
                case D1: printf("D1\n"); break;
                case D2: printf("D2\n"); break;
                case D3: printf("D3\n"); break;
            }
            printf("Lembur: %d jam\n", totalOvertime);
            printf("Total Gaji Bulan ini: Rp%d\n", totalSalary);
            return;
        }
    }
    printf("Data pegawai tidak ditemukan.\n");
}

int main() {
    int choice;
    char nip[MAX_NIP];
    int totalOvertime;
    enum WorkingClass workingClass;
    
    do {
        printf("\n=== Menu ===\n");
        printf("1. Lihat Profil Karyawan\n");
        printf("2. Hitung Uang Lembur Karyawan\n");
        printf("3. Tutup Program\n");
        printf("Pilih menu 1-3: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                printf("Masukan NIP: ");
                scanf("%s", nip);
                viewEmployeeProfile(nip);
                break;
            case 2:
                printf("Masukan NIP: ");
                scanf("%s", nip);
                printf("Terdapat 3 Golongan:\n");
                printf("1. D1\n");
                printf("2. D2\n");
                printf("3. D3\n");
                printf("Masukan Golongan (1-3): ");
                int groupChoice;
                scanf("%d", &groupChoice);
                switch(groupChoice) {
                    case 1: workingClass = D1; break;
                    case 2: workingClass = D2; break;
                    case 3: workingClass = D3; break;
                    default:
                        printf("Pilihan tidak valid!\n");
                        continue;
                }
                printf("Jumlah jam lembur: ");
                scanf("%d", &totalOvertime);
                calculateOvertimePay(nip, workingClass, totalOvertime);
                break;
            case 3:
                printf("Program ditutup. Terima kasih!\n");
                break;
            default:
                printf("Pilihan tidak valid!\n");
        }
    } while(choice != 3);

    return 0;
}