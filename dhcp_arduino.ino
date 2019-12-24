/*
Connect to Ethernet Using DHCP
Author:- Embedded Laboratory
*/

#include <SPI.h>
#include <Ethernet.h>

#define ETH_CS    10
#define SD_CS  4

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {0xB0,0xCD,0xAE,0x0F,0xDE,0x10};

IPAddress ip(192,168,1,17);

IPAddress server(192,168,1,10); 

EthernetClient client;

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting Ethernet Connection");
  pinMode(ETH_CS,OUTPUT);
  pinMode(SD_CS,OUTPUT);
  digitalWrite(ETH_CS,LOW); // Select the Ethernet Module.
  digitalWrite(SD_CS,HIGH); // De-Select the internal SD Card
  if (Ethernet.begin(mac) == 0)  // Start in DHCP Mode
  {
    Serial.println("Failed to configure Ethernet using DHCP, using Static Mode");
    // If DHCP Mode failed, start in Static Mode
    Ethernet.begin(mac, ip);
  }
  
  Serial.print("My IP address: ");
  
  for (byte thisByte = 0; thisByte < 4; thisByte++)
  {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
  
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 5000))
  {
    Serial.println("connected");
  } 
  else
  {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loop()
{
  // if there are incoming bytes available 
  // from the server, read them and print them:
  if (client.available())
  {
    char c = client.read();
    client.write(c);
    Serial.print(c);
  }

  // as long as there are bytes in the serial queue,
  // read them and send them out the socket if it's open:
  while (Serial.available() > 0)
  {
    char inChar = Serial.read();
    if (client.connected())
    {
      client.print(inChar); 
    }
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    // do nothing:
    while(true);
  }
}
