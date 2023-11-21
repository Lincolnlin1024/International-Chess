#include "latice.h"
#include <QMouseEvent>
#include <QDebug>

Latice::Latice(QWidget *parent)
    : QLabel{parent}
{
    //默认在0，0处
    pos_row=0;
    pos_col=0;
    //设置棋盘格子允许的大小范围
    this->setMinimumSize(40,40);
    this->setMaximumSize(90,90);

    //设置棋盘格子的伸缩规则
    QSizePolicy sizePolicy = this->sizePolicy();
    sizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);//水平策略尽可能伸展
    sizePolicy.setVerticalPolicy(QSizePolicy::Expanding);//垂直策略尽可能伸展
    sizePolicy.setHorizontalStretch(0);//水平伸展为0
    sizePolicy.setVerticalStretch(0);//垂直伸展为0
    sizePolicy.setHeightForWidth(true);//设置建议尺寸高度与宽度相关
    this->setAlignment(Qt::AlignCenter);
    this->setSizePolicy(sizePolicy);

}

Latice::Latice(int row,int col,QWidget *parent)
{
    //设置初值和父亲
    pos_row = row;
    pos_col = col;
    this->setParent(parent);
    //设置棋盘格子允许的大小范围
    this->setMinimumSize(40,40);
    this->setMaximumSize(90,90);

    //设置棋盘格子的伸缩规则
    QSizePolicy sizePolicy = this->sizePolicy();
    sizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);//水平策略尽可能伸展
    sizePolicy.setVerticalPolicy(QSizePolicy::Expanding);//垂直策略尽可能伸展
    sizePolicy.setHorizontalStretch(0);//水平伸展为0
    sizePolicy.setVerticalStretch(0);//垂直伸展为0
    sizePolicy.setHeightForWidth(true);//设置建议尺寸高度与宽度相关
    this->setAlignment(Qt::AlignCenter);
    this->setSizePolicy(sizePolicy);
}

Latice::Latice(Latice& latice){
    this->pos_row=latice.getPos_row();
    this->pos_col=latice.getPos_col();
    this->setMinimumSize(latice.minimumSize());
    this->setMaximumSize(latice.maximumSize());
    this->setAlignment(latice.alignment());
    this->setSizePolicy(latice.sizePolicy());
}

Latice::~Latice(){

}

Latice& Latice::operator=(Latice& latice){
    this->pos_row=latice.getPos_row();
    this->pos_col=latice.getPos_col();
    this->setMinimumSize(latice.minimumSize());
    this->setMaximumSize(latice.maximumSize());
    this->setAlignment(latice.alignment());
    this->setSizePolicy(latice.sizePolicy());
    return *this;
}

int Latice::getPos_row(){
    return pos_row;
}

int Latice::getPos_col(){
    return pos_col;
}

void Latice::setPos_row(int row){
    pos_row = row;
}

void Latice::setPos_col(int col){
    pos_col = col;
}

void Latice::mousePressEvent(QMouseEvent* event){
    QPointF clicked_position(pos_row,pos_col);

    //判断是鼠标哪个按键被按下
    if(event->button()==Qt::LeftButton){
        emit leftButtonClicked(clicked_position);
    }
    else if(event->button()==Qt::RightButton){
        emit rightButtonClicked(clicked_position);
    }
}

void Latice::enterEvent(QEnterEvent* event){
    //使用Q_UNUSED让编译器不报告event未被引用的警告
    Q_UNUSED(event);

    //设置当鼠标移入该格子对象时的格子变化
    //使用Box边框
    this->setFrameShape(QFrame::Box);
    //将边框宽度设置为2个像素点，边框风格用实线，颜色用rgb(255，270，0)，得到边框高亮的效果
    this->setStyleSheet("border-width: 2px;border-style: solid;border-color: rgb(255,170,0);");
}

void Latice::leaveEvent(QEvent* event){
    //使用Q_UNUSED让编译器不报告event未被引用的警告
    Q_UNUSED(event);

    //设置当鼠标移出该格子对象时的格子变化
    //恢复格子默认边框（无边框）
    this->setFrameShape(QFrame::NoFrame);
    //直接将边框宽度设置为0即可使之没有边框高亮
    this->setStyleSheet("border-width: 0px");
}
