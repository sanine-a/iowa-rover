#include <Arduino.h>
#include "category.h"
#include "slot.h"
#include "pins.h"

SlotReader::SlotReader(
	Model& model, unsigned int commandIndex, Scheduler& sch, 
	Rfid& rfid, byte addr, 
	ShiftLamps& lamps, ShiftLamp& lamp
) : Subscriber(&rfid), addr(addr),
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
