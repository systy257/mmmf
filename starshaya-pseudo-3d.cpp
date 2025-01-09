#include <iostream>
#include <cmath>
#include <windows.h>
#include <vector>

using namespace std;

const int SCREEN_WIDTH  = 500;        // ширина экрана в символах. берем ее из свойств окошка консоли
const int SCREEN_HEIGHT = 120;        // высота экрана в символах. Аналогично
const double SPEED = 0.02;            // скорость, с которой будем бегать по полю
const double ANGLE_SPEED = 0.05;      // скорость, с которой поворачиваемся (т.е кол-во радиан за тик)
const double VIEW_ANGLE = M_PI * 0.5; // угол обзора. M_PI - число пи, pi радиан - 180 градусов. соответственно M_PI * 0.5 это 90 градусов. как FOV в майнкрафте.
const double RAY_SPEED = 0.1;         // скорость, с которой летит пускаемый луч.
const double MAX_DIST = 5;            // радиус обзора.


pair <double, double> getDirection(double angle) { // функция, которая по углу возвращает х и у направления, куда смотрим
    return {cos(angle), sin(angle)};
}

int main() {
    const vector <string> field = { // игровое поле
        "##########",
        "#........#",
        "#........#",
        "#..####..#",
        "#.....#..#",
        "#........#",
        "#........#",
        "#...###..#",
        "#........#",
        "##########"
    };
  
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	  SetConsoleActiveScreenBuffer(hConsole);
	  DWORD dwBytesWritten = 0;
    // эти три строчки нужны для взаимодействия с консольным окошком. используем как заклинания.
  
    double angle = 0.0;                 // уголок, на который сейчас повернут персонаж
    pair <double, double> pos = {5, 5}; // текущая позиция персонажа
  
    char toPrint[SCREEN_WIDTH * SCREEN_HEIGHT]; // это то, что мы будем выводить на экран, фактически сама картинка
                                                // массив одномерный, так как текст выводится на экран сплошным полотном без перевода строчек
                                                // например, если на экран 3x3 символа вывести строчку abcde, то получится "abc"
                                                //                                                                         "de "
                                                //                                                                         "   "
    while (true) {
        fill(toPrint, toPrint + SCREEN_WIDTH * SCREEN_HEIGHT, ' '); // чистим то, что вывели на прошлой итерации цикла, то есть заполняем пробелами.
        auto dir = getDirection(angle);
        double x = dir.first;
        double y = dir.second;
        if (GetAsyncKeyState('W')) { // Если нажата кнопка W, то хотим пойти вперед
            pos.first += x * SPEED;
            pos.second += y * SPEED; 
            // x и y - это наше направление движения, полученное по углу. помножаем его на скорость и прибавляем к позиции
            if (field[floor(pos.first)][floor(pos.second)] == '#') { // если мы случайно зашли в стену, просто откатим изменение. 
                                                                     // так как скорость маленькая, мы все равно можем достаточно близко подойти к стене   
                pos.first -= x * SPEED;
                pos.second -= y * SPEED;
            }
        }
        if (GetAsyncKeyState('S')) { // тут то же самое, но идем назад
            pos.first -= x * SPEED;
            pos.second -= y * SPEED;
            if (field[floor(pos.first)][floor(pos.second)] == '#') {
                pos.first += x * SPEED;
                pos.second += y * SPEED;
            }
        }
        if (GetAsyncKeyState('A')) { // если нажали A, то поворачиваемся влево. методом подбора выяснил, что тут надо вычесть угол.
            angle -= ANGLE_SPEED;
        }
        if (GetAsyncKeyState('D')) { // если нажали A, то поворачиваемся вправо. методом исключения, тут прибавляем уголок
            angle += ANGLE_SPEED;
        }

        for (int column = 0; column < SCREEN_WIDTH; ++column) { // сложная часть - 
            double curAngle = angle - VIEW_ANGLE / 2 + column * 1.0 / SCREEN_WIDTH * VIEW_ANGLE;
            auto newDir = getDirection(curAngle);
            double dx = newDir.first;
            double dy = newDir.second;
            double curDist = 0;
            double curX = pos.first;
            double curY = pos.second;
            bool bound = false;
            while (true) {
                if (field[floor(curX)][floor(curY)] == '#') {
                    if (((-floor(curX) + curX) < 0.07 || (ceil(curX) - curX) < 0.07) &&
                        ((-floor(curY) + curY) < 0.07 || (ceil(curY) - curY) < 0.07)) {
                            bound = true;
                    }
                    break;
                }
                curDist += RAY_SPEED;
                curX += dx * RAY_SPEED;
                curY += dy * RAY_SPEED;
            }
            double white = max(0.0, (MAX_DIST - curDist) / MAX_DIST);
            int amountWhite = floor(white * SCREEN_HEIGHT / 2);
            for (int row = 0; row < SCREEN_HEIGHT; ++row) {
                if (abs(SCREEN_HEIGHT / 2 - row) <= amountWhite) {
                    if (!bound) {
                        toPrint[row * SCREEN_WIDTH + column] = '#';
                    } else {
                        toPrint[row * SCREEN_WIDTH + column] = ' ';
                    }
                } else if (row > SCREEN_HEIGHT / 2) {
                    toPrint[row * SCREEN_WIDTH + column] = '.';
                }
            }
        }
        WriteConsoleOutputCharacter(hConsole, toPrint, SCREEN_WIDTH * SCREEN_HEIGHT, {0, 0}, &dwBytesWritten);
    }
}
