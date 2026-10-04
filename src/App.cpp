#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include "DB.cpp"
#include "Recipe.cpp"
#include "RecipeArray.cpp"
#include "StringArray.cpp"

using namespace std;

class App
{
private:
    string filePath;
    DB *db;

    void commandParser(string command)
    {
        string action;
        string argument;

        size_t pos = command.find(' ');

        if (pos == string::npos)
        {
            action = command;
        }
        else
        {
            action = command.substr(0, pos);
            argument = command.substr(pos + 1);
        }

        if (action == "insert")
        {
            // when there is blank space in front of the argument.
            size_t start = 0;

            while (start < argument.length())
            {
                if (argument[start] != ' ' && argument[start] != '\t')
                {
                    break;
                }

                start++;
            }

            argument = argument.substr(start);

            // when there is blank space in the back of the argument.
            size_t end = argument.length();

            while (end > 0)
            {
                if (argument[end - 1] != ' ' && argument[end - 1] != '\t')
                {
                    break;
                }

                end--;
            }

            argument = argument.substr(0, end);

            // when the argument is empty.
            if (argument.empty())
            {
                cout << "Please enter a recipe name." << endl;
                return;
            }

            // when the argument is not empty.
            string ingredientLine;

            cout << "Ingredients: ";

            if (!getline(cin, ingredientLine))
            {
                return;
            }

            if (!ingredientLine.empty())
            {
                if (ingredientLine[ingredientLine.length() - 1] == ',')
                {
                    cout << "Ingredient names cannot be empty." << endl;
                    return;
                }
            }

            StringArray *ingredients = new StringArray();
            istringstream ingredientReader(ingredientLine);
            string ingredient;

            while (getline(ingredientReader, ingredient, ',')){
                size_t start = 0;

                while (start < ingredient.length()){
                    if (ingredient[start] != ' ' && ingredient[start] != '\t'){
                        break;
                    }
                    start++;
                }

                ingredient = ingredient.substr(start);
                size_t end = ingredient.length();

                while (end > 0){
                    if (ingredient[end - 1] != ' ' && ingredient[end - 1] != '\t'){
                        break;
                    }
                    end--;
                }

                ingredient = ingredient.substr(0, end);

                if (ingredient.empty()){
                    cout << "Ingredient names cannot be empty." << endl;
                    delete ingredients;
                    return;
                }
                ingredients->add(ingredient);
            }

            if (ingredients->size() == 0){
                cout << "Please enter at least one ingredient." << endl;
                delete ingredients;
                return;
            }

            StringArray *steps = new StringArray();
            string stepLine;

            cout << "Enter recipe steps (enter '0' to finish):" << endl;

            while (true){
                if (!getline(cin, stepLine)){
                    delete steps;
                    delete ingredients;
                    return;
                }

                if (stepLine == "0"){
                    break;
                }
                steps -> add(stepLine);
            }

            cout << "================================" << endl;
            cout << "Name: " << argument << endl;

            cout << "Ingredients: " << endl;

            for (int i = 0; i < ingredients->size(); i++)
            {
                cout << "- " << ingredients->get(i) << endl;
            }

            cout << "Steps: " << endl;

            for (int i = 0; i < steps->size(); i++)
            {
                cout << i + 1 << ". " << steps->get(i) << endl;
            }

            cout << "================================" << endl;

            string answer;
            bool saveRequested = false;

            while (true)
            {
                cout << "Save this recipe? [Y/N] : ";

                if (!getline(cin, answer))
                {
                    break;
                }

                if (answer == "Y" || answer == "y")
                {
                    saveRequested = true;
                    break;
                }

                if (answer == "N" || answer == "n")
                {
                    break;
                }

                cout << "Please enter Y or N." << endl;
            }

            if (saveRequested)
            {
                // save the recipe to the database
                // 보류.
            }

            delete steps;
            delete ingredients;
        }

        else if (action == "search")
        {
            string searchOption;
            string searchKeyword;

            istringstream searchStream(argument);
            searchStream >> searchOption;

            if (searchOption.empty())
            {
                // 전체 조회?
                // DB 전체 조회 요청 및 결과 출력
            }
            else if (searchOption == "-name" || searchOption == "-ingre")
            {
                getline(searchStream, searchKeyword);

                size_t start = 0;

                while (start < searchKeyword.length())
                {
                    if (searchKeyword[start] != ' ' && searchKeyword[start] != '\t')
                    {
                        break;
                    }

                    start++;
                }

                searchKeyword = searchKeyword.substr(start);

                size_t end = searchKeyword.length();

                while (end > 0)
                {
                    if (searchKeyword[end - 1] != ' ' && searchKeyword[end - 1] != '\t')
                    {
                        break;
                    }

                    end--;
                }

                searchKeyword = searchKeyword.substr(0, end);

                if (searchKeyword.empty())
                {
                    cout << "Please enter a search keyword." << endl;
                    return;
                }

                size_t length = searchKeyword.length();

                if (length >= 2)
                {
                    if (searchKeyword[0] == '"' && searchKeyword[length - 1] == '"')
                    {
                        searchKeyword = searchKeyword.substr(1, length - 2);
                    }
                }

                if (searchKeyword.empty()){
                    cout << "Please enter a search keyword." << endl;
                    return;
                }

                if (searchOption == "-name"){
                    RecipeArray *results = db->search(searchKeyword);

                    if (results == 0){
                        cout << "No search results found." << endl;
                        return;
                    }

                    cout << "Total " << results->size() << " results:" << endl;
                    print_recipe(results);
                }

                else if (searchOption == "-ingre"){
                    StringArray ingredients;
                    ingredients.add(searchKeyword);

                    RecipeArray *results = db->search(&ingredients);

                    if (results == 0){
                        cout << "No search results found." << endl;
                        return;
                    }
                    cout << "Total " << results->size() << " results:" << endl;
                    print_recipe(results);
                }
            }
            else
            {
                cout << "Unsupported search option." << endl;
            }
        }

        else if (action == "sort")
        {
            string sortOption;

            istringstream sortStream(argument);
            sortStream >> sortOption;

            if (sortOption.empty())
            {
                cout << "Please enter a sort option." << endl;
                return;
            }

            if (sortOption == "name")
            {
                // DB에 이름 기준 정렬 요청
                // 반환된 결과 출력
            }
            else
            {
                cout << "Unsupported sort option." << endl;
            }
        }

        else
        {
            cout << "Unsupported command." << endl;
        }
    }

    void print_recipe(RecipeArray *recipe){
        for (int i = 0; i < recipe->size(); i++){
            Recipe *current = recipe->get(i);

            cout << "===============================" << endl;
            cout << "Recipe Name: " << current->getRecipeName() << endl;

            StringArray *ingredients = current->getIngredient();

            cout << "Ingredients: " << endl;

            for (int j = 0; j < ingredients->size(); j++){
                cout << "- " << ingredients->get(j) << endl;
            }

            StringArray *steps = current->getStep();
            cout << "Steps: " << endl;

            for (int j = 0; j < steps->size(); j++){
                cout << j + 1 << ". " << steps->get(j) << endl;
            }

            cout << "===============================" << endl;
        }
    }

public:
    App(DB *database, string path)
        : filePath(path), db(database)
    {
    }

    void run()
    {
        string command;

        while (true)
        {
            cout << ">>> ";

            if (!getline(cin, command))
            {
                break;
            }

            if (command == "exit")
            {
                break;
            }

            if (command == "")
            {
                continue;
            }

            commandParser(command);
        }
    }
};
