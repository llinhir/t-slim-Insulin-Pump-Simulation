#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGlobal>

#include "machine.h" //testing
#include "profile.h" //testing
#include "simulation.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setPrevPages();

private:
    Ui::MainWindow *ui;
    Simulation *sim; // testing
    bool isCharging; // Remove this later
    bool isLoggedIn;

    // page names for swapping between pages
    enum PageIndex
    {
        OFF = 0,
        ON = 1,
        LOGIN_PAGE = 0,
        HOME_PAGE = 1,
        CREATE_PROFILE_PAGE = 2,
        OPTIONS_PAGE = 3,
        BOLUS_PAGE = 4,
    };

    // page mappings for what their previous page was
    QMap<PageIndex, PageIndex> prevPageMap;

private slots:
    bool submitPassword();
    void switchPage(PageIndex pageName);
    void turnOnOff(PageIndex pageName);
    void chargeBattery();
};
#endif // MAINWINDOW_H
