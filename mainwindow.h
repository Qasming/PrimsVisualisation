#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Graph/graph.h"
#include "GraphBuilder/graphbuilder.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateState();
    void setSpeed(qint32);
    void action();
    void reset();

    void generateGraph();
    void clearAll();

    void complitedBuildingMSTree(qint32);
private:
    void blockElements();
    void unblockElements();

private:
    Ui::MainWindow *ui;

    Graph m_graph;
    GraphBuilder *m_graphBulider;

    bool m_buildingMSTree{false};
private: //StyleSheets
    QString activeStateButtonStyle = "QPushButton{"
                           "background-color: rgb(74, 87, 94);"
                           "border-radius: 7px;"
                           "color:white;"
                           "min-width:100px;"
                           "}";
    QString inactiveStateButtonStyle = "QPushButton{"
                             "background-color: rgb(15, 91, 255);"
                             "border-radius: 7px;"
                             "min-width:100px;"
                             "color:white;"
                             "}";

    QString blockedButtonStyle = "QPushButton{"
                            "background-color: rgb(184, 184, 184);"
                            "border-radius: 7px;"
                            "min-width:100px;"
                            "color:white;"
                            "}";
    QString clickedButtonStateStyle = "QPushButton"
                               "{"
                               "background-color: rgb(15, 91, 255);"
                               "border-radius: 7px;"
                               "min-width:100px;"
                               "color:white"
                               "}"
                               "QPushButton::Pressed"
                               "{"
                               "background-color: rgb(74, 87, 94);"
                               "border-radius: 7px;"
                               "color:white"
                               "}"
                               "QPushButton::Released"
                               "{"
                               "background-color: rgb(15, 91, 255);"
                               "border-radius: 7px;	"
                               "color:white"
                               "}";
};
#endif // MAINWINDOW_H
