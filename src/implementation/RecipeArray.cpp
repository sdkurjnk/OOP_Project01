#include "../header/RecipeArray.h"

using namespace std;

RecipeArray::RecipeArray(){
    this->recipe = NULL;
    this->count = 0;
}

RecipeArray::~RecipeArray() {delete[] this->recipe;} //deallocate only array of Recipe pointers, not the Recipe instances

void RecipeArray::add(Recipe* item){
    Recipe** newArray = new Recipe*[this->count + 1];
    for (int i = 0; i < this->count; i++) {newArray[i] = this->recipe[i];}
    newArray[this->count] = item;

    delete[] this->recipe;
    this->recipe = newArray;
    this->count = this->count + 1;
}

int RecipeArray::size() {return this->count;}

Recipe* RecipeArray::get(int index){
    if (index < 0 || index >= this->count) {return NULL;}
    return this->recipe[index];
}
