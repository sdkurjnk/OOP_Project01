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
            cout << "Please enter a recipe name." << endl;
            return;
        }

        // when the argument is not empty.
        string ingredientLine;

        cout << "Ingredients: ";

        if (!getline(cin, ingredientLine)){
            return;
        }

        istringstream ingredientStream(ingredientLine);
        string ingredient;
        int ingredientCount = 0;

        while (getline(ingredientStream, ingredient, ',')){
            ingredientCount++;
        }

        if (ingredientCount == 0){
            cout << "Please enter at least one ingredient." << endl;
            return;
        }

        // allocate memory for the ingredients array.
        string* ingredients = new string[ingredientCount];
        istringstream ingredientReader(ingredientLine);

        for (int i = 0; i < ingredientCount; i++){
            getline(ingredientReader, ingredients[i], ',');

            size_t start = 0;

            while (start < ingredients[i].length()){
                if (ingredients[i][start] != ' ' && ingredients[i][start] != '\t'){
                    break;
                }

                start++;
            }

            ingredients[i] = ingredients[i].substr(start);

            size_t end = ingredients[i].length();

            while (end > 0){
                if (ingredients[i][end - 1] != ' ' && ingredients[i][end - 1] != '\t'){
                    break;
                }

                end--;
                }

                ingredients[i] = ingredients[i].substr(0, end);
            }


        string stepText; // save all the steps
        string stepLine; // save one line of the step
        int stepCount = 0; // count the number of steps

        cout << "Enter recipe steps (enter '0' to finish): " << endl;

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

        string answer;
        bool saveRequested = false;

        while (true){
            cout << "Save this recipe? [Y/N] : ";

            if (!getline(cin, answer)){
                break;
            }

            if (answer == "Y" || answer == "y"){
                saveRequested = true;
                break;
            }

            if (answer == "N" || answer == "n"){
                break;
            }

            cout << "Please enter Y or N." << endl;
        }

        if (saveRequested){
            // save the recipe to the database
            // 보류.
        }

        delete[] steps;
        delete[] ingredients; 
    }

    else if (action == "search"){
        string searchOption;
        string searchKeyword;

        istringstream searchStream(argument);
        searchStream >> searchOption;

        if (searchOption.empty()){
            // 전체 조회?
            // DB 전체 조회 요청 및 결과 출력
        }
        else if (searchOption == "-name" || searchOption == "-ingre"){
            getline(searchStream, searchKeyword);

            size_t start = 0;

            while (start < searchKeyword.length()){
                if (searchKeyword[start] != ' ' && searchKeyword[start] != '\t'){
                    break;
                }

                start++;
            }

            searchKeyword = searchKeyword.substr(start);

            size_t end = searchKeyword.length();

            while (end > 0){
                if (searchKeyword[end - 1] != ' ' && searchKeyword[end - 1] != '\t'){
                    break;
                }

                end--;
            }

            searchKeyword = searchKeyword.substr(0, end);

            if (searchKeyword.empty()){
                cout << "Please enter a search keyword." << endl;
                return;
            }

            size_t length = searchKeyword.length();

            if (length >= 2){
                if (searchKeyword[0] == '"' && searchKeyword[length - 1] == '"'){
                    searchKeyword = searchKeyword.substr(1, length - 2);
                }
            }

            // DB 연결 코드 들어갈 자리
        }
        else{
            cout << "Unsupported search option."<< endl;
        }
    }

    else if (action == "sort"){
        string sortOption;

        istringstream sortStream(argument);
        sortStream >> sortOption;

        if (sortOption.empty()){
            cout << "Please enter a sort option." << endl;
            return;
        }

        if (sortOption == "name"){
            // DB에 이름 기준 정렬 요청
            // 반환된 결과 출력
        }
        else{
            cout << "Unsupported sort option." << endl;
        }
    }

    else{
        cout << "Unsupported command." << endl;
    }
}