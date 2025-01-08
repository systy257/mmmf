#include <iostream>
#include <algorithm>

using namespace std;

struct Event {
    int cord = 0;
    int type = 0;
    int id = 0;
}; // вот тут надо ставить ;

bool cmp(Event a, Event b) { // ваша функция должна возвращать 1, если элементы с точки зрения сортировки стоят ПРАВИЛЬНО
    return a.cord < b.cord; // если вас интересует порядок только по одному параметру, можно оставить просто так.
}

int main() {
    vector <Event> v;
    for (int i = 0; i < 5; ++i) {
        v.push_back({i, i + 1, i + 2});
    }
    sort(v.begin(), v.end(), cmp);
}
