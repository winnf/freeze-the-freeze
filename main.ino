#include <Wire.h>
int mpu = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
const int MOTOR = 9;

void setup() {
  Wire.begin();
  Wire.beginTransmission(mpu);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(115200);
  Serial.println("hello");
  pinMode(13, OUTPUT);
  pinMode(MOTOR,OUTPUT);
  delay(4000);
}

boolean pitchAngle ()
{
  int n = 30*2; //collect data for 2 seconds
  int16_t data[n][6];
  double previous, current, next, currentMax, currentMin;

  // Record data for 2 seconds
  for (int i = 0; i < n; i++) {
    Wire.beginTransmission(mpu);
    Wire.write(0x3B);                              // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(mpu,14,true);                // request a total of 14 registers
    data[i][0]=Wire.read()<<8|Wire.read();              // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    data[i][1]=Wire.read()<<8|Wire.read();              // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    data[i][2]=Wire.read()<<8|Wire.read();              // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    data[i][3]=Wire.read()<<8|Wire.read();              // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    data[i][4]=Wire.read()<<8|Wire.read();              // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    data[i][5]=Wire.read()<<8|Wire.read();              // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

    if (i > 1)
    {
     previous = atan2(data[i - 2][2], data[i - 2][0]);
     current = atan2(data[i - 1][2], data[i - 1][0]);
     next = atan2(data[i][2], data[i][0]);

      if (current > previous && current > next && current > 1.27 && current < 1.42)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
  }
}

void loop() {

  boolean isFreezing = false;
  int n= 30*2; // collect data for 1 second, determine whether freezing occurs in those 3 seconds
  int16_t yRotAcceleration[n];
  int16_t yAcceleration[n];
  int16_t data[n][6];
  double previous, current, next, currentMax, currentMin;
  int peaksCount;
  peaksCount=0;
  for (int i= 1; i<n ; i++) {
   Wire.beginTransmission(mpu);
   Wire.write(0x3B);
   Wire.endTransmission(false);
   Wire.requestFrom(mpu, 14, true);
   AcX = Wire.read()<<8|Wire.read();              // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
   AcY = Wire.read()<<8|Wire.read();              // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
   AcZ = Wire.read()<<8|Wire.read();              // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
   Tmp = Wire.read()<<8|Wire.read();
   GyX = Wire.read()<<8|Wire.read();
   GyY = Wire.read()<<8|Wire.read();
   GyZ = Wire.read()<<8|Wire.read();

   yRotAcceleration[i]= GyY;
   yAcceleration[i]= AcY;
    Serial.println(GyY);
   if (yRotAcceleration[i]> 600 && yRotAcceleration[i]<1000) {
     if (yRotAcceleration[i-1]< yRotAcceleration[i] && yRotAcceleration[i] > yRotAcceleration[i+1]){
        peaksCount= peaksCount+1;
     }
      Serial.println(peaksCount);
      if (peaksCount >5 ){
        Serial.println("FREEZE");
        isFreezing = true;
        break;
      }
      else {
        Serial.println("freezing not detecting");
      }
   }
  }

  if (pitchAngle() == true || isFreezing ){
    // Turn motor on
    for(int i = 0; i < 500; i++)
    {
      digitalWrite(13, HIGH);
      //analogWrite(MOTOR, i);
    }
    delay(2000);
  } else {
    digitalWrite(13, LOW);
    //analogWrite(MOTOR, 0);
  }
  delay(33);
}:
