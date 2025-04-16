#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGlobal>

#include "machine.h" //testing
#include "profile.h" //testing
#include "simulation.h"

#include "defs.h"

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
    Simulation *sim;
    machine *mach;
    int prevPage;       // holds the integer for the previous page

    // page mappings for what their previous page was
    QMap<PageIndex, PageIndex> prevPageMap;

private slots:
    bool submitPassword();
    void switchPage(PageIndex pageName);
    void backPage();    // goes back to the previous page
    void turnOnOff(PageIndex pageName);
    void chargeBattery();
};
#endif // MAINWINDOW_H
