#include <bits/stdc++.h>
#include <random>
#include <windows.h>
#include <cstdlib>
using namespace std;
mt19937 rng(time(0));
int countpla;
const int N = 600;
const int P = 1000;
const int T = 10;
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
map<int, int> pla;
struct civ
{
	double ord;     //初始秩序值 
	double neword;  //用于计算的秩序值 
	double wei;     //质量（生命） 
	double tec;     //科技水平 
	int level;      //纪元等级 
	int kill;       //击杀数 
	int dir;        //位置信息 
	double shield;  //防御力 
}a[N*2];
int r(int n)
{
	if (n == 0) return 0;
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
	for (int i = 1; i < 2*p; i++) if (battle.count(i) == 0) e[i].clear();
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
	return a[x].level*a[x].level*a[x].tec*a[x].neword/a[y].level/a[y].level/a[y].tec/a[y].shield;//伤害比率公式 
}
double damage2(int x, int y)
{
	return a[x].neword*max(a[x].kill, 1)*r(5)*0.1/a[y].neword/max(a[y].kill, 1)/a[y].shield;//秩序值伤害公式 
}
void dead(int x)
{
	alive[x] = 0;
	vis[a[x].dir] = 0;
	countciv--;
	if (pla.count(x) != 0)
	{
		cout << "玩家" << pla[x] << "阵亡！\n";
		Sleep(1000);
		countpla--;
		pla.erase(x);
	}
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
		if (a[w].wei <= 0)
		{
			a[l].kill++;
			dead(w);
			return -2;
		}
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
			dead(x);
			dead(y);
			return -2;
		}
		if (a[x].wei < die[lev])
		{
			dead(x);
			if (a[y].wei < die[lev])
			{
				dead(y);
				return -2;
			}
			return 2;
		}
		if (a[y].wei < die[lev])
		{
			dead(y);
			return 1;
		}
		if (a[x].neword < a[x].ord*r(20)*0.01)
		{
			dead(x);
			return 2;
		}
		if (a[y].neword < a[y].ord*r(36)*0.01)
		{
			dead(y);
			return 1;
		}
	}
	return -1;
}
void chance(int x)
{
	if (r(20) == 3) a[x].wei = die[a[x].level]*1.1;
	else if(r(20) == 17)
	{
		a[x].wei *= min((r(3)+1)*(r(3)+1)*0.5, 5.0);
		a[x].tec += 0.2;
	}
	else a[x].wei *= r(17)*0.01+0.91;
}
int move(int u, int pe)
{
	int v = e[u][r(e[u].size())-1];
	while (v == a[pe].dir) v = e[u][r(e[u].size())-1];
	if (space[v])
	{
		v = r(battle.size())-1;
		auto it = battle.begin();
		while (v--) it++;
		v = it->first;
	}
	return v;
}
int main()
{
	//===初始化=== 
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
	cout << "请输入游玩人数：";
	cin >> countpla;
	cout << "正在创造宇宙……\n";
	int kcp = N/countpla;
	for (int i = 0; i < countpla; i++)
	{
		int ot = i*kcp+r(kcp);
		pla[ot] = i+1;
		a[ot].neword = 0.8+r(10)*0.01;
		a[ot].wei = 45+r(15);
		a[ot].tec = 1.9+r(10)*0.01;
		a[ot].shield = r(10)*0.1+4;
	}
	Sleep(1200);
	cout << "创造完成！\n";
	Sleep(600);
	system("cls");
	//===正式开始=== 
	cout << "顶级黑暗森林大逃杀游戏，现在开始！\n";
	Sleep(1000);
	system("cls");
//	system("start 黑暗森林·剧情.exe");
//	Sleep(32000);
	int res = MessageBoxA(0, "你准备好了吗？", "【最后通牒】", (UINT)(MB_OKCANCEL | MB_ICONWARNING));
	if (res == IDCANCEL)
	{
		system("color 04");
		cout << "懦夫不配加入勇敢者的宇宙！";
		return 0;
	}
	for (int k = 1; k <= T; k++)
	{
		int nue = countciv*kcd;
		init(nue, nue*(r(4)+1));
		cout << "\n【第" << k << "回合】：\n";
		for (int i = 1; i <= N; i++)
		{
			if (pla.count(i) != 0) cout << "请玩家" << pla[i] << "操作\n";
			int maxk = 0;
			if (!alive[i]) continue;
			vis[a[i].dir] = 0;
			int x = a[i].dir;
			for (int step = 1; step <= a[i].level; step++)
			{
				if (pla.count(i) == 0) x = move(x, i);
				else if (x != 0)
				{
					cout << "请选择下一步的目的地：";
					for (int _x : e[x]) cout << _x << ' ';
					cout << '\n';
					cin >> x;
					if (x == 0)
					{
						cout << "请稍等，机缘随机投放中……\n";
						Sleep(1500);
					}
				}
				else
				{
					cout << "0是宇宙深渊，连接万界。请输入下一步的目的地（0~" << nue*2-1 << "）：\n";
					cin >> x;
					if (x == 0)
					{
						cout << "请稍等，机缘随机投放中……\n";
						Sleep(1500);
					}
				}
				if (x == 0) chance(i);
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
							for (const auto& j : battle)
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
				a[i].shield += sqrt(a[i].wei)*r(7)*0.25;
				if (pla.count(i) != 0)
				{
					cout << "玩家" << pla[i] << "已经升至" << a[i].level << "级！\n";
					Sleep(1000);
				}
			}
			if (a[i].level==11 && a[i].kill>12 && r(5) == 2)
			{
				dead(i);
				god.push_back(i);
				cout << "【世界广播】文明" << i << "成为神明！\n";
				Sleep(2000);
			}
		}
		system("cls");
		if (countciv<=1) break;
		if (countpla == 0)
		{
			cout << "【所有玩家已被吞没，宇宙爆炸】";
			return 0;
		}
	}
	cout << "\n游戏结束！\n";
	cout << "神级文明：";
	for (const auto& i : god) cout << i << ' ';
	if (god.empty()) cout << "无";
	cout << '\n';
	cout << "存活凡级文明：";
	bool op = 1;
	for (int i = 1; i <= N; i++)
	{
		if (alive[i])
		{
			op = 0;
			cout << i << "(" << a[i].level << "级 ,击杀" << a[i].kill << ") ";
		}
	}
	if (op) cout << "全军覆没！";
	cout << "\n存活玩家：";
	if (pla.size() == 0) cout << "无！";
	else for (const auto& i : pla) cout << i.second << "(文明" << i.first << ") ";
	cout << "\n埋葬在【宇宙深渊】中的文明：\n";
	if (battle[0].empty()) cout << "无";
	else for (const auto& i : battle[0])	cout << i.first << "与" << i.second << ' ';
	cout << "\n【遗迹战场】：\n";
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
