#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>
using namespace std;

void keyboard() {
	// isinya code getch
}

struct pengguna {
    string nama;
    int pin;
    string username;
    string password;
    long long norek;
    long long saldo;
    bool isblokir;
};

struct akun {
    akun* next;
    pengguna data;
};

akun* head = nullptr;
akun* currentuser = nullptr;

long long rekeninggenerator() {
	long long newrek = (rand() % 9) + 1;
	for (int i = 0; i < 9; i++) {
		int digit = rand() % 10;
		newrek = (newrek * 10) + digit; 
	}
	return newrek;
}

void pendaftarannasabah() {
    akun* newakun = new akun;
    cout << "Masukkan Nama Nasabah : "; getline (cin, newakun->data.nama);
    cout << "Masukkan PIN : "; cin >> newakun->data.pin;
    cin.ignore(9999, '\n');
    cout << "Masukkan Username : "; getline (cin, newakun->data.username);
    cout << "Masukkan Password : "; cin >> newakun->data.password;
	newakun->data.norek = rekeninggenerator();
	cout << "Setoran Awal (Min. 50.000) : "; cin >> newakun->data.saldo;
	while (cin.fail() || newakun->data.saldo <= 50000) {
    if (cin.fail()) {
        cout << "Input tidak valid (Harus Angka)!" << endl;
        cin.clear();
        cin.ignore(9999, '\n');
    } else {
        cout << "Setoran Kurang! Minimal adalah 50.000." << endl;
    }
    cout << "Setoran Awal (Min. 50.000) : "; cin >> newakun->data.saldo;
	}
	newakun->data.isblokir = false;
    newakun->next = nullptr;

    if (head == nullptr) {
        head = newakun;
    } else {
        newakun->next = head;
        head = newakun;
    }
    cout << "Pendaftaran Berhasil! No Rekening: " << newakun->data.norek << endl;
}

void adminacc() {
    akun* admin = new akun;
    admin->data.nama = "Admin";
    admin->data.pin = 999;
    admin->data.username = "admin";
    admin->data.password = "admin123";
    admin->data.norek = 999;
	admin->next = head;
	head = admin; 
}

void menuadmin() {
    cout << "Pendaftaran Nasabah Baru";
    cin.get(); 
}

void login() {
	string usernameinput;
	string passwordinput;
	cout << "Masukkan Username : "; cin >> usernameinput;
	cout << "Masukkan Password : "; cin >> passwordinput;
	akun* temp = head;
	while (temp != nullptr) {
		if (temp->data.username == usernameinput && temp->data.password == passwordinput) {
			currentuser = temp;
			cout << "Selamat datang " << currentuser->data.username << endl;
			if (currentuser->data.username == "admin") {
				menuadmin();
			} else {
				cout << "hlo";
			}
			break;
		}
		temp = temp->next;
	}
}
int main () {
	srand(time(0));
	pendaftarannasabah();
    return 0;
}