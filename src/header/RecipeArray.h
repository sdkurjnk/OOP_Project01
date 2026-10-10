#pragma once

#include "Recipe.h"

using namespace std;

class RecipeArray
{
    private:
        Recipe** recipe; //array of Recipe instances
        int count;

    public:
        RecipeArray();
        ~RecipeArray();

        void add(Recipe* item);
        int size();
        Recipe* get(int index);
};
