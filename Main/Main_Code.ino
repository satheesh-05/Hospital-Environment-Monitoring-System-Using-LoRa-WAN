#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <DHT.h>

#define DHTPIN 26      // Pin for DHT11 sensor
#define DHTTYPE DHT11  // Type of DHT sensor
#define MQ135_PIN 27   // Analog pin for MQ135 sensor
#define LED 2          // LED pin for alerts

DHT dht(DHTPIN, DHTTYPE);

// LoRaWAN session keys (replace with your keys)
static u1_t NWKSKEY[16] = { 0xE3, 0x7C, 0x71, 0x48, 0xC2, 0x67, 0xFA, 0x3A, 0x4D, 0x3A, 0x2E, 0xB9, 0xE4, 0xF1, 0x4B, 0xE9 };
static u1_t APPSKEY[16] = { 0x02, 0xE5, 0xBD, 0x32, 0x9A, 0x96, 0x5F, 0x3A, 0xD0, 0x25, 0x81, 0x6E, 0x3F, 0x06, 0x46, 0x09 };
static const u4_t DEVADDR = 0x260B69C1;

const lmic_pinmap lmic_pins = {
    .nss = 15,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 17,
    .dio = {4, 33, 32}
};

uint8_t payload[8];
static osjob_t sendjob;
const unsigned TX_INTERVAL = 60; // Transmission interval in seconds

// Thresholds for gas levels (adjust these based on your environment)
const int SO2_THRESHOLD = 100;  // threshold for SO2 in ppb
const int NO2_THRESHOLD = 50;   // threshold for NO2 in ppb

void os_getArtEui(u1_t* buf) { memcpy(buf, NWKSKEY, 16); }
void os_getDevEui(u1_t* buf) { memcpy(buf, APPSKEY, 16); }
void os_getDevKey(u1_t* buf) { memcpy(buf, NWKSKEY, 16); }

// Function to scale MQ135 reading to desired SO2 and NO2 ranges
void readMQ135(int analogValue, int &so2, int &no2) {
    // Convert MQ135 analog reading to SO2 and NO2 levels

    // Scale MQ135 raw value (0-1023) to SO2 (1-100 ppb) and NO2 (1-50 ppb)
    // SO2 range: 1-100 ppb, NO2 range: 1-50 ppb
    so2 = map(analogValue, 0, 1023, 1, 100);  // Scale to SO2 (1–100 ppb)
    no2 = map(analogValue, 0, 1023, 1, 50);   // Scale to NO2 (1–50 ppb)
}

void readSensors() {
    // Read temperature and humidity from DHT11
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Read raw analog value from MQ135 sensor
    int mq135Value = analogRead(MQ135_PIN);

    // Convert MQ135 reading to SO2 and NO2 levels
    int so2, no2;
    readMQ135(mq135Value, so2, no2);

    // Prepare payload
    int tempInt = (int)(temperature * 100);
    payload[0] = (tempInt >> 8) & 0xFF;
    payload[1] = tempInt & 0xFF;

    int humInt = (int)(humidity * 100);
    payload[2] = (humInt >> 8) & 0xFF;
    payload[3] = humInt & 0xFF;

    payload[4] = (so2 >> 8) & 0xFF;
    payload[5] = so2 & 0xFF;

    payload[6] = (no2 >> 8) & 0xFF;
    payload[7] = no2 & 0xFF;

    // Control LED based on thresholds
    if (so2 > SO2_THRESHOLD || no2 > NO2_THRESHOLD) {
        digitalWrite(LED, HIGH);
        Serial.println("Warning: Gas levels exceeded threshold!");
    } else {
        digitalWrite(LED, LOW);
    }

    // Print values for debugging
    Serial.print("Temperature: "); Serial.print(temperature); Serial.print(" °C, ");
    Serial.print("Humidity: "); Serial.print(humidity); Serial.print(" %, ");
    Serial.print("SO2: "); Serial.print(so2); Serial.print(" ppb, ");
    Serial.print("NO2: "); Serial.println(no2);
}

void do_send(osjob_t* j) {
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        readSensors();
        LMIC_setTxData2(1, payload, sizeof(payload), 0);
        Serial.println(F("Packet queued"));
    }
}

void onEvent(ev_t ev) {
    switch (ev) {
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
            break;
        default:
            break;
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(MQ135_PIN, INPUT);
    pinMode(LED, OUTPUT);
    dht.begin();

    os_init();
    LMIC_reset();
    LMIC_setSession(0x1, DEVADDR, NWKSKEY, APPSKEY);
    do_send(&sendjob);
}

void loop() {
    os_runloop_once();
}
