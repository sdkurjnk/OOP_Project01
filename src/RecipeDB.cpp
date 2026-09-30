#include <string>
#include <Recipe.cpp>

using namespace std;

class RecipeDB
{
    private:
        Recipe* recipes;
    
    public:
        void edit(string name);

        Recipe* searchByName(string name);

        Recipe* searchByName(string ingre);

        void add(string name, string* ingredent, string* step);

        void del(string name);

        Recipe* order(Recipe* recipe, int option);
        
        void load(string fileName); //fileName은 임시로 string으로 둠.
};