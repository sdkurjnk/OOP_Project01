#include <string>

using namespace std;

class Recipe
{
    private:
        string recipeName;
        string* ingredent;
        string* step;
    
    public:
        string* getData();

        string getRecipeName();

        string* getIngredent();

        string* getStep();
};