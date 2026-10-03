#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include "DB.cpp"
#include "Recipe.cpp"

using namespace std;

class App
{
    private:
        string filePath;
        DB* db;

        void commandParser(string command);

        void print_recipe(Recipe* recipe);

    public:
        App(DB* db, string filePath);

        void run();
};

App::App(DB* database, string path)
    :filePath(path), db(database)
    {
    }

void App::run(){
    string command;

    while(true){
        cout << ">>> ";

        if(!getline(cin, command)){
            break;
        }

        if (command == "exit"){
            break;
        }

        if (command == ""){
            continue;
        }

        commandParser(command);
    }
}

void App::commandParser(string command){
    string action;
    string argument;

    size_t pos = command.find(' ');

    if (pos == string::npos){
        action = command;
    }
    else{
        action = command.substr(0, pos);
        argument = command.substr(pos + 1);
    }

    if (action == "insert"){
        // when there is blank space in front of the argument.
        size_t start = 0;

        while (start < argument.length()){
            if (argument[start] != ' ' && argument[start] != '\t'){
                break;
            }

            start++;
        }

        argument = argument.substr(start);

        // when there is blank space in the back of the argument.
        size_t end = argument.length();

        while (end > 0){
            if (argument[end - 1] != ' ' && argument[end -1] != '\t'){
                break;
            }

            end--;
        }

        argument = argument.substr(0, end);

        // when the argument is empty.
        if (argument.empty()){
            cout << "레시피 이름을 입력하세요." << endl;
            return;
        }

        // when the argument is not empty.
        string ingredientLine;

        cout << "재료를 입력하세요: ";

        if (!getline(cin, ingredientLine)){
            return;
        }

        istringstream ingredientStream(ingredientLine);
        string ingredient;
        int ingredientCount = 0;

        while (ingredientStream >> ingredient){
            ingredientCount++;
        }

        if (ingredientCount == 0){
            cout << "재료를 하나 이상 입력하세요." << endl;
            return;
        }

        // allocate memory for the ingredients array.
        string* ingredients = new string[ingredientCount];
        istringstream ingredientReader(ingredientLine);

        for (int i = 0; i < ingredientCount; i++){
            ingredientReader >> ingredients[i];
        }

        string stepText; // save all the steps
        string stepLine; // save one line of the step
        int stepCount = 0; // count the number of steps

        cout << "조리 과정을 입력하세요. (종료하려면 0을 입력.)" << endl;

        while (true) {
            if (!getline(cin, stepLine)) {
                delete[] ingredients;
                return;
            }

            if (stepLine == "0"){ // finish inputting steps
                break;
            }

            stepText += stepLine + "\n";
            stepCount++;
        }

        string* steps = new string[stepCount];
        istringstream stepReader(stepText);

        for (int i = 0; i < stepCount; i++){
            getline(stepReader, steps[i]);
        }

        delete[] steps;
        delete[] ingredients; 
    }
}