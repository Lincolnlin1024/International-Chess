#include "chessboard.h"
#include <QDebug>

ChessBoard::ChessBoard(QWidget *parent)
    : QWidget{parent}
{

}

void ChessBoard::boardInital(int type){
    ChessMan::CHESS_ID=1;
    select = NULL;
    current_team = 0;//白方先手
    board_layout = new QGridLayout();//棋盘的布局

//给每个格子对象初始化，且用数组保存，后续方便得到对应对象及其坐标值用于实现棋子的移动
    board.resize(8);//8行
    for(int i=0;i<board.size();++i)
    {
        board[i].resize(8);//8列
    }

    initialColor(type);

//设置信号和槽，点击格子对象得到对应格子坐标进行后续处理操作
    void (Lattice:: *left_click)(QPointF)=&Lattice::leftButtonClicked;
    void (ChessBoard:: *handle_left_click)(QPointF)=&ChessBoard::handleLeftClick;
    for(int i=0;i<8;++i){
        for(int j=0;j<8;++j){
            connect(board[i][j],left_click,this,handle_left_click);
        }
    }

//设置伸缩系数，使各行&各列伸缩系数同为1
    for(int i = 0;i<8;++i){
        board_layout->setColumnStretch(i,1);
        board_layout->setRowStretch(i,1);
    }

//设置控件之间空隙为0（即让棋盘格子之间不存在空隙）
    board_layout->setSpacing(0);

    this->setLayout(board_layout);
    this->resize(500,500);
}

ChessBoard::ChessBoard(ChessBoard& chessboard){
    for(int i=0;i<8;++i){
            for(int j=0;j<8;++j){
                board[i][j]=new Lattice(*chessboard.board[i][j]);
            }
        }
    for(int i=0;i<16;++i){
        teamwhite[i]=new ChessMan(*chessboard.teamwhite[i]);
    }
    for(int i=0;i<16;++i){
        teamblack[i]=new ChessMan(*chessboard.teamblack[i]);
    }

    board_layout = new QGridLayout();

    if(chessboard.select!=NULL){
        int id =chessboard.select->getId();
        if(id<17){
            select = teamwhite[id-1];
        }
        else{
            select = teamblack[id-17];
        }
    }

    for(QVector<ChessMan*>::iterator it = chessboard.death.begin();it!=chessboard.death.end();++it){
        ChessMan* dead_man = *it;
        int dead_id = dead_man->getId();
        if(dead_id<17){
            death.append(teamwhite[dead_id-1]);
            teamwhite[dead_id-1]->setIsDead(true);
        }
        else{
            death.append(teamwhite[dead_id-17]);
            teamwhite[dead_id-17]->setIsDead(true);
        }
    }

    for(int i=0;i<16;++i){
        if(!teamwhite[i]->isDead()){
            board_layout->addWidget(teamwhite[i],teamwhite[i]->getPos_row(),teamwhite[i]->getPos_col());
        }
    }
    for(int i=0;i<16;++i){
        if(!teamblack[i]->isDead()){
            board_layout->addWidget(teamblack[i],teamblack[i]->getPos_row(),teamblack[i]->getPos_col());
        }
    }

    //设置伸缩系数，使各行&各列伸缩系数同为1
    for(int i = 0;i<8;++i){
        board_layout->setColumnStretch(i,1);
        board_layout->setRowStretch(i,1);
    }

    //设置控件之间空隙为0（即让棋盘格子之间不存在空隙）
    board_layout->setSpacing(0);

    record=chessboard.record;
    //当前轮到哪方玩家
    current_team=chessboard.current_team;

}

QGridLayout* ChessBoard::getBoardLayout(){
    return board_layout;
}

void ChessBoard::End(){
    QDateTime dateTime= QDateTime::currentDateTime();//获取系统当前的时间
    QString time = dateTime.toString("yyyy-MM-dd hh：mm：ss");//格式化时间
    QString record_path = "./record/";
    record_path.append(time);
    record_path.append(".txt");

    QFile file(record_path);
    if (file.open(QIODevice::ReadWrite | QIODevice::Text)){

        QTextStream output_stream(&file);

        //写入文件内容
        for(int i=0;i<record.size();++i){
            output_stream << record[i] << "\n";
        }

        file.close();
    }
    else{
        QMessageBox::critical(this,"Error","Failed to create a new file!");//模态的对话框
    }

    QString win = "Game Over!\n";
    if(record.back()[2].toLatin1()=='/'){
        win.append("Draw!");
    }
    else if(record.back()[1].toLatin1()=='1'){
        win.append("White Win!");
    }
    else{
        win.append("Black Win!");
    }
    QMessageBox::information(this,"Game Over",win);

    emit endSuccess();

}

int ChessBoard::isCheck(int team){
    qDebug()<<"isCheck检测！";

    QPointF enemy_king;
    int count = 0;
    if(team){
        enemy_king = QPointF(teamwhite[0]->getPos_row(),teamwhite[0]->getPos_col());
        for(int i=0;i<16;++i){
            if(!teamblack[i]->isDead()&&canMoveTo(teamblack[i], enemy_king)){
                ++count;
            }
        }
    }
    else{
        enemy_king = QPointF(teamblack[0]->getPos_row(),teamblack[0]->getPos_col());
        for(int i=0;i<16;++i){
            if(!teamwhite[i]->isDead()&&canMoveTo(teamwhite[i], enemy_king)){
                ++count;
            }
        }
    }

    if(count==1){
        qDebug()<<"isCheck1！";
        return 1;
    }
    else if(count>1){
        qDebug()<<"isCheck2！";
        return 2;
    }

    qDebug()<<"isCheck0！";
    return 0;
}

bool ChessBoard::isEnd(int team){
    QPointF king;
    qDebug()<<"isEnd检测！";

    if(team){
        king = QPointF(teamwhite[0]->getPos_row(),teamwhite[0]->getPos_col());

        for(int i=0;i<16;++i){
            if(teamwhite[i]->isDead()){
                continue;
            }
            int row = 0;
            int col = 0;
            for(;row<8;++row){
                col = 0;
                for(;col<8;++col){
                    QPointF point(row,col);
                    if(canMoveTo(teamwhite[i],point)){
                        ChessMan* posible_kill = chessManAtPosition(point);
                        if(posible_kill==NULL){
                            QChar step[9]={' ',' ',' ','-',' ',' ',' ',' ','\0'};//初始化移动记录
                            switch(teamwhite[i]->getChessType()){
                            case 0:{
                                step[0]='K';
                                break;
                            }
                            case 1:{
                                step[0]='Q';
                                break;
                            }
                            case 2:{
                                step[0]='B';
                                break;
                            }
                            case 3:{
                                step[0]='N';
                                break;
                            }
                            case 4:{
                                step[0]='R';
                                break;
                            }
                            case 5:{
                                step[0]=QChar(teamwhite[i]->getPos_col()+97);
                                if(point.y()!=teamwhite[i]->getPos_col()){//当点击点和兵不在同一列又能移动时，肯定是特殊规则的吃子方式
                                    step[3]='x';
                                    ChessMan* eaten_man = chessManAtPosition(QPointF(teamwhite[i]->getPos_row(),point.y()));
                                    board_layout->removeWidget(eaten_man);
                                    eaten_man->setIsDead(true);
                                    death.append(eaten_man);
                                }
                                break;
                            }
                            }
                            step[1]=QChar(teamwhite[i]->getPos_col()+97);//由列转变成a-h
                            step[2]=QChar('8'-teamwhite[i]->getPos_row());//由行转变成1-8
                            step[4]=QChar((int)point.y()+97);
                            step[5]=QChar('8'-(int)point.x());

                            board_layout->removeWidget(teamwhite[i]);
                            board_layout->addWidget(teamwhite[i],point.x(),point.y());
                            teamwhite[i]->setPosition(point.x(),point.y());
                            teamwhite[i]->hasMove();

                            QString record_step = QString(step);
                            record.append(record_step);

                            qDebug()<<"record.back()->"<<record.back()<<record_step.capacity()<<record_step.size()<<record_step.length();

                            if(i==0){
                                king.rx()=point.x();
                                king.ry()=point.y();
                            }

                            if(current_team){
                                current_team = 0;
                            }
                            else{
                                current_team = 1;
                            }

                            if(!isAttacked(ChessMan::WHITE,king)){
                                regret();
                                qDebug()<<"isEnd没将死2！";
                                return false;
                            }
                        }
                        else{
                            QChar step[9]={' ',' ',' ','x',' ',' ',' ',' ','\0'};//初始化吃子记录
                            switch(teamwhite[i]->getChessType()){
                            case 0:{
                                step[0]='K';
                                break;
                            }
                            case 1:{
                                step[0]='Q';
                                break;
                            }
                            case 2:{
                                step[0]='B';
                                break;
                            }
                            case 3:{
                                step[0]='N';
                                break;
                            }
                            case 4:{
                                step[0]='R';
                                break;
                            }
                            case 5:{
                                step[0]=QChar(teamwhite[i]->getPos_col()+97);
                                break;
                            }
                            }
                            step[1]=QChar(teamwhite[i]->getPos_col()+97);//由列转变成a-h
                            step[2]=QChar('8'-teamwhite[i]->getPos_row());//由行转变成1-8
                            step[4]=QChar((int)point.y()+97);
                            step[5]=QChar('8'-(int)point.x());

                            board_layout->removeWidget(teamwhite[i]);
                            board_layout->removeWidget(posible_kill);
                            board_layout->addWidget(teamwhite[i],point.x(),point.y());
                            posible_kill->setIsDead(true);
                            teamwhite[i]->setPosition(point.x(),point.y());
                            teamwhite[i]->hasMove();

                            QString record_step = QString(step);
                            record.append(record_step);
                            death.append(posible_kill);

                            qDebug()<<"record.back()->"<<record.back()<<record_step.capacity()<<record_step.size()<<record_step.length();

                            if(i==0){
                                king.rx()=point.x();
                                king.ry()=point.y();
                            }

                            if(current_team){
                                current_team = 0;
                            }
                            else{
                                current_team = 1;
                            }

                            if(!isAttacked(ChessMan::WHITE,king)){
                                regret();
                                qDebug()<<"isEnd没将死3！";
                                return false;
                            }
                        }
                        regret();
                    }
                }
            }
        }
    }
    else{
        king = QPointF(teamblack[0]->getPos_row(),teamblack[0]->getPos_col());

        for(int i=0;i<16;++i){
            if(teamblack[i]->isDead()){
                qDebug()<<i<<"dead";
                continue;
            }
            int row = 0;
            int col = 0;
            for(;row<8;++row){
                qDebug()<<"row"<<row;
                col = 0;
                for(;col<8;++col){
                    qDebug()<<"col"<<col;
                    QPointF point(row,col);
                    if(canMoveTo(teamblack[i],point)){
                        ChessMan* posible_kill = chessManAtPosition(point);
                        if(posible_kill==NULL){
                            QChar step[9]={' ',' ',' ','-',' ',' ',' ',' ','\0'};//初始化移动记录
                            switch(teamblack[i]->getChessType()){
                            case 0:{
                                step[0]='K';
                                break;
                            }
                            case 1:{
                                step[0]='Q';
                                break;
                            }
                            case 2:{
                                step[0]='B';
                                break;
                            }
                            case 3:{
                                step[0]='N';
                                break;
                            }
                            case 4:{
                                step[0]='R';
                                break;
                            }
                            case 5:{
                                step[0]=QChar(teamblack[i]->getPos_col()+97);
                                if(point.y()!=teamblack[i]->getPos_col()){//当点击点和兵不在同一列又能移动时，肯定是特殊规则的吃子方式
                                    step[3]='x';
                                    ChessMan* eaten_man = chessManAtPosition(QPointF(teamblack[i]->getPos_row(),point.y()));
                                    board_layout->removeWidget(eaten_man);
                                    eaten_man->setIsDead(true);
                                    death.append(eaten_man);
                                }
                                break;
                            }
                            }
                            step[1]=QChar(teamblack[i]->getPos_col()+97);//由列转变成a-h
                            step[2]=QChar('8'-teamblack[i]->getPos_row());//由行转变成1-8
                            step[4]=QChar((int)point.y()+97);
                            step[5]=QChar('8'-(int)point.x());

                            board_layout->removeWidget(teamblack[i]);
                            board_layout->addWidget(teamblack[i],point.x(),point.y());
                            teamblack[i]->setPosition(point.x(),point.y());
                            teamblack[i]->hasMove();

                            QString record_step = QString(step);
                            record.append(record_step);

                            qDebug()<<"record.back()->"<<record.back()<<record_step.capacity()<<record_step.size()<<record_step.length();

                            if(i==0){
                                king.rx()=point.x();
                                king.ry()=point.y();
                            }

                            qDebug()<<"isEnd king:"<<king.x()<<king.y();

                            if(current_team){
                                current_team = 0;
                            }
                            else{
                                current_team = 1;
                            }

                            if(!isAttacked(ChessMan::BLACK,king)){
                                qDebug()<<"将要regret!";
                                regret();
                                qDebug()<<"isEnd没将死5！";
                                return false;
                            }
                            else{
                                qDebug()<<"不用regret?";
                            }
                        }
                        else{
                            QChar step[9]={' ',' ',' ','x',' ',' ',' ',' ','\0'};//初始化吃子记录
                            switch(teamblack[i]->getChessType()){
                            case 0:{
                                step[0]='K';
                                break;
                            }
                            case 1:{
                                step[0]='Q';
                                break;
                            }
                            case 2:{
                                step[0]='B';
                                break;
                            }
                            case 3:{
                                step[0]='N';
                                break;
                            }
                            case 4:{
                                step[0]='R';
                                break;
                            }
                            case 5:{
                                step[0]=QChar(teamblack[i]->getPos_col()+97);
                                break;
                            }
                            }
                            step[1]=QChar(teamblack[i]->getPos_col()+97);//由列转变成a-h
                            step[2]=QChar('8'-teamblack[i]->getPos_row());//由行转变成1-8
                            step[4]=QChar((int)point.y()+97);
                            step[5]=QChar('8'-(int)point.x());

                            board_layout->removeWidget(teamblack[i]);
                            board_layout->removeWidget(posible_kill);
                            board_layout->addWidget(teamblack[i],point.x(),point.y());
                            posible_kill->setIsDead(true);
                            teamblack[i]->setPosition(point.x(),point.y());
                            teamblack[i]->hasMove();

                            QString record_step = QString(step);
                            record.append(record_step);
                            death.append(posible_kill);

                            qDebug()<<"record.back()->"<<record.back()<<record_step.capacity()<<record_step.size()<<record_step.length();

                            if(i==0){
                                king.rx()=point.x();
                                king.ry()=point.y();
                            }

                            if(current_team){
                                current_team = 0;
                            }
                            else{
                                current_team = 1;
                            }

                            if(!isAttacked(ChessMan::BLACK,king)){
                                regret();
                                qDebug()<<"isEnd没将死6！";
                                return false;
                            }
                        }
                        regret();
                    }
                }
            }
        }
    }

    qDebug()<<"isEnd将死！";
    return true;
}

void ChessBoard::handleLeftClick(QPointF point){
    qDebug()<<'\n'<<"handleLeftClick点击位置："<<point.x()<<point.y();
    ChessMan* clicked_man = chessManAtPosition(point);
    bool moved = false;

    if(clicked_man!=NULL){//左键点击棋子
        qDebug()<<"handleLeftClick点击棋子!";
        qDebug()<<"handleLeftClick clicked_man:"<<clicked_man->getPos_row()<<clicked_man->getPos_col();

        if(select==NULL){//点击前未选中棋子

            if(current_team==clicked_man->getChessTeam()){
                qDebug()<<"选中新棋子！";

                clicked_man->setFrameShape(QFrame::Box);
                clicked_man->setStyleSheet("border-width: 2px;border-style: solid;border-color: rgb(255,100,0);");

                select = clicked_man;

            }
            else{
                qDebug()<<"您不能选中对方的棋子！";
            }
            return;
        }
        else if(select->getId()!=clicked_man->getId()){//点击未选中棋子
            if(select->getChessTeam()==clicked_man->getChessTeam()){
                qDebug()<<"选中同一队伍！";
                if(canCasting(clicked_man)){
                    QChar step[9]={'O','-','O',' ',' ',' ',' ',' ','\0'};//初始化为短易位的记录

                    if(select->getChessType()==ChessMan::KING){
                        int king_col = select->getPos_col();
                        int rook_col = clicked_man->getPos_col();
                        int pos_row = select->getPos_row();

                        board_layout->removeWidget(select);
                        board_layout->removeWidget(clicked_man);

                        if(rook_col<king_col){//长易位
                            board_layout->addWidget(select,pos_row,king_col-2);
                            select->setPos_col(king_col-2);

                            board_layout->addWidget(clicked_man,pos_row,king_col-1);
                            clicked_man->setPos_col(king_col-1);

                            step[3]='-';
                            step[4]='O';
                        }
                        else{//短易位
                            board_layout->addWidget(select,pos_row,king_col+2);
                            select->setPos_col(king_col+2);

                            board_layout->addWidget(clicked_man,pos_row,king_col+1);
                            clicked_man->setPos_col(king_col+1);
                        }

                    }
                    else{
                        int king_col = clicked_man->getPos_col();
                        int rook_col = select->getPos_col();
                        int pos_row = select->getPos_row();

                        board_layout->removeWidget(select);
                        board_layout->removeWidget(clicked_man);

                        if(rook_col<king_col){//长易位
                            board_layout->addWidget(clicked_man,pos_row,king_col-2);
                            clicked_man->setPos_col(king_col-2);

                            board_layout->addWidget(select,pos_row,king_col-1);
                            select->setPos_col(king_col-1);

                            step[3]='-';
                            step[4]='O';
                        }
                        else{//短易位
                            board_layout->addWidget(clicked_man,pos_row,king_col+2);
                            clicked_man->setPos_col(king_col+2);

                            board_layout->addWidget(select,pos_row,king_col+1);
                            select->setPos_col(king_col+1);
                        }
                    }
                    select->setFrameShape(QFrame::NoFrame);
                    select->setStyleSheet("border-width: 0px");
                    select->hasMove();
                    clicked_man->hasMove();
                    moved = true;
                    select = NULL;

                    //判断将军/双将/将死——只可能是车的将军
                    int check = isCheck(current_team);
                    if(check==1){//将军+
                        step[6]='+';
                        if(isEnd(current_team)){//将死#
                            step[6]='#';
                        }
                    }
                    else if(check==2){
                        step[6]='+';
                        step[7]='+';
                        if(isEnd(current_team)){//将死#
                            step[6]='#';
                        }
                    }

                    //更改当前下棋的持方
                    if(current_team){
                        current_team = 0;
                    }
                    else{
                        current_team = 1;
                    }

                    QString record_step = QString(step);
                    record.append(record_step);

                    qDebug()<<"王车易位 record.back()->"<<record.back()<<record_step.capacity()<<record_step.size()<<record_step.length();
                }
                else{

                    select->setFrameShape(QFrame::NoFrame);
                    select->setStyleSheet("border-width: 0px");
                    clicked_man->setFrameShape(QFrame::Box);
                    clicked_man->setStyleSheet("border-width: 2px;border-style: solid;border-color: rgb(255,100,0);");

                    select = clicked_man;

                    return;
                }
            }
            else{
                qDebug()<<"选中不同队伍！";

                if(canMoveTo(select,point)){//发生吃子事件
                    qDebug()<<"handleLeftClick点击棋子吃子！";

                    QChar step[9]={' ',' ',' ','x',' ',' ',' ',' ','\0'};//初始化吃子记录
                    switch(select->getChessType()){
                    case 0:{
                        step[0]='K';
                        break;
                    }
                    case 1:{
                        step[0]='Q';
                        break;
                    }
                    case 2:{
                        step[0]='B';
                        break;
                    }
                    case 3:{
                        step[0]='N';
                        break;
                    }
                    case 4:{
                        step[0]='R';
                        break;
                    }
                    case 5:{
                        step[0]=QChar(select->getPos_col()+97);
                        break;
                    }
                    }
                    step[1]=QChar(select->getPos_col()+97);//由列转变成a-h
                    step[2]=QChar('8'-select->getPos_row());//由行转变成1-8
                    step[4]=QChar((int)point.y()+97);
                    step[5]=QChar('8'-(int)point.x());

                    board_layout->removeWidget(select);
                    board_layout->removeWidget(clicked_man);
                    board_layout->addWidget(select,point.x(),point.y());

                    clicked_man->setIsDead(true);

                    select->setPosition(point.x(),point.y());
                    select->setFrameShape(QFrame::NoFrame);
                    select->setStyleSheet("border-width: 0px");
                    select->hasMove();
                    moved = true;
                    select = NULL;

                    //再判断是否将军/双将/将死
                    int check = isCheck(current_team);
                    if(check==1){//将军+
                        step[6]='+';
                        if(isEnd(current_team)){//将死#
                            step[6]='#';
                        }
                    }
                    else if(check==2){
                        step[6]='+';
                        step[7]='+';
                        if(isEnd(current_team)){//将死#
                            step[6]='#';
                        }
                    }

                    //更改当前下棋的持方
                    if(current_team){
                        current_team = 0;
                    }
                    else{
                        current_team = 1;
                    }

                    QString record_step = QString(step);
                    record.append(record_step);
                    death.append(clicked_man);

                    qDebug()<<"handleLeftClick record.back()->"<<record.back()<<record_step.capacity()<<record_step.size()<<record_step.length();
                }
                else{
                    qDebug()<<"handleLeftClick点击棋子非法移动！";
                }
            }
        }
        else{//点击的是已经点击了的棋子（执行取消选中操作）

            clicked_man->setFrameShape(QFrame::NoFrame);
            clicked_man->setStyleSheet("border-width: 0px");

            select = NULL;
            return;
        }
    }
    else{//左键点击棋盘

        if(select!=NULL){
            qDebug()<<"handleLeftClick点击棋盘移动！";

            if(canMoveTo(select,point)){

                QChar step[9]={' ',' ',' ','-',' ',' ',' ',' ','\0'};//初始化移动记录
                switch(select->getChessType()){
                case 0:{
                    step[0]='K';
                    break;
                }
                case 1:{
                    step[0]='Q';
                    break;
                }
                case 2:{
                    step[0]='B';
                    break;
                }
                case 3:{
                    step[0]='N';
                    break;
                }
                case 4:{
                    step[0]='R';
                    break;
                }
                case 5:{
                    step[0]=QChar(select->getPos_col()+97);
                    if(point.y()!=select->getPos_col()){//当点击点和兵不在同一列又能移动时，肯定是特殊规则的吃子方式
                        step[3]='x';
                        ChessMan* eaten_man = chessManAtPosition(QPointF(select->getPos_row(),point.y()));
                        board_layout->removeWidget(eaten_man);
                        eaten_man->setIsDead(true);
                        death.append(eaten_man);
                    }
                    break;
                }
                }
                step[1]=QChar(select->getPos_col()+97);//由列转变成a-h
                step[2]=QChar('8'-select->getPos_row());//由行转变成1-8
                step[4]=QChar((int)point.y()+97);
                step[5]=QChar('8'-(int)point.x());

                board_layout->removeWidget(select);
                board_layout->addWidget(select,point.x(),point.y());

                select->setPosition(point.x(),point.y());
                select->setFrameShape(QFrame::NoFrame);
                select->setStyleSheet("border-width: 0px");
                select->hasMove();
                moved = true;
                select = NULL;

                //再判断是否将军/双将/将死
                int check = isCheck(current_team);
                if(check==1){//将军+
                    step[6]='+';
                    if(isEnd(current_team)){//将死#
                        qDebug()<<"handleLeftClick将死！";
                        step[6]='#';
                    }
                    else{
                        qDebug()<<"handleLeftClick没将死！";
                    }
                }
                else if(check==2){
                    step[6]='+';
                    if(isEnd(current_team)){//将死#
                        qDebug()<<"handleLeftClick将死！";
                        step[6]='#';
                    }
                    else{
                        step[7]='+';
                        qDebug()<<"handleLeftClick没将死！";
                    }
                }

                //更改当前下棋的持方
                if(current_team){
                    current_team = 0;
                }
                else{
                    current_team = 1;
                }

                QString record_step = QString(step);
                record.append(record_step);

                qDebug()<<"handleLeftClick record.back()->"<<record.back()<<record_step.capacity()<<record_step.size()<<record_step.length();
            }
            else{
                qDebug()<<"handleLeftClick点击棋盘非法移动！";
            }
        }
    }

    if(moved&&!record.isEmpty()){
        int moved_team = 1;
        if(current_team){
            moved_team = 0;
        }

        int king_row;
        int king_col;
        if(moved_team){
            king_row = teamblack[0]->getPos_row();
            king_col = teamblack[0]->getPos_col();
        }
        else{
            king_row = teamwhite[0]->getPos_row();
            king_col = teamwhite[0]->getPos_col();
        }
        if(isAttacked(moved_team,QPointF(king_row,king_col))){//移动后己方被将军，应当不能走这一步棋
            regret();
            qDebug()<<"handleLeftClick行动后被将军！";
        }
        else{
            char chess_type = record.back()[0].toLatin1();
            char chess_row = record.back()[5].toLatin1();
            if(chess_type>96&&chess_type<105&&(chess_row=='1'||chess_row=='8')){
                int rise_type = 1;
                QString rise_pic="";
                if(current_team){
                    rise_pic=teamwhite[1]->getChessPic();
                }
                else{
                    rise_pic=teamblack[1]->getChessPic();
                }
                qDebug()<<rise_type<<rise_pic;
                pawnRising(rise_type,rise_pic);
                ChessMan* rising_man = chessManAtPosition(QPointF('8'-record.back()[5].toLatin1(),record.back()[4].toLatin1()-97));
                qDebug()<<rise_type<<rise_pic;
                rising_man->setChessType(rise_type);
                rising_man->setChessPic(rise_pic);
                rising_man->setPixmap(rise_pic);

                qDebug()<<rising_man->getChessType()<<rising_man->getChessPic();

                QString rising_move = record.back();
                record.removeLast();
                rising_move[6]='=';
                if(rise_type==1){
                    rising_move[7]='Q';
                }
                else if(rise_type==2){
                    rising_move[7]='B';
                }
                else if(rise_type==3){
                    rising_move[7]='N';
                }
                else{
                    rising_move[7]='R';
                }
                record.append(rising_move);
            }
            emit moveSuccess();
        }
    }

    if(moved&&!record.isEmpty()&&record.back()[6].toLatin1()=='#'){//将死，游戏结束
        QChar step[6]={' ',' ','-',' ','\0'};
        if(current_team){//现在是黑方持棋，则将死对方的是白方
            step[1]='1';
            step[3]='0';
        }
        else{
            step[1]='0';
            step[3]='1';
        }
        QString record_step = QString(step);

        record.append(record_step);
        qDebug()<<"handleLeftClick结束！"<<record.back();

        End();
    }
}

void ChessBoard::regret(){

    QString regret_move = record.back();

    qDebug()<<"regret当前步移除前："<<record.back();
    record.removeLast();
    qDebug()<<"regret当前步移除后："<<record.back();

    int moved_row;
    int moved_col;
    int regret_row;
    int regret_col;
    moved_row = '8'-regret_move[5].toLatin1();
    moved_col = regret_move[4].toLatin1()-'a';
    regret_row = '8'-regret_move[2].toLatin1();
    regret_col = regret_move[1].toLatin1()-'a';
    ChessMan* regret_chess = chessManAtPosition(QPointF(moved_row,moved_col));

    if(regret_move[0].toLatin1()>96&&regret_move[0].toLatin1()<105){
        regret_chess->setChessType(5);
        QString pic="";
        if(current_team){
            pic = teamwhite[0]->getChessPic();
        }
        else{
            pic = teamblack[0]->getChessPic();
        }
        int index = pic.lastIndexOf("_");//找到最后一个_的索引
        pic.truncate(index + 1);//删除_后所有内容
        pic.append("PAWN.png");//添加PAWN.png，让其可以变成兵的样式

        regret_chess->setChessPic(pic);
        regret_chess->setPixmap(pic);
    }

    qDebug()<<"regret1";
    ChessMan* relieve_chess = NULL;
    if(regret_move[3].toLatin1()=='x'){
        relieve_chess = death.back();
        death.removeLast();
    }

    qDebug()<<"regret2";
    regret_chess->setPosition(regret_row,regret_col);
    board_layout->removeWidget(regret_chess);
    board_layout->addWidget(regret_chess,regret_row,regret_col);
    regret_chess->regretMove();

    if(relieve_chess!=NULL){
        relieve_chess->setIsDead(false);

        qDebug()<<"regret3";
        int relieve_id = relieve_chess->getId();
        if(relieve_id<17){
            qDebug()<<"白";
            teamwhite[relieve_id-1] = new ChessMan(*relieve_chess);
            board_layout->addWidget(teamwhite[relieve_id-1],teamwhite[relieve_id-1]->getPos_row(),teamwhite[relieve_id-1]->getPos_col());
        }
        else{
            qDebug()<<"黑";
            teamblack[relieve_id-17] = new ChessMan(*relieve_chess);
            board_layout->addWidget(teamwhite[relieve_id-1],teamwhite[relieve_id-1]->getPos_row(),teamwhite[relieve_id-1]->getPos_col());
        }

        qDebug()<<"regret4";
        delete relieve_chess;
        qDebug()<<"regret5";
    }

    if(current_team){//持方换回去
        current_team = 0;
    }
    else{
        current_team = 1;
    }

    if(select!=NULL){
        qDebug()<<"6";
        select->setFrameShape(QFrame::NoFrame);
        select->setStyleSheet("border-width: 0px");
        qDebug()<<"7";
    }
    select = NULL;
}

QVector<QString>& ChessBoard::getRecord(){
    return record;
}

ChessMan* ChessBoard::chessManAtPosition(QPointF point){
    ChessMan* clicked_man = NULL;
    for(int i=0;i<16;++i){
        if(!teamwhite[i]->isDead()&&teamwhite[i]->getPos_row()==point.x()&&teamwhite[i]->getPos_col()==point.y()){
            clicked_man = teamwhite[i];
            break;
        }
        if(!teamblack[i]->isDead()&&teamblack[i]->getPos_row()==point.x()&&teamblack[i]->getPos_col()==point.y()){
            clicked_man = teamblack[i];
            break;
        }
    }

    if(clicked_man!=NULL){
        qDebug()<<"chessManAtPosition选中的棋子ID："<<clicked_man->getId();
    }
    else{
        qDebug()<<"chessManAtPosition选中空"<<point.x()<<point.y();
    }
    return clicked_man;
}

bool ChessBoard::canCasting(ChessMan* clicked_man){
    if(select->getChessType()==ChessMan::KING&&clicked_man->getChessType()==ChessMan::ROOK){//判断能否王车易位
        if(!select->getMoveCount()&&!clicked_man->getMoveCount()){//满足未移动过的条件
            int king_col = select->getPos_col();
            int rook_col = clicked_man->getPos_col();
            int pos_row = clicked_man->getPos_row();
            qDebug()<<king_col<<rook_col<<pos_row;

            if(rook_col<king_col){//长易位
                ChessMan* middle_chess1 = chessManAtPosition(QPointF(pos_row,king_col-1));
                ChessMan* middle_chess2 = chessManAtPosition(QPointF(pos_row,king_col-2));
                ChessMan* middle_chess3 = chessManAtPosition(QPointF(pos_row,king_col-3));
                if(!middle_chess1&&!middle_chess2&&!middle_chess3){//满足中间没有其他棋子的条件
                    if(!isAttacked(select->getChessType(),QPointF(pos_row,king_col))&&!isAttacked(select->getChessType(),QPointF(pos_row,king_col-1))
                            &&!isAttacked(select->getChessType(),QPointF(pos_row,king_col-2))){//满足王跨越的棋格不被攻击、王不正在被将军、易位后王不被将军的条件
                        return true;
                    }
                }
            }
            else{//短易位
                ChessMan* middle_chess1 = chessManAtPosition(QPointF(pos_row,king_col+1));
                ChessMan* middle_chess2 = chessManAtPosition(QPointF(pos_row,king_col+2));
                if(!middle_chess1&&!middle_chess2){//满足中间没有其他棋子的条件
                    if(!isAttacked(select->getChessType(),QPointF(pos_row,king_col))&&!isAttacked(select->getChessType(),QPointF(pos_row,king_col+1))
                            &&!isAttacked(select->getChessType(),QPointF(pos_row,king_col+2))){//满足王跨越的棋格不被攻击、王不正在被将军、易位后王不被将军的条件
                        return true;
                    }
                }
            }
        }
    }
    else if(select->getChessType()==ChessMan::ROOK&&clicked_man->getChessType()==ChessMan::KING){
        if(!select->getMoveCount()&&!clicked_man->getMoveCount()){//满足未移动过的条件
            int king_col = clicked_man->getPos_col();
            int rook_col = select->getPos_col();
            int pos_row = clicked_man->getPos_row();

            if(rook_col<king_col){//长易位
                ChessMan* middle_chess1 = chessManAtPosition(QPointF(pos_row,king_col-1));
                ChessMan* middle_chess2 = chessManAtPosition(QPointF(pos_row,king_col-2));
                ChessMan* middle_chess3 = chessManAtPosition(QPointF(pos_row,king_col-3));
                if(!middle_chess1&&!middle_chess2&&!middle_chess3){//满足中间没有其他棋子的条件
                    if(!isAttacked(select->getChessType(),QPointF(pos_row,king_col))&&!isAttacked(select->getChessType(),QPointF(pos_row,king_col-1))
                            &&!isAttacked(select->getChessType(),QPointF(pos_row,king_col-2))){//满足王跨越的棋格不被攻击、王不正在被将军、易位后王不被将军的条件
                        return true;
                    }
                }
            }
            else{//短易位
                ChessMan* middle_chess1 = chessManAtPosition(QPointF(pos_row,king_col+1));
                ChessMan* middle_chess2 = chessManAtPosition(QPointF(pos_row,king_col+2));
                if(!middle_chess1&&!middle_chess2){//满足中间没有其他棋子的条件
                    if(!isAttacked(select->getChessType(),QPointF(pos_row,king_col))&&!isAttacked(select->getChessType(),QPointF(pos_row,king_col+1))
                            &&!isAttacked(select->getChessType(),QPointF(pos_row,king_col+2))){//满足王跨越的棋格不被攻击、王不正在被将军、易位后王不被将军的条件
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool ChessBoard::canMoveTo(ChessMan* chess, QPointF point){
    qDebug()<<"canMoveTo检测！";

    if(point.x()<0||point.x()>7||point.y()<0||point.y()>7||(chess->getPos_row()==point.x()&&chess->getPos_col()==point.y())){
        return false;
    }

    ChessMan* man_at_point = chessManAtPosition(point);
    if(man_at_point!=NULL&&(chess->getChessTeam()==man_at_point->getChessTeam())){
        return false;
    }
    if(man_at_point!=NULL){
        qDebug()<<"canMoveTo:"<<chess->getId()<<man_at_point->getId();
    }
    else{
        qDebug()<<"canMoveTo:"<<chess->getId()<<point.x()<<point.y();
    }

    if(chess->getChessType()==ChessMan::KING){
        if(abs(point.x()-chess->getPos_row())<=1&&abs(point.y()-chess->getPos_col())<=1&&!isAttacked(chess->getChessTeam(),point)){
            if(chess->getChessTeam()){
                qDebug()<<"canMoveTo黑王能移动！";
            }
            else{
                qDebug()<<"canMoveTo白王能移动！";
            }
            return true;
        }
        if(chess->getChessTeam()){
            qDebug()<<"canMoveTo黑王不能移动！";
        }
        else{
            qDebug()<<"canMoveTo白王不能移动！";
        }

    }
    else if(chess->getChessType()==ChessMan::QUEEN){
        int has_chess_in_middle = 0;

        if(point.x()==chess->getPos_row()){//同一行
            if(point.y()<chess->getPos_col()){
                for(int col = point.y()+1;col<chess->getPos_col();++col){
                    if(chessManAtPosition(QPointF(point.x(),col))!=NULL){
                        has_chess_in_middle = 1;
                        break;
                    }
                }
            }
            else{
                for(int col = point.y()-1;col>chess->getPos_col();--col){
                    if(chessManAtPosition(QPointF(point.x(),col))!=NULL){
                        has_chess_in_middle = 1;
                        break;
                    }
                }
            }

            if(has_chess_in_middle==0){
                return true;
            }
        }
        else if(point.y()==chess->getPos_col()){//同一列
            if(point.x()<chess->getPos_row()){
                for(int row = point.x()+1;row<chess->getPos_row();++row){
                    if(chessManAtPosition(QPointF(row,point.y()))){
                        has_chess_in_middle = 1;
                        break;
                    }
                }
            }
            else{
                for(int row = point.x()-1;row>chess->getPos_row();--row){
                    if(chessManAtPosition(QPointF(row,point.y()))){
                        has_chess_in_middle = 1;
                        break;
                    }
                }
            }

            if(has_chess_in_middle==0){
                return true;
            }
        }
        else if(abs(point.x()-chess->getPos_row())==abs(point.y()-chess->getPos_col())){//在斜线上
            if(point.x()<chess->getPos_row()){
                int row = point.x()+1;
                if(point.y()<chess->getPos_col()){
                    int col = point.y()+1;
                    while(row<chess->getPos_row()){
                        if(chessManAtPosition(QPoint(row,col))){
                            has_chess_in_middle = 1;
                            break;
                        }
                        ++row;
                        ++col;
                    }
                }
                else{
                    int col = point.y()-1;
                    while(row<chess->getPos_row()){
                        if(chessManAtPosition(QPoint(row,col))){
                            has_chess_in_middle = 1;
                            break;
                        }
                        ++row;
                        --col;
                    }
                }
            }
            else{
                int row = point.x()-1;
                if(point.y()<chess->getPos_col()){
                    int col = point.y()+1;
                    while(row>chess->getPos_row()){
                        if(chessManAtPosition(QPoint(row,col))){
                            has_chess_in_middle = 1;
                            break;
                        }
                        --row;
                        ++col;
                    }
                }
                else{
                    int col = point.y()-1;
                    while(row>chess->getPos_row()){
                        if(chessManAtPosition(QPoint(row,col))){
                            has_chess_in_middle = 1;
                            break;
                        }
                        --row;
                        --col;
                    }
                }
            }

            if(has_chess_in_middle==0){
                return true;
            }
        }

    }
    else if(chess->getChessType()==ChessMan::BISHOP){
        int has_chess_in_middle = 0;

        if(abs(point.x()-chess->getPos_row())==abs(point.y()-chess->getPos_col())){//在斜线上
            if(point.x()<chess->getPos_row()){
                int row = point.x()+1;
                if(point.y()<chess->getPos_col()){
                    int col = point.y()+1;
                    while(row<chess->getPos_row()){
                        if(chessManAtPosition(QPoint(row,col))){
                            has_chess_in_middle = 1;
                            break;
                        }
                        ++row;
                        ++col;
                    }
                }
                else{
                    int col = point.y()-1;
                    while(row<chess->getPos_row()){
                        if(chessManAtPosition(QPoint(row,col))){
                            has_chess_in_middle = 1;
                            break;
                        }
                        ++row;
                        --col;
                    }
                }
            }
            else{
                int row = point.x()-1;
                if(point.y()<chess->getPos_col()){
                    int col = point.y()+1;
                    while(row>chess->getPos_row()){
                        if(chessManAtPosition(QPoint(row,col))){
                            has_chess_in_middle = 1;
                            break;
                        }
                        --row;
                        ++col;
                    }
                }
                else{
                    int col = point.y()-1;
                    while(row>chess->getPos_row()){
                        if(chessManAtPosition(QPoint(row,col))){
                            has_chess_in_middle = 1;
                            break;
                        }
                        --row;
                        --col;
                    }
                }
            }

            if(has_chess_in_middle==0){
                return true;
            }
        }

    }
    else if(chess->getChessType()==ChessMan::KNIGHT){
        if(((abs(point.x()-chess->getPos_row())==1)&&abs(point.y()-chess->getPos_col())==2)
                ||((abs(point.x()-chess->getPos_row())==2)&&abs(point.y()-chess->getPos_col())==1)){
            return true;
        }
    }
    else if(chess->getChessType()==ChessMan::ROOK){
        int has_chess_in_middle = 0;

        if(point.x()==chess->getPos_row()){//同一行
            if(point.y()<chess->getPos_col()){
                for(int col = point.y()+1;col<chess->getPos_col();++col){
                    if(chessManAtPosition(QPointF(point.x(),col))!=NULL){
                        has_chess_in_middle = 1;
                        break;
                    }
                }
            }
            else{
                for(int col = point.y()-1;col>chess->getPos_col();--col){
                    if(chessManAtPosition(QPointF(point.x(),col))!=NULL){
                        has_chess_in_middle = 1;
                        break;
                    }
                }
            }

            if(has_chess_in_middle==0){
                return true;
            }
        }
        else if(point.y()==chess->getPos_col()){//同一列
            if(point.x()<chess->getPos_row()){
                for(int row = point.x()+1;row<chess->getPos_row();++row){
                    if(chessManAtPosition(QPointF(row,point.y()))){
                        has_chess_in_middle = 1;
                        break;
                    }
                }
            }
            else{
                for(int row = point.x()-1;row>chess->getPos_row();--row){
                    if(chessManAtPosition(QPointF(row,point.y()))){
                        has_chess_in_middle = 1;
                        break;
                    }
                }
            }

            if(has_chess_in_middle==0){
                return true;
            }
        }

    }
    else{
        if(chess->getChessTeam()){//黑方
            if(point.y()==chess->getPos_col()){//判断移动
                if(point.x()-chess->getPos_row()==1&&chessManAtPosition(point)==NULL){
                    return true;
                }
                else if(point.x()-chess->getPos_row()==2&&chess->getMoveCount()==0&&chessManAtPosition(point)==NULL
                        &&chessManAtPosition(QPointF(point.x()-1,point.y()))==NULL){
                    return true;
                }
            }
            else if(abs(point.y()-chess->getPos_col())==1){//判断吃子
                qDebug()<<"canMoveTo黑方判断吃子！！";
                if(point.x()-chess->getPos_row()==1){
                    qDebug()<<"canMoveTo黑方差1！";
                    if(chessManAtPosition(point)!=NULL){
                        qDebug()<<"canMoveTo有子，可吃！";
                        return true;
                    }
                    else{//判断特殊规则，对方兵走两步到己方兵身边时，能斜走一步到对方兵背后并吃掉对方兵
                        QString pre_step = record.back();//上一步

                        ChessMan* behind_chess = chessManAtPosition(QPointF(point.x()-1,point.y()));
                        if(behind_chess!=NULL&&behind_chess->getChessTeam()!=chess->getChessTeam()
                                &&behind_chess->getChessType()==ChessMan::PAWN){

                            if(pre_step[0].toLatin1()==(char)((int)point.y()+97)&&pre_step[5].toLatin1()-pre_step[2].toLatin1()==2){
                                return true;
                            }
                        }
                    }
                }
            }
        }
        else{//白方
            if(point.y()==chess->getPos_col()){
                if(point.x()-chess->getPos_row()==-1&&chessManAtPosition(point)==NULL){
                    return true;
                }
                else if(point.x()-chess->getPos_row()==-2&&chess->getMoveCount()==0&&chessManAtPosition(point)==NULL
                        &&chessManAtPosition(QPointF(point.x()+1,point.y()))==NULL){
                    return true;
                }
            }
            else if(abs(point.y()-chess->getPos_col())==1){
                if(point.x()-chess->getPos_row()==-1){
                    if(chessManAtPosition(point)!=NULL){
                        return true;
                    }
                    else{//判断特殊规则，对方兵走两步到己方兵身边时，能斜走一步到对方兵背后并吃掉对方兵
                        QString pre_step = record.back();//上一步

                        ChessMan* behind_chess = chessManAtPosition(QPointF(point.x()+1,point.y()));
                        if(behind_chess!=NULL&&behind_chess->getChessTeam()!=chess->getChessTeam()
                                &&behind_chess->getChessType()==ChessMan::PAWN){

                            if(pre_step[0].toLatin1()==(char)((int)point.y()+97)&&pre_step[5].toLatin1()-pre_step[2].toLatin1()==-2){
                                return true;
                            }
                        }
                    }
                }
            }
        }

    }

    return false;
}

bool ChessBoard::isAttacked(int team, QPointF point){
    qDebug()<<"Attacked Check!";

    if(team==ChessMan::BLACK){//黑方的棋子判断isAttacked
        for(int i=0;i<8;++i){
            if(!(teamwhite[i]->isDead())&&canMoveTo(teamwhite[i],point)){
                qDebug()<<"Attacked!";
                return true;
            }
        }
        for(int i=8;i<16;++i){//兵的吃子方式与行走方式不同，单独拎出来判断
            if(!(teamwhite[i]->isDead())){
                if(teamwhite[i]->getChessType()==ChessMan::PAWN){//未升变的棋子
                    if((teamwhite[i]->getPos_row()-point.x()==1)&&abs(teamwhite[i]->getPos_col()-point.y())==1){
                        qDebug()<<"Attacked!";
                        return true;
                    }
                }
                else{//已经升变的棋子
                    if(canMoveTo(teamwhite[i],point)){
                        qDebug()<<"Attacked!";
                        return true;
                    }
                }
            }
        }
    }
    else{
        for(int i=0;i<8;++i){
            if(!(teamblack[i]->isDead())&&canMoveTo(teamblack[i],point)){
                qDebug()<<"Attacked!";
                return true;
            }
        }
        for(int i=8;i<16;++i){
            if(!(teamblack[i]->isDead())){
                if(teamblack[i]->getChessType()==ChessMan::PAWN){//未升变的棋子
                    if((teamblack[i]->getPos_row()-point.x()==-1)&&abs(teamblack[i]->getPos_col()-point.y())==1){
                        qDebug()<<"Attacked!";
                        return true;
                    }
                }
                else{//已经升变的棋子
                    if(canMoveTo(teamblack[i],point)){
                        qDebug()<<"Attacked!";
                        return true;
                    }
                }
            }
        }
    }

    qDebug()<<"Not Attacked!";
    return false;
}

void ChessBoard::changeBoardType(int choice){
    switch(choice){
    case 1:
        //格子
        for(int i = 0;i<8;++i){
            for(int j = 0;j<8;++j){
                if(i%2){
                    if(j%2){
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/White.png"));
                    }
                    else{
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/Black.png"));
                    }
                }
                else{
                    if(j%2){
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/Black.png"));
                    }
                    else{
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/White.png"));
                    }
                }
            }
        }
        //棋子
            //白
        for(int i=0;i<16;++i){
            if(i==0){
                teamwhite[i]->setChessPic(":/source/pictures/PureWhite_KING.png");
                teamwhite[i]->setPixmap(QPixmap(":/source/pictures/PureWhite_KING.png"));
            }
            else if(i==1){
                teamwhite[i]->setChessPic(":/source/pictures/PureWhite_QUEEN.png");
                teamwhite[i]->setPixmap(QPixmap(":/source/pictures/PureWhite_QUEEN.png"));
            }
            else if(i<4){
                teamwhite[i]->setChessPic(":/source/pictures/PureWhite_BISHOP.png");
                teamwhite[i]->setPixmap(QPixmap(":/source/pictures/PureWhite_BISHOP.png"));
            }
            else if(i<6){
                teamwhite[i]->setChessPic(":/source/pictures/PureWhite_KNIGHT.png");
                teamwhite[i]->setPixmap(QPixmap(":/source/pictures/PureWhite_KNIGHT.png"));
            }
            else if(i<8){
                teamwhite[i]->setChessPic(":/source/pictures/PureWhite_ROOK.png");
                teamwhite[i]->setPixmap(QPixmap(":/source/pictures/PureWhite_ROOK.png"));
            }
            else{
                teamwhite[i]->setChessPic(":/source/pictures/PureWhite_PAWN.png");
                teamwhite[i]->setPixmap(QPixmap(":/source/pictures/PureWhite_PAWN.png"));
            }
        }
            //黑
        for(int i=0;i<16;++i){
            if(i==0){
                teamblack[i]->setChessPic(":/source/pictures/PureBlack_KING.png");
                teamblack[i]->setPixmap(QPixmap(":/source/pictures/PureBlack_KING.png"));
            }
            else if(i==1){
                teamblack[i]->setChessPic(":/source/pictures/PureBlack_QUEEN.png");
                teamblack[i]->setPixmap(QPixmap(":/source/pictures/PureBlack_QUEEN.png"));
            }
            else if(i<4){
                teamblack[i]->setChessPic(":/source/pictures/PureBlack_BISHOP.png");
                teamblack[i]->setPixmap(QPixmap(":/source/pictures/PureBlack_BISHOP.png"));
            }
            else if(i<6){
                teamblack[i]->setChessPic(":/source/pictures/PureBlack_KNIGHT.png");
                teamblack[i]->setPixmap(QPixmap(":/source/pictures/PureBlack_KNIGHT.png"));
            }
            else if(i<8){
                teamblack[i]->setChessPic(":/source/pictures/PureBlack_ROOK.png");
                teamblack[i]->setPixmap(QPixmap(":/source/pictures/PureBlack_ROOK.png"));
            }
            else{
                teamblack[i]->setChessPic(":/source/pictures/PureBlack_PAWN.png");
                teamblack[i]->setPixmap(QPixmap(":/source/pictures/PureBlack_PAWN.png"));
            }
        }
        break;
    case 2:
        //格子
        for(int i = 0;i<8;++i){
            for(int j = 0;j<8;++j){
                if(i%2){
                    if(j%2){
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/Melon.png"));
                    }
                    else{
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/Brown.png"));
                    }
                }
                else{
                    if(j%2){
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/Brown.png"));
                    }
                    else{
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/Melon.png"));
                    }
                }
            }
        }
        //棋子
            //白
        for(int i=0;i<16;++i){
            if(i==0){
                teamwhite[i]->setChessPic(":/source/pictures/BrownWhite_KING.png");
                teamwhite[i]->setPixmap(QPixmap(":/source/pictures/BrownWhite_KING.png"));
            }
            else if(i==1){
                teamwhite[i]->setChessPic(":/source/pictures/BrownWhite_QUEEN.png");
                teamwhite[i]->setPixmap(QPixmap(":/source/pictures/BrownWhite_QUEEN.png"));
            }
            else if(i<4){
                teamwhite[i]->setChessPic(":/source/pictures/BrownWhite_BISHOP.png");
                teamwhite[i]->setPixmap(QPixmap(":/source/pictures/BrownWhite_BISHOP.png"));
            }
            else if(i<6){
                teamwhite[i]->setChessPic(":/source/pictures/BrownWhite_KNIGHT.png");
                teamwhite[i]->setPixmap(QPixmap(":/source/pictures/BrownWhite_KNIGHT.png"));
            }
            else if(i<8){
                teamwhite[i]->setChessPic(":/source/pictures/BrownWhite_ROOK.png");
                teamwhite[i]->setPixmap(QPixmap(":/source/pictures/BrownWhite_ROOK.png"));
            }
            else{
                teamwhite[i]->setChessPic(":/source/pictures/BrownWhite_PAWN.png");
                teamwhite[i]->setPixmap(QPixmap(":/source/pictures/BrownWhite_PAWN.png"));
            }
        }
            //黑
        for(int i=0;i<16;++i){
            if(i==0){
                teamblack[i]->setChessPic(":/source/pictures/BrownBlack_KING.png");
                teamblack[i]->setPixmap(QPixmap(":/source/pictures/BrownBlack_KING.png"));
            }
            else if(i==1){
                teamblack[i]->setChessPic(":/source/pictures/BrownBlack_QUEEN.png");
                teamblack[i]->setPixmap(QPixmap(":/source/pictures/BrownBlack_QUEEN.png"));
            }
            else if(i<4){
                teamblack[i]->setChessPic(":/source/pictures/BrownBlack_BISHOP.png");
                teamblack[i]->setPixmap(QPixmap(":/source/pictures/BrownBlack_BISHOP.png"));
            }
            else if(i<6){
                teamblack[i]->setChessPic(":/source/pictures/BrownBlack_KNIGHT.png");
                teamblack[i]->setPixmap(QPixmap(":/source/pictures/BrownBlack_KNIGHT.png"));
            }
            else if(i<8){
                teamblack[i]->setChessPic(":/source/pictures/BrownBlack_ROOK.png");
                teamblack[i]->setPixmap(QPixmap(":/source/pictures/BrownBlack_ROOK.png"));
            }
            else{
                teamblack[i]->setChessPic(":/source/pictures/BrownBlack_PAWN.png");
                teamblack[i]->setPixmap(QPixmap(":/source/pictures/BrownBlack_PAWN.png"));
            }
        }
        break;
    case 3:
        //格子
        for(int i = 0;i<8;++i){
            for(int j = 0;j<8;++j){
                if(i%2){
                    if(j%2){
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/Light_Wood.png"));
                    }
                    else{
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/Dark_Wood.png"));
                    }
                }
                else{
                    if(j%2){
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/Dark_Wood.png"));
                    }
                    else{
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/Light_Wood.png"));
                    }
                }
            }
        }
        //棋子
            //白
        for(int i=0;i<16;++i){
            if(i==0){
                teamwhite[i]->setChessPic(":/source/pictures/WoodWhite_KING.png");
                teamwhite[i]->setPixmap(QPixmap(":/source/pictures/WoodWhite_KING.png"));
            }
            else if(i==1){
                teamwhite[i]->setChessPic(":/source/pictures/WoodWhite_QUEEN.png");
                teamwhite[i]->setPixmap(QPixmap(":/source/pictures/WoodWhite_QUEEN.png"));
            }
            else if(i<4){
                teamwhite[i]->setChessPic(":/source/pictures/WoodWhite_BISHOP.png");
                teamwhite[i]->setPixmap(QPixmap(":/source/pictures/WoodWhite_BISHOP.png"));
            }
            else if(i<6){
                teamwhite[i]->setChessPic(":/source/pictures/WoodWhite_KNIGHT.png");
                teamwhite[i]->setPixmap(QPixmap(":/source/pictures/WoodWhite_KNIGHT.png"));
            }
            else if(i<8){
                teamwhite[i]->setChessPic(":/source/pictures/WoodWhite_ROOK.png");
                teamwhite[i]->setPixmap(QPixmap(":/source/pictures/WoodWhite_ROOK.png"));
            }
            else{
                teamwhite[i]->setChessPic(":/source/pictures/WoodWhite_PAWN.png");
                teamwhite[i]->setPixmap(QPixmap(":/source/pictures/WoodWhite_PAWN.png"));
            }
        }
            //黑
        for(int i=0;i<16;++i){
            if(i==0){
                teamblack[i]->setChessPic(":/source/pictures/WoodBlack_KING.png");
                teamblack[i]->setPixmap(QPixmap(":/source/pictures/WoodBlack_KING.png"));
            }
            else if(i==1){
                teamblack[i]->setChessPic(":/source/pictures/WoodBlack_QUEEN.png");
                teamblack[i]->setPixmap(QPixmap(":/source/pictures/WoodBlack_QUEEN.png"));
            }
            else if(i<4){
                teamblack[i]->setChessPic(":/source/pictures/WoodBlack_BISHOP.png");
                teamblack[i]->setPixmap(QPixmap(":/source/pictures/WoodBlack_BISHOP.png"));
            }
            else if(i<6){
                teamblack[i]->setChessPic(":/source/pictures/WoodBlack_KNIGHT.png");
                teamblack[i]->setPixmap(QPixmap(":/source/pictures/WoodBlack_KNIGHT.png"));
            }
            else if(i<8){
                teamblack[i]->setChessPic(":/source/pictures/WoodBlack_ROOK.png");
                teamblack[i]->setPixmap(QPixmap(":/source/pictures/WoodBlack_ROOK.png"));
            }
            else{
                teamblack[i]->setChessPic(":/source/pictures/WoodBlack_PAWN.png");
                teamblack[i]->setPixmap(QPixmap(":/source/pictures/WoodBlack_PAWN.png"));
            }
        }
        break;
    }
}

QVector<QVector<Lattice*>> ChessBoard::getLattice(){
    return board;
}

ChessMan* ChessBoard::getSelect(){
    return select;
}

int ChessBoard::getCurrentTeam(){
    return current_team;
}

void ChessBoard::pawnRising(int& type,QString& pic){

    QString queen_type="";
    QString bishop_type="";
    QString knight_type="";
    QString rook_type="";

    if(current_team){
        queen_type = teamwhite[1]->getChessPic();
        bishop_type = teamwhite[2]->getChessPic();
        knight_type = teamwhite[4]->getChessPic();
        rook_type = teamwhite[6]->getChessPic();
    }
    else{
        queen_type = teamblack[1]->getChessPic();
        bishop_type = teamblack[2]->getChessPic();
        knight_type = teamblack[4]->getChessPic();
        rook_type = teamblack[6]->getChessPic();
    }

    QDialog* rise_choose = new QDialog(this);
    QHBoxLayout* choice = new QHBoxLayout();
    Lattice* choice1 = new Lattice(0,0);
    Lattice* choice2 = new Lattice(0,1);
    Lattice* choice3 = new Lattice(0,2);
    Lattice* choice4 = new Lattice(0,2);
    choice1->setMaximumSize(150,150);
    choice2->setMaximumSize(150,150);
    choice3->setMaximumSize(150,150);
    choice4->setMaximumSize(150,150);
    choice1->setPixmap(QPixmap(queen_type));
    choice2->setPixmap(QPixmap(bishop_type));
    choice3->setPixmap(QPixmap(knight_type));
    choice4->setPixmap(QPixmap(rook_type));
    choice1->setScaledContents(true);
    choice2->setScaledContents(true);
    choice3->setScaledContents(true);
    choice4->setScaledContents(true);

    choice->addWidget(choice1,1);
    choice->addWidget(choice2,1);
    choice->addWidget(choice3,1);
    choice->addWidget(choice4,1);

    connect(choice1,&Lattice::leftButtonClicked,this,[=]()mutable{
        type = 1;
        pic = queen_type;
        rise_choose->close();
    });
    connect(choice2,&Lattice::leftButtonClicked,this,[=]()mutable{
        type = 2;
        pic = bishop_type;
        rise_choose->close();
    });
    connect(choice3,&Lattice::leftButtonClicked,this,[=]()mutable{
        type = 3;
        pic = knight_type;
        rise_choose->close();
    });
    connect(choice4,&Lattice::leftButtonClicked,this,[=]()mutable{
        type = 4;
        pic = rook_type;
        rise_choose->close();
    });

    rise_choose->setLayout(choice);
    rise_choose->setWindowTitle("Rising Choose");
    //放在屏幕中央
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect rect1 = screen->availableGeometry();
    rise_choose->move(rect1.size().width()/4,rect1.size().height()/4);
    rise_choose->setFixedSize(rect1.size().width()/2,rect1.size().height()/2);
    rise_choose->exec();
    rise_choose->setAttribute(Qt::WA_DeleteOnClose);//当关掉对话框时释放对象
}

void ChessBoard::recordMove(QString move){
    QPointF source;
    QPointF destination;
    ChessMan* source_man;
    record.append(move);
    if(move[0].toLatin1()=='O'){//车王易位
        if(move[4].toLatin1()=='O'){//长易位
            if(current_team){//黑棋移动
                source_man = teamblack[0];
                ChessMan* destination_man = teamblack[6];
                int king_col = source_man->getPos_col();
                int pos_row = source_man->getPos_row();
                board_layout->removeWidget(source_man);
                board_layout->removeWidget(destination_man);

                board_layout->addWidget(source_man,pos_row,king_col-2);
                source_man->setPos_col(king_col-2);

                board_layout->addWidget(destination_man,pos_row,king_col-1);
                destination_man->setPos_col(king_col-1);
            }
            else{
                source_man = teamwhite[0];
                ChessMan* destination_man = teamwhite[6];
                int king_col = source_man->getPos_col();
                int pos_row = source_man->getPos_row();
                board_layout->removeWidget(source_man);
                board_layout->removeWidget(destination_man);

                board_layout->addWidget(source_man,pos_row,king_col+2);
                source_man->setPos_col(king_col+2);

                board_layout->addWidget(destination_man,pos_row,king_col+1);
                destination_man->setPos_col(king_col+1);
            }
        }
        else{//短易位
            if(current_team){//黑棋移动
                source_man = teamblack[0];
                ChessMan* destination_man = teamblack[7];
                int king_col = source_man->getPos_col();
                int pos_row = source_man->getPos_row();
                board_layout->removeWidget(source_man);
                board_layout->removeWidget(destination_man);

                board_layout->addWidget(source_man,pos_row,king_col-2);
                source_man->setPos_col(king_col-2);

                board_layout->addWidget(destination_man,pos_row,king_col-1);
                destination_man->setPos_col(king_col-1);
            }
            else{
                source_man = teamwhite[0];
                ChessMan* destination_man = teamwhite[7];
                int king_col = source_man->getPos_col();
                int pos_row = source_man->getPos_row();
                board_layout->removeWidget(source_man);
                board_layout->removeWidget(destination_man);

                board_layout->addWidget(source_man,pos_row,king_col+2);
                source_man->setPos_col(king_col+2);

                board_layout->addWidget(destination_man,pos_row,king_col+1);
                destination_man->setPos_col(king_col+1);
            }
        }
    }
    else if(move[3].toLatin1()=='-'){
        source = QPointF('8'-move[2].toLatin1(),move[1].toLatin1()-'a');
        destination = QPointF('8'-move[5].toLatin1(),move[4].toLatin1()-'a');
        source_man = chessManAtPosition(source);

        source_man->setPosition(destination.x(),destination.y());
        board_layout->removeWidget(source_man);
        board_layout->addWidget(source_man,destination.x(),destination.y());
        source_man->hasMove();
    }
    else if(move[3].toLatin1()=='x'){
        source = QPointF('8'-move[2].toLatin1(),move[1].toLatin1()-'a');
        destination = QPointF('8'-move[5].toLatin1(),move[4].toLatin1()-'a');
        source_man = chessManAtPosition(source);
        ChessMan* destination_man = chessManAtPosition(destination);

        if(destination_man==NULL){//兵的特殊规则吃过路兵
            destination_man = chessManAtPosition(QPointF(source.x(),destination.y()));
        }

        source_man->setPosition(destination.x(),destination.y());
        board_layout->removeWidget(source_man);
        board_layout->removeWidget(destination_man);
        board_layout->addWidget(source_man,destination.x(),destination.y());
        destination_man->setIsDead(true);
        source_man->hasMove();

        death.append(destination_man);
    }

    if(move[6].toLatin1()=='='){//兵的升变
        QString rising_type="";
        int type = 1;
        if(current_team){
            if(move[7].toLatin1()=='Q'){
                rising_type = teamblack[1]->getChessPic();
                type = 1;
            }
            else if(move[7].toLatin1()=='B'){
                rising_type = teamblack[2]->getChessPic();
                type = 2;
            }
            else if(move[7].toLatin1()=='N'){
                rising_type = teamblack[4]->getChessPic();
                type = 3;
            }
            else if(move[7].toLatin1()=='R'){
                rising_type = teamblack[6]->getChessPic();
                type = 4;
            }
        }
        else{
            if(move[7].toLatin1()=='Q'){
                rising_type = teamwhite[1]->getChessPic();
                type = 1;
            }
            else if(move[7].toLatin1()=='B'){
                rising_type = teamwhite[2]->getChessPic();
                type = 2;
            }
            else if(move[7].toLatin1()=='N'){
                rising_type = teamwhite[4]->getChessPic();
                type = 3;
            }
            else if(move[7].toLatin1()=='R'){
                rising_type = teamwhite[6]->getChessPic();
                type = 4;
            }
        }

        source_man->setChessPic(rising_type);
        source_man->setPixmap(rising_type);
        source_man->setChessType(type);
    }

    if(current_team){
        current_team = 0;
    }
    else{
        current_team = 1;
    }
    emit moveSuccess();
}

void ChessBoard::initialColor(int type){
    QString man_prefix="";
    if(type==1){
        for(int i=0;i<8;++i){
            for(int j=0;j<8;++j){
                board[i][j]=new Lattice(i,j,this);
                //分行和列判断该格子是黑格子还是白格子
                if(i%2){
                    if(j%2){
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/White.png"));
                    }
                    else{
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/Black.png"));
                    }
                }
                else{
                    if(j%2){
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/Black.png"));
                    }
                    else{
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/White.png"));
                    }
                }
                board_layout->addWidget(board[i][j],i,j);
            }
        }
        man_prefix.append(":/source/pictures/Pure");
    }
    else if(type==2){
        for(int i=0;i<8;++i){
            for(int j=0;j<8;++j){
                board[i][j]=new Lattice(i,j,this);
                //分行和列判断该格子是黑格子还是白格子
                if(i%2){
                    if(j%2){
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/Melon.png"));
                    }
                    else{
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/Brown.png"));
                    }
                }
                else{
                    if(j%2){
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/Brown.png"));
                    }
                    else{
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/Melon.png"));
                    }
                }
                board_layout->addWidget(board[i][j],i,j);
            }
        }
        man_prefix.append(":/source/pictures/Brown");
    }
    else if(type==3){
        for(int i=0;i<8;++i){
            for(int j=0;j<8;++j){
                board[i][j]=new Lattice(i,j,this);
                //分行和列判断该格子是黑格子还是白格子
                if(i%2){
                    if(j%2){
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/Light_Wood.png"));
                    }
                    else{
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/Dark_Wood.png"));
                    }
                }
                else{
                    if(j%2){
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/Dark_Wood.png"));
                    }
                    else{
                        board[i][j]->setPixmap(QPixmap(":/source/pictures/Light_Wood.png"));
                    }
                }
                board_layout->addWidget(board[i][j],i,j);
            }
        }
        man_prefix.append(":/source/pictures/Wood");
    }



    //为两方创建棋子对象并且置入棋盘上
        //按白-黑的顺序，王-后-主教-骑士-车-兵的顺序创建队伍以及其相应的棋子

    for(int i=0;i<16;++i){
        if(i==0){
            teamwhite[i]=new ChessMan(ChessMan::KING,ChessMan::WHITE);
            teamwhite[i]->setChessPic(man_prefix+"White_KING.png");
            teamwhite[i]->setPixmap(QPixmap(man_prefix+"White_KING.png"));
            teamwhite[i]->setPosition(7,4);
            board_layout->addWidget(teamwhite[i],7,4);
        }
        else if(i==1){
            teamwhite[i]=new ChessMan(ChessMan::QUEEN,ChessMan::WHITE);
            teamwhite[i]->setChessPic(man_prefix+"White_QUEEN.png");
            teamwhite[i]->setPixmap(QPixmap(man_prefix+"White_QUEEN.png"));
            teamwhite[i]->setPosition(7,3);
            board_layout->addWidget(teamwhite[i],7,3);
        }
        else if(i<4){
            teamwhite[i]=new ChessMan(ChessMan::BISHOP,ChessMan::WHITE);
            teamwhite[i]->setChessPic(man_prefix+"White_BISHOP.png");
            teamwhite[i]->setPixmap(QPixmap(man_prefix+"White_BISHOP.png"));
            if(i==2){
                teamwhite[i]->setPosition(7,2);
                board_layout->addWidget(teamwhite[i],7,2);
            }
            else{
                teamwhite[i]->setPosition(7,5);
                board_layout->addWidget(teamwhite[i],7,5);
            }
        }
        else if(i<6){
            teamwhite[i]=new ChessMan(ChessMan::KNIGHT,ChessMan::WHITE);
            teamwhite[i]->setChessPic(man_prefix+"White_KNIGHT.png");
            teamwhite[i]->setPixmap(QPixmap(man_prefix+"White_KNIGHT.png"));
            if(i==4){
                teamwhite[i]->setPosition(7,1);
                board_layout->addWidget(teamwhite[i],7,1);
            }
            else{
                teamwhite[i]->setPosition(7,6);
                board_layout->addWidget(teamwhite[i],7,6);
            }
        }
        else if(i<8){
            teamwhite[i]=new ChessMan(ChessMan::ROOK,ChessMan::WHITE);
            teamwhite[i]->setChessPic(man_prefix+"White_ROOK.png");
            teamwhite[i]->setPixmap(QPixmap(man_prefix+"White_ROOK.png"));
            if(i==6){
                teamwhite[i]->setPosition(7,0);
                board_layout->addWidget(teamwhite[i],7,0);
            }
            else{
                teamwhite[i]->setPosition(7,7);
                board_layout->addWidget(teamwhite[i],7,7);
            }
        }
        else{
            teamwhite[i]=new ChessMan(ChessMan::PAWN,ChessMan::WHITE);
            teamwhite[i]->setChessPic(man_prefix+"White_PAWN.png");
            teamwhite[i]->setPixmap(QPixmap(man_prefix+"White_PAWN.png"));
            teamwhite[i]->setPosition(6,i-8);
            board_layout->addWidget(teamwhite[i],6,i-8);
        }
        teamwhite[i]->setAlignment(Qt::AlignCenter);
        teamwhite[i]->setScaledContents(true);
        teamwhite[i]->raise();
        ChessMan::CHESS_ID++;
    }

    for(int i=0;i<16;++i){
        if(i==0){
            teamblack[i]=new ChessMan(ChessMan::KING,ChessMan::BLACK);
            teamblack[i]->setChessPic(man_prefix+"Black_KING.png");
            teamblack[i]->setPixmap(QPixmap(man_prefix+"Black_KING.png"));
            teamblack[i]->setPosition(0,4);
            board_layout->addWidget(teamblack[i],0,4);
        }
        else if(i==1){
            teamblack[i]=new ChessMan(ChessMan::QUEEN,ChessMan::BLACK);
            teamblack[i]->setChessPic(man_prefix+"Black_QUEEN.png");
            teamblack[i]->setPixmap(QPixmap(man_prefix+"Black_QUEEN.png"));
            teamblack[i]->setPosition(0,3);
            board_layout->addWidget(teamblack[i],0,3);
        }
        else if(i<4){
            teamblack[i]=new ChessMan(ChessMan::BISHOP,ChessMan::BLACK);
            teamblack[i]->setChessPic(man_prefix+"Black_BISHOP.png");
            teamblack[i]->setPixmap(QPixmap(man_prefix+"Black_BISHOP.png"));
            if(i==2){
                teamblack[i]->setPosition(0,2);
                board_layout->addWidget(teamblack[i],0,2);
            }
            else{
                teamblack[i]->setPosition(0,5);
                board_layout->addWidget(teamblack[i],0,5);
            }
        }
        else if(i<6){
            teamblack[i]=new ChessMan(ChessMan::KNIGHT,ChessMan::BLACK);
            teamblack[i]->setChessPic(man_prefix+"Black_KNIGHT.png");
            teamblack[i]->setPixmap(QPixmap(man_prefix+"Black_KNIGHT.png"));
            if(i==4){
                teamblack[i]->setPosition(0,1);
                board_layout->addWidget(teamblack[i],0,1);
            }
            else{
                teamblack[i]->setPosition(0,6);
                board_layout->addWidget(teamblack[i],0,6);
            }
        }
        else if(i<8){
            teamblack[i]=new ChessMan(ChessMan::ROOK,ChessMan::BLACK);
            teamblack[i]->setChessPic(man_prefix+"Black_ROOK.png");
            teamblack[i]->setPixmap(QPixmap(man_prefix+"Black_ROOK.png"));
            if(i==6){
                teamblack[i]->setPosition(0,0);
                board_layout->addWidget(teamblack[i],0,0);
            }
            else{
                teamblack[i]->setPosition(0,7);
                board_layout->addWidget(teamblack[i],0,7);
            }
        }
        else{
            teamblack[i]=new ChessMan(ChessMan::PAWN,ChessMan::BLACK);
            teamblack[i]->setChessPic(man_prefix+"Black_PAWN.png");
            teamblack[i]->setPixmap(QPixmap(man_prefix+"Black_PAWN.png"));
            teamblack[i]->setPosition(1,i-8);
            board_layout->addWidget(teamblack[i],1,i-8);
        }
        teamblack[i]->setAlignment(Qt::AlignCenter);
        teamblack[i]->setScaledContents(true);
        teamblack[i]->raise();
        ChessMan::CHESS_ID++;
    }

}
