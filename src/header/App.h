#pragma once

#include <string>
#include "DB.h"
#include "Recipe.h"
#include "RecipeArray.h"
#include "StringArray.h"

using namespace std;

class App
{
    public:
        App(DB *database);
        void run();

    private:
        DB *db;

        string trim(string s);
        string stripQuotes(string s);
        StringArray* commandParser(string command);
        void commandCaller(StringArray *tokens);

        void handleInsert(StringArray *tokens);
        void handleSearch(StringArray *tokens);
        void handleSort(StringArray *tokens);
        void handleEdit(StringArray *tokens);
        void handleDel(StringArray *tokens);
        void handleLoad();
        void handleSave();
        void handleHelp();

        void print_recipe(RecipeArray *recipe);
        void show_results(Response *response, string failMessage);
};
