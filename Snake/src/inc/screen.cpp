#include<windows.h>
#include<iostream>
#include<cstdlib>
#include<string>
#include<cstring>
#include<ctime>
#include<cstdio>
#include<fstream>
#include<algorithm>
#include<conio.h>
#include<cmath>

#include "screen.h"
#include "block.h"

using namespace std;

void screen::coutat(block* place) //移动光标
{
	COORD position{ short((place->x)*2) , short(place->y) };
	HANDLE where = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(where, position);
}

void screen::Getscreensize() //获取地图大小
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    Screen.x = (csbi.srWindow.Right - csbi.srWindow.Left + 1)/2;
    Screen.y = (csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
}

void screen::resetMapsize() //更改地图尺寸
{
	if(Gameinfo.Map_width>Screen.x-18)
		Gameinfo.Map_width=Screen.x-18;
	if(Gameinfo.Map_height>Screen.y-2)
		Gameinfo.Map_height=Screen.y-2;
}

void screen::printfood(int n) //打印食物
{
	vector<food>::iterator it=Gamefood.begin()+n;
	coutat(it->foodblock);
	switch (it->score) 
	{
		case 1:
			printf("\033[46m&&\033[0m");
			break;
		case 2:
			printf("\033[45m$$\033[0m");
			break;
		case 3:
			printf("\033[43m@@\033[0m");
			break;
		default:
			break;
	}
	Mapblock.x=Gameinfo.Map_width+2;
	Mapblock.y=15;
	coutat(&Mapblock);
}

void screen::printsnake(int* add) //打印蛇
{
	if(*add==0) moveSnake();
	else {addSnake();*add=0;}
}

void screen::printmap() //打印地图
{
	system("cls");
	int Width=Gameinfo.Map_width;
	int Height=Gameinfo.Map_height;
	for(int i=0;i<4;i++)
	{
		switch(Gameinfo.Map_wall[i])
		{
		case 1:
			Wall[i]='X';
			break;
		case 0:
			if(i==0 || i==1) Wall[i]='=';
			else Wall[i]='|';
			break;
		default:
			break;
		}
	}
	//打印边框
	//上
	Mapblock.x=0;
	Mapblock.y=0;
	coutat(&Mapblock);
	printf("\033[44m##\033[0m");
    for(int i=1;i<Width;++i) printf("\033[44m%c%c\033[0m",Wall[0],Wall[0]);
	printf("\033[44m##\033[0m");
	//下
	Mapblock.y=Height;
	coutat(&Mapblock);
	printf("\033[44m##\033[0m");
	for(int i=1;i<Width;++i) printf("\033[44m%c%c\033[0m",Wall[1],Wall[1]);
	printf("\033[44m##\033[0m");
	//左
	Mapblock.x=0;
	Mapblock.y=0;
	for(int i=1;i<Height;++i)
	{
		Mapblock.y++;
		coutat(&Mapblock);
		printf("\033[44m%c%c\033[0m",Wall[2],Wall[2]);
	}
	//右
	Mapblock.x=Width;
	Mapblock.y=0;
	for(int i=1;i<Height;++i)
	{
		Mapblock.y++;
		coutat(&Mapblock);
		printf("\033[44m%c%c\033[0m",Wall[3],Wall[3]);
	}
	//打印障碍物
	for(vector<block>::iterator it=Gameinfo.Map_object.begin();it!=Gameinfo.Map_object.end();it++)
	{
		Mapblock.x=it->x;
		Mapblock.y=it->y;
		coutat(&Mapblock);
		printf("\033[44mXX\033[0m");
	}

}

void screen::readrecord()	//读取记录，runGame复现
{
	system("cls");
	vector<string> inlinenames;
	string tempname,recordname;
	ifstream fin_recnames("record\\recordlist.txt");
	
	while(getline(fin_recnames,tempname))
		inlinenames.push_back(tempname);
	
	fin_recnames.close();
	int l=inlinenames.size();
	int l1=l/2;
	if(l%2==1) l1++;
	Mapblock.y=Screen.y/2-l/2;
	Mapblock.x=Screen.x/2-12;
	coutat(&Mapblock);
	if(l==0) {printf("No Record!");Sleep(1500);}
	else
	{
	printf("Please choose the record you want to watch: ");
	for(int i=0;i<l;i++)
	{
		Mapblock.y++;
		coutat(&Mapblock);
		if(i==0) printf("\033[42m ");
		printf(" %s",inlinenames[i].c_str());
		if(i==0) printf("\033[0m");
	}
	Mapblock.y=Screen.y/2-l/2+1;
	int chget=0;
	while(chget!=13)
	{
		chget=getch();
		if(chget==224)
		switch(chget=getch())
		{
		case 72:
			if(Mapblock.y>Screen.y/2-l/2+1)
			{
				int sy=Mapblock.y-Screen.y/2+l/2-1;
				coutat(&Mapblock);
				printf(" %s ",inlinenames[sy].c_str());
				sy--;
				Mapblock.y--;
				coutat(&Mapblock);
				printf("\033[42m  %s\033[0m",inlinenames[sy].c_str());
			}
			break;
		case 80:
			if(Mapblock.y<Screen.y/2+l1)
			{
				int sy=Mapblock.y-Screen.y/2+l/2-1;
				coutat(&Mapblock);
				printf(" %s ",inlinenames[sy].c_str());
				sy++;
				Mapblock.y++;
				coutat(&Mapblock);
				printf("\033[42m  %s\033[0m",inlinenames[sy].c_str());
			}
			break;
		}
	}
	if(chget==13)
	{
		recordname=inlinenames[Mapblock.y-Screen.y/2+l/2-1];
		chget=0;
	}
	inlinenames.clear();
	ifstream fin_rec("record\\"+recordname+".record");
	//读取记录名
	string recs;
	string config_name,map_name;
	fin_rec>>config_name>>map_name;
	Gameinfo.restart(config_name,map_name);
	resetMapsize();
	for(int i=0;i<4;i++)
	{
		switch(Gameinfo.Map_wall[i])
		{
		case 1:
			Wall[i]='X';
			break;
		case 0:
			if(i==0 || i==1) Wall[i]='=';
			if(i==2 || i==3) Wall[i]='|';
			break;
		default:
			break;
		}
	}
	Gamescore=0;
	//初始化
	snake* temp=new snake(Gameinfo.Map_width/2,Gameinfo.Map_height/2);
	temp->next=NULL;
	head=temp;
	temp=NULL;
	delete temp;
	TheFirstSnake();
	//打印附加信息
	Gamefood.push_back({0,0});
	printmap();
	Mapblock.x=Gameinfo.Map_width+2;
	Mapblock.y=5;
	coutat(&Mapblock);
	printf("SCORE : %d",Gamescore);
	Mapblock.y+=2;
	coutat(&Mapblock);
	printf("1-point-food :\033[46m &&\033[0m");
	Mapblock.y+=2;
	coutat(&Mapblock);
	printf("2-point-food :\033[43m $$\033[0m");
	Mapblock.y+=2;
	coutat(&Mapblock);
	printf("3-point-food :\033[41m @@\033[0m");
	Mapblock.y+=2;
	coutat(&Mapblock);
	printf("config: %s",Gameinfo.Config_name.c_str());
	Mapblock.y+=2;
	coutat(&Mapblock);
	printf("map: %s",Gameinfo.Map_name.c_str());
	Mapblock.y+=2;
	int sleeptime=750/Gameinfo.Game_difficulty;
	int keyput='=';
	int add=0,ch=0;
	int* a= new int;
	*a=0;
	delete a;
	//读取一个记录时
	while(getline(fin_rec,recs))
	{
		//随时按q退出 空格随时停止
		if(_kbhit()) 
		{
			ch=getch();
			if(ch=='q') break;
			else if(ch==' ') 
			{
				Mapblock.x=Gameinfo.Map_width+2;
				Mapblock.y=17;
				coutat(&Mapblock);
				system("pause");
			}
		}
		//判断记录时内动作
		for(int i=2;i<recs.length();i++)
			switch(recs[i])
			{
			case 'F': //生成食物
				Gamefood[0].foodblock->x=int(recs[++i])-48;
				Gamefood[0].foodblock->y=int(recs[++i])-48;
				Gamefood[0].score=int(recs[++i])-48;
				printfood(0);
				break;
				case 'w': //上
				if(head->diraction != 1)
					head->diraction = 0;
				break;
			case 's': //下
				if(head->diraction != 0)
					head->diraction = 1;
				break;
			case 'a': //左
				if(head->diraction != 3)
					head->diraction = 2;
				break;
			case 'd': //右
				if(head->diraction != 2)
					head->diraction = 3;
				break;
			case 'E': //吃食物
			{
				add++;
				Gamescore+=int(recs[++i])-48;
				block* b2=new block(Gameinfo.Map_width+2,5);
				coutat(b2);
				printf("SCORE : %d\n",Gamescore);
				delete b2;
				break;
			}
			case 'R': //刷新食物
			{
				Gamefood[0].foodblock->x=int(recs[++i])-48;
				Gamefood[0].foodblock->y=int(recs[++i])-48;
				coutat(Gamefood[0].foodblock);
				printf("  ");
				break;
			}
			default:
				break;
			}
		printsnake(&add);
		Sleep(sleeptime);
	}
	fin_rec.close();
	Mapblock.x=Gameinfo.Map_width+2;
	Mapblock.y=17;
	coutat(&Mapblock);
	coutat(&Mapblock);
	printf("Game over");
	Mapblock.y+=1;
	coutat(&Mapblock);
	printf("Press any key to quit!");
	*a=getch();
	//清理
	Gamefood.clear();
	Gamerecord.clear();
	while(head->next!=NULL)
	{
		deleteSnake();
	}
	}
}

void screen::restartGame() //重载游戏配置
{
	string map_name=Gameinfo.Map_name,config_name=Gameinfo.Config_name;
	string temp;
	int chget=0;
	std::cin.clear();
	std::cin.sync();
	string restartinfo[]={"  Change Config  ",
						  "   Change Map    ",
						  "      Quit       "};
	while(chget!=13)
	{
	system("cls");
	Mapblock.y=Screen.y/2-3;
	Mapblock.x=Screen.x/2-6;
	coutat(&Mapblock);
	printf("Current Config : %s",config_name.c_str());
	Mapblock.y++;
	coutat(&Mapblock);
	printf("Current Map    : %s",map_name.c_str());
	Mapblock.y+=2;
	Mapblock.x=Screen.x/2-4;
	coutat(&Mapblock);
	printf("\033[42m");
	printf("  Change Config  ");
	printf("\033[0m");
	Mapblock.y++;
	coutat(&Mapblock);
	printf("   Change Map    ");
	Mapblock.y++;
	coutat(&Mapblock);
	printf("      Quit       ");
	Mapblock.y=Screen.y/2;
	while(chget!=13)
	{
		chget=getch();
		if(chget==224)
		switch(chget=getch())
		{
		case 72:
			if(Mapblock.y>Screen.y/2)
			{
				int sy=Mapblock.y-Screen.y/2;
				coutat(&Mapblock);
				printf("%s",restartinfo[sy].c_str());
				sy--;
				Mapblock.y--;
				coutat(&Mapblock);
				printf("\033[42m%s\033[0m",restartinfo[sy].c_str());
			}
			break;
		case 80:
			if(Mapblock.y<Screen.y/2+2)
			{
				int sy=Mapblock.y-Screen.y/2;
				coutat(&Mapblock);
				printf("%s",restartinfo[sy].c_str());
				sy++;
				Mapblock.y++;
				coutat(&Mapblock);
				printf("\033[42m%s\033[0m",restartinfo[sy].c_str());
			}
			break;
		}
	}
	system("cls");
	if(Mapblock.y==Screen.y/2)
	{
	//读取配置
		chget=0;
		ifstream fin_configs("config\\configNames.txt");
		vector<string> inlinenames;
    	while(getline(fin_configs,temp))
			inlinenames.push_back(temp);
		fin_configs.close();
		int l=inlinenames.size();
		int l1=l/2;
		if(l%2==1) l1++;
		Mapblock.y=Screen.y/2-l/2;
		Mapblock.x=Screen.x/2-10;
		coutat(&Mapblock);
		printf("Please Choose Your Config Infomation : ");
		for(int i=0;i<l;i++)
		{
			Mapblock.y++;
			coutat(&Mapblock);
			if(i==0) printf("\033[42m ");
			printf(" %s",inlinenames[i].c_str());
			if(i==0) printf("\033[0m");
		}
		Mapblock.y=Screen.y/2-l/2+1;
		coutat(&Mapblock);
		while(chget!=13)
	{
		chget=getch();
		if(chget==224)
		switch(chget=getch())
		{
		case 72:
			if(Mapblock.y>Screen.y/2-l/2+1)
			{
				int sy=Mapblock.y-Screen.y/2+l/2-1;
				coutat(&Mapblock);
				printf(" %s ",inlinenames[sy].c_str());
				sy--;
				Mapblock.y--;
				coutat(&Mapblock);
				printf("\033[42m  %s\033[0m",inlinenames[sy].c_str());
			}
			break;
		case 80:
			if(Mapblock.y<Screen.y/2+l1)
			{
				int sy=Mapblock.y-Screen.y/2+l/2-1;
				coutat(&Mapblock);
				printf(" %s ",inlinenames[sy].c_str());
				sy++;
				Mapblock.y++;
				coutat(&Mapblock);
				printf("\033[42m  %s\033[0m",inlinenames[sy].c_str());
			}
			break;
		}
	}
		if(chget==13)
		{
			config_name=inlinenames[Mapblock.y-Screen.y/2+l/2-1];
			chget=0;
		}
		inlinenames.clear();
	}
	else if(Mapblock.y==Screen.y/2+1)
	{
	//读取地图
		chget=0;
		ifstream fin_maps("maps\\mapNames.txt");
		vector<string> inlinenames;
    	while(getline(fin_maps,temp))
			inlinenames.push_back(temp);
		fin_maps.close();
		int l=inlinenames.size();
		int l1=l/2+1;
		if(l%2==0) l1++;
		Mapblock.y=Screen.y/2-l/2;
		Mapblock.x=Screen.x/2-10;
		coutat(&Mapblock);
		printf("Please Choose Your Map Infomation : ");
		for(int i=0;i<l;i++)
		{
			Mapblock.y++;
			coutat(&Mapblock);
			if(i==0) printf("\033[42m ");
			printf(" %s",inlinenames[i].c_str());
			if(i==0) printf("\033[0m");
		}
		Mapblock.y=Screen.y/2-l/2+1;
		coutat(&Mapblock);
		while(chget!=13)
	{
		chget=getch();
		if(chget==224)
		switch(chget=getch())
		{
		case 72:
			if(Mapblock.y>Screen.y/2-l/2+1)
			{
				int sy=Mapblock.y-Screen.y/2+l/2-1;
				coutat(&Mapblock);
				printf(" %s ",inlinenames[sy].c_str());
				sy--;
				Mapblock.y--;
				coutat(&Mapblock);
				printf("\033[42m  %s\033[0m",inlinenames[sy].c_str());
			}
			break;
		case 80:
			if(Mapblock.y<Screen.y/2+l1)
			{
				int sy=Mapblock.y-Screen.y/2+l/2-1;
				coutat(&Mapblock);
				printf(" %s ",inlinenames[sy].c_str());
				sy++;
				Mapblock.y++;
				coutat(&Mapblock);
				printf("\033[42m  %s\033[0m",inlinenames[sy].c_str());
			}
			break;
		}
	}
		if(chget==13)
		{
			map_name=inlinenames[Mapblock.y-Screen.y/2+l/2-1];
			chget=0;
		}
		inlinenames.clear();
	}
	else if(Mapblock.y==Screen.y/2+2) break;
	system("cls");
	}
	Gameinfo.restart(config_name,map_name);
	ofstream fout_newname("src\\Namesave.txt");
	fout_newname<<config_name<<endl;
	fout_newname<<map_name<<endl;
	fout_newname.close();
	resetMapsize();
	if(Gameinfo.Game_randSeed==-1) srand(time(nullptr));
	else srand(Gameinfo.Game_randSeed);
	system("cls");
}

bool screen::sameblocks(block* block1,block* block2) //判断是否重叠
{
	if(block1->x==block2->x && block1->y==block2->y) return true;
	else return false;
}

bool screen::judgeBlock(block* blockjud,int k=1) //判断是否重叠
{
	int judn=0;
	//边界
	if((blockjud->x==0 || blockjud->x==Gameinfo.Map_width) && Gameinfo.Map_wall[2]==1) return false;
	if((blockjud->y==0 || blockjud->y==Gameinfo.Map_height) && Gameinfo.Map_wall[0]==1) return false;
	//障碍物
	for(vector<block>::iterator it=Gameinfo.Map_object.begin();it!=Gameinfo.Map_object.end();it++)
	{
		if(it->x==blockjud->x && it->y==blockjud->y) return false;
	}
	//蛇
	snake* p_find;
	if(k==1) p_find=head;
	else p_find=head->next;
	while(p_find->next!=NULL)
	{
		if(p_find->snakeblock->x==blockjud->x && p_find->snakeblock->y==blockjud->y) return false;
		else p_find=p_find->next;
	}
	return true;
}

int screen::judgeFood() //判断吃食物
{
	for(int i=0;i<Gameinfo.Game_foodNum;i++)
	{
		if(sameblocks(head->snakeblock,Gamefood[i].foodblock)) return i;
	}
	return -1;
}

void screen::TheFirstSnake() //初始化蛇
{
	snake* p_mov=head;
	for(int i=1;i<=4;i++)
	{
		snake* p_new=new snake(p_mov->snakeblock->x-1,p_mov->snakeblock->y);
		p_mov->next=p_new;
		p_new->next=NULL;
		p_mov=p_mov->next;
		coutat(p_new->snakeblock);
		printf("\033[42m()\033[0m");
		p_new=NULL;
		delete p_new;
	}
}

void screen::addSnake() //增加蛇头
{
	//蛇头换蛇身
	coutat(head->snakeblock);
	printf("\033[42m()\033[0m");
	snake* p_new;
	//按方向增加
	switch (head->diraction)
	{
	case 0:
		p_new=new snake(head->snakeblock->x,head->snakeblock->y-1);
		break;
	case 1:
		p_new=new snake(head->snakeblock->x,head->snakeblock->y+1);
		break;
	case 2:
		p_new=new snake(head->snakeblock->x-1,head->snakeblock->y);
		break;
	case 3:
		p_new=new snake(head->snakeblock->x+1,head->snakeblock->y);
		break;
	default:
		break;
	}
	p_new->diraction=head->diraction;
	p_new->next=head;
	head=p_new;
	//穿虚墙
	if(head->snakeblock->x==Gameinfo.Map_width && Gameinfo.Map_wall[3]==0) head->snakeblock->x=1;
	if(head->snakeblock->x==0 && Gameinfo.Map_wall[2]==0) head->snakeblock->x=Gameinfo.Map_width-1;
	if(head->snakeblock->y==Gameinfo.Map_height&& Gameinfo.Map_wall[1]==0) head->snakeblock->y=1;
	if(head->snakeblock->y==0 && Gameinfo.Map_wall[0]==0) head->snakeblock->y=Gameinfo.Map_height-1;
	//新蛇头
	coutat(head->snakeblock);
	printf("\033[42m[]\033[0m");
}

void screen::deleteSnake() //删除蛇尾
{
	snake* p_mov=head;
	while(p_mov->next->next!=NULL) p_mov=p_mov->next;
	snake* p_de=p_mov->next;
	p_mov->next=NULL;
	coutat(p_de->snakeblock);
	printf("  ");
	delete p_de;
}

void screen::moveSnake() //移动蛇
{
	deleteSnake();
	addSnake();
}

void screen::refreshfood(int n,int rt,int k=1) //刷新食物
{
	if(k==1)
	{
		coutat(Gamefood[n].foodblock);
		printf("  ");
	}
	//刷新分数
	int pointfreq=rand()%10;
	if(pointfreq>=0 && pointfreq<Gameinfo.Game_foodRand[1]) Gamefood[n].score=1;
	else if(pointfreq>=Gameinfo.Game_foodRand[1] && pointfreq<Gameinfo.Game_foodRand[1]+Gameinfo.Game_foodRand[2]) Gamefood[n].score=2;
	else if(pointfreq>=Gameinfo.Game_foodRand[1]+Gameinfo.Game_foodRand[2] && pointfreq<10) Gamefood[n].score=3;
	//刷新位置
	while(1)
	{
		Gamefood[n].foodblock->x=rand()%(Gameinfo.Map_width-1)+1;
		Gamefood[n].foodblock->y=rand()%(Gameinfo.Map_height-1)+1;
		if(judgeBlock(Gamefood[n].foodblock)) break;
	}
	//刷新重置时间
	Gamefood[n].foodtime=rt+20*Gameinfo.Game_difficulty;
	Gamerecord[rt]=Gamerecord[rt]+'F'+char((Gamefood[n].foodblock->x)+48)+char((Gamefood[n].foodblock->y)+48)+char(Gamefood[n].score+48);
}

void screen::saveRecode() //保存记录
{
	system("cls");
	ifstream fin_existnames("record\\recordlist.txt");
	string temp;
	vector<string> inlinenames;
	while(getline(fin_existnames,temp)) inlinenames.push_back(temp);
	fin_existnames.close();
	if(inlinenames.size()>=25)
	{
		inlinenames.erase(inlinenames.begin());
		ofstream fout_existnames("record\\recordlist.txt");
		for(vector<string>::iterator it=inlinenames.begin();it!=inlinenames.end();it++)
			fout_existnames<<*it<<endl;
	}
	inlinenames.clear();
	printf("Please input your record name:\n");
	string recordname;
	getline(std::cin,recordname);
	if(recordname!="q")
	{
		ofstream fout_record("record\\"+recordname+".record");
		int rec=1;
		fout_record<<Gameinfo.Config_name<<' '<<Gameinfo.Map_name<<endl;
		for(vector<string>::iterator it=Gamerecord.begin();it!=Gamerecord.end();it++)
		{
			fout_record<<rec<<*it<<endl;
			rec++;
		}
		fout_record.close();
		ofstream fout_savename("record\\recordlist.txt",ios::out|ios::app);
		fout_savename<<recordname<<endl;
		fout_savename.close();
	}
	
}	

void screen::runGame() //开始运行游戏
{
	for(int i=0;i<4;i++)
	{
		switch(Gameinfo.Map_wall[i])
		{
		case 1:
			Wall[i]='X';
			break;
		case 0:
			if(i==0 || i==1) Wall[i]='=';
			else Wall[i]='|';
			break;
		default:
			break;
		}
	}
	//初始化蛇
	snake* temp=new snake(Gameinfo.Map_width/2,Gameinfo.Map_height/2);
	temp->next=NULL;
	head=temp;
	temp=NULL;
	delete temp;
	coutat(head->snakeblock);
	//打印并初始化信息
	Gamescore=0;
	int sleeptime=750/Gameinfo.Game_difficulty;
	int keyput='=';
	int add=0,foodplace=-1;
	int recordtime=0;//以记录时为单位，每一记录时刷新一次蛇
	Gamerecord.push_back(":");
	//初始化食物
	for(int i=0;i<Gameinfo.Game_foodNum;i++)
	{
		Gamefood.push_back({0,0,1});
		refreshfood(i,recordtime);
	}
	printmap();
	TheFirstSnake();
	//打印信息
	Mapblock.x=Gameinfo.Map_width+2;
	Mapblock.y=5;
	coutat(&Mapblock);
	printf("SCORE : %d",Gamescore);
	Mapblock.y+=2;
	coutat(&Mapblock);
	printf("1-point-food :\033[46m &&\033[0m");
	Mapblock.y+=2;
	coutat(&Mapblock);
	printf("2-point-food :\033[45m $$\033[0m");
	Mapblock.y+=2;
	coutat(&Mapblock);
	printf("3-point-food :\033[43m @@\033[0m");
	Mapblock.y+=2;
	coutat(&Mapblock);
	printf("config: %s",Gameinfo.Config_name.c_str());
	Mapblock.y+=2;
	coutat(&Mapblock);
	printf("map   : %s",Gameinfo.Map_name.c_str());
	for(int i=0;i<Gameinfo.Game_foodNum;i++)
		printfood(i);
	coutat(head->snakeblock);
	printf("\033[42m[]\033[0m");
	Mapblock.x=Gameinfo.Map_width+2;
	Mapblock.y=17;
	coutat(&Mapblock);
	printf("READY!");
	Sleep(1500);
	coutat(&Mapblock);
	printf("GO!   ");
	Sleep(500);
	coutat(&Mapblock);
	printf("      ");
	int* a= new int;
	*a=0;
	printsnake(a);
	delete a;
	while(keyput!='q')
	{
		Sleep(sleeptime);
		//监听键盘
		if (_kbhit())
		{
			keyput = getch();
			Gamerecord[recordtime]+=char(keyput);
			if(char(keyput)=='q') break;
			else switch (keyput)
			{
			case 119: //上
				if(head->diraction != 1)
					head->diraction = 0;
				break;
			case 115: //下
				if(head->diraction != 0)
					head->diraction = 1;
				break;
			case 97: //左
				if(head->diraction != 3)
					head->diraction = 2;
				break;
			case 100: //右
				if(head->diraction != 2)
					head->diraction = 3;
				break;
			case ' ':
				system("pause");
				coutat(&Mapblock);
				printf("                   ");
				break;
			default:
				break;
			}

		}
		printsnake(&add);
		//判断是否撞墙
		if(!judgeBlock(head->snakeblock,0)) break;
		//判断是否吃食物
		if((foodplace=judgeFood())!=-1) 
		{
			add++;
			Gamerecord[recordtime]=Gamerecord[recordtime]+'E'+char(Gamefood[foodplace].score+48);
			Gamescore+=Gamefood[foodplace].score;
			refreshfood(foodplace,recordtime,0);
			printfood(foodplace);
			block* b2=new block(Gameinfo.Map_width+2,5);
			coutat(b2);
			printf("SCORE : %d\n",Gamescore);
			delete b2;
		}
		//判断食物刷新
		for(int i=0;i<Gameinfo.Game_foodNum;i++)
			if(recordtime==Gamefood[i].foodtime) 
			{
				Gamerecord[recordtime]=Gamerecord[recordtime]+'R'+char((Gamefood[i].foodblock->x)+48)+char((Gamefood[i].foodblock->y)+48);
				refreshfood(i,recordtime);
				printfood(i);
			}
		Mapblock.x=Gameinfo.Map_width+2;
		Mapblock.y=17;
		coutat(&Mapblock);
		//保存记录，增加记录时
		recordtime++;
		Gamerecord.push_back(":");
	}
	//结束信息
	Mapblock.x=Gameinfo.Map_width+2;
	Mapblock.y=17;
	coutat(&Mapblock);
	printf("GAME OVER !          ");
	Mapblock.y+=2;
	coutat(&Mapblock);
	//判断是否破纪录
	int rank=0;
	ifstream fin_rank("src\\Rank.txt");
	fin_rank>>rank;
	if(Gamescore>rank) 
	{
		rank=Gamescore;
		printf("You've break the Score!!!");
	}
	fin_rank.close();
	ofstream fout_rank("src\\Rank.txt");
	fout_rank<<rank;
	fout_rank.close();
	Mapblock.y+=2;
	coutat(&Mapblock);
	printf("Press 'b' to save your game.\n");
	std::cin.clear();
	std::cin.sync();
	Sleep(2000);
	//判断是否保存记录
	keyput=getch();
	if(char(keyput)=='b') saveRecode();
	else system("cls");
	//清理
	Gamefood.clear();
	Gamerecord.clear();
	while(head->next!=NULL)
	{
		deleteSnake();
	}
	Gamescore=0;
}

void screen::NewMapinfoprint() //打印新建地图信息
{
	Mapblock.x=Gameinfo.Map_width+2;
	Mapblock.y=5;
	coutat(&Mapblock);
	printf("Press 'q' to quit.");
	Mapblock.y+=1;
	coutat(&Mapblock);
	printf("Enter choice to configure map :");
	Mapblock.y+=1;
	coutat(&Mapblock);
	printf("w: set width;");
	Mapblock.y+=1;
	coutat(&Mapblock);
	printf("h: set height;");
	Mapblock.y+=1;
	coutat(&Mapblock);
	printf("o: add an obstacle;");
	Mapblock.y+=1;
	coutat(&Mapblock);
	printf("d: delete the last obstacle;");
	Mapblock.y+=1;
	coutat(&Mapblock);
	printf("e: set walls use 1 or 0;");
	Mapblock.y+=1;
	coutat(&Mapblock);
	printf("Press 'c' to confirm.");
	Mapblock.y+=2;
}

int readNum() //只读数字
{
	char cr;
	int notnum=0;
	int numout=0;
	string numo;
	getline(std::cin,numo);
	std::cin.clear();
	std::cin.sync();
	int l=numo.length();
	if(l<3 && l>=1)
	{
		for(string::iterator it=numo.begin();it!=numo.end();it++)
		{
			if(*it>='0' && *it<='9') numout=numout*10+int(*it)-48;
			else return -1;
		}	
		return numout;
	}
	else return -1;
}

void screen::NewMap() //新建地图
{
	system("cls");
	string name_config=Gameinfo.Config_name,name_map=Gameinfo.Map_name;
	Gameinfo.restart("default","default");
	resetMapsize();
    string Name,name;
	ifstream fin_existname("maps\\mapNames.txt");
	string temp;
	vector<string> existnames;
	while(fin_existname>>temp)
		existnames.push_back(temp);
	int editold=0;
	if(existnames.size()>=25) {printf("No more map allowed !\nYou can choose an existed file to edit !\n");editold=1;}
	//输入新地图名
	while(editold==0)
	{
		printf("Please input your new map's name: \n");
		getline(std::cin,Name);
		vector<string>::iterator findname=find(existnames.begin(),existnames.end(),Name);
		if(Name.size()==0) printf("You did not input anything !\n");
		else if(findname==existnames.end())
		{
			string::iterator it;
			for(it=Name.begin();it!=Name.end();it++)
			{
				if((*it<'a' || *it>'z') && (*it<'A' || *it>'z') && (*it<'0' || *it>'9') && *it!='_')
					{printf("Your Name can only consist of English letters, numbers or '_' !\n");break;}
			}
			if(it==Name.end()) break;
		}
		else
		{
			printf("This file is existed !\n");
		}	
	}
	while(editold==1)
	{
		printf("Please input your map's name: \n");
		for(vector<string>::iterator it=existnames.begin();it!=existnames.end();it++)
		{
			printf("%s\n",(*it).c_str());
		}
		getline(std::cin,Name);
		vector<string>::iterator findname=find(existnames.begin(),existnames.end(),Name);
		if(Name.size()==0) printf("You did not input anything !\n");
		else if(findname!=existnames.end())
		{
			break;
		}
		else
		{
			printf("This file is not existed !\n");
		}
	}
	fin_existname.close();
	existnames.clear();
    name="maps\\"+Name+".map";
	if(Name=="q") return;
	printmap();
	NewMapinfoprint();
	int ch=0;
	while(ch!='q')
	{
		ch=getch();
		switch(ch)
		{
		case 'q': //退出
			break;
		case 'w': //编辑宽度，按p加，按m减
			coutat(&Mapblock);
			printf("Seting Width......");
			Mapblock.y++;
			coutat(&Mapblock);
			printf("m to minus or p to plus.");
			Mapblock.y--;
			while(ch!='c' && ch!='q')
			{
				ch=getch();
				switch(ch)
				{
				case 'p': //加
					if(Gameinfo.Map_width<Screen.x-18)
					{
						Gameinfo.Map_width++;
						printmap();
						NewMapinfoprint();
						coutat(&Mapblock);
						printf("Seting Width......");
						Mapblock.y++;
						coutat(&Mapblock);
						printf("m to minus or p to plus.");
						Mapblock.y--;
					}
					break;
				case 'm': //减
					if(Gameinfo.Map_width>10)
					{
						Gameinfo.Map_width--;
						printmap();
						NewMapinfoprint();
						coutat(&Mapblock);
						printf("Seting Width......");
						Mapblock.y++;
						coutat(&Mapblock);
						printf("m to minus or p to plus.");
						Mapblock.y--;
					}
					break;
				default:
					break;
				}
			}
			coutat(&Mapblock);
			printf("                  ");
			Mapblock.y++;
			coutat(&Mapblock);
			printf("                         ");
			Mapblock.y--;
			break;
		case 'h': //编辑高度，按p加，按m减
			coutat(&Mapblock);
			printf("Seting Height......");
			Mapblock.y++;
			coutat(&Mapblock);
			printf("m to minus or p to plus.");
			Mapblock.y--;
			while(ch!='c' && ch!='q')
			{
				ch=getch();
				switch(ch)
				{
				case 'p': //加
					if(Gameinfo.Map_height<Screen.y-2)
					{
						Gameinfo.Map_height++;
						printmap();
						NewMapinfoprint();
						coutat(&Mapblock);
						printf("Seting Height......");
						Mapblock.y++;
						coutat(&Mapblock);
						printf("m to minus or p to plus.");
						Mapblock.y--;
					}
					break;
				case 'm': //减
					if(Gameinfo.Map_height>5)
					{
						Gameinfo.Map_height--;
						printmap();
						NewMapinfoprint();	
						coutat(&Mapblock);
						printf("Seting Height......");
						Mapblock.y++;
						coutat(&Mapblock);
						printf("m to minus or p to plus.");
						Mapblock.y--;
					}
					break;
				default:
					break;
				}
			}
			coutat(&Mapblock);
			printf("                   ");
			Mapblock.y++;
			coutat(&Mapblock);
			printf("                         ");
			Mapblock.y--;
			break;
		case 'o': //编辑障碍物
		{
			if(Gameinfo.Map_obNum>=15)
			{
				coutat(&Mapblock);
				printf("No More obstacle allowed !");
				Sleep(1000);
				coutat(&Mapblock);
				printf("                          ");
				break;
			}
			int x=0,y=0;
			while(ch!='q')
			{
			
			while(ch!='q')
			{
				//输入坐标
				coutat(&Mapblock);
				printf("X=  ");
				ch=readNum();
				if(ch<Gameinfo.Map_width && ch>0) 
				{
					x=ch;
					break;
				}
				else
				{
					Mapblock.y+=1;
					coutat(&Mapblock);
					printf("Your input is illegal!");
					Sleep(1000);
					coutat(&Mapblock);
					printf("                      ");
					Mapblock.y--;
					coutat(&Mapblock);
					printf("                      ");
					ch=0;
				}
			}
			Mapblock.y++;
			while(ch!='q')
			{
				coutat(&Mapblock);
				printf("Y=  ");
				ch=readNum();
				if(ch<Gameinfo.Map_width && ch>0) 
				{
					y=ch;
					break;
				}
				else
				{
					Mapblock.y+=1;
					coutat(&Mapblock);
					printf("Your input is illegal!");
					Sleep(1000);
					coutat(&Mapblock);
					printf("                      ");
					Mapblock.y--;
					coutat(&Mapblock);
					printf("                      ");
					ch=0;
				}
			}
			if(x<=Gameinfo.Map_width/2 && x>=Gameinfo.Map_width/2-4 && y==Gameinfo.Map_height/2)
			{
				Mapblock.y+=1;
				coutat(&Mapblock);
				printf("Your input is illegal!");
				Sleep(1000);
				coutat(&Mapblock);
				printf("                      ");
				Mapblock.y--;
				coutat(&Mapblock);
				printf("                      ");
				Mapblock.y--;
				coutat(&Mapblock);
				printf("                      ");
				ch=0;
			}
			else break;
			}
			if(ch!='q')
			{
				Gameinfo.Map_obNum++;
				Gameinfo.Map_object.push_back({x,y});
				Mapblock.y=14;
				coutat(&Mapblock);
				printf("                      ");
				Mapblock.y++;
				coutat(&Mapblock);
				printf("                      ");
				Mapblock.y--;
				coutat(&Mapblock);
				printmap();
				NewMapinfoprint();
			}
			break;
		}
		case 'd': //删除最后一个障碍物
		{
			if(Gameinfo.Map_object.size()!=0)
			{
				Gameinfo.Map_object.pop_back();
				Gameinfo.Map_obNum--;
				printmap();
				NewMapinfoprint();
			}
			break;
		}
		case 'e': //编辑墙
		{
			std::cin.clear();
			std::cin.sync();
			while(ch!='q')
			{
				coutat(&Mapblock);
				printf("up: ");
				ch=getchar();
				if(ch=='1')
				{
					Gameinfo.Map_wall[0]=1;
					Wall[0]='X';
					break;
				}
				else if(ch=='0')
				{
					Gameinfo.Map_wall[0]=0;
					Wall[0]='=';
					break;
				}
				else if(ch=='q') break;
				else 
				{
					Mapblock.y+=1;
					coutat(&Mapblock);
					printf("Your input is illegal!");
					Sleep(1000);
					coutat(&Mapblock);
					printf("                      ");
					Mapblock.y--;
					coutat(&Mapblock);
					printf("                      ");
				}
			}
			Mapblock.y++;
			std::cin.clear();
			std::cin.sync();
			while(ch!='q')
			{
				coutat(&Mapblock);
				printf("down: ");
				ch=getchar();
				if(ch=='1')
				{
					Gameinfo.Map_wall[1]=1;
					Wall[1]='X';
					break;
				}
				else if(ch=='0')
				{
					Gameinfo.Map_wall[1]=0;
					Wall[1]='=';
					break;
				}
				else if(ch=='q') break;
				else 
				{
					Mapblock.y+=1;
					coutat(&Mapblock);
					printf("Your input is illegal!");
					Sleep(1000);
					coutat(&Mapblock);
					printf("                      ");
					Mapblock.y--;
					coutat(&Mapblock);
					printf("                      ");
				}
			}
			Mapblock.y++;
			std::cin.clear();
			std::cin.sync();
			while(ch!='q')
			{
				coutat(&Mapblock);
				printf("left: ");
				ch=getchar();
				if(ch=='1')
				{
					Gameinfo.Map_wall[2]=1;
					Wall[2]='X';
					break;
				}
				else if(ch=='0')
				{
					Gameinfo.Map_wall[2]=0;
					Wall[2]='|';
					break;
				}
				else if(ch=='q') break;
				else 
				{
					Mapblock.y+=1;
					coutat(&Mapblock);
					printf("Your input is illegal!");
					Sleep(1000);
					coutat(&Mapblock);
					printf("                      ");
					Mapblock.y--;
					coutat(&Mapblock);
					printf("                      ");
				}
			}
			Mapblock.y++;
			std::cin.clear();
			std::cin.sync();
			while(ch!='q')
			{
				coutat(&Mapblock);
				printf("right: ");
				ch=getchar();
				if(ch=='1')
				{
					Gameinfo.Map_wall[3]=1;
					Wall[3]='X';
					break;
				}
				else if(ch=='0')
				{
					Gameinfo.Map_wall[3]=0;
					Wall[3]='|';
					break;
				}
				else if(ch=='q') break;
				else 
				{
					Mapblock.y+=1;
					coutat(&Mapblock);
					printf("Your input is illegal!");
					Sleep(1000);
					coutat(&Mapblock);
					printf("                      ");
					Mapblock.y--;
					coutat(&Mapblock);
					printf("                      ");
				}
			}
			if(ch!='q')
			{
				Mapblock.y=19;
				coutat(&Mapblock);
				printf("                      ");
				Mapblock.y++;
				coutat(&Mapblock);
				printf("                      ");
				Mapblock.y++;
				coutat(&Mapblock);
				printf("                      ");
				Mapblock.y++;
				coutat(&Mapblock);
				printf("                      ");
				Mapblock.y=19;
				coutat(&Mapblock);
				printmap();
				NewMapinfoprint();
			}
			break;
		}
		default:
			break;
		}
	}
	//判断是否需要保存
	coutat(&Mapblock);
	printf("Do you want to save?[y/n]");
	while(ch!='y' && ch!='n')
		ch=getch();
	if(ch=='y')
	{
		ofstream fout_map(name);
		fout_map<<Gameinfo.Map_width<<' '<<Gameinfo.Map_height<<endl;
		fout_map<<Gameinfo.Map_wall[0]<<' '<<Gameinfo.Map_wall[1]<<' '<<Gameinfo.Map_wall[2]<<' '<<Gameinfo.Map_wall[3]<<endl;
		fout_map<<Gameinfo.Map_obNum<<endl;
		for(vector<block>::iterator it=Gameinfo.Map_object.begin();it!=Gameinfo.Map_object.end();it++)
			fout_map<<it->x<<' '<<it->y<<endl;
		fout_map.close();
		ofstream fout_name;
    	fout_name.open("maps\\mapNames.txt",ios::out|ios::app);
	    fout_name<<Name<<endl;
    	fout_name.close();
	}
	//回到原本配置
	Gameinfo.restart(name_config,name_map);
	resetMapsize();
}

void screen::configGame()	//开始界面
{
	Sleep(250);
	Getscreensize();
	resetMapsize();
	Gameinfo.restart(Gameinfo.Config_name,Gameinfo.Map_name);
	string startinfo[]={"          Start New Game          ",
	                    "View or Change Game Config and Map",
						"         Build New Config         ",
						"           Draw New Map           ",
						"           View Records           ",
						"               Quit               "};
	Mapblock.y=Screen.y/2-4;
	Mapblock.x=Screen.x/2-4;
	coutat(&Mapblock);
	printf(" Welcome to Snake");
	Mapblock.y+=2;
	Mapblock.x=Screen.x/2-8;
	coutat(&Mapblock);
	printf("\033[42m");
	printf("%s",startinfo[0].c_str());
	printf("\033[0m");
	Mapblock.y++;
	coutat(&Mapblock);
	printf("%s",startinfo[1].c_str());
	Mapblock.y++;
	coutat(&Mapblock);
	printf("%s",startinfo[2].c_str());
	Mapblock.y++;
	coutat(&Mapblock);
	printf("%s",startinfo[3].c_str());
	Mapblock.y++;
	coutat(&Mapblock);
	printf("%s",startinfo[4].c_str());
	Mapblock.y++;
	coutat(&Mapblock);
	printf("%s",startinfo[5].c_str());
	Mapblock.y=Screen.y/2-2;
	int startchoice=0;
	while(startchoice!=13)
	{
		startchoice=getch();
		if(startchoice==224)
		switch(startchoice=getch())
		{
		case 72:
		{
			if(Mapblock.y>Screen.y/2-2)
			{
				int sy=Mapblock.y-Screen.y/2+2;
				coutat(&Mapblock);
				printf("%s",startinfo[sy].c_str());
				Mapblock.y--;
				sy--;
				coutat(&Mapblock);
				printf("\033[42m%s\033[0m",startinfo[sy].c_str());
			}	
			break;
		}
		case 80:
		{
			if(Mapblock.y<Screen.y/2+3)
			{
				int sy=Mapblock.y-Screen.y/2+2;
				coutat(&Mapblock);
				printf("%s",startinfo[sy].c_str());
				Mapblock.y++;
				sy++;
				coutat(&Mapblock);
				printf("\033[42m%s\033[0m",startinfo[sy].c_str());
			}
			break;
		}
		default:
			break;
		}
	}
	if(startchoice==13)
	{
		switch(Mapblock.y-Screen.y/2+2)
		{
		case 0:
			runGame();
			break;
		case 1:
			restartGame();
			break;
		case 2:
			Gameinfo.Newconfig();
			break;
		case 3:
			NewMap();
			break;
		case 4:
			readrecord();
			break;
		case 5:
			exit(0);
			break;
		default:
			break;
		}
	}
	system("cls");
}
