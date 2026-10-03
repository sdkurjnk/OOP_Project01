#pragma once

#include <string>
#include "RecipeArray.cpp"
#include "StringArray.cpp"
#include "Recipe.cpp"
#include "DB.cpp"

using namespace std;

class RecipeDB : public DB
{
    private:
        RecipeArray* recipes;
        string filePath;

    public:
        RecipeDB(string filePath);
        ~RecipeDB();

        RecipeArray* search(string name); // Overloading
        RecipeArray* search(StringArray* ingre); // Overloading
        RecipeArray* order(int option);

        void edit(string name, string newName, StringArray* newIngredient, StringArray* newStep);
        void add(string name, StringArray* ingredient, StringArray* step);
        void del(string name);

        void load();
};
