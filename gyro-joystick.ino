#include <SPI.h>
#define CS_IMU 8 // Chip select for IMU can be any open digital pin
#define ImuToDeg(x) (((float)x)/100.0) // IMU angle info is in deg*100
#define ImuToVel(x) (((float)x)/10.0) // IMU angular vel is in deg/s*10
short roll, pitch, yaw, rolldot, pitchdot, yawdot;
short accel_x, accel_y, accel_z;

void setup()
{
  pinMode(CS_IMU, OUTPUT);
  pinMode(3, INPUT);  
  pinMode(4, INPUT);  
  pinMode(5, INPUT);  
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setDataMode(SPI_MODE3);
  delay(100);
}

void loop()
{
  ReadIMU(0);
  int pitchReading = map(ImuToDeg(pitch), -180, 180, 0, 1023);
  int yawReading = map(ImuToDeg(yaw), -180, 180, 0, 1023);
  int rollReading = map(ImuToDeg(roll), -180, 180, 0, 1023);
  // read analog inputs and set X-Y position
  Joystick.X(yawReading);
  Joystick.Y(pitchReading);
  Joystick.Z(rollReading);
  // a brief delay, so this runs 20 times per second
  delay(50);
}

void ReadIMU(byte cmd) {
digitalWrite(CS_IMU, LOW); // select device
byte byte0 = SPI.transfer(cmd); // start it
roll = getSPIint(cmd);
pitch = getSPIint(cmd);
yaw = getSPIint(cmd);
rolldot = getSPIint(cmd);
pitchdot = getSPIint(cmd);
yawdot = getSPIint(cmd);
accel_x = getSPIint(cmd);
accel_y = getSPIint(cmd);
accel_z = getSPIint(cmd);
digitalWrite(CS_IMU, HIGH); // deselct device
}

int getSPIint(int command){
byte byte0 = SPI.transfer(command);
byte byte1 = SPI.transfer(command);
int result = ( ((int)byte1 << 8) | (int)byte0 ); 
return(result);
}
