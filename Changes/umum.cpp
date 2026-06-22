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
    cout << "\n" << pusat_layar << SUB_GRAY << "     Tekan ENTER untuk melanjutkan..." << RESET;
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

string inputPinTranskasiSensor() {
    string pinStr = "";
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') { 
            if (pinStr.length() > 0) {
                pinStr.pop_back();
                cout << "\b \b";
            }
        } else if (ch >= '0' && ch <= '9') {
            pinStr.push_back(ch);
            cout << "*";
        }
    }
    cout << endl;
    return pinStr;
}

bool verifikasiPinTransaksi(int pinAsli) {
    string inputStr;
    
    while (true) {
        cout << pusat_layar << CYAN_LAUT << "Masukkan 6-Digit PIN Anda : " << RESET;
        inputStr = inputPinTranskasiSensor();

        if (inputStr.empty()) {
            cout << pusat_layar << ALERT_RED << "[Error] PIN tidak boleh kosong!\n" << RESET << endl;
            continue; 
        }
        
        if (inputStr.length() != 6) {
            cout << pusat_layar << ALERT_RED << "[Error] PIN harus tepat 6 digit!\n" << RESET << endl;
            continue;
        }
        
        bool semuaAngka = true;
        for (int i = 0; i < inputStr.length(); i++) {
            if (!isdigit(inputStr[i])) {
                semuaAngka = false;
                break;
            }
        }
        
        if (!semuaAngka) {
            cout << pusat_layar << ALERT_RED << "[Error] PIN harus berupa angka!\n" << RESET << endl;
            continue;
        }
        break; 
    }
    
    int inputPin = stoi(inputStr);

    if (inputPin != pinAsli) {
        cout << "\n"<< pusat_layar << ALERT_RED << "[GAGAL] PIN yang Anda masukkan salah!" << RESET << endl;
        return false;
    }
    
    return true;
}

char ambilKonfirmasiValid(string pesanPrompt, char opsiBenar, char opsiSalah) {
    char pilihan;
    while (true) {
        cout << pusat_layar << pesanPrompt; cin >> pilihan;
        
        if (cin.fail() || cin.peek() != '\n') {
            cout << pusat_layar << ALERT_RED << "[Error] Input tidak valid! Masukkan hanya 1 karakter.\n" << RESET;
            cin.clear();
            cin.ignore(9999, '\n');
            continue;
        }

        pilihan = tolower(pilihan);
        char opsi1 = tolower(opsiBenar);
        char opsi2 = tolower(opsiSalah);

        if (pilihan == opsi1 || pilihan == opsi2) {
            return pilihan;
        } else {
            cout << pusat_layar << ALERT_RED << "[Error] Pilihan salah! Harap masukkan (" << (char)toupper(opsiBenar) << "/" << (char)toupper(opsiSalah) << ").\n" << RESET;
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
    cout << pusat_layar << "Masukkan Nama Nasabah : ";
    getline(cin >> ws, newakun->data.nama); 
    
    while (true) {
	    cout << pusat_layar << "Masukkan PIN (6 Digit): "; 
	    string pinStr = inputPinTranskasiSensor();
	    
	    if (pinStr.length() != 6) {
	        cout << pusat_layar << ALERT_RED << "[Error] PIN harus tepat 6 digit!\n" << RESET;
	        continue;
	    }
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

	    if (pinStr[0] == '0') {
	        cout << pusat_layar << ALERT_RED << "[Error] PIN tidak boleh diawali angka 0!\n" << RESET;
	        continue;
	    }
	    
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
    string inputSaldoStr;
    while (true) {
        cout << pusat_layar << "Setoran Awal (Min 50k): "; 
        cin >> inputSaldoStr;
        bool semuaAngka = true;
        for (int i = 0; i < inputSaldoStr.length(); i++) {
            if (!isdigit(inputSaldoStr[i])) {
                semuaAngka = false;
                break;
            }
        }

        if (!semuaAngka) {
            cout << pusat_layar << ALERT_RED << "[Error] Input harus murni angka tanpa karakter lain!\n" << RESET;
            cin.clear();
            cin.ignore(9999, '\n');
            continue;
        }

        long long nominalTemp = stoll(inputSaldoStr);

        if (nominalTemp < 50000) {
            cout << pusat_layar << ALERT_RED << "[Error] Setoran Kurang! Minimal adalah 50.000.\n" << RESET;
            cin.clear();
            cin.ignore(9999, '\n');
            continue;
        }
        newakun->data.saldo = nominalTemp;
        cin.ignore(9999, '\n');
        break;
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
    tungguEnter();
}

void lihatdatanasabah() {
    bersihkanLayar();
    headmenuutama();
    
    const int LEBAR_KOTAK = 38; 

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
        string judulNasabah = "Nasabah Ke-" + to_string(no);
        int spasiJudul = (LEBAR_KOTAK - judulNasabah.length()) / 2;
        cout << pusat_layar << TEXT_BLACK << string(spasiJudul, ' ') << judulNasabah << RESET << endl;
        cout << pusat_layar << BLUE_PURE << "--------------------------------------\n" << RESET;

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
    
    
    string totalText = "Total Nasabah : " + to_string(totalnasabah);
    int spasiTotal = (LEBAR_KOTAK - totalText.length()) / 2;
    cout << pusat_layar << string(spasiTotal, ' ') << totalText << endl;
    cout << pusat_layar << BLUE_PURE << "--------------------------------------\n" << RESET;
    tungguEnter();
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

    string inputRekStr;
    long long cariRek = 0;

    while (true) {
        cout << pusat_layar << "Masukkan No Rekening : "; cin >> inputRekStr;
        
        bool semuaAngka = true;
        for (int i = 0; i < inputRekStr.length(); i++) {
            if (!isdigit(inputRekStr[i])) {
                semuaAngka = false;
                break;
            }
        }

        if (!semuaAngka) {
            cout << pusat_layar << ALERT_RED << "Input harus berupa angka murni!\n" << RESET;
            cin.clear(); cin.ignore(9999, '\n');
            continue;
        }

        cariRek = stoll(inputRekStr);
        cin.ignore(9999, '\n');
        break;
    }

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
            
            string prompt_pusat = pusat_layar + "Apakah Anda yakin? (Y/N) : "; 

            switch (pilih) {
            case 1:
                konfirmasi = ambilKonfirmasiValid("Yakin memblokir akun ini? (Y/N) : ", 'Y', 'N');
                if (konfirmasi == 'y') {
                    if (temp->data.isblokir) cout << pusat_layar << ALERT_RED << "Akun memang sudah diblokir.\n" << RESET;
                    else { temp->data.isblokir = true; cout << pusat_layar << SUCCESS_GRN << "Akun berhasil diblokir.\n" << RESET; }
                } else cout << pusat_layar << GOLD_GOLD << "Pemblokiran dibatalkan.\n" << RESET;
                break;

            case 2:
                konfirmasi = ambilKonfirmasiValid("Yakin membuka blokir akun ini? (Y/N) : ", 'Y', 'N');
                if (konfirmasi == 'y') {
                    if (!temp->data.isblokir) cout << pusat_layar << ALERT_RED << "Akun sudah aktif.\n" << RESET;
                    else { temp->data.isblokir = false; cout << pusat_layar << SUCCESS_GRN << "Akun berhasil diaktifkan.\n" << RESET; }
                } else cout << pusat_layar << GOLD_GOLD << "Pembukaan blokir dibatalkan.\n" << RESET;
                break;

            case 3:
                konfirmasi = ambilKonfirmasiValid("Yakin ingin menghapus akun ini? (Y/N) : ", 'Y', 'N');
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
            tungguEnter();
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    cout << pusat_layar << ALERT_RED << "Nomor rekening tidak ditemukan.\n" << RESET;
    tungguEnter();
}

void setortunai() {
    bersihkanLayar(); 
    headmenuutama();
    if (currentuser == nullptr) return;

    string inputNominalStr;
    long long nominal = 0;

    cout << pusat_layar << BLUE_PURE << "==========================================\n" << RESET;
    cout << pusat_layar << BLUE_PURE << "           FASILITAS SETOR TUNAI          \n" << RESET;
    cout << pusat_layar << BLUE_PURE << "==========================================\n" << RESET;
                
    cout << pusat_layar << CYAN_LAUT << "  Saldo Saat Ini : " << RESET << TEXT_BLACK << "Rp " << currentuser->data.saldo << RESET << "\n";
    
    while (true) {
        cout << pusat_layar << CYAN_LAUT << "  Nominal Setor  : " << RESET << TEXT_BLACK << "Rp " << RESET; 
        cin >> inputNominalStr;
        
        bool semuaAngka = true;
        for (int i = 0; i < inputNominalStr.length(); i++) {
            if (!isdigit(inputNominalStr[i])) { 
                semuaAngka = false; 
                break; 
            }
        }
        
        if (!semuaAngka) {
            cout << pusat_layar << ALERT_RED << "[GAGAL] Input harus murni angka tanpa karakter lain!\n" << RESET;
            cin.clear(); cin.ignore(9999, '\n');
            continue;
        }
        
        nominal = stoll(inputNominalStr);
        
        if (nominal <= 0) {
            cout << pusat_layar << ALERT_RED << "[GAGAL] Nominal harus lebih dari Rp 0!\n" << RESET;
            continue;
        }
        if (nominal % 50000 != 0) {
            cout << pusat_layar << ALERT_RED << "[GAGAL] Setoran harus kelipatan Rp 50.000!\n" << RESET;
            continue;
        }
        
        cin.ignore(9999, '\n');
        break; 
    }
                
    cout << pusat_layar << BLUE_PURE << "==========================================\n" << RESET;

    if (!verifikasiPinTransaksi(currentuser->data.pin)) {
        cout << pusat_layar << BLUE_PURE << "==========================================\n" << RESET;
        return;
    }
    
    currentuser->data.saldo += nominal;
    catatLogTransaksi(&(currentuser->data), "MASUK", "Setor Tunai Mandiri", nominal);
    cout << pusat_layar << SUCCESS_GRN << "[SUKSES] Uang berhasil disetor.\n" << RESET;
    cout << pusat_layar << TEXT_BLACK << "Saldo Terbaru  : Rp " << currentuser->data.saldo << RESET << "\n";
    cout << pusat_layar << BLUE_PURE << "==========================================\n" << RESET;
    tungguEnter();
}

void riwayattransaksi() { 
    if (currentuser == nullptr) return;
    pengguna* ptrUser = &(currentuser->data);

    cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
    cout << pusat_layar << BLUE_PURE << "       MUTASI REKENING NASABAH      \n" << RESET;
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
    cout << pusat_layar << BLUE_PURE << "     [ADMIN] MONITORING LOG NASABAH   \n" << RESET;
    cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
    
    while (true) {
        cout << pusat_layar << CYAN_LAUT << "Masukkan No Rekening Target: " << RESET; 
        cin >> rekTarget;
        
        
        if (cin.fail()) {
            cout << pusat_layar << ALERT_RED << "[GAGAL] Input harus berupa angka!\n" << RESET << endl;
            cin.clear();              
            cin.ignore(9999, '\n');   
            continue;                 
        }
        
        cin.ignore(9999, '\n');       
        break;
    }
    cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;

    akun* temp = carinasabahByRek(rekTarget);
    if (temp != nullptr) {
        cout << pusat_layar << SUCCESS_GRN << "[DATA KETEMU] Mengalihkan ke riwayat...\n" << RESET;
        tungguEnter();
        bersihkanLayar();
        headmenuutama();
        
        cout << pusat_layar << BLUE_PURE << "======================================\n" << RESET;
        cout << pusat_layar << BLUE_PURE << "     [ADMIN MODE] DETAIL MUTASI       \n" << RESET;
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
        cout << pusat_layar << BLUE_PURE << "=======================================\n" << RESET;
        tungguEnter();
    } else {
        cout << pusat_layar << ALERT_RED << "[GAGAL] Nomor rekening tidak terdaftar!\n" << RESET;
        cout << pusat_layar << BLUE_PURE << "=======================================\n" << RESET;
        tungguEnter();
    }
}

void tariktunai(pengguna* ptrUser) { 
    bersihkanLayar(); 
    headmenuutama();
    string inputNominalStr;
    long long nominal = 0;
                
    cout << pusat_layar << BLUE_PURE << "==========================================\n" << RESET;
    cout << pusat_layar << BLUE_PURE << "         FASILITAS PENARIKAN TUNAI        \n" << RESET;
    cout << pusat_layar << BLUE_PURE << "==========================================\n" << RESET;
                
    cout << pusat_layar << CYAN_LAUT << "Saldo Tersedia : " << RESET << TEXT_BLACK << "Rp " << ptrUser->saldo << RESET << "\n";
    
    while (true) {
        cout << pusat_layar << CYAN_LAUT << "Nominal Tarik  : " << RESET << TEXT_BLACK << "Rp " << RESET; 
        cin >> inputNominalStr;
        
        bool semuaAngka = true;
        for (int i = 0; i < inputNominalStr.length(); i++) {
            if (!isdigit(inputNominalStr[i])) { 
                semuaAngka = false; 
                break; 
            }
        }
        
        if (!semuaAngka) {
            cout << pusat_layar << ALERT_RED << "[GAGAL] Input harus murni angka!\n" << RESET;
            cin.clear(); cin.ignore(9999, '\n');
            continue;
        }
        
        nominal = stoll(inputNominalStr);
        
        if (nominal < 50000 || nominal % 50000 != 0) {
            cout << pusat_layar << ALERT_RED << "[GAGAL] Nominal harus kelipatan Rp 50.000!\n" << RESET;
            continue;
        }
        if (nominal > ptrUser->saldo) {
            cout << pusat_layar << ALERT_RED << "[GAGAL] Saldo Anda tidak mencukupi!\n" << RESET;
            continue;
        }
        
        cin.ignore(9999, '\n');
        break;
    }
                
    cout << pusat_layar << BLUE_PURE << "==========================================\n" << RESET;
    
    if (!verifikasiPinTransaksi(ptrUser->pin)) {
        cout << pusat_layar << BLUE_PURE << "==========================================\n" << RESET;
        tungguEnter();
        return;
    }
    
    ptrUser->saldo -= nominal;
    catatLogTransaksi(ptrUser, "KELUAR", "Penarikan Tunai Mandiri", nominal);
    cout << pusat_layar << SUCCESS_GRN << "[SUKSES] Silakan ambil uang Anda.\n" << RESET;
    cout << pusat_layar << TEXT_BLACK << "Sisa Saldo     : Rp " << ptrUser->saldo << RESET << "\n";
    cout << pusat_layar << BLUE_PURE << "==========================================\n" << RESET;
    tungguEnter();
}

void transfersaldo() {
    if (currentuser == nullptr) return;

    string inputRekStr, inputNominalStr;
    long long rekTujuan = 0, nominal = 0;
    
    cout << pusat_layar << BLUE_PURE << "=========================================\n" << RESET;
    cout << pusat_layar << BLUE_PURE << "              TRANSFER SALDO             \n" << RESET;
    cout << pusat_layar << BLUE_PURE << "=========================================\n" << RESET;
    while (true) {
        cout << pusat_layar << CYAN_LAUT << "  Nomor Rekening Tujuan : " << RESET; 
        cin >> inputRekStr;
        
        bool semuaAngka = true;
        for (int i = 0; i < inputRekStr.length(); i++) {
            if (!isdigit(inputRekStr[i])) { 
                semuaAngka = false; 
                break; 
            }
        }
        
        if (!semuaAngka) {
            cout << pusat_layar << ALERT_RED << "  [Error] Nomor rekening harus murni angka!\n" << RESET;
            cin.clear(); cin.ignore(9999, '\n');
            continue;
        }
        
        rekTujuan = stoll(inputRekStr);
        cin.ignore(9999, '\n');
        break;
    }

    if (rekTujuan == currentuser->data.norek) {
        cout << pusat_layar << ALERT_RED << " Tidak bisa transfer ke rekening sendiri!\n" << RESET;
        return;
    }

    akun* penerima = carinasabahByRek(rekTujuan);
    if (penerima == nullptr) {
        cout << pusat_layar << ALERT_RED << "  Nomor rekening tidak ditemukan!\n" << RESET;
        return;
    }

    if (penerima->data.isblokir) {
        cout << pusat_layar << ALERT_RED << "  Transfer gagal! Rekening tujuan sedang diblokir.\n" << RESET;
        return;
    }

    cout << pusat_layar << CYAN_LAUT << "  Nama Penerima         : " << RESET << penerima->data.nama << "\n";
    while (true) {
        cout << pusat_layar << CYAN_LAUT << "  Nominal Transfer      : " << RESET << "Rp "; 
        cin >> inputNominalStr;
        bool semuaAngka = true;
        for (int i = 0; i < inputNominalStr.length(); i++) {
            if (!isdigit(inputNominalStr[i])) { 
                semuaAngka = false; 
                break; 
            }
        }
        
        if (!semuaAngka) {
            cout << pusat_layar << ALERT_RED << "  [Error] Nominal transfer harus murni angka!\n" << RESET;
            cin.clear(); cin.ignore(9999, '\n');
            continue;
        }
        
        nominal = stoll(inputNominalStr);
        
        if (nominal < 10000) {
            cout << pusat_layar << ALERT_RED << "  [Error] Minimal transfer adalah Rp 10.000!\n" << RESET;
            continue;
        }
        if (nominal > currentuser->data.saldo) {
            cout << pusat_layar << ALERT_RED << "  [Error] Saldo Anda tidak mencukupi!\n" << RESET;
            continue;
        }
        
        cin.ignore(9999, '\n');
        break;
    }

    cout << "\n" << pusat_layar << SUCCESS_GRN << "=============== KONFIRMASI ===============\n" << RESET;
    cout << pusat_layar << "  Pengirim : " << currentuser->data.nama << "\n";
    cout << pusat_layar << "  Penerima : " << penerima->data.nama << "\n";
    cout << pusat_layar << "  Rekening : " << penerima->data.norek << "\n";
    cout << pusat_layar << "  Nominal  : Rp " << nominal << "\n";

    char konfirmasi = ambilKonfirmasiValid("Lanjutkan transfer? (Y/N) : ", 'Y', 'N');
    if (konfirmasi != 'y') {
        cout << pusat_layar << ALERT_RED << "\nTransfer dibatalkan.\n" << RESET;
        return;
    }
    
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
    cout << pusat_layar << BLUE_PURE << "======================================================" << RESET << "\n";
    cout << pusat_layar << BLUE_PURE << "          SECURITY ACCESS: VERIFIKASI ADMIN       " << RESET << "\n";
    cout << pusat_layar << "======================================================" << RESET << "\n";
    cout << pusat_layar << CYAN_LAUT << "  Username Akun : " << RESET; cin >> usn;
    cout << pusat_layar << CYAN_LAUT << "  Password Akun : " << RESET; pass = inputPasswordSensor(); 
    cout << pusat_layar << "======================================================" << RESET << "\n";

    for (int i = 0; i < 2; i++) {
        if (dataAdmin[i].username == usn && dataAdmin[i].password == pass) {
        	cout << pusat_layar << SUCCESS_GRN << "[Akses Diterima] Selamat Datang " << dataAdmin[i].namaAdmin << RESET;
        	tungguEnter();
            subMenuAdmin(i);
            return;
        }
    }
    cout << endl << pusat_layar << ALERT_RED << "[Akses Ditolak] Kredensial Salah!" << RESET << "\n"; 
    tungguEnter();
}

void loginAsUser() {
    bersihkanLayar();
    string usn, pass;
    cout << pusat_layar << BLUE_PURE << "==================================================" << RESET << "\n";
    cout << pusat_layar << BLUE_PURE << "             PORTAL OTENTIKASI NASABAH            " << RESET << "\n";
    cout << pusat_layar << BLUE_PURE << "==================================================" << RESET << "\n";
    cout << pusat_layar << CYAN_LAUT << "Username Akun : " << RESET; cin >> usn;
    cout << pusat_layar << CYAN_LAUT << "Password Akun : " << RESET; pass = inputPasswordSensor();
    cout << pusat_layar << BLUE_PURE << "==================================================" << RESET << "\n";

    akun* temp = carinasabahByUsk(usn);
    if (temp != nullptr && temp->data.password == pass) {
        if (temp->data.isblokir) {
            cout << pusat_layar << ALERT_RED << "[STATUS BLOKIR] Akun Anda Ditangguhkan oleh Admin!" << RESET << "\n"; 
            tungguEnter();
            return;
        }
        currentuser = temp;
        cout << pusat_layar << SUCCESS_GRN << "[BERHASIL] Selamat Datang " << currentuser->data.nama << RESET;
        tungguEnter();
        subMenuNasabah();
        return;
    }
    cout << "\n" << pusat_layar << ALERT_RED << "[Gagal Auth] Kombinasi Username & Password Keliru!" << RESET << "\n"; 
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
        cout << "\t\t\t\t\t\t\t     " << SUCCESS_GRN << "Petugas Aktif: " << dataAdmin[idx].namaAdmin << RESET << "\n";
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
            else if (kursor == 3) { kelolaakunnasabah(); }
            else if (kursor == 4) { riwayattransaksinasabah(); }
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

        cout << "\t\t\t\t\t\t\t     ===" << BLUE_PURE << "   INTERFACES MINI BANK CLIENT   " << RESET << "===   " << "\n";
        cout << "==================================================================================================================================================" << RESET << "\n";
        
        string namaNasabah = currentuser->data.nama;
        string noRekString = to_string(currentuser->data.norek);
        string stringSaldo = "Rp " + to_string(currentuser->data.saldo);

        int s1 = 21 - namaNasabah.length(); if(s1 < 0) s1 = 0;
        int s2 = 21 - noRekString.length(); if(s2 < 0) s2 = 0;
        int s3 = 21 - stringSaldo.length(); if(s3 < 0) s3 = 0;

        cout << "\t\t\t\t\t\t\t       ==============================" << endl;
        cout << "\t\t\t\t\t\t\t       " << TEXT_BLACK << "Nama Nasabah : " << RESET << namaNasabah << string(s1, ' ') << RESET << "\n";
        cout << "\t\t\t\t\t\t\t       " << TEXT_BLACK << "No. Rekening : " << RESET << noRekString << string(s2, ' ') << RESET << "\n";
        cout << "\t\t\t\t\t\t\t       " << TEXT_BLACK << "Saldo Utama  : " << RESET << stringSaldo << string(s3, ' ') << RESET << "\n";
        cout << "\t\t\t\t\t\t\t       ==============================" << endl;
        
        if (kursor == 1) cout << "\t\t\t\t\t\t\t\t     " << SUCCESS_GRN << "> Setor Tunai <" << RESET << "\n";
        else             cout << "\t\t\t\t\t\t\t\t       Setor Tunai  \n";

        if (kursor == 2) cout << "\t\t\t\t\t\t\t\t     " << SUCCESS_GRN << "> Tarik Tunai <" << RESET << "\n";
        else             cout << "\t\t\t\t\t\t\t\t       Tarik Tunai  \n";
        
        if (kursor == 3) cout << "\t\t\t\t\t\t\t\t   " << SUCCESS_GRN << " > Transfer Saldo <" << RESET << "\n";
        else             cout << "\t\t\t\t\t\t\t\t      Transfer Saldo  \n";

        if (kursor == 4) cout << "\t\t\t\t\t\t\t\t  " << SUCCESS_GRN << "> Riwayat Transaksi <" << RESET << "\n";
        else             cout << "\t\t\t\t\t\t\t\t    Riwayat Transaksi  \n";

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
		cout << "\t\t\t\t\t\t\t       ===============================" << endl;
        if (kursor == 1) cout << "\t\t\t\t\t\t\t       |  " << SUCCESS_GRN << "> Login Sebagai Nasabah < " << RESET << " |\n";
        else cout << "\t\t\t\t\t\t\t       |    Login Sebagai Nasabah    |\n";

        if (kursor == 2) cout << "\t\t\t\t\t\t\t       |  " << SUCCESS_GRN << ">  Login Sebagai Admin  < " << RESET << " |\n";
        else cout << "\t\t\t\t\t\t\t       |     Login Sebagai Admin     |\n";

        if (kursor == 3) cout << "\t\t\t\t\t\t\t       |  " << ALERT_RED << " >   Keluar Aplikasi   < " << RESET << "  |\n";
        else cout << "\t\t\t\t\t\t\t       |       Keluar Aplikasi       |\n";
        cout << "\t\t\t\t\t\t\t       ===============================" << endl;
            
        cout << BLUE_PURE << "==================================================================================================================================================" << RESET << "\n";
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
                cout << pusat_layar << SUCCESS_GRN << "[TERMINATED] Sesi mini bank ditutup. Terima kasih." << RESET << "\n\n";
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