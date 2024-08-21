#include "game_interface.h"
#include "ui_game_interface.h"
#include"mainwindow.h"
#include<QMessageBox>
#include<QTime>
#include<QSound>
Game_interface::Game_interface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game_interface)
{
    ui->setupUi(this);
    connect(ui->return_2, &QPushButton::clicked, this, &Game_interface::on_ruturn_clicked);
    connect(ui->start, &QPushButton::clicked, this, &Game_interface::InitGame);
    connect(ui->stop,&QPushButton::clicked,this,&Game_interface::PauseGame);
    // 设置游戏背景图片
        QPixmap bkground(":/bcground.png");
        bkground=bkground.scaled(this->size(),Qt::IgnoreAspectRatio);
        QPalette palatte;
        palatte.setBrush(QPalette::Background,bkground);
        this->setPalette(palatte);

        // 设置游戏背景音乐
        QMediaPlayer *player = new QMediaPlayer;
            player->setMedia(QUrl("qrc:/Inmysong.mp3"));
            player->setVolume(10); // 设置音量
            player->play();


    //固定窗口大小
    setFixedSize(this->width(),this->height());
    ui->start->setFocusPolicy(Qt::NoFocus);
    ui->return_2->setFocusPolicy((Qt::NoFocus));
    ui->stop->setFocusPolicy((Qt::NoFocus));
}

Game_interface::~Game_interface()
{

    delete ui;
}
void Game_interface::on_ruturn_clicked()
{
    //关闭当前窗口
    this->close();
    //返回主界面
    MainWindow *main_window=new MainWindow;
    //取消按钮焦点

    SaveGame();//保存数据

    main_window->show();
}
//游戏暂停
void Game_interface::PauseGame()
{
    //停止计时器
       killTimer(iGame_time);
       killTimer(iPaint_time);
       //弹出窗口是否返回游戏

       QMessageBox::StandardButton reply;
       reply = QMessageBox::question(this, "游戏暂停",
                                     "根据指示选择是否继续",
                                     QMessageBox::Yes | QMessageBox::No);


       if (reply == QMessageBox::Yes)
       {
           // 用户选择继续游戏，重新启动游戏相关的定时器
           // 注意：这里假设 iGame_time 和 iPaint_time 是已经定义并初始化好的定时器 ID
           iGame_time = startTimer(iSpeed_ms);
           iPaint_time = startTimer(irefresh_ms);
       }
       if (reply == QMessageBox::Yes)
       {
           // 用户选择继续游戏，重新启动游戏相关的定时器
           //gamePaused = false;
           iGame_time = startTimer(iSpeed_ms);
           iPaint_time = startTimer(irefresh_ms);
       }

          else
          {
              // 用户选择不继续游戏，可以在这里处理游戏退出或其他操作
              this->close();

          }




}

void Game_interface::SaveGame()
{
    //停止计时器
    killTimer(iGame_time);
    killTimer(iPaint_time);

    //读入Game_area[][]数据
    QString fileName = "Game_program.txt";
       QFile file(fileName);

   //QIODevice::WriteOnly 标志打开文件时清空原有内容，如果文件不存在，系统会尝试创建一个新的文件来满足写入操作的需求。
       if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
       {
           qDebug() << "Could not open file for writing: " << file.errorString();
           return;
       }

       // 创建一个 QTextStream 以便于写入文件
       QTextStream out(&file);

       // 写入游戏方块的数据到文件
       for (int i = 0; i < AREA_ROW; i++)
       {
           for (int j = 0; j < AREA_COL; j++)
           {
               out << Game_areas[i][j] << " ";
           }
           out << endl; // 每行结束换行
       }
       //写入难度，得分
       out<<iScore<<endl;
       out<<iSpeed_ms<<endl;
       out<<irefresh_ms<<endl;
       out<<check_point<<endl;
       out<<current_BlockPoint.p_x<<current_BlockPoint.p_y;
       //qDebug()<<current_BlockPoint.p_x<<current_BlockPoint.p_y;
       out<<endl;
       for(int i=0;i<4;i++)
       {
           for(int j=0;j<4;j++)
           {
               out<<current_blockShape[i][j]<< " ";
           }
            out << endl;
       }
       out<<current_border.up_Bound<<current_border.down_Bound<<current_border.left_Bound<<current_border.right_Bound;
       out<<endl;
       for(int i=0;i<4;i++)
       {
           for(int j=0;j<4;j++)
           {
               out<<next_blockShape[i][j]<< " ";
           }
            out << endl;
       }
       out<<isStable;


}
void Game_interface::ReadGame()
{
    QString fileName = "Game_program.txt";
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))  // 以只读和文本模式打开文件
    {
        qDebug() << "Could not open file for reading: " << file.errorString();
        return;
    }

    QTextStream in(&file);  // 创建用于读取的 QTextStream 对象
    //读取Game_areas[][]
    for(int i=0;i<AREA_ROW;i++)
    {
        for(int j=0;j<AREA_COL;j++)
        {
            in>>Game_areas[i][j];

        }

    }
    //读取得分，难度，iGame_time，check_point
    in>>iScore;
    in>>iSpeed_ms;
    in>>irefresh_ms;
    in>>check_point;
    in>>current_BlockPoint.p_x<<current_BlockPoint.p_y;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            in<<current_blockShape[i][j];
        }
    }
    in<<current_border.up_Bound<<current_border.down_Bound<<current_border.left_Bound<<current_border.right_Bound;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            in<<next_blockShape[i][j];
        }
    }
    in<<isStable;

    file.close();
    StartGame2();



}


//定义方块图案形状代码和边界

//田
int block_kind1[4][4]=
{
    {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0}
};
//右L
int block_kind2[4][4]=
{
    {0,1,0,0},
    {0,1,0,0},
    {0,1,1,0},
    {0,0,0,0}
};
//左L
int block_kind3[4][4]=
{
    {0,0,1,0},
    {0,0,1,0},
    {0,1,1,0},
    {0,0,0,0}
};
//左z
int block_kind4[4][4]=
{
    {0,1,0,0},
    {0,1,1,0},
    {0,0,1,0},
    {0,0,0,0}
};
//右z
int block_kind5[4][4]=
{
    {0,0,1,0},
    {0,1,1,0},
    {0,1,0,0},
    {0,0,0,0}
};
//倒T
int block_kind6[4][4]=
{
    {0,0,0,0},
    {0,0,1,0},
    {0,1,1,1},
    {0,0,0,0}
};
//长条
int block_kind7[4][4]=
{
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0}
};
//正确
//复制方块图案函数
void block_copy(int dblock[4][4],int sblock[4][4])
{

    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            dblock[i][j]=sblock[i][j];
        }
    }
}


//正确
//作为槽函数 点击开始调用
void Game_interface::InitGame()//初始化方块
{
    //游戏场景区域初始化为0

    for(int i=0;i<AREA_ROW;i++)
    {
        for (int j=0;j<AREA_COL;j++)
        {
            Game_areas[i][j]=0;

        }
    }
    //
    iSpeed_ms=1500;
    irefresh_ms=500;
    //初始化随机数种子
    srand(time(0));
    //关卡初始化

    check_point=1;
    //分数初始化
    iScore=0;
    //开始游戏
    StartGame();

}

//正确
void Game_interface::StartGame()//开始游戏
{
        //测试
        qDebug() << "调用startGame!";
        //方块下落计时器
        iGame_time = startTimer(iSpeed_ms);
         qDebug() << "iGame_time计时器创建";
        //界面刷新的时间间隔，用来控制游戏画面的更新频率。
        iPaint_time = startTimer(irefresh_ms);
        qDebug() << "iPaint_time计时器创建";

        //产生0~6之内的数 对应7种不同类型的方块
        int block_id = rand()%7;
        //产生方块
        CreateBlock(next_blockShape, block_id);//！！！！
        //重置方块
        ResetBlock();

}
void Game_interface::StartGame2()
{
    //把活动界面中活动方块清空
    for(int i=0;i<AREA_ROW;i++)
    {
        for (int j=0;j<AREA_COL;j++)
        {
            if(Game_areas[i][j]==1)
            {
            Game_areas[i][j]=0;
            }

        }
    }

    //当前方块初始化为0
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            current_blockShape[i][j]=0;
        }

    }
    //测试
    qDebug() << "调用startGame2!";
    //方块下落计时器
    iGame_time = startTimer(iSpeed_ms);
     qDebug() << "iGame_time计时器创建";
    //界面刷新的时间间隔，用来控制游戏画面的更新频率。
    iPaint_time = startTimer(irefresh_ms);
    qDebug() << "iPaint_time计时器创建";

    //产生0~6之内的数 对应7种不同类型的方块
    //int block_id = rand()%7;
    //重置方块
    ResetBlock();






}
void Game_interface::GameOver()//游戏结束
{
    //游戏结束停止定时器
        killTimer(iGame_time);
        killTimer(iPaint_time);
        QMessageBox::information(this, "失败","游戏结束");


}
void Game_interface::ResetBlock()//重置方块
{
    //产生当前方块
        block_copy(current_blockShape, next_blockShape);
        //计算当前方块的边界
        GetBorder(current_blockShape, current_border);

        //产生下一个方块
        int block_id = rand()%7;
        CreateBlock(next_blockShape, block_id);

        //设置初始方块坐标，以方块左上角为锚点
        block_point start_point;
        start_point.p_x = AREA_COL/2-2;
        start_point.p_y= 0;
        current_BlockPoint= start_point;


}
void Game_interface::BlockMove(ControlDirection dir)//方块移动
{
    switch (dir) {
        case UP:
            if(IsCollision(current_BlockPoint.p_x,current_BlockPoint.p_y,UP))
                break;
            //逆时针旋转90度
            RoateBlock(current_blockShape);//旋转函数
            //防止旋转后bug,i和j从0到4重新设置方块
            for(int i=0;i<4;i++)
            {
                for(int j=0;j<4;j++)
                {
                    qDebug()<<"使用gameare";
                    Game_areas[current_BlockPoint.p_y+i][current_BlockPoint.p_x+j]=current_blockShape[i][j];
                }
            }
            //重新计算边界
            GetBorder(current_blockShape,current_border);
            break;

        case DOWN:
            //方块到达边界则不再移动
            if(current_BlockPoint.p_y+current_border.down_Bound==AREA_ROW-1)
            {
                ConvertStable(current_BlockPoint.p_x,current_BlockPoint.p_y);
                ResetBlock();
                break;
            }
            //碰撞检测，只计算上下左右边界，先尝试走一格，如果碰撞则稳定方块后跳出
            if(IsCollision(current_BlockPoint.p_x,current_BlockPoint.p_y,DOWN))
            {
                //只有最终不能下落才转成稳定方块
                ConvertStable(current_BlockPoint.p_x,current_BlockPoint.p_y);
                ResetBlock();
                break;
            }
            //恢复方块上场景,为了清除移动过程中的方块残留
            for(int j=current_border.left_Bound;j<=current_border.right_Bound;j++)
            {
                qDebug()<<"使用gameare";
                Game_areas[current_BlockPoint.p_y][current_BlockPoint.p_x+j]=0;
            }
            //没有碰撞则下落一格
            current_BlockPoint.p_y+=1;
            //方块下降一格，拷贝到场景,注意左右边界
            for(int i=0;i<4;i++) //必须是0到4而不是边界索引，考虑到旋转后边界重新计算
                for(int j=current_border.left_Bound;j<=current_border.right_Bound;j++)
                    if(current_BlockPoint.p_y+i<=AREA_ROW-1&&Game_areas[current_BlockPoint.p_y+i]
                            [current_BlockPoint.p_x+j]!=2) //注意场景数组不越界,而且不会擦出稳定的方块
                        Game_areas[current_BlockPoint.p_y+i][current_BlockPoint.p_x+j]=current_blockShape[i][j];
            break;

         case LEFT:
           //到左边界或者碰撞不再往左
            if(current_BlockPoint.p_x+current_border.left_Bound==0||IsCollision(current_BlockPoint.p_x,current_BlockPoint.p_y,LEFT))
                break;
            //恢复方块右场景,为了清除移动过程中的方块残留
            for(int i=current_border.up_Bound;i<=current_border.down_Bound;i++)
                Game_areas[current_BlockPoint.p_y+i][current_BlockPoint.p_x+3]=0;
            current_BlockPoint.p_x-=1;
            //方块左移一格，拷贝到场景
            for(int i=current_border.up_Bound;i<=current_border.down_Bound;i++)
                for(int j=0;j<4;j++)
                    if(current_BlockPoint.p_x+j>=0&&Game_areas[current_BlockPoint.p_y+i][current_BlockPoint.p_x+j]!=2) //注意场景数组不越界
                        Game_areas[current_BlockPoint.p_y+i][current_BlockPoint.p_x+j]=current_blockShape[i][j];
            break;
        case RIGHT:
            if(current_BlockPoint.p_x+current_border.right_Bound==AREA_COL-1||IsCollision(current_BlockPoint.p_x,current_BlockPoint.p_y,RIGHT))
                break;
            //恢复方块左场景,为了清除移动过程中的方块残留
            for(int i=current_border.up_Bound;i<=current_border.down_Bound;i++)
                Game_areas[current_BlockPoint.p_y+i][current_BlockPoint.p_x]=0;
            current_BlockPoint.p_x+=1;
            //方块右移一格，拷贝到场景
            for(int i=current_border.up_Bound;i<=current_border.down_Bound;i++)
                for(int j=0;j<4;j++)
                    if(current_BlockPoint.p_x+j<=AREA_COL-1&&Game_areas[current_BlockPoint.p_y+i][current_BlockPoint.p_x+j]!=2) //注意场景数组不越界
                        Game_areas[current_BlockPoint.p_y+i][current_BlockPoint.p_x+j]=current_blockShape[i][j];
            break;


        case SPACE: //一次到底
            //一格一格下移，直到不能下移
            while(current_BlockPoint.p_y+current_border.down_Bound<AREA_ROW-1&&!IsCollision(current_BlockPoint.p_x,current_BlockPoint.p_y,DOWN))
            {
                //恢复方块上场景,为了清除移动过程中的方块残留
                for(int j=current_border.left_Bound;j<=current_border.right_Bound;j++)
                    Game_areas[current_BlockPoint.p_y][current_BlockPoint.p_x+j]=0;
                //没有碰撞则下落一格
                current_BlockPoint.p_y+=1;
                //方块下降一格，拷贝到场景,注意左右边界
                for(int i=0;i<4;i++) //必须是0到4
                    for(int j=current_border.left_Bound;j<=current_border.right_Bound;j++)
                        if(current_BlockPoint.p_y+i<=AREA_ROW-1&&Game_areas[current_BlockPoint.p_y+i][current_BlockPoint.p_x+j]!=2) //注意场景数组不越界,而且不会擦出稳定的方块
                            Game_areas[current_BlockPoint.p_y+i][current_BlockPoint.p_x+j]=current_blockShape[i][j];
            }
            ConvertStable(current_BlockPoint.p_x,current_BlockPoint.p_y);
            ResetBlock();
            break;
        default:
            break;
        }

    //?????????????????????????????????????????????????????????????????????
        //处理消行，整个场景上面的行依次下移
        int i=AREA_ROW-1;
        int line_count=0; //记消行数
        while(i>=1)
        {
            bool is_line_full=true;
            for(int j=0;j<AREA_COL;j++)
                if(Game_areas[i][j]==0)
                {
                    is_line_full=false;
                    i--;
                    break;
                }
            if(is_line_full)
            {
                for(int k=i;k>=1;k--)
                    for(int j=0;j<AREA_COL;j++)
                        Game_areas[k][j]=Game_areas[k-1][j];
                line_count++;//每次增加消行的行数
                QMediaPlayer *player = new QMediaPlayer;
                    player->setMedia(QUrl("qrc:/eliminate_sound.mp3"));
                    player->setVolume(50); // 设置音量
                    player->play();
            }
        }
        iScore+=line_count*10; //得分
        //增加音效
       //QSound::play("qrc:/eliminate_sound.mp3");


        //难度递增，每加十分下落速度快100
        check_point=iScore/10+1;
        iGame_time = startTimer(iSpeed_ms-100*check_point);

        //判断游戏是否结束
        for(int j=0;j<AREA_COL;j++)
            if(Game_areas[0][j]==2) //最顶端也有稳定方块
                GameOver();

}
//正确
void Game_interface::CreateBlock(int block[4][4],int block_id)//生成方块
{
    switch (block_id)
       {
       case 0:
           block_copy(block,block_kind1);
           break;
       case 1:
           block_copy(block,block_kind2);
           break;
       case 2:
           block_copy(block,block_kind3);
           break;
       case 3:
           block_copy(block,block_kind4);
           break;
       case 4:
           block_copy(block,block_kind5);
           break;
       case 5:
           block_copy(block,block_kind6);
           break;
       case 6:
           block_copy(block,block_kind7);
           break;
       default:
           break;
       }






}
//排除
void Game_interface::RoateBlock(int block[4][4])//旋转方块 顺时针
{
    int temp_block[4][4];
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            temp_block[3-j][i]=block[i][j];
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            block[i][j]=temp_block[i][j];

}
//正确
void Game_interface::GetBorder(int block[4][4],Borders &border)//计算边界
{
    int i = 0, j = 0;
        //计算上下左右边界
        //上边界
        for(i=3; i>=0; i--)
        {
            for(j=0; j<4; j++)
            {
                if(block[i][j] == 1)
                {
                    border.up_Bound = i;
                    break;
                }
            }
        }

        //下边界
        for(i=0; i<4; i++)
        {
            for(j=0; j<4; j++)
            {
                if(block[i][j] == 1)
                {
                    border.down_Bound = i;
                    break;
                }
            }
        }
        //左边界
        for(j=3; j>=0; j--)
        {
            for(i=0; i<4; i++)
            {
                if(block[i][j] == 1)
                {
                    border.left_Bound = j;
                    break;
                }
            }
        }

        //右边界
        for(j=0; j<4; j++)
        {
            for(i=0; i<4; i++)
            {
                if(block[i][j] == 1)
                {
                    border.right_Bound = j;
                    break;
                }
            }
        }
}
bool Game_interface::IsCollision(int x,int y,ControlDirection dir)//判断是否碰撞
{
    int temp_block[4][4];
        block_copy(temp_block, current_blockShape);

        Borders temp_border;
        GetBorder(temp_block, temp_border);

        switch(dir)
        {
        case UP:
            RoateBlock(temp_block);//旋转方块
            GetBorder(temp_block, temp_border); //旋转后要重新计算边界
            break;

        case DOWN:
            y += 1;
            break;

        case LEFT:
            x -= 1;
            break;

        case RIGHT:
            x += 1;
            break;

        default:
            break;
        }

        for(int i=temp_border.up_Bound; i<=temp_border.down_Bound; i++)
        {
            for(int j=temp_border.left_Bound; j<=temp_border.right_Bound; j++)
            {
                if(Game_areas[y+i][x+j]==2&&temp_block[i][j]==1||x+temp_border.left_Bound<0||x+temp_border.right_Bound>AREA_COL-1)
                    return true;
            }
        }

        return false;


}
//转换为稳定方块 排除
void Game_interface::ConvertStable(int x,int y)
{
    for(int i=current_border.up_Bound;i<=current_border.down_Bound;i++)
        for(int j=current_border.left_Bound;j<=current_border.right_Bound;j++)
            if(current_blockShape[i][j]==1)
                Game_areas[y+i][x+j]=2; //x和y别搞反
}

//事件处理函数，它在需要重绘窗口部件时被自动调用，而不是需要手动调用
void Game_interface::paintEvent(QPaintEvent*event)//游戏场景刷新
{
     qDebug()<<"painyevent事件触发";
    Q_UNUSED(event);

        QPainter painter(this);
        //游戏区域

         painter.drawRect(MARGINS,MARGINS,AREA_COL*BLOCK_SIZE,AREA_ROW*BLOCK_SIZE);


         //画方块预告   成功

             //painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
            //测试 painter.drawRect(20,10,BLOCK_SIZE,BLOCK_SIZE);
             for(int i=0; i<4; i++)
             {
                 for(int j=0; j<4; j++)
                 {
                     if(next_blockShape[i][j] == 1)
                     {


                         painter.setBrush(Qt::red);

                   painter.setBrush(Qt::red);
                   painter.drawRect(MARGINS*8+AREA_COL*BLOCK_SIZE+j*BLOCK_SIZE/2,MARGINS*13+i*BLOCK_SIZE/2,BLOCK_SIZE/2,BLOCK_SIZE/2);
                   update();

                     }
                 }
             }
          //绘制得分 成功

              painter.setPen(Qt::black);
              painter.setFont(QFont("Arial", 10));
              //数据！！！！！！！！！！！！！！
              //数据！！！！！！！！！！！！！！
              //painter.drawText(QRect(MARGINS*25,MARGINS*4,100,100),Qt::AlignCenter,QString::number(iScore));//得分
              //painter.drawText(QRect(MARGINS*25,MARGINS/3,100,100),Qt::AlignCenter,QString::number(0));//关卡
              ui->score_show->setText(QString::number(iScore));
              ui->check_show->setText(QString::number(check_point));




              //绘制下落方块和稳定方块，方块边线的颜色是根据setPen来的，默认黑色
               for(int i=0; i<AREA_ROW; i++)
                {
                    for(int j=0; j<AREA_COL; j++)
                     {
                          //绘制活动方块
                        //qDebug()<<"gamearea:"<<i<<"  "<<j<<Game_areas[i][j];
                        if(Game_areas[i][j] == 1)//？？？？？无法执行
                        {

                           qDebug()<<"绘制活动方块";
                           painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
                           painter.drawRect(j*BLOCK_SIZE+MARGINS, i*BLOCK_SIZE+MARGINS, BLOCK_SIZE, BLOCK_SIZE);
                        }
                          //绘制稳定方块

                          else if(Game_areas[i][j] == 2)
                          {

                            qDebug()<<"绘制稳定方块";
                              painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
                              painter.drawRect(j*BLOCK_SIZE+MARGINS,i*BLOCK_SIZE+MARGINS,BLOCK_SIZE,BLOCK_SIZE);
                          }
                      }
                }




}
//排除
void Game_interface::timerEvent(QTimerEvent *event)//定时
{

    qDebug()<<"timeevent事件触发";
                       //方块下落
             if(event->timerId() == iGame_time)

                  BlockMove(DOWN);

                       //刷新画面
             if(event->timerId() == iPaint_time)
                           update();
}

//排除
void Game_interface::keyPressEvent(QKeyEvent*event)//键盘响应
{
    qDebug()<<"keyEvent事件发生";
    switch(event->key())
    {
    case Qt::Key_Up:
        BlockMove(UP);
        break;
    case Qt::Key_Down:
        BlockMove(DOWN);
        break;
    case Qt::Key_Left:
        BlockMove(LEFT);
        break;
    case Qt::Key_Right:
        BlockMove(RIGHT);
        break;
    case Qt::Key_Space:
        BlockMove(SPACE);
        break;
    default:
        break;
    }

}
