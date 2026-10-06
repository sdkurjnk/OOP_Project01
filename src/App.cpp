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

    string trim(string s)
    {
        size_t start = 0;

        while (start < s.length())
        {
            if (s[start] != ' ' && s[start] != '\t')
            {
                break;
            }

            start++;
        }

        s = s.substr(start);

        size_t end = s.length();

        while (end > 0)
        {
            if (s[end - 1] != ' ' && s[end - 1] != '\t')
            {
                break;
            }

            end--;
        }

        return s.substr(0, end);
    }

    string stripQuotes(string s)
    {
        size_t length = s.length();

        if (length >= 2 && s[0] == '"' && s[length - 1] == '"'){
            return s.substr(1, length - 2);
        }

        return s;
    }

    // Tokenize a command line into [action, (option), argument(s)...].
    // Grammar:
    //   insert <name>           -> [ "insert", name ]
    //   sort <option>           -> [ "sort", option ]
    //   search                  -> [ "search" ]
    //   search -name <keyword>  -> [ "search", "-name", keyword ]
    //   search -ingre a, b, c   -> [ "search", "-ingre", "a", "b", "c" ]
    // The returned StringArray is owned by the caller (run()).
    StringArray* commandParser(string command){
        StringArray *tokens = new StringArray();

        string action;
        string rest;

        size_t pos = command.find(' ');

        if (pos == string::npos) {action = command;}
        else{
            action = command.substr(0, pos);
            rest = command.substr(pos + 1);
        }

        tokens->add(action);

        if (action == "insert"){
            // The whole remainder is the recipe name (spaces allowed).
            string name = trim(rest);

            if (!name.empty()) {tokens->add(name);}
        }
        else if (action == "sort"){
            // Only the next word matters.
            istringstream sortStream(rest);
            string option;
            sortStream >> option;

            if (!option.empty()) {tokens->add(option);}
        }
        else if (action == "search") {
            istringstream searchStream(rest);
            string option;
            searchStream >> option;

            if (!option.empty()) {
                tokens->add(option);

                // Everything after the option word.
                string after;
                getline(searchStream, after);
                after = trim(after);

                if (option == "-name"){
                    after = stripQuotes(after);

                    if (!after.empty()) {tokens->add(after);}
                }
                else if (option == "-ingre"){
                    // Split the remainder into ingredients by comma.
                    istringstream ingredientReader(after);
                    string ingredient;

                    while (getline(ingredientReader, ingredient, ',')){
                        ingredient = trim(ingredient);

                        if (!ingredient.empty()) {tokens->add(ingredient);}
                    }
                }
            }
        }

        return tokens;
    }

    void commandCaller(StringArray *tokens)
    {
        string action = tokens->get(0);

        if (action == "insert"){
            if (tokens->size() < 2){
                cout << "Please enter a recipe name." << endl;
                return;
            }

            string name = tokens->get(1);

            string ingredientLine;

            cout << "Ingredients: ";

            if (!getline(cin, ingredientLine)) {return;}

            if (!ingredientLine.empty()){
                if (ingredientLine[ingredientLine.length() - 1] == ','){
                    cout << "Ingredient names cannot be empty." << endl;
                    return;
                }
            }

            StringArray *ingredients = new StringArray();
            istringstream ingredientReader(ingredientLine);
            string ingredient;

            while (getline(ingredientReader, ingredient, ',')){
                ingredient = trim(ingredient);

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

                if (stepLine == "0") {break;}

                steps->add(stepLine);
            }

            StringArray *previewIngredients = new StringArray();
            StringArray *previewSteps = new StringArray();

            for (int i = 0; i < ingredients->size(); i++){
                previewIngredients->add(ingredients->get(i));
            }

            for (int i = 0; i < steps->size(); i++){
                previewSteps->add(steps->get(i));
            }

            Recipe preview(name, previewIngredients, previewSteps);
            RecipeArray previewList;

            previewList.add(&preview);
            print_recipe(&previewList);

            string answer;
            bool saveRequested = false;

            while (true){
                cout << "Save this recipe? [Y/N] : ";

                if (!getline(cin, answer)) {break;}

                if (answer == "Y" || answer == "y"){
                    saveRequested = true;
                    break;
                }

                if (answer == "N" || answer == "n") {break;}

                cout << "Please enter Y or N." << endl;
            }

            if (saveRequested){
                // db->add owns ingredients/steps from here (and frees them on failure).
                DBResult result = db->add(name, ingredients, steps);

                if (result == DB_OK) {cout << "Recipe saved." << endl;}
                else if (result == DB_DUPLICATE){
                    cout << "A recipe with that name already exists." << endl;
                }
                else {cout << "Recipe was not saved." << endl;}
            }
            else{
                delete steps;
                delete ingredients;
                cout << "Recipe was not saved." << endl;
            }
        }
        else if (action == "search"){
            if (tokens->size() == 1){
                // No keyword: list every recipe (ordered by name ascending).
                show_results(db->order(0), "Search failed.");
                return;
            }

            string option = tokens->get(1);

            if (option == "-name"){
                if (tokens->size() < 3){
                    cout << "Please enter a search keyword." << endl;
                    return;
                }

                show_results(db->search(tokens->get(2)), "Search failed.");
            }
            else if (option == "-ingre"){
                if (tokens->size() < 3){
                    cout << "Please enter a search keyword." << endl;
                    return;
                }

                StringArray ingredients;

                for (int i = 2; i < tokens->size(); i++){
                    ingredients.add(tokens->get(i));
                }

                show_results(db->search(&ingredients), "Search failed.");
            }
            else{cout << "Unsupported search option." << endl;}
        }
        else if (action == "sort"){
            if (tokens->size() < 2){
                cout << "Please enter a sort option." << endl;
                return;
            }

            string option = tokens->get(1);

            if (option == "name"){show_results(db->order(0), "Sort failed.");}
            else{cout << "Unsupported sort option." << endl;}
        }
        else {cout << "ERROR: Unsupported command." << endl;}}

    void print_recipe(RecipeArray *recipe){
        for (int i = 0; i < recipe->size(); i++){
            Recipe *current = recipe->get(i);
            StringArray lines;

            lines.add("Recipe Name: " + current->getRecipeName());

            StringArray *ingredients = current->getIngredient();
            string ingredientLine = "Ingredient: ";

            for (int j = 0; j < ingredients->size(); j++){
                if (j > 0) { ingredientLine += ", ";}
                ingredientLine += ingredients->get(j);
            }

            lines.add(ingredientLine);

            lines.add("Steps:");
            StringArray *steps = current->getStep();

            for (int j = 0; j < steps->size(); j++){
                lines.add(to_string(j + 1) + ". " + steps->get(j));
            }

            size_t maxLength = 0;

            for (int j = 0; j < lines.size(); j++){
                string line = lines.get(j);

                if (line.length() > maxLength) {maxLength = line.length();}
            }

            string border;

            for (size_t j = 0; j < maxLength; j++) {border += "=";}

            cout << border << endl;

            for (int j = 0; j < lines.size(); j++) {cout << lines.get(j) << endl;}

            cout << border << endl;
        }
    }

    void show_results(Response *response, string failMessage){
        if (response == 0 || response->getResult() != DB_OK){
            cout << failMessage << endl;
            delete response;
            return;
        }

        RecipeArray *results = response->getArray();

        cout << "Total " << results->size() << " results:" << endl;
        print_recipe(results);
        delete response;
    }

public:
    App(DB *database, string path){
        this->filePath = path;
        this->db = database;
    }

    void run(){
        string command;

        while (true){
            cout << ">>> ";

            if (!getline(cin, command)) {break;}

            if (command == "exit") {break;}

            if (command == "") {continue;}

            StringArray *tokens = commandParser(command);
            commandCaller(tokens);
            delete tokens;
        }
    }
};
