#include <string>
#include <iostream>
#include <RecipeDB.cpp>
#include <Recipe.cpp>

class App
{
    private:
        string fileName; //임시로 string형 fileName으로 둠.
        RecipeDB recipeDB;

        void print_recipe(Recipe* recipe);

    public:
        void commandParser(string command);
};