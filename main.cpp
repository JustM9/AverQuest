#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

// =============== ЛОКАЦИЯ ===============
class Location {
public:
    string name;
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
};

// =============== ИГРОК ===============
class Player {
public:
    string currentLocation;
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
        cout << "========================================\n";
        cout << "==         ТЕКСТОВЫЙ КВЕСТ           ==\n";
        cout << "========================================\n";
        cout << "Команды: север, юг, запад, восток, оглядеть, выход\n\n";

        string command;

        while (true) {
            // Показываем текущую локацию
            showCurrentLocation();

            cout << "> ";
            getline(cin, command);

            // Переводим в нижний регистр
            for (int i = 0; i < command.length(); i++) {
                command[i] = tolower(command[i]);
            }

            if (command == "выход") {
                cout << "До свидания!\n";
                break;
            }
            else if (command == "оглядеть") {
                cout<< "====================="<<endl;
                if (player.currentLocation == "forest") {
                    cout << "На тебя пристально смотрят птицы. Больше ничего необычного." << endl;
                }
                else if (player.currentLocation == "cave") {
                    cout<< "В углу видны кости. Что бы тут не жило, оно уже мертво"<<endl;
                }
                else if (player.currentLocation == "river") {
                    cout << "Ничего необычного"<<endl;
                }
                cout<< "====================="<<endl;

            }
            else if (command == "север" || command == "юг" || command == "запад" || command == "восток") {
                go(command);
            }
            else {
                cout << "Не понял. Команды: север, юг, запад, восток, осмотреться, выход\n";
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
    void showDesc2() {

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
    Game game;
    game.start();
    return 0;
}