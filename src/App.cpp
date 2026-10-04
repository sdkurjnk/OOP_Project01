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

            RecipeArray *existing = db->search(argument);

            if (existing == 0){
                cout << "Could not check the recipe name." << endl;
                return;
            }

            bool duplicate = false;

            for (int i = 0; i < existing->size(); i++){
                if (existing->get(i)->getRecipeName() == argument){
                    duplicate = true;
                    break;
                }
            }

            delete existing;

            if (duplicate){
                cout << "A recipe with that name already exists." << endl;
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
        
            StringArray *previewIngredients = new StringArray();
            StringArray *previewSteps = new StringArray();

            for (int i = 0; i < ingredients->size(); i++){
                previewIngredients->add(ingredients->get(i));
            }

            for (int i = 0; i < steps->size(); i++){
                previewSteps->add(steps->get(i));
            }

            Recipe preview(argument, previewIngredients, previewSteps);
            RecipeArray previewList;

            previewList.add(&preview);
            print_recipe(&previewList);
            

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

            if (saveRequested){
                db->add(argument, ingredients, steps);
            }
            else{
                delete steps;
                delete ingredients;
                cout << "Recipe was not saved." << endl;
            }
        }

        else if (action == "search")
        {
            string searchOption;
            string searchKeyword;

            istringstream searchStream(argument);
            searchStream >> searchOption;

            if (searchOption.empty())
            {
                RecipeArray *results = db->search("");

                if (results == 0){
                    cout << "Search failed." << endl;
                    return;
                }

                cout << "Total " << results->size() << " results:" << endl;
                print_recipe(results);
                delete results;
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
                        cout << "Search failed." << endl;
                        return;
                    }

                    cout << "Total " << results->size() << " results:" << endl;
                    print_recipe(results);
                    delete results;
                }

                else if (searchOption == "-ingre"){
                    StringArray ingredients;
                    ingredients.add(searchKeyword);

                    RecipeArray *results = db->search(&ingredients);

                    if (results == 0){
                        cout << "Search failed." << endl;
                        return;
                    }
                    cout << "Total " << results->size() << " results:" << endl;
                    print_recipe(results);
                    delete results;
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
                RecipeArray *results = db->order(0);

                if (results == 0){
                    cout << "Sort failed." << endl;
                    return;
                }

                cout << "Total " << results->size() << " results:" << endl;
                print_recipe(results);
                delete results;
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
            StringArray lines;

            lines.add("Recipe Name: " + current->getRecipeName());
            lines.add("Ingredients:");

            StringArray *ingredients = current->getIngredient();

            for (int j = 0; j < ingredients->size(); j++){
                lines.add("- " + ingredients->get(j));
            }

            lines.add("Steps:");
            StringArray *steps = current->getStep();

            for (int j = 0; j < steps->size(); j++){
                lines.add(to_string(j + 1) + ". " + steps->get(j));
            }

            size_t maxLength = 0;

            for (int j = 0; j < lines.size(); j++){
                string line = lines.get(j);

                if (line.length() > maxLength){
                    maxLength = line.length();
                }
            }

            string border;

            for (size_t j = 0; j < maxLength; j++){
                border += "=";
            }

            cout << border << endl;

            for (int j = 0; j < lines.size(); j++){
                cout << lines.get(j) << endl;
            }

            cout << border << endl;
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
