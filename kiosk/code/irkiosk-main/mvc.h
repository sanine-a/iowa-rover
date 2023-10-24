#pragma once

#include "programmer_buttons.h"
#include "programmer_controller.h"
#include "programmer_leds.h"


struct Model {
	void update();
};

struct View {
	Programmer::Buttons progBtns;
	Programmer::Leds progLeds;

	void update();
};

struct Controller {
	Programmer::Controller progController;

	Controller(struct Model& model, struct View& view);
	void update();
};


void mvc_update();


extern struct Model model;
extern struct View view;
extern struct Controller controller;
