#include "mvc.h"


// --===== Model =====--

void Model::update() {}


// --===== View =====--

void View::update() {
	progBtns.update();
	progLeds.update();
}


// --===== Controller =====--

Controller::Controller(struct Model& model, struct View& view) 
: progController(view.progBtns, view.progLeds) {}

void Controller::update() {
	progController.update();
}


// --===== extern globals =====--

struct Model model;
struct View view;
struct Controller controller(model, view);


void mvc_update() {
	model.update();
	view.update();
	controller.update();
}
