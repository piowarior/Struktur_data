#include <iostream>
#include <string>
using namespace std;

void printArray(string arr[], int size) {
    if (size > 0) {
        cout << "Tumpukan buku paling atas saat ini: " << arr[size - 1] << endl; 
    } else {
        cout << "Tumpukan buku kosong." << endl;
    }
    cout << endl;
}

int main() {
    int n = 0; // Penanda jumlah buku dalam tumpukan
    string arr[1000]; // Array untuk menyimpan buku

    int choice;
    do {
        cout << "Menu:\n";
        cout << "1. Push (Tambah buku)\n";
        cout << "2. Pop (Ambil buku paling atas)\n";
        cout << "3. Peek (Lihat buku paling atas)\n";
        cout << "4. isEmpty (Periksa apakah tumpukan kosong)\n";
        cout << "5. Size (Jumlah buku dalam tumpukan)\n";
        cout << "6. Exit\n";
        
        cout << "Masukkan pilihan: ";
        cin >> choice;
        cin.ignore();  // Mengabaikan karakter newline setelah memasukkan pilihan

        switch (choice) {
            case 1: {
                // Menambah buku ke dalam tumpukan
                if (n < 1000) { // Memastikan tidak melebihi batas array
                    cout << "Masukkan judul buku: ";
                    getline(cin, arr[n]);
                    n++;
                } else {
                    cout << "Tumpukan buku penuh.\n";
                }
                system("pause");
                system("cls");
                break;
            }
            case 2: {
                // Menghapus buku dari tumpukan
                if (n > 0) {
                    cout << "Mengambil buku paling atas: " << arr[n - 1] << endl;
                    n--; // Mengurangi jumlah buku
                } else {
                    cout << "Tumpukan buku kosong.\n";
                }
                system("pause");
                system("cls");
                break;
            }
            case 3: {
                // Menampilkan buku paling atas
                if (n > 0) {
                    printArray(arr, n);
                } else {
                    cout << "Tumpukan buku kosong.\n";
                }
                system("pause");
                system("cls");
                break;
            }
            case 4: {
                // Mengecek apakah tumpukan kosong
                if (n == 0) {
                    cout << "Tumpukan buku kosong.\n";
                } else {
                    cout << "Tumpukan buku tidak kosong.\n";
                }
                system("pause");
                system("cls");
                break;
            }
            case 5: {
                // Menampilkan jumlah buku dalam tumpukan
                cout << "Jumlah total buku dalam tumpukan saat ini: " << n << endl;
                system("pause");
                system("cls");
                break;
            }
            case 6: {
                // Keluar dari program
                cout << "Keluar dari program.\n";
                break;
            }
            default: {
                // Pesan untuk pilihan yang tidak valid
                cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                system("pause");
                system("cls");
                break;
            }
        }
    } while (choice != 6);

    return 0;
}