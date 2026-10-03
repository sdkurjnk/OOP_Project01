#pragma once

#include "Recipe.cpp"

using namespace std;

class RecipeArray
{
    private:
        Recipe** recipe;
        int count;

    public:
        RecipeArray();
        ~RecipeArray();

        void add(Recipe* item);
        int size();
        Recipe* get(int index);
};
