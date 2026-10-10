#include "../header/Recipe.h"

using namespace std;

Recipe::Recipe(string name, StringArray* ingredient, StringArray* step){
    this->recipeName = name;
    this->ingredient = ingredient;
    this->step = step;
}

// Destructor
Recipe::~Recipe(){
    delete this->ingredient;
    delete this->step;
}

string Recipe::getRecipeName() {return this->recipeName;}

void Recipe::setRecipeName(string newName) {this->recipeName = newName;}

StringArray* Recipe::getIngredient() {return this->ingredient;}

void Recipe::setIngredient(StringArray* newIngre){
    delete this->ingredient;
    this->ingredient = newIngre;
}

StringArray* Recipe::getStep() {return this->step;}

void Recipe::setStep(StringArray* newStep){
    delete this->step;
    this->step = newStep;
}
