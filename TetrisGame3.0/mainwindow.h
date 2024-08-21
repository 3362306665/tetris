#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"game_interface.h"

//游戏背景音乐
#include<QMediaPlayer>

//前置声明
class Game_interface;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


        //点击开始游戏跳转到游戏界面

        //点击退出退出界面

    //控制方块移动

    //计分

    //难度递增

    //记录最高分

    //保存和恢复进度

private slots:
    void on_start_clicked();

    void on_exit_clicked();

    void ContinueGame();//读取上一次的进度，继续游戏

private:
    Ui::MainWindow *ui;
    Game_interface*game_interface;//游戏界面对象


};
#endif // MAINWINDOW_H
