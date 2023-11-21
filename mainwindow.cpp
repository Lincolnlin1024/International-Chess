#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setBackgroundPixmap(QPixmap(":/source/pictures/CoolBackground.png"));

    QHBoxLayout* layout = new QHBoxLayout(this);
    QVBoxLayout* choice_button = new QVBoxLayout();
    QPushButton * two_players = new QPushButton("Two Players",this);
    QPushButton * history = new QPushButton("History",this);
    QPushButton * exit = new QPushButton("Exit",this);
    two_players->setFixedSize(280,80);
    history->setFixedSize(280,80);
    exit->setFixedSize(280,80);

    QFont font;
    //设置文字字体
    font.setFamily("Microsoft Yahei");
    //设置文字大小为50像素
    font.setPixelSize(100);
    //设置文字大小
    font.setPointSize(25);

    //设置文字倾斜
    font.setStyle(QFont::StyleItalic);
    //设置文字加粗
    font.setWeight(QFont::Bold);
    //设置字间距像素值
    font.setLetterSpacing(QFont::AbsoluteSpacing,3);
    //设置全部字母大写
    font.setCapitalization(QFont::AllUppercase);

    //设置字体
    two_players->setFont(font);
    history->setFont(font);
    exit->setFont(font);
    //设置边框背景等

    two_players->setStyleSheet(QString("QPushButton{border:5px ridge rgb(83, 83, 255);"
                                       "border-radius:15px;"
                                       "color:rgb(74, 204, 201);"
                                       "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
                                       "stop:0 rgb(117, 120, 232), stop:0.4 rgb(182, 123, 225), stop:1 rgb(109, 208, 214));}"
        "QPushButton:hover{color:rgb(194,142,120)}"
        "QPushButton:pressed{background-color:rgb(168 , 234 , 216);"
                                        "padding-left:3px;"
                                        "padding-top:3px}"));

    history->setStyleSheet(QString("QPushButton{border:5px ridge rgb(83, 83, 255);"
                                       "border-radius:15px;"
                                       "color:rgb(74, 204, 201);"
                                       "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
                                       "stop:0 rgb(117, 120, 232), stop:0.4 rgb(182, 123, 225), stop:1 rgb(109, 208, 214));}"
        "QPushButton:hover{color:rgb(194,142,120)}"
        "QPushButton:pressed{background-color:rgb(168 , 234 , 216);"
                                        "padding-left:3px;"
                                        "padding-top:3px}"));

    exit->setStyleSheet(QString("QPushButton{border:5px ridge rgb(83, 83, 255);"
                                "border-radius:15px;"
                                "color:rgb(74, 204, 201);"
                                "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
                                "stop:0 rgb(117, 120, 232), stop:0.4 rgb(182, 123, 225), stop:1 rgb(109, 208, 214));}"
        "QPushButton:hover{color:rgb(194,142,120)}"
        "QPushButton:pressed{background-color:rgb(168 , 234 , 216);"
                                "padding-left:3px;"
                                "padding-top:3px}"));

    choice_button->addSpacerItem(new QSpacerItem(160,0,QSizePolicy::Fixed,QSizePolicy::Expanding));
    choice_button->addWidget(two_players);
    choice_button->addWidget(history);
    choice_button->addWidget(exit);
    choice_button->addSpacerItem(new QSpacerItem(160,0,QSizePolicy::Fixed,QSizePolicy::Expanding));
    choice_button->setSpacing(100);

    layout->addSpacerItem(new QSpacerItem(0,160,QSizePolicy::Expanding,QSizePolicy::Fixed));
    layout->addLayout(choice_button);
    layout->addSpacerItem(new QSpacerItem(0,160,QSizePolicy::Expanding,QSizePolicy::Fixed));
    layout->setSpacing(0);

    QString folderPath = "./record";  //文件夹路径
    QDir folder(folderPath);
    if (folder.exists()) {
        folder.setFilter(QDir::Files | QDir::NoDotAndDotDot);  //设置过滤器，仅获取文件，排除 "." 和 ".."
        QStringList file_list = folder.entryList();  //获取文件列表
        files = new QListWidget();
        files->addItem("help");
        //添加文件列表
        for (const QString &fileName : file_list) {
            files->addItem(fileName);
        }
        connect(files, &QListWidget::itemDoubleClicked, this, &MainWindow::handleItemDoubleClick);
    }
    else {
        qDebug() << "Folder does not exist.";
    }


    connect(two_players,&QPushButton::released,this,[=](){
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
            if(QPixmap("./saved_game/game1.png").toImage()==QPixmap(":/source/pictures/White.png").toImage()){//新游戏
                int type = 1;
                colorChoose(type);
                Game* g = new Game(type,1);
                g->show();
                choose_project->close();
            }
            else{//打开游戏
                Game* g = new Game(4,1);
                g->show();
                choose_project->close();
            }
            this->close();
        });
        connect(project2,&Lattice::leftButtonClicked,this,[=](){
            if(QPixmap("./saved_game/game2.png").toImage()==QPixmap(":/source/pictures/White.png").toImage()){//新游戏
                int type = 1;
                colorChoose(type);
                Game* g = new Game(type,2);
                g->show();
                choose_project->close();
            }
            else{//打开游戏
                Game* g = new Game(4,2);
                g->show();
                choose_project->close();
            }
            this->close();
        });
        connect(project3,&Lattice::leftButtonClicked,this,[=](){
            if(QPixmap("./saved_game/game3.png").toImage()==QPixmap(":/source/pictures/White.png").toImage()){//新游戏
                int type = 1;
                colorChoose(type);
                Game* g = new Game(type,3);
                g->show();
                choose_project->close();
            }
            else{//打开游戏
                Game* g = new Game(4,3);
                g->show();
                choose_project->close();
            }
            this->close();
        });
        connect(project4,&Lattice::leftButtonClicked,this,[=](){
            if(QPixmap("./saved_game/game4.png").toImage()==QPixmap(":/source/pictures/White.png").toImage()){//新游戏
                int type = 1;
                colorChoose(type);
                Game* g = new Game(type,4);
                g->show();
                choose_project->close();
            }
            else{//打开游戏
                Game* g = new Game(4,4);
                g->show();
                choose_project->close();
            }
            this->close();
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
    });
    connect(history,&QPushButton::released,this,[=](){
        QScreen* screen = QGuiApplication::primaryScreen();
        QRect rect1 = screen->availableGeometry();
        files->move(rect1.size().width()*5/8,rect1.size().height()/6);
        files->resize(rect1.size().width()/4,rect1.size().height()*3/5);
        files->show();
    });
    connect(exit,&QPushButton::released,this,[=](){
        this->close();
    });

    this->resize(500,600);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setBackgroundPixmap(const QPixmap& pixmap)
{
    background_pixmap = pixmap;
    update();
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    // 绘制背景图片
    QPainter painter(this);
    painter.drawPixmap(rect(), background_pixmap);

    // 调用基类的绘制函数
    QWidget::paintEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    // 在窗口大小变化时重新设置背景图片
    QPixmap scaledPixmap = background_pixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    setBackgroundPixmap(scaledPixmap);

    // 调用基类的resizeEvent函数
    QWidget::resizeEvent(event);
}

void MainWindow::colorChoose(int& choose_type){
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
    connect(type1,&Lattice::leftButtonClicked,this,[&]()mutable{
        choose_type = 1;
        color_type->close();
    });
    connect(type2,&Lattice::leftButtonClicked,this,[&]()mutable{
        choose_type = 2;
        color_type->close();
    });
    connect(type3,&Lattice::leftButtonClicked,this,[&]()mutable{
        choose_type = 3;
        color_type->close();
    });
    chessboard_layout->addWidget(type1,1);
    chessboard_layout->addWidget(type2,1);
    chessboard_layout->addWidget(type3,1);
    color_type->setLayout(chessboard_layout);
    color_type->setWindowTitle("Color Type");
    //放在屏幕中央
    QScreen* screen1 = QGuiApplication::primaryScreen();
    QRect rect11 = screen1->availableGeometry();
    color_type->move(rect11.size().width()/3,rect11.size().height()/3);
    color_type->setFixedSize(rect11.size().width()/3,rect11.size().height()/3);
    color_type->exec();
    color_type->setAttribute(Qt::WA_DeleteOnClose);//当关掉对话框时释放对象
}

void MainWindow::handleItemDoubleClick(QListWidgetItem *item) {
    if(item->text()=="help"){
        Game* history_game = new Game(1,1);
        QDialog* help = history_game->help_list();
        QScreen* screen = QGuiApplication::primaryScreen();
        QRect rect1 = screen->availableGeometry();
        help->move(rect1.size().width()/10,rect1.size().height()/10);
        help->resize(rect1.size().width()*3/8,rect1.size().height()*4/5);
        help->show();
    }
    else{
        QString file_name = "./record/";
        file_name.append(item->text());
        QFile file(file_name);
        QListWidget * file_context = new QListWidget();

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream input_stream(&file);

            while (!input_stream.atEnd()){
                QString context = input_stream.readLine();

                if(context=="attribute:"){
                    break;
                }
                else{
                    file_context->addItem(context);
                }
            }
        }
        file.close();

        QScreen* screen = QGuiApplication::primaryScreen();
        QRect rect1 = screen->availableGeometry();
        file_context->move(rect1.size().width()*5/8,rect1.size().height()/6);
        file_context->resize(rect1.size().width()/4,rect1.size().height()*3/5);
        file_context->show();
    }
}
