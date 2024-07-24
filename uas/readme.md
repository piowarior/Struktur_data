# Sistem Perpustakaan Berbasis C++

## Deskripsi

Sistem perpustakaan berbasis C++ ini memungkinkan pengguna (user) dan admin untuk mengelola koleksi buku perpustakaan dengan berbagai fitur yang memudahkan operasional perpustakaan.

Admin memiliki hak akses penuh untuk mengelola sistem perpustakaan. Admin dapat menambahkan buku baru dengan detail seperti judul, penulis, penerbit, tahun terbit, dan kategori. Selain itu, admin dapat mengedit informasi buku yang sudah ada untuk memperbarui atau memperbaiki data, serta menghapus buku yang tidak lagi tersedia atau relevan. Admin juga memiliki kemampuan untuk mengelola pengguna, termasuk menambah, mengedit, dan menghapus pengguna serta mengatur peran mereka sebagai user atau admin.

Pengguna (user) memiliki akses untuk melihat semua buku yang tersedia di perpustakaan, mencari buku berdasarkan judul, penulis, atau kategori, meminjam buku yang tersedia, dan mengembalikan buku yang dipinjam. Sistem mencatat tanggal peminjaman dan pengembalian untuk memudahkan pelacakan dan pengelolaan buku.

Manfaat dari sistem ini meliputi efisiensi dalam pengelolaan buku dan pengguna, aksesibilitas yang memudahkan pengguna dalam mencari dan meminjam buku, serta keamanan data karena hanya admin yang dapat mengedit dan menghapus informasi.

## Fitur

### Admin
- **Menambahkan Buku:** Admin dapat menambahkan buku baru dengan detail seperti judul, penulis, penerbit, tahun terbit, dan kategori.
- **Mengedit Buku:** Admin dapat mengedit informasi buku yang sudah ada.
- **Menghapus Buku:** Admin dapat menghapus buku dari sistem.
- **Mengelola Pengguna:** Admin dapat menambahkan, mengedit, dan menghapus pengguna serta melihat daftar pengguna.

### User
- **Melihat Daftar Buku:** Pengguna dapat melihat daftar buku yang tersedia di perpustakaan.
- **Mencari Buku:** Pengguna dapat mencari buku berdasarkan judul, penulis, atau kategori.
- **Meminjam Buku:** Pengguna dapat meminjam buku dari perpustakaan.
- **Mengembalikan Buku:** Pengguna dapat mengembalikan buku yang dipinjam ke perpustakaan.

## Struktur Database

### Tabel Buku
- **ID Buku (PRIMARY KEY):** Identifikasi unik untuk setiap buku.
- **Judul:** Judul buku.
- **Penulis:** Nama penulis buku.
- **Penerbit:** Nama penerbit buku.
- **Tahun Terbit:** Tahun buku diterbitkan.
- **Kategori:** Kategori atau genre buku.

### Tabel Pengguna
- **ID Pengguna (PRIMARY KEY):** Identifikasi unik untuk setiap pengguna.
- **Nama:** Nama pengguna.
- **Email:** Alamat email pengguna.
- **Kata Sandi:** Kata sandi untuk mengakses sistem.
- **Peran:** Peran pengguna dalam sistem (User atau Admin).

### Tabel Peminjaman
- **ID Peminjaman (PRIMARY KEY):** Identifikasi unik untuk setiap transaksi peminjaman.
- **ID Pengguna (FOREIGN KEY):** Identifikasi unik pengguna yang meminjam buku.
- **ID Buku (FOREIGN KEY):** Identifikasi unik buku yang dipinjam.
- **Tanggal Peminjaman:** Tanggal buku dipinjam.
- **Tanggal Pengembalian:** Tanggal buku dikembalikan.

## Instalasi dan Penggunaan

### Prasyarat
- C++ Compiler (misalnya, g++)
- MySQL atau MariaDB

### Langkah-langkah Instalasi
1. Clone repositori ini.
   ```sh
   git clone https://github.com/piowarior/Struktur_data
