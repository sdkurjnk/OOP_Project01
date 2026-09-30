#pragma once

#include <string>
#include <iostream>
#include "DB.cpp"
#include "Recipe.cpp"

class App
{
    private:
        string filePath;
        DB* db;

        void commandParser(string command);

        void print_recipe(Recipe* recipe);

    public:
        App(DB* db, string filePath);

        void run();
};