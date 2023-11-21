#ifndef LATICE_H
#define LATICE_H

#include <QLabel>

class Latice : public QLabel
{
    Q_OBJECT
public:
    explicit Latice(QWidget *parent = nullptr);
    //提供有参构造
    Latice(int row,int col,QWidget *parent = nullptr);
    Latice(Latice& latice);
    ~Latice();
    //重载赋值运算符=
    Latice& operator=(Latice& latice);

    int getPos_row();
    int getPos_col();
    void setPos_row(int row);
    void setPos_col(int Col);

    //点击棋盘上一个位置时的响应
    void mousePressEvent(QMouseEvent* event)override;
    //鼠标在棋盘上的时候有相应的响应——当鼠标放在该格子上时，格子有高亮效果
    void enterEvent(QEnterEvent* event)override;
    void leaveEvent(QEvent* event)override;

private:
    //记录在棋盘的位置
    int pos_row;
    int pos_col;

signals:
    //被左键点击信号
    void leftButtonClicked(QPointF);
    //被右键点击信号
    void rightButtonClicked(QPointF);
};

#endif // LATICE_H
