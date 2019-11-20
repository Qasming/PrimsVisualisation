#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_graphBulider = new GraphBuilder(m_graph,this);
    ui->w_widget->layout()->addWidget(&m_graph);

    QObject::connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(setState_AddNodes()));
    QObject::connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(start()));
    QObject::connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(setState_AddArcs()));
    QObject::connect(ui->pushButton_4,SIGNAL(clicked()),this,SLOT(setState_MoveItems()));
    QObject::connect(ui->pushButton_5,SIGNAL(clicked()),this,SLOT(setState_RemoveItems()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::blockElements()
{

}

void MainWindow::setState_AddNodes()
{
    m_graph.changeState(Graph::State::AddItem);
}

void MainWindow::setState_RemoveItems()
{
    m_graph.changeState(Graph::State::RemoveItem);
}

void MainWindow::setState_MoveItems()
{
    m_graph.changeState(Graph::State::MoveItem);
}

void MainWindow::setState_AddArcs()
{
    m_graph.changeState(Graph::State::AddLine);
}

void MainWindow::setState_RemoveArcs()
{
    m_graph.changeState(Graph::State::RemoveLine);
}

void MainWindow::setState_ChangeArcsWeight()
{
    m_graph.changeState(Graph::State::ChangeLineWeight);
}

void MainWindow::setSpeed(qint32 speed)
{
    m_graphBulider->setSpeed(speed);
}

void MainWindow::start()
{
    m_graphBulider->start();
}

void MainWindow::pause()
{
    m_graphBulider->pause();
}

void MainWindow::reset()
{
    m_graphBulider->reset();
}

void MainWindow::generateGraph()
{
    //m_graphBulider->buildGraph()
}

