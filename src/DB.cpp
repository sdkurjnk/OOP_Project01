#pragma once

#include <string>
#include "RecipeArray.cpp"
#include "StringArray.cpp"

using namespace std;

class DB
{
    public:
        virtual ~DB() {}

        virtual RecipeArray* search(string name) = 0;  // Overloading
        virtual RecipeArray* search(StringArray* ingre) = 0; // Overloading
        virtual RecipeArray* order(int option) = 0;

        virtual void edit(string name, string newName, StringArray* newIngredient, StringArray* newStep) = 0;
        virtual void add(string name, StringArray* ingredient, StringArray* step) = 0;
        virtual void del(string name) = 0;

        virtual void load() = 0;
};
