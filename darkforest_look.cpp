#include <bits/stdc++.h>
#include <random>
#include <windows.h>
#include <cstdlib>
using namespace std;
mt19937 rng(time(0));
const int N = 10000;
const int P = 100000;
const int T = 100;
const int kcd = P/N;
const int die[12] = {0, 5, 10, 30, 120, 600, 3600, 25200, 100800, 1000000, 36000000, INT_MIN};
int countciv = N;
int fa[P*2];
int vis[P*2];
vector<int> e[P*2];
vector<int> god;
bool alive[N*2];
bool space[P*2];
map<int, vector<pair<int, int>>> battle;
struct civ
{
	double ord;
	double neword;
	double wei;
	double tec;
	int level;
	int kill;
	int dir;
	double shield;
}a[N*2];
int r(int n)
{
	if (n == 0) return 1;
    uniform_int_distribution<int> dist(1, n);
    return dist(rng);
}
int getfa(int x)
{
	return (x==fa[x] ? x : fa[x]=getfa(fa[x]));
}
void init(int p, int m)
{
	memset(vis, 0, sizeof(vis));
	e[0].clear();
	for (int i = 1; i < 2*P; i++) if (battle.count(i) == 0) e[i].clear();
	for (int i = 1; i < 2*p; i++)
	{
		if (battle.count(i) != 0) continue;
		fa[i] = i;
		e[i].push_back(0);
		e[0].push_back(i);
	}
	int cnt = 2*p-2-battle.size();
	while (cnt)
	{
		int x = r(2*p-1);
		int y = r(2*p-1);
		if (x == y) continue;
		if (battle.count(x)!=0 || battle.count(y)!=0) continue;
		if (getfa(x) != getfa(y))
		{
			fa[fa[x]] = fa[y];
			e[x].push_back(y);
			e[y].push_back(x);
			cnt--;
		}
	}
	for (int i = 1; i <= m; i++)
	{
		int x = r(2*p-1);
		int y = r(2*p-1);
		e[x].push_back(y);
		e[y].push_back(x);
	}
	int nit = 1;
	for (int i = 1; i <= N; i++)
	{
		if (alive[i])
		{
			a[i].dir = nit*kcd+r(kcd)-1;
			vis[a[i].dir] = i;
			nit++;
		}
	}
}
double damage(int x, int y)
{
	return a[x].level*a[x].level*a[x].tec*a[x].neword/a[y].level/a[y].level/a[y].tec/a[y].shield;
}
double damage2(int x, int y)
{
	return a[x].neword*max(a[x].kill, 1)*r(5)*0.1/a[y].neword/max(a[y].kill, 1)/a[y].shield;
}
void dead(int x)
{
	alive[x] = 0;
	vis[a[x].dir] = 0;
	countciv--;
}
int war(int x, int y)
{
	if (!alive[x] || !alive[y]) return 0;
	int w, l;
	if (a[x].level != a[y].level)
	{
		w = (a[x].level>a[y].level ? x : y);
		l = (w==x ? y : x);
		a[l].tec *= r(8)*0.1+1.2;
		a[w].kill++;
		dead(l);
		a[w].wei -= damage(l, w)*a[l].wei;
		if (a[w].wei <= die[a[w].level])
		{
			cout << "文明" << w << "与文明" << l << "同归于尽！\n";
			a[l].kill++;
			dead(w);
//Sleep(200);
			return -2;
		}
		cout << "高维文明" << w << "抹杀了低维文明" << l << "!\n";
//Sleep(200);
		return (w==x ? 1 : 2);
	}
	int lev = a[x].level;
	int round = lev*lev;
	while (round--)
	{
		int wx = a[x].wei;
		a[x].wei -= damage(y, x)*a[y].wei;
		if (a[x].wei < die[a[x].level]) a[y].wei -= damage(x, y)*wx*(r(10)*0.01+0.1);
		else a[y].wei -= damage(x, y)*wx;
		w = (a[x].neword>a[y].neword ? x : y);
		l = (w==x ? y : x);
		a[l].neword -= damage2(w, l);
		a[w].neword -= damage2(l, w);
		if (isnan(a[x].wei) || isnan(a[y].wei) || isnan(a[x].neword) || isnan(a[y].neword))
		{
			cout << "文明" << x << "与文明" << y << "的战斗造成法则坍塌，双双陨落！\n";
			dead(x);
			dead(y);
//Sleep(200);
			return -2;
		}
		if (a[x].wei < die[lev])
		{
			dead(x);
			if (a[y].wei < die[lev])
			{
				cout << "文明" << x << "与文明" << y << "同归于尽！\n";
				dead(y);
//Sleep(200);
				return -2;
			}
			cout << "文明" << y << "抹杀了文明" << x << "！\n";
//Sleep(200);
			return 2;
		}
		if (a[y].wei < die[lev])
		{
			cout << "文明" << x << "抹杀了文明" << y << "！\n";
			dead(y);
//Sleep(200);
			return 1;
		}
		if (a[x].neword < a[x].ord*r(20)*0.01)
		{
			cout << "文明" << x << "在与文明" << y << "的战斗中因内乱而毁灭！\n";
			dead(x);
//Sleep(200);
			return 2;
		}
		if (a[y].neword < a[y].ord*r(36)*0.01)
		{
			cout << "文明" << y << "在与文明" << x << "的战斗中因内乱而毁灭！\n";
			dead(y);
//Sleep(200);
			return 1;
		}
	}
	return -1;
}
int main()
{
//	SetConsoleOutputCP(65001);
 	freopen("o.txt", "w", stdout);
//	ios::sync_with_stdio(0);
//	cin.tie(0);
//	cout.tie(0);
	for (int i = 1; i <= N; i++)
	{
		a[i].ord = r(30)*0.01+0.6;
		a[i].neword = a[i].ord;
		a[i].wei = r(10)+r(10)+30;
		a[i].tec = r(99)*0.01+1;
		a[i].level = 1;
		a[i].kill = 0;
		a[i].dir = i*kcd+r(kcd)-1;
		a[i].shield = r(20)*0.1+3;
		alive[i] = 1;
		vis[a[i].dir] = i;
	}
	battle[0];
	cout << "顶级黑暗森林大逃杀游戏，现在开始！\n";
	Sleep(3000);
	for (int k = 1; k <= T; k++)
	{
		//system("cls");
		init(countciv*kcd, countciv*kcd*r(3));
		cout << "\n第" << k << "回合：\n";
		//Sleep(500);
		for (int i = 1; i <= /*(countciv+2)/3*/N; i++)
		{
			//int i = r(N);
			int maxk = 0;
			if (!alive[i]) continue;
			vis[a[i].dir] = 0;
			int step = a[i].level;
			int x = a[i].dir;
			while (step--)
			{
				x = e[x][r(e[x].size())-1];
				while (x == a[i].dir) x = e[x][r(e[x].size())-1];
				if (space[x])
				{
					x = r(battle.size())-1;
					auto it = battle.begin();
					while (x--) it++;
					x = it->first;
				}
				if (vis[x])
				{
					int enemy = vis[x];
					if (abs(a[enemy].level-a[i].level) >= 3) continue;
					int win = war(enemy, i);
					if (win!=-1 && x!=0 && win!=-2 && battle.count(x)==0) space[x] = 1;
					if (win==-2)
					{
						if (battle.count(x) == 0)
						{
							for (auto j : battle)
							{
								if (j.first == 0) continue;
								e[j.first].push_back(x);
								e[x].push_back(j.first);
							}
						}
						battle[x].push_back({enemy, i});
					}
					if (win == 2)
					{
						a[i].wei += a[enemy].wei*a[enemy].neword*a[enemy].level/a[i].level*(r(10)*0.01+0.1)*a[i].tec;
						a[i].tec += a[enemy].tec*a[enemy].level*(r(20)*0.01+0.3)/a[i].tec;
						maxk++;
					}
					else if (win == 1)
					{
						a[enemy].wei += a[i].wei*a[i].neword*a[i].level/a[enemy].level*(r(10)*0.01+0.1)*a[enemy].tec;
						a[enemy].tec += a[i].tec*a[i].level*(r(20)*0.01+0.3)/a[enemy].tec;
					}
					if (!alive[i]) break;
					if (maxk > a[i].level*10) break;
				}
			}
			if (alive[i]) 
			{
				vis[x] = i;
				a[i].dir = x; 
			}
		}
		for (int i = 1; i <= N; i++)
		{
			if (!alive[i]) continue;
			a[i].tec += (r(20)*0.01+0.2)/a[i].level/a[i].tec;
			a[i].neword += a[i].neword*a[i].tec*(r(20)*0.01+0.1)/a[i].level;
			a[i].wei += a[i].level*a[i].neword*(r(20)+10)*0.1;
			while (a[i].tec>=2.3 && a[i].level<11)
			{
				a[i].level++;
				a[i].tec /= 2;
				a[i].neword += a[i].level*r(3)*0.01;
				a[i].wei *= a[i].level;
				a[i].shield += sqrt(a[i].wei);
				cout << "恭喜文明" << i << "成为" << a[i].level << "级文明\n";
//Sleep(200);
			}
			if (a[i].level==11 && a[i].kill>12 && r(5) == 2)
			{
				dead(i);
				god.push_back(i);
				cout << "【世界广播】文明" << i << "成为神明\n";
//Sleep(200);
			}
		}
		cout << "剩余文明数量：" << countciv << '\n';
		//Sleep(1800);
		//system("pause");
		if (countciv <= 1) break;
	}
	cout << "\n游戏结束！\n";
	//Sleep(2000);
	cout << "神级文明：";
	for (auto i : god) cout << i << ' ';
	if (god.empty()) cout << "无";
	cout << '\n';
	//Sleep(1000);
	cout << "幸存凡级文明：";
	bool op = 1;
	for (int i = 1; i <= N; i++)
	{
		if (alive[i])
		{
			op = 0;
			cout << i << "(" << a[i].level << "级，击杀" << a[i].kill << ") ";
		}
	}
	if (op) cout << "全军覆没！";
	cout << "\n埋葬在宇宙深渊的文明：\n";
	if (battle[0].empty()) cout << "无";
	else for (const auto& i : battle[0]) cout << i.first << "与" << i.second << ' ';
	cout << "\n遗迹战场：\n";
	if (battle.size() == 1) cout << "无";
	else
	{
		for (const auto& i : battle)
		{
			if (i.first == 0) continue;
			cout << i.first << "号空间共埋葬了" << i.second.size() << "对文明：";
			for (const auto& j : i.second) cout << "文明" << j.first << "与文明" << j.second << ' ';
			cout << '\n';
		}
	}
	return 0;
}
/*
*/
