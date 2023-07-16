#include <RH_RF69.h>
#include <SPI.h>
/************ Radio Setup ***************/
#define RF69_FREQ 915.0
#define RFM69_CS 8
#define RFM69_INT 7
#define RFM69_RST 4

#define LED 13
RH_RF69 rf69(RFM69_CS, RFM69_INT);
int16_t packetnum = 0; 

/**
 Called when the board starts. The function initializes serial communication.
*/
void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }  // wait until serial console is open,
  remove if not tethered to computer pinMode(LED, OUTPUT);
  //Setting the RFM 69 as an output pin
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);
  Serial.println("Feather RFM69 TX Test!");
  Serial.println();
  // Radio Initialization
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);
  if (!rf69.init()) {
    Serial.println("RFM69 radio init failed");
    while (1)
      ;
  }
  Serial.println("RFM69 radio init OK!");
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd25
  //
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
  }
  // If you are using a high power RF69 eg RFM69HW, you *must* set a
  Tx power with the
      // ishighpowermodule flag set like this:
      rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg
  // must be true for 69HCW
  //  The encryption key has to be the same as the one in the serve

  uint8_t key[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                   0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

  rf69.setEncryptionKey(key);
  pinMode(LED, OUTPUT);
  Serial.print("RFM69 radio @");
  Serial.print((int)RF69_FREQ);
  Serial.println(" MHz");
}
void loop() {
  delay(1000);
  char input = Serial.read();
  if (Serial.available() && input != 0) {
    char radiopacket[2] = "n";
    radiopacket[0] = input;
    Serial.print("Sending ");
    Serial.println(radiopacket);
    // Send a message!
    rf69.send((uint8_t *)radiopacket, strlen(radiopacket));
    rf69.waitPacketSent();
    // Now wait for a reply
    uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf69.waitAvailableTimeout(500)) {
      // Should be a reply message for us now
      if (rf69.recv(buf, &len)) {
        Serial.print("Got a reply: ");
        Serial.println((char *)buf);
      } else {
        Serial.println("Receive failed");
      }
    } else {
      Serial.println("No reply, is another RFM69 listening?");
    }
    input = 0;
  }
}
