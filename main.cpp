#include <iostream>
#include <cstdio>
#include <time.h>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
using namespace std;

class Blocks
{
    public:
    int x[6],y[6];
    Blocks(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
    {
        x[1]=x1;y[1]=y1;
        x[2]=x2;y[2]=y2;
        x[3]=x3;y[3]=y3;
        x[4]=x4;y[4]=y4;
    }
};
Blocks blocks[7]={{0,0,0,1,2,0,2,1},
                  {0,0,2,0,2,1,4,1},
                  {0,1,2,1,2,0,4,0},
                  {0,1,2,0,2,1,4,1},
                  {0,0,0,1,2,1,4,1},
                  {0,0,0,1,2,0,4,0},
                  {2,0,2,1,2,2,2,3}};

int MapStatus[50][50];
int Point;
int gameover;

void Pos(int x,int y)
{
    COORD pos;
	HANDLE hOutput;
    pos.X=x;
    pos.Y=y;
    hOutput=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOutput,pos);
}

void creatMap()
{

    system("mode con cols=60 lines=27");
    for(int i=0;i<=30;i+=2)
    {
        Pos(i,0);
        cout<<"□";
        Pos(i,24);
        cout<<"□";
        MapStatus[0][i]=1;
        MapStatus[24][i]=1;
    }
    for(int i=1;i<24;i++)
    {
        Pos(0,i);
        cout<<"□";
        Pos(30,i);
        cout<<"□";
        MapStatus[i][0]=1;
        MapStatus[i][30]=1;
    }
    for(int i=0;i<25;i++)
    {
        Pos(33,i);
        cout<<"|";
    }
    for(int i=34;i<59;i++)
    {
        Pos(i,8);
        cout<<"=";
        Pos(i,16);
        cout<<"=";
        Pos(i,24);
        cout<<"=";
    }
    Pos(42,1);
    cout<<"下个方块:";
    Pos(42,10);
    cout<<"得分:";
    Pos(42,12);
    cout<<Point;
    Pos(42,17);
    cout<<"操作方法:";
    Pos(40,19);
    cout<<"←向左移动";
    Pos(40,20);
    cout<<"→向右移动";
    Pos(40,21);
    cout<<"↑旋转";
    Pos(40,22);
    cout<<"↓向下加速";
    Pos(40,23);
    cout<<"F1键暂停";
}

void pau()
{
    Pos(0,25);
    system("pause");
    Pos(0,25);
    cout<<"                      ";
}

int BlockRandom()
{
    for(int i=0;i<4;i++)
    {
        Pos(42,3+i);
        cout<<"        ";
    }
    int dx,dy;
    srand((unsigned)time(NULL));
    int b = rand()%7;
    for(int i=1;i<=4;i++)
    {
        dx=blocks[b].x[i];
        dy=blocks[b].y[i];
        Pos(42+dx,3+dy);
        cout<<"■";
    }
    return b;
}

bool judge(int x,int y,Blocks &blo)
{
    int dx,dy;
    for(int i=1;i<=4;i++)
    {
        dx=blo.x[i];
        dy=blo.y[i];
        if(MapStatus[y+dy][x+dx]==1){
            if(y==1) gameover=1;
            return false;
        }
    }
    return true;
}

Blocks Change(int x,int y,int b,Blocks const &blo)
{
    int t,dx,dy;
    Blocks B=blo;
    int flag=0;
    if(b>0&&b<6)
    {
        for(int i=1;i<=4;i++)
        {
            t=blo.x[i];
            B.x[i]=4-blo.y[i]*2;
            B.y[i]=t/2;
            if(x+B.x[i]<=0||x+B.x[i]>=30)
            {flag=1;break;}
        }
    }
    else if(b==6)
    {
        for(int i=1;i<=4;i++)
        {
            t=blo.x[i];
            B.x[i]=6-blo.y[i]*2;
            B.y[i]=t/2;
            if(x+B.x[i]<=0||x+B.x[i]>=30)
            {flag=1;break;}
        }
    }
    for(int i=1;i<=4;i++)
    {
        dx=blo.x[i];
        dy=blo.y[i];
        Pos(x+dx,y+dy);
        cout<<" ";
    }
    return flag?blo:B;
}

void BlockDrop(int b)
{
    int dx,dy;
    int x=14,y=0;
    int f1,f2;
    int ifchange;
    int t;
    int flag=0;
    Blocks blo = blocks[b];
    while(1)
    {
        ifchange=0;
        f1=0,f2=0;
        t=300;
        Sleep(130);
        if(GetAsyncKeyState(VK_LEFT)&&y!=0)          //操作输入
        {
            f1-=2;
            t=130;
        }
        else if(GetAsyncKeyState(VK_RIGHT)&&y!=0)
        {
            f1+=2;
            t=130;
        }
        else if(GetAsyncKeyState(VK_DOWN)&&y!=0)
        {
            t=0;
            f2=1;
        }3
        else if(GetAsyncKeyState(VK_F1)&&y!=0)
        {
            pau();
        }
        else if(GetAsyncKeyState(VK_UP)&&y!=0)
        {
            ifchange=1;
            blo = Change(x,y,b,blo);
            t=150;
        }
        else
            f2=1;
        x += f1;y += f2;
        if(MapStatus[y][x+blo.x[4]]||MapStatus[y][x+blo.x[1]]) {x-=f1;f1=0;} //左右越界
        if(!judge(x,y,blo))                       //下落合法判定
        {
            if(gameover) return ;
            x -= f1;y-= f2;
            for(int i=1;i<=4;i++)               //落块标记
            {
                dx=blo.x[i];
                dy=blo.y[i];
                MapStatus[dy+y][dx+x]=1;
            }
            break;
        }
        Sleep(t);
        if(!ifchange&&flag)
        {
            for(int i=1;i<=4;i++)
            {
                dx=blo.x[i];
                dy=blo.y[i];
                Pos(x-f1+dx,y-f2+dy);
                cout<<" ";
            }
        }
        for(int i=1;i<=4;i++)
        {
            dx=blo.x[i];
            dy=blo.y[i];
            Pos(x+dx,y+dy);
            cout<<"■";
        }
        if(!flag)flag=1;
    }
}

void clearBlocks()
{
    int flag=0;
    int sum;
    for(int i=1;i<=23;i++)
    {
        sum=0;
        for(int j=2;j<30;j++)
        {
            sum += MapStatus[i][j];
        }
        if(sum == 14)
        {
            flag=1;
            Point += 10;
            for(int k=2;k<30;k++)
            {
                for(int l=i;l>=2;l--)
                {
                    MapStatus[l][k]=MapStatus[l-1][k];
                }
            }
        }
    }
    if(flag)
    {
        for(int i=1;i<=23;i++)
        {
            for(int j=2;j<30;j+=2)
            {
                Pos(j,i);
                if(MapStatus[i][j])
                    printf("■");
                else
                    printf(" ");
            }
        }
        Pos(42,12);
        cout<<Point;
    }
}

void StartGame()
{
    int d=BlockRandom();
    int b;
    Pos(6,10);
    cout<<"按SPACE键开始游戏";
    Pos(6,11);
    cout<<"游戏前请关闭输入法";
    getch();
    Pos(6,10);
    cout<<"                    ";
    Pos(6,11);
    cout<<"                    ";
    Pos(0,25);
    while(1)
    {
        b=BlockRandom();
        BlockDrop(d);
        if(gameover) return ;
        d=b;
        clearBlocks();
    }
}

void GameOver()
{
    Pos(10,10);
    cout<<"                    ";
    Pos(10,11);
    cout<<"                    ";
    Pos(10,12);
    cout<<"                    ";
    Pos(10,10);
    cout<<"游戏结束！！";
    Pos(10,11);
    cout<<"得分:"<<Point;
    Pos(10,12);
    cout<<"按F2重新开始";
    while(1)
    if(GetAsyncKeyState(VK_F2))
    {
        break;
    }
    system("cls");
}

int main()
{
    while(1)
    {
        Point=0;
        gameover=0;
        creatMap();
        Pos(0,25);
        StartGame();
        GameOver();
    }
    return 0;
}
