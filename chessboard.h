#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QtWidgets>
#include "Lattice.h"
#include "chessman.h"
#include <QVector>
#include <QPointF>
#include <QString>
#include <QCloseEvent>

class ChessBoard : public QWidget
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *parent = nullptr);
    //初始化
    void boardInital(int type);
    //拷贝
    ChessBoard(ChessBoard& chessboard);
    //游戏结束后的操作
    void End();
    //判断team方是否将军对方，返回值为1则单将，为2则双将，为0则未将军
    int isCheck(int team);
    //判断是否将死，team为将死对面的一方
    bool isEnd(int team);
    //返回棋盘布局，让别的窗口能够显示棋盘的画面
    QGridLayout* getBoardLayout();
    //格子被鼠标点击后的处理槽函数
    void handleLeftClick(QPointF point);
    //悔棋
    void regret();
    //前面几步不给悔棋（开局就悔棋也太逊了）
    QVector<QString>& getRecord();
    //获取该坐标上的棋子，若无棋子则返回NULL
    ChessMan* chessManAtPosition(QPointF point);
    //判断能否王车易位
    bool canCasting(ChessMan* clicked_man);
    //判断select的棋子能否移动至点击的位置
    bool canMoveTo(ChessMan* chess, QPointF point);
    //判断team方的棋子走到point位置是否被敌方棋子攻击到
    bool isAttacked(int team, QPointF point);
    //更改棋盘颜色
    void changeBoardType(int choice);
    //当前持方
    int getCurrentTeam();
    //兵的升变
    void pawnRising(int& type,QString& pic);
    //读取存档时的行动
    void recordMove(QString move);
    //初始化棋子和格子
    void initialColor(int type);

    QVector<QVector<Lattice*>> getLattice();
    ChessMan* getSelect();
private:
    //格子大小
    //int Latticesize;
    //棋盘格子记录，共8×8个格子
    QVector<QVector<Lattice*>> board;
    //黑白两个队伍的棋子
    ChessMan* teamwhite[16];
    ChessMan* teamblack[16];
    //游戏棋盘
    QGridLayout* board_layout;
    //记录选中的棋子
    ChessMan* select;
    //记录死亡的棋子，用于悔棋时让棋子重新填入棋盘
    QVector<ChessMan*> death;
    //移动数据记录
    QVector<QString> record;
    //当前轮到哪方玩家
    int current_team;

signals:
    void moveSuccess();
    void endSuccess();
};

#endif // CHESSBOARD_H
