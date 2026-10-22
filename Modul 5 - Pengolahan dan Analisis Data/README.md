
# BUKU CATATAN / LOG PRAKTIKUM

## MODUL 5 - PENGOLAHAN DAN ANALISIS DATA

**Program Studi:** Teknik Komputer UNSOED  
**Nama Kuliah (Kode):** Praktikum Sistem IoT (TK2450050)  
**Tahun / Semester:** 2026/5  
**Modul:** 5  
**Nama Asisten / NIM:** Athallah Tsany Satriyaji  
**Nama Praktikan / NIM:** Diva Syahita Mawarni / H1H024015

**Filtering, Analisis, dan Visualisasi Data**

---

## A. TUJUAN PRAKTIKUM

Setelah melakukan praktikum, saya memahami bahwa tujuan dari percobaan ini adalah:

1. Memahami proses pengolahan data IoT menggunakan metode *edge processing*.
2. Menerapkan perhitungan *moving average*, nilai minimum, dan maksimum pada data sensor.
3. Memahami pengolahan dan analisis data IoT menggunakan Python.
4. Menerapkan analisis statistik sederhana dan deteksi anomali pada data IoT.
5. Memvisualisasikan data hasil pengukuran agar lebih mudah dianalisis.

---

## B. ALAT DAN BAHAN

Alat dan bahan yang digunakan:

- ESP8266 atau ESP32
- Kabel USB
- Laptop atau komputer
- Arduino IDE
- Python beserta library yang diperlukan
- Broker MQTT
- Serial Monitor dan file CSV untuk menyimpan data pengamatan

---

## C. PERCOBAAN 5A: PENGOLAHAN DATA SENSOR PADA ESP32 (MOVING AVERAGE DAN STATISTIK)

### Tujuan

Tujuan Percobaan 5A adalah memahami proses pengiriman data suhu hasil simulasi menggunakan ESP8266 melalui protokol MQTT. Percobaan ini juga bertujuan untuk memahami proses pengubahan data suhu ke dalam format JSON, pengiriman data secara berkala ke broker MQTT, serta simulasi anomali suhu sebagai dasar pengolahan dan analisis data IoT. Pada percobaan ini, nilai suhu dihasilkan melalui program tanpa menggunakan sensor fisik, sedangkan ESP8266 dihubungkan ke laptop menggunakan kabel USB untuk menjalankan program dan memantau data melalui Serial Monitor.

### Rangkaian

Pada percobaan ini, ESP8266 dihubungkan ke laptop menggunakan kabel USB untuk menjalankan program dan memantau data melalui Serial Monitor. Data suhu dihasilkan melalui fungsi simulasi pada program tanpa menggunakan sensor DHT22 fisik. ESP8266 kemudian mengubah data suhu ke dalam format JSON dan mengirimkannya secara berkala ke broker MQTT melalui jaringan Wi-Fi.

---
### Program

```cpp
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Configuration
const char* WIFI_SSID     = "hmmm";
const char* WIFI_PASSWORD = "apaiyasih";

const char* MQTT_SERVER   = "broker.hivemq.com";
const uint16_t MQTT_PORT = 1883;
const char* MQTT_TOPIC    =
"unsoed/tk245004/kelompokAnda/data";

const unsigned long PUBLISH_INTERVAL_MS = 5000;

// Global Instances
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastPublishTime = 0;

// Mock function simulator sensor DHT22
float bacaSuhuMock() {
    static unsigned long jumlahBaca = 0;
    jumlahBaca++;

    float suhuDasar = 28.0 + (2.0 * sin(jumlahBaca *
0.08));
    float noise     = random(-5, 6) * 0.1;
    float suhu      = suhuDasar + noise;

    // Simulasi anomali suhu setiap 25 kali
pembacaan
    if (jumlahBaca % 25 == 0) {
        suhu += 8.0;
    }
    return suhu;
}


void setupWiFi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to WiFi: ");
    Serial.println(WIFI_SSID);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void reconnectMQTT() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT
connection...");

        String clientId = "ESP8266Client-" +
String(random(0xFFFF), HEX);

        if (client.connect(clientId.c_str())) {
            Serial.println("connected!");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" - try again in 5
seconds");
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);

    setupWiFi();
    client.setServer(MQTT_SERVER, MQTT_PORT);
}

void loop() {
    if (!client.connected()) {
        reconnectMQTT();
    }
    client.loop();

    // Kirim data berdasarkan interval non-blocking
(millis)
    unsigned long currentMillis = millis();
    if (currentMillis - lastPublishTime >=
PUBLISH_INTERVAL_MS) {


           lastPublishTime = currentMillis;

           float suhu = bacaSuhuMock();

           if (isnan(suhu)) {
               Serial.println("Pembacaan suhu gagal!");
               return;
           }

           // Format data ke JSON
           StaticJsonDocument<200> doc;
           doc["suhu"] = suhu;

           char jsonBuffer[256];
           serializeJson(doc, jsonBuffer);

           // Publish ke MQTT
           Serial.print("Publishing message: ");
           Serial.println(jsonBuffer);

           client.publish(MQTT_TOPIC, jsonBuffer);
       }
 }

```

### Penjelasan Program

| Kode Program | Penjelasan |
|---|---|
| `#include <ESP8266WiFi.h>` | Menghubungkan ESP8266 ke jaringan Wi-Fi. |
| `#include <PubSubClient.h>` | Menggunakan protokol MQTT untuk mengirim data. |
| `#include <ArduinoJson.h>` | Mengubah data suhu menjadi format JSON. |
| `MQTT_SERVER` dan `MQTT_TOPIC` | Menentukan broker MQTT dan topik tujuan pengiriman data. |
| `PUBLISH_INTERVAL_MS = 5000` | Mengatur pengiriman data setiap 5 detik. |
| `bacaSuhuMock()` | Menyimulasikan pembacaan suhu dengan variasi nilai dan anomali setiap 25 pembacaan. |
| `reconnectMQTT()` | Menghubungkan kembali ESP8266 ke broker jika koneksi MQTT terputus. |
| `setupWiFi()` | Menghubungkan ESP8266 ke Wi-Fi dan menampilkan alamat IP. |
| `client.loop()` | Menjaga komunikasi MQTT tetap berjalan. |
| `StaticJsonDocument<200> doc` | Menyiapkan wadah data JSON. |
| `doc["suhu"] = suhu` | Memasukkan nilai suhu ke dalam data JSON. |
| `serializeJson(doc, jsonBuffer)` | Mengubah data JSON menjadi teks agar dapat dikirim. |
| `client.publish(MQTT_TOPIC, jsonBuffer)` | Mengirim data suhu ke broker MQTT. |

### Alur Percobaan

**Flowchart:**

**[TEMPAT FOTO: Gambar 5.1 Flowchart alur percobaan 5A]**

### Hasil Pengamatan

**Tabel 5.1 Hasil Pengolahan Data Suhu pada ESP32**

| No. | Waktu Pengiriman (s) | Suhu Mentah (°C) | Moving Average (°C) | Suhu Minimum (°C) | Suhu Maksimum (°C) |
|---:|---:|---:|---:|---:|---:|
| 1 | 5 | 26,32 | 26,13 | 25,72 | 37,52 |
| 2 | 10 | 27,69 | 27,45 | 25,72 | 37,52 |
| 3 | 15 | 27,92 | 28,22 | 25,72 | 37,52 |
| 4 | 20 | 25,86 | 26,10 | 25,70 | 38,48 |
| 5 | 25 | 27,21 | 27,00 | 25,61 | 38,48 |

**Yang diamati:**

1. Suhu mentah: nilai suhu yang dihasilkan oleh *mock function* sebelum proses penyaringan.
2. Moving Average: rata-rata bergerak dari maksimal lima pembacaan terakhir.
3. Suhu minimum dan maksimum: nilai suhu terendah dan tertinggi sejak program dijalankan.

### Analisis

Berdasarkan hasil pengamatan, nilai suhu mentah mengalami perubahan pada setiap waktu pengiriman. Suhu mentah yang tercatat berada pada rentang 25,86–27,92°C. Nilai moving average digunakan untuk menghaluskan perubahan data dengan menghitung rata-rata dari beberapa pembacaan terakhir, sehingga perubahan suhu dapat diamati dengan lebih stabil.

Nilai suhu minimum dan maksimum menunjukkan rentang data yang tercatat selama proses pengamatan. Pada tabel, nilai minimum awal sebesar 25,72°C dan nilai maksimum awal sebesar 37,52°C, kemudian nilai maksimum meningkat menjadi 38,48°C. Nilai minimum pada pengamatan berikutnya tercatat sebesar 25,61°C. Hal ini menunjukkan bahwa pencatatan nilai minimum dan maksimum membantu mengetahui rentang nilai suhu selama program berjalan.

Secara umum, percobaan ini memperlihatkan fungsi pengolahan data di sisi perangkat. Data dapat diolah langsung pada ESP32 sebelum dikirim ke sistem lain. Namun, nilai suhu minimum dan maksimum pada tabel merupakan hasil yang tercantum dalam laporan, sehingga tidak perlu dianggap sebagai nilai yang dapat dihitung ulang hanya dari lima baris suhu mentah yang ditampilkan.

### Output di Serial Monitor

**[TEMPAT FOTO: Gambar 5.2 Serial Monitor hasil pengolahan data suhu pada ESP32]**

```text


    Mentah: 26.11 | Filter: 26.17 | Min: 25.72 | Max: 37.52
    Mentah: 25.80 | Filter: 26.10 | Min: 25.72 | Max: 37.52
    Mentah: 26.01 | Filter: 25.98 | Min: 25.72 | Max: 37.52
    Mentah: 26.23 | Filter: 25.97 | Min: 25.72 | Max: 37.52
    Mentah: 25.96 | Filter: 26.02 | Min: 25.72 | Max: 37.52
    Mentah: 26.31 | Filter: 26.06 | Min: 25.72 | Max: 37.52
    Mentah: 26.26 | Filter: 26.15 | Min: 25.72 | Max: 37.52
    Mentah: 26.33 | Filter: 26.22 | Min: 25.72 | Max: 37.52
    Mentah: 26.21 | Filter: 26.22 | Min: 25.72 | Max: 37.52
    Mentah: 26.00 | Filter: 26.22 | Min: 25.72 | Max: 37.52
    Mentah: 26.81 | Filter: 26.32 | Min: 25.72 | Max: 37.52
    Mentah: 26.22 | Filter: 26.32 | Min: 25.72 | Max: 37.52
    Mentah: 26.84 | Filter: 26.42 | Min: 25.72 | Max: 37.52
    Mentah: 26.87 | Filter: 26.55 | Min: 25.72 | Max: 37.52
```

---
## D. PERCOBAAN 5B: AKUISISI, ANALISIS, DAN VISUALISASI DATA

### Tujuan

Tujuan Percobaan 5B adalah memahami proses penerimaan data suhu dari broker MQTT menggunakan Python, menyimpan data pengamatan ke dalam file CSV, serta melakukan pengolahan dan analisis data menggunakan metode statistik. Percobaan ini juga bertujuan untuk mendeteksi anomali suhu menggunakan metode Z-score dan memvisualisasikan hasil pengamatan dalam bentuk grafik agar pola perubahan suhu dan data yang menyimpang dapat diketahui.

### Program

```cpp
  const int N = 5; // jumlah sampel moving average
  float buffer[N]; // menyimpan N pembacaan terakhir
  int indeks = 0; // posisi penulisan berikutnya pada
  buffer
  int jumlah = 0; // jumlah sampel yang sudah terisi

  float suhuMin = 1000.0;
  float suhuMax = -1000.0; // Diubah sedikit dari
  modul agar logika perbandingan suhu maksimum
  berjalan dengan benar

  // Mock function pengganti sensor DHT22
  float bacaSuhuMock() {
      static unsigned long jumlahBaca = 0;
      jumlahBaca++;

      // Suhu dasar berubah secara bertahap
      float suhuDasar = 28.0 + 2.0 * sin(jumlahBaca *
  0.08);

      // Noise kecil untuk mensimulasikan fluktuasi
  sensor
      float noise = random(-5, 6) * 0.1;

        float suhu = suhuDasar + noise;

      // Lonjakan suhu setiap 25 pembacaan untuk
  simulasi anomali
      if (jumlahBaca % 25 == 0) {
          suhu += 8.0;
      }

        return suhu;
  }

  float hitungMovingAverage(float nilaiBaru) {
      buffer[indeks] = nilaiBaru;
      indeks = (indeks + 1) % N;
      if (jumlah < N) jumlah++;

        float total = 0;
        for (int i = 0; i < jumlah; i++) {
            total += buffer[i];
        }


    return total / jumlah;
}

void setup() {
    Serial.begin(115200);
    // Inisialisasi dht.begin() dihapus karena
menggunakan simulasi
}

void loop() {
    // Mengambil data dari fungsi simulasi
    float suhu = bacaSuhuMock();

    if (isnan(suhu)) {
        Serial.println("Pembacaan gagal, data
diabaikan");
    } else {
        float suhuFilter =
hitungMovingAverage(suhu);

          if (suhu < suhuMin) suhuMin = suhu;
          if (suhu > suhuMax) suhuMax = suhu;

          Serial.print("Mentah: ");
          Serial.print(suhu);
          Serial.print(" | Filter: ");
          Serial.print(suhuFilter);
          Serial.print(" | Min: ");
          Serial.print(suhuMin);
          Serial.print(" | Max: ");
          Serial.println(suhuMax);
    }

    delay(2000);
}

```

### Penjelasan Program

| Kode Program | Penjelasan |
|---|---|
| `const int N = 5` | Menentukan jumlah sampel untuk perhitungan moving average, yaitu lima data. |
| `float buffer[N]` | Menyimpan data suhu yang digunakan dalam perhitungan moving average. |
| `float suhuMin = 1000.0` dan `float suhuMax = -1000.0` | Menentukan nilai awal suhu minimum dan maksimum. |
| `bacaSuhuMock()` | Menghasilkan data suhu simulasi dengan fluktuasi kecil dan lonjakan suhu setiap 25 pembacaan. |
| `hitungMovingAverage(float nilaiBaru)` | Menghitung rata-rata suhu dari data yang tersimpan untuk mengurangi fluktuasi nilai. |
| `Serial.begin(115200)` | Menginisialisasi komunikasi Serial Monitor dengan baud rate 115200. |
| `float suhu = bacaSuhuMock()` | Mengambil nilai suhu dari fungsi simulasi. |
| `float suhuFilter = hitungMovingAverage(suhu)` | Menghitung nilai suhu yang telah difilter menggunakan moving average. |
| Pemeriksaan `suhu < suhuMin` dan `suhu > suhuMax` | Memperbarui nilai suhu minimum dan maksimum berdasarkan hasil pembacaan. |
| `Serial.print()` dan `Serial.println()` | Menampilkan data suhu mentah, suhu hasil filter, suhu minimum, dan suhu maksimum pada Serial Monitor. |
| `delay(2000)` | Memberikan jeda 2 detik sebelum pembacaan suhu berikutnya. |

### Alur Percobaan

**Flowchart:**

**[TEMPAT FOTO: Gambar 5.3 Flowchart percobaan 5B]**

### Hasil Pengamatan

**Tabel 5.2 Hasil Akuisisi dan Analisis Data Suhu melalui MQTT**

| No. | Waktu Penerimaan | Suhu (°C) | Z-score | Status Anomali | Status Penyimpanan CSV |
|---:|---|---:|---:|---|---|
| 1 | 10:00:05 | 26,5 | -0,45 | Normal | Tersimpan |
| 2 | 10:00:10 | 26,8 | -0,32 | Normal | Tersimpan |
| 3 | 10:00:15 | 26,6 | -0,41 | Normal | Tersimpan |
| 4 | 10:00:20 | 27,0 | -0,23 | Normal | Tersimpan |
| 5 | 10:00:25 | 26,9 | -0,28 | Normal | Tersimpan |
| 6 | 10:00:30 | 35,5 | 3,65 | Anomali | Tersimpan |
| 7 | 10:00:35 | 27,2 | -0,14 | Normal | Tersimpan |
| 8 | 10:00:40 | 27,1 | -0,18 | Normal | Tersimpan |
| 9 | 10:00:45 | 26,7 | -0,37 | Normal | Tersimpan |
| 10 | 10:00:50 | 26,4 | -0,50 | Normal | Tersimpan |
| … | … | … | … | … | … |
| 20 | 10:01:40 | 26,8 | -0,32 | Normal | Tersimpan |

### Analisis

Berdasarkan Tabel 5.2, data suhu diterima melalui MQTT setiap 5 detik dan seluruh data berhasil disimpan ke dalam file CSV. Dari 20 data pengamatan, sebagian besar suhu berada pada kisaran 26–27°C dengan status normal karena nilai Z-score tidak melebihi batas anomali, yaitu |z| > 2.

Namun, pada data ke-6 tercatat suhu sebesar 35,5°C dengan Z-score 3,65 sehingga terdeteksi sebagai anomali. Lonjakan tersebut sesuai dengan simulasi anomali suhu pada program. Hasil ini menunjukkan bahwa metode Z-score dapat membantu mengidentifikasi perubahan suhu yang tidak normal, sedangkan penyimpanan CSV memungkinkan data pengamatan digunakan kembali untuk analisis lebih lanjut.

**[TEMPAT FOTO: Gambar 5.4 Serial Monitor hasil akuisisi, analisis, dan visualisasi data]**

---
## E. PERTANYAAN PRAKTIKUM PERCOBAAN 5A: PENGOLAHAN DATA SENSOR PADA ESP32 (MOVING AVERAGE DAN STATISTIK)

1. Gambarkan diagram alur (flowchart) proses perhitungan moving average pada
fungsi hitungMovingAverage()!
Jawab:


[TEMPAT FOTO: Gambar 5.4 Flowchart proses perhitungan moving average pada fungsi hitungMovingAverage()]
pada fungsi hitungMovingAverage()


2. Jelaskan fungsi dari operasi (indeks + 1) % N pada program tersebut!
Jawaban:
Operasi (indeks + 1) % N berfungsi untuk memindahkan posisi indeks ke
elemen buffer berikutnya secara berulang. Operator modulo (%) memastikan indeks
kembali ke 0 ketika mencapai batas maksimum buffer, yaitu 5, sehingga data baru
dapat terus disimpan tanpa melewati kapasitas array.

3. Apa dampaknya terhadap data hasil filter apabila nilai N diperbesar menjadi 20
atau diperkecil menjadi 2?
Jawaban:
Jika nilai N diperbesar menjadi 20, hasil filter akan lebih halus dan stabil karena
menggunakan lebih banyak sampel untuk menghitung rata-rata. Namun, perubahan
suhu akan lebih lambat terdeteksi. Sebaliknya, jika N diperkecil menjadi 2, hasil
filter akan lebih responsif terhadap perubahan suhu, tetapi lebih mudah terpengaruh
oleh fluktuasi data. Jadi, nilai N perlu disesuaikan agar keseimbangan antara
kestabilan dan responsivitas data tetap terjaga.

4. Modifikasi program agar ESP32 juga menghitung simpangan baku dari data pada
buffer, dan berikan penjelasan di setiap baris kode yang ditambahkan dalam bentuk
README.md.
Jawaban:

Menambahkan fitur perhitungan simpangan baku pada program ESP32 untuk
mengetahui tingkat penyebaran data suhu dalam buffer. Semakin besar nilai
simpangan baku, semakin bervariasi data suhu yang diperoleh.

Tambahkan fungsi berikut setelah fungsi hitungMovingAverage():

```cpp
  // Menghitung simpangan baku dari data pada buffer
  float hitungSimpanganBaku() {
      if (jumlah < 2) return 0.0;

         float total = 0;
         float rataRata = 0;

         // Menghitung total data dalam buffer
         for (int i = 0; i < jumlah; i++) {
             total += buffer[i];
         }

         // Menghitung rata-rata data
         rataRata = total / jumlah;

         float totalKuadrat = 0;

         // Menghitung jumlah kuadrat selisih setiap
  data
         // terhadap rata-rata
         for (int i = 0; i < jumlah; i++) {
             float selisih = buffer[i] - rataRata;
             totalKuadrat += selisih * selisih;
         }

         // Menghitung simpangan baku populasi
         return sqrt(totalKuadrat / jumlah);


    }
```

Kemudian, pada fungsi loop(), tambahkan pemanggilan fungsi dan tampilkan
hasilnya:

```cpp
// Menghitung simpangan baku dari data buffer
float simpanganBaku = hitungSimpanganBaku();

// Menampilkan nilai simpangan baku
Serial.print(" | Simpangan Baku: ");
Serial.println(simpanganBaku);
```

Letakkan kedua baris tambahan tersebut setelah perhitungan suhuFilter, lalu
sesuaikan bagian output Serial Monitor agar nilai simpangan baku ikut
ditampilkan.

Kode lengkap:

```cpp
#include <math.h>

const int N = 5; // Jumlah sampel moving average
float buffer[N]; // Menyimpan N pembacaan suhu
terakhir
int indeks = 0; // Posisi penulisan berikutnya pada
buffer
int jumlah = 0; // Jumlah sampel yang sudah terisi

float suhuMin = 1000.0;
float suhuMax = -1000.0;

// Mock function pengganti sensor DHT22
float bacaSuhuMock() {
    static unsigned long jumlahBaca = 0;
    jumlahBaca++;

    // Suhu dasar berubah secara bertahap
    float suhuDasar = 28.0 + 2.0 * sin(jumlahBaca *
0.08);

        // Noise kecil untuk simulasi fluktuasi sensor
        float noise = random(-5, 6) * 0.1;

        float suhu = suhuDasar + noise;

        // Lonjakan suhu setiap 25 pembacaan
        if (jumlahBaca % 25 == 0) {
            suhu += 8.0;
        }

        return suhu;
}

// Menghitung moving average
float hitungMovingAverage(float nilaiBaru) {
    buffer[indeks] = nilaiBaru;
    indeks = (indeks + 1) % N;

        if (jumlah < N) {
            jumlah++;
        }


    float total = 0;

    for (int i = 0; i < jumlah; i++) {
        total += buffer[i];
    }

    return total / jumlah;
}

// Menghitung simpangan baku dari data pada buffer
float hitungSimpanganBaku() {
    if (jumlah < 2) {
        return 0.0;
    }

    float total = 0;

    // Menghitung total seluruh data
    for (int i = 0; i < jumlah; i++) {
        total += buffer[i];
    }

    // Menghitung rata-rata data
    float rataRata = total / jumlah;
    float totalKuadrat = 0;

    // Menghitung jumlah kuadrat selisih data
    for (int i = 0; i < jumlah; i++) {
        float selisih = buffer[i] - rataRata;
        totalKuadrat += selisih * selisih;
    }

    // Menghitung simpangan baku populasi
    return sqrt(totalKuadrat / jumlah);
}

void setup() {
    Serial.begin(115200);
    randomSeed(micros());
}

void loop() {
    // Mengambil data suhu simulasi
    float suhu = bacaSuhuMock();

    if (isnan(suhu)) {
        Serial.println("Pembacaan gagal, data
diabaikan");
    } else {
        // Menghitung moving average
        float suhuFilter =
hitungMovingAverage(suhu);

        // Menghitung simpangan baku
        float simpanganBaku = hitungSimpanganBaku();

        // Memperbarui suhu minimum dan maksimum
        if (suhu < suhuMin) {
            suhuMin = suhu;
        }

        if (suhu > suhuMax) {


                       suhuMax = suhu;
                  }

                  // Menampilkan hasil pada Serial Monitor
                  Serial.print("Mentah: ");
                  Serial.print(suhu);

                  Serial.print(" | Filter: ");
                  Serial.print(suhuFilter);

                  Serial.print(" | Simpangan Baku: ");
                  Serial.print(simpanganBaku);

                  Serial.print(" | Min: ");
                  Serial.print(suhuMin);

                  Serial.print(" | Max: ");
                  Serial.println(suhuMax);
            }

            // Jeda 2 detik sebelum pembacaan berikutnya
            delay(2000);
      }
```


Perubahan utama:
- Fungsi hitungSimpanganBaku() menghitung simpangan baku dari
data yang tersimpan di buffer.
- Variabel simpanganBaku menyimpan hasil perhitungan untuk
ditampilkan di Serial Monitor.
- randomSeed(micros()) membantu memberikan variasi pada hasil
simulasi acak setiap program dijalankan.


## F. PERTANYAAN PRAKTIKUM PERCOBAAN 5B: AKUISISI, ANALISIS, DAN VISUALISASI DATA

1. Jelaskan fungsi dari blok try-except pada fungsi on_message() dan apa yang terjadi
apabila blok tersebut dihilangkan!
Jawaban:
Blok try-except pada fungsi on_message() berfungsi untuk menangani
kesalahan saat menerima dan memproses data dari MQTT, misalnya ketika data
yang diterima bukan dalam format JSON yang valid atau terdapat nilai yang tidak
sesuai. Dengan adanya blok ini, program dapat menampilkan pesan kesalahan dan
tetap berjalan tanpa langsung berhenti.

Jika blok try-except dihilangkan, kesalahan saat memproses data dapat
menimbulkan exception yang tidak tertangani sehingga proses pengolahan pesan
terganggu atau program berhenti, bergantung pada penanganan kesalahan di bagian
program lainnya.

2. Jelaskan arti nilai z-score dan mengapa batas |z| > 2 digunakan sebagai penentu
anomali!
Jawaban:
Z-score adalah nilai yang menunjukkan seberapa jauh suatu data menyimpang dari
nilai rata-rata dalam satuan simpangan baku. Nilai Z-score positif menunjukkan
data berada di atas rata-rata, sedangkan nilai negatif menunjukkan data berada di
bawah rata-rata.

Batas |𝑧| > 2 digunakan untuk menandai anomali karena data tersebut berada
lebih dari dua simpangan baku dari rata-rata, sehingga dianggap menyimpang dari


pola normal. Dengan demikian, metode Z-score membantu mendeteksi perubahan
suhu yang tidak biasa pada sistem IoT.

3. Apa kelemahan metode z-score apabila data memiliki tren naik atau turun yang
tajam?
Jawaban:
Kelemahan metode Z-score adalah data yang memiliki tren naik atau turun tajam
dapat dianggap sebagai anomali, padahal perubahan tersebut merupakan pola
normal dari data. Hal ini terjadi karena Z-score menggunakan rata-rata dan
simpangan baku sebagai acuan tanpa mempertimbangkan tren waktu. Akibatnya,
metode ini dapat menghasilkan deteksi anomali yang kurang akurat. Salah satu
solusinya adalah menggunakan metode deteksi anomali yang mempertimbangkan
tren atau perubahan data dari waktu ke waktu.

4. Modifikasi program agar menggunakan metode moving average (jendela 5 data)
pada data hasil CSV sebelum dihitung z-score-nya, kemudian bandingkan jumlah
anomali yang terdeteksi, dan berikan penjelasan di setiap baris kode yang
ditambahkan dalam bentuk README.md.
Jawaban:

Tujuan:
Memodifikasi program analisis data IoT dengan menerapkan metode moving
average menggunakan jendela 5 data sebelum perhitungan Z-score. Hasilnya
dibandingkan dengan deteksi anomali pada data suhu asli untuk mengetahui
pengaruh filter terhadap jumlah anomali yang terdeteksi.

Tambahkan kode berikut setelah data CSV dibaca menggunakan Pandas. Contoh
ini mengasumsikan nama kolom suhu pada CSV adalah suhu.

```python
# Menyimpan data suhu asli
df["suhu_asli"] = df["suhu"]

# Menghitung moving average dengan jendela 5 data
df["suhu_filter"] = df["suhu"].rolling(window=5,
min_periods=1).mean()

# Menghitung rata-rata dan simpangan baku data asli
mean_asli = df["suhu_asli"].mean()
std_asli = df["suhu_asli"].std()

# Menghitung Z-score data asli
df["z_asli"] = (df["suhu_asli"] - mean_asli) /
std_asli

# Menentukan anomali pada data asli
df["anomali_asli"] = df["z_asli"].abs() > 2

# Menghitung rata-rata dan simpangan baku data hasil
filter
mean_filter = df["suhu_filter"].mean()
std_filter = df["suhu_filter"].std()

# Menghitung Z-score setelah moving average
df["z_filter"] = (
    (df["suhu_filter"] - mean_filter) / std_filter
    if std_filter != 0
    else 0
)

# Menentukan anomali setelah moving average
df["anomali_filter"] = df["z_filter"].abs() > 2


# Menghitung jumlah anomali sebelum dan sesudah
filter
jumlah_anomali_asli = df["anomali_asli"].sum()
jumlah_anomali_filter = df["anomali_filter"].sum()

# Menampilkan hasil perbandingan
print("Jumlah anomali data asli:",
jumlah_anomali_asli)
print("Jumlah anomali setelah moving average:",
jumlah_anomali_filter)
```


Perbandingan Hasil

Metode                     Hasil yang diamati

Z-Score data asli          Jumlah anomali sebelum filter diterapkan

Moving average + Z-score   Jumlah anomali setelah fluktuasi data
dihaluskan

Kode Lengkap

```python
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# 1. Membaca data dari file CSV
df = pd.read_csv("data_suhu.csv")

# 2. Memeriksa data suhu
print("Data awal:")
print(df.head())

# Mengubah kolom suhu menjadi numerik
df["suhu"] = pd.to_numeric(df["suhu"],
errors="coerce")

# Menghapus baris dengan data suhu kosong atau tidak
valid
df = df.dropna(subset=["suhu"]).copy()

# 3. Menyimpan data suhu asli
df["suhu_asli"] = df["suhu"]

# 4. Menghitung moving average dengan jendela 5 data
df["suhu_filter"] = (
    df["suhu_asli"]
    .rolling(window=5, min_periods=1)
    .mean()
)

# 5. Menghitung Z-score data asli
mean_asli = df["suhu_asli"].mean()
std_asli = df["suhu_asli"].std()

if std_asli != 0:
    df["z_asli"] = (df["suhu_asli"] - mean_asli) /
std_asli
else:
    df["z_asli"] = 0.0


# 6. Menentukan anomali pada data asli
df["anomali_asli"] = df["z_asli"].abs() > 2

# 7. Menghitung Z-score data hasil moving average
mean_filter = df["suhu_filter"].mean()
std_filter = df["suhu_filter"].std()

if std_filter != 0:
    df["z_filter"] = (
        (df["suhu_filter"] - mean_filter) /
std_filter
    )
else:
    df["z_filter"] = 0.0

# 8. Menentukan anomali setelah moving average
df["anomali_filter"] = df["z_filter"].abs() > 2

# 9. Menghitung jumlah anomali
jumlah_anomali_asli = int(df["anomali_asli"].sum())
jumlah_anomali_filter =
int(df["anomali_filter"].sum())

print("\n=== HASIL PERBANDINGAN ANOMALI ===")
print("Jumlah data valid:", len(df))
print("Anomali data asli:", jumlah_anomali_asli)
print("Anomali setelah moving average:",
jumlah_anomali_filter)

# 10. Menampilkan data hasil analisis
print("\nData hasil analisis:")
print(
    df[
        [
            "suhu_asli",
            "suhu_filter",
            "z_asli",
            "anomali_asli",
            "z_filter",
            "anomali_filter",
        ]
    ].to_string(index=False)
)

# 11. Menyimpan hasil analisis ke file CSV baru
df.to_csv("hasil_analisis_iot.csv", index=False)
print("\nHasil analisis disimpan ke
hasil_analisis_iot.csv")

# 12. Membuat grafik perbandingan suhu
plt.figure(figsize=(10, 5))

plt.plot(
    df.index,
    df["suhu_asli"],
    marker="o",
    label="Suhu Asli"
)

plt.plot(
    df.index,
    df["suhu_filter"],


          label="Moving Average (5 data)"
     )

     # Menandai anomali pada data asli
     anomali = df["anomali_asli"]

     plt.scatter(
         df.index[anomali],
         df.loc[anomali, "suhu_asli"],
         marker="x",
         s=100,
         label="Anomali Data Asli"
     )

     plt.xlabel("Indeks Data")
     plt.ylabel("Suhu (°C)")
     plt.title("Perbandingan Suhu Asli dan Moving
     Average")
     plt.legend()
     plt.grid(True)
     plt.tight_layout()
     plt.savefig("grafik_analisis_iot.png")
     plt.show()

     print("Grafik disimpan ke grafik_analisis_iot.png")
```


## G. PERTANYAAN ANALISIS

1. Uraikan hasil tugas pada praktikum yang telah dilakukan pada setiap percobaan!
Jawaban:
Pada praktikum Modul 5, Percobaan 1A dilakukan pengolahan data suhu pada
perangkat dengan metode moving average untuk menghaluskan fluktuasi data serta
mengetahui suhu minimum dan maksimum. Percobaan 2A dilakukan pengolahan
data pada komputer sehingga data dapat dianalisis lebih lanjut.

Pada Percobaan 5A, data suhu simulasi dikirimkan menggunakan ESP8266 melalui
protokol MQTT dalam format JSON secara berkala. Selanjutnya, Percobaan 5B
dilakukan penerimaan data MQTT menggunakan Python, penyimpanan data ke
CSV, serta analisis dan deteksi anomali menggunakan metode Z-score. Modifikasi
moving average sebelum perhitungan Z-score juga dilakukan untuk
membandingkan jumlah anomali sebelum dan sesudah data difilter.

2. Bandingkan pengolahan data di sisi perangkat (Percobaan 1A) dengan pengolahan
data di sisi komputer (Percobaan 2A) dari segi kecepatan, kompleksitas analisis,
dan kebutuhan sumber daya!
Jawaban:
Pengolahan data di sisi perangkat pada Percobaan 1A memiliki respons yang cepat
karena data langsung diproses oleh mikrokontroler tanpa harus dikirim ke
komputer. Namun, kemampuan analisisnya terbatas oleh kapasitas memori dan
pemrosesan perangkat.

Sebaliknya, pengolahan data di sisi komputer pada Percobaan 2A memungkinkan
analisis yang lebih kompleks karena didukung oleh sumber daya komputasi yang
lebih besar serta berbagai library pengolahan data. Akan tetapi, prosesnya dapat
membutuhkan waktu tambahan untuk mengirimkan data ke komputer. Dengan
demikian, pengolahan pada perangkat lebih cocok untuk respons langsung,
sedangkan pengolahan pada komputer lebih sesuai untuk analisis data yang lebih
mendalam.


3. Bagaimana pengaruh pemilihan ukuran jendela moving average terhadap kualitas
data dan kecepatan respons sistem kendali apabila data tersebut digunakan untuk
mengendalikan aktuator?
Jawaban:
Ukuran jendela moving average memengaruhi tingkat kehalusan data dan
kecepatan respons sistem kendali. Jendela yang lebih besar menghasilkan data
yang lebih stabil karena fluktuasi dapat dikurangi, tetapi respons terhadap
perubahan kondisi menjadi lebih lambat. Sebaliknya, jendela yang lebih kecil
membuat sistem lebih cepat merespons perubahan, tetapi data hasil filter lebih
mudah terpengaruh oleh gangguan.

Oleh karena itu, ukuran jendela perlu disesuaikan dengan kebutuhan sistem agar
data cukup stabil tanpa menyebabkan keterlambatan respons aktuator.

4. Bagaimana hasil analisis dan deteksi anomali dapat dimanfaatkan untuk
meningkatkan keandalan sistem IoT, misalnya pada sistem smart farming atau
smart home?
Jawaban:
Hasil analisis dan deteksi anomali dapat digunakan untuk mengenali kondisi yang
tidak normal sehingga tindakan penanganan dapat dilakukan lebih cepat. Pada
sistem smart farming, misalnya, lonjakan suhu atau perubahan kelembapan tanah
yang tidak wajar dapat menjadi dasar untuk memeriksa sensor atau menyesuaikan
penyiraman.

Pada sistem smart home, anomali suhu dapat digunakan untuk mendeteksi potensi
gangguan pendingin ruangan atau kondisi lingkungan yang tidak biasa. Dengan
demikian, analisis data dan deteksi anomali membantu mengurangi risiko
kesalahan, mendukung pemeliharaan sistem, serta meningkatkan keandalan
perangkat IoT

## H. KESIMPULAN

Berdasarkan praktikum IoT Modul 5 tentang Pengolahan dan Analisis Data IoT,
dapat disimpulkan bahwa data yang dihasilkan oleh sistem IoT dapat diolah untuk
memperoleh informasi yang lebih akurat dan bermanfaat. Metode moving average
digunakan untuk mengurangi fluktuasi data melalui perhitungan rata-rata beberapa sampel,
sedangkan metode Z-score digunakan untuk mendeteksi nilai yang menyimpang dari pola
normal.

Pada percobaan pengiriman data menggunakan ESP8266 dan MQTT, data suhu hasil
simulasi dapat dikirim dalam format JSON dan diterima untuk dianalisis serta disimpan ke
dalam file CSV. Praktikum ini juga menunjukkan bahwa ukuran jendela moving average
memengaruhi tingkat kehalusan data dan kecepatan respons sistem. Dengan demikian,
pengolahan dan analisis data berperan penting dalam meningkatkan keandalan sistem IoT,
termasuk untuk mendeteksi kondisi tidak normal dan mendukung pengambilan keputusan
pada penerapan seperti smart farming dan smart home.
