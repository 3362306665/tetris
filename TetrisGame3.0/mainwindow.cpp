#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"game_interface.h"
#include <fstream>  // 文件操作流
#include <iostream> // 错误处理
#include <iomanip>  // 用于设置文件格式





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1024,1024);
    this->setWindowTitle("俄罗斯方块");
    game_interface=new Game_interface;
    //start发出信号（clicked）,給this界面，调用槽函数on_start_clicked
     connect(ui->start, &QPushButton::clicked, this, &MainWindow::on_start_clicked);
     connect(ui->continue_2,&QPushButton::clicked,this,&MainWindow::ContinueGame);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_start_clicked()
{


    //隐藏当前界面
    this->hide();
   //点击开始按钮跳转到游戏界面
    game_interface->show();

}
void MainWindow::ContinueGame()
{
    //隐藏当前界面
    this->hide();
   //点击继续按钮跳转到游戏界面
    game_interface->show();
    game_interface->ReadGame();

}


void MainWindow::on_exit_clicked()//??????????????????????
{
    this->close();
}
