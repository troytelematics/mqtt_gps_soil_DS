/* This sample code demonstrates the normal use of a TinyGPS object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 3(rx) and 4(tx).
*/
#include "config.h"
#include "definitions.h"
#include "comm.h"
#include "gps.h"
#include "moisture_sensor.h"
#include <HardwareSerial.h>
#include <SPI.h>



void setup()
{
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, GPS_RXPIN, GPS_TXPIN);
  setupConfig();  //Setup wifi info and other default info and store in spi file system
  getMacId();

  setup_wifi();
  setupMqtt();

  //Print the wakeup reason for ESP32
  print_wakeup_reason();
  // New instance of structure for gps info
  gps_info gps_data;

  //Get the GPS as it takes longer
  gps_data = getGpsReading();

  //Get the Soil Test and combine with gps to send
  getSoilTest(gps_data);

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
                 " Seconds");
  esp_deep_sleep_start();

}

//Nothing to loop thru because deep sleep
void loop()
{
}



