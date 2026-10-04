#include <iostream>
#include <string>
using namespace std;

int main()
{
    string lines[] = {
        "Recipe Name: Salmon",
        "Ingredients:",
        "- salmon",
        "- olive oil",
        "Steps:",
        "1. Prepare salmon.",
        "2. Add olive oil.",
        "3. Cook until the salmon is ready."
    };

    int lineCount = 8;
    size_t maxLength = 0;

    for (int i = 0; i < lineCount; i++)
    {
        if (lines[i].length() > maxLength)
        {
            maxLength = lines[i].length();
        }
    }

    string border;

    for (size_t i = 0; i < maxLength; i++)
    {
        border += "=";
    }

    cout << border << endl;

    for (int i = 0; i < lineCount; i++)
    {
        cout << lines[i] << endl;
    }

    cout << border << endl;
    cout << "Border length: " << maxLength << endl;

    return 0;
}