#pragma once

#include <string>
#include "RecipeArray.h"
#include "StringArray.h"
#include "Recipe.h"
#include "DB.h"

using namespace std;

class RecipeDB : public DB
{
    public:
        RecipeDB(string filePath);
        ~RecipeDB();

        // query
        Response* search(string name);     // Overloading
        Response* search(StringArray* ingre); // Overloading
        Response* order(int option);

        // mutation
        DBResult edit(string name, string newName, StringArray* newIngre, StringArray* newStep);
        DBResult add(string name, StringArray* ingre, StringArray* step);
        DBResult del(string name);

        // persistence
        DBResult save();
        DBResult load();

    private:
        RecipeArray* recipes;
        string filePath;

        int findIndex(string name);
        string pad2(int number);
        string makeBorder(int width);
        bool isBorder(string line);
};
