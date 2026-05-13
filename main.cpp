#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <sstream>

using namespace std;

// =============== ЛОКАЦИЯ ===============
class Location {
public:
    string name;
    vector <string> items;
    string description;
    string exitNorth;
    string exitSouth;
    string exitWest;
    string exitEast;

    void show() {
        cout << "\n=== " << name << " ===\n";
        cout << description << "\n";
        cout << "Можно идти: ";

        if (exitNorth != "") cout << "север ";
        if (exitSouth != "") cout << "юг ";
        if (exitWest != "") cout << "запад ";
        if (exitEast != "") cout << "восток ";

        cout << "\n";
        cout<< "====================="<<endl;
    }

    bool canGo(string direction) {
        if (direction == "север" && exitNorth != "") return true;
        if (direction == "юг" && exitSouth != "") return true;
        if (direction == "запад" && exitWest != "") return true;
        if (direction == "восток" && exitEast != "") return true;
        return false;
    }

    string getExit(string direction) {
        if (direction == "север") return exitNorth;
        if (direction == "юг") return exitSouth;
        if (direction == "запад") return exitWest;
        if (direction == "восток") return exitEast;
        return "";
    }
    void addItem(string item) {
        items.push_back(item);
    }
    void delItem(string item) {
        for (int i = 0; i < items.size(); i++) {
            if (items[i] == item) {
                items.erase(items.begin() + i);
                break;
            }
        }
    }
    bool hasItem(string item) {
        for (int i = 0; i < items.size(); i++) {
            if (items[i] == item) {
                return true;
            }
        }
        return false;
    }
};

// =============== ИГРОК ===============
class Player {
public:
    string currentLocation;
    vector<string> inventory;

    void showInv() {
        if (inventory.size() == 0) cout<< "Инвентарь пуст.\n";
        else {
            cout << "У вас есть: ";
            for (int i = 0;i<inventory.size();i++) {
                string temp = inventory[i];
                cout << temp;
                if ( (i+1) != inventory.size()) cout << ",";
            }
            cout<<endl;
        }
    }
    void takeItem(string item) {
        inventory.push_back(item);
    }
    bool hasItem(string itemName) {
        bool flag = false;
        for (int i =0;i<inventory.size();i++) {
            if (itemName == inventory[i]) {
                flag = true;
            }
            if (flag) {
                return true;
                break;
            }
        }
        if (!flag) {
            return false;
        }

    }
};

// =============== ИГРА ===============
class Game {
private:
    Location forest;
    Location cave;
    Location river;
    Player player;

public:
    Game() {
        // ЛЕС
        forest.name = "Лесная поляна";
        forest.addItem("мохнатое кольцо");
        forest.addItem("ржавый ключ");
        forest.description = "Ты стоишь на поляне. Вокруг деревья.";
        forest.exitNorth = "cave";
        forest.exitSouth = "";
        forest.exitWest = "";
        forest.exitEast = "river";

        // ПЕЩЕРА
        cave.name = "Тёмная пещера";
        cave.description = "Здесь холодно и темно. Слышно, как капает вода.";
        cave.exitNorth = "";
        cave.exitSouth = "forest";
        cave.exitWest = "";
        cave.exitEast = "";

        // РЕКА
        river.name = "Берег реки";
        river.description = "Ты у реки. Вода прозрачная, видно камни на дне.";
        river.exitNorth = "";
        river.exitSouth = "";
        river.exitWest = "forest";
        river.exitEast = "";

        player.currentLocation = "forest";
    }

    void start() {

        string command;
        //state машина
        while (true) {



            cout << "> ";
            getline(cin, command);


            for (int i = 0; i < command.length(); i++) {
                command[i] = tolower(command[i]);
            }
            if (command == "выход") {
                cout << "До свидания!\n";
                break;
            }
            else if (command == "осмотреться") {
                showCurrentLocation();
            }
            else if (command == "север" || command == "юг" || command == "запад" || command == "восток") {
                go(command);
            }

            else if (command.rfind("взять",0) == 0) {
                stringstream ss(command);
                string com;
                ss>>com;
                string itemName;
                getline(ss,itemName);
                if (!itemName.empty() && itemName[0] == ' ') {
                    itemName.erase(0, 1);
                }
                Location* loc = getCurrentLocation();
                if (loc->hasItem(itemName) == true) {
                    player.takeItem(itemName);
                    loc->delItem(itemName);
                    cout << "Вы подобрали: "<< itemName <<endl;
                }
                else{ cout<<"Такого предмета здесь нет."<<endl;}

            }

            else if (command == "инвентарь" || command == "инв") {
                player.showInv();
            }
            else if (command == "помощь"){
                cout << "Команды: север, юг, запад, восток, инвентарь, оглядеть, выход\n";
            }
            else if (command.rfind("осмотреть",0) == 0) {
                stringstream ss(command);
                string com;
                ss>>com;
                string itemName;
                getline(ss,itemName);
                if (!itemName.empty() && itemName[0] == ' ') {
                    itemName.erase(0, 1);
                }
                if (player.hasItem(itemName) == true) {
                    if (itemName == "мохнатое кольцо") {
                        cout << "Кольцо, сплетённое из чьих-то волос. Тёплое на ощупь. Если поднести его к уху — слышен едва уловимый шёпот."<<endl;
                    }
                    else {
                        cout << "Ничего необычного."<<endl;
                    }
                }
                else {cout << "Такого предмета нет."<<endl;}
            }
            else {
                cout<<" Не правильная команда.\n ";
            }
        }
    }

private:
    Location* getCurrentLocation() {
        if (player.currentLocation == "forest") return &forest;
        if (player.currentLocation == "cave") return &cave;
        if (player.currentLocation == "river") return &river;
        return nullptr;
    }

    void showCurrentLocation() {
        Location* loc = getCurrentLocation();
        if (loc != nullptr) {
            loc->show();

        }
    }

    void go(string direction) {
        Location* loc = getCurrentLocation();

        if (loc == nullptr) {
            cout << "Ошибка!\n";
            return;
        }

        if (loc->canGo(direction)) {
            string nextLocation = loc->getExit(direction);
            player.currentLocation = nextLocation;
        } else {
            cout << "Туда нельзя пойти!\n";
        }
    }
};

// =============== ГЛАВНАЯ ФУНКЦИЯ ===============
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cout << R"(
=========================================
           .-:::::::::::::::::.
           :-     ..::..     :-
           :-     :+.++=:    :-
           --  .=:+*.+=-+    ::
           -:  .--#=-*-=*    :-
           -:   :=+-+-*+#.   --
            .-: .--=+=++=. .-.
            ..:-:  ... :-::.
                .::::::..
=========================================
    ДОБРО ПОЖАЛОВАТЬ В ТЕКСТОВЫЙ КВЕСТ
=========================================
Нажмите Enter чтобы продолжить...
)" << endl;
    cin.ignore();
    Game game;
    cout<<R"(
Ты просыпаешься в незнакомом лесу. Голова гудит, одежда порвана.
Последнее, что ты помнишь — яркая вспышка.
Теперь нужно выбраться и понять, что произошло.

Нажмите Enter чтобы продолжить...
        )" << endl;
    cin.ignore();
    game.start();
    return 0;
}
