#include "Arduino.h"
#include "MQTT_Ethernet.h"

#include <SPI.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


/* -----Configuration::------------ */
/* -------------------------------- */
const char tripodNo = '$tripodNo'; // TODO: change this to the tripod/teensy number (zero based)
/* -------------------------------- */
/* -----ALL DONE---NOW UPLOAD------ */


#include <Ethernet.h>
#include <EthernetClient.h>
#include <Dns.h>
#include <Dhcp.h>

#define IO_SERVER      "10.0.0.1"
#define IO_SERVERPORT  1883
#define IO_USERNAME    "guest"
#define IO_KEY         "guest"


//Set up the ethernet client
EthernetClient client;

Adafruit_MQTT_Client mqtt(&client, IO_SERVER, IO_SERVERPORT, IO_USERNAME, IO_KEY);

// You don't need to change anything below this line!
#define halt(s) { Serial.println(F( s )); while(1);  }

Adafruit_MQTT_Publish touchstate_senders[3][2] = {

 {Adafruit_MQTT_Publish(&mqtt,  "tripods/0/tube/0/side/0"),  Adafruit_MQTT_Publish(&mqtt,  "tripods/0/tube/0/side/1") },

 {Adafruit_MQTT_Publish(&mqtt,  "tripods/0/tube/1/side/0"),  Adafruit_MQTT_Publish(&mqtt,  "tripods/0/tube/1/side/1") },

 {Adafruit_MQTT_Publish(&mqtt,  "tripods/0/tube/2/side/0"),  Adafruit_MQTT_Publish(&mqtt,  "tripods/0/tube/2/side/1") }

};

MQTT_Ethernet::MQTT_Ethernet() {
  // Initialise the Client
  Serial.print(F("\nInit the Client..."));

//  touchstate_senders [0][0] = Adafruit_MQTT_Publish(&mqtt,  "tripods/0/tube/0/side/0");
//  touchstate_senders [0][1] = Adafruit_MQTT_Publish(&mqtt,  "tripods/0/tube/0/side/1");
//  touchstate_senders [1][0] = Adafruit_MQTT_Publish(&mqtt,  "tripods/0/tube/1/side/0");
//  touchstate_senders [1][1] = Adafruit_MQTT_Publish(&mqtt,  "tripods/0/tube/1/side/1");
//  touchstate_senders [2][0] = Adafruit_MQTT_Publish(&mqtt,  "tripods/0/tube/2/side/0");
//  touchstate_senders [2][1] = Adafruit_MQTT_Publish(&mqtt,  "tripods/0/tube/2/side/1");
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_Ethernet::MQTT_connect() {
  int8_t ret;
  
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
  
  Serial.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
  }
  Serial.println("MQTT Connected!");
}

void MQTT_Ethernet::send_touchState(int tube, int side, boolean touchState) {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // Now we can publish stuff!
  //Serial.print(F("\nSending touchState "));
  //Serial.print(touchState);
  //Serial.print("...");

//  if (millis()%2000 < 2) {
  Serial.println("tube = " + String(tube) + "  side = " + String(side));
  Adafruit_MQTT_Publish touchstate_sender = touchstate_senders [tube][side];
  if (!touchstate_sender.publish((uint32_t)touchState)) {
    Serial.println(F("Failed"));
  } else {
    Serial.print(F("MQTT Send: "));
    Serial.println(touchState);
  }
//  }
}
