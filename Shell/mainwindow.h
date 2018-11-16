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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
   void addInputLine();
   void commands();
   void saveFile();
private:
    Ui::MainWindow *ui;
    int contLine = 1;
    QLineEdit* currentInput = nullptr;
    QTextEdit* currentVim = nullptr;
    QFile* file =  new QFile();
    QString manMenu(QString);
    QDir* path = new QDir();
    bool eventFilter(QObject *object, QEvent *event);
};

#endif // MAINWINDOW_H
