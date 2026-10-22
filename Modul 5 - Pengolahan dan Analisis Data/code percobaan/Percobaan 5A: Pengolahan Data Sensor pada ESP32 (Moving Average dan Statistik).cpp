#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Configuration
const char* WIFI_SSID     = "hmmm"; 
const char* WIFI_PASSWORD = "apaiyasih";

const char* MQTT_SERVER   = "broker.hivemq.com";
const uint16_t MQTT_PORT  = 1883;
const char* MQTT_TOPIC    = "unsoed/tk245004/kelompokAnda/data";

const unsigned long PUBLISH_INTERVAL_MS = 5000;

// Global Instances
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastPublishTime = 0;

// Mock function simulator sensor DHT22
float bacaSuhuMock() {
    static unsigned long jumlahBaca = 0;
    jumlahBaca++;

    float suhuDasar = 28.0 + (2.0 * sin(jumlahBaca * 0.08));
    float noise     = random(-5, 6) * 0.1;
    float suhu      = suhuDasar + noise;

    // Simulasi anomali suhu setiap 25 kali pembacaan
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
        Serial.print("Attempting MQTT connection...");
        
        String clientId = "ESP8266Client-" + String(random(0xFFFF), HEX);

        if (client.connect(clientId.c_str())) {
            Serial.println("connected!");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" - try again in 5 seconds");
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

    // Kirim data berdasarkan interval non-blocking (millis)
    unsigned long currentMillis = millis();
    if (currentMillis - lastPublishTime >= PUBLISH_INTERVAL_MS) {
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
