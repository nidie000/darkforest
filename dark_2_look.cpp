#include<bits/stdc++.h>
#include<random>
using namespace std;
const int N = 100, M = 100;
const int Civ = 500;
int sx, sy, tx, ty;
int f[N+5][M+5];
int head, tail, to[Civ+5];
pair<int, int> ad[Civ+5];
int lv[Civ+5], ex[Civ+5], dmg[Civ+5];
bool ale[Civ+5];
int r(int n)
{
	if (n == 0) return 1;
    uniform_int_distribution<int> dist(1, n);
    return dist(rng);
}
int main()
{
	head = 1, tail = Civ;
	for (int i = 1; i < Civ; i++) to[i] = i+1;
	return 0;
}
