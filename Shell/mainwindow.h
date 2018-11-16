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
private:
    Ui::MainWindow *ui;
    int contLine = 1;
    QLineEdit* currentInput = nullptr;
    QString manMenu(QString);
    QDir* path = new QDir();
};

#endif // MAINWINDOW_H
