The Hospital Environment Monitoring System is designed to enhance the operational efficiency of healthcare facilities by providing a robust platform for real-time monitoring of critical environmental parameters. The objectives outlined for this project are pivotal for ensuring optimal conditions within hospitals, thereby contributing to patient safety and comfort. The primary objectives include:

<p align="center">
  <img src="https://github.com/user-attachments/assets/0b72fc99-eb52-4c69-beaa-f7dabd016048" width="500" height="500" alt="image" />
</p>



**Components Used:**
MQ135 Gas Sensor, 
DHT11, 
LCD 16x2 Display Module, 
ESP32 development board (LoRa Based), 
Lora gateway.

**Description**
**1. Sensor Data Acquisition:**

The process begins with the strategic placement of sensors throughout various hospital zones. The MQ135 sensor is employed to monitor air quality by detecting harmful gases such as ammonia, sulfide, benzene vapors, smoke, and other toxic compounds, while the DHT11 sensor measures temperature and humidity levels. These sensors continuously collect environmental data, ensuring that critical conditions within the hospital are consistently monitored. The deployment of these sensors is essential for establishing a baseline understanding of the hospital's environment, which is vital for patient safety and comfort.

**2. Data Processing:**

Once the sensors have gathered the necessary data, it is transmitted to the Wireless Development Module (WDM) powered by the ESP32 microcontroller. In this phase, the data undergoes a series of calibration and validation checks to ensure its accuracy and reliability. Calibration involves adjusting the sensor readings to account for any discrepancies or offsets, while validation ensures that the data meets predefined quality standards. This processing step is crucial, as it confirms that only reliable data is forwarded for transmission, thereby enhancing the overall integrity of the monitoring system.

**3. Data Transmission:**

After processing, validated data is delivered from the WDM to a LoRa gateway via the SX1276 LoRa module. This transmission phase is especially important due to LoRa technology's capacity to offer long-distance communication with low power usage. Using this technology, the device can efficiently transmit data over long distances, which is especially useful in hospital settings where physical barriers may prevent wireless transmission. The robustness of this transmission ensures that data is continually delivered without loss, allowing environmental monitoring to continue uninterrupted.


**Transmission Video Link:** https://drive.google.com/drive/folders/1OQrQlqHilM5IpAwVT6YGRo6CNFKdkajH?usp=drive_link
