#include<iostream>
#include<cstdlib>
#include<windows.h>
using namespace std;
void cc(string s, int t)
{
	for (int i = 0; i < s.size(); i++)
	{
		cout << s[i];
		cout.flush();
		Sleep(t);
	}
}
int main()
{
	system("color 04");
	cc(" -----【来自黑暗森林的警告】-----\n", 100);
	cc("     -----【最后通牒】-----\n\n", 100);
	Sleep(1000);
	cc(" 这里，危机四伏，稍有不慎就会迷失。\n", 50);
	Sleep(400);
	cc(" 所有人不能同时活着，杀戮无处不在。\n", 50);
	Sleep(400);
	cc(" 你一旦选择加入，就无法返回。\n", 50);
	Sleep(400);
	cc(" 你的前方是一片黑暗，只有变强。\n", 50);
	Sleep(400);
	cc(" 在这里，你能祈求的只有一样东西\n", 50);
	cc("              ——活着。\n", 50);
	system("color 08");
	Sleep(600);
	system("color 04");
	cc(" 没有人能帮你，一切都要靠自己。\n\n", 50);
	Sleep(1000);
	cc(" 你", 200);
	Sleep(300);
	cc("准备好迎接挑战了吗？\n", 80);
	Sleep(2000);
	return 0;
}
