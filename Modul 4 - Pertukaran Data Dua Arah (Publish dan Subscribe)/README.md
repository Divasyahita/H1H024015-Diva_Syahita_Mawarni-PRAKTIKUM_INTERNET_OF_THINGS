# MODUL 3 - KOMUNIKASI PERTUKARAN DATA
## A. TUJUAN PRAKTIKUM

Setelah melakukan praktikum, saya memahami bahwa tujuan dari percobaan ini adalah:

1. Memahami konsep pertukaran data dua arah pada sistem IoT.
2. Memahami mekanisme subscribe untuk menerima perintah melalui MQTT.
3. Mengimplementasikan penerimaan perintah MQTT untuk mengendalikan aktuator.
4. Mengimplementasikan proses publish data sensor dan subscribe perintah secara bersamaan.
5. Memahami penggunaan millis() untuk melakukan proses secara non-blocking.
6. Mengetahui kendala yang dapat terjadi pada komunikasi IoT ketika terdapat masalah pada perangkat sensor.

Tujuan tersebut sesuai dengan modul yang mengharapkan ESP8266 dapat mengirim data sensor sekaligus menerima perintah kendali secara bersamaan melalui MQTT.

---

## B. ALAT DAN BAHAN

Alat dan bahan yang digunakan:

- ESP8266 DevKit
- Sensor DHT11
- LED
- Resistor 220 Ohm
- Breadboard
- Kabel jumper
- Kabel USB
- Laptop/PC
- Arduino IDE
- Library DHT
- Library PubSubClient
- Library ArduinoJson
- Jaringan WiFi
- MQTT Client
- Broker MQTT broker.hivemq.com

---

## C. PERCOBAAN 4A: SUBSCRIBE DAN DESERIALISASI DATA JSON UNTUK KENDALI AKTUATOR

### Tujuan

Pada percobaan 4A, saya bertujuan untuk memahami mekanisme subscribe pada MQTT dan proses deserialisasi data JSON yang diterima untuk mengendalikan LED sebagai aktuator.

ESP8266 melakukan subscribe pada topic perintah. Ketika menerima pesan JSON seperti `{"perintah":"ON"}` atau `{"perintah":"OFF"}`, pesan tersebut diproses dan digunakan untuk mengubah kondisi LED. Modul mengharapkan Serial Monitor menampilkan pesan yang diterima beserta hasil parsing dan perubahan status LED.

### Rangkaian

Pada percobaan ini ESP8266 dihubungkan dengan LED sebagai aktuator. LED digunakan untuk menunjukkan hasil dari perintah yang diterima melalui MQTT.
<img width="1280" height="563" alt="WhatsApp Image 2026-09-29 at 13 48 06" src="https://github.com/user-attachments/assets/4a81f0f2-3f84-40cb-8a37-66c53ac5f606" />

---

### Program

```cpp
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

// Konfigurasi WiFi
const char* ssid = "hmmm";
const char* password = "apaiyasih";

// Konfigurasi Broker MQTT
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

// Definisi Topic MQTT (Gunakan nama topic yang unik
// untuk kelompok Anda)
const char* topicData =
"unsoed/tk245004/kelompokAnda/data";
const char* topicPerintah =
"unsoed/tk245004/kelompokAnda/perintah";

// Konfigurasi Pin Hardware
#define DHTPIN D4
#define DHTTYPE DHT11
const int ledPin = 26; // Sesuaikan dengan pin LED Anda
// (misal GPIO 2 untuk NodeMCU)

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

// Variabel untuk manajemen waktu non-blocking
// menggunakan millis()
unsigned long waktuTerakhirPublish = 0;
const long intervalPublish = 5000; // Publish data
// setiap 5 detik

// Fungsi callback untuk menerima pesan MQTT secara
// asynchronous (subscribe)
void callback(char* topic, byte* payload, unsigned int
length) {
  String pesan = "";
  for (unsigned int i = 0; i < length; i++) {
    pesan += (char)payload[i];
  }

  // Deserialisasi data JSON yang diterima
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc,
pesan);
  if (error) {
    return; // Abaikan jika parsing gagal
  }

  const char* perintah = doc["perintah"];

  // Kontrol aktuator LED berdasarkan perintah JSON
  // ("ON" / "OFF")
  digitalWrite(ledPin, String(perintah) == "ON" ? HIGH
: LOW);
  Serial.print("Perintah diterima -> Aktuator: ");
  Serial.println(perintah);
}

// Fungsi untuk menghubungkan ke jaringan WiFi
void hubungkanWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi berhasil terhubung!");
}

// Fungsi untuk menghubungkan ke broker MQTT dan
// melakukan subscribe
void hubungkanMQTT() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke broker MQTT...");
    String clientId = "ESP8266Client-" +
String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("berhasil!");
      client.subscribe(topicPerintah);
      Serial.println("Terhubung dan subscribe topic
perintah");
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
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  dht.begin();
  hubungkanWiFi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void loop() {
  // Menjaga koneksi MQTT tetap aktif
  if (!client.connected()) {
    hubungkanMQTT();
  }

  client.loop(); // Wajib dipanggil terus-menerus agar
  // pesan subscribe responsif

  // Proses publish data sensor secara berkala
  // menggunakan non-blocking millis()
  if (millis() - waktuTerakhirPublish >
  intervalPublish) {
    waktuTerakhirPublish = millis();

    float suhu = dht.readTemperature();
    if (!isnan(suhu)) {
      JsonDocument doc;
      doc["suhu"] = suhu;
      char buffer[128];
      serializeJson(doc, buffer);
      client.publish(topicData, buffer);
      Serial.print("Data terkirim: ");
      Serial.println(buffer);
    }
  }
}
```

### Penjelasan Program

1. **Library WiFi**  
   Digunakan agar ESP8266 dapat terhubung ke jaringan WiFi.

2. **Library PubSubClient**  
   Digunakan untuk melakukan komunikasi menggunakan protokol MQTT.

3. **Library ArduinoJson**  
   Digunakan untuk melakukan deserialisasi data JSON yang diterima.

4. **Fungsi callback**  
   Fungsi ini dijalankan ketika terdapat pesan baru pada topic yang telah di-subscribe.

5. **client.subscribe()**  
   Digunakan untuk mendaftarkan ESP8266 agar menerima pesan dari topic tertentu.

6. **deserializeJson()**  
   Digunakan untuk mengubah pesan JSON yang diterima menjadi data yang dapat dibaca program.

7. **client.loop()**  
   Digunakan agar ESP8266 terus memeriksa pesan MQTT yang masuk dan menjaga koneksi dengan broker.

### Alur Percobaan

**Flowchart:**

<img width="438" height="1624" alt="Untitled Diagram drawio (5)" src="https://github.com/user-attachments/assets/c6f55ca4-270d-4cb4-8c75-f833aced414e" />

### Hasil Pengamatan

**Tabel 4.1 Hasil Penerimaan dan Pemrosesan Perintah MQTT**

| No. | Perintah JSON | Pesan Diterima | Hasil Parsing | Status LED | Keterangan |
|---|---|---|---|---|---|
| 1 | `{"perintah":"ON"}` | Perintah diterima -> Aktuator: ON | ON | Menyala | LED berubah menjadi menyala setelah menerima perintah ON |
| 2 | `{"perintah":"OFF"}` | Perintah diterima -> Aktuator: OFF | OFF | Mati | LED berubah menjadi menyala setelah menerima perintah OFF |
| 3 | `{"perintah":"ON"}` | Perintah diterima -> Aktuator: ON | ON | Menyala | LED berubah menjadi menyala setelah menerima perintah ON |
| 4 | `{"perintah":"OFF"}` | Perintah diterima -> Aktuator: OFF | OFF | Mati | LED berubah menjadi menyala setelah menerima perintah OFF |
| 5 | `{"perintah":"ON"}` | Perintah diterima -> Aktuator: ON | ON | Menyala | LED berubah menjadi menyala setelah menerima perintah ON |

### Analisis

Berdasarkan hasil pengamatan, ESP8266 berhasil terhubung ke jaringan WiFi dan broker MQTT. Perangkat kemudian melakukan subscribe pada topic perintah untuk menerima pesan dari MQTT. Pesan yang diterima dalam format JSON dapat diproses, kemudian nilai perintah digunakan untuk mengendalikan LED sebagai aktuator.

Ketika perintah “ON” diterima, LED menyala, sedangkan ketika perintah “OFF” diterima, LED mati. Hasil perintah juga ditampilkan pada Serial Monitor sehingga proses komunikasi dapat diamati. Hal ini menunjukkan bahwa komunikasi subscribe MQTT dan pengendalian aktuator melalui JSON dapat berjalan sesuai fungsi yang dirancang.

### Output di serial monitor

#### Saat ON
<img width="956" height="498" alt="Screenshot 2026-09-29 133640" src="https://github.com/user-attachments/assets/0b6055dc-6d18-4f74-aa1f-81cb9206d585" />

#### Saat OFF
<img width="959" height="504" alt="Screenshot 2026-09-29 133828" src="https://github.com/user-attachments/assets/9196fd6b-8f74-4d29-9099-48225341370f" />

#### Led menyala saat ON
<img width="1369" height="738" alt="WhatsApp Image 2026-09-29 at 14 24 42" src="https://github.com/user-attachments/assets/16af92cf-44d9-4532-ad0f-01ac0deb939b" />

#### Led mati saat OFF
<img width="1445" height="722" alt="WhatsApp Image 2026-09-29 at 14 25 13" src="https://github.com/user-attachments/assets/61b8e1d1-a7bb-4aaa-a0a3-b84fb7d36887" />

---

## D. PERCOBAAN 4B: PERTUKARAN DATA DUA ARAH (PUBLISH DAN SUBSCRIBE SECARA BERSAMAAN)

### Tujuan

Pada percobaan 4B, saya bertujuan untuk mengimplementasikan komunikasi dua arah pada sistem IoT, yaitu ESP8266 melakukan publish data suhu dari sensor DHT11 dan pada saat yang sama melakukan subscribe perintah MQTT untuk mengendalikan LED.

Percobaan ini merupakan integrasi antara proses publish data sensor dan subscribe perintah aktuator. Data sensor seharusnya dikirim secara berkala setiap 5 detik menggunakan pendekatan non-blocking dengan millis().

### Rangkaian
<img width="1280" height="694" alt="Rangkaian percobaan 2" src="https://github.com/user-attachments/assets/779aef04-9c3f-47c3-b6a3-5cdbba308462" />

### Program

```cpp
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

// Konfigurasi WiFi
const char* ssid = "hmmm";
const char* password = "apaiyasih";

// Konfigurasi Broker MQTT
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

// Definisi Topic MQTT (Ganti "kelompokAnda" dengan
// nama kelompok Anda)
const char* topicData =
"unsoed/tk245004/kelompokAnda/data";
const char* topicPerintah =
"unsoed/tk245004/kelompokAnda/perintah";

// Konfigurasi Pin Hardware
#define DHTPIN D4
#define DHTTYPE DHT11
const int ledPin = D2; // Menggunakan pin D2 pada
// ESP8266

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long waktuTerakhirPublish = 0;
const long intervalPublish = 5000; // publish data
// setiap 5 detik (non-blocking)

void callback(char* topic, byte* payload, unsigned int
length) {
  String pesan = "";
  for (unsigned int i = 0; i < length; i++) {
    pesan += (char)payload[i];
  }

  JsonDocument doc;
  if (deserializeJson(doc, pesan)) return; // abaikan
  // jika parsing gagal

  const char* perintah = doc["perintah"];
  digitalWrite(ledPin, String(perintah) == "ON" ? HIGH
: LOW);
  Serial.print("Perintah diterima -> Aktuator: ");
  Serial.println(perintah);
}

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
    String clientId = "ESP8266Client-" +
String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      client.subscribe(topicPerintah);
      Serial.println("berhasil! Terhubung dan subscribe
topic perintah");
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
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  dht.begin();
  hubungkanWiFi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    hubungkanMQTT();
  }

  client.loop(); // memproses pesan masuk secara
  // terus-menerus

  // Publish data sensor secara berkala tanpa memblokir
  // proses subscribe
  if (millis() - waktuTerakhirPublish >
  intervalPublish) {
    waktuTerakhirPublish = millis();

    float suhu = dht.readTemperature();
    if (!isnan(suhu)) {
      JsonDocument doc;
      doc["suhu"] = suhu;
      char buffer[128];
      serializeJson(doc, buffer);
      client.publish(topicData, buffer);
      Serial.print("Data terkirim: ");
      Serial.println(buffer);
    }
  }
}
```

### Penjelasan Program

1. **Library DHT**  
   Digunakan untuk membaca data dari sensor DHT11.

2. **DHTPIN dan DHTTYPE**  
   Digunakan untuk menentukan pin dan jenis sensor yang digunakan.

3. **dht.begin()**  
   Digunakan untuk menginisialisasi sensor DHT11.

4. **client.loop()**  
   Digunakan untuk memproses pesan MQTT yang masuk secara terus-menerus.

5. **millis()**  
   Digunakan untuk mengatur waktu publish setiap 5 detik tanpa menggunakan delay() yang dapat menghambat proses komunikasi MQTT.

6. **dht.readTemperature()**  
   Digunakan untuk membaca nilai suhu dari sensor DHT11.

7. **isnan(suhu)**  
   Digunakan untuk memeriksa apakah hasil pembacaan suhu valid atau tidak.

8. **client.publish()**  
   Digunakan untuk mengirim data suhu dalam format JSON ke topic MQTT.

### Alur Percobaan

**Flowchart:**

<img width="627" height="1371" alt="Untitled Diagram drawio (6)" src="https://github.com/user-attachments/assets/09ce254d-6e15-4c4f-93b8-a7eebb854a79" />

### Hasil Pengamatan

**Tabel 4.2. Hasil Pertukaran Data Dua Arah**

| No | Waktu Pengiriman (s) | Data Suhu Dipublish | Perintah Diterima | Status LED | Data Diterima Subscriber | Keterangan |
|---|---:|---|---|---|---|---|
| 1 | 0 | `{"suhu":28.3}` | - | Mati | `{"suhu":28.3}` | Data suhu berhasil dipublish dan diterima subscriber |
| 2 | 5 | `{"suhu":28.2}` | - | Mati | `{"suhu":28.2}` | Data suhu berhasil dipublish |
| 3 | 10 | `{"suhu":28.2}` | - | Mati | `{"suhu":28.2}` | Data suhu berhasil dipublish |
| 4 | 15 | `{"suhu":28.2}` | - | Mati | `{"suhu":28.2}` | Data suhu berhasil dipublish |
| 5 | 20 | `{"suhu":28.2}` | ON | Menyala | `{"suhu":28.2}` | LED merespons perintah ON |
| 6 | 25 | `{"suhu":28.2}` | - | Menyala | `{"suhu":28.2}` | Data suhu berhasil dipublish |
| 7 | 30 | `{"suhu":28.2}` | - | Menyala | `{"suhu":28.2}` | Data suhu berhasil dipublish |
| 8 | 35 | `{"suhu":28.2}` | - | Menyala | `{"suhu":28.2}` | Data suhu berhasil dipublish |
| 9 | 40 | `{"suhu":28.2}` | OFF | Mati | `{"suhu":28.2}` | LED merespons perintah OFF |
| 10 | 45 | `{"suhu":28.2}` | - | Menyala | `{"suhu":28.2}` | Data suhu berhasil dipublish |

Berdasarkan hasil pengamatan, komunikasi dua arah menggunakan MQTT berhasil dilakukan. ESP8266 dapat mempublish data suhu dari DHT11 setiap 5 detik dalam format JSON, dengan suhu yang terbaca sekitar 28,2–28,3°C. Data tersebut juga berhasil diterima oleh subscriber.

Selain mengirim data suhu, ESP8266 juga dapat menerima perintah MQTT secara bersamaan. Perintah ON berhasil membuat LED menyala, sedangkan perintah OFF membuat LED mati. Hal ini menunjukkan bahwa proses publish dan subscribe dapat berjalan secara bersamaan tanpa mengganggu pengiriman data sensor.

Penggunaan millis() juga membuat proses publish berjalan secara non-blocking, sehingga ESP8266 tetap dapat memproses perintah MQTT dengan responsif. Dengan demikian, percobaan 4B menunjukkan bahwa komunikasi MQTT dua arah antara sensor, broker, subscriber, dan aktuator dapat berjalan dengan baik.

#### Perintah OFF
<img width="1280" height="719" alt="Percobaan 2 perintah off" src="https://github.com/user-attachments/assets/c33cb003-dc23-49c5-843d-e264b0b39188" />

#### Perintah ON
<img width="1280" height="715" alt="Percobaan 2 perintah on" src="https://github.com/user-attachments/assets/672c9293-d039-458b-85d4-d3819211376f" />

#### Hasil di serial monitor
<img width="856" height="637" alt="Percobaan 2 serial monitor" src="https://github.com/user-attachments/assets/9b51b430-b835-46e0-8998-fb6a79a3097c" />

---
## E. PERTANYAAN PRAKTIKUM PERCOBAAN 3A: KOMUNIKASI DATA MENGGUNAKAN HTTP

### 1. Gambarkan diagram alur (flowchart) proses penerimaan dan pemrosesan pesan pada fungsi callback di atas!

**Jawaban:**

<img width="560" height="1278" alt="Untitled Diagram drawio (7)" src="https://github.com/user-attachments/assets/ddade906-a702-49cd-8eb7-1a5ef39377cc" />

### 2. Apa yang akan terjadi apabila pesan yang dipublikasikan bukan merupakan format JSON yang valid?

**Jawaban:**

Apabila pesan yang diterima bukan JSON yang valid, proses `deserializeJson()` akan menghasilkan error. Program kemudian menjalankan:

```cpp
if (error) {
  return;
}
```

Artinya, pesan tersebut diabaikan dan fungsi `callback()` langsung berhenti.

LED tidak akan dikendalikan berdasarkan pesan tersebut karena nilai perintah tidak berhasil diambil.

### 3. Jelaskan mengapa fungsi client.subscribe() dipanggil di dalam fungsi hubungkanMQTT(), bukan di dalam setup()!

**Jawaban:**

`client.subscribe()` diletakkan di dalam `hubungkanMQTT()` karena fungsi tersebut digunakan untuk menghubungkan kembali ESP8266 ke broker MQTT ketika koneksi terputus.

Saat koneksi berhasil dibuat, program langsung melakukan:

```cpp
client.subscribe(topicPerintah);
```

Dengan begitu, setiap kali ESP8266 berhasil melakukan koneksi atau reconnect, perangkat akan kembali melakukan subscribe ke topic perintah. Jika hanya diletakkan di `setup()`, proses subscribe tidak otomatis dilakukan kembali setelah koneksi MQTT terputus dan tersambung ulang.

### 4. Modifikasi program agar data JSON yang diterima juga memuat nilai intensitas (misalnya {"perintah": "ON", "intensitas": 200}) yang digunakan untuk mengatur kecerahan LED menggunakan PWM (analogWrite/ledcWrite), dan berikan penjelasan di setiap baris kode yang ditambahkan dalam bentuk README.md

**Jawaban:**

Contoh pesan JSON yang digunakan:

```json
{"perintah":"ON","intensitas":200}
```

Pada ESP8266, bagian `callback()` dapat dimodifikasi menjadi:

```cpp
void callback(char* topic, byte* payload, unsigned
int length) {
  String pesan = "";
  for (unsigned int i = 0; i < length; i++) {
    pesan += (char)payload[i];
  }

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc,
pesan);
  if (error) {
    return;
  }

  const char* perintah = doc["perintah"];
  int intensitas = doc["intensitas"] | 0;

  if (String(perintah) == "ON") {
    analogWrite(ledPin, intensitas);
  } else {
    analogWrite(ledPin, 0);
  }

  Serial.print("Perintah diterima -> Aktuator: ");
  Serial.print(perintah);
  Serial.print(", Intensitas: ");
  Serial.println(intensitas);
}
```
---
## F. PERTANYAAN PRAKTIKUM PERCOBAAN 3B: KOMUNIKASI MQTT

### 1. Apa fungsi dari topic pada protokol MQTT, dan mengapa topic yang digunakan perlu dibuat unik?

**Jawaban:**

Penggunaan delay() yang lama sebaiknya dihindari karena dapat menghentikan sementara eksekusi program. Hal ini menyebabkan ESP8266 tidak dapat segera memproses pesan MQTT yang masuk sehingga respons terhadap perintah dari subscriber menjadi terlambat. Pada komunikasi dua arah, perangkat perlu tetap melakukan publish data sensor sekaligus menerima perintah secara responsif.

### 2. Jelaskan cara kerja mekanisme non-blocking menggunakan fungsi millis() pada program di atas!

**Jawaban:**

Mekanisme non-blocking menggunakan millis() dilakukan dengan membandingkan waktu saat ini dengan waktu terakhir data dipublish. Jika selisih waktu sudah mencapai interval yang ditentukan, yaitu 5 detik, ESP8266 akan membaca sensor dan mengirimkan data melalui MQTT. Selama menunggu interval tersebut, program tetap menjalankan `client.loop()` sehingga perangkat masih dapat menerima dan memproses perintah MQTT tanpa harus berhenti menunggu.

### 3. Apa yang akan terjadi apabila fungsi client.loop() jarang dipanggil (misalnya hanya sekali setiap 10 detik)?

**Jawaban:**

Apabila `client.loop()` jarang dipanggil, pesan MQTT yang masuk tidak dapat diproses dengan cepat. Akibatnya, respons terhadap perintah seperti ON atau OFF pada LED menjadi terlambat. Komunikasi subscribe juga menjadi kurang responsif karena fungsi `client.loop()` berperan dalam menjaga komunikasi MQTT tetap aktif dan memproses pesan yang diterima.

### 4. Modifikasi program agar menambahkan satu topic perintah baru untuk mengendalikan aktuator kedua (misalnya buzzer), dengan fungsi callback yang dapat membedakan topic mana yang menerima pesan, dan berikan penjelasan di setiap baris kode yang ditambahkan dalam bentuk README.md!

**Jawaban:**

```cpp
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

// Konfigurasi WiFi
const char* ssid = "hmmm";
const char* password = "apaiyasih";

// Konfigurasi Broker MQTT
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

// Topic MQTT
const char* topicData =
"unsoed/tk245004/kelompokAnda/data";
const char* topicPerintah =
"unsoed/tk245004/kelompokAnda/perintah";
const char* topicBuzzer =
"unsoed/tk245004/kelompokAnda/buzzer";

// Konfigurasi Pin
#define DHTPIN D4
#define DHTTYPE DHT11
const int ledPin = 26;
const int buzzerPin = D5;

// Inisialisasi sensor dan MQTT
DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

// Variabel waktu
unsigned long waktuTerakhirPublish = 0;
const long intervalPublish = 5000;

// Fungsi callback untuk menerima pesan MQTT
void callback(char* topic, byte* payload, unsigned int
length) {
  String pesan = "";

  // Membaca isi pesan
  for (unsigned int i = 0; i < length; i++) {
    pesan += (char)payload[i];
  }

  // Parsing JSON
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc,
pesan);
  if (error) {
    return;
  }

  // Mengambil nilai perintah
  const char* perintah = doc["perintah"];

  // Jika pesan berasal dari topic LED
  if (String(topic) == topicPerintah) {
    digitalWrite(
      ledPin,
      String(perintah) == "ON" ? HIGH : LOW
    );

    Serial.print("Perintah LED: ");
    Serial.println(perintah);
  }

  // Jika pesan berasal dari topic buzzer
  else if (String(topic) == topicBuzzer) {
    digitalWrite(
      buzzerPin,
      String(perintah) == "ON" ? HIGH : LOW
    );

    Serial.print("Perintah Buzzer: ");
    Serial.println(perintah);
  }
}

// Fungsi menghubungkan WiFi
void hubungkanWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi berhasil terhubung!");
}

// Fungsi menghubungkan MQTT
void hubungkanMQTT() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke broker MQTT...");
    String clientId =
    "ESP8266Client-" + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("berhasil!");

      // Subscribe topic LED
      client.subscribe(topicPerintah);

      // Subscribe topic buzzer
      client.subscribe(topicBuzzer);

      Serial.println("Subscribe topic LED dan buzzer
berhasil");
    } else {
      Serial.print("gagal, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi dalam 2 detik");
      delay(2000);
    }
  }
}

// Fungsi setup
void setup() {
  Serial.begin(115200);

  // Konfigurasi LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Konfigurasi buzzer
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  // Inisialisasi DHT11
  dht.begin();

  // Hubungkan WiFi
  hubungkanWiFi();

  // Konfigurasi MQTT
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

// Fungsi loop
void loop() {
  // Memastikan MQTT tetap terhubung
  if (!client.connected()) {
    hubungkanMQTT();
  }

  // Memproses pesan MQTT
  client.loop();

  // Publish data suhu setiap 5 detik
  if (millis() - waktuTerakhirPublish >
  intervalPublish) {
    waktuTerakhirPublish = millis();

    float suhu = dht.readTemperature();
    if (!isnan(suhu)) {
      JsonDocument doc;
      doc["suhu"] = suhu;
      char buffer[128];
      serializeJson(doc, buffer);
      client.publish(topicData, buffer);
      Serial.print("Data terkirim: ");
      Serial.println(buffer);
    }
  }
}
```

---

## G. PERTANYAAN ANALISIS

### 1. Uraikan hasil tugas pada praktikum yang telah dilakukan pada setiap percobaan!

**Jawaban:**

Pada Percobaan 4A, ESP8266 berhasil terhubung ke WiFi dan broker MQTT serta melakukan subscribe pada topic perintah. Data JSON yang diterima berhasil diparsing untuk mengambil nilai perintah. Perintah ON dapat menyalakan LED, sedangkan perintah OFF dapat mematikan LED.

Pada Percobaan 4B, komunikasi dua arah berhasil dilakukan. ESP8266 dapat membaca suhu dari sensor DHT11 dan mempublish data suhu dalam format JSON secara berkala. Pada saat yang sama, ESP8266 juga dapat menerima perintah melalui MQTT untuk mengendalikan LED. Penggunaan millis() membuat proses pengiriman data tidak menghambat proses penerimaan pesan MQTT.

### 2. Bandingkan mekanisme komunikasi satu arah (publish saja, seperti pada Modul Praktikum 3) dengan komunikasi dua arah (publish dan subscribe) yang diimplementasikan pada modul ini!

**Jawaban:**

Komunikasi satu arah hanya melakukan proses publish, sehingga perangkat bertugas mengirimkan data ke broker atau subscriber tanpa menerima perintah balik. Contohnya adalah pengiriman data sensor pada Modul Praktikum 3.

Sementara itu, komunikasi dua arah pada modul ini memungkinkan perangkat melakukan publish data sekaligus subscribe dan menerima perintah dari MQTT. Dengan demikian, perangkat tidak hanya mengirim informasi, tetapi juga dapat menerima perintah untuk mengendalikan aktuator seperti LED.

### 3. Mengapa pendekatan non-blocking (menggunakan millis()) lebih sesuai dibandingkan pendekatan blocking (menggunakan delay()) pada sistem IoT yang memerlukan komunikasi dua arah secara real-time?

**Jawaban:**

Pendekatan non-blocking menggunakan millis() lebih sesuai karena program tidak perlu berhenti selama menunggu waktu pengiriman data. Selama menunggu interval publish, fungsi `client.loop()` tetap dapat dijalankan untuk memproses pesan MQTT yang masuk. Sebaliknya, delay() dapat menghentikan eksekusi program sementara sehingga penerimaan dan pemrosesan perintah dapat menjadi terlambat.

### 4. Berikan contoh penerapan komunikasi dan pertukaran data dua arah pada sistem IoT nyata (sesuaikan dengan bidang peminatan masing-masing mahasiswa), dan jelaskan manfaatnya dibandingkan sistem yang hanya satu arah!

**Jawaban:**

Salah satu contohnya pada sistem smart farming. Sensor dapat mengirimkan data seperti suhu dan kelembapan tanah ke server melalui MQTT. Sebaliknya, server dapat mengirimkan perintah kembali ke ESP32 untuk mengaktifkan atau mematikan pompa air berdasarkan kondisi tanaman.

Dibandingkan sistem satu arah yang hanya mengirimkan data sensor, komunikasi dua arah memungkinkan sistem tidak hanya memantau kondisi, tetapi juga memberikan tindakan atau kontrol secara langsung. Hal ini membuat sistem IoT lebih interaktif dan dapat merespons kondisi lingkungan dengan lebih cepat.

---

## H. KESIMPULAN

Berdasarkan praktikum yang telah dilakukan, komunikasi dan pertukaran data menggunakan protokol MQTT pada ESP8266 berhasil diterapkan melalui mekanisme publish dan subscribe. ESP8266 dapat menerima data JSON, melakukan proses parsing, serta mengendalikan LED berdasarkan perintah ON dan OFF. Selain itu, ESP8266 juga dapat mengirimkan data suhu dari sensor DHT11 secara berkala kepada subscriber.

Penggunaan metode non-blocking dengan millis() membuat proses pengiriman data tidak menghambat penerimaan pesan MQTT sehingga komunikasi dapat berjalan lebih responsif. Penambahan topic baru juga menunjukkan bahwa MQTT dapat digunakan untuk mengendalikan lebih dari satu aktuator, seperti LED dan buzzer, melalui topic yang berbeda.

