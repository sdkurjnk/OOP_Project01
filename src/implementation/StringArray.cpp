#include "../header/StringArray.h"

using namespace std;

StringArray::StringArray(){
    this->str = NULL;
    this->count = 0;
}

StringArray::~StringArray() {delete[] this->str;}

void StringArray::add(string item){
    string* newArray = new string[this->count + 1];
    for (int i = 0; i < this->count; i++){
        newArray[i] = this->str[i];
    }
    newArray[this->count] = item;

    delete[] this->str;
    this->str = newArray;
    this->count = this->count + 1;
}

int StringArray::size() {return this->count;}

string StringArray::get(int index){
    if (index < 0 || index >= this->count) {return "";}
    return this->str[index];
}
