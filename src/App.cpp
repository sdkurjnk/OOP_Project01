#pragma once

#include <string>
#include <iostream>
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

