#pragma once

#include <string>

using namespace std;

class Recipe
{
    private:
        string recipeName;
        string* ingredent;
        string* step;
    
    public:
        string getRecipeName();

        void setRecipeName(string newName);

        string* getIngredient();

        void setIngredient(string* newIngre);

        string* getStep();

        void setStep(string* newStep);
};