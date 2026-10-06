#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "RecipeArray.cpp"
#include "StringArray.cpp"
#include "Recipe.cpp"
#include "DB.cpp"

using namespace std;

class RecipeDB : public DB
{
    private:
        RecipeArray* recipes;
        string filePath;

        int findIndex(string name){
            for (int i = 0; i < this->recipes->size(); i++){
                if (this->recipes->get(i)->getRecipeName() == name) {return i;}
            }

            return -1;
        }

        string pad2(int number){
            string s = to_string(number);

            if (s.length() < 2) {s = "0" + s;}

            return s;
        }

        string makeBorder(int width){
            string bar = "";

            for (int i = 0; i < width; i++){
                bar += "=";
            }

            return bar;
        }

        bool isBorder(string line){
            if (line.empty()) {return false;}

            for (int i = 0; i < line.length(); i++){
                if (line[i] != '=') {return false;}
            }

            return true;
        }

        DBResult save(){
            ofstream out(this->filePath.c_str()); // overwrite the whole file

            if (!out.is_open()) {return DB_SAVE_FAILED;}

            for (int i = 0; i < this->recipes->size(); i++){
                Recipe* r = this->recipes->get(i);

                StringArray* ingre = r->getIngredient();
                StringArray* step = r->getStep();

                //1. make every line as string
                //layout: [0] name, [1] ingredient, [2] "Step:", [3..] step lines
                int lineCount = 3 + step->size();
                string* lines = new string[lineCount];

                lines[0] = "Name: " + r->getRecipeName();

                string ingreLine = "Ingredient: ";
                for (int j = 0; j < ingre->size(); j++){
                    if (j > 0) {ingreLine += ", ";}
                    ingreLine += ingre->get(j);
                }
                lines[1] = ingreLine;

                lines[2] = "Step:";

                for (int j = 0; j < step->size(); j++){
                    lines[3 + j] = pad2(j + 1) + ": " + step->get(j);
                }

                //2. border width = max length among the card lines
                int width = 0;

                for (int j = 0; j < lineCount; j++){
                    if ((int)lines[j].length() > width) {width = (int)lines[j].length();}
                }

                string bar = makeBorder(width);

                //3. write
                out << bar << "\n";

                for (int j = 0; j < lineCount; j++) {out << lines[j] << "\n";}

                out << bar << "\n";

                delete[] lines;
            }

            out.close();
            return DB_OK;
        }

        DBResult load(){
            ifstream in(this->filePath.c_str());

            //No file: start with an empty database.
            if (!in.is_open()) {
                return DB_FILE_NOT_FOUND;
            }

            string line;

            //Fields of the card
            string name = "";
            bool hasName = false;
            StringArray* ingre = NULL;
            StringArray* steps = NULL;

            while (getline(in, line)){
                if (!line.empty() && line[line.length() - 1] == '\r'){
                    line = line.substr(0, line.length() - 1);
                }

                //blank lines carry no data - skip
                if (line.empty()) {continue;}

                if (isBorder(line)){ //making card and store to this->recipes
                    if (!name.empty()){
                        if (ingre == NULL) {ingre = new StringArray();}
                        if (steps == NULL) {steps = new StringArray();}

                        this->recipes->add(new Recipe(name, ingre, steps));
                    }
                    else if (hasName || ingre != NULL || steps != NULL){
                        delete ingre;
                        delete steps;
                    }

                    //reset for the next card
                    name = "";
                    hasName = false;
                    ingre = NULL;
                    steps = NULL;
                    continue;
                }

                if (line.compare(0, 6, "Name: ") == 0){ //name field
                    name = line.substr(6);
                    hasName = true;
                }
                else if (line.compare(0, 12, "Ingredient: ") == 0){ //ingredient field
                    if (ingre == NULL) {ingre = new StringArray();}

                    string rest = line.substr(12);
                    int pos = 0;

                    while (true){
                        int next = rest.find(", ", pos);
                        string token;

                        if (next == string::npos) {token = rest.substr(pos);}
                        else{
                            token = rest.substr(pos, next - pos);
                        }

                        if (!token.empty()) {ingre->add(token);}

                        if (next == string::npos) {break;}

                        pos = next + 2;
                    }
                }
                else if (line == "Step:"){ //step field - but only "Step: "
                    if (steps == NULL) {steps = new StringArray();}
                }
                else{
                    //real step line: "NN: <text>"
                    if (steps == NULL) {steps = new StringArray();}

                    int p = line.find(": ");

                    if (p == string::npos) {steps->add(line);}
                    else{
                        steps->add(line.substr(p + 2));
                    }
                }
            }

            //In case the file does not end with a border, finalize the last card.
            if (!name.empty()){
                if (ingre == NULL) {ingre = new StringArray();}
                if (steps == NULL) {steps = new StringArray();}

                this->recipes->add(new Recipe(name, ingre, steps));
            }
            else if (hasName || ingre != NULL || steps != NULL){
                delete ingre;
                delete steps;
            }

            in.close();
            return DB_OK;
        }

    public:
        RecipeDB(string filePath){
            this->filePath = filePath;
            this->recipes = new RecipeArray();
            this->load();
        }

        ~RecipeDB(){
            this->save();
            // RecipeDB owns the Recipe objects, so it must delete each one.
            for (int i = 0; i < this->recipes->size(); i++){
                delete this->recipes->get(i);
            }
            // RecipeArray is non-owning: its destructor only frees the pointer array.
            delete this->recipes;
        }

        // -------------------------------------------------------------------
        // query
        // -------------------------------------------------------------------

        //empty name -> Response(NULL, DB_EMPTY_NAME)
        Response* search(string name){ // Overloading
            if (name.empty()){
                return new Response(NULL, DB_EMPTY_NAME);
            }

            RecipeArray* result = new RecipeArray();

            for (int i = 0; i < this->recipes->size(); i++){
                Recipe* r = this->recipes->get(i);

                if (r->getRecipeName() == name) {result->add(r);}
            }

            return new Response(result, DB_OK);
        }

        //empty/NULL ingre -> Response(NULL, DB_NULL_ARG)
        Response* search(StringArray* ingre){ // Overloading
            if (ingre == NULL || ingre->size() == 0){
                return new Response(NULL, DB_NULL_ARG);
            }

            RecipeArray* result = new RecipeArray();

            for (int i = 0; i < this->recipes->size(); i++){ //about every Recipe in RecipeDB
                Recipe* r = this->recipes->get(i);
                StringArray* have = r->getIngredient(); //ingredients of each Recipe instances

                // --- OR: match if the recipe has at least one requested ingredient ---
                // (AND version — match only if the recipe has every requested ingredient:
                //  bool matchesAll = true;
                //  for (int j = 0; j < ingre->size(); j++){
                //      bool found = false;
                //      for (int k = 0; k < have->size(); k++){
                //          if (have->get(k) == ingre->get(j)){ found = true; break; }
                //      }
                //      if (!found){ matchesAll = false; break; }
                //  }
                //  if (matchesAll) {result->add(r);}  )
                bool matchesAny = false;

                for (int k = 0; k < have->size(); k++){ //about every ingredient the recipe has
                    for (int j = 0; j < ingre->size(); j++){ //about every requested ingredient
                        if (have->get(k) == ingre->get(j)){
                            matchesAny = true;
                            break;
                        }
                    }

                    if (matchesAny) {break;}
                }

                if (matchesAny) {result->add(r);}
            }

            return new Response(result, DB_OK);
        }

        //bad option -> Response(NULL, DB_BAD_OPTION)
        Response* order(int option){ // 0 = ascending, 1 = descending
            if (option != 0 && option != 1){
                return new Response(NULL, DB_BAD_OPTION);
            }

            int n = this->recipes->size();

            RecipeArray* result = new RecipeArray();

            if (n == 0) {return new Response(result, DB_OK);}

            // sort in a temporary pointer array first
            // And then add the pointers in result in sorted order.
            Recipe** sorted = new Recipe*[n];

            for (int i = 0; i < n; i++){
                sorted[i] = this->recipes->get(i);
            }

            // selection sort by name (option 0 = ascending, option 1 = descending)
            for (int i = 0; i < n - 1; i++){
                int pick = i;

                for (int j = i + 1; j < n; j++){
                    bool better;

                    if (option == 0){
                        better = sorted[j]->getRecipeName() < sorted[pick]->getRecipeName();
                    }
                    else{
                        better = sorted[j]->getRecipeName() > sorted[pick]->getRecipeName();
                    }

                    if (better){
                        pick = j;
                    }
                }

                if (pick != i){
                    Recipe* tmp = sorted[i];
                    sorted[i] = sorted[pick];
                    sorted[pick] = tmp;
                }
            }

            for (int i = 0; i < n; i++) {result->add(sorted[i]);}

            delete[] sorted;

            return new Response(result, DB_OK);
        }

        // -------------------------------------------------------------------
        // mutation
        // -------------------------------------------------------------------

        //if the parameter is null, skip that
        DBResult edit(string name, string newName, StringArray* newIngre, StringArray* newStep){
            if (name.empty()){
                delete newIngre;
                delete newStep;
                return DB_EMPTY_NAME;
            }

            int index = findIndex(name);

            if (index < 0){
                delete newIngre;
                delete newStep;
                return DB_NOT_FOUND;
            }

            Recipe* r = this->recipes->get(index);

            if (!newName.empty()) {r->setRecipeName(newName);}

            //deletes the old array, owns the new one
            if (newIngre != NULL) {r->setIngredient(newIngre);}

            if (newStep != NULL) {r->setStep(newStep);}

            return save();
        }

        DBResult add(string name, StringArray* ingre, StringArray* step){
            if (name.empty()){
                delete ingre;
                delete step;
                return DB_EMPTY_NAME;
            }

            if (ingre == NULL || step == NULL){
                delete ingre;
                delete step;
                return DB_NULL_ARG;
            }

            // name uniqueness is enforced here (single source of truth)
            if (findIndex(name) >= 0){
                delete ingre;
                delete step;
                return DB_DUPLICATE;
            }

            this->recipes->add(new Recipe(name, ingre, step));
            return save();
        }

        DBResult del(string name){
            if (name.empty()){
                return DB_EMPTY_NAME;
            }

            int index = findIndex(name);

            if (index < 0){
                return DB_NOT_FOUND;
            }

            // Build a new array with every recipe except the deleted one,
            // delete the removed recipe, then swap arrays.
            RecipeArray* remaining = new RecipeArray();

            for (int i = 0; i < this->recipes->size(); i++){
                if (i == index) {delete this->recipes->get(i);}
                else{
                    remaining->add(this->recipes->get(i));
                }
            }

            delete this->recipes;
            this->recipes = remaining;

            return save();
        }
};
