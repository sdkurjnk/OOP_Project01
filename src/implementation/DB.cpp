#include "../header/DB.h"

using namespace std;

Response::Response(RecipeArray* array, DBResult result){
    this->recipeArray = array;
    this->responseResult = result;
}

Response::~Response() {delete this->recipeArray;}

RecipeArray* Response::getArray() {return this->recipeArray;}

DBResult Response::getResult() {return this->responseResult;}
