#include "Arduino.h"
#include "Artnet.h"

/* -----Configuration::------------------- */
/* --------------------------------------- */
byte UniverseID1 = { // TODO: teensy index (0 based)
  $universe1};

byte UniverseID2 = { // TODO: teensy index (0 based) + 1
  $universe2};
/* --------------------------------------- */
/* -NEXT CHANGE IN FILE mqtt_ethernet.cpp- */


 /*Ethernet Notes:
 this was tested using a Wiz820io ethernet module. Standard SPI pin connections, as well as reset on Wiz820io to T3 pin 9
 
 Artnet Notes:
 ARTNET RECEIVER v3.1
 
 This SCRIPT allows you to use arduino with ethernet shield or wifi shield and recieve artnet data. Up to you to use channels as you want.
 
 Tested with Arduino 1.0.5, so this code should work with the new EthernetUdp library (instead of the depricated Udp library)
 
 If you have implemented improvements to this sketch, please contribute by sending back the modified sketch. It will be a pleasure to let them accessible to community
 
 Original code by (c)Christoph Guillermet, designed to be used with the free open source lighting software WhiteCat: http://www.le-chat-noir-numerique.fr
 karistouf@yahoo.fr
 
 v3, modifications by David van Schoorisse <d.schoorisse@gmail.com>
 Ported code to make use of the new EthernetUdp library used by Arduino 1.0 and higher.
 
 V3.1 by MSBERGER 130801
 - performance gain by shrinking buffer sizes from "UDP_TX_PACKET_MAX_SIZE" to 768
 - implementation of selction / filtering SubnetID and UniverseID (was already prepared by karistouf)
 - channel count starts at 0 instead of 1 (the digital and vvvv way)
 - artnet start_address+n is now mapped to "arduino-channel" 0+n (was also start_address+n before), now it is similar to lighting fixtures
 */

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// libraries
#include <stdlib.h>
#include <SPI.h>        
#include <Ethernet.h>
#include <EthernetUdp.h>
//#include "FastLED.h"
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// from Artnet Receiver Sketch
#define short_get_high_byte(x) ((HIGH_BYTE & x) >> 8)
#define short_get_low_byte(x)  (LOW_BYTE & x)
#define bytes_to_short(h,l) ( ((h << 8) & 0xff00) | (l & 0x00FF) );

// the next two variables are set when a packet is received
byte remoteIp[4];        // holds received packet's originating IP
unsigned int remotePort; // holds received packet's originating port

  
short first_universe = UniverseID1;
short second_universe = UniverseID1+1;

//buffers
const int MAX_BUFFER_UDP = 768;
char packetBuffer[531]; //buffer to store incoming data
//byte buffer_channel_arduino[number_of_channels]; //buffer to store filetered DMX data

// art net parameters
unsigned int localPort = 6454;      // artnet UDP port is by default 6454
const int art_net_header_size = 17;
const int max_packet_size = 530; //512 data + 18 byte header
char ArtNetHead[8] = "Art-Net";
char OpHbyteReceive = 0;
char OpLbyteReceive = 0;
//short is_artnet_version_1=0;
//short is_artnet_version_2=0;
short seq_artnet=0;
//short artnet_physical=0;
short incoming_universe = 0;
boolean is_opcode_is_dmx = 0;
boolean is_opcode_is_artpoll = 0;
boolean match_artnet = 1;
short Opcode = 0;

EthernetUDP Udp;

int rgbSerialIdx = 0; // index to keep track of serial inputs

int bytecounter;

Artnet::Artnet() {
  Udp.begin(localPort);
  Serial.println(F("START ARTNET"));
}

void Artnet::receive_artnet(byte* rgb, int start_address, int total_number_of_channels) {
  int packetSize = Udp.parsePacket();
 
  //FIXME: test/debug check
  if(packetSize>art_net_header_size && packetSize<=max_packet_size) {//check size to avoid unneeded checks
    //if(packetSize) {
    
    //Serial.println(F("RCVD PKG!"));

    IPAddress remote = Udp.remoteIP();    
    remotePort = Udp.remotePort();
    Udp.read(packetBuffer,531);
  
    //Serial.println(packetBuffer);
        
    //read header
    match_artnet=1;
    for (int i=0;i<7;i++) {
      //if not corresponding, this is not an artnet packet, so we stop reading
      if(char(packetBuffer[i])!=ArtNetHead[i]) {
        match_artnet=0;
        break;
      } 
    } 

    //if its an artnet header
    if(match_artnet==1) { 
      //artnet protocole revision, not really needed
      //is_artnet_version_1=packetBuffer[10]; 
      //is_artnet_version_2=packetBuffer[11];*/

      //sequence of data, to avoid lost packets on routers
      seq_artnet=packetBuffer[12];

      //physical port of  dmx N°
      //artnet_physical=packetBuffer[13];*/

      //operator code enables to know wich type of message Art-Net it is
      Opcode=bytes_to_short(packetBuffer[9],packetBuffer[8]);

      //if opcode is DMX type
      if(Opcode==0x5000) {
        is_opcode_is_dmx=1;
        is_opcode_is_artpoll=0;
      }   

      //if opcode is artpoll 
      else if(Opcode==0x2000) {
        is_opcode_is_artpoll=1;
        is_opcode_is_dmx=0;
        //( we should normally reply to it, giving ip adress of the device)
      } 

      //if its DMX data we will read it now
      if(is_opcode_is_dmx=1) {

        //read incoming universe
        incoming_universe= bytes_to_short(packetBuffer[15],packetBuffer[14])
          //if it is selected universe DMX will be read
          if(incoming_universe==first_universe) {
            //Serial.println(F("RCVD PKG UNIVERSE 1!"));
            //getting data from a channel position, on a precise amount of channels, this to avoid to much operation if you need only 4 channels for example
            //channel position
            for(int i = start_address; i < 512; i++) {
              //buffer_channel_arduino[i-start_address]= byte(packetBuffer[i+art_net_header_size+1]);
              rgb[i-start_address]= byte(packetBuffer[i+art_net_header_size+1]);
            }  
          }
          else if(incoming_universe==second_universe) {
            //Serial.println(F("RCVD PKG UNIVERSE 2!"));

            //getting data from a channel position, on a precise amount of channels, this to avoid to much operation if you need only 4 channels for example
            //channel position
            for(int i = start_address; i < total_number_of_channels-512; i++) {
              //buffer_channel_arduino[i-start_address]= byte(packetBuffer[i+art_net_header_size+1]);
              rgb[i-start_address+512]= byte(packetBuffer[i+art_net_header_size+1]);
            } 
          }
      }
    }
  }
}
