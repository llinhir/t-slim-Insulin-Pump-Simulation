#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QtCharts>

QT_CHARTS_USE_NAMESPACE

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

    // set the time
    tm *timeInfo = mach->getCurrentTimeStruct();
    QString timeString = QString::asprintf("%02d:%02d", timeInfo->tm_hour, timeInfo->tm_min);
    if (timeInfo->tm_hour > 12)
    {
        timeString += " PM";
        timeString.replace(0, 2, QString::number(timeInfo->tm_hour - 12));
    }
    else
    {
        timeString += " AM";
        if (timeInfo->tm_hour == 0)
        {
            timeString.replace(0, 2, "12");
        }
    }
    ui->timeLabel->setText(timeString);

    // mapping previous pages for the Back button
    setPrevPages();

    // connecting slots for when OFF
    connect(ui->powerOnButton, &QPushButton::clicked, this, [this]()
            { turnOnOff(ON); });
    connect(ui->powerOffButton, &QPushButton::clicked, this, [this]()
            { turnOnOff(OFF); });

    connect(ui->chargeButton, &QPushButton::clicked, this, [this]()
            { chargeBattery(); });
    connect(ui->refillInsulinButton, &QPushButton::clicked, this, [this]()
            { mach->refillInsulin(); });

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
    connect(ui->viewGraphButton, &QPushButton::clicked, this, [this]()
            { switchPage(GRAPH_PAGE); });
    connect(ui->insulinButton, &QPushButton::clicked, this, [this]()
            { switchPage(INSULIN_PAGE); });

    // connecting slots for the back buttons
    connect(ui->optionsBack, &QPushButton::clicked, this, [this]()
            { switchPage(prevPageMap[OPTIONS_PAGE]); });
    connect(ui->createProfileBack, &QPushButton::clicked, this, [this]()
            { switchPage(prevPageMap[CREATE_PROFILE_PAGE]); });
    connect(ui->bolusBack, &QPushButton::clicked, this, [this]()
            { switchPage(prevPageMap[BOLUS_PAGE]); });
    connect(ui->selectProfileBack, &QPushButton::clicked, this, [this]()
            { switchPage(prevPageMap[SELECT_ACTIVE_PROFILE_PAGE]); });
    connect(ui->editProfileBack, &QPushButton::clicked, this, [this]()
            { switchPage(prevPageMap[EDIT_PROFILE_PAGE]); });
    connect(ui->saveProfileBack, &QPushButton::clicked, this, [this]()
            { switchPage(prevPageMap[EDIT_SPECIFIC_PROFILE_PAGE]); });

    // THIS IS ONLY FOR TESTING AND CONCEPT, please dont remove until done -_-
    QLineSeries *batterySeries = new QLineSeries();
    *batterySeries << QPointF(0, 100) << QPointF(1, 40) << QPointF(2, 80) << QPointF(3, 50); // Example data

    QChart *batteryChart = new QChart();
    batteryChart->addSeries(batterySeries);
    batteryChart->createDefaultAxes();
    batteryChart->setTitle("Fake data for graph testing");

    QChartView *chartView = new QChartView(batteryChart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Set the chart view in the UI (for example, add to the layout)
    ui->chartLayout->addWidget(chartView);
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
    if (pageName == OFF)
    {
        mach->powerOff();
    }
    else
    {
        mach->powerOn();
    }
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
    bool isCharging = mach->getIsCharging();

    if (isCharging)
    {
        ui->chargeButton->setText("Charge");
        mach->setIsCharging(false);
        ui->batteryLabel->setText("Battery: Not Charging");
    }
    else
    {
        ui->chargeButton->setText("Stop Charging");
        mach->setIsCharging(true);
        ui->batteryLabel->setText("Battery ⚡: Charging");
    }

    // ui->batteryLabel->setText("Battery ⚡");
    // ui->batteryBar->setStyleSheet("QProgressBar::chunk {background-color: green}");
}

void MainWindow::setPrevPages()
{
    prevPageMap[LOGIN_PAGE] = LOGIN_PAGE;
    prevPageMap[HOME_PAGE] = LOGIN_PAGE;
    prevPageMap[OPTIONS_PAGE] = HOME_PAGE;
    prevPageMap[BOLUS_PAGE] = HOME_PAGE;
    prevPageMap[CREATE_PROFILE_PAGE] = PROFILES_PAGE;
    prevPageMap[EDIT_PROFILE_PAGE] = PROFILES_PAGE;
    prevPageMap[EDIT_SPECIFIC_PROFILE_PAGE] = EDIT_PROFILE_PAGE;
    prevPageMap[SELECT_ACTIVE_PROFILE_PAGE] = BOLUS_PAGE;
}

// void MainWindow::charge what is this for???
