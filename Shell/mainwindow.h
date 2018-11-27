#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QLayout>
#include <QLabel>
#include <QDir>
#include <QDate>
#include <QTextStream>
#include <QTextEdit>
#include <QKeyEvent>
#include <QTreeWidgetItem>
#include <QGraphicsItem>
#include <QToolButton>
#include <QGraphicsGridLayout>
#include <QGraphicsLayoutItem>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void clicked(QString*);
public slots:
   void addInputLine();
   void commands();
   void redireccionarPath();
   void redireccionarPathGraphics(QString newPath);
private:
    Ui::MainWindow *ui;
    int contLine = 1;
    QLineEdit* currentInput = nullptr;
    QTextEdit* currentVim = nullptr;
    QFile* file =  new QFile();
    QString manMenu(QString);
    QDir* path = new QDir();
    bool eventFilter(QObject *object, QEvent *event);
    //void updateTreeView();
    QTreeWidgetItem* addTreeChild(QTreeWidgetItem *parent,QString name);
    void updateTreeView(QDir* dir,QTreeWidgetItem* root,int cont);
    void updateGraphicView();
};

#endif // MAINWINDOW_H
