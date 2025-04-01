#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m(new machine())
{
    // initializing values and classes
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(LOGIN_PAGE);
    profile = new Profile(1, 1, 1, 1, "Bean senjamin", "terrorism");
    m = new machine();

    // connecting slots
    connect(ui->submitPassword, &QPushButton::clicked, this, &MainWindow::submitPassword);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::submitPassword(){
    QString password = ui->passwordInput->text(); // Assuming "passwordInput" is your QLineEdit
    if(password.toStdString() == profile->getPassword()){
        qInfo("Correct Password");
        ui->stackedWidget->setCurrentIndex(HOME_PAGE);
        return true;
    }else{
        qInfo("Incorrect Password");
        return false;
    }

}
