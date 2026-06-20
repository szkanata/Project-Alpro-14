#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

const string RESET       = "\033[0m";
const string ALERT_RED   = "\033[31;1m";    
const string SUCCESS_GRN = "\033[32;1m";    
const string GOLD_GOLD   = "\033[33;1m";    
const string CYAN_LAUT   = "\033[36;1m";    
const string SUB_GRAY    = "\033[90m";      
const string TEXT_BLACK  = "\033[30;1m";
const string BLUE_PURE   = "\033[34;1m";

void loginAsUser();
void loginAsAdmin();
void subMenuAdmin(int idx);
void subMenuNasabah();
void pendaftarannasabah();
void transfersaldo();

void bersihkanLayar() {
    cout << "\033[2J\033[H"; 
}

void tungguEnter() {
    cout << "\n\n" << SUB_GRAY << "Tekan ENTER untuk melanjutkan..." << RESET;
    while (_getch() != '\r'); 
}

void buatSpasiMenu(int jumlah) {
    for (int i = 0; i < jumlah; i++) cout << "\n";
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

struct Admin {
    string username;
    string password;
    string namaAdmin;
};

struct akun {
    akun* next;
    pengguna data;
};

akun* head = nullptr;
akun* currentuser = nullptr;
int totalnasabah = 0;

Admin dataAdmin[2] = {
    {"admin1", "passadmin1", "Budi Santoso (Auditor)"},
    {"superadmin", "root123", "IT Core Developer"}
};

// Fungsi pembantu sementara untuk sensor password (bisa kamu kembangkan nanti)
string inputPasswordSensor() {
    string pass = "";
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') { // Backspace
            if (pass.length() > 0) {
                pass.pop_back();
                cout << "\b \b";
            }
        } else {
            pass.push_back(ch);
            cout << "*";
        }
    }
    cout << endl;
    return pass;
}

long long rekeninggenerator() {
    long long newrek = (rand() % 9) + 1;
    for (int i = 0; i < 9; i++) {
        int digit = rand() % 10;
        newrek = (newrek * 10) + digit; 
    }
    return newrek;
}

void pendaftarannasabah() {
    system("cls");
    akun* newakun = new akun;
    cout << BLUE_PURE << "=== PENDAFTARAN NASABAH BARU ===" << RESET << endl;
    
    cin.ignore(9999, '\n'); // Bersihkan sisa buffer sebelum getline
    cout << "Masukkan Nama Nasabah : "; getline(cin, newakun->data.nama);
    cout << "Masukkan PIN          : "; cin >> newakun->data.pin;
    
    cin.ignore(9999, '\n');
    cout << "Masukkan Username     : "; getline(cin, newakun->data.username);
    cout << "Masukkan Password     : "; cin >> newakun->data.password;
    
    cout << "Setoran Awal (Min. 50.000) : "; cin >> newakun->data.saldo;
    while (cin.fail() || newakun->data.saldo < 50000) {
        if (cin.fail()) {
            cout << ALERT_RED << "Input tidak valid (Harus Angka)!" << RESET << endl;
            cin.clear();
            cin.ignore(9999, '\n');
        } else {
            cout << ALERT_RED << "Setoran Kurang! Minimal adalah 50.000." << RESET << endl;
        }
        cout << "Setoran Awal (Min. 50.000) : "; cin >> newakun->data.saldo;
    }
    
    newakun->data.norek = rekeninggenerator();
    newakun->data.isblokir = false;
    newakun->next = nullptr;

    if (head == nullptr) {
        head = newakun;
    } else {
        newakun->next = head;
        head = newakun;
    }
    
    cout << SUCCESS_GRN << "\nPendaftaran Berhasil! No Rekening: " << newakun->data.norek << RESET << endl;
    totalnasabah++;
    tungguEnter();
}

void lihatdatanasabah() { cout << "--- FITUR LIHAT DATA NASABAH ---\n"; }
void kelolaakunnasabah() { cout << "--- FITUR KELOLA AKUN NASABAH ---\n"; }
void setortunai() { cout << "--- FITUR SETOR TUNAI ---\n"; }
void tariktunai() { cout << "--- FITUR TARIK TUNAI ---\n"; }
void riwayattransaksi() { cout << "--- FITUR RIWAYAT TRANSAKSI CLIENT ---\n"; }
void riwayattransaksinasabah() { cout << "--- FITUR RIWAYAT TRANSAKSI NASABAH (ADMIN) ---\n"; }

void menuUtama() {
    int kursor = 1;
    const int totalOpsi = 3;
    char tombol;

    while (true) {
        system("cls");
        cout << BLUE_PURE << "======================================" << RESET << "\n";
        cout << BLUE_PURE << "   BANK CENTRAL ASIA - MINI BANK CORE " << RESET << "\n";
        cout << BLUE_PURE << "======================================" << RESET << "\n";
        buatSpasiMenu(1);

        if (kursor == 1) 
            cout << SUCCESS_GRN << "> Login Sebagai Nasabah <" << RESET << "\n";
        else 
            cout << "  Login Sebagai Nasabah   \n";

        if (kursor == 2) 
            cout << SUCCESS_GRN << "> Login Sebagai Admin <" << RESET << "\n";
        else 
            cout << "  Login Sebagai Admin   \n";

        if (kursor == 3) 
            cout << ALERT_RED << "> Keluar Aplikasi <" << RESET << "\n";
        else 
            cout << "  Keluar Aplikasi   \n" << endl;
            
        cout << BLUE_PURE << "======================================" << RESET << "\n";
        cout << SUB_GRAY << "Navigasi: Panah Atas/Bawah | Enter untuk memilih" << RESET << "\n";
        
        tombol = _getch();
        if (tombol == 0 || tombol == -32 || (int)tombol == 224) {
            tombol = _getch();
            if (tombol == 72 || tombol == 73) kursor = (kursor == 1) ? totalOpsi : kursor - 1;
            else if (tombol == 80 || tombol == 81) kursor = (kursor == totalOpsi) ? 1 : kursor + 1;
        } 
        else if (tombol == '\r') { 
            if (kursor == 1) loginAsUser();
            else if (kursor == 2) loginAsAdmin();
            else if (kursor == 3) {
                system("cls");
                cout << SUCCESS_GRN << "[TERMINATED] Sesi mini bank ditutup. Terima kasih." << RESET << "\n\n";
                break;
            }
        }
    }
}

void loginAsAdmin() {
    system("cls"); 
    string usn;
    string pass;
    cout << BLUE_PURE << "======================================" << RESET << "\n";
    cout << BLUE_PURE << "   SECURITY ACCESS: VERIFIKASI ADMIN  " << RESET << "\n";
    cout << BLUE_PURE << "======================================" << RESET << "\n";
    cout << CYAN_LAUT << "Username ID Admin : " << RESET; cin >> usn;
    cout << CYAN_LAUT << "Sandi Pengawas    : " << RESET; pass = inputPasswordSensor(); 
    cout << BLUE_PURE << "======================================" << RESET << "\n";

    for (int i = 0; i < 2; i++) {
        if (dataAdmin[i].username == usn && dataAdmin[i].password == pass) {
            subMenuAdmin(i);
            return;
        }
    }
    cout << endl << ALERT_RED << "[Akses Ditolak] Kredensial Salah!" << RESET << "\n"; 
    tungguEnter();
}

void loginAsUser() {
    system("cls");
    string usn, pass;
    cout << BLUE_PURE << "======================================" << RESET << "\n";
    cout << BLUE_PURE << "         PORTAL OTENTIKASI NASABAH    " << RESET << "\n";
    cout << BLUE_PURE << "======================================" << RESET << "\n";
    cout << CYAN_LAUT << "Username Akun : " << RESET; cin >> usn;
    cout << CYAN_LAUT << "Password      : " << RESET; pass = inputPasswordSensor();
    cout << BLUE_PURE << "======================================" << RESET << "\n";
    
    akun* temp = head;
    while (temp != nullptr) {
        if (temp->data.username == usn && temp->data.password == pass) {
            if (temp->data.isblokir) {
                cout << ALERT_RED << "[STATUS BLOKIR] Akun Anda Ditangguhkan oleh Admin!" << RESET << "\n"; 
                tungguEnter();
                return;
            }
            currentuser = temp;
            subMenuNasabah();
            return;
        }
        temp = temp->next;
    }
    
    cout << "\n" << ALERT_RED << "[Gagal Auth] Kombinasi Username & Password Keliru!" << RESET << "\n"; 
    tungguEnter();
}

void subMenuAdmin(int idx) {
    int kursor = 1; 
    const int totalOpsi = 5; 
    char tombol;

    while (true) {
        system("cls");
        cout << BLUE_PURE << "======================================" << RESET << "\n";
        cout << BLUE_PURE << "      KONSOL OPERATOR SUPERVISOR      " << RESET << "\n";
        cout << BLUE_PURE << "======================================" << RESET << "\n";
        cout << SUB_GRAY << "Petugas Aktif: " << dataAdmin[idx].namaAdmin << RESET << "\n";
        
        if (kursor == 1) {
        cout << CYAN_LAUT << "> Pendaftaran Nasabah Baru <" << RESET << "\n";
        } else {
            cout << "Pendaftaran Nasabah Baru  \n";
        }

        if (kursor == 2) {
            cout << CYAN_LAUT << "> Lihat Data Nasabah <" << RESET << "\n";
        } else {
            cout << "Lihat Data Nasabah  \n";
        }

        if (kursor == 3) {
            cout <<CYAN_LAUT << "> Kelola Akun Nasabah <" << RESET << "\n";
        } else {
            cout << "Kelola Akun Nasabah  \n";
        }

        if (kursor == 4) {
            cout << CYAN_LAUT << "> Riwayat Transaksi Nasabah <" << RESET << "\n";
        } else {
            cout << "Riwayat Transaksi Nasabah  \n";
        }

        if (kursor == 5) {
            cout << ALERT_RED << "> Logout Sesi <" << RESET << "\n";
        } else {
            cout << "Logout Sesi  \n";
        }
        cout << BLUE_PURE << "======================================" << RESET << "\n";
        cout << SUB_GRAY << "Navigasi: Panah / PageUp / PageDown | Enter untuk memilih" << RESET << "\n";
        tombol = _getch();
        if (tombol == 0 || tombol == -32 || tombol == 224) {
		    tombol = _getch();
		    if (tombol == 72 || tombol == 73) {
		        if (kursor == 1) {
		            kursor = totalOpsi;
		        } else {
		            kursor = kursor - 1;
		        }
		    } 
		    else if (tombol == 80 || tombol == 81) {
		        if (kursor == totalOpsi) {
		            kursor = 1;
		        } else {
		            kursor = kursor + 1;
		        }
		    }
		} else if (tombol == '\r') {
			if (kursor == 1) {
				system("cls");
                pendaftarannasabah();
                tungguEnter();
            } 
            else if (kursor == 2) {
                system("cls");
                lihatdatanasabah();
                tungguEnter();
            }
            else if (kursor == 3) {
                system("cls");
                kelolaakunnasabah();
                tungguEnter();
            }
            else if (kursor == 4) {
                system("cls");
                riwayattransaksinasabah();
                tungguEnter();
            }
            else if (kursor == 5) {
                break;
            }
        }
    }
}

void subMenuNasabah() {
    int kursor = 1; 
    const int totalOpsi = 5; 
    char tombol;

    while (true) {
        system("cls");
        cout << BLUE_PURE << "======================================" << RESET << "\n";
        cout << BLUE_PURE << "      INTERFACES MINI BANK CLIENT     " << RESET << "\n"; 
        cout << BLUE_PURE << "======================================" << RESET << "\n";
        string namaNasabah = currentuser->data.nama;
        int s1 = 21 - namaNasabah.length(); if(s1 < 0) s1 = 0;
        cout << BLUE_PURE << "| " << TEXT_BLACK << "Nama Nasabah : " << RESET << namaNasabah << string(s1, ' ') << BLUE_PURE << " |" << RESET << "\n";
        
        string noRekString = to_string(currentuser->data.norek);
        int s2 = 21 - noRekString.length(); if(s2 < 0) s2 = 0;
        cout << BLUE_PURE << "| " << TEXT_BLACK << "No. Rekening : " << RESET << noRekString << string(s2, ' ') << BLUE_PURE << " |" << RESET << "\n";
        
        string stringSaldo = "Rp " + to_string(currentuser->data.saldo);
        int s3 = 21 - stringSaldo.length(); if(s3 < 0) s3 = 0;
        cout << BLUE_PURE << "| " << TEXT_BLACK << "Saldo Utama  : " << RESET << stringSaldo << string(s3, ' ') << BLUE_PURE << " |" << RESET << "\n";
        cout << BLUE_PURE << "======================================" << RESET << "\n";
        if (kursor == 1) {
            cout << SUCCESS_GRN << "> Setor Tunai <" << RESET << "\n";
        } else {
            cout << "Setor Tunai  \n";
        }

        if (kursor == 2) {
            cout << SUCCESS_GRN << "> Tarik Tunai <" << RESET << "\n";
    	} else {
            cout << "Tarik Tunai  \n";
		}
		
        if (kursor == 3) {
            cout << SUCCESS_GRN << "> Transfer Saldo <" << RESET << "\n";
        } else {
            cout << "Transfer Saldo  \n";
        }

        if (kursor == 4) {
            cout << SUCCESS_GRN << "> Riwayat Transaksi <" << RESET << "\n";
        } else {
            cout << "Riwayat Transaksi  \n";
        }

        if (kursor == 5) {
            cout << ALERT_RED << "> Logout / Selesai <" << RESET << "\n";
        } else {
            cout << "Logout / Selesai  \n";
        }
        cout << BLUE_PURE << "======================================" << RESET << "\n";
        cout << SUB_GRAY << "Navigasi: Panah / PageUp / PageDown | Enter untuk memilih" << RESET << "\n";
        tombol = _getch();
        if (tombol == 0 || tombol == -32 || tombol == 224) {
		    tombol = _getch();
		    if (tombol == 72 || tombol == 73) {
		        if (kursor == 1) {
		            kursor = totalOpsi;
		        } else {
		            kursor = kursor - 1;
		        }
		    } 
		    else if (tombol == 80 || tombol == 81) {
		        if (kursor == totalOpsi) {
		            kursor = 1;
		        } else {
		            kursor = kursor + 1;
		        }
		    } 
		}
        else if (tombol == '\r') {
            if (kursor == 5) {
                system("cls");
                cout << endl << SUCCESS_GRN << "[OK] Kartu dikeluarkan. Sesi aman berakhir." << RESET << "\n";
                tungguEnter();
                break;
            }
            if (kursor == 1) { 
            	system("cls");
                setortunai();
                tungguEnter();
            }
            else if (kursor == 2) { 
                system("cls");
                tariktunai();
                tungguEnter();
            }
            else if (kursor == 3) { 
                system("cls");
                transfersaldo();
                tungguEnter();
            }
            else if (kursor == 4) { 
                system("cls");
                riwayattransaksi();
                tungguEnter();
        	}
        }
    }
}
void transfersaldo() {
long long rekTujuan;
long long nominal;
char konfirmasi;

cout << BLUE_PURE
     << "======================================\n";
cout << "            TRANSFER SALDO            \n";
cout << "======================================"
     << RESET << endl;

cout << CYAN_LAUT
     << "Nomor Rekening Tujuan : "
     << RESET;
cin >> rekTujuan;

// Tidak boleh transfer ke rekening sendiri
if (rekTujuan == currentuser->data.norek) {
    cout << ALERT_RED
         << "\nTidak bisa transfer ke rekening sendiri!"
         << RESET << endl;
    return;
}

// Cari rekening tujuan
akun* penerima = head;

while (penerima != nullptr) {
    if (penerima->data.norek == rekTujuan) {
        break;
    }
    penerima = penerima->next;
}

// Rekening tidak ditemukan
if (penerima == nullptr) {
    cout << ALERT_RED
         << "\nNomor rekening tidak ditemukan!"
         << RESET << endl;
    return;
}

// Rekening diblokir
if (penerima->data.isblokir) {
    cout << ALERT_RED
         << "\nTransfer gagal! Rekening tujuan sedang diblokir."
         << RESET << endl;
    return;
}

cout << SUCCESS_GRN
     << "\nNama Penerima : "
     << RESET
     << penerima->data.nama << endl;

cout << CYAN_LAUT
     << "Nominal Transfer : Rp "
     << RESET;
cin >> nominal;

// Validasi nominal
if (nominal <= 0) {
    cout << ALERT_RED
         << "\nNominal transfer tidak valid!"
         << RESET << endl;
    return;
}

if (nominal < 10000) {
    cout << ALERT_RED
         << "\nMinimal transfer Rp 10.000!"
         << RESET << endl;
    return;
}

// Cek saldo
if (nominal > currentuser->data.saldo) {
    cout << ALERT_RED
         << "\nSaldo tidak mencukupi!"
         << RESET << endl;
    return;
}

// Konfirmasi transfer
cout << GOLD_GOLD
     << "\n========== KONFIRMASI =========="
     << RESET << endl;

cout << "Pengirim : "
     << currentuser->data.nama << endl;

cout << "Penerima : "
     << penerima->data.nama << endl;

cout << "Rekening : "
     << penerima->data.norek << endl;

cout << "Nominal  : Rp "
     << nominal << endl;

cout << "\nLanjutkan transfer? (Y/N) : ";
cin >> konfirmasi;

if (konfirmasi != 'Y' && konfirmasi != 'y') {
    cout << GOLD_GOLD
         << "\nTransfer dibatalkan."
         << RESET << endl;
    return;
}

// Proses transfer
currentuser->data.saldo -= nominal;
penerima->data.saldo += nominal;

cout << SUCCESS_GRN
     << "\n======================================\n";
cout << "         TRANSFER BERHASIL!           \n";
cout << "======================================"
     << RESET << endl;

cout << "Transfer Ke : "
     << penerima->data.nama << endl;

cout << "Nominal     : Rp "
     << nominal << endl;

cout << "Sisa Saldo  : Rp "
     << currentuser->data.saldo << endl;

}

int main () {
    srand(time(0));
    menuUtama(); // Pintu utama langsung menjalankan menu interaktif
    return 0;
}