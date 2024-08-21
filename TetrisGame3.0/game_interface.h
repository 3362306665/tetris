#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H
#include <QWidget>
#include <QMainWindow>
#include<QMessageBox>
#include<QPainter>
#include<QKeyEvent>
#include<time.h>
//游戏背景音乐
#include<QMediaPlayer>
//游戏场景边距

const int MARGINS=20;
//游戏场景行数和列数
const int AREA_ROW=15,AREA_COL=12;
//单个方块单元的边长  宽和高都是40像素
const int BLOCK_SIZE=40;
//控制方向

enum ControlDirection
{
    UP,DOWN,LEFT,RIGHT,SPACE
};
//边界信息处理
class Borders
{
public:
    int up_Bound;
    int down_Bound;
    int left_Bound;
    int right_Bound;

};
//坐标位置
class block_point
{
    public:
    int p_x;
    int p_y;

};

class MainWindow;

namespace Ui {
class Game_interface;
}

class Game_interface : public QWidget
{
    Q_OBJECT

public:
    explicit Game_interface(QWidget *parent = nullptr);
    ~Game_interface();





    void StartGame();//开始游戏
    void StartGame2();//第二种开始游戏
    void GameOver();//游戏结束
    void ResetBlock();//重置方块

    void BlockMove(ControlDirection dir);//方块移动
    //void BlockMove2(ControlDirection dir);
    void CreateBlock(int block[4][4],int block_id);//生成方块
    void RoateBlock(int[4][4]);//旋转方块
    void GetBorder(int[4][4],Borders &border);//计算边界
    bool IsCollision(int x,int y,ControlDirection dir);//判断是否碰撞
    void ConvertStable(int x,int y);//转换为稳定方块
    void SaveGame();//保存游戏数据
    void ReadGame();//读入游戏数据

    virtual void paintEvent(QPaintEvent*event);//游戏场景刷新
    virtual void timerEvent(QTimerEvent*event);//定时器事件
    virtual void keyPressEvent(QKeyEvent*event);//键盘响应

public:
    //游戏场景区域：1表示活动的方块，2表示稳定标准的方块，0表示空
    int Game_areas[AREA_ROW][AREA_COL]={};
    //当前方块的坐标
    block_point current_BlockPoint;
    //当前方块的形状
    int current_blockShape[4][4];
    //当前方块的边界位置
    Borders current_border;
    //下一个方块的形状
    int next_blockShape[4][4];
    //当前方块是否为稳定标准
    bool isStable;
    //游戏分数统计
    int iScore=0;
    //方块下落计时器
    int iGame_time;

    //渲染刷新计时器
    int iPaint_time;

    //下落时间间隔
    int iSpeed_ms;
    //刷新时间间隔
    int irefresh_ms;
    //难度等级
    int check_point;
    //是否暂停
    bool is_pause;



private slots:
    void on_ruturn_clicked();
    void InitGame();//初始化游戏
    void PauseGame();//暂停游戏并弹出新窗口








private:
    Ui::Game_interface *ui;
     QWidget*gamearea;


};

#endif // GAME_INTERFACE_H
