#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // initializing values and classes
    ui->setupUi(this);
    ui->simulation->setCurrentIndex(OFF);
    ui->stackedWidget->setCurrentIndex(LOGIN_PAGE); // This is cucrrently uncommented for testing (so you can test which qStackedWidget page youre working on)
    sim = new Simulation(ui);
    mach = sim->getMachine();
    tm *timeInfo = mach->getCurrentTimeStruct();
    ui->dateTimeEdit->setDateTime(QDateTime::fromSecsSinceEpoch(mktime(timeInfo)));

    // mapping previous pages for the Back button
    setPrevPages();

    // connecting slots for when OFF
    connect(ui->powerOnButton, &QPushButton::clicked, this, [this]()
            { turnOnOff(ON); });
    connect(ui->chargeButton, &QPushButton::clicked, this, [this]()
            { chargeBattery(); });

    // connecting slots for when ON
    connect(ui->submitPassword, &QPushButton::clicked, this, &MainWindow::submitPassword);
    connect(ui->homeButton, &QPushButton::clicked, this, [this]()
            { switchPage(HOME_PAGE); });
    connect(ui->optionsButton, &QPushButton::clicked, this, [this]()
            { switchPage(OPTIONS_PAGE); });
    connect(ui->bolusButton, &QPushButton::clicked, this, [this]()
            { switchPage(BOLUS_PAGE); });
    connect(ui->createProfileButton, &QPushButton::clicked, this, [this]()
            { switchPage(CREATE_PROFILE_PAGE); });
    connect(ui->createProfileSaveButton, &QPushButton::clicked, this, [this]()
            { switchPage(CREATE_PROFILE_PAGE); }); // TODO: Map this to the options.createProfile() function
    connect(ui->profilesButton, &QPushButton::clicked, this, [this]()
            { switchPage(PROFILES_PAGE); });
    connect(ui->editProfileButton, &QPushButton::clicked, this, [this]()
            { switchPage(EDIT_PROFILE_PAGE); });

    // connecting slots for the back buttons
    connect(ui->optionsBack, &QPushButton::clicked, this, [this]()
            { switchPage(prevPageMap[OPTIONS_PAGE]); });
    connect(ui->createProfileBack, &QPushButton::clicked, this, [this]()
            { switchPage(prevPageMap[CREATE_PROFILE_PAGE]); });
    connect(ui->bolusBack, &QPushButton::clicked, this, [this]()
            { switchPage(prevPageMap[BOLUS_PAGE]); });
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::submitPassword()
{
    cout << "Submit Password" << endl;
    QString password = ui->passwordInput->text(); // Assuming "passwordInput" is your QLineEdit
    if (sim->enterPassword(password.toStdString()))
    {
        qInfo("Correct Password");
        mach->setIsLoggedIn(true);
        switchPage(HOME_PAGE);
        return true;
    }
    else
    {
        qInfo("Incorrect Password");
        return false;
    }
}

void MainWindow::turnOnOff(PageIndex pageName)
{
    if (mach->getCurrentBatteryLevel() > 0)
    {
        ui->simulation->setCurrentIndex(pageName);
    }
}

void MainWindow::switchPage(PageIndex pageName)
{
    if (mach->getIsLoggedIn())
    {
        ui->stackedWidget->setCurrentIndex(pageName);
    }
}

// this one currently
void MainWindow::chargeBattery()
{
    ui->batteryLabel->setText("Battery ⚡");
    ui->batteryBar->setStyleSheet("QProgressBar::chunk {background-color: green}");
}

void MainWindow::setPrevPages()
{
    prevPageMap[LOGIN_PAGE] = LOGIN_PAGE;
    prevPageMap[HOME_PAGE] = LOGIN_PAGE;
    prevPageMap[OPTIONS_PAGE] = HOME_PAGE;
    prevPageMap[BOLUS_PAGE] = HOME_PAGE;
    prevPageMap[CREATE_PROFILE_PAGE] = OPTIONS_PAGE;
}

// void MainWindow::charge what is this for???
