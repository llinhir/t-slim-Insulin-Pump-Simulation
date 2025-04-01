#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGlobal>

#include "machine.h" //testing
#include "profile.h" //testing

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

private:
    Ui::MainWindow *ui;
    machine *m; // testing
    Profile* profile;

    // page names for swapping between pages
    enum PageIndex {
        LOGIN_PAGE = 0,
        HOME_PAGE = 1,
        CREATE_PROFILE_PAGE = 2,
        OPTIONS_PAGE = 3
    };

private slots:
    bool submitPassword();

};
#endif // MAINWINDOW_H
