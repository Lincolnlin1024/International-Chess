#include "game.h"

Game::Game(QWidget *parent)
    : QMainWindow{parent}
{
}

Game::Game(int choice,int saved){
    if(choice==4){//打开已有棋局
        QFile file;
        if(saved==1){
            file.setFileName("./saved_game/game1.txt");
        }
        else if(saved==2){
            file.setFileName("./saved_game/game2.txt");
        }
        else if(saved==3){
            file.setFileName("./saved_game/game3.txt");
        }
        else{
            file.setFileName("./saved_game/game4.txt");
        }
        twoPlayersInit(1,saved);
        newProject(file,this);
        this->saved_project = saved;
    }
    else{
        twoPlayersInit(choice,saved);
    }
}

void Game::twoPlayersInit(int type,int saved){

    setBackgroundPixmap(QPixmap(":/source/pictures/GameBackground.png"));
    saved_project = saved;
    board_color_type = type;
    null_pixmap = QPixmap(":/source/pictures/White.png");

    // 棋盘（游戏主体）
    chessboard = new ChessBoard(this);
    chessboard->boardInital(type);
    QWidget* center = new QWidget(this);
    QHBoxLayout * layout_main = new QHBoxLayout();

    QHBoxLayout * layout_board = new QHBoxLayout();
    layout_board->addSpacerItem(new QSpacerItem(0,160,QSizePolicy::Expanding,QSizePolicy::Fixed));
    layout_board->addWidget(chessboard);
    layout_board->addSpacerItem(new QSpacerItem(0,160,QSizePolicy::Expanding,QSizePolicy::Fixed));
    layout_board->setSpacing(0);
    QWidget * board = new QWidget(this);
    board->setLayout(layout_board);

    QGridLayout * layout_user = new QGridLayout();
    QFont font;
        //设置文字字体
    font.setFamily("Microsoft Yahei");
    font.setWeight(QFont::Bold);
    font.setPixelSize(10);
    font.setPointSize(10);
        //基本控件初始化
    basicElementInitial(font,type);
    QLabel * step_count = new QLabel(this);
    QString text_step = "count";
    step_count->setText(text_step);
    step_count->setText(text_step.split("",Qt::SkipEmptyParts).join("\n"));
    step_count->setAlignment(Qt::AlignCenter);
    step_count->setFixedSize(30,90);
    step_count->setFont(font);
        //行动信息栏
    QTextBrowser * movement_information = new QTextBrowser(this);
    QString text_move = "Game Start!";
    movement_information->insertPlainText(text_move);
    movement_information->moveCursor(QTextCursor::End);
    movement_information->append(QString(""));
    movement_information->setAlignment(Qt::AlignCenter);
    movement_information->setFixedSize(240,150);
    movement_information->setFont(font);
        //认输按钮
    QPushButton * give_up_button = new QPushButton("Give Up",this);
    give_up_button->setFixedSize(75,30);
    give_up_button->setFont(font);
    connect(give_up_button,&QPushButton::clicked,this,[=](){
        if(QMessageBox::Yes == QMessageBox::question(this,"Give Up","Do you really want to give up",QMessageBox::Yes|QMessageBox::No,QMessageBox::No)){
            QChar step[6]={' ',' ','-',' ','\0'};

            if(chessboard->getCurrentTeam()){//现在是黑方持棋，则认输的是黑方
                qDebug()<<"Black Give Up";
                step[1]='1';
                step[3]='0';
                chessboard->getRecord().append("Black Give Up");
            }
            else{
                qDebug()<<"White Give Up";
                qDebug()<<chessboard->getCurrentTeam();
                step[1]='0';
                step[3]='1';
                chessboard->getRecord().append("White Give Up");
            }
            QString record_step = QString(step);

            chessboard->getRecord().append(record_step);

            chessboard->End();
        }
    });
        //求和按钮
    QPushButton * draw_button = new QPushButton("Draw",this);
    draw_button->setFixedSize(75,30);
    draw_button->setFont(font);
    connect(draw_button,&QPushButton::clicked,this,[=](){
        if(QMessageBox::Yes == QMessageBox::question(this,"Draw","Do you really want to draw a match",QMessageBox::Yes|QMessageBox::No,QMessageBox::No)){
            QChar step[9]={' ','1','/','2','-','1','/','2','\0'};
            QString record_step = QString(step);

            chessboard->getRecord().append(record_step);

            chessboard->End();
        }
    });
        //返回菜单按钮
    QPushButton * return_button = new QPushButton("Return To Menu",this);
    return_button->setFixedSize(155,30);
    return_button->setFont(font);
    connect(return_button,&QPushButton::clicked,this,[=](){
        if(QMessageBox::Yes == QMessageBox::question(this,"Return To Menu","Do you really want to return",QMessageBox::Yes|QMessageBox::No,QMessageBox::No)){
            save_project();
            MainWindow * menu = new MainWindow();
            this->close();
            menu->show();
        }
    });
        //悔棋按钮
    QPushButton * regret_button = new QPushButton("Regret",this);
    regret_button->setFixedSize(155,30);
    regret_button->setFont(font);
        //布局
    layout_user->addWidget(avatar1,1,1,2,2,Qt::Alignment());
    layout_user->addWidget(avatar2,1,4,2,2,Qt::Alignment());
    layout_user->addWidget(holder1,2,1,2,2,Qt::Alignment());
    layout_user->addWidget(holder2,2,4,2,2,Qt::Alignment());
    layout_user->addWidget(step1,3,1,1,2,Qt::Alignment());
    layout_user->addWidget(step2,3,4,1,2,Qt::Alignment());
    layout_user->addWidget(step_count,2,0,3,1,Qt::Alignment());
    layout_user->addWidget(movement_information,4,0,5,8,Qt::Alignment());;
    layout_user->addWidget(give_up_button,8,1,1,3,Qt::AlignLeft);
    layout_user->addWidget(draw_button,8,3,1,3,Qt::AlignLeft);
    layout_user->addWidget(return_button,10,1,1,5,Qt::Alignment());
    layout_user->addWidget(regret_button,12,1,1,5,Qt::Alignment());

    layout_user->setSpacing(20);
    QWidget * user = new QWidget();
    user->setLayout(layout_user);

    user->setStyleSheet(QString("QWidget { "
                                "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
                                "stop:0 rgb(117, 120, 232), stop:0.4 rgb(182, 123, 225), stop:1 rgb(109, 208, 214)); "

                                "border:5px ridge rgb(180, 237, 221);"
                                "border-radius:15px;"
                                "color:rgb(72, 100, 210);"
                                "}"));

    layout_main->addWidget(board);
    layout_main->addWidget(user);

    center->setLayout(layout_main);
    setCentralWidget(center);

    // 菜单栏
    QMenuBar * menu = menuBar();
    setMenuBar(menu);
    QMenu * file_menu = menu->addMenu("File");
    QMenu * edit_menu = menu->addMenu("Edit");
    QMenu * puzzle_menu = menu->addMenu("Puzzle");

    QAction * new_action = file_menu->addAction("New Game",QKeySequence(QLatin1String("Ctrl+N")));
    file_menu->addSeparator();
    QAction * open_action = file_menu->addAction("Open Game",QKeySequence(QLatin1String("Ctrl+O")));
    QAction * save_action = file_menu->addAction("Save Game",QKeySequence(QLatin1String("Ctrl+S")));
    file_menu->addSeparator();
    QAction * tool_bar = file_menu->addAction("Tool Bar Switch",QKeySequence(QLatin1String("Ctrl+T")));

    new_action->setFont(font);
    open_action->setFont(font);
    save_action->setFont(font);
    tool_bar->setFont(font);

    connect(chessboard,&ChessBoard::endSuccess,this,[=](){
        if(saved_project==1){
            QFile file("./saved_game/game1.txt");
            file.open(QFile::WriteOnly|QFile::Truncate);
            file.close();
            null_pixmap.save("./saved_game/game1.png");
        }
        else if(saved_project==2){
            QFile file("./saved_game/game2.txt");
            file.open(QFile::WriteOnly|QFile::Truncate);
            file.close();
            null_pixmap.save("./saved_game/game2.png");
        }
        else if(saved_project==3){
            QFile file("./saved_game/game3.txt");
            file.open(QFile::WriteOnly|QFile::Truncate);
            file.close();
            null_pixmap.save("./saved_game/game3.png");
        }
        else{
            QFile file("./saved_game/game4.txt");
            file.open(QFile::WriteOnly|QFile::Truncate);
            file.close();
            null_pixmap.save("./saved_game/game4.png");
        }

        MainWindow * menu = new MainWindow();
        this->close();
        menu->show();
    });
    connect(new_action, &QAction::triggered, this, &Game::new_project);
    connect(open_action, &QAction::triggered, this, &Game::open_project);
    connect(save_action, &QAction::triggered, this, &Game::save_project);


    // 工具栏
    toolbar = new QToolBar(this);
    addToolBar(Qt::LeftToolBarArea,toolbar);//加了Qt::LeftToolBarArea参数就默认在左边，没这个参数默认在上面
    //设置不能移动
    toolbar->setMovable(false);//让它不能移动&浮动
    //工具栏中设置file菜单的内容
    toolbar->addAction(new_action);
    toolbar->addSeparator();//添加分割线
    toolbar->addAction(open_action);
    toolbar->addAction(save_action);
    toolbar->addSeparator();//添加分割线
    toolbar->addAction(tool_bar);
    toolbar->setVisible(true);
        //用于增加/减少工具栏时适配窗口大小
    connect(tool_bar, &QAction::triggered, this, [=](){
        if(toolbar->isVisible()){
            toolbar->setVisible(false);
            QSize now_size = this->size();
            if(now_size.width()<1200){
                now_size.rwidth()-=100;
                this->resize(now_size);
            }
        }
        else{
            toolbar->setVisible(true);
            QSize now_size = this->size();
            if(now_size.width()<1200){
                now_size.rwidth()+=100;
                this->resize(now_size);
            }
        }
    });

    QAction * regret_action = edit_menu->addAction("Regret Button",QKeySequence(QLatin1String("Ctrl+Z")));
    QAction * color_action = edit_menu->addAction("Chessboard Color",QKeySequence(QLatin1String("Ctrl+C")));
    QAction * background_action = edit_menu->addAction("Change Background",QKeySequence(QLatin1String("Ctrl+B")));

    regret_action->setFont(font);
    color_action->setFont(font);
    background_action->setFont(font);

        //更换棋盘颜色
    connect(color_action, &QAction::triggered, this, [=](){
        QDialog* color_type = new QDialog(this);//创建在堆区，就不会一闪而过了
        QHBoxLayout* chessboard_layout = new QHBoxLayout();
        Lattice* type1 = new Lattice(0,0);
        Lattice* type2 = new Lattice(0,1);
        Lattice* type3 = new Lattice(0,2);
        type1->setMaximumSize(150,150);
        type2->setMaximumSize(150,150);
        type3->setMaximumSize(150,150);
        type1->setPixmap(QPixmap(":/source/pictures/Type1.png"));
        type2->setPixmap(QPixmap(":/source/pictures/Type2.png"));
        type3->setPixmap(QPixmap(":/source/pictures/Type3.png"));
        type1->setScaledContents(true);
        type2->setScaledContents(true);
        type3->setScaledContents(true);
        connect(type1,&Lattice::leftButtonClicked,this,[=](){
            chessboard->changeBoardType(1);
            board_color_type = 1;
            avatar1->setPixmap(PixmapToRound(":/source/pictures/White.png",avatar1));
            avatar2->setPixmap(PixmapToRound(":/source/pictures/Black.png",avatar2));
            holder1->setPixmap(QPixmap(":/source/pictures/PureWhite_KING.png"));
            holder2->setPixmap(QPixmap(":/source/pictures/PureBlack_KING.png"));
        });
        connect(type2,&Lattice::leftButtonClicked,this,[=](){
            chessboard->changeBoardType(2);
            board_color_type = 2;
            avatar1->setPixmap(PixmapToRound(":/source/pictures/Melon.png",avatar1));
            avatar2->setPixmap(PixmapToRound(":/source/pictures/Black.png",avatar2));
            holder1->setPixmap(QPixmap(":/source/pictures/BrownWhite_KING.png"));
            holder2->setPixmap(QPixmap(":/source/pictures/BrownBlack_KING.png"));
        });
        connect(type3,&Lattice::leftButtonClicked,this,[=](){
            chessboard->changeBoardType(3);
            board_color_type = 3;
            avatar1->setPixmap(PixmapToRound(":/source/pictures/Light_Wood.png",avatar1));
            avatar2->setPixmap(PixmapToRound(":/source/pictures/Black.png",avatar2));
            holder1->setPixmap(QPixmap(":/source/pictures/WoodWhite_KING.png"));
            holder2->setPixmap(QPixmap(":/source/pictures/WoodBlack_KING.png"));
        });
        chessboard_layout->addWidget(type1,1);
        chessboard_layout->addWidget(type2,1);
        chessboard_layout->addWidget(type3,1);
        color_type->setLayout(chessboard_layout);
        color_type->setWindowTitle("Color Type");
        //放在屏幕中央
        QScreen* screen = QGuiApplication::primaryScreen();
        QRect rect1 = screen->availableGeometry();
        color_type->move(rect1.size().width()/3,rect1.size().height()/3);
        color_type->setFixedSize(rect1.size().width()/3,rect1.size().height()/3);
        color_type->exec();
        color_type->setAttribute(Qt::WA_DeleteOnClose);//当关掉对话框时释放对象
    });
        //更换背景图片
    connect(background_action, &QAction::triggered, this, [=](){
        //选择文件
        QString file_path = QFileDialog::getOpenFileName(nullptr, "Select Image", "", "Images (*.png *.jpg *.jpeg)");
        if (file_path.isEmpty()){
            setBackgroundPixmap(background_pixmap);
        }
        else{
            setBackgroundPixmap(QPixmap(file_path));
        }
    });


    help_action = puzzle_menu->addAction("Help",QKeySequence(QLatin1String("Ctrl+H")));
    QDialog* help = help_list();
    help_action->setFont(font);
    connect(help_action, &QAction::triggered, this, [=](){
        QScreen* screen = QGuiApplication::primaryScreen();
        QRect rect1 = screen->availableGeometry();
        help->move(rect1.size().width()*3/10,rect1.size().height()/10);
        help->resize(rect1.size().width()*3/8,rect1.size().height()*4/5);
        help->show();
    });


    // 状态栏
    QStatusBar * status_bar = new QStatusBar();
    //设置到窗口中
    setStatusBar(status_bar);
    //放标签控件
        //选择棋子信息
    QLabel * select_message = new QLabel("Select :",this);
    QLabel* sel = new QLabel("None",this);
    QVector<QVector<Lattice*>> lat = chessboard->getLattice();
    for(int i = 0;i<8;++i){
        for(int j = 0;j<8;++j){
            connect(lat[i][j],&Lattice::leftButtonClicked,this,[=](){
                if(chessboard->getSelect()!=NULL){
                    QMetaEnum meta_team = QMetaEnum::fromType<ChessMan::CHESSTEAM>();
                    QMetaEnum meta_type = QMetaEnum::fromType<ChessMan::CHESSTYPE>();
                    QString chess_message = QString("");
                    chess_message.append(meta_team.valueToKey((ChessMan::CHESSTEAM)chessboard->getSelect()->getChessTeam()));
                    chess_message.append(" ");
                    chess_message.append(meta_type.valueToKey((ChessMan::CHESSTYPE)chessboard->getSelect()->getChessType()));
                    chess_message.append(" id:");
                    chess_message.append(QString::number(chessboard->getSelect()->getId()));
                    sel->setText(chess_message);
                }
                else{
                    sel->setText("None");
                }
            });
        }
    }
    status_bar->addWidget(select_message);
    select_message->setMinimumWidth(60);
    status_bar->addWidget(sel);
    sel->setMinimumWidth(120);
        //行动信息
    QLabel * move_message = new QLabel("Move Message :",this);
    QLabel* mov = new QLabel("None",this);
    status_bar->addPermanentWidget(move_message);
    move_message->setMinimumWidth(100);
    status_bar->addPermanentWidget(mov);
    mov->setMinimumWidth(100);
        //成功移动一步后的信息变化
    connect(chessboard,&ChessBoard::moveSuccess,this,[=](){
        if(chessboard->getRecord().size()>0){
            mov->setText(chessboard->getRecord().at(chessboard->getRecord().size()-1));

            movement_information->insertPlainText(chessboard->getRecord().at(chessboard->getRecord().size()-1));
            movement_information->moveCursor(QTextCursor::End);
            movement_information->append(QString(""));

            if(chessboard->getCurrentTeam()==ChessMan::WHITE){
                avatar1->setStyleSheet("border-width: 3px;border-style: solid;border-color: rgb(22, 237, 254)");
                avatar2->setStyleSheet("border-width: 0px");
            }
            else{
                avatar1->setStyleSheet("border-width: 0px");
                avatar2->setStyleSheet("border-width: 3px;border-style: solid;border-color: rgb(22, 237, 254)");
            }
        }
    });
    connect(regret_action,&QAction::triggered,this,[=](){
        if(chessboard->getRecord().size()>4){
            int step;
            if(chessboard->getCurrentTeam()==ChessMan::BLACK){
                step = step1->text().toInt();
            }
            else{
                step = step2->text().toInt();
            }
            if(step<5){
                QString regret_message = QString(chessboard->getRecord().at(chessboard->getRecord().size()-1));
                regret_message.append(" Regret!");
                mov->setText(regret_message);

                movement_information->insertPlainText(regret_message);
                movement_information->moveCursor(QTextCursor::End);
                movement_information->append(QString(""));

                if(chessboard->getCurrentTeam()==ChessMan::BLACK){
                    step1->setText(QString::number(++step));
                    if(step==5){
                        step1->setStyleSheet("color:red");
                    }
                    avatar1->setStyleSheet("border-width: 3px;border-style: solid;border-color: rgb(22, 237, 254)");
                    avatar2->setStyleSheet("border-width: 0px");
                }
                else{
                    step2->setText(QString::number(++step));
                    if(step==5){
                        step2->setStyleSheet("color:red");
                    }
                    avatar1->setStyleSheet("border-width: 0px");
                    avatar2->setStyleSheet("border-width: 3px;border-style: solid;border-color: rgb(22, 237, 254)");
                }
                emit backtrack();
            }
            else{
                QMessageBox::critical(this,"Regret Fault","Too many regret moves!");
            }
        }
    });
        //真正悔棋操作
    connect(this, &Game::backtrack, this, [=](){
        chessboard->regret();
    });
        //另外的悔棋按钮，询问
    connect(regret_button,&QPushButton::clicked,this,[=](){
        if(QMessageBox::Yes == QMessageBox::question(this,"Regret!","Do you really want to regret",QMessageBox::Yes|QMessageBox::No,QMessageBox::No)){
            regret_action->trigger();
        }
    });

        //规则另一个进入接口
    QPushButton * rule_at_status = new QPushButton("Help",this);
    rule_at_status->setStyleSheet(QString("QPushButton{border:none}"
                                          "QPushButton:hover{color:rgb(0,0,255)}"));
    status_bar->addPermanentWidget(rule_at_status);
    rule_at_status->setMinimumWidth(50);
    connect(rule_at_status,&QPushButton::clicked,this,[=](){
        QScreen* screen = QGuiApplication::primaryScreen();
        QRect rect1 = screen->availableGeometry();
        help->move(rect1.size().width()*3/10,rect1.size().height()/10);
        help->resize(rect1.size().width()*3/8,rect1.size().height()*4/5);
        help->show();
    });

    this->resize(970,640);
}

void Game::changeEvent(QEvent *event){
    static bool flag = false;
    if(QEvent::WindowStateChange == event->type()){
        QWindowStateChangeEvent * stateEvent = dynamic_cast<QWindowStateChangeEvent*>(event);
        if(Q_NULLPTR != stateEvent){
            Qt::WindowStates old_state = stateEvent->oldState();//变化前窗口状态
            Qt::WindowStates new_state = this->windowState();//当前窗口状态
            if(Qt::WindowNoState == old_state&&Qt::WindowMaximized==new_state){//之前是正常窗口
                if(toolbar->isVisible()){
                    flag = true;//打开了tool栏
                }
                else{
                    flag = false;//未打开tool栏
                }
            }
            else if(Qt::WindowMaximized == old_state&&Qt::WindowNoState==new_state){//之前是最大化
                if(toolbar->isVisible()&&!flag){
                    QSize now_size = this->size();
                    if(now_size.width()<900){
                        now_size.rwidth()+=100;
                        this->resize(now_size);
                    }
                }
                else if(!toolbar->isVisible()&&flag){
                    QSize now_size = this->size();
                    if(now_size.width()<900){
                        now_size.rwidth()-=100;
                        this->resize(now_size);
                    }
                }
            }
        }
    }
}

QPixmap Game::PixmapToRound(const QString &src, QLabel* target_label)//让图片变成圆形（用于头像）
{
    QPixmap pixmap(src);
    pixmap = pixmap.scaled(target_label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    int width = target_label->size().width();
    int height = target_label->size().height();

    QPixmap image(width,height);
    image.fill(Qt::transparent);

    QPainterPath painterPath;
    painterPath.addEllipse(0, 0, width, height);

    QPainter painter(&image);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setClipPath(painterPath);
    painter.drawPixmap(0, 0, width, height, pixmap);
    //绘制到label
    return image;
}

void Game::setBackgroundPixmap(const QPixmap& pixmap)
{
    background_pixmap = pixmap;
    update();
}

void Game::paintEvent(QPaintEvent* event)
{
    // 绘制背景图片
    QPainter painter(this);
    painter.drawPixmap(rect(), background_pixmap);

    // 调用基类的绘制函数
    QWidget::paintEvent(event);
}

void Game::resizeEvent(QResizeEvent* event)
{
    // 在窗口大小变化时重新设置背景图片
    QPixmap scaledPixmap = background_pixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    setBackgroundPixmap(scaledPixmap);

    // 调用基类的resizeEvent函数
    QWidget::resizeEvent(event);
}

void Game::newProject(QFile& file, Game* new_game){
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream input_stream(&file);

        while (!input_stream.atEnd()){
            QString move = input_stream.readLine();

            if(move=="attribute:"){
                break;
            }
            new_game->chessboard->recordMove(move);
        }

        QString attribute = input_stream.readLine();
        new_game->step1->setText(attribute);
        if(attribute=="5"){
            step1->setStyleSheet("color:red");
        }

        attribute = input_stream.readLine();
        new_game->step2->setText(attribute);
        if(attribute=="5"){
            step2->setStyleSheet("color:red");
        }

        attribute = input_stream.readLine();
        new_game->board_color_type=attribute.toInt();
        new_game->chessboard->changeBoardType(new_game->board_color_type);
        qDebug()<<new_game->board_color_type;
        if(new_game->board_color_type==1){
            new_game->avatar1->setPixmap(PixmapToRound(":/source/pictures/White.png",new_game->avatar1));
            new_game->avatar2->setPixmap(PixmapToRound(":/source/pictures/Black.png",new_game->avatar2));
            new_game->holder1->setPixmap(QPixmap(":/source/pictures/PureWhite_KING.png"));
            new_game->holder2->setPixmap(QPixmap(":/source/pictures/PureBlack_KING.png"));
        }
        else if(new_game->board_color_type==2){
            new_game->avatar1->setPixmap(PixmapToRound(":/source/pictures/Melon.png",new_game->avatar1));
            new_game->avatar2->setPixmap(PixmapToRound(":/source/pictures/Black.png",new_game->avatar2));
            new_game->holder1->setPixmap(QPixmap(":/source/pictures/BrownWhite_KING.png"));
            new_game->holder2->setPixmap(QPixmap(":/source/pictures/BrownBlack_KING.png"));
        }
        else{
            new_game->avatar1->setPixmap(PixmapToRound(":/source/pictures/Light_Wood.png",avatar1));
            new_game->avatar2->setPixmap(PixmapToRound(":/source/pictures/Black.png",avatar2));
            new_game->holder1->setPixmap(QPixmap(":/source/pictures/WoodWhite_KING.png"));
            new_game->holder2->setPixmap(QPixmap(":/source/pictures/WoodBlack_KING.png"));
        }

        this->close();
        new_game->show();
        file.close();
    }
    else{
        QMessageBox::critical(this,"Error","Save Error！");//模态的对话框
    }
}

void Game::basicElementInitial(QFont font,int type){
        //头像
    avatar1 = new QLabel(this);
    avatar1->setFixedSize(60,60);
    avatar1->setScaledContents(true);
    avatar1->setStyleSheet("border-width: 3px;border-style: solid;border-color: rgb(22, 237, 254)");
    avatar2 = new QLabel(this);
    avatar2->setFixedSize(60,60);
    avatar2->setScaledContents(true);
    avatar2->setStyleSheet("border-width: 0px");
        //持方
    holder1 = new QLabel(this);
    holder1->setFixedSize(60,60);
    holder1->setScaledContents(true);
    holder2 = new QLabel(this);
    holder2->setFixedSize(60,60);
    holder2->setScaledContents(true);
        //颜色初始化
    if(type==1){
        avatar1->setPixmap(PixmapToRound(":/source/pictures/White.png",avatar1));
        avatar2->setPixmap(PixmapToRound(":/source/pictures/Black.png",avatar2));
        holder1->setPixmap(QPixmap(":/source/pictures/PureWhite_KING.png"));
        holder2->setPixmap(QPixmap(":/source/pictures/PureBlack_KING.png"));
    }
    else if(type==2){
        avatar1->setPixmap(PixmapToRound(":/source/pictures/Melon.png",avatar1));
        avatar2->setPixmap(PixmapToRound(":/source/pictures/Black.png",avatar2));
        holder1->setPixmap(QPixmap(":/source/pictures/BrownWhite_KING.png"));
        holder2->setPixmap(QPixmap(":/source/pictures/BrownBlack_KING.png"));
    }
    else if(type==3){
        avatar1->setPixmap(PixmapToRound(":/source/pictures/Light_Wood.png",avatar1));
        avatar2->setPixmap(PixmapToRound(":/source/pictures/Black.png",avatar2));
        holder1->setPixmap(QPixmap(":/source/pictures/WoodWhite_KING.png"));
        holder2->setPixmap(QPixmap(":/source/pictures/WoodBlack_KING.png"));
    }
        //悔棋的步数
    step1 = new QLabel("0",this);
    step1->setAlignment(Qt::AlignCenter);
    step1->setFixedSize(60,30);
    step1->setFont(font);
    step2 = new QLabel("0",this);
    step2->setAlignment(Qt::AlignCenter);
    step2->setFixedSize(60,30);
    step2->setFont(font);
}

void Game::new_project(){
    QDialog* choose_project = new QDialog(this);//创建在堆区，就不会一闪而过了
    QGridLayout* project_layout = new QGridLayout();
    Lattice* project1 = new Lattice(0,0);
    Lattice* project2 = new Lattice(0,1);
    Lattice* project3 = new Lattice(0,2);
    Lattice* project4 = new Lattice(0,3);
    project1->setMaximumSize(300,300);
    project2->setMaximumSize(300,300);
    project3->setMaximumSize(300,300);
    project4->setMaximumSize(300,300);
    project1->setPixmap(QPixmap("./saved_game/game1.png"));
    project2->setPixmap(QPixmap("./saved_game/game2.png"));
    project3->setPixmap(QPixmap("./saved_game/game3.png"));
    project4->setPixmap(QPixmap("./saved_game/game4.png"));
    project1->setScaledContents(true);
    project2->setScaledContents(true);
    project3->setScaledContents(true);
    project4->setScaledContents(true);
    connect(project1,&Lattice::leftButtonClicked,this,[=](){
        save_project();
            //清空记录
        QFile file("./saved_game/game1.txt");
        file.open(QFile::WriteOnly|QFile::Truncate);
        file.close();
        null_pixmap.save("./saved_game/game1.png");

        Game* new_game = new Game(1,1);
        newProject(file,new_game);
        new_game->saved_project = 1;

        this->close();
        new_game->show();
        choose_project->close();
    });
    connect(project2,&Lattice::leftButtonClicked,this,[=](){
        save_project();
            //清空记录
        QFile file("./saved_game/game2.txt");
        file.open(QFile::WriteOnly|QFile::Truncate);
        file.close();
        null_pixmap.save("./saved_game/game2.png");

        Game* new_game = new Game(1,1);
        newProject(file,new_game);
        new_game->saved_project = 2;

        this->close();
        new_game->show();
        choose_project->close();
    });
    connect(project3,&Lattice::leftButtonClicked,this,[=](){
        save_project();
            //清空记录
        QFile file("./saved_game/game3.txt");
        file.open(QFile::WriteOnly|QFile::Truncate);
        file.close();
        null_pixmap.save("./saved_game/game3.png");

        Game* new_game = new Game(1,1);
        newProject(file,new_game);
        new_game->saved_project = 3;

        this->close();
        new_game->show();
        choose_project->close();
    });
    connect(project4,&Lattice::leftButtonClicked,this,[=](){
        save_project();
            //清空记录
        QFile file("./saved_game/game4.txt");
        file.open(QFile::WriteOnly|QFile::Truncate);
        file.close();
        null_pixmap.save("./saved_game/game4.png");

        Game* new_game = new Game(1,1);
        newProject(file,new_game);
        new_game->saved_project = 4;

        this->close();
        new_game->show();
        choose_project->close();
    });
    project_layout->addWidget(project1,0,0);
    project_layout->addWidget(project2,0,1);
    project_layout->addWidget(project3,1,0);
    project_layout->addWidget(project4,1,1);
    choose_project->setLayout(project_layout);
    choose_project->setWindowTitle("Project Choose");
    //放在屏幕中央
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect rect1 = screen->availableGeometry();
    choose_project->move(rect1.size().width()/4,rect1.size().height()/6);
    choose_project->setFixedSize(rect1.size().width()/2,rect1.size().height()*2/3);
    choose_project->exec();
    choose_project->setAttribute(Qt::WA_DeleteOnClose);//当关掉对话框时释放对象
}

void Game::open_project(){
    QDialog* choose_project = new QDialog(this);//创建在堆区，就不会一闪而过了
    QGridLayout* project_layout = new QGridLayout();
    Lattice* project1 = new Lattice(0,0);
    Lattice* project2 = new Lattice(0,1);
    Lattice* project3 = new Lattice(0,2);
    Lattice* project4 = new Lattice(0,3);
    project1->setMaximumSize(300,300);
    project2->setMaximumSize(300,300);
    project3->setMaximumSize(300,300);
    project4->setMaximumSize(300,300);
    project1->setPixmap(QPixmap("./saved_game/game1.png"));
    project2->setPixmap(QPixmap("./saved_game/game2.png"));
    project3->setPixmap(QPixmap("./saved_game/game3.png"));
    project4->setPixmap(QPixmap("./saved_game/game4.png"));
    project1->setScaledContents(true);
    project2->setScaledContents(true);
    project3->setScaledContents(true);
    project4->setScaledContents(true);
    connect(project1,&Lattice::leftButtonClicked,this,[=](){
        save_project();

        QFile file("./saved_game/game1.txt");
        Game* new_game = new Game(1,1);
        newProject(file,new_game);
        new_game->saved_project = 1;

        this->close();
        new_game->show();
        choose_project->close();
    });
    connect(project2,&Lattice::leftButtonClicked,this,[=](){
        save_project();

        QFile file("./saved_game/game2.txt");
        Game* new_game = new Game(1,1);
        newProject(file,new_game);
        new_game->saved_project = 2;

        this->close();
        new_game->show();
        choose_project->close();
    });
    connect(project3,&Lattice::leftButtonClicked,this,[=](){
        save_project();

        QFile file("./saved_game/game3.txt");
        Game* new_game = new Game(1,1);
        newProject(file,new_game);
        new_game->saved_project = 3;

        this->close();
        new_game->show();
        choose_project->close();
    });
    connect(project4,&Lattice::leftButtonClicked,this,[=](){
        save_project();

        QFile file("./saved_game/game4.txt");
        Game* new_game = new Game(1,1);
        newProject(file,new_game);
        new_game->saved_project = 4;

        this->close();
        new_game->show();
        choose_project->close();
    });
    project_layout->addWidget(project1,0,0);
    project_layout->addWidget(project2,0,1);
    project_layout->addWidget(project3,1,0);
    project_layout->addWidget(project4,1,1);
    choose_project->setLayout(project_layout);
    choose_project->setWindowTitle("Project Choose");
    //放在屏幕中央
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect rect1 = screen->availableGeometry();
    choose_project->move(rect1.size().width()/4,rect1.size().height()/6);
    choose_project->setFixedSize(rect1.size().width()/2,rect1.size().height()*2/3);
    choose_project->exec();
    choose_project->setAttribute(Qt::WA_DeleteOnClose);//当关掉对话框时释放对象
}

void Game::save_project(){
    QWidget *widget = chessboard; //截取棋盘当前状态
    QPixmap pixmap = widget->grab(QRect(QPoint(0, 0), widget->size()));//截取保存为图片

    if(saved_project==1){
        QFile file("./saved_game/game1.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream output_stream(&file);

            QVector<QString> record = chessboard->getRecord();
            //写入文件内容
            for(int i=0;i<record.size();++i){
                output_stream << record[i] << "\n";
            }

            output_stream<<"attribute:\n"<<step1->text().toInt()<<"\n"<<step2->text().toInt()<<"\n"
                        <<board_color_type<<"\n";

            file.close();
        }
        else{
            QMessageBox::critical(this,"Error","Save Error！");//模态的对话框
        }
        pixmap.save("./saved_game/game1.png"); //保存到对应文件夹
    }
    else if(saved_project==2){
        QFile file("./saved_game/game2.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream output_stream(&file);

            QVector<QString> record = chessboard->getRecord();
            //写入文件内容
            for(int i=0;i<record.size();++i){
                output_stream << record[i] << "\n";
            }

            output_stream<<"attribute:\n"<<step1->text().toInt()<<"\n"<<step2->text().toInt()<<"\n"
                        <<board_color_type<<"\n";

            file.close();
        }
        else{
            QMessageBox::critical(this,"Error","Save Error！");//模态的对话框
        }
        pixmap.save("./saved_game/game2.png");
    }
    else if(saved_project==3){
        QFile file("./saved_game/game3.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream output_stream(&file);

            QVector<QString> record = chessboard->getRecord();
            //写入文件内容
            for(int i=0;i<record.size();++i){
                output_stream << record[i] << "\n";
            }

            output_stream<<"attribute:\n"<<step1->text().toInt()<<"\n"<<step2->text().toInt()<<"\n"
                        <<board_color_type<<"\n";

            file.close();
        }
        else{
            QMessageBox::critical(this,"Error","Save Error！");//模态的对话框
        }
        pixmap.save("./saved_game/game3.png");
    }
    else if(saved_project==4){
        QFile file("./saved_game/game4.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream output_stream(&file);

            QVector<QString> record = chessboard->getRecord();
            //写入文件内容
            for(int i=0;i<record.size();++i){
                output_stream << record[i] << "\n";
            }

            output_stream<<"attribute:\n"<<step1->text().toInt()<<"\n"<<step2->text().toInt()<<"\n"
                        <<board_color_type<<"\n";

            file.close();
        }
        else{
            QMessageBox::critical(this,"Error","Save Error！");//模态的对话框
        }
        pixmap.save("./saved_game/game4.png");
    }

}

QDialog* Game::help_list(){
    // 规则页面的创建
    QDialog* help = new QDialog(this);//创建在堆区，就不会一闪而过了
    QHBoxLayout* help_layout = new QHBoxLayout();
    help->setLayout(help_layout);
    help->setWindowTitle("Rule");
    //设置水平的头——棋子、移动规则
    QTreeWidget* help_tree = new QTreeWidget(this);
    help_tree->setHeaderLabels(QStringList()<<"Chess"<<"Move Rule");//QStingList这里用了匿名对象
    help_tree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    help_tree->header()->setStretchLastSection(false);
    //创建头节点——普通规则、吃子行为、特殊规则、记录方式
    QTreeWidgetItem * base_item = new QTreeWidgetItem(QStringList()<<"Base Move");
    QTreeWidgetItem * eat_item = new QTreeWidgetItem(QStringList()<<"Eat Move");
    QTreeWidgetItem * special_item = new QTreeWidgetItem(QStringList()<<"Special Move");
    QTreeWidgetItem * record_item = new QTreeWidgetItem(QStringList()<<"Record");
    //加载顶层结点
    help_tree->addTopLevelItem(base_item);
    help_tree->addTopLevelItem(eat_item);
    help_tree->addTopLevelItem(special_item);
    help_tree->addTopLevelItem(record_item);
    //追加子结点
        //普通规则
    QStringList b_king,b_queen,b_bishop,b_knight,b_rook,b_pawn;
    b_king<<"KING"<<"横、直、斜都可以走，但每次限走一步；任何被敌方控制的格子，己方王都不能走进去。否则，算“送王”犯规。累计三次犯规就要判负。";
    b_queen<<"QUEEN"<<"横、直、斜都可以走，步数不受限制，不能越子行棋";
    b_bishop<<"BISHOP"<<"只能斜走，格数不限，不能越子行棋";
    b_knight<<"KNIGHT"<<"每行一步棋，先横走或直走一格，然后再往外斜走一格；或者先斜走一格，最后再往外横走或竖走一格（即“日”字）";
    b_rook<<"ROOK"<<"横、竖均可以走，步数不受限制，不能斜走，不能越子行棋";
    b_pawn<<"PAWN"<<"只能向前直走，每次行棋只能走一格。兵走第一步时，可以走一格或走两格";
    QTreeWidgetItem * king = new QTreeWidgetItem(b_king);
    QTreeWidgetItem * queen = new QTreeWidgetItem(b_queen);
    QTreeWidgetItem * bishop = new QTreeWidgetItem(b_bishop);
    QTreeWidgetItem * knight = new QTreeWidgetItem(b_knight);
    QTreeWidgetItem * rook = new QTreeWidgetItem(b_rook);
    QTreeWidgetItem * pawn = new QTreeWidgetItem(b_pawn);
    base_item->addChild(king);
    base_item->addChild(queen);
    base_item->addChild(bishop);
    base_item->addChild(knight);
    base_item->addChild(rook);
    base_item->addChild(pawn);
        //吃子行动
    QStringList b_eat1,b_eat2;
    b_eat1<<"兵"<<"兵的吃子方法与行棋方向不一样，它是直走斜吃，即如果兵的前斜进一格内有对方棋子，就可以吃掉它，从而占据该格位置";
    b_eat2<<"除了兵之外的棋子"<<"除了兵之外的棋子，凡是行棋能走到的地方，而这个格子上有对方的棋子，就能吃掉对方的棋子";
    QTreeWidgetItem * eat1 = new QTreeWidgetItem(b_eat1);
    QTreeWidgetItem * eat2 = new QTreeWidgetItem(b_eat2);
    eat_item->addChild(eat1);
    eat_item->addChild(eat2);
        //特殊规则
    QStringList s_en_passant,s_cast,s_promote;
    s_en_passant<<"吃过路兵"<<"如果对方的兵第一次行棋且直进两格，刚好形成本方有兵与其横向紧贴并列，则本方的兵可以立即斜进，把对方的兵吃掉，并视为一步棋。这个动作必须立刻进行，缓着后无效";
    s_cast<<"车王易位"<<"王和车都没走过的前提下（两棋在同一条横线中），互换的行棋途中或起点和目的地，王都不会受到对方棋子攻击，即可进行一次车王互换位置";
    s_promote<<"兵的升变"<<"本方任何一个兵直进达到对方底线时，即可升变为除“王”和“兵”以外的任何一种棋子，可升变为“后”、“车”、“马”、“象”，不能不变。这被视为一步棋。兵升变之后，按新棋子的规则走棋";
    QTreeWidgetItem * en_passant = new QTreeWidgetItem(s_en_passant);
    QTreeWidgetItem * cast = new QTreeWidgetItem(s_cast);
    QTreeWidgetItem * promote = new QTreeWidgetItem(s_promote);
    special_item->addChild(en_passant);
    special_item->addChild(cast);
    special_item->addChild(promote);
        //记录方式
    QStringList r_board,r_king,r_queen,r_bishop,r_knight,r_rook,r_pawn,
            r_move,r_eat,r_check,r_double_check,r_mate,
            r_short_cast,r_long_cast,r_rise,r_bwin,r_wwin,r_draw;
    r_board<<"Board Tag"<<"从左到右为a-h列，从下到上为1-8行";
    r_king<<"KING"<<"记号为K";
    r_queen<<"QUEEN"<<"记号为Q";
    r_bishop<<"BISHOP"<<"记号为B";
    r_knight<<"KNIGHT"<<"记号为N";
    r_rook<<"ROOK"<<"记号为R";
    r_pawn<<"PAWN"<<"记号为该列小写字母";
    r_move<<"Move"<<"记号为-";
    r_eat<<"Eat"<<"记号为x";
    r_check<<"Check"<<"记号为+";
    r_double_check<<"Double Check"<<"记号为++";
    r_mate<<"Check Mate"<<"记号为#";
    r_short_cast<<"Short Castling"<<"记号为O-O";
    r_long_cast<<"Long Castling"<<"记号为O-O-O";
    r_rise<<"Rising Of PAWN"<<"记号为=";
    r_bwin<<"Black Win"<<"记号为0-1";
    r_wwin<<"White Win"<<"记号为1-0";
    r_draw<<"Draw"<<"记号为1/2-1/2";
    QTreeWidgetItem * rboa = new QTreeWidgetItem(r_board);
    QTreeWidgetItem * rkin = new QTreeWidgetItem(r_king);
    QTreeWidgetItem * rque = new QTreeWidgetItem(r_queen);
    QTreeWidgetItem * rbis = new QTreeWidgetItem(r_bishop);
    QTreeWidgetItem * rkni = new QTreeWidgetItem(r_knight);
    QTreeWidgetItem * rroo = new QTreeWidgetItem(r_rook);
    QTreeWidgetItem * rpaw = new QTreeWidgetItem(r_pawn);
    QTreeWidgetItem * rmov = new QTreeWidgetItem(r_move);
    QTreeWidgetItem * reat = new QTreeWidgetItem(r_eat);
    QTreeWidgetItem * rche = new QTreeWidgetItem(r_check);
    QTreeWidgetItem * rdou = new QTreeWidgetItem(r_double_check);
    QTreeWidgetItem * rmat = new QTreeWidgetItem(r_mate);
    QTreeWidgetItem * rsho = new QTreeWidgetItem(r_short_cast);
    QTreeWidgetItem * rlon = new QTreeWidgetItem(r_long_cast);
    QTreeWidgetItem * rris = new QTreeWidgetItem(r_rise);
    QTreeWidgetItem * rbwi = new QTreeWidgetItem(r_bwin);
    QTreeWidgetItem * rwwi = new QTreeWidgetItem(r_wwin);
    QTreeWidgetItem * rdra = new QTreeWidgetItem(r_draw);
    record_item->addChild(rboa);
    record_item->addChild(rkin);
    record_item->addChild(rque);
    record_item->addChild(rbis);
    record_item->addChild(rkni);
    record_item->addChild(rroo);
    record_item->addChild(rpaw);
    record_item->addChild(rmov);
    record_item->addChild(reat);
    record_item->addChild(rche);
    record_item->addChild(rdou);
    record_item->addChild(rmat);
    record_item->addChild(rsho);
    record_item->addChild(rlon);
    record_item->addChild(rris);
    record_item->addChild(rbwi);
    record_item->addChild(rwwi);
    record_item->addChild(rdra);

    help_tree->expandAll();//默认将所有项目展开
    help_layout->addWidget(help_tree);
    return help;
}
