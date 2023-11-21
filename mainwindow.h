#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QFileDialog>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setBackgroundPixmap(const QPixmap& pixmap);
    void paintEvent(QPaintEvent* event)override;
    void resizeEvent(QResizeEvent* event)override;
    void colorChoose(int& choose_type);
    void handleItemDoubleClick(QListWidgetItem *item) ;

private:
    Ui::MainWindow *ui;
    QPixmap background_pixmap;
    QListWidget* files;
};
#endif // MAINWINDOW_H
