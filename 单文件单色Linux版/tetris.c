#include<stdio.h>
#include <stdlib.h>
#include<termios.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>

#define debug 0  //1开启参数函数，0关闭参数显示
#define FrameX  16  //游戏窗口左上角的X轴坐标
#define FrameY  4  //游戏窗口左上角的Y轴坐标
#define height  20 //游戏窗口的高度
#define width   10//游戏窗口的宽度
#define StartX  1+width/2//俄罗斯方块中心出现位置横坐标 
#define StartY  2//俄罗斯方块中心出现位置纵坐标
int Suspend = -1; 
int Line = 0;//消除的行数 
int Level = 1;//等级
int Score = 0;//积分
int a[height + 2][width + 2] = { 0 };//俄罗斯方块数据数组
typedef struct
{
    int kind_current;//当前方块种类
    int state_current;//当前方块状态
    int kind_next;//下一方块种类
    int state_next;//下一方块状态
    int left;//1为可左移，0为不可左移 
    int right;//1为可右移，0为不可右移
    int down;//1为可下移，0为不可下移
    int trans;//1为可旋转，0为不可旋转 
    int X;//中心方块横坐标
    int Y;//中心方块纵坐标
    int temp[7][4][5][5];//俄罗斯方块生成数组 
}MyTetris;
//[]*********[]*********[]**************[][]*******[][]***[][]
//[][][]***[][][]***[][][]***[][][][]*****[][]***[][]*****[][]
MyTetris tetris =
{
    0, 0, 0, 0, 1, 1, 1, 1, StartX, StartY,
    { {
        { { 0, 0, 0, 0, 0 }, { 0, 1, 0, 0, 0 }, { 0, 1, 1, 1, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 0, 1, 1, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 0, 0, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 1, 1, 1, 0 }, { 0, 0, 0, 1, 0 }, { 0, 0, 0, 0, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 1, 1, 0, 0 }, { 0, 0, 0, 0, 0 } }
    }, {
        { { 0, 0, 0, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 1, 1, 1, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 1, 1, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 0, 0, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 1, 1, 1, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 0, 0, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 1, 1, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 0, 0, 0 } }
    }, {
        { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 1, 0 }, { 0, 1, 1, 1, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 1, 1, 0 }, { 0, 0, 0, 0, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 1, 1, 1, 0 }, { 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 1, 1, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 0, 0, 0 } }
    }, {
        { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 1, 1, 1, 1 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 1, 0, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 1, 1, 1, 1 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 1, 0, 0 } }
    }, {
        { { 0, 0, 0, 0, 0 }, { 0, 1, 1, 0, 0 }, { 0, 0, 1, 1, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 1, 0 }, { 0, 0, 1, 1, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 0, 0, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 1, 1, 0, 0 }, { 0, 0, 1, 1, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 1, 0 }, { 0, 0, 1, 1, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 0, 0, 0 } },
    }, {
        { { 0, 0, 0, 0, 0 }, { 0, 0, 1, 1, 0 }, { 0, 1, 1, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 1, 0, 0, 0 }, { 0, 1, 1, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 0, 0, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 0, 1, 1, 0 }, { 0, 1, 1, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 1, 0, 0, 0 }, { 0, 1, 1, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 0, 0, 0 } },
    }, {
        { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 1, 1, 0 }, { 0, 0, 1, 1, 0 }, { 0, 0, 0, 0, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 1, 1, 0 }, { 0, 0, 1, 1, 0 }, { 0, 0, 0, 0, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 1, 1, 0 }, { 0, 0, 1, 1, 0 }, { 0, 0, 0, 0, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 1, 1, 0 }, { 0, 0, 1, 1, 0 }, { 0, 0, 0, 0, 0 } }
    } }
};
MyTetris *pt = &tetris;
pthread_mutex_t lock;
pthread_t tid_a, tid_b;

int AutoDown();
void Initialization();
void MakeTetris();
void PrintTetris();
void CleanOld();
void DrawNew();
void Transform();
void Judge_MoveAndTansform();
void JustForDebugging();
void PrintNextTetris();
void CleanLine();
int  KeyControl(int ch);
void GotoXY(int x, int y);
void Initial_terminal();
void Recovery_terminal();
void StartTimer();
void DrwaGameframe();
void FacePlate();
int GameOver();
void Explation();
void Regulation();
void Welcom();
void GamePlay();
void GameRun();

/****************************************
* 确定屏幕光标位置
****************************************/
void GotoXY(int x, int y)
{
    printf("\033[%d;%dH",y,x);
}
/****************************************
* 终端初始化
****************************************/
void Initial_terminal()
{
    printf("\033[?25l");//隐藏光标
    system("stty -icanon");//关缓冲
    system("stty -echo");//关回显
}
/****************************************
* 终端恢复
****************************************/
void Recovery_terminal()
{
    system("stty icanon");//恢复缓冲
    system("stty echo");//恢复回显
    printf("\033[?25h");//恢复光标
}
/****************************************
* 打印俄罗斯方块边框
****************************************/
void DrwaGameframe()
{
    int i = 0;
    GotoXY(FrameX + width - 5, FrameY - 2);
    printf("俄 罗 斯 方 块");
    GotoXY(FrameX, FrameY);
    for (i = 0; i < width + 2; i++)printf("[]");//打印上横框
    GotoXY(FrameX, FrameY + height + 1);
    for (i = 0; i < width + 2; i++)printf("[]");//打印下横框
    for (i = 1; i <= height; i++)
    {
        GotoXY(FrameX, FrameY + i);
        printf("[]");                            //打印左竖框
    }
    for (i = 1; i <= height; i++)
    {
        GotoXY(FrameX + 2 * width + 2, FrameY + i);
        printf("[]");                             //打印右竖框 
    }
    GotoXY(FrameX + 2 * width + 8, FrameY - 1);
    printf("Level:%d", Level);
    GotoXY(FrameX + 2 * width + 8, FrameY + 1);
    printf("积分:%d", Score);
    GotoXY(FrameX + 2 * width + 8, FrameY + 3);
    printf("消去的行数:%d", Line);
    fflush(stdout);
}
/****************************************
* 游戏开始界面
****************************************/
void FacePlate()
{
    printf("\n\n\n");
    printf("                           俄  罗  斯  方  块             \n");
    printf("\n\n");
    printf("                  []                         []           \n");
    printf("                  [][]   [][]       [][]     []         []\n");
    printf("                  []       [][]     [][]     []     [][][]\n");
    printf("                                             []           \n");
    printf("\n\n");
    printf("                         1.开始游戏   2.按键说明          \n");
    printf("\n\n");
    printf("                         3.游戏规则   4.退出游戏          \n");
    printf("\n\n");
    printf("                           请选择[1 2 3 4]:");
}
/****************************************
* 游戏结束界面
****************************************/
int GameOver()
{
    int n;
    system("clear");
    Recovery_terminal();
    printf("\n\n\n\n\n\n\n\n");
    printf("      [][][]      []       []    []   [][][][]        [][]    []      [] [][][][]   [][][]  \n");
    printf("    []           [][]     [] [][] []  []            []    []   []    []  []         []   [] \n");
    printf("    []   [][]   []  []   []   []   [] [][][]       []      []   []  []   [][][]     [][][]  \n");
    printf("    []     []  [][][][]  []   []   [] []            []    []     [][]    []         []   [] \n");
    printf("      [][][]  []      [] []   []   [] [][][][]        [][]        []     [][][][]   []    []\n");
    printf("\n\n\n\n\n");
    printf("                                       1:再玩一局          2:退出游戏                             ");
    printf("\n\n");
    printf("                                                 选择[1/2]:");
    scanf("%d", &n);
	return n;
}
/****************************************
* 游戏按键介绍界面
****************************************/
void Explation()
{
    system("clear");
    printf("\n\n\n");
    printf("                                    按键说明                \n\n\n");
    printf("                ************************************************\n");
    printf("                *   tip1: 玩家可以通过 A D方向键来移动方块   *\n");
    printf("                *                                              *\n");
    printf("                *   tip2: 通过 W 使方块旋转                    *\n");
    printf("                *                                              *\n");
    printf("                *   tip3: 通过 S 加速方块下落                  *\n");
    printf("                *                                              *\n");
    printf("                *   tip4: 按空格键暂停游戏，再按空格键继续     *\n");
    printf("                *                                              *\n");
    printf("                *   tip5: 按ESC退出游戏                        *\n");
    printf("                ************************************************\n");
    getchar();//按任意键返回主界面
    getchar();//按任意键返回主界面
    system("clear");
    Welcom();
}
/****************************************
* 游戏规则介绍界面
****************************************/
void Regulation()
{
    system("clear");
    GotoXY(0, 3);
    printf("                                         按键说明                     \n\n\n");
    printf("                **********************************************************\n");
    printf("                *   tip1: 不同形状的小方块从屏幕上方落下，玩家通过调整   *\n");
    printf("                *         方块的位置和方向，使他们在屏幕底部拼出完整的   *\n");
    printf("                *         一条或几条                                     *\n");
    printf("                *                                                        *\n");
    printf("                *   tip2: 每消除一行，积分增加100                        *\n");
    printf("                *                                                        *\n");
    printf("                *   tip3: 每累计1000分，会提升一个等级                   *\n");
    printf("                *                                                        *\n");
    printf("                *   tip4: 提升等级会使方块下落速度加快，游戏难度加大     *\n");
    printf("                **********************************************************\n");
    getchar();//按任意键返回主界面
    getchar();//按任意键返回主界面
    system("clear");
    Welcom();
}
/****************************************
* 游戏欢迎界面
****************************************/
void Welcom()
{
    int n = 0;
    system("clear");
    FacePlate();
    scanf("%d", &n);//输入选项
    switch (n)
    {
    case 1:GamePlay();break;//游戏开始 
    case 2:Explation(); break;//按键说明函数 
    case 3:Regulation(); break;//游戏规则 
    case 4:exit(0); break;//关闭游戏 
    }
}
/****************************************
* 游戏参数初始化
****************************************/
void Initialization()
{
    int i = 0, j = 0;
    Initial_terminal();
    pt->kind_current = rand() % 7;
    pt->kind_next = rand() % 7;
    pt->state_current = rand() % 4;
    pt->state_next = rand() % 4;
    pt->left = 1;
    pt->right = 1;
    pt->down = 1;
    pt->trans = 1;
    pt->X = StartX;
    pt->Y = StartY;
    Line = 0;
    Level = 1;
    Score = 0;
    for (i = 1; i <= height + 1; i++)
        for (j = 1; j <= width + 1; j++)a[i][j] = 0;
    for (i = 1; i <= width; i++)a[height + 1][i] = 1;
    for (i = 0; i <= height + 1; i++)
    {
        a[i][0] = 1;
        a[i][width + 1] = 1;
    }
}
/****************************************
* 生成俄罗斯方块
****************************************/
void MakeTetris()
{
    srand(time(NULL));
    pt->kind_current = pt->kind_next;
    pt->state_current = pt->state_next;
    pt->kind_next = rand() % 7;
    pt->state_next = rand() % 4;
}
/****************************************
* 打印出俄罗斯方块
****************************************/
void PrintTetris()
{
    int i = 0, j = 0;
    for (i = 0; i < 5; i++)
    for (j = 0; j < 5; j++)
        a[pt->Y - 2 + i][pt->X - 2 + j] += pt->temp[pt->kind_current][pt->state_current][i][j];
    for (i = 1; i <= height; i++)
    {
        GotoXY(FrameX + 2, FrameY + i);
        for (j = 1; j <= width; j++)
        if (a[i][j])printf("[]");
        else printf("  ");
    }
    fflush(stdout);
}
/****************************************
* 清除旧的俄罗斯方块
****************************************/
void CleanOld()
{
    int i = 0, j = 0;
    for (i = 0; i < 5; i++)
    for (j = 0; j < 5; j++)
    if (pt->temp[pt->kind_current][pt->state_current][i][j])
    {
        a[pt->Y - 2 + i][pt->X - 2 + j] = 0;
        GotoXY(FrameX + 2 * pt->X - 4 + 2 * j, FrameY + pt->Y - 2 + i);
        printf("  ");
    }
    fflush(stdout);
}
/****************************************
* 打印新的俄罗斯方块
****************************************/
void DrawNew()
{
    int i = 0, j = 0;
    for (i = 0; i < 5; i++)
    for (j = 0; j < 5; j++)
    if (pt->temp[pt->kind_current][pt->state_current][i][j])
    {
        a[pt->Y + i - 2][pt->X + j - 2] = pt->temp[pt->kind_current][pt->state_current][i][j];
        GotoXY(FrameX + 2 * pt->X - 4 + 2 * j, FrameY + pt->Y - 2 + i);
        printf("[]");
    }
    fflush(stdout);
}
/****************************************
* 旋转俄罗斯方块
****************************************/
void Transform()
{
    if (pt->state_current != 3)pt->state_current++;
    else pt->state_current = 0;
}
/****************************************
* 判断俄罗斯方块是否可移动和变形
****************************************/
void Judge_MoveAndTansform()
{
    int t;
    switch (pt->kind_current)
    {
    case 0:
    case 1:
    case 2:t = 4 * pt->kind_current + pt->state_current + 1; break;
    case 3:
    case 4:
    case 5:t = 2 * pt->kind_current + pt->state_current % 2 + 7; break;
    case 6:t = 19; break;
    }
    switch (t)
    {
    case 1:
        if (a[pt->Y - 1][pt->X - 2] == 0 && a[pt->Y][pt->X - 2] == 0)pt->left = 1; else pt->left = 0;
        if (a[pt->Y - 1][pt->X] == 0 && a[pt->Y][pt->X + 2] == 0)pt->right = 1; else pt->right = 0;
        if (a[pt->Y + 1][pt->X - 1] == 0 && a[pt->Y + 1][pt->X] == 0 && a[pt->Y + 1][pt->X + 1] == 0)pt->down = 1; else pt->down = 0;
        if (a[pt->Y - 1][pt->X] == 0 && a[pt->Y - 1][pt->X + 1] == 0 && a[pt->Y + 1][pt->X] == 0)pt->trans = 1; else pt->trans = 0;
        break;
    case 2:
        if (a[pt->Y - 1][pt->X - 1] == 0 && a[pt->Y][pt->X - 1] == 0 && a[pt->Y + 1][pt->X - 1] == 0)pt->left = 1; else pt->left = 0;
        if (a[pt->Y - 1][pt->X + 2] == 0 && a[pt->Y][pt->X + 1] == 0 && a[pt->Y + 1][pt->X + 1] == 0)pt->right = 1; else pt->right = 0;
        if (a[pt->Y + 2][pt->X] == 0 && a[pt->Y][pt->X + 1] == 0)pt->down = 1; else pt->down = 0;
        if (a[pt->Y][pt->X - 1] == 0 && a[pt->Y][pt->X + 1] == 0 && a[pt->Y + 1][pt->X + 1] == 0)pt->trans = 1; else pt->trans = 0;
        break;
    case 3:
        if (a[pt->Y][pt->X - 2] == 0 && a[pt->Y + 1][pt->X] == 0)pt->left = 1; else pt->left = 0;
        if (a[pt->Y][pt->X + 2] == 0 && a[pt->Y + 1][pt->X + 2] == 0)pt->right = 1; else pt->right = 0;
        if (a[pt->Y + 1][pt->X - 1] == 0 && a[pt->Y + 1][pt->X] == 0 && a[pt->Y + 2][pt->X + 1] == 0)pt->down = 1; else pt->down = 0;
        if (a[pt->Y - 1][pt->X] == 0 && a[pt->Y + 1][pt->X - 1] == 0 && a[pt->Y + 1][pt->X] == 0)pt->trans = 1; else pt->trans = 0;
        break;
    case 4:
        if (a[pt->Y - 1][pt->X - 1] == 0 && a[pt->Y][pt->X - 1] == 0 && a[pt->Y + 1][pt->X - 2] == 0)pt->left = 1; else pt->left = 0;
        if (a[pt->Y - 1][pt->X + 1] == 0 && a[pt->Y][pt->X + 1] == 0 && a[pt->Y + 1][pt->X + 1] == 0)pt->right = 1; else pt->right = 0;
        if (a[pt->Y + 2][pt->X - 1] == 0 && a[pt->Y + 2][pt->X] == 0)pt->down = 1; else pt->down = 0;
        if (a[pt->Y - 1][pt->X - 1] == 0 && a[pt->Y][pt->X - 1] == 0 && a[pt->Y][pt->X + 1] == 0)pt->trans = 1; else pt->trans = 0;
        break;
    case 5:
        if (a[pt->Y - 1][pt->X - 1] == 0 && a[pt->Y][pt->X - 2] == 0)pt->left = 1; else pt->left = 0;
        if (a[pt->Y - 1][pt->X + 1] == 0 && a[pt->Y][pt->X + 2] == 0)pt->right = 1; else pt->right = 0;
        if (a[pt->Y + 1][pt->X - 1] == 0 && a[pt->Y + 1][pt->X] == 0 && a[pt->Y + 1][pt->X + 1] == 0)pt->down = 1; else pt->down = 0;
        if (a[pt->Y + 1][pt->X] == 0)pt->trans = 1; else pt->trans = 0;
        break;
    case 6:
        if (a[pt->Y - 1][pt->X - 1] == 0 && a[pt->Y][pt->X - 1] == 0 && a[pt->Y + 1][pt->X - 1] == 0)pt->left = 1; else pt->left = 0;
        if (a[pt->Y - 1][pt->X + 1] == 0 && a[pt->Y][pt->X + 2] == 0 && a[pt->Y + 1][pt->X + 1] == 0)pt->right = 1; else pt->right = 0;
        if (a[pt->Y + 2][pt->X] == 0 && a[pt->Y + 1][pt->X + 1] == 0)pt->down = 1; else pt->down = 0;
        if (a[pt->Y][pt->X - 1] == 0)pt->trans = 1; else pt->trans = 0;
        break;
    case 7:
        if (a[pt->Y][pt->X - 2] == 0 && a[pt->Y + 1][pt->X - 1] == 0)pt->left = 1; else pt->left = 0;
        if (a[pt->Y][pt->X + 2] == 0 && a[pt->Y + 1][pt->X + 1] == 0)pt->right = 1; else pt->right = 0;
        if (a[pt->Y + 1][pt->X - 1] == 0 && a[pt->Y + 2][pt->X] == 0 && a[pt->Y + 1][pt->X + 1] == 0)pt->down = 1; else pt->down = 0;
        if (a[pt->Y - 1][pt->X] == 0)pt->trans = 1; else pt->trans = 0;
        break;
    case 8:
        if (a[pt->Y - 1][pt->X - 1] == 0 && a[pt->Y][pt->X - 2] == 0 && a[pt->Y + 1][pt->X - 1] == 0)pt->left = 1; else pt->left = 0;
        if (a[pt->Y - 1][pt->X + 1] == 0 && a[pt->Y][pt->X + 1] == 0 && a[pt->Y + 1][pt->X + 1] == 0)pt->right = 1; else pt->right = 0;
        if (a[pt->Y + 1][pt->X - 1] == 0 && a[pt->Y + 2][pt->X] == 0)pt->down = 1; else pt->down = 0;
        if (a[pt->Y][pt->X + 1] == 0)pt->trans = 1; else pt->trans = 0;
        break;
    case 9:
        if (a[pt->Y][pt->X - 2] == 0 && a[pt->Y - 1][pt->X] == 0)pt->left = 1; else pt->left = 0;
        if (a[pt->Y - 1][pt->X + 2] == 0 && a[pt->Y][pt->X + 2] == 0)pt->right = 1; else pt->right = 0;
        if (a[pt->Y + 1][pt->X - 1] == 0 && a[pt->Y + 1][pt->X] == 0 && a[pt->Y + 1][pt->X + 1] == 0)pt->down = 1; else pt->down = 0;
        if (a[pt->Y - 1][pt->X] == 0 && a[pt->Y + 1][pt->X] == 0 && a[pt->Y + 1][pt->X + 1] == 0)pt->trans = 1; else pt->trans = 0;
        break;
    case 10:
        if (a[pt->Y - 1][pt->X - 1] == 0 && a[pt->Y][pt->X - 1] == 0 && a[pt->Y + 1][pt->X - 1] == 0)pt->left = 1; else pt->left = 0;
        if (a[pt->Y - 1][pt->X + 1] == 0 && a[pt->Y][pt->X + 1] == 0 && a[pt->Y + 1][pt->X + 2] == 0)pt->right = 1; else pt->right = 0;
        if (a[pt->Y + 2][pt->X] == 0 && a[pt->Y + 2][pt->X + 1] == 0)pt->down = 1; else pt->down = 0;
        if (a[pt->Y][pt->X - 1] == 0 && a[pt->Y + 1][pt->X - 1] == 0 && a[pt->Y][pt->X + 1] == 0)pt->trans = 1; else pt->trans = 0;
        break;
    case 11:
        if (a[pt->Y][pt->X - 2] == 0 && a[pt->Y + 1][pt->X - 2] == 0)pt->left = 1; else pt->left = 0;
        if (a[pt->Y][pt->X + 2] == 0 && a[pt->Y + 1][pt->X] == 0)pt->right = 1; else pt->right = 0;
        if (a[pt->Y + 2][pt->X - 1] == 0 && a[pt->Y + 1][pt->X] == 0 && a[pt->Y + 1][pt->X + 1] == 0)pt->down = 1; else pt->down = 0;
        if (a[pt->Y - 1][pt->X - 1] == 0 && a[pt->Y - 1][pt->X] == 0 && a[pt->Y + 1][pt->X] == 0)pt->trans = 1; else pt->trans = 0;
        break;
    case 12:
        if (a[pt->Y - 1][pt->X - 2] == 0 && a[pt->Y][pt->X - 1] == 0 && a[pt->Y + 1][pt->X - 1] == 0)pt->left = 1; else pt->left = 0;
        if (a[pt->Y - 1][pt->X + 1] == 0 && a[pt->Y][pt->X + 1] == 0 && a[pt->Y + 1][pt->X + 1] == 0)pt->right = 1; else pt->right = 0;
        if (a[pt->Y][pt->X - 1] == 0 && a[pt->Y + 2][pt->X] == 0)pt->down = 1; else pt->down = 0;
        if (a[pt->Y - 1][pt->X + 1] == 0 && a[pt->Y][pt->X - 1] == 0 && a[pt->Y][pt->X + 1] == 0)pt->trans = 1; else pt->trans = 0;
        break;
    case 13:
        if (a[pt->Y][pt->X - 2] == 0)pt->left = 1; else pt->left = 0;
        if (a[pt->Y][pt->X + 3] == 0)pt->right = 1; else pt->right = 0;
        if (a[pt->Y + 1][pt->X - 1] == 0 && a[pt->Y + 1][pt->X] == 0 && a[pt->Y + 1][pt->X + 1] == 0 && a[pt->Y + 1][pt->X + 2] == 0)pt->down = 1; 
		else pt->down = 0;
        if (a[pt->Y - 1][pt->X] == 0 && a[pt->Y + 1][pt->X] == 0 && a[pt->Y + 2][pt->X] == 0)pt->trans = 1; else pt->trans = 0;
        break;
    case 14:
        if (a[pt->Y - 1][pt->X - 1] == 0 && a[pt->Y][pt->X - 1] == 0 && a[pt->Y + 1][pt->X - 1] == 0 && a[pt->Y + 2][pt->X - 1] == 0)pt->left = 1; 
		else pt->left = 0;
        if (a[pt->Y - 1][pt->X + 1] == 0 && a[pt->Y][pt->X + 1] == 0 && a[pt->Y + 1][pt->X + 1] == 0 && a[pt->Y + 2][pt->X + 1] == 0)pt->right = 1; 
		else pt->right = 0;
        if (a[pt->Y + 3][pt->X] == 0)pt->down = 1; else pt->down = 0;
        if (a[pt->Y][pt->X - 1] == 0 && a[pt->Y][pt->X + 1] == 0 && a[pt->Y][pt->X + 2] == 0)pt->trans = 1; else pt->trans = 0;
        break;
    case 15:
        if (a[pt->Y - 1][pt->X - 2] == 0 && a[pt->Y][pt->X - 1] == 0)pt->left = 1; else pt->left = 0;
        if (a[pt->Y - 1][pt->X + 1] == 0 && a[pt->Y][pt->X + 2] == 0)pt->right = 1; else pt->right = 0;
        if (a[pt->Y][pt->X - 1] == 0 && a[pt->Y + 1][pt->X] == 0 && a[pt->Y + 1][pt->X + 1] == 0)pt->down = 1; else pt->down = 0;
        if (a[pt->Y - 1][pt->X + 1] == 0 && a[pt->Y + 1][pt->X] == 0)pt->trans = 1; else pt->trans = 0;
        break;
    case 16:
        if (a[pt->Y - 1][pt->X] == 0 && a[pt->Y][pt->X - 1] == 0 && a[pt->Y + 1][pt->X - 1] == 0)pt->left = 1; else pt->left = 0;
        if (a[pt->Y - 1][pt->X + 2] == 0 && a[pt->Y][pt->X + 2] == 0 && a[pt->Y + 1][pt->X + 1] == 0)pt->right = 1; else pt->right = 0;
        if (a[pt->Y + 2][pt->X] == 0 && a[pt->Y + 1][pt->X + 1] == 0)pt->down = 1; else pt->down = 0;
        if (a[pt->Y - 1][pt->X - 1] == 0 && a[pt->Y - 1][pt->X] == 0)pt->trans = 1; else pt->trans = 0;
        break;
    case 17:
        if (a[pt->Y - 1][pt->X - 1] == 0 && a[pt->Y][pt->X - 2] == 0)pt->left = 1; else pt->left = 0;
        if (a[pt->Y - 1][pt->X + 2] == 0 && a[pt->Y][pt->X + 1] == 0)pt->right = 1; else pt->right = 0;
        if (a[pt->Y + 1][pt->X - 1] == 0 && a[pt->Y + 1][pt->X] == 0 && a[pt->Y][pt->X + 1] == 0)pt->down = 1; else pt->down = 0;
        if (a[pt->Y][pt->X + 1] == 0 && a[pt->Y + 1][pt->X + 1] == 0)pt->trans = 1; else pt->trans = 0;
        break;
    case 18:
        if (a[pt->Y - 1][pt->X - 2] == 0 && a[pt->Y][pt->X - 2] == 0 && a[pt->Y + 1][pt->X - 1] == 0)pt->left = 1; else pt->left = 0;
        if (a[pt->Y - 1][pt->X + 1] == 0 && a[pt->Y][pt->X + 1] == 0 && a[pt->Y + 1][pt->X + 1] == 0)pt->right = 1; else pt->right = 0;
        if (a[pt->Y + 1][pt->X - 1] == 0 && a[pt->Y + 2][pt->X] == 0)pt->down = 1; else pt->down = 0;
        if (a[pt->Y - 1][pt->X] == 0 && a[pt->Y - 1][pt->X + 1] == 0)pt->trans = 1; else pt->trans = 0;
        break;
    case 19:
        if (a[pt->Y][pt->X - 1] == 0 && a[pt->Y + 1][pt->X - 1] == 0)pt->left = 1; else pt->left = 0;
        if (a[pt->Y][pt->X + 2] == 0 && a[pt->Y + 1][pt->X + 2] == 0)pt->right = 1; else pt->right = 0;
        if (a[pt->Y + 2][pt->X] == 0 && a[pt->Y + 2][pt->X + 1] == 0)pt->down = 1; else pt->down = 0;
        pt->trans = 0;
        break;
    default:break;
    }
}
/****************************************
* 关键参数显示函数(用于调试)
****************************************/
#if debug
void JustForDebugging()
{
    int i = 0, j = 0;
    for (i = 0; i < height + 2; i++)
    {
        GotoXY(FrameX + 2 * width + 24, FrameY + i);
        for (j = 0; j < width + 2; j++)printf("%d", a[i][j]);
    }
    GotoXY(FrameX + 3 * width + 30, FrameY + 2);
    printf("X=%2d", pt->X);
    GotoXY(FrameX + 3 * width + 30, FrameY + 3);
    printf("Y=%2d", pt->Y);
    GotoXY(FrameX + 3 * width + 30, FrameY + 4);
    printf("Left=%d", pt->left);
    GotoXY(FrameX + 3 * width + 30, FrameY + 5);
    printf("Right=%d", pt->right);
    GotoXY(FrameX + 3 * width + 30, FrameY + 6);
    printf("Down=%d", pt->down);
    GotoXY(FrameX + 3 * width + 30, FrameY + 7);
    printf("Trans=%d", pt->trans);
    fflush(stdout);//清空输出缓冲区
}
#elif !debug
void JustForDebugging()
{
	
}
#endif
/****************************************
* 打印下一个俄罗斯方块
****************************************/
void PrintNextTetris()
{
    int i = 0, j = 0;
    GotoXY(FrameX + 2 * width + 8, FrameY + 5);
    printf("下一个方块:%2d", pt->kind_next);
    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
        {
            GotoXY(FrameX + 2 * width + 2 * j + 8, FrameY + 6 + i);
            if (pt->temp[pt->kind_next][pt->state_next][i][j])printf("[]");
            else printf("  ");
        }
    fflush(stdout);
}
/****************************************
* 清除满行
****************************************/
void CleanLine()
{
    int i = height, j = 1, k = 0, sum = 0;
    for (i = height; i > 0; i--)
    {
        sum = 0;
        for (j = 1; j <= width; j++)sum += a[i][j];
        if (sum == width)
        {
            Line++;
            for (k = i; k > 1; k--)
            for (j = 1; j <= width; j++)a[k][j] = a[k - 1][j];
            i++;
        }
    }
    Level = 1 + Line / 10;
    Score = Line * 100;
    GotoXY(FrameX + 2 * width + 8, FrameY - 1);
    printf("Level:%d", Level);
    GotoXY(FrameX + 2 * width + 8, FrameY + 1);
    printf("积分:%d", Score);
    GotoXY(FrameX + 2 * width + 8, FrameY + 3);
    printf("消去的行数:%d", Line);
    fflush(stdout);
}
/****************************************
* 游戏开始函数
****************************************/
void GamePlay()
{ 
    system("clear");    
    Initialization();
    DrwaGameframe();
    MakeTetris();
    DrawNew();
    PrintNextTetris();
}
/****************************************
* 自动下落函数
****************************************/
int AutoDown()
{
    Judge_MoveAndTansform();
    JustForDebugging();
    if(pt->down)
    {    
        CleanOld();
        pt->Y++;
        DrawNew();
    }
    JustForDebugging();
    if(!pt->down)
    {
        CleanLine();
        pt->X=StartX;
        pt->Y=StartY;
        MakeTetris();
        Judge_MoveAndTansform();
        if(!pt->down)
        {
            PrintTetris();
            sleep(1);
            return 1;          
        }
        PrintTetris(); 
        PrintNextTetris();    
    }
    JustForDebugging();
    return 0;
}
/****************************************
* 键盘输入判定
****************************************/
int KeyControl(int ch)
{
	if(-1==Suspend||32==ch)
	{
        Judge_MoveAndTansform();
        switch (ch)
            {
                case 'w':if(pt->trans){CleanOld();Transform();DrawNew();}break;//变形
                case 'a':if(pt->left){CleanOld();pt->X--;DrawNew();}break;//向上
                case 'd':if(pt->right){CleanOld();pt->X++;DrawNew();}break;//->向右
                case 's':if (pt->down){ CleanOld();pt->Y++; DrawNew(); }break;//向下
                case 32:Suspend=-Suspend;break;//空格键 暂停游戏
                case 27:system("clear");Recovery_terminal();return 1;//ESC键 退出 
                default:break;
            }
	}
	return 0;
}


void *thread_functiona(void *arg)
{
	int ret, tmp=0;
	while(1)
	{
		while(1==Suspend);
		/*lock*/
		ret = pthread_mutex_lock(&lock);
		if (0 != ret)
		{
			perror("pthread_mutex_lock");
			pthread_exit(NULL);
		}

		tmp=AutoDown();
		/*unlock*/
		ret = pthread_mutex_unlock(&lock);
		if (0 != ret)
		{
			perror("pthread_mutex_unlock");
			pthread_exit(NULL);
		}
		if(tmp)break;
		usleep(1000000);
	}
	pthread_exit(NULL);
}


void *thread_functionb(void *arg)
{
	int i, ch=0, ret, tmp;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	while(1)
	{
		ch=getchar();
		/*lock*/
		ret = pthread_mutex_lock(&lock);
		if (0 != ret)
		{
			perror("pthread_mutex_lock");
			pthread_exit(NULL);
		}
		tmp=KeyControl(ch);
		/*unlock*/
		ret = pthread_mutex_unlock(&lock);
		if (0 != ret)
		{
			perror("pthread_mutex_unlock");
			pthread_exit(NULL);
		}
		if(tmp)break;
	}
	pthread_cancel(tid_a);
	
}

void GameRun()
{
	int ret;
	/*初始化互斥锁*/
	ret = pthread_mutex_init(&lock, NULL);
	if (0 != ret)
	{
		perror("pthread_mutex_init");
		exit(EXIT_FAILURE);
	}
	ret = pthread_create(&tid_a, NULL, thread_functiona, NULL);
	if (0 != ret)
	{
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	ret = pthread_create(&tid_b, NULL, thread_functionb, NULL);
	if (0 != ret)
	{
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	
	ret = pthread_join(tid_a, NULL);
	if (0 != ret)
	{
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}
	pthread_cancel(tid_b);

	ret = pthread_join(tid_b, NULL);
	if (0 != ret)
	{
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}
	
	ret = pthread_mutex_destroy(&lock);
	if (0 != ret)
	{
		perror("pthread_mutex_lock");
		exit(EXIT_FAILURE);
	}
}

int main()
{
loop:
	Welcom();
	GameRun();
	if(1==GameOver())goto loop;
	system("clear");
    Recovery_terminal();
    return 0;
}