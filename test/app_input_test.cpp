#include "../src/App.cpp"
#include "../src/RecipeDB.cpp"

int main()
{
    RecipeDB recipeDB("test_recipes.txt");
    App app(&recipeDB, "test_recipes.txt");
    app.run();

    return 0;
}