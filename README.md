# OBJ Voxelizer

Program ini mengubah model 3D mesh dalam format `.obj` menjadi representasi voxel (kumpulan kubus kecil) menggunakan struktur data *Octree* dan algoritma *Divide and Conquer*. Hasil voxelisasi disimpan sebagai file `.obj` baru dan dapat dilihat melalui viewer 3D interaktif.

---

## Author

| Nama | NIM |
|---|---|
| Hakam Avicena Mustain | 13524075 |
| Rava Khoman Tuah Saragih | 13524107 |

---

## Requirements

- C++ Compiler dengan dukungan *C++17* (`g++` atau `clang++`)
- *GNU Make*
- *SDL2* — library untuk visualisasi 3D

### Instalasi SDL2

**Linux (Ubuntu/Debian)**
```bash
sudo apt-get install build-essential
sudo apt-get install libsdl2-dev
```

**macOS**
```bash
brew install sdl2
```

**Windows (MSYS2/MinGW)**
```bash
pacman -S mingw-w64-x86_64-toolchain
pacman -S mingw-w64-x86_64-SDL2
```

---

## Cara Kompilasi

Jalankan:

```bash
make
```

Untuk membersihkan hasil build:

```bash
make clean
```

---

## Cara Menjalankan

```bash
make run
```

atau jalankan executable langsung setelah kompilasi:

```bash
./voxelizer
```

---

## Cara Penggunaan

Program akan meminta dua input secara interaktif:

**1. Path ke file `.obj`**
```
Masukkan path obj file Anda:
teapot.obj
```

**2. Kedalaman maksimum octree** (minimal 1)
```
Masukkan tingkat kedalaman pohon maksimum:
4
```

> Semakin besar nilai kedalaman, semakin detail hasil voxelisasi, namun waktu komputasi dan memori yang dibutuhkan meningkat.

### Contoh Output

```
Jumlah Vertex (input)   : 530
Jumlah Triangle (input) : 1024
Bounding Box            : -84.4453, -86.4664, -92.4691, 98.2769, 96.2558, 90.2531
Statistik node octree:
0 : 1
1 : 8
2 : 64
3 : 192
4 : 864
Statistik node di-prune:
2 : 40
3 : 84
4 : 446
Kedalaman octree        : 4
Jumlah voxel            : 418
Jumlah vertex output    : 3344
Jumlah face output      : 5016
Output disimpan di      : teapot_voxelized_max-depth_4.obj
Waktu eksekusi          : 53 ms
```

Setelah proses selesai:
- File hasil voxelisasi disimpan otomatis di direktori yang sama dengan file input, dengan nama `<nama_file>_voxelized_max-depth_<depth>.obj`
- Viewer 3D interaktif terbuka secara otomatis

---

## Struktur Direktori

```
.
├── src/          # Source code program
├── bin/          # File executable hasil kompilasi
├── test/         # Data uji dan solusi yang digunakan dalam laporan
├── doc/          # Laporan dalam format PDF
└── README.md
```
