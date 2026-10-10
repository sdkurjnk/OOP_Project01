#include <string>
#include <iostream>
#include "header/App.h"
#include "header/RecipeDB.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2){
        cout << "Please enter filePath like: main.exe file.txt" << endl;
        return 1;
    }

    string filePath = argv[1];

    RecipeDB recipeDB(filePath);
    App app(&recipeDB);
    app.run();
    return 0;
}
