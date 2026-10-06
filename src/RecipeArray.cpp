#pragma once

#include <stdlib.h>
#include <iostream>
#include "Recipe.cpp"

using namespace std;

class RecipeArray
{
    private:
        Recipe** recipe; //array of Recipe instances
        int count;

    public:
        RecipeArray(){
            this->recipe = NULL;
            this->count = 0;
        }

        ~RecipeArray() {free(this->recipe);} //deallocate only array of Recipe instances

        void add(Recipe* item){
            this->recipe = (Recipe**)realloc(this->recipe, sizeof(Recipe*) * (this->count + 1));
            this->recipe[this->count] = item;
            this->count = this->count + 1;
        }

        int size() {return this->count;}

        Recipe* get(int index){
            if (index < 0 || index >= this->count) {
                cout << "ERROR: index of RecipeArray should be in 0 to arraysize-1." <<endl;
                return NULL;
            }
            return this->recipe[index];
        }
};
