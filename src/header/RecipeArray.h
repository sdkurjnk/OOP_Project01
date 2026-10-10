#pragma once

#include "Recipe.h"

using namespace std;

class RecipeArray
{
    public:
        RecipeArray();
        ~RecipeArray();

        void add(Recipe* item);
        int size();
        Recipe* get(int index);

    private:
        Recipe** recipe; //array of Recipe instances
        int count;
};
