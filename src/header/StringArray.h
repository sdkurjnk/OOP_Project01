#pragma once

#include <string>

using namespace std;

class StringArray
{
    public:
        StringArray();
        ~StringArray();

        void add(string item);
        int size();
        string get(int index);

    private:
        string* str; //Array of string values
        int count;
};
