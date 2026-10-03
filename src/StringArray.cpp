#pragma once

#include <string>

using namespace std;

class StringArray
{
    private:
        string* str;
        int count;

    public:
        StringArray();
        ~StringArray();

        void add(string item);
        int size();
        string get(int index);
};
