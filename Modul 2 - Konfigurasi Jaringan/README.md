# MODUL 2 – KONFIGURASI JARINGAN

# A. TUJUAN PRAKTIKUM

Setelah melakukan praktikum, tujuan yang saya pahami adalah:

1. Memahami konsep dasar jaringan WiFi pada ESP32/ESP8266.
2. Memahami perbedaan mode Station (STA) dan Access Point (AP).
3. Memahami cara menghubungkan ESP ke jaringan WiFi yang sudah tersedia menggunakan mode Station.
4. Memahami cara menjadikan ESP sebagai Access Point sehingga dapat membuat jaringan WiFi sendiri.
5. Mengetahui parameter jaringan seperti IP Address, MAC Address, dan RSSI.
6. Memahami cara menggabungkan mode Station dan Access Point menjadi AP+STA.

---

# B. ALAT DAN BAHAN

1. ESP32/ESP8266 DevKit
2. Kabel USB
3. Laptop/PC
4. Arduino IDE
5. Jaringan WiFi/hotspot
6. Smartphone/laptop untuk menguji Access Point
7. LED

---

# C. PERCOBAAN 2A – MODE STATION (STA)

## 1. Tujuan

Percobaan ini bertujuan untuk memahami cara ESP terhubung ke jaringan WiFi yang sudah tersedia menggunakan mode Station (STA). Selain itu, dilakukan pengamatan terhadap status koneksi, IP Address, MAC Address, RSSI, dan indikator LED.

## 2. Rangkaian Percobaan

Pada percobaan ini ESP terhubung ke laptop menggunakan kabel USB. GPIO 2 digunakan untuk LED sebagai indikator status koneksi WiFi.

### Dokumentasi Rangkaian

><img width="1280" height="720" alt="image" src="https://github.com/user-attachments/assets/19b71a74-6169-42a5-a773-b2472f4d8519" />

## 3. Flowchart Percobaan 2A

### Diagram Alur Proses Koneksi WiFi

Alur program:

```text
Mulai
  ↓
Inisialisasi Serial dan LED
  ↓
Atur mode WiFi menjadi Station
  ↓
Mulai koneksi WiFi
  ↓
Apakah WiFi sudah terhubung?
  ├── Tidak → Tunggu 500 ms → Cetak "." → Cek kembali
  │
  └── Ya
       ↓
   Tampilkan IP, MAC, dan RSSI
       ↓
   LED menyala
       ↓
   Masuk ke loop
       ↓
   Cek status WiFi setiap 5 detik
       ↓
   Terhubung? → Status Terhubung
       ↓
   Terputus? → Status Terputus + LED mati
       ↓
   Ulangi
```

## 4. Program Percobaan 2A

```cpp
#include <ESP8266WiFi.h>

const char* ssid     = "NAMA_WIFI_ANDA";
const char* password = "PASSWORD_WIFI_ANDA";

const int ledPin = 2; // LED indikator status koneksi

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Set mode WiFi menjadi Station
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Jika berhasil terhubung
  Serial.println();
  Serial.println("WiFi berhasil terhubung!");
  Serial.print("IP Address  : ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC Address : ");
  Serial.println(WiFi.macAddress());
  Serial.print("RSSI (dBm)  : ");
  Serial.println(WiFi.RSSI());

  digitalWrite(ledPin, HIGH);
}

void loop() {
  // Cek status koneksi setiap 5 detik
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Status: Terhubung");
  } else {
    Serial.println("Status: Terputus");
    digitalWrite(ledPin, LOW);
  }

  delay(5000);
}
```

## 5. Penjelasan Program

`#include <ESP8266WiFi.h>`

Digunakan untuk memasukkan library WiFi agar board dapat menggunakan fungsi-fungsi untuk mengatur koneksi jaringan.

`const char* ssid`

Digunakan untuk menyimpan nama jaringan WiFi yang akan dihubungkan.

`const char* password`

Digunakan untuk menyimpan password jaringan WiFi.

`const int ledPin = 2`

Menentukan GPIO 2 sebagai pin untuk LED indikator koneksi.

`Serial.begin(115200)`

Memulai komunikasi serial dengan baud rate 115200 agar informasi dari board dapat dilihat melalui Serial Monitor.

`pinMode(ledPin, OUTPUT)`

Mengatur GPIO 2 sebagai output karena digunakan untuk mengendalikan LED.

`digitalWrite(ledPin, LOW)`

Membuat LED dalam keadaan mati saat program pertama kali dijalankan.

`WiFi.mode(WIFI_STA)`

Mengatur board ke mode Station, sehingga board bertindak sebagai client yang terhubung ke jaringan WiFi yang sudah tersedia.

`WiFi.begin(ssid, password)`

Memulai proses koneksi ke WiFi menggunakan SSID dan password yang telah ditentukan.

`while (WiFi.status() != WL_CONNECTED)`

Program akan terus melakukan pengecekan sampai board berhasil terhubung ke WiFi.

`delay(500)`

Memberikan jeda selama 500 ms sebelum melakukan pengecekan berikutnya.

`Serial.print(".")`

Menampilkan tanda titik pada Serial Monitor selama proses koneksi berlangsung.

`WiFi.localIP()`

Digunakan untuk mendapatkan IP Address yang diberikan oleh jaringan WiFi.

`WiFi.macAddress()`

Digunakan untuk menampilkan MAC Address board.

`WiFi.RSSI()`

Digunakan untuk mengetahui kekuatan sinyal WiFi dalam satuan dBm.

`digitalWrite(ledPin, HIGH)`

Menyalakan LED sebagai indikator bahwa koneksi WiFi berhasil.

`WiFi.status() == WL_CONNECTED`

Digunakan untuk mengecek apakah board masih terhubung ke WiFi.

`delay(5000)`

Memberikan jeda selama 5 detik sehingga pengecekan status dilakukan setiap 5 detik.

---

# D. HASIL PENGAMATAN PERCOBAAN 2A

## 1. Tabel pengamatan Koneksi WiFi Mode Station

| No | Waktu (s) | Status    | IP Address    | MAC Address       | RSSI (dBm) | LED   |
| -- | --------: | --------- | ------------- | ----------------- | ---------: | ----- |
| 1  |         0 | Terhubung | 10.226.85.81  | 84:F3:EB:5A:42:52 |        -56 | Nyala |
| 2  |         5 | Terhubung | 10.226.85.81  | 84:F3:EB:5A:42:52 |        -56 | Nyala |
| 3  |        10 | Terhubung | 10.226.85.81  | 84:F3:EB:5A:42:52 |        -56 | Nyala |
| 4  |        15 | Terhubung | 10.226.85.81  | 84:F3:EB:5A:42:52 |        -56 | Nyala |
| 5  |        20 | Terhubung | 10.226.85.81  | 84:F3:EB:5A:42:52 |        -56 | Nyala |
| 6  |        25 | Terhubung | 10.213.234.80 | 84:F3:EB:5A:42:52 |        -50 | Nyala |
| 7  |        30 | Terhubung | 10.213.234.80 | 84:F3:EB:5A:42:52 |        -50 | Nyala |
| 8  |        35 | Terhubung | 10.213.234.80 | 84:F3:EB:5A:42:52 |        -50 | Nyala |
| 9  |        40 | Terhubung | 10.213.234.80 | 84:F3:EB:5A:42:52 |        -50 | Nyala |
| 10 |        45 | Terhubung | 10.213.234.80 | 84:F3:EB:5A:42:52 |        -50 | Nyala |

## Analisis

Berdasarkan hasil pengamatan, ESP berhasil terhubung ke jaringan WiFi menggunakan mode Station. Hal ini ditunjukkan oleh status koneksi yang selalu terhubung dan LED yang selalu menyala selama pengamatan.

MAC Address tetap `84:F3:EB:5A:42:52` karena MAC Address merupakan identitas perangkat. Sementara itu, IP Address dapat berubah karena diberikan oleh jaringan yang digunakan.

Nilai RSSI berubah dari `-56 dBm` menjadi `-50 dBm`. Nilai RSSI yang lebih mendekati 0 menunjukkan sinyal yang lebih kuat. Meskipun terjadi perubahan IP dan RSSI, koneksi tetap stabil dan tidak menyebabkan LED mati.

<img width="649" height="448" alt="image" src="https://github.com/user-attachments/assets/cfafa018-3808-45e3-95a7-2ff5caafedeb" />
<img width="511" height="402" alt="image" src="https://github.com/user-attachments/assets/f1e943c9-a92d-4d04-b435-981121a6b97b" />


## 2. Tabel Pengujian Mode Station (STA)

| No. | Kondisi Pengujian | SSID | Password | Status Koneksi | Output Serial Monitor | Keterangan |
|---|---|---|---|---|---|---|
| 1 | Kredensial Benar | hmmm|apaiyasih  | Terhubung | WiFi berhasil terhubung! IP Address : 10.226.85.81 MAC Address : 84:F3:EB:5A:42:52 RSSI (dBm) : -54 Status: Terhubung Status: Terhubung Status: Terhubung | BERHASIL dan LED nyala |
| 2 | Password Salah | hmmm |pipnurr  | Tidak Terhubung | ……………. | GAGAL dan LED mati |
| 3 | SSID Salah | .. |apaiyasih  | Tidak Terhubung | ……………. | GAGAL dan LED mati |

### Hasil Pengujian

Pada saat menggunakan kredensial yang benar, ESP berhasil terhubung dan Serial Monitor menampilkan informasi koneksi seperti IP Address, MAC Address, RSSI, dan status terhubung.

Ketika password atau SSID salah, ESP tidak berhasil melakukan koneksi. Serial Monitor terus menampilkan tanda titik karena program masih berada pada proses `while` selama status WiFi belum `WL_CONNECTED`.

#ketika password salah
<img width="870" height="423" alt="image" src="https://github.com/user-attachments/assets/f94db537-09ed-4ee7-982a-9cb250d69691" />


---

# F. PERCOBAAN 2B – MODE ACCESS POINT (AP)

## 1. Tujuan

Percobaan ini bertujuan untuk memahami cara board membuat jaringan WiFi sendiri menggunakan mode Access Point (AP), sehingga perangkat lain seperti smartphone atau laptop dapat terhubung langsung ke jaringan yang dibuat board.

## 2. Rangkaian/Setup Percobaan

Pada percobaan AP, ESP membuat jaringan WiFi sendiri dan smartphone digunakan sebagai client untuk menguji jaringan tersebut.

## 3. Program Percobaan 2B

```cpp
#include <ESP8266WiFi.h>

const char* ap_ssid     = "apip diva hana";
const char* ap_password = "12345678";

void setup() {
  Serial.begin(115200);

  // Set mode WiFi menjadi Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);

  IPAddress apIP = WiFi.softAPIP();

  Serial.println("Access Point aktif!");
  Serial.print("SSID       : ");
  Serial.println(ap_ssid);
  Serial.print("IP Address : ");
  Serial.println(apIP);
}

void loop() {
  // Menampilkan jumlah perangkat yang terhubung setiap 5 detik
  int jumlahClient = WiFi.softAPgetStationNum();

  Serial.print("Jumlah perangkat terhubung: ");
  Serial.println(jumlahClient);

  delay(5000);
}
```

## 4. Penjelasan Program

`#include <ESP8266WiFi.h>`

Memanggil library WiFi agar board dapat menggunakan fungsi jaringan.

`ap_ssid`

Menentukan nama jaringan WiFi yang akan dibuat oleh board.

`ap_password`

Menentukan password Access Point.

`Serial.begin(115200)`

Memulai komunikasi dengan Serial Monitor menggunakan baud rate 115200.

`WiFi.mode(WIFI_AP)`

Mengatur board menjadi Access Point sehingga board berperan sebagai penyedia jaringan WiFi.

`WiFi.softAP(ap_ssid, ap_password)`

Membuat jaringan WiFi menggunakan SSID dan password yang telah ditentukan.

`WiFi.softAPIP()`

Digunakan untuk mengetahui IP Address Access Point yang dibuat oleh board.

`WiFi.softAPgetStationNum()`

Digunakan untuk mengetahui jumlah perangkat yang sedang terhubung ke Access Point.

`delay(5000)`

Memberikan jeda selama 5 detik sebelum jumlah perangkat diperiksa kembali.

---

# H. HASIL PENGAMATAN PERCOBAAN 2B

## 1. Pengamatan Access Point (AP)

| No | Parameter                    | Nilai Konfigurasi | Hasil Pengamatan   |
| -- | ---------------------------- | ----------------- | ------------------ |
| 1  | SSID                         | apip diva hana    | apip diva hana     |
| 2  | Password                     | 12345678          | Berhasil digunakan |
| 3  | IP Address AP                | 192.168.4.1       | 192.168.4.1        |
| 4  | Status AP                    | Aktif             | Aktif              |
| 5  | SSID terdeteksi client       | Ya                | Ya                 |
| 6  | Perangkat berhasil terhubung | -                 | Ya, 2 smartphone   |

Hasil pengamatan menunjukkan bahwa Access Point berhasil dibuat menggunakan SSID “apip diva hana” dengan password “12345678”. IP Address Access Point yang diperoleh adalah 192.168.4.1. SSID dapat ditemukan oleh perangkat lain dan berhasil digunakan untuk menghubungkan dua smartphone 

# 2. Pengamatan Jumlah Perangkat Terhubung ke AP

| No | Waktu (s) | Jumlah Client | Perangkat           | Keterangan               |
| -- | --------: | ------------: | ------------------- | ------------------------ |
| 1  |         0 |             0 | Tidak ada           | Belum ada yang terhubung |
| 2  |         5 |             0 | Tidak ada           | Belum ada yang terhubung |
| 3  |        10 |             0 | Tidak ada           | Belum ada yang terhubung |
| 4  |        15 |             1 | HP Apip             | Terhubung                |
| 5  |        20 |             1 | HP Apip             | Terhubung                |
| 6  |        25 |             1 | HP Apip             | Terhubung                |
| 7  |        30 |             2 | HP Apip dan HP Diva | Terhubung                |
| 8  |        35 |             2 | HP Apip dan HP Diva | Terhubung                |
| 9  |        40 |             2 | HP Apip dan HP Diva | Terhubung                |
| 10 |        45 |             2 | HP Apip dan HP Diva | Terhubung                |

### Analisis

Hasil percobaan menunjukkan bahwa ESP berhasil berfungsi sebagai Access Point. Jaringan dengan SSID **“apip diva hana”** dapat ditemukan oleh smartphone dan berhasil digunakan oleh dua perangkat secara bersamaan.

Jumlah client bertambah sesuai dengan perangkat yang berhasil terhubung. Pada awalnya jumlah client adalah 0, kemudian menjadi 1 setelah HP Apip terhubung, dan menjadi 2 setelah HP Diva terhubung. Hal ini menunjukkan bahwa fungsi `WiFi.softAPgetStationNum()` berhasil digunakan untuk memantau jumlah perangkat yang terhubung ke Access Point.

<img width="747" height="423" alt="image" src="https://github.com/user-attachments/assets/27691e84-2023-42e0-818f-4c459e303b03" />
<img width="1077" height="655" alt="image" src="https://github.com/user-attachments/assets/318e3edd-0db3-41cb-bb37-f1a56b87b609" />

---

# J. PERTANYAAN PRAKTIKUM 2A

## 1. Gambarkan diagram alur (flowchart) proses koneksi ESP32 ke jaringan WiFi pada program di atas! 

<img width="579" height="1990" alt="Untitled Diagram drawio (5)" src="https://github.com/user-attachments/assets/ba906672-1d63-4690-a9b7-df023d6941ea" />

## 2. Apa fungsi dari perintah WiFi.mode(WIFI_STA) pada program tersebut? 

`WiFi.mode(WIFI_STA)` berfungsi untuk mengatur ESP agar bekerja dalam mode Station (STA), yaitu sebagai perangkat yang terhubung ke jaringan WiFi yang sudah tersedia. Dengan mode ini, ESP dapat melakukan koneksi ke WiFi menggunakan SSID dan password yang telah ditentukan.

## 3. Jelaskan apa yang terjadi apabila SSID atau password yang dimasukkan salah! 

Jika SSID atau password yang dimasukkan salah, ESP tidak dapat terhubung ke jaringan WiFi. Pada program, proses akan terus berada pada perulangan `while (WiFi.status() != WL_CONNECTED)`, sehingga pada Serial Monitor hanya muncul tanda titik (`.`) secara berulang. Karena koneksi tidak berhasil, informasi IP, MAC, dan RSSI tidak ditampilkan serta LED tidak menyala.

## 4. Modifikasi program agar ESP32 mencoba menghubungkan ulang (reconnect) secara otomatis apabila koneksi WiFi terputus, dan berikan penjelasan di setiap baris kode yang ditambahkan dalam bentuk README.md! 

Program telah dimodifikasi agar ketika koneksi WiFi terputus, ESP akan mendeteksi status tersebut, mematikan LED, kemudian menjalankan `WiFi.begin(ssid, password)` untuk mencoba terhubung kembali secara otomatis.

**[TEMPAT FOTO HASIL RECONNECT]**

> Tambahkan screenshot Serial Monitor yang menunjukkan proses reconnect.

---

# K. PERTANYAAN PRAKTIKUM 2B

## 1. Mengapa IP Default AP adalah `192.168.4.1`?

Alamat IP default Access Point pada ESP umumnya bernilai `192.168.4.1` karena merupakan alamat IP bawaan yang digunakan ESP sebagai gateway ketika bekerja dalam mode Access Point. Dengan alamat tersebut, perangkat yang terhubung ke jaringan WiFi yang dibuat ESP dapat berkomunikasi dengan ESP sebagai pusat jaringan.

## 2. Perbedaan Station dan Access Point

Perbedaan mendasar antara mode Station (STA) dan Access Point (AP) terletak pada peran ESP dalam jaringan WiFi.

Pada mode **Station**, ESP berperan sebagai **client** yang terhubung ke jaringan WiFi yang sudah tersedia.

Sedangkan pada mode **Access Point**, ESP berperan sebagai **pembuat jaringan WiFi**, sehingga perangkat lain seperti HP atau laptop dapat terhubung ke ESP.

## 3. Risiko Password Access Point Tidak Ada atau Terlalu Sederhana

Apabila password Access Point tidak diberikan atau terlalu sederhana, jaringan WiFi yang dibuat ESP menjadi tidak aman dan mudah diakses oleh pihak yang tidak berwenang.

Hal tersebut dapat menyebabkan perangkat lain terhubung ke ESP tanpa izin dan berpotensi mengakses atau mengganggu komunikasi serta data pada jaringan. Oleh karena itu, password Access Point sebaiknya menggunakan kombinasi karakter yang cukup kuat agar tidak mudah ditebak.

## 4. Modifikasi program agar ESP32 berjalan pada mode AP+STA (terhubung ke WiFi rumah sekaligus menyediakan Access Point), dan berikan penjelasan di setiap baris kode nya dalam bentuk README.md! 

### 1. Tujuan

Modifikasi ini bertujuan agar ESP dapat melakukan dua fungsi secara bersamaan, yaitu:

1. Terhubung ke WiFi rumah sebagai **Station (STA)**.
2. Menyediakan jaringan WiFi sendiri sebagai **Access Point (AP)**.

### 2. Program AP + STA

```cpp
#include <ESP8266WiFi.h>

// WiFi rumah untuk mode Station
const char* ssid     = "NAMA_WIFI_RUMAH";
const char* password = "PASSWORD_WIFI_RUMAH";

// Access Point yang dibuat ESP
const char* ap_ssid     = "ESP32_AccessPoint";
const char* ap_password = "12345678";

void setup() {
  Serial.begin(115200);

  // Mengatur ESP menjadi mode AP + STA
  WiFi.mode(WIFI_AP_STA);

  // Menghubungkan ESP ke WiFi rumah
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi rumah");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi rumah berhasil terhubung!");

  Serial.print("IP Station : ");
  Serial.println(WiFi.localIP());

  // Membuat Access Point
  WiFi.softAP(ap_ssid, ap_password);

  Serial.println("Access Point aktif!");

  Serial.print("SSID AP    : ");
  Serial.println(ap_ssid);

  Serial.print("IP AP      : ");
  Serial.println(WiFi.softAPIP());
}

void loop() {

  // Menampilkan status WiFi rumah
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Status STA: Terhubung");
  } else {
    Serial.println("Status STA: Terputus");
  }

  // Menampilkan jumlah perangkat yang terhubung ke AP
  int jumlahClient = WiFi.softAPgetStationNum();

  Serial.print("Jumlah perangkat AP: ");
  Serial.println(jumlahClient);

  delay(5000);
}
```

###PENJELASAN PROGRAM AP + STA

`#include <ESP8266WiFi.h>`

Memasukkan library WiFi agar ESP dapat menggunakan fungsi untuk mengatur koneksi jaringan.

`ssid`

Menyimpan nama WiFi rumah yang akan digunakan oleh ESP dalam mode Station.

`password`

Menyimpan password WiFi rumah.

`ap_ssid`

Menentukan nama jaringan WiFi yang dibuat ESP sebagai Access Point.

`ap_password`

Menentukan password untuk Access Point.

`Serial.begin(115200)`

Memulai komunikasi Serial Monitor dengan baud rate 115200.

`WiFi.mode(WIFI_AP_STA)`

Mengatur ESP agar bekerja dalam **dua mode sekaligus**, yaitu Station dan Access Point.

`WiFi.begin(ssid, password)`

Memulai koneksi ESP ke WiFi rumah sebagai Station.

`while (WiFi.status() != WL_CONNECTED)`

Mengecek koneksi WiFi secara terus-menerus sampai ESP berhasil terhubung.

`delay(500)`

Memberikan jeda selama 500 ms sebelum pengecekan berikutnya.

`Serial.print(".")`

Menampilkan tanda titik selama ESP masih mencoba terhubung ke WiFi rumah.

`WiFi.localIP()`

Menampilkan IP Address yang diperoleh ESP dari WiFi rumah dalam mode Station.

`WiFi.softAP(ap_ssid, ap_password)`

Membuat Access Point menggunakan SSID dan password yang telah ditentukan.

`WiFi.softAPIP()`

Menampilkan IP Address yang digunakan ESP sebagai Access Point.

`WiFi.status()`

Digunakan untuk mengecek status koneksi ESP ke WiFi rumah.

`WiFi.softAPgetStationNum()`

Digunakan untuk mengetahui jumlah perangkat yang terhubung ke Access Point ESP.

`delay(5000)`

Memberikan jeda selama 5 detik sebelum status koneksi dan jumlah client diperiksa kembali.

---

### 3. Konsep AP + STA

Pada mode AP+STA, ESP mempunyai dua peran dalam jaringan:

```text
                 WiFi Rumah
                     │
                     │
                     ▼
                ┌─────────┐
                │   ESP   │
                │ AP + STA│
                └─────────┘
                     │
            ┌────────┴────────┐
            │                 │
            ▼                 ▼
          HP 1              Laptop
        Client AP          Client AP
```

**STA:** ESP terhubung ke WiFi rumah.

**AP:** ESP membuat jaringan WiFi sendiri yang dapat digunakan perangkat lain.

---

# N. KESIMPULAN

Berdasarkan praktikum konfigurasi jaringan, dapat disimpulkan bahwa ESP dapat dikonfigurasi dalam beberapa mode jaringan WiFi, yaitu **Station (STA), Access Point (AP), dan AP+STA**.

Pada mode Station, ESP berfungsi sebagai perangkat yang terhubung ke jaringan WiFi yang sudah tersedia dan dapat memperoleh informasi seperti IP Address, MAC Address, serta RSSI. Jika SSID atau password salah, ESP tidak dapat terhubung ke jaringan.

Pada mode Access Point, ESP berfungsi sebagai pembuat jaringan WiFi sendiri sehingga perangkat lain seperti smartphone dapat terhubung. Access Point memiliki IP default `192.168.4.1` dan dapat digunakan untuk memantau jumlah perangkat yang terhubung.

Selain itu, penggunaan password yang kuat diperlukan untuk menjaga keamanan jaringan. Melalui modifikasi program, ESP juga dapat bekerja dalam mode AP+STA, yaitu terhubung ke jaringan WiFi yang sudah tersedia sekaligus menyediakan Access Point untuk perangkat lain.

Dengan demikian, praktikum ini memberikan pemahaman mengenai cara kerja dan konfigurasi jaringan WiFi pada ESP serta penerapannya dalam sistem IoT.

---

# O. DOKUMENTASI PRAKTIKUM

## Dokumentasi Percobaan 2A

**[FOTO 1 – RANGKAIAN STA]**

> Tempel foto di sini.

**[FOTO 2 – SERIAL MONITOR STA]**

> Tempel screenshot Serial Monitor di sini.

**[FOTO 3 – HASIL LED]**

> Tempel foto LED indikator di sini.

---

## Dokumentasi Percobaan 2B

**[FOTO 4 – RANGKAIAN/SETUP AP]**

> Tempel foto di sini.

**[FOTO 5 – SERIAL MONITOR AP]**

> Tempel screenshot Serial Monitor di sini.

**[FOTO 6 – SMARTPHONE TERHUBUNG AP]**

> Tempel screenshot/foto smartphone yang terhubung ke AP di sini.

---

## Dokumentasi Modifikasi Reconnect

**[FOTO 7 – PROSES RECONNECT]**

> Tempel screenshot Serial Monitor ketika ESP melakukan reconnect di sini.

---

## Dokumentasi Modifikasi AP+STA

**[FOTO 8 – SERIAL MONITOR AP+STA]**

> Tempel screenshot Serial Monitor di sini.

**[FOTO 9 – PERANGKAT TERHUBUNG AP]**

> Tempel foto/screenshot perangkat yang terhubung ke AP ESP di sini.

**[DIAGRAM 1 – AP+STA]**

> Tempel diagram jaringan AP+STA di sini.

---

# P. CATATAN / KENDALA PRAKTIKUM

Tuliskan kendala yang dialami selama praktikum.

Contoh:

* Koneksi WiFi perlu menggunakan SSID dan password yang benar.
* Jika password atau SSID salah, ESP tidak dapat menyelesaikan proses koneksi.
* Perubahan jaringan dapat menyebabkan IP Address yang diperoleh ESP berubah.
* Access Point perlu menggunakan password agar tidak mudah diakses oleh perangkat yang tidak berwenang.

**[TEMPAT CATATAN KENDALA TAMBAHAN]**

> Tambahkan kendala atau catatan lain yang ditemukan selama praktikum di sini.
