#pragma once

#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

class Recipe; //forward declaration: RecipeArray only stores/returns Recipe pointers

class StringArray
{
    private:
        string* str; //Array of string values
        int count;

    public:
        StringArray(){
            this->str = NULL;
            this->count = 0;
        }

        ~StringArray() {delete[] this->str;}

        void add(string item){
            string* newArray = new string[this->count + 1];
            for (int i = 0; i < this->count; i++){
                newArray[i] = this->str[i];
            }
            newArray[this->count] = item;

            delete[] this->str;
            this->str = newArray;
            this->count = this->count + 1;
        }

        int size() {return this->count;}

        string get(int index){
            if (index < 0 || index >= this->count) {
                cout << "ERROR: index of StringArray should be in 0 to arraysize-1." <<endl;
                return "";
            }
            return this->str[index];
        }
};

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
