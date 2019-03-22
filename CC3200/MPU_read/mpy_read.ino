
#include <Wire.h> // For I2C communication
const int MPU_addr=0x68;  // I2C address of the MPU6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
void setup()   {                
 Serial.begin(9600);
 pinMode(13, OUTPUT); //Set laser diode pin
 Wire.begin();

 delay(50);
 
 Wire.beginTransmission(MPU_addr);
 Wire.write(0x6B);  // PWR_MGMT_1 register
 Wire.write(0);     // set to zero (wakes up the MPU-6050)
 Wire.endTransmission(true);
 
}


void loop() {


  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 6 registers

  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  Wire.endTransmission(true);
  float ax=AcX;
  ax=ax/4096;
      //  ax=(ax/65536); //Convert to g-force (2g sensitivity)
   float ay=AcY;
   ay=ay/4096;
        //ay=(ay/65536); //Convert to g-force (2g sensitivity)
   float az=AcZ;
   az/=4096;
        //az=(az/65536); //Convert to g-force (2g sensitivity)
    float gx=GyX;
        gx=(gx/4096); //Convert to g-force (2g sensitivity)
   float gy=GyY;
        gy=(gy/4096); //Convert to g-force (2g sensitivity)
   float gz=GyZ;
       gz=(gz/4096); //Convert to g-force (2g sensitivity)
  Serial.print(ax);
  Serial.print("\t");
  Serial.print(ay);
  Serial.print("\t");
  Serial.print(az);
  Serial.print("\t");
  Serial.print(gx);
  Serial.print("\t");
  Serial.print(gy);
  Serial.print("\t");
  Serial.print(gz);
  Serial.print("\n");

}
