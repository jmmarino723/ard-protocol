#include <Arduino.h>
#include "datalink/datalink.h"
#include "package/package.h"


Datalink datalink = Datalink(0x7E, 90);
uint16_t send_freq = 3000;
long timer = millis();
char serialRead;
const int LED_PIN = 22;

void setup() {
    Serial.begin(9600);
    Serial.setTimeout(1000);
    pinMode(LED_PIN, OUTPUT);
}

Package dataParser(Datalink link, Stream &uart) {
    link.read(uart);
    uint8_t size = link.available();
    if (size){
        return Package(link.getPayload(), size);
    } else {
        return Package(0);
    }
}


void loop() {
    Package pack = dataParser(datalink, Serial);

    if (pack.getBuffer()[0]==0x7E){
        Package output = Package(3);
        output.addData(0x0B, pack.getValue(0x0A));
        datalink.send(output.dump(), output.getSize(), Serial);
        delay(500);
        if (pack.getBuffer()[1]==0x01){
            digitalWrite(LED_PIN, HIGH);
        }
        else if (pack.getBuffer()[1]==0x00){
            digitalWrite(LED_PIN, LOW);
        }
    }

    if (millis() - timer >= send_freq) {
        timer = millis();
        Package output = Package(3);
        output.addData(0x0A, 10);
        datalink.send(output.dump(), output.getSize(), Serial);
    }
    delay(500);
    if (Serial.available() > 0) {

        serialRead = Serial.read();
        if (serialRead == '7E037E010080'){
            digitalWrite(LED_PIN, HIGH);
        }
        else if (serialRead == '7E037E000081'){
            digitalWrite(LED_PIN, LOW);            
        }
  }
}