#pragma once

#include <string>
#include "StringArray.h"

using namespace std;

class Recipe
{
    public:
        Recipe(string name, StringArray* ingredient, StringArray* step);
        ~Recipe();

        string getRecipeName();
        void setRecipeName(string newName);
        StringArray* getIngredient();
        void setIngredient(StringArray* newIngre);
        StringArray* getStep();
        void setStep(StringArray* newStep);

    private:
        string recipeName;
        StringArray* ingredient;
        StringArray* step;
};
