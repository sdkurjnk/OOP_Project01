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
        DB *db;

        string trim(string s){
            int start = 0;

            while (start < (int)s.length()){
                if (s[start] != ' ' && s[start] != '\t') {break;}

                start++;
            }

            s = s.substr(start);

            int end = s.length();

            while (end > 0){
                if (s[end - 1] != ' ' && s[end - 1] != '\t') {break;}

                end--;
            }

            return s.substr(0, end);
        }

        string stripQuotes(string s){
            int length = s.length();

            if (length >= 2 && s[0] == '"' && s[length - 1] == '"'){
                return s.substr(1, length - 2);
            }

            return s;
        }

        StringArray* commandParser(string command){
            StringArray *tokens = new StringArray();

            string action;
            string rest;

            int pos = command.find(' ');

            if (pos == (int)string::npos) {action = command;}
            else{
                action = command.substr(0, pos);
                rest = command.substr(pos + 1);
            }

            tokens->add(action);

            if (action == "insert" || action == "edit" || action == "del"){
                // The whole remainder is the recipe name (spaces allowed).
                string name = trim(rest);

                if (!name.empty()) {tokens->add(name);}
            }
            else if (action == "sort"){
                // sort <key> [asc|desc]
                istringstream sortStream(rest);
                string key;
                sortStream >> key;

                if (!key.empty()){
                    tokens->add(key);

                    string direction;
                    sortStream >> direction;

                    if (!direction.empty()) {tokens->add(direction);}
                }
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
                    cout << "ERROR: Please enter a recipe name." << endl;
                    return;
                }

                string name = tokens->get(1);

                string ingredientLine;

                cout << "Ingredients: ";

                if (!getline(cin, ingredientLine)) {return;}

                if (!ingredientLine.empty()){
                    if (ingredientLine[ingredientLine.length() - 1] == ','){
                        cout << "ERROR: Ingredient names cannot be empty." << endl;
                        return;
                    }
                }

                StringArray *ingredients = new StringArray();
                istringstream ingredientReader(ingredientLine);
                string ingredient;

                while (getline(ingredientReader, ingredient, ',')){
                    ingredient = trim(ingredient);

                    if (ingredient.empty()){
                        cout << "ERROR: Ingredient names cannot be empty." << endl;
                        delete ingredients;
                        return;
                    }

                    ingredients->add(ingredient);
                }

                if (ingredients->size() == 0){
                    cout << "ERROR: Please enter at least one ingredient." << endl;
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

                    if (result == DB_OK) {cout << "INFO: Recipe saved." << endl;}
                    else if (result == DB_DUPLICATE){
                        cout << "ERROR: A recipe with that name already exists." << endl;
                    }
                    else {cout << "ERROR: Recipe was not saved." << endl;}
                }
                else{
                    delete steps;
                    delete ingredients;
                    cout << "INFO: Recipe was not saved." << endl;
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
                        cout << "ERROR: Please enter a search keyword." << endl;
                        return;
                    }

                    show_results(db->search(tokens->get(2)), "Search failed.");
                }
                else if (option == "-ingre"){
                    if (tokens->size() < 3){
                        cout << "ERROR: Please enter a search keyword." << endl;
                        return;
                    }

                    StringArray ingredients;

                    for (int i = 2; i < tokens->size(); i++){
                        ingredients.add(tokens->get(i));
                    }

                    show_results(db->search(&ingredients), "Search failed.");
                }
                else{cout << "ERROR: Unsupported search option." << endl;}
            }
            else if (action == "sort"){
                if (tokens->size() < 2){
                    cout << "ERROR: Please enter a sort option." << endl;
                    return;
                }

                string key = tokens->get(1);

                if (key != "name"){
                    cout << "ERROR: Unsupported sort option." << endl;
                    return;
                }

                int option = 0; // ascending by default

                if (tokens->size() >= 3){
                    string direction = tokens->get(2);

                    if (direction == "desc") {option = 1;}
                    else if (direction == "asc") {option = 0;}
                    else{
                        cout << "ERROR: Unsupported sort option." << endl;
                        return;
                    }
                }

                show_results(db->order(option), "Sort failed.");
            }
            else if (action == "edit"){
                if (tokens->size() < 2){
                    cout << "ERROR: Please enter a recipe name." << endl;
                    return;
                }

                string name = tokens->get(1);

                // Empty input keeps the current field.
                cout << "New name (leave empty to keep): ";
                string newName;

                if (!getline(cin, newName)) {return;}

                newName = trim(newName);

                cout << "New ingredients (comma-separated, leave empty to keep): ";
                string ingredientLine;

                if (!getline(cin, ingredientLine)) {return;}

                StringArray *newIngre = NULL;
                string trimmedIngre = trim(ingredientLine);

                if (!trimmedIngre.empty()){
                    if (trimmedIngre[trimmedIngre.length() - 1] == ','){
                        cout << "ERROR: Ingredient names cannot be empty." << endl;
                        return;
                    }

                    newIngre = new StringArray();
                    istringstream ingredientReader(ingredientLine);
                    string ingredient;

                    while (getline(ingredientReader, ingredient, ',')){
                        ingredient = trim(ingredient);

                        if (ingredient.empty()){
                            cout << "ERROR: Ingredient names cannot be empty." << endl;
                            delete newIngre;
                            return;
                        }

                        newIngre->add(ingredient);
                    }
                }

                StringArray *newStep = NULL;
                string answer;

                cout << "Replace steps? [Y/N] : ";

                if (!getline(cin, answer)){
                    delete newIngre;
                    return;
                }

                if (answer == "Y" || answer == "y"){
                    newStep = new StringArray();
                    string stepLine;

                    cout << "Enter recipe steps (enter '0' to finish):" << endl;

                    while (true){
                        if (!getline(cin, stepLine)){
                            delete newStep;
                            delete newIngre;
                            return;
                        }

                        if (stepLine == "0") {break;}

                        newStep->add(stepLine);
                    }
                }

                // db->edit owns newIngre/newStep from here (and frees them on failure).
                DBResult result = db->edit(name, newName, newIngre, newStep);

                if (result == DB_OK) {cout << "INFO: Recipe updated." << endl;}
                else if (result == DB_NOT_FOUND){
                    cout << "ERROR: No recipe with that name." << endl;
                }
                else if (result == DB_EMPTY_NAME){
                    cout << "ERROR: Please enter a recipe name." << endl;
                }
                else {cout << "ERROR: Edit failed." << endl;}
            }
            else if (action == "del"){
                if (tokens->size() < 2){
                    cout << "ERROR: Please enter a recipe name." << endl;
                    return;
                }

                string name = tokens->get(1);

                string answer;
                bool deleteRequested = false;

                while (true){
                    cout << "Delete this recipe? [Y/N] : ";

                    if (!getline(cin, answer)) {return;}

                    if (answer == "Y" || answer == "y"){
                        deleteRequested = true;
                        break;
                    }

                    if (answer == "N" || answer == "n") {break;}

                    cout << "Please enter Y or N." << endl;
                }

                if (!deleteRequested){
                    cout << "INFO: Recipe was not deleted." << endl;
                    return;
                }

                DBResult result = db->del(name);

                if (result == DB_OK) {cout << "INFO: Recipe deleted." << endl;}
                else if (result == DB_NOT_FOUND){
                    cout << "ERROR: No recipe with that name." << endl;
                }
                else if (result == DB_EMPTY_NAME){
                    cout << "ERROR: Please enter a recipe name." << endl;
                }
                else {cout << "ERROR: Delete failed." << endl;}
            }
            else if (action == "load"){
                DBResult result = db->load();

                if (result == DB_OK) {cout << "INFO: Recipes loaded." << endl;}
                else if (result == DB_FILE_NOT_FOUND){
                    cout << "ERROR: No saved file to load." << endl;
                }
                else {cout << "ERROR: Load failed." << endl;}
            }
            else if (action == "save"){
                DBResult result = db->save();

                if (result == DB_OK) {cout << "INFO: Recipes saved." << endl;}
                else if (result == DB_SAVE_FAILED){
                    cout << "ERROR: Could not open the file for writing." << endl;
                }
                else {cout << "ERROR: Save failed." << endl;}
            }
            else if (action == "help"){
                StringArray *lines = db->help();

                for (int i = 0; i < lines->size(); i++){
                    cout << lines->get(i) << endl;
                }

                delete lines;
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

                int maxLength = 0;

                for (int j = 0; j < lines.size(); j++){
                    string line = lines.get(j);

                    if ((int)line.length() > maxLength) {maxLength = line.length();}
                }

                string border;

                for (int j = 0; j < maxLength; j++) {border += "=";}

                cout << border << endl;

                for (int j = 0; j < lines.size(); j++) {cout << lines.get(j) << endl;}

                cout << border << endl;
            }
        }

        void show_results(Response *response, string failMessage){
            if (response == NULL || response->getResult() != DB_OK){
                cout << "ERROR: " << failMessage << endl;
                delete response;
                return;
            }

            RecipeArray *results = response->getArray();

            cout << "Total " << results->size() << " results:" << endl;
            print_recipe(results);
            delete response;
        }

    public:
        App(DB *database){
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
