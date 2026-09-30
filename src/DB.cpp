#pragma once

#include <string>
#include "Recipe.cpp"

using namespace std;

class DB
{    
    public:
        virtual ~DB() {} //WHY?

        virtual Recipe* search(string name) = 0; //Overloading

        virtual Recipe* search(string* ingre) = 0; //Overloading

        virtual Recipe* order(int option) = 0;

        virtual void edit(string name, string newName, string* newIngredient, string* newStep) = 0;

        virtual void add(string name, string* ingredient, string* step) = 0;

        virtual void del(string name) = 0;
        
        virtual void load(string filePath) = 0;
};