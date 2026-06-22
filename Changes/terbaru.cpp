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

const int MAX_MUTASI = 30;

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

// Menggunakan pointer Linked List sebagai basis utama
akun* head = nullptr;
akun* currentuser = nullptr;
int totalnasabah = 0;

Admin dataAdmin[2] = {
    {"admin1", "passadmin1", "Budi Santoso (Auditor)"},
    {"superadmin", "root123", "IT Core Developer"}
};
pengguna dummyNasabah[2] = {
    {
        "Lintang", 
        1234, 
        "lintang", 
        "lintang123", 
        1234567890, 
        750000, 
        false, 
        {}, // riwayat kosong
        0   // totalMutasi
    },
    {
        "Suci", 
        5678, 
        "suci", 
        "suci123", 
        9876543210, 
        1200000, 
        false, 
        {}, // riwayat kosong
        0   // totalMutasi
    }
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
    cout << "\033[2J\033[H"; 
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
        if (temp->data.username == usn) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr; 
}

akun* carinasabahByRek(long long rek) {
    akun* temp = head;
    while (temp != nullptr) {
        if (temp->data.norek == rek) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}

bool apakahUserSudahAda(string usn, long long rek) {
    if (carinasabahByUsk(usn) != nullptr) {
        cout << ALERT_RED << "\n[Error] Username '" << usn << "' sudah digunakan nasabah lain!" << RESET << endl;
        return true;
    }
    if (carinasabahByRek(rek) != nullptr) {
        return true; 
    }
    return false;
}

void pendaftarannasabah() {
    bersihkanLayar();
    akun* newakun = new akun;
    cout << BLUE_PURE << "=== PENDAFTARAN NASABAH BARU ===" << RESET << endl;
    
    cin.ignore(9999, '\n'); 
    cout << "Masukkan Nama Nasabah : "; getline(cin, newakun->data.nama);
    cout << "Masukkan PIN          : "; cin >> newakun->data.pin;
    
    cin.ignore(9999, '\n');
    string tempUsn;
    cout << "Masukkan Username     : "; getline(cin, tempUsn);
    
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
    
    newakun->data.norek = tempRek;
    newakun->data.isblokir = false;
    newakun->data.totalMutasi = 0;
    newakun->next = head;
    head = newakun;
    
    catatLogTransaksi(&(newakun->data), "MASUK", "Setoran Saldo Pembuatan Akun", newakun->data.saldo);
    cout << SUCCESS_GRN << "\nPendaftaran Berhasil! No Rekening: " << newakun->data.norek << RESET << endl;
    totalnasabah++;
    tungguEnter();
}

void lihatdatanasabah() {
    cout << BLUE_PURE << "======================================\n";
    cout << "          DATA SELURUH NASABAH        \n";
    cout << "======================================\n" << RESET;

    if (head == nullptr) {
        cout << ALERT_RED << "Belum ada data nasabah.\n" << RESET;
        return;
    }

    akun* temp = head;
    int no = 1;

    while (temp != nullptr) {
        cout << BLUE_PURE
             << "--------------------------------------\n"
             << RESET;

        cout << TEXT_BLACK << "Nasabah Ke-" << no << RESET << endl;
        cout << "Nama           : " << temp->data.nama << endl;
        cout << "No Rekening    : " << temp->data.norek << endl;
        cout << "Username       : " << temp->data.username << endl;
        cout << "Saldo          : Rp " << temp->data.saldo << endl;
        cout << "Status Akun    : ";

        if (temp->data.isblokir)
            cout << ALERT_RED << "DIBLOKIR" << RESET << endl;
        else
            cout << SUCCESS_GRN << "AKTIF" << RESET << endl;

        temp = temp->next;
        no++;
    }

    cout << BLUE_PURE
         << "--------------------------------------\n"
         << RESET;
    cout << "Total Nasabah : " << totalnasabah << endl;
}

void kelolaakunnasabah() {
    system("cls");
    cout << BLUE_PURE
         << "======================================\n";
    cout << "         KELOLA AKUN NASABAH          \n";
    cout << "======================================\n"
         << RESET;

    if (head == nullptr) {
        cout << ALERT_RED
             << "Belum ada data nasabah.\n"
             << RESET;
        return;
    }

    long long cariRek;
    cout << "Masukkan No Rekening : ";
    cin >> cariRek;

    akun* temp = head;
    akun* prev = nullptr;

    while (temp != nullptr) {
        if (temp->data.norek == cariRek) {

            cout << "\nData Ditemukan\n";
            cout << "Nama         : " << temp->data.nama << endl;
            cout << "No Rekening  : " << temp->data.norek << endl;
            cout << "Saldo        : Rp " << temp->data.saldo << endl;

            cout << "Status       : ";
            if (temp->data.isblokir)
                cout << ALERT_RED << "DIBLOKIR" << RESET << endl;
            else
                cout << SUCCESS_GRN << "AKTIF" << RESET << endl;

            int pilih;

            do {
                cout << "\n1. Blokir Akun\n";
                cout << "2. Buka Blokir Akun\n";
                cout << "3. Hapus Akun\n";
                cout << "4. Batal\n";
                cout << "Pilih : ";
                cin >> pilih;

                if (pilih < 1 || pilih > 4) {
                    cout << ALERT_RED
                         << "Pilihan tidak valid!\n"
                         << RESET;
                }

            } while (pilih < 1 || pilih > 4);

            char konfirmasi;

            switch (pilih) {

            case 1:
                cout << "Yakin ingin memblokir akun ini? (Y/T) : ";
                cin >> konfirmasi;

                while (konfirmasi != 'Y' && konfirmasi != 'y' &&
                       konfirmasi != 'T' && konfirmasi != 't') {
                    cout << ALERT_RED
                         << "Input tidak valid! Masukkan Y atau T.\n"
                         << RESET;

                    cout << "Yakin ingin memblokir akun ini? (Y/T) : ";
                    cin >> konfirmasi;
                }

                if (konfirmasi == 'Y' || konfirmasi == 'y') {
                    if (temp->data.isblokir) {
                        cout << ALERT_RED
                             << "Akun sudah diblokir.\n"
                             << RESET;
                    } else {
                        temp->data.isblokir = true;
                        cout << SUCCESS_GRN
                             << "Akun berhasil diblokir.\n"
                             << RESET;
                    }
                } else {
                    cout << GOLD_GOLD
                         << "Pemblokiran dibatalkan.\n"
                         << RESET;
                }
                break;

            case 2:
                cout << "Yakin ingin membuka blokir akun ini? (Y/T) : ";
                cin >> konfirmasi;

                while (konfirmasi != 'Y' && konfirmasi != 'y' &&
                       konfirmasi != 'T' && konfirmasi != 't') {
                    cout << ALERT_RED
                         << "Input tidak valid! Masukkan Y atau T.\n"
                         << RESET;

                    cout << "Yakin ingin membuka blokir akun ini? (Y/T) : ";
                    cin >> konfirmasi;
                }

                if (konfirmasi == 'Y' || konfirmasi == 'y') {
                    if (!temp->data.isblokir) {
                        cout << ALERT_RED
                             << "Akun sudah aktif.\n"
                             << RESET;
                    } else {
                        temp->data.isblokir = false;
                        cout << SUCCESS_GRN
                             << "Akun berhasil diaktifkan.\n"
                             << RESET;
                    }
                } else {
                    cout << GOLD_GOLD
                         << "Pembukaan blokir dibatalkan.\n"
                         << RESET;
                }
                break;

            case 3:
                cout << "Yakin ingin menghapus akun ini? (Y/T) : ";
                cin >> konfirmasi;

                while (konfirmasi != 'Y' && konfirmasi != 'y' &&
                       konfirmasi != 'T' && konfirmasi != 't') {
                    cout << ALERT_RED
                         << "Input tidak valid! Masukkan Y atau T.\n"
                         << RESET;

                    cout << "Yakin ingin menghapus akun ini? (Y/T) : ";
                    cin >> konfirmasi;
                }

                if (konfirmasi == 'Y' || konfirmasi == 'y') {

                    if (temp == head)
                        head = head->next;
                    else
                        prev->next = temp->next;

                    delete temp;
                    totalnasabah--;

                    cout << SUCCESS_GRN
                         << "Akun berhasil dihapus.\n"
                         << RESET;
                } else {
                    cout << GOLD_GOLD
                         << "Penghapusan dibatalkan.\n"
                         << RESET;
                }
                break;

            case 4:
                cout << "Operasi dibatalkan.\n";
                break;
            }

            return;
        }

        prev = temp;
        temp = temp->next;
    }

    cout << ALERT_RED
         << "Nomor rekening tidak ditemukan.\n"
         << RESET;
}

void setortunai() {
    bersihkanLayar(); 
    if (currentuser == nullptr) {
        cout << ALERT_RED << "[ERROR] Tidak ada sesi nasabah aktif!" << RESET << "\n";
        return;
    }

    long long nominal;
    buatSpasiMenu(1);
    cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
    cout << "                    " << BLUE_PURE << "        FASILITAS SETOR TUNAI         " << RESET << "\n";
    cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
    buatSpasiMenu(1);
                
    cout << "                    " << TEXT_BLACK << "Saldo Saat Ini : " << RESET << GOLD_GOLD << "Rp " << currentuser->data.saldo << RESET << "\n";
    cout << "                    " << CYAN_LAUT << "Nominal Setor  : Rp " << RESET; cin >> nominal;
                
    buatSpasiMenu(1);
    cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";

    if (cin.fail() || nominal <= 0) {
        cin.clear();
        cin.ignore(9999, '\n');
        cout << "                    " << ALERT_RED << "[GAGAL] Input nominal tidak valid!" << RESET << "\n";
    } else if (nominal % 50000 != 0) {
        cout << "                    " << ALERT_RED << "[GAGAL] Setoran harus kelipatan Rp 50.000!" << RESET << "\n";
    } else {
        currentuser->data.saldo += nominal;
        catatLogTransaksi(&(currentuser->data), "MASUK", "Setor Tunai Mandiri", nominal);
                    
        cout << "                    " << SUCCESS_GRN << "[SUKSES] Uang berhasil disetor." << RESET << "\n";
        cout << "                    " << TEXT_BLACK << "Saldo Terbaru  : " << RESET << GOLD_GOLD << "Rp " << currentuser->data.saldo << RESET << "\n";
    }
    cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
}

void riwayattransaksi() { 
    if (currentuser == nullptr) {
        cout << ALERT_RED << "Tidak ada sesi nasabah yang aktif!" << RESET << "\n";
        return;
    }

    pengguna* ptrUser = &(currentuser->data);

    cout << BLUE_PURE << "======================================" << RESET << "\n";
    cout << BLUE_PURE << "          MUTASI REKENING NASABAH      " << RESET << "\n";
    cout << BLUE_PURE << "======================================" << RESET << "\n";
    cout << TEXT_BLACK << "No Rekening : " << RESET << ptrUser->norek << "\n";
    cout << TEXT_BLACK << "Nama        : " << RESET << ptrUser->nama << "\n";
    cout << BLUE_PURE << "--------------------------------------" << RESET << "\n";

    if (ptrUser->totalMutasi == 0) {
        cout << "       Belum ada riwayat transaksi.    \n";
        cout << BLUE_PURE << "======================================" << RESET << "\n";
        return;
    }

    for (int i = 0; i < ptrUser->totalMutasi; i++) {
        cout << i + 1 << ". [" << GOLD_GOLD << ptrUser->riwayat[i].tipe << RESET << "] "
             << ptrUser->riwayat[i].detail << "\n"
             << "   Jumlah: " << SUCCESS_GRN << "Rp " << ptrUser->riwayat[i].jumlah << RESET << "\n";
        cout << SUB_GRAY << "   ----------------------------------" << RESET << "\n";
    }
    cout << BLUE_PURE << "======================================" << RESET << "\n";
}

void riwayattransaksinasabah() { 
    long long rekTarget;
    cout << BLUE_PURE << "======================================" << RESET << "\n";
    cout << BLUE_PURE << "      [ADMIN] MONITORING LOG NASABAH   " << RESET << "\n";
    cout << BLUE_PURE << "======================================" << RESET << "\n";
    cout << CYAN_LAUT << "Masukkan Nomor Rekening Target: " << RESET; 
    cin >> rekTarget;
    cout << BLUE_PURE << "======================================" << RESET << "\n";

    akun* temp = carinasabahByRek(rekTarget);

    if (temp != nullptr) {
        cout << SUCCESS_GRN << "[DATA KETEMU] Mengalihkan ke riwayat...\n" << RESET;
        tungguEnter();
        bersihkanLayar();
        
        cout << BLUE_PURE << "======================================" << RESET << "\n";
        cout << BLUE_PURE << "       [ADMIN MODE] DETAIL MUTASI       " << RESET << "\n";
        cout << BLUE_PURE << "======================================" << RESET << "\n";
        cout << TEXT_BLACK << "No Rekening : " << RESET << temp->data.norek << "\n";
        cout << TEXT_BLACK << "Nama Nasabah: " << RESET << temp->data.nama << "\n";
        cout << BLUE_PURE << "--------------------------------------" << RESET << "\n";

        if (temp->data.totalMutasi == 0) {
            cout << "       Belum ada riwayat transaksi.    \n";
        } else {
            for (int i = 0; i < temp->data.totalMutasi; i++) {
                cout << i + 1 << ". [" << GOLD_GOLD << temp->data.riwayat[i].tipe << RESET << "] "
                     << temp->data.riwayat[i].detail << "\n"
                     << "   Jumlah: " << SUCCESS_GRN << "Rp " << temp->data.riwayat[i].jumlah << RESET << "\n";
                cout << SUB_GRAY << "   ----------------------------------" << RESET << "\n";
            }
        }
        cout << BLUE_PURE << "======================================" << RESET << "\n";
    } else {
        cout << ALERT_RED << "[GAGAL] Nomor rekening tidak ditemukan di sistem!" << RESET << "\n";
        cout << BLUE_PURE << "======================================" << RESET << "\n";
    }
}

void tariktunai(pengguna* ptrUser) { 
    bersihkanLayar(); 
    long long nominal;
                
    buatSpasiMenu(1);
    cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
    cout << "                    " << BLUE_PURE << "       FASILITAS PENARIKAN TUNAI       " << RESET << "\n";
    cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
    buatSpasiMenu(1);
                
    cout << "                    " << TEXT_BLACK << "Saldo Tersedia : " << RESET << GOLD_GOLD << "Rp " << ptrUser->saldo << RESET << "\n";
    cout << "                    " << CYAN_LAUT << "Nominal Tarik  : Rp " << RESET; cin >> nominal;
                
    buatSpasiMenu(1);
    cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";

    if (nominal <= 0 || nominal % 50000 != 0) {
        cout << "                    " << ALERT_RED << "[GAGAL] Nominal harus kelipatan Rp 50.000!" << RESET << "\n";
    } else if (nominal > ptrUser->saldo) {
        cout << "                    " << ALERT_RED << "[GAGAL] Saldo Anda tidak mencukupi!" << RESET << "\n";
    } else {
        ptrUser->saldo -= nominal;
        catatLogTransaksi(ptrUser, "KELUAR", "Penarikan Tunai Mandiri", nominal);
                    
        cout << "                    " << SUCCESS_GRN << "[SUKSES] Silakan ambil uang Anda." << RESET << "\n";
        cout << "                    " << TEXT_BLACK << "Sisa Saldo     : " << RESET << GOLD_GOLD << "Rp " << ptrUser->saldo << RESET << "\n";
    }
                
    cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
    tungguEnter();
}

void transfersaldo() {
    if (currentuser == nullptr) return;

    long long rekTujuan;
    long long nominal;
    char konfirmasi;

    cout << BLUE_PURE << "======================================\n";
    cout << "              TRANSFER SALDO            \n";
    cout << "======================================" << RESET << endl;

    cout << CYAN_LAUT << "Nomor Rekening Tujuan : " << RESET;
    cin >> rekTujuan;

    if (rekTujuan == currentuser->data.norek) {
        cout << ALERT_RED << "\nTidak bisa transfer ke rekening sendiri!" << RESET << endl;
        return;
    }

    akun* penerima = carinasabahByRek(rekTujuan);

    if (penerima == nullptr) {
        cout << ALERT_RED << "\nNomor rekening tidak ditemukan!" << RESET << endl;
        return;
    }

    if (penerima->data.isblokir) {
        cout << ALERT_RED << "\nTransfer gagal! Rekening tujuan sedang diblokir." << RESET << endl;
        return;
    }

    cout << SUCCESS_GRN << "\nNama Penerima : " << RESET << penerima->data.nama << endl;
    cout << CYAN_LAUT << "Nominal Transfer : Rp " << RESET; cin >> nominal;

    if (nominal <= 0 || nominal < 10000 || nominal > currentuser->data.saldo) {
        cout << ALERT_RED << "\nNominal tidak valid atau saldo tidak cukup (Min. Rp 10.000)!" << RESET << endl;
        return;
    }

    cout << GOLD_GOLD << "\n========== KONFIRMASI ==========" << RESET << endl;
    cout << "Pengirim : " << currentuser->data.nama << endl;
    cout << "Penerima : " << penerima->data.nama << endl;
    cout << "Rekening : " << penerima->data.norek << endl;
    cout << "Nominal  : Rp " << nominal << endl;

    cout << "\nLanjutkan transfer? (Y/N) : "; cin >> konfirmasi;

    if (konfirmasi != 'Y' && konfirmasi != 'y') {
        cout << GOLD_GOLD << "\nTransfer dibatalkan." << RESET << endl;
        return;
    }
    currentuser->data.saldo -= nominal;
    penerima->data.saldo += nominal;

    catatLogTransaksi(&(currentuser->data), "KELUAR", "Transfer ke " + penerima->data.nama, nominal);
    catatLogTransaksi(&(penerima->data), "MASUK", "Transfer dari " + currentuser->data.nama, nominal);

    cout << SUCCESS_GRN << "\n======================================\n";
    cout << "           TRANSFER BERHASIL!           \n";
    cout << "======================================" << RESET << endl;
}

void loginAsAdmin() {
    bersihkanLayar(); 
    string usn, pass;
    cout << BLUE_PURE << "======================================" << RESET << "\n";
    cout << BLUE_PURE << "    SECURITY ACCESS: VERIFIKASI ADMIN  " << RESET << "\n";
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
    bersihkanLayar();
    string usn, pass;
    cout << BLUE_PURE << "======================================" << RESET << "\n";
    cout << BLUE_PURE << "         PORTAL OTENTIKASI NASABAH    " << RESET << "\n";
    cout << BLUE_PURE << "======================================" << RESET << "\n";
    cout << CYAN_LAUT << "Username Akun : " << RESET; cin >> usn;
    cout << CYAN_LAUT << "Password PIN  : " << RESET; pass = inputPasswordSensor();
    cout << BLUE_PURE << "======================================" << RESET << "\n";
    
    akun* temp = carinasabahByUsk(usn);
    if (temp != nullptr && temp->data.password == pass) {
        if (temp->data.isblokir) {
            cout << ALERT_RED << "[STATUS BLOKIR] Akun Anda Ditangguhkan oleh Admin!" << RESET << "\n"; 
            tungguEnter();
            return;
        }
        currentuser = temp;
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
        cout << BLUE_PURE << "======================================" << RESET << "\n";
        cout << BLUE_PURE << "         KONSOL OPERATOR SUPERVISOR    " << RESET << "\n";
        cout << BLUE_PURE << "======================================" << RESET << "\n";
        cout << SUB_GRAY << "Petugas Aktif: " << dataAdmin[idx].namaAdmin << RESET << "\n";
        
        if (kursor == 1) cout << CYAN_LAUT << "> Pendaftaran Nasabah Baru <" << RESET << "\n";
        else cout << "Pendaftaran Nasabah Baru  \n";

        if (kursor == 2) cout << CYAN_LAUT << "> Lihat Data Nasabah <" << RESET << "\n";
        else cout << "Lihat Data Nasabah  \n";

        if (kursor == 3) cout << CYAN_LAUT << "> Kelola Akun Nasabah <" << RESET << "\n";
        else cout << "Kelola Akun Nasabah  \n";

        if (kursor == 4) cout << CYAN_LAUT << "> Riwayat Transaksi Nasabah <" << RESET << "\n";
        else cout << "Riwayat Transaksi Nasabah  \n";

        if (kursor == 5) cout << ALERT_RED << "> Logout Sesi <" << RESET << "\n";
        else cout << "Logout Sesi  \n";
        
        cout << BLUE_PURE << "======================================" << RESET << "\n";
        cout << SUB_GRAY << "Navigasi: Panah Atas/Bawah | Enter untuk memilih" << RESET << "\n";
        tombol = _getch();
        if (tombol == 0 || tombol == -32 || tombol == 224) {
            tombol = _getch();
            if (tombol == 72) kursor = (kursor == 1) ? totalOpsi : kursor - 1;
            else if (tombol == 80) kursor = (kursor == totalOpsi) ? 1 : kursor + 1;
        } else if (tombol == '\r') {
            if (kursor == 1) pendaftarannasabah(); 
            else if (kursor == 2) { lihatdatanasabah(); tungguEnter(); }
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
        cout << BLUE_PURE << "======================================" << RESET << "\n";
        cout << BLUE_PURE << "        INTERFACES MINI BANK CLIENT    " << RESET << "\n"; 
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
        
        if (kursor == 1) cout << SUCCESS_GRN << "> Setor Tunai <" << RESET << "\n";
        else cout << "Setor Tunai  \n";

        if (kursor == 2) cout << SUCCESS_GRN << "> Tarik Tunai <" << RESET << "\n";
        else cout << "Tarik Tunai  \n";
        
        if (kursor == 3) cout << SUCCESS_GRN << "> Transfer Saldo <" << RESET << "\n";
        else cout << "Transfer Saldo  \n";

        if (kursor == 4) cout << SUCCESS_GRN << "> Riwayat Transaksi <" << RESET << "\n";
        else cout << "Riwayat Transaksi  \n";

        if (kursor == 5) cout << ALERT_RED << "> Logout / Selesai <" << RESET << "\n";
        else cout << "Logout / Selesai  \n";
        
        cout << BLUE_PURE << "======================================" << RESET << "\n";
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
                cout << endl << SUCCESS_GRN << "[OK] Kartu dikeluarkan. Sesi aman berakhir." << RESET << "\n";
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

        if (head == nullptr) {
            head = newakun;
        } else {
            newakun->next = head;
            head = newakun;
        }
        totalnasabah++;
    }
}

void menuUtama() {
   static bool isInitialized = false;
    if (!isInitialized) {
        srand(time(0));
        tambahNasabahDariArray(dummyNasabah, 2);
        isInitialized = true; // Mengunci agar tidak terjadi duplikasi data jika kembali ke menu utama
    }
    int kursor = 1;
    const int totalOpsi = 3;
    char tombol;

    while (true) {
        bersihkanLayar();
        cout << BLUE_PURE << "======================================" << RESET << "\n";
        cout << BLUE_PURE << "    BANK CENTRAL ASIA - MINI BANK CORE " << RESET << "\n";
        cout << BLUE_PURE << "======================================" << RESET << "\n";
        buatSpasiMenu(1);

        if (kursor == 1) cout << SUCCESS_GRN << "> Login Sebagai Nasabah <" << RESET << "\n";
        else cout << "  Login Sebagai Nasabah   \n";

        if (kursor == 2) cout << SUCCESS_GRN << "> Login Sebagai Admin <" << RESET << "\n";
        else cout << "  Login Sebagai Admin   \n";

        if (kursor == 3) cout << ALERT_RED << "> Keluar Aplikasi <" << RESET << "\n";
        else cout << "  Keluar Aplikasi   \n" << endl;
            
        cout << BLUE_PURE << "======================================" << RESET << "\n";
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