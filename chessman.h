#ifndef CHESSMAN_H
#define CHESSMAN_H

#include <QLabel>
#include <QGridLayout>

class ChessMan : public QLabel
{
    Q_OBJECT
public:
    explicit ChessMan(QWidget *parent = nullptr);
    //拷贝构造
    ChessMan(ChessMan& chessman);
    //有参构造
    ChessMan(int type,int team);
    ChessMan(int type,int team,int x,int y);
    //重载赋值运算符=
    ChessMan& operator=(ChessMan& chessman);

    //棋子类型 王0 后1 相（主教）2 马（骑士）3 车（城堡）4 兵5
    enum CHESSTYPE{KING,QUEEN,BISHOP,KNIGHT,ROOK,PAWN};
    //棋子阵营 白方0 黑方1
    enum CHESSTEAM{WHITE,BLACK};
    //注册到元对象系
    Q_ENUM(CHESSTYPE)
    Q_ENUM(CHESSTEAM)

    int getId();
    int getPos_row();
    int getPos_col();
    int getMoveCount();
    int getChessType();
    int getChessTeam();
    bool isDead();
    void setPos_row(int row);
    void setPos_col(int col);
    void setPosition(int row,int col);
    void setIsDead(bool val);
    void setChessType(int type);
    void setChessPic(QString pic);
    void hasMove();
    void regretMove();
    QString getChessPic();

    //点击棋盘上一个位置时的响应
    void mousePressEvent(QMouseEvent* event)override;
    //鼠标在棋盘上的时候有相应的响应——当鼠标放在该格子上时，格子有高亮效果
    void enterEvent(QEnterEvent* event)override;
    void leaveEvent(QEvent* event)override;
    void PostMouseEventToLattice(QMouseEvent* event);
    void PostEnterEventToLattice(QEnterEvent* event);
    void PostLeaveEventToLattice(QEvent* event);

    static int CHESS_ID;
private:
    //记录棋子id
    int id;
    //记录在棋盘的位置
    int pos_row;
    int pos_col;
    //记录该棋子对象行动的次数（用于判断特殊规则）
    int movecount;
    //棋子所属的类型
    int chesstype;
    //棋子所属的阵营
    int chessteam;
    //棋子是否被吃
    bool isdead;
    //棋子的图片(棋子类型改变时也要跟着改变）
    QString chessPic;
signals:

};

#endif // CHESSMAN_H
