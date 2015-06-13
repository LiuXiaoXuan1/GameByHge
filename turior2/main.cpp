#include "hge.h"
#include "hgesprite.h"
#include <list>
#include <stdlib.h>
#include <time.h>
#include <hgerect.h>
using namespace std;

 HGE *hge=NULL;
 hgeSprite *hspr=NULL;//背景精灵
 HTEXTURE htex;//背景纹理句柄
 
 hgeSprite *hsprplane1=NULL;//精灵飞机
 HTEXTURE htexplane1;//飞机的纹理句柄

 hgeSprite *hsprplane2=NULL;//精灵飞机
 HTEXTURE htexplane2;//纹理飞机

 hgeSprite *hsprbullet=NULL;//精灵子弹
 HTEXTURE htexbullet;//纹理子弹

 hgeSprite *hsprenemy=NULL;//精灵敌人
 HTEXTURE htexenemy;//纹理敌人


 //背景图的坐标数据
 float y1=0,y2=852;

 //飞机图的坐标数据
 float x_plane=480/2-48,y_plane=852-216;

 //控制子弹的状态
 bool Anistate=true;

 //子弹的坐标数据
// float x_bullet=x_plane+102/2,y_bullet=y_plane-11;

 //声效
 HEFFECT hfet;//声音

 //子弹结构体
 enum BulletState{Die,Live};
 struct Bullet1{
	  BulletState bulletstate1;//子弹状态
      float x_bullet1;
	  float y_bullet1;
 };
 struct Bullet2{
	  BulletState bulletstate2;//子弹状态
      float x_bullet2;
	  float y_bullet2;
 };
 //敌人结构体
 struct Enemy{
	 float x_enemy;
	 float y_enemy;
 };

 int i=0;//敌人数目

 list<Bullet1> vBullet1;
 list<Bullet2> vBullet2;
 Enemy enemy[10];

 //帧函数：循环  游戏中数据的更新
 bool FrameFunc(){
	 //按ESC键时游戏退出
	 if(hge->Input_GetKeyState(HGEK_ESCAPE))
		 return true;

	 //更新背景数据
	 y1-=0.5;
	 y2-=0.5;
	 if(y1<=-852){
		 y1=852;
	 }
	 if(y2<=-852){
		 y2=852;
	 }

	 //更新飞机的移动数据
     if(hge->Input_GetKeyState(HGEK_RIGHT)){//右方向
		 x_plane+=0.4;
		 if(x_plane>=480){
			 x_plane=-102;
		 }
	 }
	 if(hge->Input_GetKeyState(HGEK_LEFT)){//左方向
		 x_plane-=0.4;
		 if(x_plane<=0){
			 x_plane=480+96;
		 }
	 }

	 static float begintime=hge->Timer_GetTime();
	 float endtime=hge->Timer_GetTime();
	 if(endtime-begintime>=0.1f){
		  Anistate=!Anistate;
		  begintime=endtime;
	 }

	 //更新子弹的数据
	 static float beginBulletTime=hge->Timer_GetTime();//开始数据，设为静态变量
	 float endBulletTime=hge->Timer_GetTime();
	 if(hge->Input_GetKeyState(HGEK_J)){
		 if(endBulletTime-beginBulletTime>=0.2f){
			 Bullet1 bullet1;
			 bullet1.bulletstate1=Live;
			 bullet1.x_bullet1=x_plane+20;
			 bullet1.y_bullet1=y_plane+10;
			 vBullet1.push_back(bullet1);
			 Bullet2 bullet2;
			 bullet2.bulletstate2=Live;
			 bullet2.x_bullet2=x_plane+85;
			 bullet2.y_bullet2=y_plane+10;
			 vBullet2.push_back(bullet2);
			 beginBulletTime=endBulletTime;
		 }
	 }

	 //更新子弹的数据
	 for(i=0;i<10;i++){
        srand(time(NULL));
	    enemy[i].x_enemy=rand()%(480-98);
	    enemy[i].y_enemy=rand()%(798-73);
	 }

     //遍历数组
	 if(!vBullet1.empty()){
		  list<Bullet1>::iterator it1=vBullet1.begin();
		  for(;it1!=vBullet1.end();it1++){
			  it1->y_bullet1-=2;
		  }
	 }
	 if(!vBullet2.empty()){
		  list<Bullet2>::iterator it2=vBullet2.begin();
		  for(;it2!=vBullet2.end();it2++){
			 it2->y_bullet2-=2;
		  }
	 }

	 //加载声音
     hge->Effect_PlayEx(hfet,100,50);
	
	 //退出游戏
	 return false;
 }

 //渲染函数: 循环  绘图
 bool RenderFunc(){
	 //通知引擎开始绘图
	 hge->Gfx_BeginScene();

	 //清屏（默认是黑色）
     hge->Gfx_Clear(0);

	 //实际绘图,在System_Initiate上加载图片时没有起到真正绘图的作用,此为背景图
	 hspr->Render(0,y1);
	 hspr->Render(0,y2);

	 //绘制飞机图 480 852
	 hsprplane1->Render(x_plane,y_plane);

	 //绘制子弹图
     if(!vBullet1.empty()){
		 list<Bullet1>::iterator it1=vBullet1.begin();
		 for(;it1!=vBullet1.end();it1++){
			 hsprbullet->Render(it1->x_bullet1,it1->y_bullet1);
		 }
	 }
	 if(!vBullet2.empty()){
		 list<Bullet2>::iterator it2=vBullet2.begin();
		 for(;it2!=vBullet2.end();it2++){
			 hsprbullet->Render(it2->x_bullet2,it2->y_bullet2);
		 }
	 }

	 //绘制敌人图
     for(i=0;i<10;i++){
		srand(time(NULL));
		enemy[i].x_enemy=rand()%(480-98);
		enemy[i].y_enemy=rand()%(798-73);
		hsprenemy->Render(enemy[i].x_enemy,enemy[i].y_enemy);
	 }

     //结束绘图
	 hge->Gfx_EndScene();

	 return false;
 }

 //入口函数
 int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
	 //给hge指针赋值：hge引擎版本号
	 hge=hgeCreate(HGE_VERSION);

	 //设置帧函数
	 hge->System_SetState(HGE_FRAMEFUNC,FrameFunc);

	 //设置渲染函数
	 hge->System_SetState(HGE_RENDERFUNC,RenderFunc);

	 //设置窗口属性
	 hge->System_SetState(HGE_TITLE,"欢迎来到旋子的世界");

	 hge->System_SetState(HGE_WINDOWED,true);
	 hge->System_SetState(HGE_SCREENWIDTH,480);
	 hge->System_SetState(HGE_SCREENHEIGHT,852);

	 //不隐藏鼠标
	 hge->System_SetState(HGE_HIDEMOUSE,false);

	 //不使用声音
	 hge->System_SetState(HGE_USESOUND,false);
	 hge->System_SetState(HGE_FPS,1000);//帧

	 //初始化引擎
	 if(hge->System_Initiate()){
		 //自己的初始化  如:加载图片

		 //加载背景图片(关联背景图片)
         htex=hge->Texture_Load("res/background.png");
         hspr=new hgeSprite(htex,0,0,480,852);

		 //加载飞机图片
		 htexplane1=hge->Texture_Load("res/hero1.png");
         hsprplane1=new hgeSprite(htexplane1,0,0,102,126);

		 //加载子弹图片
         htexbullet=hge->Texture_Load("res/bullet2.png");
		 hsprbullet=new hgeSprite(htexbullet,0,0,5,11);

		 //加载敌人图片
         htexenemy=hge->Texture_Load("res/enemy1.png");
		 hsprenemy=new hgeSprite(htexenemy,0,0,57,43);

		 //加载声音
         hfet=hge->Effect_Load("res/game_music.mp3");

		 //清空数组
		 vBullet1.clear();

		 //启动引擎: 循环执行帧函数
		 hge->System_Start();
	 }
	 else{
         //初始化失败
		 MessageBox(NULL, hge->System_GetErrorMessage(), "Error",
                  MB_OK | MB_ICONERROR | MB_APPLMODAL);
	 }

	 //关闭窗口
	 //关闭引擎
	 hge->System_Shutdown();
	 
	 //释放hge指针
	 hge->Release();


	 delete hspr;
     delete hsprplane1;
	 delete hsprplane2;
	 delete hsprbullet;

	 return 0;
 }
