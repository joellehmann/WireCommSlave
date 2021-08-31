// WireCommSlave between two ESP8266 ###################################
// Masterthesis V 0.1.0 ################################################
// by Joel Lehmann #####################################################
// 30.08.2021 ##########################################################

#include <Arduino.h>
#include <Wire.h>
#include "BlueDot_BME280.h"

#define SDA_PIN D5
#define SCL_PIN D6
const int16_t I2C_MASTER = 0x42;
const int16_t I2C_SLAVE = 0x08;
char request;
String req;
int cnt = 0;
char json[120];
bool bPrintMeta;
bool bPrintMes;

BlueDot_BME280 bme1;
int bme1Detected = 0;   

//######################################################################
// Wire On Request #####################################################
//######################################################################

void requestEvent() {
  
  if (req == "reqMetaCount")
  {
    Wire.write("5\n");
    Serial.println("Sent Metadata: 5");
    bPrintMeta = true;
  }else if (req == "0" && bPrintMeta)
  {
  Wire.write("DEVID:BME280EXT1\0");  
  }else if (req == "1" && bPrintMeta){
  Wire.write("SENSOR:BME280\0");
  }else if (req == "2" && bPrintMeta){
  Wire.write("DATE:08-30-2021\0");
  }else if (req == "3" && bPrintMeta){
  Wire.write("EDIT:Lehmann, Joel\0");
  }else if (req == "4" && bPrintMeta){
  Wire.write("MCU:ESP8266\0");
  bPrintMeta = false;
  }else if (req == "reqMesCount"){
  Wire.write("3\n");
  bPrintMes = true;
  }else if (req == "0" && bPrintMes)
  {
  Wire.write("Temperature:°C\0");
  }else if (req == "1" && bPrintMes)
  {
  Wire.write("Humidity:%\0");
  }else if (req == "2" && bPrintMes)
  {
  Wire.write("Pressure:hPa\0");
  bPrintMes = false;
  }else if (req == "reqTemperature")
  {
  Wire.write((String(bme1.readTempC())+"\0").c_str());
  }else if (req == "reqHumidity")
  {
  Wire.write((String(bme1.readHumidity())+"\0").c_str());
  }else if (req == "reqPressure")
  {
  Wire.write((String(bme1.readPressure())+"\0").c_str());
  }
  req = "";
  
}

//######################################################################
// Wire On Receive #####################################################
//######################################################################

void receiveEvent(size_t howMany) {
  Serial.println("Received something");
  (void) howMany;
  while (1 < Wire.available()) 
  {
    request = Wire.read();
    
    req = req + request;
  }
    Serial.println(req);
}

//######################################################################
// Setup ###############################################################
//######################################################################

void setup() {
  Serial.begin(9600);
  Wire.begin(SDA_PIN, SCL_PIN, I2C_SLAVE);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  bme1.parameter.communication = 1;
  bme1.parameter.SPI_cs = D3;        
  bme1.parameter.SPI_mosi = D2;                       
  bme1.parameter.SPI_miso = D4;                       
  bme1.parameter.SPI_sck = D1; 
  bme1.parameter.I2CAddress = 0x77;
  bme1.parameter.sensorMode = 0b11; 
  bme1.parameter.IIRfilter = 0b100;
  bme1.parameter.humidOversampling = 0b101;
  bme1.parameter.tempOversampling = 0b101;
  bme1.parameter.pressOversampling = 0b101;
  bme1.parameter.tempOutsideCelsius = 15; 
  bme1.parameter.tempOutsideFahrenheit = 59;

    if (bme1.init() != 0x60)
  {    
    Serial.println(F("Ops! First BME280 Sensor not found!"));
    bme1Detected = 0;
  }

  else
  {
    Serial.println(F("First BME280 Sensor detected!"));
    bme1Detected = 1;
  }

  
}

void loop() {}

