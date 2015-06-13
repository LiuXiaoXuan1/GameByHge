#include "hge.h"
#include "hgesprite.h"
#include <list>
#include <stdlib.h>
#include <time.h>
#include <hgerect.h>
using namespace std;

 HGE *hge=NULL;
 hgeSprite *hspr=NULL;//��������
 HTEXTURE htex;//����������
 
 hgeSprite *hsprplane1=NULL;//����ɻ�
 HTEXTURE htexplane1;//�ɻ���������

 hgeSprite *hsprplane2=NULL;//����ɻ�
 HTEXTURE htexplane2;//����ɻ�

 hgeSprite *hsprbullet=NULL;//�����ӵ�
 HTEXTURE htexbullet;//�����ӵ�

 hgeSprite *hsprenemy=NULL;//�������
 HTEXTURE htexenemy;//�������


 //����ͼ����������
 float y1=0,y2=852;

 //�ɻ�ͼ����������
 float x_plane=480/2-48,y_plane=852-216;

 //�����ӵ���״̬
 bool Anistate=true;

 //�ӵ�����������
// float x_bullet=x_plane+102/2,y_bullet=y_plane-11;

 //��Ч
 HEFFECT hfet;//����

 //�ӵ��ṹ��
 enum BulletState{Die,Live};
 struct Bullet1{
	  BulletState bulletstate1;//�ӵ�״̬
      float x_bullet1;
	  float y_bullet1;
 };
 struct Bullet2{
	  BulletState bulletstate2;//�ӵ�״̬
      float x_bullet2;
	  float y_bullet2;
 };
 //���˽ṹ��
 struct Enemy{
	 float x_enemy;
	 float y_enemy;
 };

 int i=0;//������Ŀ

 list<Bullet1> vBullet1;
 list<Bullet2> vBullet2;
 Enemy enemy[10];

 //֡������ѭ��  ��Ϸ�����ݵĸ���
 bool FrameFunc(){
	 //��ESC��ʱ��Ϸ�˳�
	 if(hge->Input_GetKeyState(HGEK_ESCAPE))
		 return true;

	 //���±�������
	 y1-=0.5;
	 y2-=0.5;
	 if(y1<=-852){
		 y1=852;
	 }
	 if(y2<=-852){
		 y2=852;
	 }

	 //���·ɻ����ƶ�����
     if(hge->Input_GetKeyState(HGEK_RIGHT)){//�ҷ���
		 x_plane+=0.4;
		 if(x_plane>=480){
			 x_plane=-102;
		 }
	 }
	 if(hge->Input_GetKeyState(HGEK_LEFT)){//����
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

	 //�����ӵ�������
	 static float beginBulletTime=hge->Timer_GetTime();//��ʼ���ݣ���Ϊ��̬����
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

	 //�����ӵ�������
	 for(i=0;i<10;i++){
        srand(time(NULL));
	    enemy[i].x_enemy=rand()%(480-98);
	    enemy[i].y_enemy=rand()%(798-73);
	 }

     //��������
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

	 //��������
     hge->Effect_PlayEx(hfet,100,50);
	
	 //�˳���Ϸ
	 return false;
 }

 //��Ⱦ����: ѭ��  ��ͼ
 bool RenderFunc(){
	 //֪ͨ���濪ʼ��ͼ
	 hge->Gfx_BeginScene();

	 //������Ĭ���Ǻ�ɫ��
     hge->Gfx_Clear(0);

	 //ʵ�ʻ�ͼ,��System_Initiate�ϼ���ͼƬʱû����������ͼ������,��Ϊ����ͼ
	 hspr->Render(0,y1);
	 hspr->Render(0,y2);

	 //���Ʒɻ�ͼ 480 852
	 hsprplane1->Render(x_plane,y_plane);

	 //�����ӵ�ͼ
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

	 //���Ƶ���ͼ
     for(i=0;i<10;i++){
		srand(time(NULL));
		enemy[i].x_enemy=rand()%(480-98);
		enemy[i].y_enemy=rand()%(798-73);
		hsprenemy->Render(enemy[i].x_enemy,enemy[i].y_enemy);
	 }

     //������ͼ
	 hge->Gfx_EndScene();

	 return false;
 }

 //��ں���
 int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
	 //��hgeָ�븳ֵ��hge����汾��
	 hge=hgeCreate(HGE_VERSION);

	 //����֡����
	 hge->System_SetState(HGE_FRAMEFUNC,FrameFunc);

	 //������Ⱦ����
	 hge->System_SetState(HGE_RENDERFUNC,RenderFunc);

	 //���ô�������
	 hge->System_SetState(HGE_TITLE,"��ӭ�������ӵ�����");

	 hge->System_SetState(HGE_WINDOWED,true);
	 hge->System_SetState(HGE_SCREENWIDTH,480);
	 hge->System_SetState(HGE_SCREENHEIGHT,852);

	 //���������
	 hge->System_SetState(HGE_HIDEMOUSE,false);

	 //��ʹ������
	 hge->System_SetState(HGE_USESOUND,false);
	 hge->System_SetState(HGE_FPS,1000);//֡

	 //��ʼ������
	 if(hge->System_Initiate()){
		 //�Լ��ĳ�ʼ��  ��:����ͼƬ

		 //���ر���ͼƬ(��������ͼƬ)
         htex=hge->Texture_Load("res/background.png");
         hspr=new hgeSprite(htex,0,0,480,852);

		 //���طɻ�ͼƬ
		 htexplane1=hge->Texture_Load("res/hero1.png");
         hsprplane1=new hgeSprite(htexplane1,0,0,102,126);

		 //�����ӵ�ͼƬ
         htexbullet=hge->Texture_Load("res/bullet2.png");
		 hsprbullet=new hgeSprite(htexbullet,0,0,5,11);

		 //���ص���ͼƬ
         htexenemy=hge->Texture_Load("res/enemy1.png");
		 hsprenemy=new hgeSprite(htexenemy,0,0,57,43);

		 //��������
         hfet=hge->Effect_Load("res/game_music.mp3");

		 //�������
		 vBullet1.clear();

		 //��������: ѭ��ִ��֡����
		 hge->System_Start();
	 }
	 else{
         //��ʼ��ʧ��
		 MessageBox(NULL, hge->System_GetErrorMessage(), "Error",
                  MB_OK | MB_ICONERROR | MB_APPLMODAL);
	 }

	 //�رմ���
	 //�ر�����
	 hge->System_Shutdown();
	 
	 //�ͷ�hgeָ��
	 hge->Release();


	 delete hspr;
     delete hsprplane1;
	 delete hsprplane2;
	 delete hsprbullet;

	 return 0;
 }
