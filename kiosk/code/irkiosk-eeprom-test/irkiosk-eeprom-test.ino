#include <EEPROM.h>

#define EEPROM_OFFSET 2
#define TAG_SIZE 5
#define TAG_ADDRESS(k) (((TAG_SIZE+1)*k)+EEPROM_OFFSET)

typedef struct { byte x[TAG_SIZE]; } rfid_tag_t;


int get_n_tags() {
	int n = 0;
	n |= EEPROM.read(0);
	n |= (EEPROM.read(1) << 8);
	return n;
}
void set_n_tags(int n) {
	int lo = n & 0x00ff;
	int hi = (n >> 8) & 0x00ff;
	EEPROM.update(0, lo);
	EEPROM.update(1, hi);
}


bool address_matches(rfid_tag_t tag, const int address) {
	for (int i=0; i<TAG_SIZE; i++) {
		if (EEPROM.read(address + i) != tag.x[i]) {
			return false;
		}
	}
	return true;
}
int find_tag_index(rfid_tag_t tag) {
	int n_tags = get_n_tags();
	for (int i=0; i<n_tags; i++) {
		const int address = TAG_ADDRESS(i);
		if (address_matches(tag, address)) { return i; }
	}
	// no matching addresses
	return -1;
};


void write_tag(int index, rfid_tag_t tag, byte category) {
	const int address = TAG_ADDRESS(index);
	for (int i=0; i<TAG_SIZE; i++) {
		EEPROM.update(address+i, tag.x[i]);
	}
	EEPROM.update(address+TAG_SIZE, category);
}


void setup() {
	Serial.begin(115200);
	rfid_tag_t tag = {{0, 1, 2, 3, 4 }};
	set_n_tags(700);
	write_tag(450, tag, 0);

	delay(20);

	Serial.println("looking for tag...");
	unsigned long start = micros();
	int idx = find_tag_index(tag);
	unsigned long stop = micros();
	Serial.print("found tag at index "); Serial.println(idx);
	Serial.print("took "); Serial.print(stop-start); Serial.println(" us");
}


void loop() {}
