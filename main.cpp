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

        }
        return flag;
    }
};

// =============== ИГРА ===============
class Game {
private:
    Location forest;
    Location cave;
    Location deep_cave;
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
        cave.description = "Здесь холодно и темно.";
        cave.exitNorth = "";
        cave.exitSouth = "forest";
        cave.exitWest = "";
        cave.exitEast = "";

        deep_cave.name = "Глубь пещеры";
        deep_cave.description = "Здесь почти ничего не видно. Слышен шум капающей воды. В самом углу виднеется какая то тень. Это человек.";
        deep_cave.exitNorth = "";
        deep_cave.exitSouth = "";
        deep_cave.exitWest = "";
        deep_cave.exitEast = "";

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
            else if (command.rfind("идти",0) == 0) {
                stringstream ss(command);
                string com;
                ss>>com;
                string exit;
                getline(ss,exit);
                if (!exit.empty() && exit[0] == ' ') {
                    exit.erase(0, 1);
                }
                if (exit == "север" || exit == "юг" || exit == "запад" || exit == "восток") {
                    go(exit);
                    showCurrentLocation();
                }
                else if (exit == "вглубь" && player.currentLocation == "cave") {
                    player.currentLocation = "deep_cave";
                }
                else {cout << "Неправильная команда\n";}
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
                cout << "В этих местах даже бог не поможет\n";
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
                    else if (itemName == "листок бумаги") {
                        cout << "Листок бумаги с написанным текстом. Видимо это дневник Человека в углу. Текст гласит: ";cin.ignore();
                        cout << "\"21.02 Моим скитаниям пришёл конец.\"";cin.ignore();
                        cout << "\"Я нашёл заброшённую лачугу. Переночую в ней. Команта с кроватью довольно уютная.\""; cin.ignore();
                        cout << "\"Правда эти картины с искажёнными лицами наводят жути.\""; cin.ignore();
                        cout << "\"У меня осталось не так много еды, так что надо есть поменьше. Глаза слипаются, ложусь спать.\""; cin.ignore();
                        cout << " На обратной стороне листка виднеется большая, размашистая надпись: ";cin.ignore();
                        cout << "\"ЭТО БЫЛИ ОКНА\"";cin.ignore();
                    }
                    else {
                        cout << "Ничего необычного."<<endl;
                    }
                }
                else {cout << "Такого предмета нет."<<endl;}
            }
            //deep_cave part
            if (player.currentLocation == "deep_cave") {
                if (command=="выйти") {
                    player.currentLocation = "cave";
                }
                else if (command == "говорить") {
                    cout << "Вы произносите: \'Эй, у вас всё порядке?\'"; cin.ignore();
                    cout<< "Высокый, тощий мужчина подходит к вам"; cin.ignore();
                    cout << "Вы разглядываете лицо этого человека"; cin.ignore();
                    cout << "Он явно на грани безумия"; cin.ignore();
                    cout << "\'Они...  Они там? Ты их в-видел?\' - заикаясь спросил этот мужчина"; cin.ignore();
                    string ans;
                    cout << "1: Да   2: Нет"<<endl;
                    cin >> ans;
                    if (ans == "1" || ans == "Да" || ans == "да") {
                        cout << "На его лице застыла гримаса ужаса."; cin.ignore();
                        cout << "Безумец резко толкает тебя и убегает"; cin.ignore();
                        cout << "Толчок был слабым. Безумца нигде не видно"; cin.ignore();
                        deep_cave.addItem("листок бумаги");
                        deep_cave.description = "Здесь почти ничего не видно. Слышен шум капающей воды. Человека в углу больше нет. Он обронил листок бумаги.";

                    }
                    else if (ans == "2" || ans == "Нет") {
                        cout << "Напряжение на его лице заметно спало"; cin.ignore();
                        cout << "\' Они идут за мной. Они меня найдут, Они найдут и тебя. Уходи\' - тихо прошептал мужчина."; cin.ignore();
                        cout << "Человек снова вжался в угол пещеры и пристально смотрел то на меня, то на проход позади."; cin.ignore();

                        string ans1;
                        cout << "1.Вы знаете что это за место? 2.Уйти"<<endl;
                        cin >> ans1;
                        if (ans1 == "1") {
                            cout << "\'Гамленская долина\'"; cin.ignore();
                        }
                        else {
                            cout <<"Вы вышли обратно в пещеру"<<endl;
                            player.currentLocation = "cave";
                        }
                    }


                }
            }
        }
    }

private:
    Location* getCurrentLocation() {
        if (player.currentLocation == "forest") return &forest;
        if (player.currentLocation == "cave") return &cave;
        if (player.currentLocation == "river") return &river;
        if (player.currentLocation == "deep_cave") return &deep_cave;
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
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Russian");

    cout<<"========================================="<<endl;
    cout<<"ДОБРО ПОЖАЛОВАТЬ В ТЕКСТОВЫЙ КВЕСТ"<<endl;
    cout<<"========================================="<<endl;
    cin.ignore();
    Game game;
    cout<<"Ты просыпаешься в незнакомом лесу. Голова гудит, одежда порвана.";cin.ignore();
    cout<<"Последнее, что ты помнишь — яркая вспышка.";cin.ignore();
    cout<<"Теперь нужно выбраться и понять, что произошло.";cin.ignore();

    game.start();
    return 0;
}
