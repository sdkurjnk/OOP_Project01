#pragma once

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

        ~RecipeArray() {delete[] this->recipe;} //deallocate only array of Recipe pointers, not the Recipe instances

        void add(Recipe* item){
            Recipe** newArray = new Recipe*[this->count + 1];
            for (int i = 0; i < this->count; i++) {newArray[i] = this->recipe[i];}
            newArray[this->count] = item;

            delete[] this->recipe;
            this->recipe = newArray;
            this->count = this->count + 1;
        }

        int size() {return this->count;}

        Recipe* get(int index){
            if (index < 0 || index >= this->count) {return NULL;}
            return this->recipe[index];
        }
};
