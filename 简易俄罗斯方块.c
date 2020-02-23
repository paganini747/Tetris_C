#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<conio.h>
#include<windows.h>

#define FrameX  16  //游戏窗口左上角的X轴坐标
#define FrameY  4  //游戏窗口左上角的Y轴坐标
#define height  20 //游戏窗口的高度
#define width   10//游戏窗口的宽度 
#define StartX  1+width/2//俄罗斯方块中心出现位置横坐标 
#define StartY  2	 //俄罗斯方块中心出现位置横坐标 

int s[5][5]={0};//俄罗斯方块生成数组 
int a[height+2][width+2]={0};//俄罗斯方块打印数组
int flag_old=0;	//当前俄罗斯方块序号 
int flag_next=0;//下一个俄罗斯方块序号 
int left=1;	 //1为可左移，0为不可左移 
int right=1;//1为可右移，0为不可右移
int down=1;	//1为可下移，0为不可下移
int trans=1;//1为可旋转，0为不可旋转 
int X=StartX;//中心方块横坐标
int Y=StartY;//中心方块纵坐标
int Line=0;	//消除的行数 
int Level=1;//等级
int Score=0;//积分
clock_t start_t,end_t;	//获取系统时间 

void GameOver();
void Welcom();
/****************************************
 * 获取屏幕光标位置
****************************************/
void gotoxy(int x, int y)
{
 	COORD pos;
 	pos.X = x;//横坐标
 	pos.Y = y;//纵坐标
 	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
/****************************************
 * 生成俄罗斯方块 
****************************************/
void MakeTetris()
{
	int i=0,j=0;
	srand(time(NULL));
	if(flag_old==0)flag_old=rand()%19+1;
	else flag_old=flag_next;
	flag_next=rand()%19+1;
	for(i=0;i<5;i++)for(j=0;j<5;j++)s[i][j]=0;		
	switch(flag_old)
	{
		case  1:s[1][1]=1;s[2][1]=1;s[2][2]=1;s[2][3]=1;break;//■
		case  2:s[1][2]=1;s[1][3]=1;s[2][2]=1;s[3][2]=1;break;//■■■
		case  3:s[2][1]=1;s[2][2]=1;s[2][3]=1;s[3][3]=1;break;
		case  4:s[1][2]=1;s[2][2]=1;s[3][1]=1;s[3][2]=1;break;
	
		case  5:s[1][2]=1;s[2][1]=1;s[2][2]=1;s[2][3]=1;break;//  ■
		case  6:s[1][2]=1;s[2][2]=1;s[2][3]=1;s[3][2]=1;break;//■■■
		case  7:s[2][1]=1;s[2][2]=1;s[2][3]=1;s[3][2]=1;break;//
		case  8:s[1][2]=1;s[2][1]=1;s[2][2]=1;s[3][2]=1;break;

		case  9:s[1][3]=1;s[2][1]=1;s[2][2]=1;s[2][3]=1;break;//    ■
		case 10:s[1][2]=1;s[2][2]=1;s[3][2]=1;s[3][3]=1;break;//■■■
		case 11:s[2][1]=1;s[2][2]=1;s[2][3]=1;s[3][1]=1;break;
		case 12:s[1][1]=1;s[1][2]=1;s[2][2]=1;s[3][2]=1;break;
		
		case 13:s[2][1]=1;s[2][2]=1;s[2][3]=1;s[2][4]=1;break;//■■■■
		case 14:s[1][2]=1;s[2][2]=1;s[3][2]=1;s[4][2]=1;break;
		
		case 15:s[1][1]=1;s[1][2]=1;s[2][2]=1;s[2][3]=1;break;//■■
		case 16:s[1][3]=1;s[2][2]=1;s[2][3]=1;s[3][2]=1;break;//  ■■
		
		case 17:s[1][2]=1;s[1][3]=1;s[2][1]=1;s[2][2]=1;break;//  ■■
		case 18:s[1][2]=1;s[2][2]=1;s[2][3]=1;s[3][3]=1;break;//■■
		
		case 19:s[2][2]=1;s[2][3]=1;s[3][2]=1;s[3][3]=1;break;//■■
														      //■■
		default :break;
	}
}
/****************************************
 * 打印出俄罗斯方块 
****************************************/
void PrintTetris()
{
	int i=0,j=0;
	for(i=0;i<5;i++)
		for(j=0;j<5;j++)a[Y-2+i][X-2+j]=s[i][j];			
	for(i=1;i<=height;i++)
		{
		gotoxy(FrameX+2,FrameY+i);
		for(j=1;j<=width;j++)
			if(a[i][j])printf("■");
			else printf("  ");
		}
}
/****************************************
 * 清除旧的俄罗斯方块 
****************************************/
void CleanOld()
{
	int i=0,j=0;
	switch(flag_old)
	{
		case  1:a[Y-1][X-1]=0;a[Y][X-1]=0;a[Y][X]=0;a[Y][X+1]=0;break;//■
		case  2:a[Y-1][X]=0;a[Y-1][X+1]=0;a[Y][X]=0;a[Y+1][X]=0;break;//■■■
		case  3:a[Y][X-1]=0;a[Y][X]=0;a[Y][X+1]=0;a[Y+1][X+1]=0;break;
		case  4:a[Y-1][X]=0;a[Y][X]=0;a[Y+1][X-1]=0;a[Y+1][X]=0;break;
	
		case  5:a[Y-1][X]=0;a[Y][X-1]=0;a[Y][X]=0;a[Y][X+1]=0;break;//  ■
		case  6:a[Y-1][X]=0;a[Y][X]=0;a[Y+1][X]=0;a[Y][X+1]=0;break;//■■■
		case  7:a[Y][X-1]=0;a[Y][X]=0;a[Y][X+1]=0;a[Y+1][X]=0;break;//
		case  8:a[Y-1][X]=0;a[Y][X-1]=0;a[Y][X]=0;a[Y+1][X]=0;break;

		case  9:a[Y-1][X+1]=0;a[Y][X-1]=0;a[Y][X]=0;a[Y][X+1]=0;break;//    ■
		case 10:a[Y-1][X]=0;a[Y][X]=0;a[Y+1][X]=0;a[Y+1][X+1]=0;break;//■■■
		case 11:a[Y][X-1]=0;a[Y][X]=0;a[Y][X+1]=0;a[Y+1][X-1]=0;break;
		case 12:a[Y-1][X-1]=0;a[Y-1][X]=0;a[Y][X]=0;a[Y+1][X]=0;break;
		
		case 13:a[Y][X-1]=0;a[Y][X]=0;a[Y][X+1]=0;a[Y][X+2]=0;break;//■■■■
		case 14:a[Y-1][X]=0;a[Y][X]=0;a[Y+1][X]=0;a[Y+2][X]=0;break;
		
		case 15:a[Y-1][X-1]=0;a[Y-1][X]=0;a[Y][X]=0;a[Y][X+1]=0;break;//■■
		case 16:a[Y-1][X+1]=0;a[Y][X]=0;a[Y][X+1]=0;a[Y+1][X]=0;break;//  ■■
		
		case 17:a[Y-1][X]=0;a[Y-1][X+1]=0;a[Y][X-1]=0;a[Y][X]=0;break;//  ■■
		case 18:a[Y-1][X]=0;a[Y][X]=0;a[Y][X+1]=0;a[Y+1][X+1]=0;break;//■■
		
		case 19:a[Y][X]=0;a[Y][X+1]=0;a[Y+1][X]=0;a[Y+1][X+1]=0;break;//■■
														      		  //■■
		default :break;
	}
	for(i=0;i<5;i++)
		for(j=0;j<5;j++)
			if(s[i][j]==1)
			{
				a[Y+i-2][X+j-2]=0;
				gotoxy(FrameX+2*X-4+2*j,FrameY+Y-2+i);
				printf("  ");
			}
}
/****************************************
 * 打印新的俄罗斯方块 
****************************************/     
void DrawNew()
{
	int i=0,j=0;
	for(i=0;i<5;i++)
		for(j=0;j<5;j++)
			if(s[i][j]==1)
			{
				a[Y+i-2][X+j-2]=1;
				gotoxy(FrameX+2*X-4+2*j,FrameY+Y-2+i);
				printf("■");
			}

}
/****************************************
 * 旋转俄罗斯方块 
****************************************/
void Transform()
{
int i=0,j=0;
if(flag_old==4||flag_old==8||flag_old==12)flag_old=flag_old-3;
else if(flag_old==14||flag_old==16||flag_old==18)flag_old=flag_old-1;
else if(flag_old==19)flag_old=19;
else flag_old++;
for(i=0;i<5;i++)
	for(j=0;j<5;j++)s[i][j]=0;
	switch(flag_old)
	{
		case  1:s[1][1]=1;s[2][1]=1;s[2][2]=1;s[2][3]=1;break;//■
		case  2:s[1][2]=1;s[1][3]=1;s[2][2]=1;s[3][2]=1;break;//■■■
		case  3:s[2][1]=1;s[2][2]=1;s[2][3]=1;s[3][3]=1;break;
		case  4:s[1][2]=1;s[2][2]=1;s[3][1]=1;s[3][2]=1;break;
	
		case  5:s[1][2]=1;s[2][1]=1;s[2][2]=1;s[2][3]=1;break;//  ■
		case  6:s[1][2]=1;s[2][2]=1;s[2][3]=1;s[3][2]=1;break;//■■■
		case  7:s[2][1]=1;s[2][2]=1;s[2][3]=1;s[3][2]=1;break;//
		case  8:s[1][2]=1;s[2][1]=1;s[2][2]=1;s[3][2]=1;break;

		case  9:s[1][3]=1;s[2][1]=1;s[2][2]=1;s[2][3]=1;break;//    ■
		case 10:s[1][2]=1;s[2][2]=1;s[3][2]=1;s[3][3]=1;break;//■■■
		case 11:s[2][1]=1;s[2][2]=1;s[2][3]=1;s[3][1]=1;break;
		case 12:s[1][1]=1;s[1][2]=1;s[2][2]=1;s[3][2]=1;break;
		
		case 13:s[2][1]=1;s[2][2]=1;s[2][3]=1;s[2][4]=1;break;//■■■■
		case 14:s[1][2]=1;s[2][2]=1;s[3][2]=1;s[4][2]=1;break;
		
		case 15:s[1][1]=1;s[1][2]=1;s[2][2]=1;s[2][3]=1;break;//■■
		case 16:s[1][3]=1;s[2][2]=1;s[2][3]=1;s[3][2]=1;break;//  ■■
		
		case 17:s[1][2]=1;s[1][3]=1;s[2][1]=1;s[2][2]=1;break;//  ■■
		case 18:s[1][2]=1;s[2][2]=1;s[2][3]=1;s[3][3]=1;break;//■■
		
		case 19:s[2][2]=1;s[2][3]=1;s[3][2]=1;s[3][3]=1;break;//■■
														      //■■	
		default :break;
	}
}
/****************************************
 * 判断俄罗斯方块是否可移动和变形 
****************************************/
void Judge_MoveAndTansform()
{
switch(flag_old)
	{
		case  1:if(a[Y-1][X-2]==0&&a[Y][X-2]==0)left=1;else left=0;										//■
				if(a[Y-1][X]==0&&a[Y][X+2]==0)right=1;else right=0;										//■■■
				if(a[Y+1][X-1]==0&&a[Y+1][X]==0&&a[Y+1][X+1]==0)down=1;else down=0;
				if(a[Y-1][X]==0&&a[Y-1][X+1]==0&&a[Y+1][X]==0)trans=1;else trans=0;
				break;
		case  2:if(a[Y-1][X-1]==0&&a[Y][X-1]==0&&a[Y+1][X-1]==0)left=1;else left=0;  					//■■
				if(a[Y-1][X+2]==0&&a[Y][X+1]==0&&a[Y+1][X+1]==0)right=1;else right=0;					//■
				if(a[Y+2][X]==0&&a[Y][X+1]==0)down=1;else down=0;					 					//■
				if(a[Y][X-1]==0&&a[Y][X+1]==0&&a[Y+1][X+1]==0)trans=1;else trans=0;
				break;
		case  3:if(a[Y][X-2]==0&&a[Y+1][X]==0)left=1;else left=0;										//■■■
				if(a[Y][X+2]==0&&a[Y+1][X+2]==0)right=1;else right=0;									//    ■
				if(a[Y+1][X-1]==0&&a[Y+1][X]==0&&a[Y+2][X+1]==0)down=1;else down=0;
				if(a[Y-1][X]==0&&a[Y+1][X-1]==0&&a[Y+1][X]==0)trans=1;else trans=0;
				break;
		case  4:if(a[Y-1][X-1]==0&&a[Y][X-1]==0&&a[Y+1][X-2]==0)left=1;else left=0;	 					//  ■
				if(a[Y-1][X+1]==0&&a[Y][X+1]==0&&a[Y+1][X+1]==0)right=1;else right=0;					//  ■
				if(a[Y+2][X-1]==0&&a[Y+2][X]==0)down=1;else down=0;					 					//■■
				if(a[Y-1][X-1]==0&&a[Y][X-1]==0&&a[Y][X+1]==0)trans=1;else trans=0;
				break;
		case  5:if(a[Y-1][X-1]==0&&a[Y][X-2]==0)left=1;else left=0;				    					//  ■
				if(a[Y-1][X+1]==0&&a[Y][X+2]==0)right=1;else right=0;									//■■■
				if(a[Y+1][X-1]==0&&a[Y+1][X]==0&&a[Y+1][X+1]==0)down=1;else down=0;
				if(a[Y+1][X]==0)trans=1;else trans=0;
				break;
		case  6:if(a[Y-1][X-1]==0&&a[Y][X-1]==0&&a[Y+1][X-1]==0)left=1;else left=0;	 					//■
				if(a[Y-1][X+1]==0&&a[Y][X+2]==0&&a[Y+1][X+1]==0)right=1;else right=0;					//■■
				if(a[Y+2][X]==0&&a[Y+1][X+1]==0)down=1;else down=0;					 					//■
				if(a[Y][X-1]==0)trans=1;else trans=0;
				break;
		case  7:if(a[Y][X-2]==0&&a[Y+1][X-1]==0)left=1;else left=0;	 						 			//■■■
				if(a[Y][X+2]==0&&a[Y+1][X+1]==0)right=1;else right=0;						 			//  ■
				if(a[Y+1][X-1]==0&&a[Y+2][X]==0&&a[Y+1][X+1]==0)down=1;else down=0;
				if(a[Y-1][X]==0)trans=1;else trans=0;
				break;
		case  8:if(a[Y-1][X-1]==0&&a[Y][X-2]==0&&a[Y+1][X-1]==0)left=1;else left=0;	 					//  ■
				if(a[Y-1][X+1]==0&&a[Y][X+1]==0&&a[Y+1][X+1]==0)right=1;else right=0;					//■■
				if(a[Y+1][X-1]==0&&a[Y+2][X]==0)down=1;else down=0;										//  ■
				if(a[Y][X+1]==0)trans=1;else trans=0;
				break;
		case  9:if(a[Y][X-2]==0&&a[Y-1][X]==0)left=1;else left=0;										//    ■
				if(a[Y-1][X+2]==0&&a[Y][X+2]==0)right=1;else right=0;									//■■■
				if(a[Y+1][X-1]==0&&a[Y+1][X]==0&&a[Y+1][X+1]==0)down=1;else down=0;
				if(a[Y-1][X]==0&&a[Y+1][X]==0&&a[Y+1][X+1]==0)trans=1;else trans=0;
				break;
		case 10:if(a[Y-1][X-1]==0&&a[Y][X-1]==0&&a[Y+1][X-1]==0)left=1;else left=0;  					//■
				if(a[Y-1][X+1]==0&&a[Y][X+1]==0&&a[Y+1][X+2]==0)right=1;else right=0;					//■
				if(a[Y+2][X]==0&&a[Y+2][X+1]==0)down=1;else down=0;										//■■
				if(a[Y][X-1]==0&&a[Y+1][X-1]==0&&a[Y][X+1]==0)trans=1;else trans=0;
				break;		
		case 11:if(a[Y][X-2]==0&&a[Y+1][X-2]==0)left=1;else left=0;					 					//■■■
				if(a[Y][X+2]==0&&a[Y+1][X]==0)right=1;else right=0;					 					//■
				if(a[Y+2][X-1]==0&&a[Y+1][X]==0&&a[Y+1][X+1]==0)down=1;else down=0;
				if(a[Y-1][X-1]==0&&a[Y-1][X]==0&&a[Y+1][X]==0)trans=1;else trans=0;
				break;
		case 12:if(a[Y-1][X-2]==0&&a[Y][X-1]==0&&a[Y+1][X-1]==0)left=1;else left=0;  					//■■
				if(a[Y-1][X+1]==0&&a[Y][X+1]==0&&a[Y+1][X+1]==0)right=1;else right=0;					//  ■
				if(a[Y][X-1]==0&&a[Y+2][X]==0)down=1;else down=0;					 					//  ■
				if(a[Y-1][X+1]==0&&a[Y][X-1]==0&&a[Y][X+1]==0)trans=1;else trans=0;
				break;
		case 13:if(a[Y][X-2]==0)left=1;else left=0;														//■■■■
				if(a[Y][X+3]==0)right=1;else right=0;										
				if(a[Y+1][X-1]==0&&a[Y+1][X]==0&&a[Y+1][X+1]==0&&a[Y+1][X+2]==0)down=1;else down=0;
				if(a[Y-1][X]==0&&a[Y+1][X]==0&&a[Y+2][X]==0)trans=1;else trans=0;
				break;
		case 14:if(a[Y-1][X-1]==0&&a[Y][X-1]==0&&a[Y+1][X-1]==0&&a[Y+2][X-1]==0)left=1;else left=0;		//■
				if(a[Y-1][X+1]==0&&a[Y][X+1]==0&&a[Y+1][X+1]==0&&a[Y+2][X+1]==0)right=1;else right=0;	//■
				if(a[Y+3][X]==0)down=1;else down=0;														//■
				if(a[Y][X-1]==0&&a[Y][X+1]==0&&a[Y][X+2]==0)trans=1;else trans=0;						//■
				break;
		case 15:if(a[Y-1][X-2]==0&&a[Y][X-1]==0)left=1;else left=0;										//■■
				if(a[Y-1][X+1]==0&&a[Y][X+2]==0)right=1;else right=0;									//  ■■
				if(a[Y][X-1]==0&&a[Y+1][X]==0&&a[Y+1][X+1]==0)down=1;else down=0;
				if(a[Y-1][X+1]==0&&a[Y+1][X]==0)trans=1;else trans=0;
				break;
		case 16:if(a[Y-1][X]==0&&a[Y][X-1]==0&&a[Y+1][X-1]==0)left=1;else left=0;						//  ■
				if(a[Y-1][X+2]==0&&a[Y][X+2]==0&&a[Y+1][X+1]==0)right=1;else right=0;					//■■
				if(a[Y+2][X]==0&&a[Y+1][X+1]==0)down=1;else down=0;										//■
				if(a[Y-1][X-1]==0&&a[Y-1][X]==0)trans=1;else trans=0;
				break;
		case 17:if(a[Y-1][X-1]==0&&a[Y][X-2]==0)left=1;else left=0; 									//  ■■
				if(a[Y-1][X+2]==0&&a[Y][X+1]==0)right=1;else right=0;									//■■
				if(a[Y+1][X-1]==0&&a[Y+1][X]==0&&a[Y][X+1]==0)down=1;else down=0;
				if(a[Y][X+1]==0&&a[Y+1][X+1]==0)trans=1;else trans=0;
				break;
		case 18:if(a[Y-1][X-1]==0&&a[Y][X-1]==0&&a[Y+1][X]==0)left=1;else left=0;						//■
				if(a[Y-1][X+1]==0&&a[Y][X+2]==0&&a[Y+1][X+2]==0)right=1;else right=0;					//■■
				if(a[Y+1][X]==0&&a[Y+2][X+1]==0)down=1;else down=0;										//  ■
				if(a[Y+1][X-1]==0&&a[Y+1][X]==0)trans=1;else trans=0;
				break;
		case 19:if(a[Y][X-1]==0&&a[Y+1][X-1]==0)left=1;else left=0;										//■■
				if(a[Y][X+2]==0&&a[Y+1][X+2]==0)right=1;else right=0;									//■■
				if(a[Y+2][X]==0&&a[Y+2][X+1]==0)down=1;else down=0;
				trans=0;
				break;
		default :break;
	}
}
/****************************************
 * 键盘输入判定 
****************************************/
void KeyControl()
{
int i=0,j=0,ch=0;
if(_kbhit())       				  	 	
   			{
    			ch=_getch();
    			Judge_MoveAndTansform();
    			if(ch==72)//变形
				{
					Judge_MoveAndTansform();
					if(trans)
					{	
					CleanOld();
					Transform();
					DrawNew();	
					gotoxy(0,0);
					}
				}
				else if(ch==75)//<-向左 
				{
					Judge_MoveAndTansform();
					if(left)
					{			
					CleanOld();
					X--;
		        	DrawNew();	
		        	gotoxy(0,0);
		    		}
		    	}
				else if(ch==77)//->向右 
				{
					Judge_MoveAndTansform();
					if(right)
					{	
					CleanOld();
					X++;
		        	DrawNew();
		        	gotoxy(0,0);
					}
				}
				else if(ch==80)//向下 
				{
				Judge_MoveAndTansform();
					if(down)
					{			 
					CleanOld();
					Y++;
					DrawNew();
					gotoxy(0,0);
					}
				}
				else if(ch==32)//暂停
				{
					_getch();
				} 
				else if(ch==27)//退出 
				{
					system("cls");
					exit(0);
				} 
			}
}
/****************************************
 * 打印俄罗斯方块边框 
****************************************/
void DrwaGameframe()
{
	int i=0;
	gotoxy(FrameX+width-5,FrameY-2);
	printf("俄 罗 斯 方 块");
 	gotoxy(FrameX,FrameY);
	printf("■");									//打印框角
 	gotoxy(FrameX+2*width+2,FrameY);
 	printf("■");
 	gotoxy(FrameX,FrameY+height+1);
 	printf("■");
 	gotoxy(FrameX+2*width+2,FrameY+height+1);
 	printf("■");
	for(i=1;i<=width;i++)
 	{
  		gotoxy(FrameX+2*i,FrameY);
  		printf("■");         						//打印上横框
 	}
 	for(i=1;i<=width;i++)
 	{
  		gotoxy(FrameX+2*i,FrameY+height+1);
  		printf("■");         						//打印下横框
 	}
 	for(i=1;i<=height;i++)
	{
  		gotoxy(FrameX,FrameY+i);
  		printf("■");        	 					//打印左竖框
 	}
 	for(i=1;i<=height;i++)
 	{
  		gotoxy(FrameX+2*width+2,FrameY+i);
  		printf("■");         						 //打印右竖框 
    }
    for(i=1;i<=width;i++)
 	{
  		a[height+1][i]=1;
 	}
 	for(i=0;i<=height+1;i++)
	{
  	 	a[i][0]=1;     
		a[i][width+1]=1;
 	}
 	gotoxy(FrameX+2*width+8,FrameY-1);
	printf("Level:%d",Level);
	gotoxy(FrameX+2*width+8,FrameY+1);
	printf("积分:%d",Score);
 	gotoxy(FrameX+2*width+8,FrameY+3);
	printf("消去的行数:%d",Line);
}
/****************************************
 * 游戏参数初始化 
****************************************/
void Initialization()
{
	Line = 0;							
	Level = 1;
	Score = 0;
	int i=0,j=0;
	for(i=0;i<5;i++)
		for(j=0;j<5;j++)s[i][j]=0;
	for(i=0;i<=height+2;i++)
		for(j=0;j<=width+2;j++)a[i][j]=0;
	for(i=1;i<=width;i++)a[height+1][i]=1;
 	for(i=0;i<=height+1;i++)
	{
  	 	a[i][0]=1;     
		a[i][width+1]=1;
 	}
}
/****************************************
 * 清除满行 
****************************************/
void CleanLine()
{
int i=height,j=1,k=0,sum=0;
for(i=height;i>0;i--)
	{
	sum=0;
	for(j=1;j<=width;j++)sum+=a[i][j];
		if(sum==width)
		{
		Line++;
		for(k=i;k>1;k--)
			for(j=1;j<width;j++)a[k][j]=a[k-1][j];
			i++;
		}
	}
	Level=1+Line/10;
	Score=Line*100;
	gotoxy(FrameX+2*width+8,FrameY-1);
	printf("Level:%d",Level);
	gotoxy(FrameX+2*width+8,FrameY+1);
	printf("积分:%d",Score);
	gotoxy(FrameX+2*width+8,FrameY+3);
	printf("消去的行数:%d",Line);
}
/****************************************
 * 关键参数显示函数(用于调试) 
****************************************/
/*
void JustForDebugging()
{
	int i=0,j=0;
	for(i=0;i<height+2;i++)
		{
			gotoxy(FrameX+2*width+24,FrameY+i);
			for(j=0;j<width+2;j++)
    		printf("%d",a[i][j]);		
		}
	 gotoxy(FrameX+3*width+30,FrameY+1);
	 printf("flag_old=%2d",flag_old);
	 gotoxy(FrameX+3*width+30,FrameY+2);
	 printf("X=%d",X);
	 gotoxy(FrameX+3*width+30,FrameY+3);
	 printf("Y=%d",Y);
	 gotoxy(FrameX+3*width+30,FrameY+4);
	 printf("Left=%d",left);
	 gotoxy(FrameX+3*width+30,FrameY+5);
	 printf("Right=%d",right);
	 gotoxy(FrameX+3*width+30,FrameY+6);
	 printf("Down=%d",down);
	 gotoxy(FrameX+3*width+30,FrameY+7);
	 printf("Trans=%d",trans);
}
*/
/****************************************
 * 打印下一个俄罗斯方块 
****************************************/
void PrintNextTetris()
{
int i=0,j=0,temp[5][5];
	for(i=0;i<5;i++)
		for(j=0;j<5;j++)temp[i][j]=0;
	switch(flag_next)
	{
		case  1:temp[1][1]=1;temp[2][1]=1;temp[2][2]=1;temp[2][3]=1;break;//■
		case  2:temp[1][2]=1;temp[1][3]=1;temp[2][2]=1;temp[3][2]=1;break;//■■■
		case  3:temp[2][1]=1;temp[2][2]=1;temp[2][3]=1;temp[3][3]=1;break;
		case  4:temp[1][2]=1;temp[2][2]=1;temp[3][1]=1;temp[3][2]=1;break;
	
		case  5:temp[1][2]=1;temp[2][1]=1;temp[2][2]=1;temp[2][3]=1;break;//  ■
		case  6:temp[1][2]=1;temp[2][2]=1;temp[2][3]=1;temp[3][2]=1;break;//■■■
		case  7:temp[2][1]=1;temp[2][2]=1;temp[2][3]=1;temp[3][2]=1;break;//
		case  8:temp[1][2]=1;temp[2][1]=1;temp[2][2]=1;temp[3][2]=1;break;

		case  9:temp[1][3]=1;temp[2][1]=1;temp[2][2]=1;temp[2][3]=1;break;//    ■
		case 10:temp[1][2]=1;temp[2][2]=1;temp[3][2]=1;temp[3][3]=1;break;//■■■
		case 11:temp[2][1]=1;temp[2][2]=1;temp[2][3]=1;temp[3][1]=1;break;
		case 12:temp[1][1]=1;temp[1][2]=1;temp[2][2]=1;temp[3][2]=1;break;
		
		case 13:temp[2][1]=1;temp[2][2]=1;temp[2][3]=1;temp[2][4]=1;break;//■■■■
		case 14:temp[1][2]=1;temp[2][2]=1;temp[3][2]=1;temp[4][2]=1;break;
		
		case 15:temp[1][1]=1;temp[1][2]=1;temp[2][2]=1;temp[2][3]=1;break;//■■
		case 16:temp[1][3]=1;temp[2][2]=1;temp[2][3]=1;temp[3][2]=1;break;//  ■■
		
		case 17:temp[1][2]=1;temp[1][3]=1;temp[2][1]=1;temp[2][2]=1;break;//  ■■
		case 18:temp[1][2]=1;temp[2][2]=1;temp[2][3]=1;temp[3][3]=1;break;//■■
		
		case 19:temp[2][2]=1;temp[2][3]=1;temp[3][2]=1;temp[3][3]=1;break;//■■
																		  //■■	
		default :break;
	}
	gotoxy(FrameX+2*width+8,FrameY+5);
	printf("下一个方块:");
	printf("%2d",flag_next);
	
	for(i=0;i<5;i++)
		for(j=0;j<5;j++)
		{
		gotoxy(FrameX+2*width+2*j+8,FrameY+6+i);
			if(temp[i][j])printf("■");
			else printf("  ");
		}	
		gotoxy(0,0);
}
/****************************************
 * 游戏开始函数 
****************************************/
void GamePlay()
{  
	system("cls");		   
	Initialization();
	DrwaGameframe();
	MakeTetris();
	DrawNew();
	PrintNextTetris();
	start_t=clock();
	while(1)
	{
		KeyControl();
		end_t=clock();
		//JustForDebugging();
			if(end_t-start_t>1000-100*Level)  
			{
				Judge_MoveAndTansform();
				//JustForDebugging();
				if(down)
				{	
					CleanOld();
					Y++;
					DrawNew();
					gotoxy(0,0);
				}
				//JustForDebugging();	
				if(!down)
				{
					CleanLine();
					X=StartX;
					Y=StartY;
					MakeTetris();
					Judge_MoveAndTansform();
					if(!down)
					{
						int i=0,j=0;
						for(i=0;i<5;i++)
							for(j=0;j<5;j++)a[Y-2+i][X-2+j]+=s[i][j];			
							for(i=1;i<=height;i++)
								{
									gotoxy(FrameX+2,FrameY+i);
									for(j=1;j<=width;j++)
										if(a[i][j])printf("■");
										else printf("  ");
								}
						Sleep(1);
						GameOver();
					}
					PrintTetris(); 
					PrintNextTetris();					
				}
				//JustForDebugging();
				start_t=clock();
			}
	}
} 
/****************************************
 * 游戏结束界面 
****************************************/
void GameOver()
{
   				system("cls");
                gotoxy(0,8);
				printf("\t  ■■■      ■       ■    ■   ■■■■        ■■    ■      ■ ■■■■   ■■■  \n");
                printf("\t■           ■■     ■ ■■ ■  ■            ■    ■   ■    ■  ■         ■   ■ \n");
                printf("\t■   ■■   ■  ■   ■   ■   ■ ■■■       ■      ■   ■  ■   ■■■     ■■■  \n");
				printf("\t■     ■  ■■■■  ■   ■   ■ ■            ■    ■     ■■    ■         ■   ■ \n");
				printf("\t  ■■■  ■      ■ ■   ■   ■ ■■■■        ■■        ■     ■■■■   ■    ■\n");
				gotoxy(40,18); 
				printf("1:再玩一局");
				gotoxy(60,18);
				printf("2:退出");
				int n;				
				gotoxy(48,20);
				printf("选择【1/2】：");
				scanf_s("%d", &n);
    			switch (n)
    			{
    				case 1:GamePlay();break;
    				case 2:exit(0);break;
    			}
}
/****************************************
 * 游戏按键介绍界面 
****************************************/
void Explation()
{
	int i,j = 1;
    system("cls");
    gotoxy(32,3);
    printf("按键说明");
    for (i = 6; i <= 16; i++)   	//输出上下边框
	{
		for (j = 15; j <= 60; j++)  //输出左右边框
		{
			gotoxy(j, i);
			if (i == 6 || i == 16) printf("*");
			else if (j == 15 || j == 60) printf("*");
		}
	}
    gotoxy(18,7);
    printf("tip1: 玩家可以通过 ← →方向键来移动方块");
    gotoxy(18,9);
    printf("tip2: 通过 ↑使方块旋转");
    gotoxy(18,11);
    printf("tip3: 通过 ↓加速方块下落");
    gotoxy(18,13);
    printf("tip4: 按空格键暂停游戏，再按空格键继续");
    gotoxy(18,15);
    printf("tip5: 按ESC退出游戏");
    _getch();                //按任意键返回主界面
    system("cls");
    Welcom();
}
/****************************************
 * 游戏规则介绍界面 
****************************************/
void Regulation()
{
	int i,j = 1;
    system("cls");
    gotoxy(34,3);
    printf("游戏规则");
    for (i = 6; i <= 18; i++)   	//输出上下边框
	{
		for (j = 12; j <= 70; j++)  //输出左右边框
		{
			gotoxy(j, i);
			if (i == 6 || i == 18) printf("*");
			else if (j == 12 || j == 70) printf("*");
		}
	}
    gotoxy(16,7);
    printf("tip1: 不同形状的小方块从屏幕上方落下，玩家通过调整");
    gotoxy(22,9);
    printf("方块的位置和方向，使他们在屏幕底部拼出完整的");
    gotoxy(22,11);
    printf("一条或几条");
    gotoxy(16,13);
    printf("tip2: 每消除一行，积分增加100");
    gotoxy(16,15);
    printf("tip3: 每累计1000分，会提升一个等级");
    gotoxy(16,17);
    printf("tip4: 提升等级会使方块下落速度加快，游戏难度加大");
	_getch();//按任意键返回主界面
	system("cls");
	Welcom();
}
/****************************************
 * 游戏欢迎界面 
****************************************/
void Welcom()
{
	int n=0;
	gotoxy(28,3);
    printf("俄  罗  斯  方  块\n");	
    
	gotoxy(18,5);
    printf("■");						//■
    gotoxy(18,6);						//■■
    printf("■■");						//■
	gotoxy(18,7);
    printf("■");

	gotoxy(26,6);
    printf("■■");					    //■■
	gotoxy(28,7);						//  ■■
    printf("■■");
    
	gotoxy(36,6);						//■■
    printf("■■");						//■■
    gotoxy(36,7);
    printf("■■");

	gotoxy(45,5);
    printf("■");						//■
    gotoxy(45,6);						//■
	printf("■");						//■
	gotoxy(45,7);						//■
	printf("■");
	gotoxy(45,8);
	printf("■");
	
	gotoxy(56,6);
    printf("■");						//    ■
    gotoxy(52,7); 						//■■■
	printf("■■■");
					
	gotoxy(25,12);				
	printf("1.开始游戏");			
	gotoxy(40,12);
	printf("2.按键说明");
	gotoxy(25,15);
	printf("3.游戏规则");
	gotoxy(40,15);
	printf("4.退出");
	gotoxy(25,18);
	printf("请选择[1 2 3 4]:[ ]\b\b");
    scanf_s("%d", &n);    			//输入选项
    switch (n)
    {
    	case 1:GamePlay();break;//游戏开始 
    	case 2:Explation();break;//按键说明函数 
    	case 3:Regulation();break;//游戏规则 
    	case 4:exit(0);break;//关闭游戏 
    }
}
/****************************************
 * 主函数 
****************************************/
int main()
{	
	Welcom();	
	return 0;
}