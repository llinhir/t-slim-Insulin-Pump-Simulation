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
            {
                switchPage(INSULIN_PAGE); // TODO: also call the set basal rate function
            });
    connect(ui->historyButton, &QPushButton::clicked, this, [this]()
            { switchPage(HISTORY_PAGE); });

    // connecting slots for the back buttons
    connect(ui->optionsBack, &QPushButton::clicked, this, [this]()
            { backPage(); });
    connect(ui->createProfileBack, &QPushButton::clicked, this, [this]()
            { backPage(); });
    connect(ui->bolusBack, &QPushButton::clicked, this, [this]()
            { backPage(); });
    connect(ui->selectProfileBack, &QPushButton::clicked, this, [this]()
            { backPage(); });
    connect(ui->editProfileBack, &QPushButton::clicked, this, [this]()
            { backPage(); });
    connect(ui->saveProfileBack, &QPushButton::clicked, this, [this]()
            { backPage(); });
    connect(ui->createProfileBack_2, &QPushButton::clicked, this, [this]()
            { backPage(); });
    connect(ui->graphBack, &QPushButton::clicked, this, [this]()
            { backPage(); });
    connect(ui->insulinBack, &QPushButton::clicked, this, [this]()
            { backPage(); });
    connect(ui->historyBack, &QPushButton::clicked, this, [this]()
            { backPage(); });

    // THIS IS ONLY FOR TESTING AND CONCEPT, please dont remove until done -_-
    glucoseSeries = new QSplineSeries(); // Curved line
    mach->setGlucoseSeries(glucoseSeries);
    QValueAxis *xAxis = new QValueAxis();
    QValueAxis *yAxis = new QValueAxis();

    // Y-axis: fixed 0 to 15
    yAxis->setRange(0, 15);
    yAxis->setTitleText("Glucose (mmol/L)");
    yAxis->setTickCount(6); // Optional

    // X-axis: start from 0, will grow later
    xAxis->setRange(0, 72); // Initial 10 steps shown

    //glucoseSeries->append(0, 0); // the 4.2 is hard coded

    QPen pen(Qt::blue);
    pen.setStyle(Qt::DotLine);
    pen.setWidth(2);
    glucoseSeries->setPen(pen);

    QChart *glucoseChart = new QChart();
    glucoseChart->addSeries(glucoseSeries);
    glucoseChart->setTitle("Blood Glucose Monitoring");
    glucoseChart->addAxis(xAxis, Qt::AlignBottom);
    glucoseChart->addAxis(yAxis, Qt::AlignLeft);
    glucoseSeries->attachAxis(xAxis);
    glucoseSeries->attachAxis(yAxis);

    QChartView *chartView = new QChartView(glucoseChart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Set the chart view in the UI
    ui->chartLayout->addWidget(chartView);

    // For the Immediate Bolus Fraction spinbox
    ui->immediateFractionBox->setSingleStep(10);
    ui->immediateFractionBox->setRange(0, 100);
    ui->immediateFractionBox->setSuffix("%");
    ui->immediateFractionBox->setValue(60);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::createGraph(){
//    glucoseSeries = new QSplineSeries();
//    glucoseSeries->append(0, GlucoseVector[0]);
//}

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
        prevPage = ui->stackedWidget->currentIndex();
        ui->stackedWidget->setCurrentIndex(pageName);
    }
}

void MainWindow::backPage()
{
    int currentPage = ui->stackedWidget->currentIndex();

    // If a specific previous page is saved from before
    if (prevPage != -1)
    {
        ui->stackedWidget->setCurrentIndex(prevPage);
        prevPage = -1;
        return;
    }

    // Otherwise, try to get the page from the map
    if (prevPageMap.contains(static_cast<PageIndex>(currentPage)))
    {
        int previous = prevPageMap[static_cast<PageIndex>(currentPage)];
        ui->stackedWidget->setCurrentIndex(previous);
    }
    else
    {
        qDebug() << "couldn't go back a page" << currentPage;
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
    prevPageMap[PROFILES_PAGE] = OPTIONS_PAGE;
    prevPageMap[INSULIN_PAGE] = OPTIONS_PAGE;
    prevPageMap[GRAPH_PAGE] = HOME_PAGE;
    prevPageMap[HISTORY_PAGE] = OPTIONS_PAGE;
}

// void MainWindow::charge what is this for???
