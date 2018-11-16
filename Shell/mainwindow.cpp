#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    path->setPath(path->root().path());
    addInputLine();
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
                    currentVim->setDisabled(1);
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
                 ;
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

    }else if(command[0] == "chmod"){
        if(command.size() > 1){
           file->setFileName(path->path()+"/"+command[1]);
           if(file->exists()){
               lineNum = new QLabel(QString::number(contLine)+"     >> Los permisos han cambiado";
           }
           }else{
               lineNum = new QLabel(QString::number(contLine)+"     >> El archivo no existe " + path->path()+"/"+command[1]);
           }
        }else{
            lineNum = new QLabel(QString::number(contLine)+"     >> Introduzca un archivo txt");
        }
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
