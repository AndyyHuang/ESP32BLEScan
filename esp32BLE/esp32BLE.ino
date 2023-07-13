#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

int scanTime = 10; //In seconds
BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      std::string time = getTime();
      if (advertisedDevice.haveManufacturerData() && isApple(advertisedDevice) && advertisedDevice.haveRSSI()) {
        std::string payload = byteArrayToHexString(advertisedDevice.getPayload(), advertisedDevice.getPayloadLength());
        int rssi = advertisedDevice.getRSSI();
        std::string mac = advertisedDevice.getAddress().toString();
        Serial.printf("~%s\t%s\t%i\t%s\n", mac.c_str(), time.c_str(), rssi, payload.c_str());
      }
    }

// Time resets upon boot.
     std::string getTime() {
       int elapsed = millis();
       int milliseconds = elapsed % 1000;
       int seconds = (int) floor(elapsed / 1000.0) % 60;
       int minutes = (int) (elapsed / 60000.0) % 60;
       int hours = (int) (elapsed / 3600000.0);
       std::ostringstream oss;
       oss << std::setfill('0');
       oss << std::setw(2) << hours << ":";
       oss << std::setw(2) << minutes << ":";
       oss << std::setw(2) << seconds << ":";
       oss << std::setw(3) << milliseconds;
       return oss.str();
     }

// Continues to display elapsed time even after reboot.
//    std::string getTime() {
//      // Get the current time with milliseconds
//      auto currentTime = std::chrono::system_clock::now();
//      auto currentTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(
//          currentTime.time_since_epoch()
//      ).count() % 1000;
//
//      // Convert the current time to a string format with milliseconds
//      std::time_t currentTimeSec = std::chrono::system_clock::to_time_t(currentTime);
//      std::tm* timeinfo = std::gmtime(&currentTimeSec);
//      std::stringstream ss;
//      ss << std::put_time(timeinfo, "%H:%M:%S");
//      ss << ":" << std::setfill('0') << std::setw(3) << currentTimeMs;
//      std::string timeString = ss.str();
//      return timeString;
//    }

    bool isApple(BLEAdvertisedDevice advertisedDevice) {
      std::string data = byteArrayToHexString((uint8_t*)advertisedDevice.getManufacturerData().data(), advertisedDevice.getManufacturerData().length());
      if (data.find("4c00") != std::string::npos) {
          return true;
        }
      return false;
    }

    std::string byteArrayToHexString(uint8_t* byteArray, size_t length) {
        char* data = BLEUtils::buildHexData(NULL, byteArray, length);
        if (data != NULL) {
          std::string copied_data = std::string(data);
          free(data);
          return copied_data;
        }
        return "ERROR CONVERTING BYTES TO STRING";
    }
};

void setup() {
  Serial.begin(115200);
  BLEDevice::init("");
  Serial.println("Scanning...");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
//  pBLEScan->setInterval(100);
//  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, true);
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
}
