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
const string NAVY_BLUE = "\033[38;2;5;83;250m";

const int MAX_MUTASI = 30;
string pusat_layar = string(55, ' ');

void headmenuutama () {
    cout << "==================================================================================================================================================" << endl;
    cout << R"(|  )" << NAVY_BLUE << R"( ___   _   _  _ _  _______   ________ ___  )" << RESET << R"(       )" << NAVY_BLUE << R"(__  __ ___ _  _ ___   ___   _   _  _ _  _____ _  _  ___   _____   _____ _________ ___  __  )"<< RESET <<" |" << endl;
    cout << R"(|  )" << NAVY_BLUE << R"(| _ ) /_\ | \| | |/ / _ ) \ /  /_   _| __| )" << RESET << R"( ___  )" << NAVY_BLUE << R"(|  \/  |_ _| \| |_ _| | _ ) /_\ | \| | |/ /_ _| \| |/ __| / __\ \ / / __|_   __| __|  \/  | )"<< RESET <<" |" << endl;
    cout << R"(|  )" << NAVY_BLUE << R"(| _ \/ _ \| .` | ' <| _ \ \ V /  | | | _|  )" << RESET << R"(|___| )" << NAVY_BLUE << R"(| |\/| || || .` || |  | _ \/ _ \| .` | ' < | || .` | (_ | \__ \ \ V /\__ \ | | | _|| |\/| | )"<< RESET <<" |" << endl;
    cout << R"(|  )" << NAVY_BLUE << R"(|___/_/ \_\_|\_|_|\_\___/  |_|   |_| |___| )" << RESET << R"(      )" << NAVY_BLUE << R"(|_|  |_|___|_|\_|___| |___/_/ \_\_|\_|_|\_\___|_|\_|\___| |___/  |_| |___/ |_| |___|_|  |_| )"<< RESET <<" |" << endl;
    cout << "==================================================================================================================================================" << endl;
}

struct Transaksi {
    string tipe; 
    string detail;
    long long jumlah; 
};

struct pengguna {
    string nama;
    int pin; 
    string username;
    string password;
    long long norek;
    long long saldo;
    bool isblokir;
    Transaksi riwayat[MAX_MUTASI];
    int totalMutasi = 0;
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

pengguna dummyNasabah[2] = {
    {"Lintang", 123456, "lintang", "lintang123", 1234567890, 750000, false, {}, 0},
    {"Suci", 654321, "suci", "suci123", 9876543210, 1200000, false, {}, 0}
};

// Prototype fungsi
void loginAsUser();
void loginAsAdmin();
void subMenuAdmin(int idx);
void subMenuNasabah();
void pendaftarannasabah();
void transfersaldo();
void tariktunai(pengguna* ptrUser);
void setortunai();
void riwayattransaksi();
void riwayattransaksinasabah();
void lihatdatanasabah();
void kelolaakunnasabah();
void catatLogTransaksi(pengguna* n, string tipe, string detail, long long nominal);
akun* carinasabahByUsk(string usn);
akun* carinasabahByRek(long long rek);
bool apakahUserSudahAda(string usn, long long rek);

void bersihkanLayar() {
    system("cls"); 
}

void tungguEnter() {
    cout << "\n\n" << SUB_GRAY << "Tekan ENTER untuk melanjutkan..." << RESET;
    while (_getch() != '\r'); 
}

void buatSpasiMenu(int jumlah) {
    for (int i = 0; i < jumlah; i++) cout << "\n";
}

void catatLogTransaksi(pengguna* n, string tipe, string detail, long long nominal) {
    int pos = n->totalMutasi;
    if (pos < MAX_MUTASI) {
        n->riwayat[pos] = {tipe, detail, nominal};
        n->totalMutasi++;
    }
}

// Fungsi pembantu untuk mengambil input bertipe string dengan sensor bintang (*)
string inputPinTranskasiSensor() {
    string pinStr = "";
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') { 
            if (pinStr.length() > 0) {
                pinStr.pop_back();
                cout << "\b \b";
            }
        } else if (ch >= '0' && ch <= '9') { // Batasi hanya menerima angka
            pinStr.push_back(ch);
            cout << "*";
        }
    }
    cout << endl;
    return pinStr;
}

// Validasi PIN Transaksi terenkripsi bintang (*)
bool verifikasiPinTransaksi(int pinAsli) {
    string inputStr;
    
    while (true) {
        cout << CYAN_LAUT << "Masukkan 6-Digit PIN Anda : " << RESET;
        inputStr = inputPinTranskasiSensor();
        
        // 1. Validasi jika nasabah langsung menekan Enter (string kosong)
        if (inputStr.empty()) {
            cout << ALERT_RED << "[Error] PIN tidak boleh kosong!\n" << RESET << endl;
            continue; // Ulangi loop untuk meminta input lagi
        }
        
        // 2. Validasi panjang harus tepat 6 digit
        if (inputStr.length() != 6) {
            cout << ALERT_RED << "[Error] PIN harus tepat 6 digit!\n" << RESET << endl;
            continue; // Ulangi loop
        }
        
        // 3. Validasi memastikan semua karakter murni angka (Pencegahan Ekstra)
        bool semuaAngka = true;
        for (int i = 0; i < inputStr.length(); i++) {
            if (!isdigit(inputStr[i])) {
                semuaAngka = false;
                break;
            }
        }
        
        if (!semuaAngka) {
            cout << ALERT_RED << "[Error] PIN harus berupa angka!\n" << RESET << endl;
            continue; // Ulangi loop
        }
        
        // Jika lolos semua pengecekan di atas, keluar dari loop while
        break; 
    }
    
    int inputPin = stoi(inputStr);

    // Cocokkan dengan PIN asli milik nasabah
    if (inputPin != pinAsli) {
        cout << ALERT_RED << "\n[GAGAL] PIN yang Anda masukkan salah!" << RESET << endl;
        return false;
    }
    
    return true;
}

// Failure Handling untuk Konfirmasi Pilihan Y/T atau Y/N
char ambilKonfirmasiValid(string pesanPrompt, char opsiBenar, char opsiSalah) {
    char pilihan;
    while (true) {
        cout << pesanPrompt;
        cin >> pilihan;
        
        // Cek jika input gagal (misal memasukkan banyak karakter)
        if (cin.fail() || cin.peek() != '\n') {
            cout << ALERT_RED << "[Error] Input tidak valid! Masukkan hanya 1 karakter.\n" << RESET;
            cin.clear();
            cin.ignore(9999, '\n');
            continue;
        }

        // Standardisasi ke huruf kecil
        pilihan = tolower(pilihan);
        char opsi1 = tolower(opsiBenar);
        char opsi2 = tolower(opsiSalah);

        if (pilihan == opsi1 || pilihan == opsi2) {
            return pilihan; // Mengembalikan input yang sudah divalidasi
        } else {
            cout << ALERT_RED << "[Error] Pilihan salah! Harap masukkan (" << (char)toupper(opsiBenar) << "/" << (char)toupper(opsiSalah) << ").\n" << RESET;
        }
    }
}

string inputPasswordSensor() {
    string pass = "";
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') { 
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

akun* carinasabahByUsk(string usn) {
    akun* temp = head;
    while (temp != nullptr) {
        if (temp->data.username == usn) return temp;
        temp = temp->next;
    }
    return nullptr; 
}

akun* carinasabahByRek(long long rek) {
    akun* temp = head;
    while (temp != nullptr) {
        if (temp->data.norek == rek) return temp;
        temp = temp->next;
    }
    return nullptr;
}

bool apakahUserSudahAda(string usn, long long rek) {
    if (carinasabahByUsk(usn) != nullptr) {
        cout << ALERT_RED << "\n[Error] Username '" << usn << "' sudah digunakan nasabah lain!" << RESET << endl;
        return true;
    }
    if (carinasabahByRek(rek) != nullptr) return true; 
    return false;
}

void pendaftarannasabah() {
    bersihkanLayar();
    headmenuutama();
    akun* newakun = new akun;
    
    cout << pusat_layar << BLUE_PURE << "     === PENDAFTARAN NASABAH BARU ===\n" << RESET;
    cout << pusat_layar << "==========================================\n";
    cout << pusat_layar << "Masukkan Nama Nasabah : "; // Membersihkan buffer sebelum getline
    getline(cin, newakun->data.nama); 
    
    while (true) {
	    cout << pusat_layar << "Masukkan PIN (6 Digit): "; 
	    string pinStr = inputPinTranskasiSensor();
	    
	    // 1. Validasi panjang karakter
	    if (pinStr.length() != 6) {
	        cout << pusat_layar << ALERT_RED << "[Error] PIN harus tepat 6 digit!\n" << RESET;
	        continue;
	    }
	    
	    // 2. Validasi apakah semua karakter adalah angka (Menggantikan Catch)
	    bool semuaAngka = true;
	    for (int i = 0; i < pinStr.length(); i++) {
	        if (!isdigit(pinStr[i])) {
	            semuaAngka = false;
	            break;
	        }
	    }
	    
	    if (!semuaAngka) {
	        cout << pusat_layar << ALERT_RED << "[Error] PIN wajib diisi angka saja!\n" << RESET;
	        continue;
	    }
	    
	    // 3. Validasi angka pertama tidak boleh 0
	    if (pinStr[0] == '0') {
	        cout << pusat_layar << ALERT_RED << "[Error] PIN tidak boleh diawali angka 0!\n" << RESET;
	        continue;
	    }
	    
	    // Jika lolos semua pengecekan, konversi aman dilakukan dan keluar dari loop
	    newakun->data.pin = stoi(pinStr);
	    break; 
	}
	
    string tempUsn;
    cout << pusat_layar << "Masukkan Username     : "; getline(cin, tempUsn);
    
    long long tempRek = rekeninggenerator();
    while (carinasabahByRek(tempRek) != nullptr) {
        tempRek = rekeninggenerator();
    }

    if (apakahUserSudahAda(tempUsn, tempRek)) {
        delete newakun;
        tungguEnter();
        return;
    }

    newakun->data.username = tempUsn;
    cout << pusat_layar << "Masukkan Password     : "; cin >> newakun->data.password;
    cin.ignore(9999, '\n');
    cout << pusat_layar << "Setoran Awal (Min 50k): "; cin >> newakun->data.saldo;
    while (cin.fail() || newakun->data.saldo < 50000) {
        if (cin.fail()) {
            cout << pusat_layar << ALERT_RED << "Input tidak valid (Harus Angka)!\n" << RESET;
            cin.clear();
            cin.ignore(9999, '\n');
        } else {
            cout << pusat_layar << ALERT_RED << "Setoran Kurang! Minimal adalah 50.000.\n" << RESET;
        }
        cout << pusat_layar << "Setoran Awal (Min 50k): "; cin >> newakun->data.saldo;
    }
    cin.ignore(9999, '\n');
    newakun->data.norek = tempRek;
    newakun->data.isblokir = false;
    newakun->data.totalMutasi = 0;
    newakun->next = head;
    head = newakun;
    cout << pusat_layar << "==========================================\n";
    catatLogTransaksi(&(newakun->data), "MASUK", "Setoran Saldo Pembuatan Akun", newakun->data.saldo);
    cout << pusat_layar << SUCCESS_GRN << "Pendaftaran Berhasil! No Rek: " << newakun->data.norek << "\n" << RESET;
    totalnasabah++;
    cout << pusat_layar << SUB_GRAY << "Tekan ENTER Untuk Melanjutkan..." << endl;
    cin.get();
}

void lihatdatanasabah() {
    bersihkanLayar();
    headmenuutama();
    
    // Lebar kotak tabel data nasabah
    const int LEBAR_KOTAK = 38; 
    
    // Jumlah spasi untuk mendorong seluruh tabel agar berada di tengah layar konsol
    // Angka 55 ini disesuaikan agar posisinya pas di tengah, sejajar dengan menu utama
    string pusat_layar = string(55, ' '); 

    cout << pusat_layar << BLUE_PURE << "======================================\n";
    cout << pusat_layar << "          DATA SELURUH NASABAH        \n";
    cout << pusat_layar << "======================================\n" << RESET;

    if (head == nullptr) {
        cout << pusat_layar << ALERT_RED << "Belum ada data nasabah.\n" << RESET;
        return;
    }

    akun* temp = head;
    int no = 1;

    while (temp != nullptr) {
        cout << pusat_layar << BLUE_PURE << "--------------------------------------\n" << RESET;
        
        // Membuat tulisan "Nasabah Ke-X" tepat di tengah-tengah kotak tabel
        string judulNasabah = "Nasabah Ke-" + to_string(no);
        int spasiJudul = (LEBAR_KOTAK - judulNasabah.length()) / 2;
        cout << pusat_layar << TEXT_BLACK << string(spasiJudul, ' ') << judulNasabah << RESET << endl;
        cout << pusat_layar << BLUE_PURE << "--------------------------------------\n" << RESET;

        // Jarak spasi di dalam tabel agar titik dua (:) tetap lurus secara vertikal
        string pad_dalam = "     "; 
        
        cout << pusat_layar << pad_dalam << "Nama        : " << temp->data.nama << endl;
        cout << pusat_layar << pad_dalam << "No Rekening : " << temp->data.norek << endl;
        cout << pusat_layar << pad_dalam << "Username    : " << temp->data.username << endl;
        cout << pusat_layar << pad_dalam << "Saldo       : Rp " << temp->data.saldo << endl;
        cout << pusat_layar << pad_dalam << "Status Akun : ";

        if (temp->data.isblokir) cout << ALERT_RED << "DIBLOKIR" << RESET << endl;
        else cout << SUCCESS_GRN << "AKTIF" << RESET << endl;

        temp = temp->next;
        no++;
    }
    cout << pusat_layar << BLUE_PURE << "--------------------------------------\n" << RESET;
    
    // Membuat tulisan Total Nasabah tepat di tengah-tengah kotak tabel
    string totalText = "Total Nasabah : " + to_string(totalnasabah);
    int spasiTotal = (LEBAR_KOTAK - totalText.length()) / 2;
    cout << pusat_layar << string(spasiTotal, ' ') << totalText << endl;
    cout << pusat_layar << BLUE_PURE << "--------------------------------------\n" << RESET;
    cout << pusat_layar << "Tekan ENTER Untuk Melanjutkan" << endl;
    cin.get();
}

void kelolaakunnasabah() {
    bersihkanLayar();
    headmenuutama();
    cout << pusat_layar << BLUE_PURE << "======================================\n";
    cout << pusat_layar << "          KELOLA AKUN NASABAH          \n";
    cout << pusat_layar << "======================================\n" << RESET;

    if (head == nullptr) {
        cout << pusat_layar << ALERT_RED << "Belum ada data nasabah.\n" << RESET;
        return;
    }

    long long cariRek;
    cout << pusat_layar << "Masukkan No Rekening : "; cin >> cariRek;

    akun* temp = head;
    akun* prev = nullptr;

    while (temp != nullptr) {
        if (temp->data.norek == cariRek) {
            cout << "\n" << pusat_layar << "Data Ditemukan:\n";
            cout << pusat_layar << "Nama         : " << temp->data.nama << "\n";
            cout << pusat_layar << "No Rekening  : " << temp->data.norek << "\n";
            cout << pusat_layar << "Saldo        : Rp " << temp->data.saldo << "\n";
            cout << pusat_layar << "Status       : ";
            if (temp->data.isblokir) cout << ALERT_RED << "DIBLOKIR\n" << RESET;
            else cout << SUCCESS_GRN << "AKTIF\n" << RESET;

            int pilih;
            do {
                cout << "\n" << pusat_layar << "1. Blokir Akun\n"
                     << pusat_layar << "2. Buka Blokir Akun\n"
                     << pusat_layar << "3. Hapus Akun\n"
                     << pusat_layar << "4. Batal\n"
                     << pusat_layar << "Pilih (1-4) : ";
                cin >> pilih;
                if (cin.fail() || pilih < 1 || pilih > 4) {
                    cout << pusat_layar << ALERT_RED << "Pilihan tidak valid!\n" << RESET;
                    cin.clear(); cin.ignore(9999, '\n');
                    pilih = -1;
                }
            } while (pilih < 1 || pilih > 4);

            char konfirmasi;
            // Ditambahkan spasi pendorong kustom di dalam fungsi konfirmasi bawaanmu
            string prompt_pusat = pusat_layar + "Apakah Anda yakin? (Y/N) : "; 

            switch (pilih) {
            case 1:
                konfirmasi = ambilKonfirmasiValid(pusat_layar + "Yakin memblokir akun ini? (Y/N) : ", 'Y', 'N');
                if (konfirmasi == 'y') {
                    if (temp->data.isblokir) cout << pusat_layar << ALERT_RED << "Akun memang sudah diblokir.\n" << RESET;
                    else { temp->data.isblokir = true; cout << pusat_layar << SUCCESS_GRN << "Akun berhasil diblokir.\n" << RESET; }
                } else cout << pusat_layar << GOLD_GOLD << "Pemblokiran dibatalkan.\n" << RESET;
                break;

            case 2:
                konfirmasi = ambilKonfirmasiValid(pusat_layar + "Yakin membuka blokir akun ini? (Y/N) : ", 'Y', 'N');
                if (konfirmasi == 'y') {
                    if (!temp->data.isblokir) cout << pusat_layar << ALERT_RED << "Akun sudah aktif.\n" << RESET;
                    else { temp->data.isblokir = false; cout << pusat_layar << SUCCESS_GRN << "Akun berhasil diaktifkan.\n" << RESET; }
                } else cout << pusat_layar << GOLD_GOLD << "Pembukaan blokir dibatalkan.\n" << RESET;
                break;

            case 3:
                konfirmasi = ambilKonfirmasiValid(pusat_layar + "Yakin ingin menghapus akun ini? (Y/N) : ", 'Y', 'N');
                if (konfirmasi == 'y') {
                    if (temp == head) head = head->next;
                    else prev->next = temp->next;
                    delete temp;
                    totalnasabah--;
                    cout << pusat_layar << SUCCESS_GRN << "Akun berhasil dihapus dari sistem.\n" << RESET;
                } else cout << pusat_layar << GOLD_GOLD << "Penghapusan dibatalkan.\n" << RESET;
                break;

            case 4:
                cout << pusat_layar << "Operasi dibatalkan.\n";
                break;
            }
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    cout << pusat_layar << ALERT_RED << "Nomor rekening tidak ditemukan.\n" << RESET;
}

void setortunai() {
    bersihkanLayar(); 
    headmenuutama();
    if (currentuser == nullptr) return;

    long long nominal;
    cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
    cout << pusat_layar << BLUE_PURE << "        FASILITAS SETOR TUNAI         \n" << RESET;
    cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
                
    cout << pusat_layar << CYAN_LAUT << "Saldo Saat Ini : " << RESET << TEXT_BLACK << "Rp " << currentuser->data.saldo << RESET << "\n";
    cout << pusat_layar << CYAN_LAUT << "Nominal Setor  : " << RESET << TEXT_BLACK << "Rp " << RESET; cin >> nominal;
                
    cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;

    if (cin.fail() || nominal <= 0) {
        cin.clear(); cin.ignore(9999, '\n');
        cout << pusat_layar << ALERT_RED << "[GAGAL] Input nominal tidak valid!\n" << RESET;
    } else if (nominal % 50000 != 0) {
        cout << pusat_layar << ALERT_RED << "[GAGAL] Setoran harus kelipatan Rp 50.000!\n" << RESET;
    } else {
        cout << pusat_layar;
        if (!verifikasiPinTransaksi(currentuser->data.pin)) {
            cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
            return;
        }
        currentuser->data.saldo += nominal;
        catatLogTransaksi(&(currentuser->data), "MASUK", "Setor Tunai Mandiri", nominal);
        cout << pusat_layar << SUCCESS_GRN << "[SUKSES] Uang berhasil disetor.\n" << RESET;
        cout << pusat_layar << TEXT_BLACK << "Saldo Terbaru  : Rp " << currentuser->data.saldo << RESET << "\n";
    }
    cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
}

void riwayattransaksi() { 
    if (currentuser == nullptr) return;
    pengguna* ptrUser = &(currentuser->data);

    cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
    cout << pusat_layar << BLUE_PURE << "          MUTASI REKENING NASABAH      \n" << RESET;
    cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
    cout << pusat_layar << TEXT_BLACK << "No Rekening : " << RESET << ptrUser->norek << "\n";
    cout << pusat_layar << TEXT_BLACK << "Nama        : " << RESET << ptrUser->nama << "\n";
    cout << pusat_layar << BLUE_PURE << "--------------------------------------\n" << RESET;

    if (ptrUser->totalMutasi == 0) {
        cout << pusat_layar << "       Belum ada riwayat transaksi.    \n";
        cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
        return;
    }

    for (int i = 0; i < ptrUser->totalMutasi; i++) {
        cout << pusat_layar << i + 1 << ". [" << GOLD_GOLD << ptrUser->riwayat[i].tipe << RESET << "] " << ptrUser->riwayat[i].detail << "\n"
             << pusat_layar << "   Jumlah: " << SUCCESS_GRN << "Rp " << ptrUser->riwayat[i].jumlah << RESET << "\n";
        cout << pusat_layar << SUB_GRAY << "   ----------------------------------\n" << RESET;
    }
    cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
}

void riwayattransaksinasabah() { 
    long long rekTarget;
    cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
    cout << pusat_layar << BLUE_PURE << "      [ADMIN] MONITORING LOG NASABAH   \n" << RESET;
    cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
    cout << pusat_layar << CYAN_LAUT << "Masukkan Nomor Rekening Target: " << RESET; cin >> rekTarget;
    cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;

    akun* temp = carinasabahByRek(rekTarget);
    if (temp != nullptr) {
        cout << pusat_layar << SUCCESS_GRN << "[DATA KETEMU] Mengalihkan ke riwayat...\n" << RESET;
        tungguEnter();
        bersihkanLayar();
        headmenuutama();
        
        cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
        cout << pusat_layar << BLUE_PURE << "       [ADMIN MODE] DETAIL MUTASI       \n" << RESET;
        cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
        cout << pusat_layar << TEXT_BLACK << "No Rekening : " << RESET << temp->data.norek << "\n";
        cout << pusat_layar << TEXT_BLACK << "Nama Nasabah: " << RESET << temp->data.nama << "\n";
        cout << pusat_layar << BLUE_PURE << "--------------------------------------\n" << RESET;

        if (temp->data.totalMutasi == 0) {
            cout << pusat_layar << "       Belum ada riwayat transaksi.    \n";
        } else {
            for (int i = 0; i < temp->data.totalMutasi; i++) {
                cout << pusat_layar << i + 1 << ". [" << GOLD_GOLD << temp->data.riwayat[i].tipe << RESET << "] " << temp->data.riwayat[i].detail << "\n"
                     << pusat_layar << "   Jumlah: " << SUCCESS_GRN << "Rp " << temp->data.riwayat[i].jumlah << RESET << "\n";
                cout << pusat_layar << SUB_GRAY << "   ----------------------------------\n" << RESET;
            }
        }
        cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
    } else {
        cout << pusat_layar << ALERT_RED << "[GAGAL] Nomor rekening tidak terdaftar!\n" << RESET;
        cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
    }
}

void tariktunai(pengguna* ptrUser) { 
    bersihkanLayar(); 
    headmenuutama();
    long long nominal;
                
    cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
    cout << pusat_layar << BLUE_PURE << "       FASILITAS PENARIKAN TUNAI       \n" << RESET;
    cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
                
    cout << pusat_layar << CYAN_LAUT << "Saldo Tersedia : " << RESET << TEXT_BLACK << "Rp " << ptrUser->saldo << RESET << "\n";
    cout << pusat_layar << CYAN_LAUT << "Nominal Tarik  : " << RESET << TEXT_BLACK << "Rp " << RESET; cin >> nominal;
                
    cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;

    if (nominal <<= 0 || nominal % 50000 != 0) {
        cout << pusat_layar << ALERT_RED << "[GAGAL] Nominal harus kelipatan Rp 50.000!\n" << RESET;
    } else if (nominal > ptrUser->saldo) {
        cout << pusat_layar << ALERT_RED << "[GAGAL] Saldo Anda tidak mencukupi!\n" << RESET;
    } else {
        
        cout << pusat_layar; 
        if (!verifikasiPinTransaksi(ptrUser->pin)) {
            cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
            tungguEnter();
            return;
        }
        ptrUser->saldo -= nominal;
        catatLogTransaksi(ptrUser, "KELUAR", "Penarikan Tunai Mandiri", nominal);
        cout << pusat_layar << SUCCESS_GRN << "[SUKSES] Silakan ambil uang Anda.\n" << RESET;
        cout << pusat_layar << TEXT_BLACK << "Sisa Saldo     : Rp " << ptrUser->saldo << RESET << "\n";
    }
    cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
    tungguEnter();
}

void transfersaldo() {
    if (currentuser == nullptr) return;

    long long rekTujuan, nominal;
    cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
    cout << pusat_layar << BLUE_PURE << "              TRANSFER SALDO            \n" << RESET;
    cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;

    cout << pusat_layar << CYAN_LAUT << "Nomor Rekening Tujuan : " << RESET; cin >> rekTujuan;

    if (rekTujuan == currentuser->data.norek) {
        cout << pusat_layar << ALERT_RED << "\nTidak bisa transfer ke rekening sendiri!\n" << RESET;
        return;
    }

    akun* penerima = carinasabahByRek(rekTujuan);
    if (penerima == nullptr) {
        cout << pusat_layar << ALERT_RED << "\nNomor rekening tidak ditemukan!\n" << RESET;
        return;
    }

    if (penerima->data.isblokir) {
        cout << pusat_layar << ALERT_RED << "\nTransfer gagal! Rekening tujuan sedang diblokir.\n" << RESET;
        return;
    }

    cout << pusat_layar << CYAN_LAUT << "Nama Penerima         : " << RESET << penerima->data.nama << "\n";
    cout << pusat_layar << CYAN_LAUT << "Nominal Transfer      : " << RESET << "Rp "; cin >> nominal;

    if (nominal <= 0 || nominal < 10000 || nominal > currentuser->data.saldo) {
        cout << pusat_layar << ALERT_RED << "\nNominal tidak valid atau saldo kurang (Min. Rp 10.000)!\n" << RESET;
        return;
    }

    cout << "\n" << pusat_layar << SUCCESS_GRN << "========== KONFIRMASI ==========\n" << RESET;
    cout << pusat_layar << "Pengirim : " << currentuser->data.nama << "\n";
    cout << pusat_layar << "Penerima : " << penerima->data.nama << "\n";
    cout << pusat_layar << "Rekening : " << penerima->data.norek << "\n";
    cout << pusat_layar << "Nominal  : Rp " << nominal << "\n";

    char konfirmasi = ambilKonfirmasiValid(pusat_layar + "\nLanjutkan transfer? (Y/N) : ", 'Y', 'N');
    if (konfirmasi != 'y') {
        cout << pusat_layar << ALERT_RED << "\nTransfer dibatalkan.\n" << RESET;
        return;
    }
    
    cout << pusat_layar;
    if (!verifikasiPinTransaksi(currentuser->data.pin)) return;

    currentuser->data.saldo -= nominal;
    penerima->data.saldo += nominal;

    catatLogTransaksi(&(currentuser->data), "KELUAR", "Transfer ke " + penerima->data.nama, nominal);
    catatLogTransaksi(&(penerima->data), "MASUK", "Transfer dari " + currentuser->data.nama, nominal);

    cout << "\n" << pusat_layar << SUCCESS_GRN << "======================================\n";
    cout << pusat_layar << "           TRANSFER BERHASIL!           \n";
    cout << pusat_layar << "======================================\n" << RESET;
}

void loginAsAdmin() {
    bersihkanLayar(); 
    string usn, pass;
    cout << BLUE_PURE << "======================================" << RESET << "\n";
    cout << BLUE_PURE << "    SECURITY ACCESS: VERIFIKASI ADMIN  " << RESET << "\n";
    cout << BLUE_PURE << "======================================" << RESET << "\n";
    cout << CYAN_LAUT << "Username Akun : " << RESET; cin >> usn;
    cout << CYAN_LAUT << "Password Akun : " << RESET; pass = inputPasswordSensor(); 
    cout << BLUE_PURE << "======================================" << RESET << "\n";

    for (int i = 0; i < 2; i++) {
        if (dataAdmin[i].username == usn && dataAdmin[i].password == pass) {
        	cout << SUCCESS_GRN << "[Akses Diterima] Selamat Datang " << dataAdmin[i].namaAdmin << RESET;
        	tungguEnter();
            subMenuAdmin(i);
            return;
        }
    }
    cout << endl << ALERT_RED << "[Akses Ditolak] Kredensial Salah!" << RESET << "\n"; 
    tungguEnter();
}

void loginAsUser() {
    bersihkanLayar();
    string usn, pass;
    cout << BLUE_PURE << "======================================" << RESET << "\n";
    cout << BLUE_PURE << "         PORTAL OTENTIKASI NASABAH    " << RESET << "\n";
    cout << BLUE_PURE << "======================================" << RESET << "\n";
    cout << CYAN_LAUT << "Username Akun : " << RESET; cin >> usn;
    cout << CYAN_LAUT << "Password Akun : " << RESET; pass = inputPasswordSensor();
    cout << BLUE_PURE << "======================================" << RESET << "\n";
    
    akun* temp = carinasabahByUsk(usn);
    if (temp != nullptr && temp->data.password == pass) {
        if (temp->data.isblokir) {
            cout << ALERT_RED << "[STATUS BLOKIR] Akun Anda Ditangguhkan oleh Admin!" << RESET << "\n"; 
            tungguEnter();
            return;
        }
        currentuser = temp;
        cout << SUCCESS_GRN << "[BERHASIL] Selamat Datang " << currentuser->data.nama << RESET;
        tungguEnter();
        subMenuNasabah();
        return;
    }
    cout << "\n" << ALERT_RED << "[Gagal Auth] Kombinasi Username & Password Keliru!" << RESET << "\n"; 
    tungguEnter();
}

void subMenuAdmin(int idx) {
    int kursor = 1; 
    const int totalOpsi = 5; 
    char tombol;

    while (true) {
        bersihkanLayar();
        headmenuutama();
        cout << "\t\t\t\t\t\t\t     ===" << BLUE_PURE << " KONSOL OPERATOR SUPERVISOR " << RESET << "===   " << "\n";
        cout << "==================================================================================================================================================" << RESET << "\n";
        cout << "\t\t\t\t\t\t\t      " << SUCCESS_GRN << "Petugas Aktif: " << dataAdmin[idx].namaAdmin << RESET << "\n";
        cout << "==================================================================================================================================================" << RESET << "\n";
        cout << "\t\t\t\t\t\t\t       ==============================" << endl;
        if (kursor == 1) cout << "\t\t\t\t\t\t\t\t" << CYAN_LAUT << "> Pendaftaran Nasabah Baru <" << RESET << "\n";
        else cout << "\t\t\t\t\t\t\t\t  Pendaftaran Nasabah Baru  \n";

        if (kursor == 2) cout << "\t\t\t\t\t\t\t\t" << CYAN_LAUT << "   > Lihat Data Nasabah <" << RESET << "\n";
        else cout << "\t\t\t\t\t\t\t\t     Lihat Data Nasabah  \n";

        if (kursor == 3) cout << "\t\t\t\t\t\t\t\t  " << CYAN_LAUT << "> Kelola  Akun  Nasabah <" << RESET << "\n";
        else cout << "\t\t\t\t\t\t\t\t    Kelola  Akun  Nasabah  \n";

        if (kursor == 4) cout << "\t\t\t\t\t\t\t\t" << CYAN_LAUT << "> Riwayat Transaksi Nasabah <" << RESET << "\n";
        else cout << "\t\t\t\t\t\t\t\t  Riwayat Transaksi Nasabah  \n";

        if (kursor == 5) cout << "\t\t\t\t\t\t\t\t       " << ALERT_RED << "> Logout Sesi <" << RESET << "\n";
        else cout << "\t\t\t\t\t\t\t\t\t Logout Sesi  \n";
        cout << "\t\t\t\t\t\t\t       ==============================" << endl;
        cout << BLUE_PURE << "==================================================================================================================================================" << RESET << "\n";
        cout << SUB_GRAY << "Navigasi: Panah Atas/Bawah | Enter untuk memilih" << RESET << "\n";
        tombol = _getch();
        if (tombol == 0 || tombol == -32 || tombol == 224) {
            tombol = _getch();
            if (tombol == 72) kursor = (kursor == 1) ? totalOpsi : kursor - 1;
            else if (tombol == 80) kursor = (kursor == totalOpsi) ? 1 : kursor + 1;
        } else if (tombol == '\r') {
            if (kursor == 1) pendaftarannasabah(); 
            else if (kursor == 2) { lihatdatanasabah(); }
            else if (kursor == 3) { kelolaakunnasabah(); tungguEnter(); }
            else if (kursor == 4) { riwayattransaksinasabah(); tungguEnter(); }
            else if (kursor == 5) break;
        }
    }
}

void subMenuNasabah() {
    int kursor = 1; 
    const int totalOpsi = 5; 
    char tombol;

    while (true) {
        bersihkanLayar();
        headmenuutama();
        
        // Judul header menu nasabah diselaraskan ke tengah
        cout << "\t\t\t\t\t\t\t     ===" << BLUE_PURE << "    INTERFACES MINI BANK CLIENT   " << RESET << "===   " << "\n";
        cout << "==================================================================================================================================================" << RESET << "\n";
        
        // Mengambil data string informasi nasabah
        string namaNasabah = currentuser->data.nama;
        string noRekString = to_string(currentuser->data.norek);
        string stringSaldo = "Rp " + to_string(currentuser->data.saldo);
        
        // Perhitungan padding spasi di dalam card info nasabah (lebar total di dalam card disesuaikan)
        int s1 = 21 - namaNasabah.length(); if(s1 < 0) s1 = 0;
        int s2 = 21 - noRekString.length(); if(s2 < 0) s2 = 0;
        int s3 = 21 - stringSaldo.length(); if(s3 < 0) s3 = 0;
        
        // Menampilkan card informasi nasabah di posisi tengah menggunakan tabulasi \t
        cout << "\t\t\t\t\t\t\t       ==============================" << endl;
        cout << "\t\t\t\t\t\t\t       " << TEXT_BLACK << "Nama Nasabah : " << RESET << namaNasabah << string(s1, ' ') << RESET << "\n";
        cout << "\t\t\t\t\t\t\t       " << TEXT_BLACK << "No. Rekening : " << RESET << noRekString << string(s2, ' ') << RESET << "\n";
        cout << "\t\t\t\t\t\t\t       " << TEXT_BLACK << "Saldo Utama  : " << RESET << stringSaldo << string(s3, ' ') << RESET << "\n";
        cout << "\t\t\t\t\t\t\t       ==============================" << endl;
        
        // Opsi-opsi menu interaktif yang disamakan padding-nya agar TIDAK GOYANG sama sekali
        if (kursor == 1) cout << "\t\t\t\t\t\t\t\t     " << SUCCESS_GRN << "> Setor Tunai <" << RESET << "\n";
        else             cout << "\t\t\t\t\t\t\t\t       Setor Tunai  \n";

        if (kursor == 2) cout << "\t\t\t\t\t\t\t\t     " << SUCCESS_GRN << "> Tarik Tunai <" << RESET << "\n";
        else             cout << "\t\t\t\t\t\t\t\t       Tarik Tunai  \n";
        
        if (kursor == 3) cout << "\t\t\t\t\t\t\t\t   " << SUCCESS_GRN << " > Transfer Saldo <" << RESET << "\n";
        else             cout << "\t\t\t\t\t\t\t\t      Transfer Saldo  \n";

        if (kursor == 4) cout << "\t\t\t\t\t\t\t\t  " << SUCCESS_GRN << "> Riwayat  Transaksi <" << RESET << "\n";
        else             cout << "\t\t\t\t\t\t\t\t    Riwayat  Transaksi  \n";

        if (kursor == 5) cout << "\t\t\t\t\t\t\t\t\t" << ALERT_RED << "> Logout <" << RESET << "\n";
        else             cout << "\t\t\t\t\t\t\t\t\t  Logout  \n";
        
        cout << "\t\t\t\t\t\t\t       ==============================" << endl;
        cout << BLUE_PURE << "==================================================================================================================================================" << RESET << "\n";
        cout << SUB_GRAY << "Navigasi: Panah Atas/Bawah | Enter untuk memilih" << RESET << "\n";
        
        tombol = _getch();
        if (tombol == 0 || tombol == -32 || tombol == 224) {
            tombol = _getch();
            if (tombol == 72) kursor = (kursor == 1) ? totalOpsi : kursor - 1;
            else if (tombol == 80) kursor = (kursor == totalOpsi) ? 1 : kursor + 1;
        } 
        else if (tombol == '\r') {
            if (kursor == 1) { setortunai(); tungguEnter(); }
            else if (kursor == 2) { tariktunai(&(currentuser->data)); }
            else if (kursor == 3) { bersihkanLayar(); transfersaldo(); tungguEnter(); }
            else if (kursor == 4) { bersihkanLayar(); riwayattransaksi(); tungguEnter(); }
            else if (kursor == 5) {
                bersihkanLayar();
                cout << "\t\t\t\t\t\t\t" << SUCCESS_GRN << "[OK] Kartu dikeluarkan. Sesi aman berakhir." << RESET << "\n";
                currentuser = nullptr;
                tungguEnter();
                break;
            }
        }
    }
}

void tambahNasabahDariArray(pengguna arr[], int jumlahData) {
    for (int i = 0; i < jumlahData; i++) {
        akun* newakun = new akun;
        newakun->data = arr[i];
        newakun->next = nullptr;
        if (head == nullptr) head = newakun;
        else { newakun->next = head; head = newakun; }
        totalnasabah++;
    }
}

void menuUtama() {
    static bool isInitialized = false;
    if (!isInitialized) {
        srand(time(0));
        tambahNasabahDariArray(dummyNasabah, 2);
        isInitialized = true; 
    }
    int kursor = 1;
    const int totalOpsi = 3;
    char tombol;

    while (true) {
        bersihkanLayar();
        headmenuutama();
		cout << "\t\t\t\t\t\t\t       ==============================" << endl;
        if (kursor == 1) cout << "\t\t\t\t\t\t\t       |  " << SUCCESS_GRN << "> Login Sebagai Nasabah <" << RESET << " |\n";
        else cout << "\t\t\t\t\t\t\t       |    Login Sebagai Nasabah   |\n";

        if (kursor == 2) cout << "\t\t\t\t\t\t\t       |  " << SUCCESS_GRN << ">  Login Sebagai Admin  <" << RESET << " |\n";
        else cout << "\t\t\t\t\t\t\t       |     Login Sebagai Admin    |\n";

        if (kursor == 3) cout << "\t\t\t\t\t\t\t       |  " << ALERT_RED << ">    Keluar Aplikasi    <" << RESET << " |\n";
        else cout << "\t\t\t\t\t\t\t       |       Keluar Aplikasi      |\n";
        cout << "\t\t\t\t\t\t\t       ==============================" << endl;
            
        cout << BLUE_PURE << "==============================================================================================================================================" << RESET << "\n";
        cout << SUB_GRAY << "Navigasi: Panah Atas/Bawah | Enter untuk memilih" << RESET << "\n";
        
        tombol = _getch();
        if (tombol == 0 || tombol == -32 || tombol == 224) {
            tombol = _getch();
            if (tombol == 72) kursor = (kursor == 1) ? totalOpsi : kursor - 1;
            else if (tombol == 80) kursor = (kursor == totalOpsi) ? 1 : kursor + 1;
        } 
        else if (tombol == '\r') { 
            if (kursor == 1) loginAsUser();
            else if (kursor == 2) loginAsAdmin();
            else if (kursor == 3) {
                bersihkanLayar();
                cout << SUCCESS_GRN << "[TERMINATED] Sesi mini bank ditutup. Terima kasih." << RESET << "\n\n";
                break;
            }
        }
    }
}

int main () {
    srand(time(0));
    menuUtama(); 
    return 0;
}