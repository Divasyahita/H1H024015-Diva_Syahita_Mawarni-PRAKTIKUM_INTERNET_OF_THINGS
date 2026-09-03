# Modul 1 - Sensor dan Aktuator
---
README ini berisi pertanyaan praktikum, dokumentasi, penjelasan fungsi, kode, percabangan/conditional, serta library atau dependencies yang digunakan pada Percobaan 1A dan Percobaan 2A.

A. TUJUAN PRAKTIKUM 

Berdasarkan pemahaman setelah melakukan praktikum, tujuan yang ingin dicapai adalah:

1. Memahami bagaimana ESP32 memperoleh data dari sensor. 
2. Memahami cara membaca suhu dan kelembaban menggunakan sensor DHT11. 
3. Memahami cara kerja aktuator relay yang dikendalikan oleh ESP32. 
4. Mengimplementasikan sistem yang dapat mengaktifkan atau menonaktifkan 
aktuator berdasarkan suhu. 
5. Memahami hubungan antara data yang dibaca sensor → keputusan program → 
respons aktuator. 
6. Menganalisis bagaimana sistem IoT dapat merespons perubahan kondisi 
lingkungan.

B. ALAT DAN BAHAN  
1. ESP32 
2. Sensor DHT11
3. Modul Relay 
4. Breadboard 
5. Kabel Jumper 
6. Kabel USB 

---

# Percobaan 1A: Akuisisi Data Sensor DHT11 (Suhu dan Kelembaban)

code
```cpp
#include <DHT.h>

#define DHTPIN 4          // pin data DHT11 terhubung ke GPIO 4
#define DHTTYPE DHT11     // tipe sensor yang digunakan diubah menjadi DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();            // inisialisasi sensor DHT11
  Serial.println("Memulai akuisisi data sensor DHT11...");
}

void loop() {
  // Membaca data kelembaban dan suhu
  float kelembaban = dht.readHumidity();
  float suhu = dht.readTemperature();

  // Periksa apakah pembacaan berhasil
  if (isnan(kelembaban) || isnan(suhu)) {
    Serial.println("Gagal membaca data dari sensor DHT11!");
  } else {
    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" °C, Kelembaban: ");
    Serial.print(kelembaban);
    Serial.println("%");
  }
  
  delay(2000); // jeda pembacaan setiap 2 detik
}
```

## 1. Penjelasan Setiap Fungsi

Program Percobaan 1A digunakan untuk membaca **suhu dan kelembapan dari sensor DHT11**, kemudian menampilkan hasilnya pada Serial Monitor.

| Fungsi | Penjelasan |
|---|---|
| `setup()` | Menjalankan proses inisialisasi satu kali saat program dimulai. |
| `loop()` | Menjalankan proses pembacaan sensor secara berulang. |
| `Serial.begin(115200)` | Memulai komunikasi Serial dengan baud rate 115200. |
| `dht.begin()` | Menginisialisasi sensor DHT11. |
| `dht.readHumidity()` | Membaca nilai kelembapan dari sensor. |
| `dht.readTemperature()` | Membaca nilai suhu dari sensor. |
| `isnan()` | Memeriksa apakah hasil pembacaan sensor berupa nilai NaN atau tidak valid. |
| `Serial.print()` | Menampilkan teks atau data pada Serial Monitor tanpa pindah baris. |
| `Serial.println()` | Menampilkan teks atau data pada Serial Monitor dan berpindah ke baris berikutnya. |
| `delay(2000)` | Memberikan jeda selama 2 detik sebelum pembacaan berikutnya. |

---

## 2. Penjelasan Percabangan / Conditional

Percabangan pada Percobaan 1A digunakan untuk memeriksa apakah pembacaan sensor berhasil.

```cpp
if (isnan(kelembaban) || isnan(suhu)) {
    Serial.println("Gagal membaca data dari sensor DHT11!");
} else {
    // menampilkan suhu dan kelembapan
}
```
### Kondisi `else`

Jika pembacaan suhu dan kelembapan berhasil, program akan masuk ke bagian `else`.

Pada bagian ini, program menampilkan hasil pembacaan sensor menggunakan `Serial.print()` dan `Serial.println()`.

Contoh hasil:

```text
Suhu: 30.20 °C, Kelembaban: 51.00%
```
---

# Percobaan 2A: Kendali Aktuator Relay Berdasarkan Data Sensor 

code
```cpp
#include <DHT.h>

#define DHTPIN 4          // pin data DHT11 terhubung ke GPIO 4
#define DHTTYPE DHT11
#define RELAYPIN 26       // pin kendali relay/LED indikator

DHT dht(DHTPIN, DHTTYPE);

const float suhuThreshold = 30.0; // ambang batas suhu (°C)

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW); // pastikan aktuator mati di awal
}

void loop() {
  float suhu = dht.readTemperature();

  if (isnan(suhu)) {
    Serial.println("Gagal membaca data sensor!");
  } else {
    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" °C -> ");
    
    // Kendali aktuator berdasarkan hasil akuisisi data sensor
    if (suhu > suhuThreshold) {
      digitalWrite(RELAYPIN, HIGH); // aktifkan relay/LED
      Serial.println("Aktuator: ON");
    } else {
      digitalWrite(RELAYPIN, LOW);  // matikan relay/LED
      Serial.println("Aktuator: OFF");
    }
  }
  
  delay(2000); // jeda pembacaan setiap 2 detik
}
```

Program pada Percobaan 2A digunakan untuk membaca suhu menggunakan sensor **DHT11**, kemudian mengendalikan **relay/LED indikator** berdasarkan nilai suhu yang telah ditentukan sebagai threshold.

Threshold yang digunakan adalah **30°C**. Jika suhu lebih dari 30°C, aktuator akan menyala. Jika suhu 30°C atau kurang, aktuator akan mati.

---

## 1. Penjelasan Setiap Fungsi

| Fungsi | Penjelasan |
|---|---|
| `setup()` | Menjalankan proses inisialisasi satu kali saat mikrokontroler mulai dijalankan. |
| `loop()` | Menjalankan proses pembacaan suhu dan pengendalian aktuator secara berulang. |
| `Serial.begin(115200)` | Memulai komunikasi Serial dengan baud rate 115200. |
| `dht.begin()` | Menginisialisasi sensor DHT11 agar siap digunakan. |
| `pinMode(RELAYPIN, OUTPUT)` | Mengatur GPIO 26 sebagai pin output untuk mengendalikan relay/LED. |
| `digitalWrite(RELAYPIN, LOW)` | Memberikan kondisi LOW pada pin sehingga aktuator berada dalam kondisi mati. |
| `dht.readTemperature()` | Membaca nilai suhu dari sensor DHT11. |
| `isnan(suhu)` | Memeriksa apakah hasil pembacaan suhu berupa NaN atau tidak valid. |
| `digitalWrite(RELAYPIN, HIGH)` | Memberikan kondisi HIGH pada pin sehingga aktuator diaktifkan. |
| `digitalWrite(RELAYPIN, LOW)` | Memberikan kondisi LOW pada pin sehingga aktuator dimatikan. |
| `Serial.print()` | Menampilkan teks atau data pada Serial Monitor tanpa berpindah baris. |
| `Serial.println()` | Menampilkan teks atau data pada Serial Monitor kemudian berpindah ke baris berikutnya. |
| `delay(2000)` | Memberikan jeda selama 2 detik sebelum melakukan pembacaan berikutnya. |

### Variabel Threshold

```cpp
const float suhuThreshold = 30.0;
```

Variabel suhuThreshold digunakan sebagai nilai ambang batas suhu.

Nilai 30.0 berarti batas yang digunakan dalam sistem adalah 30°C. Keyword const digunakan agar nilai threshold tidak berubah selama program berjalan.

## 2. Penjelasan Percabangan / Conditional

Pada Percobaan 2A terdapat dua percabangan `if-else`, yaitu percabangan untuk memeriksa pembacaan sensor dan percabangan untuk mengendalikan aktuator.

## A. Pemeriksaan Pembacaan Sensor

```cpp
if (isnan(suhu)) {
  Serial.println("Gagal membaca data sensor!");
} else {
  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.print(" °C -> ");
  
  // Kendali aktuator
}
```
Percabangan ini digunakan untuk memeriksa apakah sensor berhasil membaca nilai suhu.

- Jika `isnan(suhu)` bernilai **true**, berarti hasil pembacaan suhu tidak valid atau sensor gagal membaca data.
- Jika kondisi tersebut terpenuhi, program menampilkan pesan **"Gagal membaca data sensor!"**.
- Jika pembacaan berhasil, program masuk ke bagian `else` dan melanjutkan proses pengendalian aktuator.

## B. Pengendalian Aktuator Berdasarkan Suhu

```cpp
if (suhu > suhuThreshold) {
  digitalWrite(RELAYPIN, HIGH);
  Serial.println("Aktuator: ON");
} else {
  digitalWrite(RELAYPIN, LOW);
  Serial.println("Aktuator: OFF");
}
```

Percabangan ini digunakan untuk menentukan kondisi aktuator berdasarkan nilai suhu.

Nilai threshold yang digunakan adalah:

```cpp
const float suhuThreshold = 30.0;
```
Artinya, batas suhu yang digunakan adalah **30°C**.

- Jika **suhu > 30°C**, kondisi `if` terpenuhi sehingga aktuator dinyalakan dengan `digitalWrite(RELAYPIN, HIGH)`.
- Jika **suhu <= 30°C**, kondisi `if` tidak terpenuhi sehingga program masuk ke `else` dan aktuator dimatikan dengan `digitalWrite(RELAYPIN, LOW)`.

Jadi, threshold digunakan sebagai batas pengambilan keputusan untuk menentukan kapan aktuator menyala atau mati.

# Library / Dependencies yang Diperlukan
| Library / Dependency      | Fungsi                                                                          |
| ------------------------- | ------------------------------------------------------------------------------- |
| `DHT Sensor Library`      | Membaca data dari sensor DHT11                                                  |
| `Adafruit Unified Sensor` | Dependency yang dibutuhkan oleh DHT Sensor Library                              |
| `Arduino Framework`       | Menyediakan fungsi seperti `Serial`, `pinMode()`, `digitalWrite()`, dan `delay()` |

---


# Pertanyaan Praktikum 

## Percobaan 1A: Akuisisi Data Sensor DHT22

**1.Gambarkan diagram alur (flowchart) proses akuisisi data sensor DHT22 pada program di atas!**
<img width="668" height="1037" alt="Untitled Diagram drawio (4)" src="https://github.com/user-attachments/assets/0b9bbe1b-c2de-4c28-af80-6431689ed6f0" />

**2. Apa fungsi dari perintah `isnan()` pada program tersebut?**

isnan() digunakan untuk memeriksa apakah nilai hasil pembacaan sensor merupakan NaN (Not a Number). Pada program, fungsi ini digunakan untuk  mengetahui apakah pembacaan suhu atau kelembaban gagal. Jika hasilnya tidak valid, program menampilkan pesan bahwa sensor gagal dibaca. 

**3. Jelaskan mengapa diperlukan jeda (delay) minimal sekitar 2 detik antar pembacaan sensor DHT22!**

Diperlukan jeda (delay) minimal sekitar 2 detik antar pembacaan sensor DHT22 karena sensor DHT22 membutuhkan waktu untuk melakukan proses pengukuran dan memperbarui data suhu serta kelembaban. Jika pembacaan dilakukan terlalu cepat atau berulang tanpa jeda, sensor dapat memberikan data yang tidak stabil atau tidak valid. Oleh karena itu, delay(2000) digunakan agar ESP32 menunggu sekitar 2 detik sebelum melakukan pembacaan berikutnya, sehingga data yang diperoleh lebih stabil dan sesuai dengan kemampuan kerja sensor.  

**4. Modifikasi program agar data suhu dan kelembaban dirata-ratakan dari 5 kali pembacaan sebelum ditampilkan, dan berikan penjelasan di setiap baris kode yang ditambahkan.**

Berikut adalah bagian *loop()* yang dimodifikasi untuk mencari rata-rata dari 5 kali pembacaan:
```cpp
void loop() {
  // 1. Inisialisasi variabel untuk menampung total jumlah suhu dan kelembaban
  float totalSuhu = 0;
  float totalKelembaban = 0;
  
  // 2. Inisialisasi variabel untuk menghitung berapa kali sensor sukses membaca data
  int pembacaanBerhasil = 0;

  // 3. Memulai perulangan (looping) sebanyak 5 kali pengambilan data
  for (int i = 0; i < 5; i++) {
    // 4. Membaca data kelembaban dan suhu dari sensor
    float kelembaban = dht.readHumidity();
    float suhu = dht.readTemperature();
    
    // 5. Validasi: Memeriksa apakah kelembaban DAN suhu berhasil terbaca (bukan NaN)
    if (!isnan(kelembaban) && !isnan(suhu)) {
      // 6. Jika berhasil, tambahkan nilai bacaan ke total akumulasi
      totalKelembaban += kelembaban;
      totalSuhu += suhu;
      
      // 7. Tambah (increment) jumlah penghitung sukses
      pembacaanBerhasil++;
    } else {
      // 8. Tampilkan pesan jika terjadi gagal baca pada salah satu siklus
      Serial.println("Gagal membaca sampel data dari sensor!");
    }
    
    // 9. Berikan jeda 2 detik setiap kali selesai membaca data dari sensor
    delay(2000); 
  }

  // 10. Mengecek apakah minimal ada 1 data yang berhasil dibaca agar tidak terjadi pembagian dengan nol
  if (pembacaanBerhasil > 0) {
    // 11. Menghitung rata-rata dengan membagi total nilai dengan jumlah data yang sukses dibaca
    float rataSuhu = totalSuhu / pembacaanBerhasil;
    float rataKelembaban = totalKelembaban / pembacaanBerhasil;
    
    // 12. Menampilkan hasil rata-rata ke Serial Monitor
    Serial.print("Rata-rata Suhu: "); 
    Serial.print(rataSuhu); 
    Serial.println(" °C");
    Serial.print("Rata-rata Kelembaban: "); 
    Serial.print(rataKelembaban); 
    Serial.println(" %");
  } else {
    // 13. Pesan jika kelima perulangan gagal dibaca oleh sensor
    Serial.println("Gagal total menghitung rata-rata.");
  }
}
```

---

## Percobaan 2A: Kendali Aktuator Relay Berdasarkan Data Sensor

**1. Mengapa diperlukan nilai ambang batas (threshold) dalam sistem kendali aktuator berbasis sensor?**

Nilai ambang batas (*threshold*) berfungsi sebagai titik acuan (referensi) atau aturan pengambilan keputusan (logika IF) bagi mikrokontroler. Dengan adanya nilai ambang batas, sistem dapat memutuskan kapan harus mengirimkan sinyal perintah (ON/OFF) kepada aktuator berdasarkan data fisik lingkungan (seperti suhu) yang ditangkap oleh sensor. Tanpa nilai ini, mikrokontroler tidak tahu pada kondisi suhu berapa ia harus mengambil tindakan untuk mengubah status dari aktuator.

**2. Jelaskan apa yang akan terjadi apabila nilai suhu Threshold diturunkan menjadi sangat rendah, misalnya 20.0!**

Jika nilai suhuThreshold diturunkan dari 30°C menjadi 20°C, aktuator akan lebih sering menyala, karena kondisi untuk menyalakan aktuator menjadi lebih mudah terpenuhi. 
*   **Program awal:** const float suhuThreshold = 30.0; 
*   **diubah menjadi:** const float suhuThreshold = 20.0; 

Misalnya suhu lingkungan yang terbaca adalah 25°C:  
25°C > 20°C → Aktuator ON  
Padahal dengan threshold 30°C:  
25°C > 30°C → Aktuator OFF  

Jadi, semakin rendah nilai threshold, semakin banyak kondisi suhu yang menyebabkan aktuator menyala.  

**3. Apa perbedaan antara kendali aktuator secara terus-menerus (kondisi tunggal) dengan kendali menggunakan histerisis (dua ambang batas)?**

Kendali aktuator dengan kondisi tunggal hanya menggunakan satu nilai threshold sebagai batas untuk menentukan aktuator ON atau OFF, sehingga ketika suhu melewati batas tersebut, status aktuator dapat langsung berubah. Sementara itu, kendali menggunakan histerisis memakai dua nilai ambang batas, yaitu batas untuk menyalakan dan batas untuk mematikan aktuator, sehingga aktuator dapat mempertahankan kondisi sebelumnya ketika suhu berada di antara kedua batas tersebut. Dengan demikian, kendali histerisis dapat membuat kerja aktuator lebih stabil dan mengurangi perubahan ON/OFF yang terlalu sering. 

**4. Modifikasi program agar menggunakan dua ambang batas (histerisis), misalnya aktuator menyala pada suhu di atas 30°C dan baru mati pada suhu di bawah 28°C, dan berikan penjelasan di setiap baris kode nya.**
Berikut adalah bagian *loop()* yang dimodifikasi beserta variabel pendukungnya:

```cpp
// 1. Mendefinisikan dua variabel konstanta (histerisis) untuk batas atas dan bawah
const float suhuAtas = 30.0;
const float suhuBawah = 28.0;

void loop() {
  // 2. Membaca data suhu dari sensor
  float suhu = dht.readTemperature();

  // 3. Mengecek apakah sensor berhasil membaca suhu
  if (isnan(suhu)) {
    Serial.println("Gagal membaca data sensor!");
  } else {
    // 4. Menampilkan suhu saat ini di Serial Monitor
    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" °C -> ");

    // 5. Logika Histerisis batas atas: Jika suhu lebih besar dari 30.0
    if (suhu > suhuAtas) {
      // 6. Memberi tegangan HIGH untuk mengaktifkan Relay/LED
      digitalWrite(RELAYPIN, HIGH); 
      Serial.println("Aktuator: ON");
    } 
    // 7. Logika Histerisis batas bawah: Jika suhu kurang dari 28.0
    else if (suhu < suhuBawah) {
      // 8. Menurunkan tegangan ke LOW untuk mematikan Relay/LED
      digitalWrite(RELAYPIN, LOW);
      Serial.println("Aktuator: OFF");
    }
    // 9. Kondisi jika suhu di antara 28.0 dan 30.0
    // Pada rentang ini (contoh: 29.5), tidak ada instruksi perintah perubahan.
    // Artinya mikrokontroler akan menahan (holding) perintah 'digitalWrite' yang 
    // terakhir kali dieksekusi, sehingga status aktuator tetap stabil.
  }
  
  // 10. Jeda 2 detik sebelum perulangan berikutnya
  delay(2000); 
}
```

