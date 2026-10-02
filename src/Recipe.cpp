#pragma once

#include <string>

using namespace std;

class Recipe
{
    private:
        string recipeName;
        string* ingredient;
        int ingredientCount;
        string* step;
        int stepCount;

    public:
        //Constructor
        Recipe(string name, string* ingredient, int ingredientCount, string* step, int stepCount){
            this->recipeName = name;

            this->ingredient = new string[ingredientCount];
            this->ingredientCount = ingredientCount;
            for (int i = 0; i < ingredientCount; i++) {
                this->ingredient[i] = ingredient[i];
            }

            this->step = new string[stepCount];
            this->stepCount = stepCount;
            for (int i = 0; i < stepCount; i++){
                this->step[i] = step[i];
            }
        }

        //Destructor
        ~Recipe(){
            delete[] this->ingredient;
            delete[] this->step;
        }

        string getRecipeName() {return this->recipeName;}

        void setRecipeName(string newName) {this->recipeName = newName;}

        string* getIngredient() {return this->ingredient;}

        int getIngredientCount() {return this->ingredientCount;}

        void setIngredient(string* newIngre, int newCount){
            string* temp = new string[newCount];

            for (int i = 0; i < newCount; i++){
                temp[i] = newIngre[i];
            }

            delete[] this->ingredient;

            this->ingredient = temp;
            this->ingredientCount = newCount;
        }

        string* getStep() {return this->step;}
        
        int getStepCount() {return this->stepCount;}

        void setStep(string* newStep, int newCount){
            string* temp = new string[newCount];

            for (int i = 0; i < newCount; i++){
                temp[i] = newStep[i];
            }

            delete[] this->step;

            this->step = temp;
            this->stepCount = newCount;
        }
};