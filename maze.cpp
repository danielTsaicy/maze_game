#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<cctype>
#include<queue>
#include<utility>
#include<algorithm>
#include<conio.h>
#include<windows.h>
using namespace std;
typedef pair<int, int> intint;

void setcolor(int color = 7)
{
	HANDLE con = GetStdHandle(STD_OUTPUT_HANDLE);
  	SetConsoleTextAttribute(con, color);
}

void setpos(int xpos, int ypos)
{
	COORD scrn;
	HANDLE con = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(con, scrn);
}

int main()
{
	srand(time(NULL));
	system("color 80");
	for(;;)
	{
		int maze[21][81] = {};
		int cnt = 1;
		int goal = 0,progress = 1;
		queue<intint> path;
		const intint EXIT = {19,80};
		for(int i = 1; i < 20; i += 2)
		{
			for(int j = 1; j < 80; j += 2)
			{
				maze[i][j] = ++cnt;
				++goal;
			}
		}
		maze[1][0] = maze[1][1] = maze[19][80] = 2;
		while(progress < goal)
		{
			int x = rand()%79 + 1;
			int y = rand()%19 + 1;
			if((x+y)%2 == 0) continue;
			if(maze[y][x] != 0) continue;
			if(x%2 == 1 && maze[y-1][x] == maze[y+1][x]) continue;
			if(x%2 == 0 && maze[y][x-1] == maze[y][x+1]) continue;
			if(x%2 == 1)
			{
				maze[y][x] = min(maze[y-1][x], maze[y+1][x]);
				path.push(make_pair(y, x));
			}
			else
			{
				maze[y][x] = min(maze[y][x-1], maze[y][x+1]);
				path.push(make_pair(y, x));
			}
			while(!path.empty())
			{
				intint now = path.front(); path.pop();
				int now_y = now.first; int now_x = now.second;
				if(maze[now_y-1][now_x] != 0 && maze[now_y-1][now_x] != maze[now_y][now_x])
				{
					path.push(make_pair(now_y-1, now_x));
					maze[now_y-1][now_x] = maze[now_y][now_x];
					if(maze[now_y-1][now_x] == 2 && ((now_y-1)&1) && (now_x&1)) ++progress;
				}
				if(maze[now_y+1][now_x] != 0 && maze[now_y+1][now_x] != maze[now_y][now_x])
				{
					path.push(make_pair(now_y+1, now_x));
					maze[now_y+1][now_x] = maze[now_y][now_x];
					if(maze[now_y+1][now_x] == 2 && ((now_y+1)&1) && (now_x&1)) ++progress;
				}
				if(maze[now_y][now_x-1] != 0 && maze[now_y][now_x-1] != maze[now_y][now_x])
				{
					path.push(make_pair(now_y, now_x-1));
					maze[now_y][now_x-1] = maze[now_y][now_x];
					if(maze[now_y][now_x-1] == 2 && (now_y&1) && ((now_x-1)&1)) ++progress;
				}
				if(maze[now_y][now_x+1] != 0 && maze[now_y][now_x+1] != maze[now_y][now_x])
				{
					path.push(make_pair(now_y, now_x+1));
					maze[now_y][now_x+1] = maze[now_y][now_x];
					if(maze[now_y][now_x+1] == 2 && (now_y&1) && ((now_x+1)&1)) ++progress;
				}
			}
		}
		setpos(0, 0);
		for(int i = 0; i < 21; ++i)
		{
			for(int j = 0;j < 81; ++j)
			{
				if(maze[i][j] == 0) setcolor(0);
				else setcolor(255);
				printf(" ");
			}
			printf("\n");
		}
		setcolor(241);
		int x = 0, y = 1;
		bool auto_find = false;
		char player = '@', move;
		setpos(x, y); printf("%c", player);
		while((move = getch()))
		{
			setpos(x, y); printf(" ");
			move = toupper(move);
			switch(move)
			{
				case 'W':
					if(maze[y-1][x] == 2)
						--y;
					break;
				case 'S':
					if(maze[y+1][x] == 2)
						++y;
					break;
				case 'A':
					if(maze[y][x-1] == 2)
						--x;
					break;
				case 'D':
					if(maze[y][x+1] == 2)
						++x;
					break;
				case 'F':
					auto_find = true;
					break;
			}
			if(auto_find)
			{
				setcolor(224);
				intint origin_point = make_pair(y, x);
				queue<intint> find_road;
				int auto_x = x,auto_y = y;
				maze[auto_y][auto_x] = -10000;
				find_road.push(make_pair(auto_y, auto_x));
				while(make_pair(auto_y,auto_x) != EXIT)
				{
					intint now = find_road.front(); find_road.pop();
					auto_y = now.first; auto_x = now.second;
					if(maze[auto_y-1][auto_x] == 2)
					{
						maze[auto_y-1][auto_x] = maze[auto_y][auto_x]+1;
						find_road.push(make_pair(auto_y-1, auto_x));
					}
					if(maze[auto_y+1][auto_x] == 2)
					{
						maze[auto_y+1][auto_x] = maze[auto_y][auto_x]+1;
						find_road.push(make_pair(auto_y+1, auto_x));
					}
					if(maze[auto_y][auto_x-1] == 2)
					{
						maze[auto_y][auto_x-1] = maze[auto_y][auto_x]+1;
						find_road.push(make_pair(auto_y, auto_x-1));
					}
					if(maze[auto_y][auto_x+1] == 2)
					{
						maze[auto_y][auto_x+1] = maze[auto_y][auto_x]+1;
						find_road.push(make_pair(auto_y, auto_x+1));
					}
				}
				while(make_pair(auto_y, auto_x) != origin_point)
				{
					if(maze[auto_y-1][auto_x] < maze[auto_y][auto_x])
					{
						maze[auto_y][auto_x] = -1;
						--auto_y;
						continue;
					}
					if(maze[auto_y+1][auto_x] < maze[auto_y][auto_x])
					{
						maze[auto_y][auto_x] = -1;
						++auto_y;
						continue;
					}
					if(maze[auto_y][auto_x-1] < maze[auto_y][auto_x])
					{
						maze[auto_y][auto_x] = -1;
						--auto_x;
						continue;
					}
					if(maze[auto_y][auto_x+1] < maze[auto_y][auto_x])
					{
						maze[auto_y][auto_x] = -1;
						++auto_x;
						continue;
					}
				}
				while(make_pair(y,x) != EXIT)
				{
					setpos(x, y); printf(" ");
					maze[y][x] = 0;
					if(maze[y-1][x] == -1) --y;
					else if(maze[y+1][x] == -1) ++y;
					else if(maze[y][x-1] == -1) --x;
					else if(maze[y][x+1] == -1) ++x;
					setpos(x, y); printf("%c", player);
					Sleep(100);
				}
				break;
			}
			setpos(x, y); printf("%c", player);
			if(make_pair(y,x) == EXIT) break;
			setpos(EXIT.second, EXIT.first); printf(" ");
		}
		setcolor(0); system("pause");
	}
}