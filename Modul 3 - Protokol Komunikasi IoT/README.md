# MODUL 3 - PROTOKOL KOMUNIKASI IOT
## HTTP dan MQTT Dengan Format JSON
---

# A. TUJUAN PRAKTIKUM

Setelah melakukan praktikum, saya memahami bahwa tujuan dari percobaan ini adalah:

1. Memahami konsep dasar protokol komunikasi pada sistem IoT.
2. Mengetahui karakteristik serta perbedaan protokol HTTP dan MQTT.
3. Mengimplementasikan pengiriman data dari ESP32 ke server menggunakan HTTP POST.
4. Mengimplementasikan pengiriman data dari ESP32 ke broker menggunakan MQTT publish-subscribe.
5. Menggunakan format JSON untuk pertukaran data antara ESP32, server, broker, dan subscriber.
6. Mengetahui kelebihan dan kekurangan HTTP dan MQTT berdasarkan hasil percobaan.

Tujuan tersebut sesuai dengan tujuan praktikum pada modul, yaitu memahami HTTP dan MQTT serta mengimplementasikan keduanya menggunakan ESP32 dan format JSON.

---

# B. ALAT DAN BAHAN

Alat dan bahan yang digunakan:

- ESP8266
- Kabel USB
- Laptop/PC
- Arduino IDE
- Library ArduinoJson
- Library PubSubClient
- Jaringan WiFi yang terhubung internet
- MQTT client, seperti MQTT Explorer/HiveMQ WebSocket Client
- Broker MQTT broker.hivemq.com
- Endpoint HTTP httpbin.org/post
  
---
# C. PERCOBAAN 3A: KOMUNIKASI DATA MENGGUNAKAN HTTP

## Tujuan

Pada percobaan 3A, saya bertujuan untuk memahami cara ESP32 mengirimkan data ke server menggunakan protokol HTTP dengan metode POST. Data yang dikirim berupa data suhu dan kelembaban dalam format JSON.

## Rangkaian

Pada percobaan ini ESP32 terhubung ke laptop menggunakan kabel USB. ESP32 digunakan untuk menjalankan program dan mengirimkan data melalui jaringan WiFi.

<img width="769" height="1024" alt="foto rangkaian" src="https://github.com/user-attachments/assets/0b7078a1-5af0-4bb3-beb9-c58d9939e38f" />

## Program

```cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char* ssid = "..";
const char* password = "pipnurrr";
const char* serverUrl = "https://httpbin.org/post"; // endpoint uji HTTP POST

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi berhasil terhubung!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    http.begin(client, serverUrl);
    http.addHeader("Content-Type", "application/json");

    // Membuat objek data sensor dalam format JSON
    JsonDocument doc;
    doc["suhu"] = 28.5; // contoh data suhu (°C)
    doc["kelembaban"] = 65.0; // contoh data kelembaban (%)

    String requestBody;
    serializeJson(doc, requestBody);

    Serial.print("Mengirim data: ");
    Serial.println(requestBody);

    // Mengirim data melalui HTTP POST
    int httpResponseCode = http.POST(requestBody);

    if (httpResponseCode > 0) {
      Serial.print("Kode Response HTTP: ");
      Serial.println(httpResponseCode);
      Serial.println("Isi Response:");
      Serial.println(http.getString());
    } else {
      Serial.print("Pengiriman gagal, kode error: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }

  delay(10000); // kirim data setiap 10 detik
}
```
## Penjelasan Program

### 1. Library wifi

```cpp
#include <WiFi.h>
```

Digunakan agar ESP32 dapat terhubung ke jaringan WiFi.

### 2. Library HTTPClient

```cpp
#include <HTTPClient.h>
```

Digunakan untuk melakukan komunikasi HTTP antara ESP32 dengan server.

### 3. Library ArduinoJson

```cpp
#include <ArduinoJson.h>
```

Digunakan untuk membuat data dalam format JSON.

### 4. Konfigurasi WiFi dan server

```cpp
const char* ssid = "NAMA_WIFI_ANDA";
const char* password = "PASSWORD_WIFI_ANDA";
const char* serverUrl = "https://httpbin.org/post";
```

Bagian ini digunakan untuk menentukan jaringan WiFi dan alamat server tujuan.

### 5. Menghubungkan ESP32 ke WiFi

```cpp
WiFi.begin(ssid, password);
```

Perintah tersebut memulai proses koneksi ESP32 ke WiFi.

Program kemudian melakukan pengecekan sampai ESP32 berhasil terhubung:

```cpp
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}
```

### 6. Membuat data JSON

```cpp
JsonDocument doc;
doc["suhu"] = 28.5;
doc["kelembaban"] = 65.0;
```

Data suhu dan kelembaban dimasukkan ke dalam objek JSON.

Hasilnya menjadi:

```json
{"suhu":28.5,"kelembaban":65.0}
```

### 7. Mengubah JSON menjadi string

```cpp
String requestBody;
serializeJson(doc, requestBody);
```

Digunakan untuk mengubah objek JSON menjadi teks yang dapat dikirimkan melalui HTTP.

### 8. Menentukan tipe data

```cpp
http.addHeader("Content-Type", "application/json");
```

Bagian ini memberitahu server bahwa data yang dikirim memiliki format JSON.

### 9. Mengirim data

```cpp
int httpResponseCode = http.POST(requestBody);
```

Perintah tersebut mengirimkan data menggunakan metode HTTP POST.

### 10. Membaca response

```cpp
Serial.println(http.getString());
```

Digunakan untuk menampilkan isi balasan dari server pada Serial Monitor.

### 11. Jeda pengiriman

```cpp
delay(10000);
```

Data dikirim setiap 10 detik.

Program percobaan HTTP pada modul memang menggunakan HTTP POST, JSON, dan interval pengiriman 10 detik.

## Alur Percobaan
<img width="372" height="683" alt="Untitled Diagram drawio (5)" src="https://github.com/user-attachments/assets/0d8d2588-3b2d-4e6a-8112-db2082b3314f" />


## Hasil Pengamatan

### Tabel 1. Hasil Pengiriman Data melalui HTTP POST

| No. | Waktu Pengiriman (s) | Data JSON yang Dikirim | HTTP Response Code | Response Body | Status Pengiriman |
|---:|---:|---|---:|---|---|
| 1 | 0 | `{"suhu":28.5,"kelembaban":65.0}` | 200 | `"data": "{\"suhu\":28.5,\"kelembaban\":65.0}"` | Berhasil |
| 2 | 10 | `{"suhu":28.5,"kelembaban":65.0}` | 200 | `"data": "{\"suhu\":28.5,\"kelembaban\":65.0}"` | Berhasil |
| 3 | 20 | `{"suhu":28.5,"kelembaban":65.0}` | 200 | `"data": "{\"suhu\":28.5,\"kelembaban\":65.0}"` | Berhasil |
| 4 | 30 | `{"suhu":28.5,"kelembaban":65.0}` | 200 | `"data": "{\"suhu\":28.5,\"kelembaban\":65.0}"` | Berhasil |
| 5 | 40 | `{"suhu":28.5,"kelembaban":65.0}` | 200 | `"data": "{\"suhu\":28.5,\"kelembaban\":65.0}"` | Berhasil |
| 6 | 50 | `{"suhu":28.5,"kelembaban":65.0}` | 200 | `"data": "{\"suhu\":28.5,\"kelembaban\":65.0}"` | Berhasil |
| 7 | 60 | `{"suhu":28.5,"kelembaban":65.0}` | 200 | `"data": "{\"suhu\":28.5,\"kelembaban\":65.0}"` | Berhasil |
| 8 | 70 | `{"suhu":28.5,"kelembaban":65.0}` | 200 | `"data": "{\"suhu\":28.5,\"kelembaban\":65.0}"` | Berhasil |
| 9 | 80 | `{"suhu":28.5,"kelembaban":65.0}` | 200 | `"data": "{\"suhu\":28.5,\"kelembaban\":65.0}"` | Berhasil |
| 10 | 90 | `{"suhu":28.5,"kelembaban":65.0}` | 200 | `"data": "{\"suhu\":28.5,\"kelembaban\":65.0}"` | Berhasil |

## Analisis

Berdasarkan hasil pengamatan, ESP32 berhasil mengirimkan data JSON ke server menggunakan HTTP POST. Server memberikan response code 200 dan response body menunjukkan bahwa data JSON diterima. Hasil pengujian menunjukkan pengiriman berhasil pada seluruh 10 percobaan dari detik 0 sampai 90.

Response body yang diterima menunjukkan data yang dikirim kembali oleh server, yaitu:

```text
"data": "{\"suhu\":28.5,\"kelembaban\":65.0}"
```

Hal ini menunjukkan bahwa data JSON berhasil diterima oleh server.

### Output di serial monitor
<img width="1024" height="777" alt="image (4)" src="https://github.com/user-attachments/assets/9adabb11-37e3-462e-adbc-fdcd8ced60b7" />

### Output di browser
<img width="1255" height="420" alt="image (5)" src="https://github.com/user-attachments/assets/ecdc3332-47a1-4145-a3ff-7a3556e9e1e6" />

---

# D. PERCOBAAN 3B: KOMUNIKASI MQTT

## Tujuan

Pada percobaan 3B, saya bertujuan untuk memahami komunikasi MQTT menggunakan pola publish-subscribe. ESP32 bertindak sebagai publisher yang mengirim data JSON ke broker MQTT melalui topic tertentu, kemudian data tersebut diterima oleh subscriber.

Modul menetapkan bahwa ESP32 mempublikasikan data secara berkala dan data diverifikasi melalui MQTT client yang melakukan subscribe pada topic yang sama.

## Program

Program menggunakan tiga library utama:

```cpp
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "..";
const char* password = "pipnurrr";
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttTopic = "apipdivahana";

WiFiClient espClient;
PubSubClient client(espClient);

void hubungkanWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi berhasil terhubung!");
}

void hubungkanMQTT() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke broker MQTT...");
    String clientId = "ESP32Client-" +
                      String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("berhasil terhubung!");
    } else {
      Serial.print("gagal, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi dalam 2 detik");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  hubungkanWiFi();
  client.setServer(mqttServer, mqttPort);
}

void loop() {
  if (!client.connected()) {
    hubungkanMQTT();
  }

  client.loop();

  // Membuat data sensor dalam format JSON
  JsonDocument doc;
  doc["suhu"] = 28.5;
  doc["kelembaban"] = 65.0;

  char buffer[128];
  serializeJson(doc, buffer);

  // Mempublikasikan data ke topic MQTT
  client.publish(mqttTopic, buffer);

  Serial.print("Data terkirim ke topic ");
  Serial.print(mqttTopic);
  Serial.print(": ");
  Serial.println(buffer);

  delay(5000); // publish data setiap 5 detik
}
```

## Penjelasan Program

### 1. Library wifi

```cpp
#include <WiFi.h>
```

Digunakan agar ESP32 dapat terhubung ke jaringan WiFi.

### 2. Library PubSubClient

Library PubSubClient

Digunakan untuk melakukan komunikasi MQTT.

### 3. Library ArduinoJson

```cpp
#include <ArduinoJson.h>
```

Digunakan untuk membuat data dalam format JSON.

### 4. Menentukan broker dan topic

```cpp
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttTopic =
"unsoed/tk245004/kelompokAnda/sensor";
```

Bagian tersebut menentukan alamat broker MQTT, port, dan topic yang digunakan untuk mengirim data.

### 5. Membuat koneksi MQTT

```cpp
PubSubClient client(espClient);
```

Digunakan untuk membuat client MQTT pada ESP32.

### 6. Mengecek koneksi broker

```cpp
while (!client.connected()) {
```

Bagian ini mengecek apakah ESP32 sudah terhubung dengan broker MQTT.

Jika belum terhubung, ESP32 akan mencoba menghubungkan kembali.

### 7. client.loop()

```cpp
client.loop();
```

Digunakan untuk menjaga komunikasi MQTT tetap berjalan dan memproses komunikasi antara client dengan broker.

### 8. Membuat data JSON

```cpp
JsonDocument doc;
doc["suhu"] = 28.5;
doc["kelembaban"] = 65.0;
```

Data suhu dan kelembaban dimasukkan ke dalam format JSON.

### 9. Serialize JSON

```cpp
char buffer[128];
serializeJson(doc, buffer);
```

Data JSON diubah menjadi bentuk string yang disimpan dalam buffer.

### 10. Publish data

```cpp
client.publish(mqttTopic, buffer);
```

Data JSON dikirimkan ke broker MQTT melalui topic yang telah ditentukan.

### 11. Delay

```cpp
delay(5000);
```

Data dipublikasikan setiap 5 detik sesuai program pada modul.

## Alur Percobaan
<img width="345" height="683" alt="Untitled Diagram drawio (6)" src="https://github.com/user-attachments/assets/45e2abf5-db0f-47a6-9981-b601bb7f2aec" />

## Hasil Pengamatan

### Tabel 2. Hasil Komunikasi Data melalui MQTT

| No | Waktu Pengiriman (s) | Status Broker | Topic | Data JSON yang Dipublish | Data Diterima Subscriber | Status Pengiriman |
|---:|---:|---|---|---|---|---|
| 1 | 0 | Terhubung | apip diva hana | `{"suhu":28.5,"kelembaban":65.0}` | `{"suhu":28.5,"kelembaban":65.0}` | Berhasil |
| 2 | 10 | Terhubung | apip diva hana | `{"suhu":28.5,"kelembaban":65.0}` | `{"suhu":28.5,"kelembaban":65.0}` | Berhasil |
| 3 | 20 | Terhubung | apip diva hana | `{"suhu":28.5,"kelembaban":65.0}` | `{"suhu":28.5,"kelembaban":65.0}` | Berhasil |
| 4 | 30 | Terhubung | apip diva hana | `{"suhu":28.5,"kelembaban":65.0}` | `{"suhu":28.5,"kelembaban":65.0}` | Berhasil |
| 5 | 40 | Terhubung | apip diva hana | `{"suhu":28.5,"kelembaban":65.0}` | `{"suhu":28.5,"kelembaban":65.0}` | Berhasil |
| 6 | 50 | Terhubung | apip diva hana | `{"suhu":28.5,"kelembaban":65.0}` | `{"suhu":28.5,"kelembaban":65.0}` | Berhasil |
| 7 | 60 | Terhubung | apip diva hana | `{"suhu":28.5,"kelembaban":65.0}` | `{"suhu":28.5,"kelembaban":65.0}` | Berhasil |
| 8 | 70 | Terhubung | apip diva hana | `{"suhu":28.5,"kelembaban":65.0}` | `{"suhu":28.5,"kelembaban":65.0}` | Berhasil |
| 9 | 80 | Terhubung | apip diva hana | `{"suhu":28.5,"kelembaban":65.0}` | `{"suhu":28.5,"kelembaban":65.0}` | Berhasil |
| 10 | 90 | Terhubung | apip diva hana | `{"suhu":28.5,"kelembaban":65.0}` | `{"suhu":28.5,"kelembaban":65.0}` | Berhasil |

Berdasarkan hasil pengamatan, ESP32 berhasil terhubung ke broker MQTT dan mempublikasikan data JSON melalui topic apipdivahana. Data yang dipublikasikan juga berhasil diterima oleh subscriber. Seluruh 10 pengujian menunjukkan status berhasil.

Hasil tersebut menunjukkan bahwa proses publish dari ESP32 → broker → subscriber berjalan dengan baik.

<img width="1280" height="826" alt="image (6)" src="https://github.com/user-attachments/assets/e81acc29-02cb-4dae-ac7c-75e24f0624fe" />

<img width="1024" height="786" alt="image (7)" src="https://github.com/user-attachments/assets/e7e48773-db5d-4b77-ad2e-9a767fe2068c" />

---

# E. PERTANYAAN PRAKTIKUM PERCOBAAN 3A: KOMUNIKASI DATA MENGGUNAKAN HTTP

## 1. Gambarkan diagram alur (flowchart) proses pengiriman data melalui HTTP POST pada program di atas!

**Jawaban:**
<img width="371" height="891" alt="Untitled Diagram drawio (7)" src="https://github.com/user-attachments/assets/5ac19391-7d6f-47b5-9043-e2366ad4f6a3" />

## 2. Apa fungsi dari perintah `http.addHeader("Content-Type", "application/json")` pada program tersebut?

**Jawaban:**

Perintah `http.addHeader("Content-Type", "application/json")` digunakan untuk memberi informasi kepada server mengenai format data yang dikirim oleh ESP32. Nilai application/json menunjukkan bahwa isi request yang dikirim menggunakan format JSON.

Header ini penting karena server perlu mengetahui cara membaca atau memproses data yang diterimanya. Pada percobaan ini, data suhu dan kelembaban dibuat dalam bentuk JSON, sehingga Content-Type harus disesuaikan dengan format tersebut. Dengan adanya header ini, server dapat mengenali request sebagai data JSON dan memprosesnya dengan tepat.

## 3. Jelaskan arti dari kode response HTTP 200 dan sebutkan salah satu contoh kode response HTTP lain beserta artinya!

**Jawaban:**

Response code 200 menunjukkan bahwa request HTTP yang dikirim oleh ESP32 berhasil diterima dan diproses oleh server. Pada percobaan ini, seluruh pengiriman data mendapatkan response code 200, sehingga dapat disimpulkan bahwa proses komunikasi antara ESP32 dengan server berhasil dilakukan. Response body yang diterima juga menampilkan kembali data JSON yang dikirim.

Contoh lainnya adalah:

- **404 Not Found** → halaman atau endpoint yang diminta tidak ditemukan.
- **500 Internal Server Error** → terjadi kesalahan pada sisi server.

## 4. Modifikasi program agar ESP32 dapat mengirimkan data tambahan berupa waktu (dalam milidetik sejak dinyalakan menggunakan millis()) ke dalam JSON yang dikirim, dan berikan penjelasan di setiap baris kode yang ditambahkan dalam bentuk README.md

**Jawaban:**

Bagian yang ditambahkan:

```cpp
doc["waktu"] = millis();
```

Sehingga JSON menjadi:

```json
{
  "suhu": 28.5,
  "kelembaban": 65.0,
  "waktu": 12345
}
```

`millis()` digunakan untuk mengetahui waktu dalam milidetik sejak ESP32 mulai dijalankan.

**Program hasil modifikasi:**

```cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "NAMA_WIFI";
const char* password = "PASSWORD_WIFI";
const char* serverUrl = "https://httpbin.org/post";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi terhubung!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type",
                   "application/json");

    JsonDocument doc;
    doc["suhu"] = 28.5;
    doc["kelembaban"] = 65.0;

    // Baris tambahan untuk memasukkan waktu sejak
    // ESP32 menyala
    doc["waktu"] = millis();

    String requestBody;
    serializeJson(doc, requestBody);

    Serial.println("Mengirim data:");
    Serial.println(requestBody);

    int responseCode = http.POST(requestBody);

    if (responseCode > 0) {
      Serial.print("Response Code: ");
      Serial.println(responseCode);

      String responseBody = http.getString();
      Serial.println("Response Body:");
      Serial.println(responseBody);
    } else {
      Serial.print("Error: ");
      Serial.println(http.errorToString(responseCode));
    }

    http.end();
  }

  delay(10000);
}
```

---

# F. PERTANYAAN PRAKTIKUM PERCOBAAN 3B: KOMUNIKASI MQTT

## 1. Apa fungsi dari topic pada protokol MQTT, dan mengapa topic yang digunakan perlu dibuat unik?

**Jawaban:**

Topic pada MQTT berfungsi sebagai alamat atau jalur komunikasi yang digunakan untuk menentukan ke mana data akan dikirim dan dari mana data akan diterima. ESP32 melakukan publish data ke suatu topic, sedangkan subscriber melakukan subscribe pada topic yang sama agar dapat menerima data tersebut.

Topic perlu dibuat unik agar data dari satu perangkat atau kelompok tidak tercampur dengan data dari perangkat atau kelompok lain. Jika beberapa perangkat menggunakan topic yang sama tanpa tujuan yang jelas, subscriber dapat menerima data dari berbagai perangkat sekaligus sehingga sulit membedakan sumber datanya.

Pada praktikum, topic digunakan sebagai tempat ESP32 melakukan publish data sensor dan subscriber menggunakannya untuk menerima data tersebut.

## 2. Jelaskan fungsi dari perintah `client.loop()` yang dipanggil pada setiap iterasi `loop()`!

**Jawaban:**

`client.loop()` digunakan untuk menjaga komunikasi antara ESP32 dengan broker MQTT tetap berjalan. Fungsi ini membantu client MQTT memproses komunikasi yang masuk maupun keluar serta menjaga koneksi MQTT tetap aktif.

Pada program praktikum, `client.loop()` dipanggil secara terus-menerus di dalam `loop()`. Hal ini diperlukan karena MQTT menggunakan koneksi yang dapat berlangsung secara terus-menerus, berbeda dengan HTTP yang melakukan request kemudian menerima response. Jika fungsi tersebut tidak dijalankan dengan baik, komunikasi MQTT dapat terganggu dan client tidak dapat memproses komunikasi dengan broker secara normal.

## 3. Apa yang akan terjadi apabila koneksi ke broker MQTT terputus di tengah program berjalan?

**Jawaban:**

Jika koneksi ESP32 dengan broker MQTT terputus, ESP32 tidak dapat melakukan publish data ke broker. Oleh karena itu, program praktikum memiliki bagian untuk memeriksa status koneksi MQTT menggunakan `client.connected()`.

Jika client tidak terhubung, program akan menjalankan fungsi `hubungkanMQTT()` untuk mencoba melakukan koneksi kembali ke broker.

Setelah koneksi berhasil, ESP32 dapat kembali melakukan publish data JSON ke topic yang telah ditentukan. Mekanisme reconnect ini membuat sistem lebih mampu menghadapi gangguan koneksi tanpa harus menghentikan program secara keseluruhan.

---

# G. PERTANYAAN ANALISIS

## 1. Uraikan hasil tugas pada praktikum yang telah dilakukan pada setiap percobaan!

**Jawaban:**

Pada percobaan 3A menggunakan HTTP, ESP32 berhasil terhubung ke WiFi dan mengirimkan data suhu serta kelembaban dalam format JSON menggunakan metode HTTP POST. Dari 10 kali pengujian, seluruh pengiriman mendapatkan response code 200 dan data berhasil diterima oleh server.

Pada percobaan 3B menggunakan MQTT, ESP32 berhasil terhubung ke broker dan melakukan publish data JSON melalui topic apipdivahana. Data yang dikirim berhasil diterima oleh subscriber dan seluruh 10 kali pengujian menunjukkan status Berhasil.

## 2. Bandingkan besar overhead data dan pola komunikasi antara protokol HTTP dan MQTT berdasarkan hasil percobaan yang telah dilakukan!

**Jawaban:**

HTTP menggunakan pola request-response, yaitu ESP32 mengirim request ke server kemudian menunggu response. Pada praktikum, ESP32 mengirim data menggunakan HTTP POST dan menerima response dari server.

Sedangkan MQTT menggunakan pola publish-subscribe, yaitu ESP32 melakukan publish data ke broker melalui topic dan subscriber menerima data dari topic tersebut. MQTT memiliki overhead yang lebih ringan dibandingkan HTTP dan koneksinya dapat dipertahankan. Oleh karena itu, MQTT lebih efisien untuk komunikasi data yang dikirim berulang kali.

## 3. Untuk skenario pengiriman data sensor secara terus-menerus setiap beberapa detik dalam jangka waktu lama, protokol manakah (HTTP atau MQTT) yang lebih sesuai digunakan? Jelaskan alasannya!

**Jawaban:**

MQTT lebih sesuai untuk pengiriman data sensor secara terus-menerus setiap beberapa detik. MQTT memiliki overhead yang relatif rendah dan menggunakan koneksi yang dapat dipertahankan, sehingga cocok untuk komunikasi data IoT yang berlangsung secara berkala dan berkelanjutan.

Selain itu, dengan mekanisme publish-subscribe, ESP32 cukup mengirim data ke broker melalui topic, kemudian subscriber dapat menerima data tersebut. Hal ini membuat MQTT lebih sesuai untuk sistem monitoring sensor yang membutuhkan pengiriman data secara rutin.

## 4. Bagaimana peran format JSON dalam mendukung interoperabilitas data antara perangkat IoT dan berbagai platform/aplikasi yang berbeda?

**Jawaban:**

JSON berperan sebagai format pertukaran data yang terstruktur dan dapat digunakan pada berbagai perangkat maupun platform. Dalam praktikum, data suhu dan kelembaban disimpan dalam bentuk pasangan key-value, seperti `"suhu":28.5` dan `"kelembaban":65.0`.

Dengan menggunakan format JSON, data yang dikirim ESP32 melalui HTTP maupun MQTT dapat lebih mudah dibaca dan diproses oleh server, aplikasi, atau platform lain yang mendukung JSON. Oleh karena itu, JSON membantu perangkat dan sistem yang berbeda untuk saling bertukar data dengan format yang sama.

---

# H. KESIMPULAN

Berdasarkan praktikum Protokol Komunikasi IoT menggunakan HTTP dan MQTT dengan format JSON, dapat disimpulkan bahwa ESP32 berhasil melakukan komunikasi data melalui kedua protokol tersebut. Pada percobaan HTTP, ESP32 berhasil mengirimkan data suhu dan kelembaban dalam format JSON menggunakan metode POST ke server dan seluruh pengujian memperoleh response code 200. Pada percobaan MQTT, ESP32 juga berhasil terhubung ke broker, melakukan publish data melalui topic, dan data berhasil diterima oleh subscriber.

HTTP menggunakan pola request-response, sedangkan MQTT menggunakan pola publish-subscribe dengan broker sebagai perantara. MQTT memiliki overhead yang lebih ringan sehingga lebih sesuai untuk pengiriman data sensor secara berkala dan terus-menerus. Sementara itu, JSON berperan sebagai format data yang terstruktur sehingga data dari ESP32 dapat lebih mudah dipertukarkan dan diproses oleh berbagai server, aplikasi, maupun platform.

Dengan demikian, praktikum ini menunjukkan bahwa pemilihan protokol komunikasi perlu disesuaikan dengan kebutuhan sistem. HTTP cocok untuk komunikasi berbasis request dan response, sedangkan MQTT lebih sesuai untuk sistem IoT yang membutuhkan pengiriman data secara rutin dan berkelanjutan.

---

# I. KENDALA

1. Pengaturan board di Arduino IDE belum diubah ke ESP8266 sehingga library `ESP8266WiFi.h` tidak ditemukan saat kompilasi.
2. Protokol HTTPS pada ESP8266 wajib menyertakan objek `WiFiClientSecure` dan perintah `client.setInsecure()`, jika tidak maka pengiriman data akan gagal.
3. Memori RAM (heap) ESP8266 sangat terbatas sehingga beban enkripsi HTTPS rawan menyebabkan gagal koneksi (kode error -1) atau board crash/restart.
4. Ketidakcocokan versi ArduinoJson; penulisan `JsonDocument doc;` akan memicu error jika library masih menggunakan ArduinoJson versi 6.
5. Pembuatan objek WiFiClientSecure berulang kali di dalam `loop()` menyebabkan fragmentasi dan penumpukan memori RAM.
6. ESP8266 hanya mendukung frekuensi 2.4 GHz, sehingga koneksi akan tertahan (stuck) jika menghubungkan ke WiFi frekuensi 5 GHz.
7. Serial Monitor menampilkan karakter aneh saat booting awal karena baud rate bawaan ESP8266 berada di 74880 bps, bukan 115200 bps.
