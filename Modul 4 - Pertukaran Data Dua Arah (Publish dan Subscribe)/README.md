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
