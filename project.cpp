#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <ctime>   
#include <conio.h> 

using namespace std;

// ============================================================================
// [MODUL 1]: PEMROGRAMAN DASAR & KONSTANTA GLOBAL
// ============================================================================
const int MAX_NASABAH = 50;
const int MAX_MUTASI = 30;

// Palet Warna UI Sistem Mini Bank Kontras Tinggi Sesuai Sketsa
const string RESET       = "\033[0m";
const string ALERT_RED   = "\033[31;1m";    
const string SUCCESS_GRN = "\033[32;1m";    
const string GOLD_GOLD   = "\033[33;1m";    
const string CYAN_LAUT   = "\033[36;1m";    
const string SUB_GRAY    = "\033[90m";      
const string TEXT_BLACK  = "\033[30;1m";    // Hitam pekat untuk teks informasi & bintang password
const string BLUE_PURE   = "\033[34;1m";    // Warna Biru Murni (#0000FF)

// ============================================================================
// [MODUL 5]: STRUCT (STRUKTUR DATA BENTUKAN)
// ============================================================================
struct Transaksi {
    string tipe; 
    string detail;
    long long jumlah; 
};

struct Nasabah {
    string nama;
    string pin;
    string username;
    string password;
    string noRekening;
    long long saldo;
    bool terblokir;      
    bool apakahAktif;    
    
    Transaksi riwayat[MAX_MUTASI];
    int totalMutasi = 0;
};

struct Admin {
    string username;
    string password;
    string namaAdmin;
};

// ============================================================================
// [MODUL 3]: ARRAY (DATABASE STATIS)
// ============================================================================
Nasabah dataNasabah[MAX_NASABAH];
int slotNasabahKini = 0;

const int TOTAL_ADMIN_DUMMY = 2;
Admin dataAdmin[TOTAL_ADMIN_DUMMY] = {
    {"admin1", "passadmin1", "Budi Santoso (Auditor)"},
    {"superadmin", "root123", "IT Core Developer"}
};

// ============================================================================
// [MODUL 4]: FUNCTION & SUB-PROGRAM HELPERS
// ============================================================================
void bersihkanLayar() {
    cout << "\033[2J\033[H"; 
}

void tungguEnter() {
    cout << "\n\n                               " << SUB_GRAY << "Tekan ENTER untuk melanjutkan..." << RESET;
    while (_getch() != '\r'); 
}

void buatSpasiMenu(int jumlah) {
    for (int i = 0; i < jumlah; i++) cout << "\n";
}

int cariIndeksRekening(string norek) {
    for (int i = 0; i < slotNasabahKini; i++) {
        if (dataNasabah[i].apakahAktif && dataNasabah[i].noRekening == norek) return i;
    }
    return -1;
}

string generateNomorRekeningUnik() {
    int angkaAcak = (rand() % (999999 - 100000 + 1)) + 100000;
    string kandidatNorek = to_string(angkaAcak);
    if (cariIndeksRekening(kandidatNorek) != -1) return generateNomorRekeningUnik(); 
    return kandidatNorek;
}

void catatLogTransaksi(Nasabah* n, string tipe, string detail, long long nominal) {
    int pos = n->totalMutasi;
    if (pos < MAX_MUTASI) {
        n->riwayat[pos] = {tipe, detail, nominal};
        n->totalMutasi++;
    }
}

// Fungsi pembantu khusus untuk input sensor bintang berwarna hitam pekat
string inputPasswordSensor() {
    string pass = "";
    char ch;
    while (true) {
        ch = _getch();
        if (ch == '\r') { 
            break;
        } 
        else if (ch == '\b') { 
            if (pass.length() > 0) {
                pass.pop_back();
                cout << "\b \b"; 
            }
        } 
        else if (ch != 0 && ch != -32) { 
            pass.push_back(ch);
            cout << TEXT_BLACK << "*" << RESET; 
        }
    }
    cout << endl;
    return pass;
}

// Prototipe Menu Utama
void menuUtama();
void loginAsAdmin();
void loginAsUser();
void subMenuAdmin(int idxAdmin);
void subMenuNasabah(Nasabah* ptrUser);

// ============================================================================
// MAIN FUNCTION PROGRAM
// ============================================================================
int main() {
    srand(time(0)); 
    
    // Data Awal Dummy untuk simulasi core bank
    dataNasabah[0] = {"Ahmad Fauzi", "1234", "ahmad123", "passahmad", "102938", 750000, false, true};
    catatLogTransaksi(&dataNasabah[0], "MASUK", "Setoran Saldo Pembuatan Akun", 750000);
    slotNasabahKini++;

    dataNasabah[1] = {"Rian Hidayat", "8888", "rian_hd", "passrian", "554422", 24500000, false, true};
    catatLogTransaksi(&dataNasabah[1], "MASUK", "Penerimaan Kliring Payroll", 24500000);
    slotNasabahKini++;

    menuUtama();
    return 0;
}

// ============================================================================
// C. MAIN MENU CONSOLE (SESUAI REVISI POSISI NAVIGASI DI LUAR BAWAH BOX)
// ============================================================================
void menuUtama() {
    int kursor = 1;
    const int totalOpsi = 3;
    char tombol;

    while (true) {
        bersihkanLayar();
        buatSpasiMenu(3);
        
        // Header Bingkai Atas Biru Murni
        cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
        cout << "                    " << BLUE_PURE << "  BANK CENTRAL ASIA - MINI BANK CORE  " << RESET << "\n";
        cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
        buatSpasiMenu(1);

        if (kursor == 1) 
            cout << "                         " << SUCCESS_GRN << "> Login Sebagai Nasabah <" << RESET << "\n";
        else 
            cout << "                           Login Sebagai Nasabah  \n";

        if (kursor == 2) 
            cout << "                          " << SUCCESS_GRN << "> Login Sebagai Admin <" << RESET << "\n";
        else 
            cout << "                            Login Sebagai Admin  \n";

        if (kursor == 3) 
            cout << "                               " << ALERT_RED << "> Keluar Akun <" << RESET << "\n";
        else 
            cout << "                                 Keluar Akun  \n";

        buatSpasiMenu(1);
        cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
        
        // POSISI BARU: Petunjuk Navigasi berada di luar area penutup menu utama (paling bawah)
        cout << "               " << SUB_GRAY << "Navigasi: Panah / PageUp / PageDown | Enter untuk memilih" << RESET << "\n";
        buatSpasiMenu(1);
        
        tombol = _getch();
        
        if (tombol == 0 || tombol == -32) {
            tombol = _getch();
            if (tombol == 72 || tombol == 73) kursor = (kursor == 1) ? totalOpsi : kursor - 1;
            else if (tombol == 80 || tombol == 81) kursor = (kursor == totalOpsi) ? 1 : kursor + 1;
        } 
        else if (tombol == '\r') { 
            if (kursor == 1) loginAsUser();
            else if (kursor == 2) loginAsAdmin();
            else if (kursor == 3) {
                bersihkanLayar();
                buatSpasiMenu(2);
                cout << "                 " << SUCCESS_GRN << "[TERMINATED] Sesi mini bank ditutup. Terima kasih." << RESET << "\n\n";
                break;
            }
        }
    }
}

void loginAsAdmin() {
    bersihkanLayar();
    string usn, pass;
    buatSpasiMenu(2);
    cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
    cout << "                    " << BLUE_PURE << "   SECURITY ACCESS: VERIFIKASI ADMIN  " << RESET << "\n";
    cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
    buatSpasiMenu(1);
    
    cout << "                     " << CYAN_LAUT << "Username ID Admin : " << RESET; cin >> usn;
    cout << "                     " << CYAN_LAUT << "Sandi Pengawas    : " << RESET; pass = inputPasswordSensor(); 

    buatSpasiMenu(1);
    cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";

    for (int i = 0; i < TOTAL_ADMIN_DUMMY; i++) {
        if (dataAdmin[i].username == usn && dataAdmin[i].password == pass) {
            subMenuAdmin(i);
            return;
        }
    }
    cout << "\n                    " << ALERT_RED << "[Akses Ditolak] Kredensial Salah!" << RESET << "\n"; 
    tungguEnter();
}

void loginAsUser() {
    bersihkanLayar();
    string usn, pass;
    buatSpasiMenu(2);
    cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
    cout << "                    " << BLUE_PURE << "       PORTAL OTENTIKASI NASABAH      " << RESET << "\n";
    cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
    buatSpasiMenu(1);
    
    cout << "                     " << CYAN_LAUT << "Username Akun : " << RESET; cin >> usn;
    cout << "                     " << CYAN_LAUT << "Password PIN  : " << RESET; pass = inputPasswordSensor();

    buatSpasiMenu(1);
    cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";

    for (int i = 0; i < slotNasabahKini; i++) {
        if (dataNasabah[i].apakahAktif && dataNasabah[i].username == usn && dataNasabah[i].password == pass) {
            if (dataNasabah[i].terblokir) {
                cout << "\n                    " << ALERT_RED << "[STATUS BLOKIR] Akun Anda Ditangguhkan oleh Admin!" << RESET << "\n"; 
                tungguEnter();
                return;
            }
            subMenuNasabah(&dataNasabah[i]);
            return;
        }
    }
    cout << "\n                    " << ALERT_RED << "[Gagal Auth] Kombinasi Username & Password Keliru!" << RESET << "\n"; 
    tungguEnter();
}

// ============================================================================
// A. KONSOL OPERASIONAL ADMIN
// ============================================================================
void subMenuAdmin(int idxAdmin) {
    int kursor = 1; 
    const int totalOpsi = 5; 
    char tombol;

    while (true) {
        bersihkanLayar();
        buatSpasiMenu(2);
        cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
        cout << "                    " << BLUE_PURE << "      KONSOL OPERATOR SUPERVISOR      " << RESET << "\n";
        cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
        cout << "                     " << SUB_GRAY << "Petugas Aktif: " << dataAdmin[idxAdmin].namaAdmin << RESET << "\n";
        buatSpasiMenu(2);

        if (kursor == 1)
        cout << "                       " << CYAN_LAUT << "> Pendaftaran Nasabah Baru <" << RESET << "\n";
        else
            cout << "                         Pendaftaran Nasabah Baru  \n";

        if (kursor == 2)
            cout << "                          " << CYAN_LAUT << "> Lihat Data Nasabah <" << RESET << "\n";
        else
            cout << "                            Lihat Data Nasabah  \n";

        if (kursor == 3)
            cout << "                         " << CYAN_LAUT << "> Kelola Akun Nasabah <" << RESET << "\n";
        else
            cout << "                           Kelola Akun Nasabah  \n";

        if (kursor == 4)
            cout << "                       " << CYAN_LAUT << "> Riwayat Transaksi Nasabah <" << RESET << "\n";
        else
            cout << "                         Riwayat Transaksi Nasabah  \n";

        if (kursor == 5)
            cout << "                              " << ALERT_RED << "> Logout Sesi <" << RESET << "\n";
        else
            cout << "                                Logout Sesi  \n";

        buatSpasiMenu(1);
        cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
        
        // Navigasi luar kotak untuk halaman admin
        cout << "               " << SUB_GRAY << "Navigasi: Panah / PageUp / PageDown | Enter untuk memilih" << RESET << "\n";
        buatSpasiMenu(1);

        tombol = _getch();
        if (tombol == 0 || tombol == -32) {
            tombol = _getch();
            if (tombol == 72 || tombol == 73) kursor = (kursor == 1) ? totalOpsi : kursor - 1;
            else if (tombol == 80 || tombol == 81) kursor = (kursor == totalOpsi) ? 1 : kursor + 1;
        } 
        else if (tombol == '\r') {
            if (kursor == 5) break;

            if (kursor == 1) { 
                bersihkanLayar();
                if (slotNasabahKini >= MAX_NASABAH) { cout << " Database Penuh!\n"; tungguEnter(); continue; }
                Nasabah n;
                buatSpasiMenu(1);
                cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
                cout << "                    " << BLUE_PURE << "   FORMULIR PENDAFTARAN NASABAH BARU  " << RESET << "\n";
                cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
                buatSpasiMenu(1);
                cout << "                    " << CYAN_LAUT << "Nama Sesuai KTP        : " << RESET; cin.ignore(); getline(cin, n.nama);
                cout << "                    " << CYAN_LAUT << "PIN Rekening (4 angka) : " << RESET; n.pin = inputPasswordSensor();
                cout << "                    " << CYAN_LAUT << "Daftarkan Username     : " << RESET; cin >> n.username;
                cout << "                    " << CYAN_LAUT << "Daftarkan Password     : " << RESET; n.password = inputPasswordSensor();
                
                buatSpasiMenu(1);
                cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";

                n.noRekening = generateNomorRekeningUnik();
                n.saldo = 0; n.terblokir = false; n.apakahAktif = true; n.totalMutasi = 0;
                
                dataNasabah[slotNasabahKini] = n;
                catatLogTransaksi(&dataNasabah[slotNasabahKini], "MASUK", "Registrasi Awal Bank", 0);
                slotNasabahKini++;
                
                cout << "\n                    " << SUCCESS_GRN << "[SUKSES] Akun Terbuka! No Rekening Baru: " << n.noRekening << RESET << "\n"; 
                tungguEnter();
            }
            else if (kursor == 2) { 
                bersihkanLayar();
                buatSpasiMenu(1);
                cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
                cout << "                    " << BLUE_PURE << "     DATA MASTER REKENING NASABAH     " << RESET << "\n";
                cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
                buatSpasiMenu(1);
                for (int i = 0; i < slotNasabahKini; i++) {
                    if (dataNasabah[i].apakahAktif) {
                        string statusAkun = dataNasabah[i].terblokir ? (ALERT_RED + "TERBLOKIR") : (SUCCESS_GRN + "AKTIF");
                        int spacePad = 15 - dataNasabah[i].nama.length();
                        if (spacePad < 1) spacePad = 1;
                        cout << "             " << BLUE_PURE << "» " << RESET << TEXT_BLACK << "Nama: " << RESET << GOLD_GOLD << dataNasabah[i].nama << string(spacePad, ' ') << RESET 
                             << TEXT_BLACK << " | Rek: " << RESET << GOLD_GOLD << dataNasabah[i].noRekening << RESET
                             << " [" << statusAkun << RESET << "]\n";
                    }
                }
                buatSpasiMenu(1);
                cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
                tungguEnter();
            }
            else if (kursor == 3) { 
                bersihkanLayar();
                string rek;
                cout << "\n                    Masukkan Nomor Rekening Target: "; cin >> rek;
                int idx = cariIndeksRekening(rek);
                if (idx != -1) {
                    cout << "                    Pemilik Sah: " << GOLD_GOLD << dataNasabah[idx].nama << RESET << "\n\n";
                    cout << "                    [1] Blokir Akun (Toggle Status)\n                    [2] Hapus Akun\n\n                    Pilih tindakan: ";
                    int opsiAksi; cin >> opsiAksi;
                    if (opsiAksi == 1) {
                        dataNasabah[idx].terblokir = !dataNasabah[idx].terblokir;
                        cout << "\n                    " << SUCCESS_GRN << "[OK] Status Blokir Akun Berhasil Diubah!" << RESET << "\n";
                    } else if (opsiAksi == 2) {
                        dataNasabah[idx].apakahAktif = false; 
                        cout << "\n                    " << ALERT_RED << "[OK] Akun Telah Dihapus dari Core System!" << RESET << "\n";
                    }
                } else cout << "                    Data nomor rekening tidak ditemukan.\n";
                tungguEnter();
            }
            else if (kursor == 4) { 
                bersihkanLayar();
                string rek;
                cout << "\n                    Masukkan No Rekening Audit: "; cin >> rek;
                int idx = cariIndeksRekening(rek);
                if (idx != -1) {
                    bersihkanLayar(); 
                    buatSpasiMenu(1);
                    cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
                    cout << "                    " << BLUE_PURE << "     JOURNAL AUDIT INTERNAL MUTASI    " << RESET << "\n";
                    cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
                    cout << "                    Nama    : " << GOLD_GOLD << dataNasabah[idx].nama << RESET << "\n";
                    cout << "                    No. Rek : " << GOLD_GOLD << dataNasabah[idx].noRekening << RESET << "\n";
                    buatSpasiMenu(1);
                    for (int j = 0; j < dataNasabah[idx].totalMutasi; j++) {
                        string w = (dataNasabah[idx].riwayat[j].tipe == "MASUK") ? SUCCESS_GRN : ALERT_RED;
                        cout << "             " << w << "[" << dataNasabah[idx].riwayat[j].tipe << "] " << RESET 
                             << dataNasabah[idx].riwayat[j].detail << " -> " << GOLD_GOLD << "Rp " << dataNasabah[idx].riwayat[j].jumlah << RESET << "\n";
                    }
                    buatSpasiMenu(1);
                    cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
                } else cout << "                    Nomor rekening salah atau tidak ditemukan.\n";
                tungguEnter();
            }
        }
    }
}

// ============================================================================
// B. PORTAL TRANSAKSI NASABAH (NAVIGASI DI LUAR AREA KOTAK)
// ============================================================================
void subMenuNasabah(Nasabah* ptrUser) {
    int kursor = 1; 
    const int totalOpsi = 5; 
    char tombol;

    while (true) {
        bersihkanLayar();
        buatSpasiMenu(1);
        
        // Atas & Judul Menggunakan Warna Biru Murni Elegan Sesuai Sketsa Kedua
        cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
        cout << "                    " << BLUE_PURE << "      INTERFACES MINI BANK CLIENT     " << RESET << "\n"; 
        cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
        
        // Bagian Informasi: Sejajar, Simetris Dengan Bingkai Samping Biru Murni
        int s1 = 21 - ptrUser->nama.length(); if(s1 < 0) s1 = 0;
        cout << "                    " << BLUE_PURE << "| " << TEXT_BLACK << "Nama Nasabah : " << RESET << ptrUser->nama << string(s1, ' ') << BLUE_PURE << " |" << RESET << "\n";
        
        int s2 = 21 - ptrUser->noRekening.length(); if(s2 < 0) s2 = 0;
        cout << "                    " << BLUE_PURE << "| " << TEXT_BLACK << "No. Rekening : " << RESET << ptrUser->noRekening << string(s2, ' ') << BLUE_PURE << " |" << RESET << "\n";
        
        string stringSaldo = "Rp " + to_string(ptrUser->saldo);
        int s3 = 21 - stringSaldo.length(); if(s3 < 0) s3 = 0;
        cout << "                    " << BLUE_PURE << "| " << TEXT_BLACK << "Saldo Utama  : " << RESET << stringSaldo << string(s3, ' ') << BLUE_PURE << " |" << RESET << "\n";
        
        // Garis Pembatas Tengah Sejajar Berwarna Biru Murni
        cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
        buatSpasiMenu(1);

        // Pilihan Menu Interaktif
        if (kursor == 1)
            cout << "                              " << SUCCESS_GRN << "> Setor Tunai <" << RESET << "\n";
        else
            cout << "                                Setor Tunai  \n";

        if (kursor == 2)
            cout << "                              " << SUCCESS_GRN << "> Tarik Tunai <" << RESET << "\n";
        else
            cout << "                                Tarik Tunai  \n";

        if (kursor == 3)
            cout << "                            " << SUCCESS_GRN << "> Transfer Saldo <" << RESET << "\n";
        else
            cout << "                              Transfer Saldo  \n";

        if (kursor == 4)
            cout << "                           " << SUCCESS_GRN << "> Riwayat Transaksi <" << RESET << "\n";
        else
            cout << "                             Riwayat Transaksi  \n";

        if (kursor == 5)
            cout << "                            " << ALERT_RED << "> Logout / Selesai <" << RESET << "\n";
        else
            cout << "                              Logout / Selesai  \n";

        // Garis Batas Bawah Tambahan Sesuai Sketsa Gambar Pertama
        buatSpasiMenu(1);
        cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
        
        // POSISI BARU: Petunjuk Navigasi berada di luar kotak terbawah
        cout << "               " << SUB_GRAY << "Navigasi: Panah / PageUp / PageDown | Enter untuk memilih" << RESET << "\n";
        buatSpasiMenu(1);

        tombol = _getch();
        if (tombol == 0 || tombol == -32) {
            tombol = _getch();
            if (tombol == 72 || tombol == 73) kursor = (kursor == 1) ? totalOpsi : kursor - 1;
            else if (tombol == 80 || tombol == 81) kursor = (kursor == totalOpsi) ? 1 : kursor + 1;
        } 
        else if (tombol == '\r') {
            if (kursor == 5) {
                bersihkanLayar();
                cout << "\n                    " << SUCCESS_GRN << "[OK] Kartu dikeluarkan. Sesi aman berakhir." << RESET << "\n";
                tungguEnter();
                break;
            }

            if (kursor == 1) { 
                bersihkanLayar(); long long nominal;
                cout << "\n                    " << BLUE_PURE << "[DEPOSIT BOX TUNAI]" << RESET << "\n";
                cout << "                    Masukkan Nominal Setor : Rp "; cin >> nominal;
                
                if (nominal <= 0 || nominal % 50000 != 0) {
                    cout << "\n                    " << ALERT_RED << "[TRANSAKSI GAGAL] Setoran wajib kelipatan Rp 50.000!" << RESET << "\n";
                } else {
                    ptrUser->saldo += nominal; 
                    catatLogTransaksi(ptrUser, "MASUK", "Setoran Uang Cash", nominal);
                    cout << "\n                    " << SUCCESS_GRN << "[NOTIFIKASI BERHASIL] Dana Berhasil Dikreditkan Ke Saldo Akun." << RESET << "\n";
                }
                tungguEnter();
            }
            else if (kursor == 2) { 
                bersihkanLayar(); 
                long long nominal;
                
                buatSpasiMenu(1);
                cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
                cout << "                    " << BLUE_PURE << "      FASILITAS PENARIKAN TUNAI      " << RESET << "\n";
                cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
                buatSpasiMenu(1);
                
                cout << "                    " << TEXT_BLACK << "Saldo Tersedia : " << RESET << GOLD_GOLD << "Rp " << ptrUser->saldo << RESET << "\n";
                cout << "                    " << CYAN_LAUT << "Nominal Tarik  : Rp " << RESET; cin >> nominal;
                
                buatSpasiMenu(1);
                cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";

                // LOGIKA VALIDASI
                if (nominal <= 0 || nominal % 50000 != 0) {
                    cout << "                    " << ALERT_RED << "[GAGAL] Nominal harus kelipatan Rp 50.000!" << RESET << "\n";
                } else if (nominal > ptrUser->saldo) {
                    cout << "                    " << ALERT_RED << "[GAGAL] Saldo Anda tidak mencukupi!" << RESET << "\n";
                } else {
                    // PROSES TRANSAKSI
                    ptrUser->saldo -= nominal;
                    catatLogTransaksi(ptrUser, "KELUAR", "Penarikan Tunai Mandiri", nominal);
                    
                    cout << "                    " << SUCCESS_GRN << "[SUKSES] Silakan ambil uang Anda." << RESET << "\n";
                    cout << "                    " << TEXT_BLACK << "Sisa Saldo     : " << RESET << GOLD_GOLD << "Rp " << ptrUser->saldo << RESET << "\n";
                }
                
                cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
                tungguEnter();
            }
            
            else if (kursor == 3) { 
                bersihkanLayar(); string targetRek; long long nominalTF;
                cout << "\n                    " << BLUE_PURE << "[SISTEM TRANSFER ELEKTRONIK]" << RESET << "\n";
                cout << "                    Masukkan Nomor Rekening Tujuan : "; cin >> targetRek;
                
                int idxTujuan = cariIndeksRekening(targetRek);
                if (idxTujuan == -1 || targetRek == ptrUser->noRekening) {
                    cout << "\n                    " << ALERT_RED << "[TRANSAKSI GAGAL] Nomor Rekening Tujuan Tidak Valid!" << RESET << "\n";
                } else {
                    cout << "                    Nama Penerima : " << SUCCESS_GRN << dataNasabah[idxTujuan].nama << RESET << "\n";
                    cout << "                    Nominal Transfer (Min. Rp 10.000) : Rp "; cin >> nominalTF;
                    
                    if (nominalTF < 10000) {
                        cout << "\n                    " << ALERT_RED << "[TRANSAKSI GAGAL] Batas minimal transfer Rp 10.000!" << RESET << "\n";
                    } else if (nominalTF > ptrUser->saldo) {
                        cout << "\n                    " << ALERT_RED << "[TRANSAKSI GAGAL] Saldo Anda tidak cukup!" << RESET << "\n";
                    } else {
                        ptrUser->saldo -= nominalTF;
                        dataNasabah[idxTujuan].saldo += nominalTF;
                        
                        catatLogTransaksi(ptrUser, "KELUAR", "Transfer ke Rek " + targetRek, nominalTF);
                        catatLogTransaksi(&dataNasabah[idxTujuan], "MASUK", "Terima Transfer dari Rek " + ptrUser->noRekening, nominalTF);
                        cout << "\n                    " << SUCCESS_GRN << "[NOTIFIKASI BERHASIL] Transfer Terkirim Sempurna!" << RESET << "\n";
                    }
                }
                tungguEnter();
            }
            else if (kursor == 4) { 
                bersihkanLayar(); 
                buatSpasiMenu(1);
                cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
                cout << "                    " << BLUE_PURE << "     HISTORI LOG MUTASI REKENING      " << RESET << "\n";
                cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
                buatSpasiMenu(1);
                if (ptrUser->totalMutasi == 0) {
                    cout << "                    " << SUB_GRAY << "Belum ada catatan aktivitas transaksi." << RESET << "\n";
                } else {
                    for (int j = 0; j < ptrUser->totalMutasi; j++) {
                        string lambang = (ptrUser->riwayat[j].tipe == "MASUK") ? "(+) " : "(-) ";
                        string warnaGaya = (ptrUser->riwayat[j].tipe == "MASUK") ? SUCCESS_GRN : ALERT_RED;
                        
                        cout << "             " << warnaGaya << lambang << "[" << ptrUser->riwayat[j].tipe << "] " << RESET 
                             << ptrUser->riwayat[j].detail << "\n             -> Nilai: " << GOLD_GOLD << "Rp " << ptrUser->riwayat[j].jumlah << RESET << "\n\n";
                    }
                }
                buatSpasiMenu(1);
                cout << "                    " << BLUE_PURE << "======================================" << RESET << "\n";
                tungguEnter();
            }
        }
    }
}