# 🏦 MINI BANK — Sistem Informasi & Manajemen Perbankan CLI

![Language](https://img.shields.io/badge/Language-C%2FC%2B%2B-blue?style=flat-square&logo=c)
![Platform](https://img.shields.io/badge/Platform-CLI%20%2F%20Terminal-black?style=flat-square&logo=gnometerminal)
![Status](https://img.shields.io/badge/Status-Completed-green?style=flat-square)

**MINI BANK** adalah aplikasi berbasis CLI (*Command Line Interface*) yang dirancang untuk mensimulasikan layanan transaksi perbankan dasar dan pengelolaan data nasabah menggunakan struktur data **Linked List**.

---

## 📋 Daftar Isi

- [Fitur Utama](#-fitur-utama)
- [Struktur Program](#-struktur-program)
- [Persyaratan Sistem](#-persyaratan-sistem)
- [Instalasi & Menjalankan Program](#-instalasi--menjalankan-program)
- [Cara Penggunaan](#-cara-penggunaan)
- [Struktur Data Internal](#-struktur-data-internal)

## ✨ Fitur Utama

### 🔐 Sistem Keamanan & Akses
- **Otentikasi Dua Jalur:** Gerbang masuk terpisah untuk Admin (Operator) dan Nasabah.
- **Sensor Input Kata Sandi:** Input *password* dan PIN otomatis disamarkan menggunakan karakter asterisk (`*`).
- **Sistem Blokir Otomatis:** Nasabah dengan status akun ditangguhkan tidak dapat masuk ke sistem sebelum diaktifkan kembali oleh Admin.

### 🛠️ Fitur Admin (Konsol Operator Supervisor)
| Fitur | Keterangan |
|---|---|
| **Pendaftaran Nasabah** | Membuat akun baru dengan sistem *generator* nomor rekening unik otomatis dan minimal setoran awal Rp 50.000. |
| **Lihat Data Nasabah** | Menampilkan seluruh data nasabah beserta saldo dan status akun secara *real-time*. |
| **Kelola Akun Nasabah** | Fitur dinamis untuk memblokir, membuka blokir, atau menghapus akun nasabah secara permanen dari sistem. |
| **Monitoring Log Nasabah** | Memantau seluruh riwayat mutasi keuangan dari nomor rekening target tertentu demi kebutuhan audit. |

### 👤 Fitur Nasabah (Interfaces Mini Bank Client)
| Fitur | Keterangan |
|---|---|
| **Setor Tunai** | Menambah saldo tabungan mandiri secara instan dengan validasi kelipatan uang Rp 50.000. |
| **Tarik Tunai** | Melakukan penarikan tunai aman dengan validasi kecukupan saldo dan kelipatan nominal Rp 50.000. |
| **Transfer Saldo** | Mengirimkan dana antar-rekening nasabah dilengkapi proteksi pembatasan transfer ke rekening sendiri, rekening terblokir, dan batas minimal transfer Rp 10.000. |
| **Riwayat Transaksi** | Cetak mutasi rekening lengkap yang menampilkan log alur keluar-masuk dana beserta detail tujuan transaksi. |

## 📂 Struktur Program
MAIN MENU
├── PORTAL OTENTIKASI NASABAH → Login Nasabah (Akses Tabungan)
└── SECURITY ACCESS ADMIN     → Login Admin (Konsol Operator)

KONSOL OPERATOR (ADMIN)
├── Pendaftaran Nasabah Baru
├── Lihat Data Nasabah
├── Kelola Akun Nasabah (Blokir, Buka Blokir, Hapus Akun)
├── Riwayat Transaksi Nasabah (Audit Log)
└── Logout Sesi

MINI BANK CLIENT (NASABAH)
├── Dashboard Info (Nama, No. Rekening, Saldo Utama)
├── Setor Tunai
├── Tarik Tunai
├── Transfer Saldo
└── Riwayat Transaksi (Mutasi Rekening)

## 💻 Persyaratan Sistem

- **Sistem Operasi:** Windows (Direkomendasikan karena menggunakan pustaka `<conio.h>`)
- **Compiler:** `g++` (GCC versi 7.0 ke atas) atau `MinGW`
- **Terminal:** Command Prompt (CMD) / PowerShell yang mendukung ANSI Escape Code (untuk pewarnaan antarmuka).

## 🚀 Instalasi & Menjalankan Program

### 1. Buat Direktori Projek

Silakan salin kode program ke dalam berkas bernama `main.cpp` di komputer Anda, lalu buka terminal pada direktori tersebut.

### 2. Kompilasi Program

Jalankan perintah berikut pada terminal untuk melakukan kompilasi:
```bash
g++ main.cpp -o MiniBank.exe

📖 Cara Penggunaan
Kredensial Akun Default (Uji Coba)
Sistem telah dilengkapi dengan akun bawaan untuk mempermudah proses pengujian fungsionalitas program:

1. Akun Admin Bawaan
3. Jalankan Program
Eksekusi program melalui terminal Anda:
MiniBank.exe

* Akun Auditor
  Username : admin1
  Password : passadmin1

* Akun IT Core Developer
  Username : superadmin
  Password : root123
2. Akun Nasabah Dummy Bawaan

* Nasabah 1
  Username : lintang
  Password : lintang123
  No Rek   : 1234567890

* Nasabah 2
  Username : suci
  Password : suci123
  No Rek   : 9876543210
🧠 Struktur Data Internal
Program ini menerapkan implementasi struktur data tingkat lanjut untuk memastikan efisiensi manajemen memori:

Linked List Dinamis (akun* head): Digunakan sebagai basis utama penyimpanan data nasabah. Alokasi memori dilakukan secara dinamis, memungkinkan proses penambahan (insertion) dan penghapusan (deletion) akun nasabah berjalan efisien tanpa batasan ukuran array statis.

Array of Struct di dalam Node: Setiap node nasabah menyimpan data riwayat mutasi transaksi terstruktur (Transaksi riwayat[MAX_MUTASI]) untuk melacak rekam jejak finansial secara personal.
