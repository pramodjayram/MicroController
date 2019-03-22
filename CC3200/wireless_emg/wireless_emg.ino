#include <Wire.h>
#include <string.h>
#include <stdlib.h>
#include "Initialise.h"
#ifndef __CC3200R1M1RGC__
// Do not include SPI for CC3200 LaunchPad
#include <SPI.h>
#endif
#include <WiFi.h>
int wait_timer=0,wait_flag=0;
int first=0;
//// your network name also called SSID
char ssid[] = "NameofSSID";
//// your network password
char password[] = "password";
IPAddress ip,remoteip;
unsigned int localPort = 1001;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back
int port=0;
int port1=0;
WiFiUDP Udp;


WiFiServer server(1001);
void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  int i = 0;
  byte buff[6]; 
  initialconfig();
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  Serial.println(ssid); 
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    delay(1000);
    WiFi.begin(ssid, password);
  }

  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");

  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nIP Address obtained");
  printWifiStatus();
  Serial.println("\nWaiting for a connection from a client...");
  Udp.begin(localPort);
}

void loop() {

    result=analogRead(A1);
 
  getimuvalue();
roll1 = (atan2(acc_x,sqrt(sq(acc_y)+sq(acc_z))))*180/3.14;
pitch1 = (atan2(acc_y,sqrt(sq(acc_x)+sq(acc_z))))*180/3.14;
sprintf(roll_1,"%2.2f",roll1);
sprintf(pitch_1,"%2.2f",pitch1);
sprintf(accx,"%2.2f",acc_x);
sprintf(accy,"%2.2f",acc_y);
sprintf(accz,"%2.2f",acc_z);
sprintf(gyrx,"%2.2f",gyr_x);
sprintf(gyry,"%2.2f",gyr_y);
sprintf(gyrz,"%2.2f",gyr_z);
sprintf(str_emg,"%3.2f",result);
sprintf(str_sampling,"%5.2f",sampling_rate);
sprintf(str_roll,"%3.2f",roll1);

//Replace the ip adress with the broadcast ip of the network and change the port number to desired port
Udp.beginPacket("broadcastip", 3000);
    Udp.write(accx);
    Udp.write(",");
    Udp.write(accy);
    Udp.write(",");
    Udp.write(accz);
    Udp.write(",");
    Udp.write(gyrx);
    Udp.write(",");
    Udp.write(gyry);
    Udp.write(",");
    Udp.write(gyrz);
    Udp.write(",");
    Udp.write(str_emg);
    Udp.write(",");
    Udp.write(pitch_1);
    Udp.write(",");
    Udp.write(roll_1);
    Udp.write("\n");
    Udp.endPacket();

}

void getGyroValues () {
  byte MSB, LSB;
  MSB = read_gyro(0x29);
  LSB = read_gyro(0x28);
  gyr_x = ((MSB << 8) | LSB);
  MSB = read_gyro(0x2B);
  LSB = read_gyro(0x2A);
  gyr_y = ((MSB << 8) | LSB);
  MSB = read_gyro(0x2D);
  LSB = read_gyro(0x2C);
  gyr_z = ((MSB << 8) | LSB);
}
int read_gyro (byte regAddr) {
  Wire.beginTransmission(GYRO_ADD);
  Wire.write(regAddr);                
  Wire.endTransmission();          
  Wire.requestFrom(GYRO_ADD, 1); 
  while(!Wire.available()) 
  { 
  };      
  return(Wire.read());               
}




void getimuvalue(){
  byte buff[6];
  int i;
  Wire.beginTransmission(ACCEL_ADDRESS); 
  WIRE_SEND(0x32); 
  Wire.endTransmission();  
  Wire.beginTransmission(ACCEL_ADDRESS);
  Wire.requestFrom(ACCEL_ADDRESS, 6);
  
  while(Wire.available()) 
  { 
    buff[i++] = WIRE_RECEIVE();
  }
  Wire.endTransmission();  
  if (i == 6)  
  {
    acc_x = (((int16_t) buff[1]) << 8) | buff[0]; 
    acc_y = (((int16_t) buff[3]) << 8) | buff[2]; 
    acc_z = (((int16_t) buff[5]) << 8) | buff[4]; 
  }
  if(acc_x>60000)
  acc_x=acc_x-65536;
  if(acc_y>60000)
  acc_y=acc_y-65536;
  if(acc_z>60000)
  acc_z=acc_z-65536;
  acc_x=((acc_x-256.0)*(1.0+1.0)/(-256.0-256.0)-1.0);
  acc_y=((acc_y-247.0)*(1.0+1.0)/(-265.0-247.0)-1.0);
  acc_z=((acc_z-236.0)*(1.0+1.0)/(-260.0-236.0)-1.0);  
   gyr_yp=gyr_y;
    getGyroValues();
  if(gyr_x>35000)
    gyr_x=gyr_x-65536;
  if(gyr_y>35000)
    gyr_y=gyr_y-65536;
  if(gyr_z>35000)
    gyr_z=gyr_z-65536;
  gyr_x=gyr_x*0.00875;   
  gyr_y=gyr_y*0.00875;
  gyr_z=gyr_z*0.00875;    
  gyr_x=(gyr_x*alpha)+(1-alpha)*gyr_xo;  
  gyr_xo=gyr_x;
  gyr_y=(gyr_y*alpha)+(1-alpha)*gyr_yo;
  gyr_yo=gyr_y;
  gyr_z=(gyr_z*alpha)+(1-alpha)*gyr_zo;
  gyr_zo=gyr_z;  
}

void initialconfig(){
  int i;
  byte buff[6];
  Wire.begin();
  Serial.begin(9600);  
  Wire.beginTransmission(ACCEL_ADDRESS);
  WIRE_SEND(0x2D);
  WIRE_SEND(0x08); 
  Wire.endTransmission();
  Wire.beginTransmission(ACCEL_ADDRESS);
  WIRE_SEND(0x31);
  WIRE_SEND(0x0A);
  Wire.endTransmission();
  Wire.beginTransmission(ACCEL_ADDRESS);
  WIRE_SEND(0x2C);
  WIRE_SEND(0x09);
  Wire.endTransmission();
   Wire.beginTransmission(GYRO_ADD);
  Wire.write(0x20);
  Wire.write(0x1F);      
  Wire.endTransmission();
  Wire.beginTransmission(GYRO_ADD);
  Wire.write(0x22);
  Wire.write(0x08);      
  Wire.endTransmission();
  Wire.beginTransmission(GYRO_ADD);   
  Wire.write(0x23);          
  Wire.write(0x80);           
  Wire.endTransmission();
  Wire.beginTransmission(mag_address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();
  
  //delay(2000);
    while(time!=5)                       //Calculation of the initial mean pitch and roll
  { 
   Wire.beginTransmission(ACCEL_ADDRESS); 
   WIRE_SEND(0x32); 
   Wire.endTransmission();  
   Wire.beginTransmission(ACCEL_ADDRESS);
   Wire.requestFrom(ACCEL_ADDRESS, 6);
   
  while(Wire.available()) 
  { 
    buff[i++] = WIRE_RECEIVE();
  }
  Wire.endTransmission();  
  if (i == 6)  
  {
    acc_x = (((int16_t) buff[1]) << 8) | buff[0]; 
    acc_y = (((int16_t) buff[3]) << 8) | buff[2]; 
    acc_z = (((int16_t) buff[5]) << 8) | buff[4]; 
  }
 if(acc_x>60000)
  acc_x=acc_x-65536;
  if(acc_y>60000)
  acc_y=acc_y-65536;
  if(acc_z>60000)
  acc_z=acc_z-65536;
  acc_x=((acc_x-256.0)*(1.0+1.0)/(-256.0-256.0)-1.0);
  acc_y=((acc_y-247.0)*(1.0+1.0)/(-265.0-247.0)-1.0);
  acc_z=((acc_z-236.0)*(1.0+1.0)/(-260.0-236.0)-1.0);   
  count++;
  roll += (atan2(acc_x,sqrt(sq(acc_y)+sq(acc_z))))*180/3.14;
  pitch += (atan2(acc_y,sqrt(sq(acc_z)+sq(acc_x))))*180/3.14;
  yaw +=(atan2(acc_z,sqrt(sq(acc_y)+sq(acc_x))))*180/3.14;
  time++;
  if(time=5)
  { roll=(roll/count); 
    pitch=(pitch/count);
    yaw=(yaw/count);
  }
 }}

void printWifiStatus() {
  Serial.begin(115200);
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi IP address:
 ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}



