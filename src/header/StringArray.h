#pragma once

#include <string>

using namespace std;

class StringArray
{
    private:
        string* str; //Array of string values
        int count;

    public:
        StringArray();
        ~StringArray();

        void add(string item);
        int size();
        string get(int index);
};
