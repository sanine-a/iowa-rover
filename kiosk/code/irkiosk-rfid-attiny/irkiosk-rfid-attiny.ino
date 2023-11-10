#include "ID12LA.h"
#include "Tag.h"

#include <Wire.h>

using namespace smm;

RfidTag tag;
ID12LA reader;


#define BASE_ADDRESS 0x70

#define CMD_SET_ADDR 0x44
#define CMD_CLEAR_TAG 0x54

#define READ_PIN 0
#define LED_PIN 10
#define FLASH_TIME 100

#define ADDR_P1 1
#define ADDR_P2 2
#define ADDR_P3 3


// briefly blink the LED
void flash() {
    digitalWrite(LED_PIN, 1);
    delay(FLASH_TIME);
    digitalWrite(LED_PIN, 0);
}


// process incoming ID12LA data
void onReadTag(RfidTag& t) {
    memcpy(tag.tagData, t.tagData, 5*sizeof(byte));
    flash();
}


// transmit the last scanned tag over i2c
void sendTag() {
    Wire.write(tag.tagData, 5);
    Wire.write(tag.checksum());
}


// clear the last stored tag to all 0xff
void clearTag() {
    memset(tag.tagData, 0xff, 5 * sizeof(byte));
}


// process incoming i2c commands
void processCommand(int n) {
    if (n < 1)
	return;

    byte command = Wire.read();
    if (command == CMD_CLEAR_TAG)
	clearTag();
}


// determine the i2c address from the on-board jumpers
uint8_t read_offset() {
	pinMode(ADDR_P1, INPUT_PULLUP);
	pinMode(ADDR_P2, INPUT_PULLUP);
	pinMode(ADDR_P3, INPUT_PULLUP);
	delay(10);
	uint8_t x = (digitalRead(ADDR_P3) << 2) | (digitalRead(ADDR_P2) << 1) | (digitalRead(ADDR_P1) << 0);
	return 0x07 & (~x);

}


void setup() {
    reader.setup(READ_PIN, onReadTag);
    
    Wire.begin(BASE_ADDRESS + read_offset());
    Wire.onRequest(sendTag);
    Wire.onReceive(processCommand);

    pinMode(LED_PIN, OUTPUT);
    flash();
}

void loop() {
    reader.update();
}
