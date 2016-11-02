#include "Arduino.h"
#include "Sensors.h"

#define numAntennas 12
#define pcbSide 0
#define farSide 1

// antenna pins Slave 1
int slaveOnePcbSideAntenna =  3;   // PCB side antenna
int slaveOneFarSideAntenna =  5;   // Far side antenna (start coax)
int slaveOnePcbSideAntenna2 = 23;   // PCB side antenna 2
int slaveOneFarSideAntenna2 = 22;   // Far side antenna 2 (start coax)

// antenna pins Slave 2
int slaveTwoPcbSideAntenna = 21;   // PCB side antenna
int slaveTwoFarSideAntenna = 19;   // Far side antenna (start coax)
int slaveTwoPcbSideAntenna2 = 18;   // PCB side antenna 2
int slaveTwoFarSideAntenna2 = 24;   // Far side antenna 2 (start coax)

// antenna pins master PCB
int MasterPcbSideAntenna = 32;   // PCB side antenna
int MasterFarSideAntenna = 31;   // Far side antenna (start coax)
int MasterPcbSideAntenna2 = 30;   // PCB side antenna 2
int MasterFarSideAntenna2 = 29;   // Far side antenna 2 (start coax)

Sensors::Sensors() {
    // configure all antenna pins as input
    pinMode(MasterPcbSideAntenna, INPUT);
    pinMode(MasterFarSideAntenna, INPUT);
    pinMode(MasterPcbSideAntenna2, INPUT);
    pinMode(MasterFarSideAntenna2, INPUT);

    pinMode(slaveOnePcbSideAntenna, INPUT);
    pinMode(slaveOneFarSideAntenna, INPUT);
    pinMode(slaveOnePcbSideAntenna2, INPUT);
    pinMode(slaveOneFarSideAntenna2, INPUT);

    pinMode(slaveTwoPcbSideAntenna, INPUT);
    pinMode(slaveTwoFarSideAntenna, INPUT);
    pinMode(slaveTwoPcbSideAntenna2, INPUT);
    pinMode(slaveTwoFarSideAntenna2, INPUT);

    // activate pullup resistors of the antenna inputs on Teensy
    digitalWrite(MasterPcbSideAntenna, HIGH);
    digitalWrite(MasterFarSideAntenna, HIGH);
    digitalWrite(MasterPcbSideAntenna2, HIGH);
    digitalWrite(MasterFarSideAntenna2, HIGH);

    digitalWrite(slaveOnePcbSideAntenna, HIGH);
    digitalWrite(slaveOneFarSideAntenna, HIGH);
    digitalWrite(slaveOnePcbSideAntenna2, HIGH);
    digitalWrite(slaveOneFarSideAntenna2, HIGH);

    digitalWrite(slaveTwoPcbSideAntenna, HIGH);
    digitalWrite(slaveTwoFarSideAntenna, HIGH);
    digitalWrite(slaveTwoPcbSideAntenna2, HIGH);
    digitalWrite(slaveTwoFarSideAntenna2, HIGH);
}

void Sensors::read(boolean touchData[][2]) {  // if not working, try to erase 3
  touchData[0][0] = !(bool)digitalRead(MasterPcbSideAntenna) || !(bool)digitalRead(MasterPcbSideAntenna2);    // PCB side antenna s1, s3
  touchData[0][1] = !(bool)digitalRead(MasterFarSideAntenna) || !(bool)digitalRead(MasterFarSideAntenna2);    // Far side antenna (start coax) s4, s2
  //Serial.println(touchData[0][0]);
  
  touchData[1][0] = !(bool)digitalRead(slaveOnePcbSideAntenna) || !(bool)digitalRead(slaveOnePcbSideAntenna2);    // PCB side antenna
  touchData[1][1] = !(bool)digitalRead(slaveOneFarSideAntenna) || !(bool)digitalRead(slaveOneFarSideAntenna2);    // Far side antenna (start coax)

  touchData[2][0] = !(bool)digitalRead(slaveTwoPcbSideAntenna) || !(bool)digitalRead(slaveTwoPcbSideAntenna2);    // PCB side antenna
  touchData[2][1] = !(bool)digitalRead(slaveTwoPcbSideAntenna) || !(bool)digitalRead(slaveTwoFarSideAntenna2);   // Far side antenna (start coax)
}


