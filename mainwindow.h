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
    enum State{
      None, Bulding, Completed
    };
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void blockElements();
private slots:
    void setState_AddNodes();
    void setState_RemoveItems();
    void setState_MoveItems();
    void setState_AddArcs();
    void setState_RemoveArcs();
    void setState_ChangeArcsWeight();

    void setSpeed(qint32);

    void start();
    void pause();
    void reset();
    void generateGraph();

private:
    Ui::MainWindow *ui;

    Graph m_graph;
    GraphBuilder *m_graphBulider;
};
#endif // MAINWINDOW_H
