#pragma once

#include <string>
#include "Recipe.cpp"
#include "DB.cpp"

using namespace std;

class RecipeDB : public DB
{
    private:
        Recipe* recipes;
    
    public:
        RecipeDB(string filePath);

        Recipe* search(string name); //Overloading

        Recipe* search(string* ingre); //Overloading

        Recipe* order(int option);

        void edit(string name, string newName, string* newIngredent, string* newStep);

        void add(string name, string* ingredent, string* step);

        void del(string name);
        
        void load(string filePath);
};