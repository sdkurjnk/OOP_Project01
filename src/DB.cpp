#pragma once

#include <string>
#include "RecipeArray.cpp"
#include "StringArray.cpp"

using namespace std;

// Result codes returned by mutation methods (add/edit/del).
enum DBResult {
    DB_OK = 0,       // success
    DB_EMPTY_NAME,   // recipe name was empty
    DB_NULL_ARG,     // a required array argument was NULL
    DB_NOT_FOUND     // no recipe matched the given name
};

class DB
{
    public:
        virtual ~DB() {}

        virtual RecipeArray* search(string name) = 0;  // Overloading
        virtual RecipeArray* search(StringArray* ingre) = 0; // Overloading
        virtual RecipeArray* order(int option) = 0;

        virtual int edit(string name, string newName, StringArray* newIngredient, StringArray* newStep) = 0;
        virtual int add(string name, StringArray* ingredient, StringArray* step) = 0;
        virtual int del(string name) = 0;
};
