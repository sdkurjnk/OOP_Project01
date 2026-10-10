#pragma once

#include <string>
#include "RecipeArray.h"
#include "StringArray.h"

using namespace std;

enum DBResult {
    DB_OK = 0,         // success
    DB_EMPTY_NAME,     // recipe name was empty
    DB_NULL_ARG,       // a required array argument was NULL
    DB_NOT_FOUND,      // no recipe matched the given name
    DB_BAD_OPTION,     // order: option was not a valid value
    DB_DUPLICATE,      // add: a recipe with that name already exists
    DB_SAVE_FAILED,    // save: could not open the file for writing
    DB_FILE_NOT_FOUND  // load: file did not exist (started with empty DB)
};

class Response
{
    private:
        RecipeArray* recipeArray;
        DBResult responseResult;

    public:
        Response(RecipeArray* array, DBResult result);
        ~Response();

        RecipeArray* getArray();
        DBResult getResult();
};

class DB
{
    public:
        virtual ~DB() {}

        virtual Response* search(string name) = 0;  // Overloading
        virtual Response* search(StringArray* ingre) = 0; // Overloading
        virtual Response* order(int option) = 0;

        virtual DBResult edit(string name, string newName, StringArray* newIngre, StringArray* newStep) = 0;
        virtual DBResult add(string name, StringArray* ingre, StringArray* step) = 0;
        virtual DBResult del(string name) = 0;
        virtual DBResult save() = 0;
        virtual DBResult load() = 0;
};
