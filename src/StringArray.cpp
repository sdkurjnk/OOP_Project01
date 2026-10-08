#pragma once

#include <string>

using namespace std;

class StringArray
{
    private:
        string* str; //Array of string values
        int count;

    public:
        StringArray(){
            this->str = NULL;
            this->count = 0;
        }

        ~StringArray() {delete[] this->str;}

        void add(string item){
            string* newArray = new string[this->count + 1];
            for (int i = 0; i < this->count; i++){
                newArray[i] = this->str[i];
            }
            newArray[this->count] = item;

            delete[] this->str;
            this->str = newArray;
            this->count = this->count + 1;
        }

        int size() {return this->count;}

        string get(int index){
            if (index < 0 || index >= this->count) {return "";}
            return this->str[index];
        }
};
