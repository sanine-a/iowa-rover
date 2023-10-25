#include <Arduino.h>
#include "category.h"
#include "slot.h"
#include "pins.h"

SlotReader::SlotReader(
	Model& model, unsigned int commandIndex, Scheduler& sch, 
	Rfid& rfid, byte addr, 
	ShiftLamps& lamps, ShiftLamp& lamp,
	SlotButtons& slotButtons
) : 
	Subscriber<SlotButtonEvent>::Subscriber(&slotButtons),
	Subscriber<RfidEvent>::Subscriber(&rfid), 
	addr(addr),
	model(model), commandIndex(commandIndex), sch(sch), 
	lamps(lamps), lamp(lamp) {}


void SlotReader::on(RfidEvent e) {
	if (e.sourceAddr != addr) { return; } // ignore messages not from the address we are interested in
	byte category = model.tbl.findTag(e.tag);
	if (category == CATEGORY_UNKNOWN) {
		// flash lamp twice to indicate unknown tag
		lamp.turnOn();
		lamps.show();
		sch.setTimeout([this]{
			lamp.turnOff(); lamps.show(); sch.setTimeout([this]{
				lamp.turnOn(); lamps.show(); sch.setTimeout([this]{
					lamp.turnOff(); lamps.show();
				}, 500);
			}, 500);
		}, 500);
	} else {
		model.commands[commandIndex].action = category;
		model.commands[commandIndex].amount = 0.5;
		lamp.turnOff();
		lamps.show();
		sch.setTimeout([this]{
			lamp.turnOn(); lamps.show();
		}, 200);
	}
}


void SlotReader::on(SlotButtonEvent e) {
	if (e.sourceSlot != commandIndex) { return; }
	if (e.btn == SlotButtonEvent::Button::PUSH_INCREMENT) {
		model.commands[commandIndex].amount += (1.0/8.0);
	} else if (e.btn == SlotButtonEvent::Button::PUSH_DECREMENT) {
		model.commands[commandIndex].amount -= (1.0/8.0);
	}

	if (model.commands[commandIndex].amount < 0) {
		model.commands[commandIndex].amount  = 0;
	}
	if (model.commands[commandIndex].amount > 1) {
		model.commands[commandIndex].amount  = 1;
	}

	Serial.print(commandIndex); Serial.print(" amount: "); Serial.println(model.commands[commandIndex].amount);
}


SlotButton::SlotButton(
	Model& model, Publisher<SlotButtonEvent> *publisher,
	unsigned int slotIndex, SlotButtonEvent::Button buttonType, 
	unsigned int buttonPin, ShiftLamps& lamps, ShiftLamp& lamp
) : 
	model(model), publisher(publisher),
	slotIndex(slotIndex), buttonType(buttonType), 
	PolledSwitch(buttonPin), lamps(lamps), lamp(lamp)
{}

void SlotButton::update() {
	PolledSwitch::update();
	if (model.commands[slotIndex].action != action) {
		action = model.commands[slotIndex].action;
		if (action == Model::Command::Action::NONE) {
			lamp.turnOff(); lamps.show();
		} else {
			lamp.turnOn(); lamps.show();
		}
	}
}

void SlotButton::onLow() {
	if (action == Model::Command::Action::NONE) { return; } // ignore presses when the action is NONE
	struct SlotButtonEvent event = { slotIndex, buttonType };
	publisher->publish(event);
}
void SlotButton::onHigh() {}


SlotButtons::SlotButtons(Model& model, ShiftLamps& lamps) :
	inc1(model, this, 0, SlotButtonEvent::Button::PUSH_INCREMENT, UP1_BTN, lamps, lamps.inc1),
	dec1(model, this, 0, SlotButtonEvent::Button::PUSH_DECREMENT, DOWN1_BTN, lamps, lamps.dec1),
	inc2(model, this, 1, SlotButtonEvent::Button::PUSH_INCREMENT, UP2_BTN, lamps, lamps.inc2),
	dec2(model, this, 1, SlotButtonEvent::Button::PUSH_DECREMENT, DOWN2_BTN, lamps, lamps.dec2),
	inc3(model, this, 2, SlotButtonEvent::Button::PUSH_INCREMENT, UP3_BTN, lamps, lamps.inc3),
	dec3(model, this, 2, SlotButtonEvent::Button::PUSH_DECREMENT, DOWN3_BTN, lamps, lamps.dec3),
	inc4(model, this, 3, SlotButtonEvent::Button::PUSH_INCREMENT, UP4_BTN, lamps, lamps.inc4),
	dec4(model, this, 3, SlotButtonEvent::Button::PUSH_DECREMENT, DOWN4_BTN, lamps, lamps.dec4)
{}

void SlotButtons::update() {
	inc1.update(); dec1.update();
	inc2.update(); dec2.update();
	inc3.update(); dec3.update();
	inc4.update(); dec4.update();
}


SlotBarGraph::SlotBarGraph(Model& model, unsigned int slotIndex, Adafruit_NeoPixel& strip) :
	model(model), slotIndex(slotIndex), strip(strip) {}


void SlotBarGraph::update() {
	if (model.commands[slotIndex].action == Model::Command::Action::NONE) { 
		showAmount(0);
	} else {
		showAmount(8 * model.commands[slotIndex].amount);
	}
}

void SlotBarGraph::showAmount(unsigned int amount) {
	auto color = strip.Color(255, 0, 0);
	unsigned int offset = slotIndex * 8;
	for (int i=0; i<8; i++) {
		unsigned int px = i + offset;
		if (i < amount) {
			strip.setPixelColor(px, color);
		} else {
			strip.setPixelColor(px, strip.Color(0, 0, 0));
		}
	}
}

void SlotBarGraph::clear() {
	showAmount(0);
}


SlotBarGraphs::SlotBarGraphs(Model& model) :
	strip(32, NEOPIXELS, NEO_GRB + NEO_KHZ800),
	bar1(model, 0, strip),
	bar2(model, 1, strip),
	bar3(model, 2, strip),
	bar4(model, 3, strip)
{
	strip.begin();
	strip.clear();
}


void SlotBarGraphs::update() {
	bar1.update();
	bar2.update();
	bar3.update();
	bar4.update();
	strip.show();
}
