#include <string>
#include <iostream>
#include "App.cpp"
#include "RecipeDB.cpp"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2){
        cout << "Please enter filePath like: main.exe file.txt" << endl; 
        return 1;
    }

    string filePath = argv[1];

    RecipeDB recipeDB(filePath);
    App app(&recipeDB, argv[1]);
    app.run();
    return 0;
}