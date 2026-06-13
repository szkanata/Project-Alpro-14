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

void signup() {
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

void adminacc() {
    akun* admin = new akun;
    admin->data.nama = "Admin";
    admin->data.pin = 999999;
    admin->data.username = "admin";
    admin->data.password = "admin123";
	admin->next = head;
	head = admin; 
}

void adminmenu() {
    cout << "Pendaftaran Nasabah Baru";
    cin.get(); 
    signup();
}

int main () {
	adminacc();
	string usernameinput;
	string passwordinput;
	cout << "Masukkan Username : "; cin >> usernameinput;
	cout << "Masukkan Password : "; cin >> passwordinput;
	akun* temp = head;
	while (temp != nullptr) {
		if (temp->data.username == usernameinput && passwordinput == passwordinput) {
			cout << "Selamat datang " << temp->data.username << endl;
			adminmenu();
			break;
		}
		temp = temp->next;
	}
    return 0;
}