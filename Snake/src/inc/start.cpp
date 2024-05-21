#include<fstream>
#include<cstdio>
#include<ctime>
#include<windows.h>
#include<cstdlib>
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<conio.h>

#include "block.h"
#include "start.h"
using namespace std;

start::start()
{
    cout<<"loading......"<<endl;
    ifstream fin_lastname("src\\Namesave.txt");
    fin_lastname>>Config_name>>Map_name;
    fin_lastname.close();
    ifstream fin_config,fin_map;
    string configName="config\\"+Config_name+".config";
    string mapName="maps\\"+Map_name+".map";
    //读取游戏配置
    fin_config.open(configName);
    if(fin_config)
    {
        fin_config>>Game_difficulty>>Game_randSeed>>Game_foodNum;
        float freq=0;
        for(int i=1;i<=3;++i)
        {
            fin_config>>freq;
            Game_foodRand[i]=int(freq*10);
        }
        fin_config.close();
    }
    else printf("load failed.");
    //读取地图配置
    fin_map.open(mapName);
    if(fin_map)
    {
        fin_map>>Map_width>>Map_height;
        for(int i=0;i<4;i++)
        {
            fin_map>>Map_wall[i];
        }
        fin_map>>Map_obNum;
        if(Map_obNum!=0)
        {
            for(int i=0;i<Map_obNum;i++)
            {
                int x;int y;
                fin_map>>x>>y;
                Map_object.push_back({x,y});
            }
        }
        fin_map.close();
    }
    else printf("load failed.");
    system("cls");
}

//新建配置 while作用直接看printf
int start::Newconfig()
{
    system("cls");
    string Name,name;
    printf("input q to quit.\n");
    ifstream fin_existname("config\\configNames.txt");
	string temp;
	vector<string> existnames;
	while(fin_existname>>temp)
		existnames.push_back(temp);
	int editold=0;
	if(existnames.size()>=25) {printf("No more map allowed !\nYou can choose an existed file to edit !\n");editold=1;}
	//输入新配置名
	while(editold==0)
	{
		printf("Please input your new config's name: \n");
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
		printf("Please input your config's name: \n");
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
    name="config\\"+Name+".config";
    ofstream fout(name);
    string ch;
    if(Name=="q") return 0;
    //输入难度
    while(ch!="q")
    {
        printf("\nPlease input your config difficulty from 1 to 10: \n");
        getline(cin,ch);
        if(ch=="q")
        {
            fout.close();
            if(remove(name.c_str())==0) printf("Your file will not save.\n");
            else printf("Your operation failed.\n");
            printf("You'll return to start scene.\n");
            Sleep(1000);
            return 0;
        }
        else if(ch.length()==1)
        {
            if(ch[0]>='1' && ch[0]<='9')
            {
                fout<<int(ch[0])-48<<endl;
                break;
            }
            else printf("Your input is illegal!\n");
        }
        else if(ch=="10")
        {
            fout<<10<<endl;
            break;
        }
        else
        {
            printf("Your input is illegal!\n");
        }
    }
    //输入随机种子
    while(ch!="q")
    {
        printf("\nPlease input your config random seed larger than 0 or -1: \n");
        getline(cin,ch);
        if(ch=="q")
        {
            fout.close();
            if(remove(name.c_str())==0) printf("Your file will not save.\n");
            else printf("Your operation failed.\n");
            printf("You'll return to start scene.\n");
            Sleep(1000);
            return 0;
        }
        else if(ch[0]=='-')
        {
            if(ch[1]=='1' && ch.length()==2) {fout<<-1<<endl;break;}
            else printf("Your input is illegal!\n");
        }
        else
        {
            int seedn=0,m=1;
            long long unsigned int l=ch.length();
            for(long long unsigned int i=l-1;i>0;--i)
            {
                if(ch[i]<'0' || ch[i]>'9')
                {
                    seedn=-10*m;
                    break;
                }
                else
                {
                    seedn+=(int(ch[i])-48)*m;
                    m*=10;
                } 
            }
            seedn+=(int(ch[0])-48)*m;
            if(seedn<0) printf("Your input is illegal!\n");
            else {fout<<seedn<<endl;break;}
        }
    }
    //输入食物数量
    while(ch!="q")
    {
        printf("\nPlease input your config food number from 1 to 5: \n");
        getline(cin,ch);
        if(ch=="q")
        {
            fout.close();
            if(remove(name.c_str())==0) printf("Your file will not save.\n");
            else printf("Your operation failed.\n");
            printf("You'll return to start scene.\n");
            Sleep(1000);
            return 0;
        }
        else if(ch.length()==1)
        {
            int foodn=int(ch[0])-48;
            if(foodn>=1 && foodn<=5) {fout<<foodn<<endl;break;}
            else printf("Your input is illegal!\n");
            
        }
        else
        {
            printf("Your input is illegal!\n");
        }
    }
    //输入食物概率
    while(ch!="q")
    {
        int foodfreq[4]={};
    while(ch!="q")
    {
        printf("\nPlease input your config 1-point-food frequency less than 1 which has one decimal place: \n");
        getline(cin,ch);
        if(ch=="q")
        {
            fout.close();
            if(remove(name.c_str())==0) printf("Your file will not save.\n");
            else printf("Your operation failed.\n");
            printf("You'll return to start scene.\n");
            Sleep(1000);
            return 0;
        }
        else if(ch.length()==1 && ch[0]=='0')
        {
            foodfreq[1]=0;
            break;
        }
        else if(ch.length()==3 && ch[0]=='0' && ch[1]=='.')
        {
            if(ch[2]>='0' && ch[2]<='9')
            {
                foodfreq[1]=int(ch[2])-48;
                break;
            }
        }
        else printf("Your input is illegal!\n");
    }
    while(ch!="q")
    {
        printf("\nPlease input your config 2-point-food frequency less than 1 which has one decimal place: \n");
        getline(cin,ch);
        if(ch=="q")
        {
            fout.close();
            if(remove(name.c_str())==0) printf("Your file will not save.\n");
            else printf("Your operation failed.\n");
            printf("You'll return to start scene.\n");
            Sleep(1000);
            return 0;
        }
        else if(ch.length()==1 && ch[0]=='0')
        {
            foodfreq[2]=0;
            break;
        }
        else if(ch.length()==3 && ch[0]=='0' && ch[1]=='.')
        {
            if(ch[2]>='0' && ch[2]<='9')
            {
                foodfreq[2]=int(ch[2])-48;
                break;
            }
        }
        else printf("Your input is illegal!\n");
    }
    while(ch!="q")
    {
        printf("\nPlease input your config 3-point-food frequency less than 1 which has one decimal place: \n");
        getline(cin,ch);
        if(ch=="q")
        {
            fout.close();
            if(remove(name.c_str())==0) printf("Your file will not save.\n");
            else printf("Your operation failed.\n");
            printf("You'll return to start scene.\n");
            Sleep(1000);
            return 0;
        }
        else if(ch.length()==1 && (ch[0]=='0' || ch[0]=='1'))
        {
            
            foodfreq[3]=int(ch[0])-48;
            break;
        }
        else if(ch.length()==3 && ch[0]=='0' && ch[1]=='.')
        {
            if(ch[2]>='0' && ch[2]<='9')
            {
                foodfreq[3]=int(ch[2])-48;
                break;
            }
        }
        else printf("Your input is illegal!\n");
    }
        foodfreq[0]=foodfreq[1]+foodfreq[2]+foodfreq[3];
        if(foodfreq[0]==10) 
            {fout<<"0."<<foodfreq[1]<<" 0."<<foodfreq[2]<<" 0."<<foodfreq[3]<<endl;break;}
        else printf("The sum of your inputs is not 1!\n");
    }
    fout.close();
    if(ch!="q")
    {
    printf("\nDo you want to save it?[y/n]\n"); 
    int chget=0;
    while(chget!='y' && chget!='n') chget=getch();
    if(chget=='y')
    {
    ofstream fout_name;
    fout_name.open("config\\configNames.txt",ios::out|ios::app);
    fout_name<<Name<<endl;
    fout_name.close();
    }
    else
    {
        remove(name.c_str());
    }
    }
    else remove(name.c_str());
    printf("You'll return to start scene.\n");
    Sleep(1000);
    system("cls");
    return 0;
}

void coutat(block* place) //移动光标
{
	COORD position{ short((place->x)*2) , short(place->y) };
	HANDLE where = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(where, position);
}

//重载配置
void start::restart(string configname,string mapname)
{
    printf("loading......");
    ifstream fin_config,fin_map;
    string configName="config\\"+configname+".config";
    string mapName="maps\\"+mapname+".map";
    //读取游戏配置
    fin_config.open(configName);
    if(fin_config)
    {
        fin_config>>Game_difficulty>>Game_randSeed>>Game_foodNum;
        float freq=0;
        for(int i=1;i<=3;++i)
        {
            fin_config>>freq;
            Game_foodRand[i]=int(freq*10);
        }
        fin_config.close();
        Config_name=configname;
    }
    else printf("load failed.");
    Map_object.clear();
    //读取地图配置
    fin_map.open(mapName);
    if(fin_map)
    {
        fin_map>>Map_width>>Map_height;
        for(int i=0;i<4;i++)
        {
            fin_map>>Map_wall[i];
        }
        fin_map>>Map_obNum;
        if(Map_obNum!=0)
        {
            for(int i=0;i<Map_obNum;i++)
            {
                int x;int y;
                fin_map>>x>>y;
                Map_object.push_back({x,y});
            }
        }
        fin_map.close();
        Map_name=mapname;
    }
    else printf("load failed.");
    system("cls");
}