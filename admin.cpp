#include <iostream>
#include <conio.h>
using namespace std;

struct pengguna {
    string nama;
    int pin;
    string username;
    string password;
};

struct akun {
    akun* next;
    pengguna data;
};

akun* head = nullptr;

void signup () {
    akun* newakun = new akun;
    cout << "Masukkan Nama Nasabah : "; cin >> newakun->data.nama;
    cout << "Masukkan PIN : "; cin >> newakun->data.pin;
    cout << "Masukkan Username : "; cin >> newakun->data.username;
    cout << "Masukkan Password : "; cin >> newakun->data.password;

    newakun->next = nullptr;

    if (head == nullptr) {
        head = newakun;
    } else {
        newakun->next = head;
        head = newakun;
    }
}

void admin () {
    cout << "Pendaftaran Nasabah Baru";
    cin.get(); 
    signup();
    return 0;
}