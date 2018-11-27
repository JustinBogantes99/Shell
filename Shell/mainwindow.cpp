#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <iostream>
#include <QDesktopWidget>
#include <QPushButton>
#include <QSignalMapper>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(QDesktopWidget().availableGeometry(this).size() * 1);
    path->setPath(path->root().path());
    ui->treeWidget->setColumnCount(1);
    addInputLine();
    QTreeWidgetItem* rootItem = new QTreeWidgetItem();
    rootItem->setText(0,"C:");
    ui->treeWidget->addTopLevelItem(rootItem);
    updateTreeView(path,rootItem,0);
    updateGraphicView();
    connect(ui->treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(redireccionarPath()));

}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == currentVim && event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Insert)
            if (file->open(QIODevice::ReadWrite)) {
                    QTextStream stream(file);
                    stream << currentVim->toPlainText() << endl;
                    file->close();
                    currentVim->setReadOnly(1);
            }
    }
    else
        return false;
}

void MainWindow::addInputLine(){
    if(currentInput != nullptr){
        disconnect(currentInput);
        currentInput->setDisabled(1);

    }
    QListWidgetItem* itemN = new QListWidgetItem();
    QWidget* widget = new QWidget();
    QLabel* lineNum = new QLabel(QString::number(contLine)+"      " + path->path() +" >>");
    contLine++;
    QLineEdit* line = new QLineEdit();
    line->setStyleSheet("QLineEdit { background: rgb(0, 0, 0); selection-background-color: rgb(255, 255, 255);color: rgb(255, 255, 255)}");
    line->setFrame(0);

    currentInput = line;
    QHBoxLayout* widgetLayout = new QHBoxLayout();
    widgetLayout->addWidget(lineNum);
    widgetLayout->addWidget(line);
    widget->setLayout(widgetLayout);
    itemN->setSizeHint(widget->sizeHint());
    ui->Shell->addItem(itemN);
    ui->Shell->setItemWidget(itemN, widget);
    connect(currentInput, SIGNAL(returnPressed()),SLOT(commands()));
}

void MainWindow::updateGraphicView(){
    QGraphicsScene*  scene =  new QGraphicsScene();
    QStringList files = path->entryList();
    for(int i = 2; i < files.size();i++){

        QToolButton* item = new QToolButton();
        item->setGeometry(((i-2)%5)*120,((i-2)/5)*85,110,85);
        item->setIcon(QIcon(":/resources/images/carpeta.png"));
        item->setIconSize(QSize(100,60));
        item->setToolButtonStyle((Qt::ToolButtonTextUnderIcon));
        item->setStyleSheet("QToolButton { background-color : white;border-radius: 0; }");
        item->setText(files[i]);


       /* QGraphicsPixmapItem* item = new QGraphicsPixmapItem();
        QPixmap pixmap = QPixmap(":/resources/images/carpeta.png");
        item->setPixmap(pixmap);
        item->setScale(1);

        QGraphicsTextItem* item2 = new QGraphicsTextItem();
        item2->setPlainText(files[i]);
        QPointF point = QPointF(((i-2)%5)*110,((i-2)/5)*85);
        QPointF pointItem = item->mapFromScene(point);
        item->setPos(pointItem);

        QPointF point2 = QPointF(((i-2)%5)*110,((i-2)/5)*85);
        QPointF pointItem2 = item2->mapFromScene(point2);
        item2->setPos(pointItem2);*/
        scene->addWidget(item);
       // scene->addItem(item2);
        QSignalMapper* sigmapper = new QSignalMapper(this);
        connect (item, SIGNAL(clicked()), sigmapper, SLOT(map())) ;
        sigmapper -> setMapping (item, files[i]) ;
        connect(sigmapper, SIGNAL(mapped(QString)),this,SLOT(redireccionarPathGraphics(QString)));



    }
    ui->graphicsView->setScene(scene);
}

void MainWindow::redireccionarPathGraphics(QString Newpath){
    path->cd(Newpath);
    ui->treeWidget->clear();
    QTreeWidgetItem* rootItem = new QTreeWidgetItem();
    rootItem->setText(0,path->path());
    ui->treeWidget->addTopLevelItem(rootItem);
    updateTreeView(path,rootItem,0);
    addInputLine();
    updateGraphicView();

}

void MainWindow::updateTreeView(QDir* dir,QTreeWidgetItem* root,int cont){
    cont++;
    QStringList files = dir->entryList();
    if(cont <= 2){
        if(!dir->isEmpty() || files.size() >= 2 || cont >= 2){
            for (int i = 2; i < files.size(); ++i){
                QTreeWidgetItem* rootItem = addTreeChild(root,files[i]);
                QDir* dirHijo =  new QDir();
                if(dir->path().back() == "/")
                    dirHijo->setPath(dir->path()+files[i]);
                else{
                    dirHijo->setPath(dir->path()+"/"+files[i]);
                }
                if(dirHijo->exists() && !dirHijo->isEmpty()){
                    updateTreeView(dirHijo,rootItem,cont);
                }
            }
        }
    }
}

void MainWindow::redireccionarPath(){
    QTreeWidgetItem* current = ui->treeWidget->currentItem();
    QString txt =  current->text(0);
    while(current->parent()){
        current = current->parent();
        txt = current->text(0) + "/" + txt;
    }
    path->cd(txt);
    ui->treeWidget->clear();
    QTreeWidgetItem* rootItem = new QTreeWidgetItem();
    rootItem->setText(0,path->path());
    ui->treeWidget->addTopLevelItem(rootItem);
    updateTreeView(path,rootItem,0);
    addInputLine();
    updateGraphicView();

}

/*void MainWindow::updateTreeView(){
    QStringList files = path->entryList();
    QList<QTreeWidgetItem *> items;
    for (int i = 0; i < files.size(); ++i){
        items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(files[i])));
        QDir* pathInterno = new QDir();
        pathInterno->setPath(path->path()+"/"+files[i]);
        QStringList filesInternos = pathInterno->entryList();
        QList<QTreeWidgetItem *> itemsInternos;
        for(int e = 0; e < filesInternos.size();e++)
            addTreeChild(items[i],filesInternos[e]);
    }
    ui->treeWidget->insertTopLevelItems(0, items);

}*/

QTreeWidgetItem* MainWindow::addTreeChild(QTreeWidgetItem *parent,QString name)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();

    // QTreeWidgetItem::setText(int column, const QString & text)
    treeItem->setText(0, name);

    // QTreeWidgetItem::addChild(QTreeWidgetItem * child)
    parent->addChild(treeItem);
    return treeItem;
}

QString MainWindow::manMenu(QString command){
    if(command == "man"){
        return  "Proporciona información sobre los comandos u otros programas del entorno Linux";
    }else if(command == "ls"){
        return "Lista el contenido de un directorio.";
    }else if(command == "cd"){
        return "Cambia el directorio corriente (Change Directory) en que nos encontramos posicionados.""\n"
               " Si se utiliza sin argumentos, cambia al directorio raíz del del usuario.";
    }else if(command == "mkdir"){
        return "Crea entradas de directorios.";
    }else if(command == "rm"){
        return "Elimina archivos o directorios.";
    }else if(command == "vim"){
        return "Visualizar y editar archivos";
    }else if(command == "passwd"){
        return "Cambia la contraseña del usuario. Las contraseñas "
               "deben cumplir las condiciones que se indican en el mismo o en las carteleras";
    }else if(command == "cp"){
        return "Copiar archivos o directorios. Para copiar un directorio se debe agregar el modificador '-r'.";
    } else if(command == "mv"){
        return "Mueve archivos o directorios.";
    }else if(command == "exit"){
        return "Cierra la shell actual.";
    }else if(command == "date"){
        return "Indica la fecha y hora.";
    }else if(command == "clear"){
        return "Limpia la pantalla de la terminal";
    }else if(command == "cat"){
        return "Visualiza y concatena ficheros.";
    }else if(command == "chmod"){
        return "Cambia los permisos de lectura/escritura/ejecución de ficheros/directorios, " "\n"
               "para poder modificar los permisos de un fichero debes ser propietario del mismo o root";
    }else{
        return "No existe el commando " + command ;
    }
}

void MainWindow::commands(){
    QString line = currentInput->text();
    QStringList command = line.split(' ');
    QWidget* widget = new QWidget();
    QHBoxLayout* widgetLayout = new QHBoxLayout();
    QListWidgetItem* itemN = new QListWidgetItem();
    QLabel* lineNum;
    if(command[0] == "man"){
        if(command.size() > 1){
            lineNum = new QLabel(QString::number(contLine)+"     >>" + manMenu(command[1]));

        } else {
            lineNum = new QLabel(QString::number(contLine)+"     >> El comando man necesita el otro comando del cual quiere información");

        }
    } else if(command[0] == "cd"){
        if(command.size() > 1){
            if(path->cd(command[1])){
                lineNum = new QLabel(QString::number(contLine)+"     >> Cambio al path " + command[1]);
                ui->treeWidget->clear();
                QTreeWidgetItem* rootItem = new QTreeWidgetItem();
                rootItem->setText(0,path->path());
                ui->treeWidget->addTopLevelItem(rootItem);
                updateTreeView(path,rootItem,0);
                updateGraphicView();
            }else{
                lineNum = new QLabel(QString::number(contLine)+"     >> El path " + command[1] + " no existe");
            }
        }else{
            lineNum = new QLabel(QString::number(contLine)+"     >> Por favor ingrese un path valido");
        }
    } else if(command[0] == "ld"){
        QStringList files = path->entryList();
        QString allFiles;
        for(int i=0; i < files.size();i++){
            allFiles = allFiles + files[i] + "              ";
            if(i%4 == 0){
                allFiles = allFiles + "\n"+"\n";
            }
        }
        lineNum = new QLabel(QString::number(contLine)+ "     >> " + allFiles);
    }else if(command[0] == "mkdir"){
        if(command.size() > 1){
            if(path->mkdir(command[1])){
                lineNum = new QLabel(QString::number(contLine)+"     >> creo el directorio " + command[1]);
                path->cd(path->path());
            }else{
                lineNum = new QLabel(QString::number(contLine)+"     >> El directorio " + command[1] + " ya existe");
            }
        }else{
            lineNum = new QLabel(QString::number(contLine)+"     >> Por favor ingrese un directorio valido");
        }
    }else if(command[0] == "rm"){
        if(command.size() > 1){
            if(path->remove(command[1])){
                lineNum = new QLabel(QString::number(contLine)+"     >> se elimino el archivo/directorio " + command[1]);
                path->cd(path->path());
            }else{
                if(path->rmdir(command[1])){
                    lineNum = new QLabel(QString::number(contLine)+"     >> se elimino el archivo/directorio " + command[1]);
                    path->cd(path->path());
                }else
                    lineNum = new QLabel(QString::number(contLine)+"     >> No se pudo eliminar el archivo/directorio " + command[1]);
            }
        }else{
            lineNum = new QLabel(QString::number(contLine)+"     >> Por favor ingrese un archivo/directorio valido");
        }
    }else if(command[0] == "date"){
        QDate* date = new QDate();
        lineNum = new QLabel(QString::number(contLine)+"     >> Fecha:  " + date->currentDate().toString());
    }else if(command[0] == "vim"){
        if(command.size() > 1){
           file->setFileName(path->path()+"/"+command[1]);
           if(file->exists()){
               if(!file->open(QIODevice::ReadWrite)) {
                   lineNum = new QLabel(QString::number(contLine)+"     >> No se pudo abrir el archivo ");
               }else{
                   QTextStream in(file);
                   QStringList* fields =  new QStringList();
                   QString txt = "";
                   while(!in.atEnd()) {
                       QString line = in.readLine();
                       fields->append(line);
                       txt = txt + line + "\n";
                   }
                   currentVim =  new QTextEdit();
                   currentVim->setText(txt);
                   widgetLayout->addWidget(currentVim);
                   currentVim->installEventFilter(this);;
                   file->close();
                   lineNum = new QLabel();
               }
           }else{
               lineNum = new QLabel(QString::number(contLine)+"     >> El archivo no existe " + path->path()+"/"+command[1]);
           }
        }else{
            lineNum = new QLabel(QString::number(contLine)+"     >> Introduzca un archivo txt");
        }
    }else if(command[0] == "cat"){
        if(command.size() >2){
            file->setFileName(path->path()+"/"+command[1]);
            QFile* file2 =  new QFile();
            file2->setFileName(path->path()+"/"+command[2]);
            if(file->exists() && file2->exists()){
                if(!file->open(QIODevice::ReadWrite)) {
                    lineNum = new QLabel(QString::number(contLine)+"     >> No se pudo abrir el archivo ");
                }else{
                    QTextStream in(file);
                    QStringList* fields =  new QStringList();
                    QString txt = "";
                    while(!in.atEnd()) {
                        QString line = in.readLine();
                        fields->append(line);
                        txt = txt + line + "\n";
                    }
                    if (file2->open(QIODevice::ReadWrite)) {

                        QTextStream in(file2);
                        QStringList* fields =  new QStringList();
                        while(!in.atEnd()) {
                            QString line = in.readLine();
                            fields->append(line);
                            txt = txt + line + "\n";
                        }
                    }
                    QTextStream stream(file);
                    stream << txt << endl;
                    file->close();
                }
            }
        }else if(command.size() > 1){
           file->setFileName(path->path()+"/"+command[1]);
           if(file->exists()){
               if(!file->open(QIODevice::ReadWrite)) {
                   lineNum = new QLabel(QString::number(contLine)+"     >> No se pudo abrir el archivo ");
               }else{
                   QTextStream in(file);
                   QStringList* fields =  new QStringList();
                   QString txt = "";
                   while(!in.atEnd()) {
                       QString line = in.readLine();
                       fields->append(line);
                       txt = txt + line + "\n";
                   }
                   currentVim =  new QTextEdit();
                   currentVim->setText(txt);
                   widgetLayout->addWidget(currentVim);
                   currentVim->setReadOnly(1);
                   file->close();
                   lineNum = new QLabel();
               }
           }else{
               lineNum = new QLabel(QString::number(contLine)+"     >> El archivo no existe " + path->path()+"/"+command[1]);
           }
        }else{
            lineNum = new QLabel(QString::number(contLine)+"     >> Introduzca un archivo txt");
        }
    }else if(command[0] == "cp"){
        if(command.size() > 2){
            if(QFile::exists(command[1])){
                if(QFile::copy(command[1],command[2])){
                    lineNum = new QLabel(QString::number(contLine)+"     >> El archivo ha sido copiado");
                }else{
                    lineNum = new QLabel(QString::number(contLine)+"     >> El archivo no ha podido ser copiado");
                }
            }else{
               lineNum = new QLabel(QString::number(contLine)+"     >> El archivo ingresado no existe");
            }
        }else{
            lineNum = new QLabel(QString::number(contLine)+"     >> Por favor ingrese el origen y el desino del archivo a copiar");
        }
    }else if(command[0] == "chmod"){
    if(command.size() > 4){
       file->setFileName(path->path()+"/"+command[4]);
       if(file->exists()){
           QString userP = command[1].split('=')[1];
           QString groupP = command[2].split("=")[1];
           QString otherP = command[3].split("=")[1];
           if(userP.contains("r"))
             file->setPermissions(QFileDevice::ReadUser);
           if(userP.contains("w"))
               file->setPermissions(QFileDevice::WriteUser);
           if(userP.contains("x"))
               file->setPermissions(QFileDevice::ExeUser);
           if(groupP.contains("r"))
             file->setPermissions(QFileDevice::ReadGroup);
           if(groupP.contains("w"))
               file->setPermissions(QFileDevice::WriteGroup);
           if(groupP.contains("x"))
               file->setPermissions(QFileDevice::ExeGroup);
           if(otherP.contains("r"))
             file->setPermissions(QFileDevice::ReadOther);
           if(otherP.contains("w"))
               file->setPermissions(QFileDevice::WriteOther);
           if(otherP.contains("x"))
               file->setPermissions(QFileDevice::ExeOther);
       }else{
           lineNum = new QLabel(QString::number(contLine)+"     >> El archivo no existe " + path->path()+"/"+command[1]);
       }
     }else{
        lineNum = new QLabel(QString::number(contLine)+"     >> Introduzca un archivo txt");
     }
    }else if(command[0] == "mv"){
        if(command.size() > 2){
            if(QFile::exists(command[1])){
                if(QFile::rename(command[1],command[2])){
                    lineNum = new QLabel(QString::number(contLine)+"     >> El archivo ha sido movido");
                    //mv C:/boi2/meVoyAMover.txt C:/boi/
                }else{
                    lineNum = new QLabel(QString::number(contLine)+"     >> El archivo no ha podido ser movido");
                }
            }else{
               lineNum = new QLabel(QString::number(contLine)+"     >> El archivo ingresado no existe");
            }
        }else{
            lineNum = new QLabel(QString::number(contLine)+"     >> Por favor ingrese el origen y el desino del archivo a mover");
        }
    }else if(command[0] == "clear"){
        ui->Shell->clear();
        contLine = 0;
        lineNum = new QLabel();
    }else if(command[0] == "exit"){
       QCoreApplication::exit();
    }else{
        lineNum = new QLabel(QString::number(contLine)+"     >> No existe el comando " + command[0]);
    }
    contLine++;
    widgetLayout->addWidget(lineNum);
    widget->setLayout(widgetLayout);
    itemN->setSizeHint(widget->sizeHint());
    ui->Shell->addItem(itemN);
    ui->Shell->setItemWidget(itemN, widget);
    addInputLine();

}

MainWindow::~MainWindow()
{
    delete ui;
}
