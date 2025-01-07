#include <iostream>
#include <numeric>

using namespace std;

constexpr int MOD = 1'000'000'007; // это простое число

int add(int a, int b) { // сложение по модулю (можно писать просто (a + b) % MOD, 
                        //                     но это работает дольше)
    a += b;
    if (a >= MOD) {
        a -= MOD;
    }
    return a;
}

int sub(int a, int b) { // вычитание по модулю
    a -= b;
    if (a < 0) {
        a += MOD;
    }
    return a;
}

int mul(int a, int b) { // умножение по модулю
    return 1ll * a * b % MOD; // 1ll - это просто long long единица. обычная единица это int.
}

int binpow(int a, int n) { // быстрое возведение в степень с циклом вместо рекурсии. работает быстрее
    int res = 1;
    while (n) {
        if (n & 1) {
            res = mul(res, a);
        }
        n >>= 1;
        a = mul(a, a);
    }
    return res;
}

signed main() {
    cout << gcd(1, 2); // это встроенный алгоритм евклида. лежит в библиотеке <numeric>
}   
