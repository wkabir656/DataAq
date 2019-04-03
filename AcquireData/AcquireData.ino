#include "FreqMeasureMulti.h"
#include "elapsedMillis.h"
#include "SparkFunLSM6DS3.h"
#include "i2c_t3.h"
#include "SPI.h"
#include <SD.h>
//Includes all necessary libraries

//Creates instances for frequency measurement class and IMU class
FreqMeasureMulti f1;
FreqMeasureMulti f2;
LSM6DS3 SensorOne( I2C_MODE, 0x6A );
LSM6DS3 SensorTwo( I2C_MODE, 0x6B );

//setting up global variables
String fileName;
String dataString;
boolean dataAq = true;
String state;
int numFiles = 1;
elapsedMillis timestamp;
int count;
int fileCloser = 0;

void setup() {
  //starts serial ports with baud rates, 1 second delay to give time for setup
  Serial.begin(9600);
  delay(1000);
  Serial.println("Processor came out of reset.\n");
  pinMode(13,OUTPUT);

  //sets initial values for data collection
  //Call .begin() to configure the IMUs, checks if IMUs are operational


  if ( SensorOne.begin() != 0 )
  {
    //Serial.println("Problem starting the IMU at 0x6A.");
  }
  else
  {
    //Serial.println("IMU at 0x6A started.");
  }
  if ( SensorTwo.begin() != 0 )
  {
    //Serial.println("Problem starting the IMU at 0x6B.");
  }
  else
  {
    //Serial.println("IMU at 0x6B started.");
  }

  //
  // see if the card is present and can be initialized:

  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }

  Serial.println("We good");
  fileNameCreator();

}

void loop() {
  /*
  count = count + 1;
  
    if (Serial1.available() > 0) { // Checks whether data is coming into serial port from bluetooth
    state = Serial1.readString(); // Reads the data from the serial port
    }
  
  if (state == "Start" && count == 1) //if "Start" string is given, calls for file creation and makes data collection condition true
  {
    fileNameCreator();
    dataAq = true;
    Serial.println("Starting data collection now.");

  }
  
  */
  if (int(timestamp/1000) == fileCloser)
  {
    dataFile.close();
    fileCloser = fileCloser + 60;
    File dataFile = SD.open(fileName.c_str(), FILE_WRITE);
  }
  digitalWrite(13,HIGH);  
  //opens CSV file for writing

  //read analog signals from LDS
  int readLDS0 = analogRead(2);
  float voltage0 = readLDS0;// * (3.3 / 1023.0);

  int readLDS1 = analogRead(3);
  float voltage1 = readLDS1;// * (3.3 / 1023.0);

  int readLDS2 = analogRead(4);
  float voltage2 = readLDS2;// * (3.3 / 1023.0);

  int readLDS3 = analogRead(5);
  float voltage3 = readLDS3;// * (3.3 / 1023.0);

  //read acceleration and gyroscope data from two IMUs connected via I2C
  float s1RawAccelX = SensorOne.readFloatAccelX();
  float s1RawAccelY = SensorOne.readFloatAccelY();
  float s1RawAccelZ = SensorOne.readFloatAccelZ();

  float s1RawGyroX = SensorOne.readFloatGyroX();
  float s1RawGyroY = SensorOne.readFloatGyroY();
  float s1RawGyroZ = SensorOne.readFloatGyroZ();

  float s2RawAccelX = SensorTwo.readFloatAccelX();
  float s2RawAccelY = SensorTwo.readFloatAccelY();
  float s2RawAccelZ = SensorTwo.readFloatAccelZ();

  float s2RawGyroX = SensorTwo.readFloatGyroX();
  float s2RawGyroY = SensorTwo.readFloatGyroY();
  float s2RawGyroZ = SensorTwo.readFloatGyroZ();

  //Reads RPM data from hall effect sensors
  float freq1 = f1.countToFrequency(f1.read());
  float freq2 = f2.countToFrequency(f2.read());


  //writes data to string
  dataString = String(timestamp / 1000.0000, 4) + "," + String(voltage0, 4) + "," + String(voltage1, 4) + "," + String(voltage2, 4) + "," + String(voltage3, 4) + "," +
               String(s1RawAccelX, 4) + "," + String(s1RawAccelY, 4) + "," + String(s1RawAccelZ, 4)  + "," + String(s1RawGyroX, 4) + "," + String(s1RawGyroY, 4) + "," + String(s1RawGyroZ, 4) +
               + "," + String(s2RawAccelX, 4) + "," + String(s2RawAccelY, 4) + "," + String(s2RawAccelZ, 4)  + "," + String(s2RawGyroX, 4) + "," + String(s2RawGyroY, 4) + "," + String(s2RawGyroZ, 4)
               + "," + String(freq1, 4) + "," + String(freq2, 4);

  //prints string of data to CSV file, closes CSV file
  //Serial.println(dataString);
  dataFile.println(dataString);
 






}
void fileNameCreator()
{
  //Initializes SD card, checks how many existing files are on the card, numbers file accordingly
  //Serial.println("card initialized.");

  File root = SD.open("/");
  while (File thisFile = root.openNextFile()) {
    if (!thisFile.isDirectory()) {
      numFiles++;
    }
  }


  //converts new file name string to char array
  fileName = (String) numFiles + ".csv";
  //Serial.println("Current filename: " + fileName);
  //charFileName[fileName.length() + 1];
  // fileName.toCharArray(charFileName, sizeof(charFileName));


  //opens new CSV file, prints out header in CSV file
  File dataFile = SD.open(fileName.c_str(), FILE_WRITE);
  if (dataFile)
  {
    //Serial.println("File opened.");
  }
  dataFile.println("Time,LDS1,LDS2,LDS3,LDS4,X1-Accel,Y1-Accel,Z1-Accel,X1-Gyro,Y1-Gyro,Z1-Gyro,X2-Accel,Y2-Accel,Z2-Accel,X2-Gyro,Y2-Gyro,Z2-Gyro,RPM1,RPM2");
  dataFile.close();

  //sets timer to 0
  timestamp = 0;
  File dataFile = SD.open(fileName.c_str(), FILE_WRITE);
}
