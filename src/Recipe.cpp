#pragma once

#include <string>
#include "Array.cpp"

using namespace std;

class Recipe
{
    private:
        string recipeName;
        StringArray* ingredient;
        StringArray* step;

    public:
        Recipe(string name, StringArray* ingredient, StringArray* step){
            this->recipeName = name;
            this->ingredient = ingredient;
            this->step = step;
        }

        // Destructor
        ~Recipe(){
            delete this->ingredient;
            delete this->step;
        }

        string getRecipeName() {return this->recipeName;}

        void setRecipeName(string newName) {this->recipeName = newName;}

        StringArray* getIngredient() {return this->ingredient;}

        void setIngredient(StringArray* newIngre){
            delete this->ingredient;
            this->ingredient = newIngre;
        }

        StringArray* getStep() {return this->step;}

        void setStep(StringArray* newStep){
            delete this->step;
            this->step = newStep;
        }
};
