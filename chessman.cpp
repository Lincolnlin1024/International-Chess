#include "chessman.h"
#include <QMouseEvent>
#include <QApplication>

int ChessMan::CHESS_ID = 1;

ChessMan::ChessMan(QWidget *parent)
    : QLabel{parent}
{
    id = CHESS_ID;
    this->pos_row=-1;
    this->pos_col=-1;
    this->chesstype=PAWN;
    this->chessteam=BLACK;
    this->movecount = 0;
    this->isdead = false;
    //设置棋子允许的大小范围与格子一致
    this->setMinimumSize(40,40);
    this->setMaximumSize(90,90);
}

ChessMan::ChessMan(ChessMan& chessman){
    id = chessman.getId();
    this->pos_row=chessman.getPos_row();
    this->pos_col=chessman.getPos_col();
    this->movecount = chessman.movecount;
    this->chesstype = chessman.chesstype;
    this->chessteam = chessman.chessteam;
    this->isdead = chessman.isdead;
    this->chessPic = chessman.chessPic;
    this->setAlignment(chessman.alignment());
    this->setScaledContents(chessman.hasScaledContents());
    this->setPixmap(QPixmap(chessPic));
    //设置棋子允许的大小范围与格子一致
    this->setMinimumSize(40,40);
    this->setMaximumSize(90,90);
}

ChessMan::ChessMan(int type,int team){
    id=CHESS_ID;
    this->pos_row=-1;
    this->pos_col=-1;
    this->chesstype=type;
    this->chessteam=team;
    this->movecount = 0;
    this->isdead = false;
    //设置棋子允许的大小范围与格子一致
    this->setMinimumSize(40,40);
    this->setMaximumSize(90,90);
}

ChessMan::ChessMan(int type,int team,int row,int col){
    id=CHESS_ID;
    this->pos_row=row;
    this->pos_col=col;
    this->chesstype=type;
    this->chessteam=team;
    this->movecount = 0;
    this->isdead = false;
    //设置棋子允许的大小范围与格子一致
    this->setMinimumSize(40,40);
    this->setMaximumSize(90,90);
}

ChessMan& ChessMan::operator=(ChessMan& chessman){
    id=chessman.getId();
    this->pos_row=chessman.getPos_row();
    this->pos_col=chessman.getPos_col();
    this->movecount = chessman.movecount;
    this->chesstype = chessman.chesstype;
    this->chessteam = chessman.chessteam;
    this->isdead = chessman.isdead;
    this->chessPic = chessman.chessPic;
    this->setAlignment(chessman.alignment());
    this->setScaledContents(chessman.hasScaledContents());
    this->setPixmap(QPixmap(chessPic));
    //设置棋子允许的大小范围与格子一致
    this->setMinimumSize(40,40);
    this->setMaximumSize(90,90);
    return *this;
}

int ChessMan::getId(){
    return id;
}

int ChessMan::getPos_row(){
    return pos_row;
}

int ChessMan::getPos_col(){
    return pos_col;
}

int ChessMan::getMoveCount(){
    return movecount;
}

int ChessMan::getChessType(){
    return chesstype;
}

int ChessMan::getChessTeam(){
    return chessteam;
}

bool ChessMan::isDead(){
    return isdead;
}

void ChessMan::setPos_row(int row){
    pos_row = row;
}

void ChessMan::setPos_col(int col){
    pos_col = col;
}

void ChessMan::setPosition(int row,int col){
    pos_row = row;
    pos_col = col;
}

void ChessMan::setIsDead(bool val){
    isdead = val;

    if(val){
        this->setParent(NULL);
    }

}

void ChessMan::setChessType(int type){
    chesstype = type;
}

void ChessMan::setChessPic(QString pic){
    chessPic = pic;
}

void ChessMan::hasMove(){
    movecount++;
}

void ChessMan::regretMove(){
    movecount--;
}

void ChessMan::mousePressEvent(QMouseEvent* event){

    //将事件传递给底下的格子对象
    PostMouseEventToLattice(event);
}

void ChessMan::enterEvent(QEnterEvent* event){
    //使用Q_UNUSED让编译器不报告event未被引用的警告
    Q_UNUSED(event);

    //当鼠标移到该棋子上时变成鼠标指针样式
    setCursor(Qt::PointingHandCursor);

    //将事件传递给底下的格子对象
    PostEnterEventToLattice(event);
}

void ChessMan::leaveEvent(QEvent* event){
    //使用Q_UNUSED让编译器不报告event未被引用的警告
    Q_UNUSED(event);

    //当鼠标移开该棋子后变回正常样式
    setCursor(Qt::ArrowCursor);

    //将事件传递给底下的格子对象
    PostLeaveEventToLattice(event);
}

void ChessMan::PostMouseEventToLattice(QMouseEvent* event){

    if(this->parentWidget()) {
        this->setAttribute(Qt::WA_TransparentForMouseEvents, true);//打开事件透明（当前控件不接收事件，让其覆盖的控件接收？）
        QPoint point = this->mapTo(this->parentWidget(), event->pos());//计算事件发生的坐标
        QWidget *lat = this->parentWidget()->childAt(point);//找出覆盖对象（需要接收事件的对象）
        if (lat) {
            point = lat->mapFrom(this->parentWidget(), point);
            QMouseEvent *trans_event = new QMouseEvent(event->type(), point, event->button(),event->buttons(), event->modifiers());
            QApplication::postEvent(lat, trans_event);//传递事件给目标对象
        }
        this->setAttribute(Qt::WA_TransparentForMouseEvents, false);//关闭事件透明
    }
}

void ChessMan::PostEnterEventToLattice(QEnterEvent* event){

    if(this->parentWidget()) {
        this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        QPointF point = this->mapTo(this->parentWidget(), event->position());
        QWidget *lat = this->parentWidget()->childAt(point.toPoint());
        if (lat) {
            QEnterEvent *trans_event = new QEnterEvent(point,point,point);
            QApplication::postEvent(lat, trans_event);
        }
        this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    }
}

void ChessMan::PostLeaveEventToLattice(QEvent* event){
    Q_UNUSED(event);

    if(this->parentWidget()) {
        this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        QPoint point(this->x(),this->y());
        QWidget *lat = this->parentWidget()->childAt(point);
        if (lat) {
            QEvent *trans_event = new QEvent(QEvent::Leave);
            QApplication::postEvent(lat, trans_event);
        }
        this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    }
}

QString ChessMan::getChessPic(){
    return chessPic;
}
