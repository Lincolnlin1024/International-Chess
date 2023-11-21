#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include <QMessageBox>
#include "mainwindow.h"
#include "chessboard.h"

class Game : public QMainWindow
{
    Q_OBJECT
public:
    explicit Game(QWidget *parent = nullptr);
    Game(int choice,int saved);
    void twoPlayersInit(int type,int saved);
    QPixmap PixmapToRound(const QString &src, QLabel* target_label);

    void setBackgroundPixmap(const QPixmap& pixmap);
    void paintEvent(QPaintEvent* event)override;
    void resizeEvent(QResizeEvent* event)override;
    void newProject(QFile& file, Game* new_game);
    void basicElementInitial(QFont font,int type);
    void new_project();
    void open_project();
    void save_project();
    QDialog* help_list();

protected:
   void changeEvent(QEvent *event)override;

private:
    ChessBoard* chessboard;
    QToolBar* toolbar;
    QPixmap background_pixmap;//背景图片
    QPixmap null_pixmap;//存档没有棋局时的图
    int saved_project;//记录应该保存到几号存档
    //记录信息
    QLabel * step1;
    QLabel * step2;
    QLabel * avatar1;
    QLabel * avatar2;
    QLabel * holder1;
    QLabel * holder2;
    QAction * help_action;
    int board_color_type;//记录棋盘的颜色
signals:
    void backtrack();
};

#endif // GAME_H
