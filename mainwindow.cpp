#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Minimum Spannig Tree (Prim's algorithm)");
    m_graphBulider = new GraphBuilder(m_graph,this);
    ui->vlayout->insertWidget(0,&m_graph);

    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, QColor(44, 56, 68));
    setAutoFillBackground(true);
    setPalette(Pal);

    QObject::connect(m_graphBulider,
                     SIGNAL(buildingCompleted(qint32)),
                     this,
                     SLOT(complitedBuildingMSTree(qint32)));

    QObject::connect(ui->speedSlider,
                     SIGNAL(valueChanged(int)),
                     m_graphBulider,
                     SLOT(setSpeed(int)));

    QObject::connect(ui->genFGButton,
                     SIGNAL(clicked()),
                     this,
                     SLOT(generateGraph()));

    QObject::connect(ui->actionButton,
                     SIGNAL(clicked()),
                     this,
                     SLOT(action()));

    QObject::connect(ui->resetButton,
                     SIGNAL(clicked()),
                     this,
                     SLOT(reset()));

    QObject::connect(ui->clearAllButton,
                     SIGNAL(clicked()),
                     this,
                     SLOT(clearAll()));

    QObject::connect(ui->addNodesButton,
                     SIGNAL(clicked()),
                     this,
                     SLOT(updateState()));
    QObject::connect(ui->addEdgesButton,
                     SIGNAL(clicked()),
                     this,
                     SLOT(updateState()));
    QObject::connect(ui->removeNodesButton,
                     SIGNAL(clicked()),
                     this,
                     SLOT(updateState()));
    QObject::connect(ui->removeEdgesButton,
                     SIGNAL(clicked()),
                     this,
                     SLOT(updateState()));
    QObject::connect(ui->moveNodesButton,
                     SIGNAL(clicked()),
                     this,
                     SLOT(updateState()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::blockElements()
{
    ui->addEdgesButton->setEnabled(false);
    ui->addNodesButton->setEnabled(false);
    ui->removeEdgesButton->setEnabled(false);
    ui->removeNodesButton->setEnabled(false);
    ui->genFGButton->setEnabled(false);
    ui->clearAllButton->setEnabled(false);
    ui->moveNodesButton->setEnabled(false);
    ui->countNodes->setEnabled(false);

    ui->addEdgesButton->setStyleSheet(blockedButtonStyle);
    ui->addNodesButton->setStyleSheet(blockedButtonStyle);
    ui->removeEdgesButton->setStyleSheet(blockedButtonStyle);
    ui->removeNodesButton->setStyleSheet(blockedButtonStyle);
    ui->genFGButton->setStyleSheet(blockedButtonStyle);
    ui->moveNodesButton->setStyleSheet(blockedButtonStyle);
    ui->clearAllButton->setStyleSheet(blockedButtonStyle);
}

void MainWindow::unblockElements()
{
    ui->addEdgesButton->setEnabled(true);
    ui->addNodesButton->setEnabled(true);
    ui->removeEdgesButton->setEnabled(true);
    ui->removeNodesButton->setEnabled(true);
    ui->genFGButton->setEnabled(true);
    ui->clearAllButton->setEnabled(true);
    ui->moveNodesButton->setEnabled(true);
    ui->countNodes->setEnabled(true);

    ui->addEdgesButton->setStyleSheet(inactiveStateButtonStyle);
    ui->addNodesButton->setStyleSheet(inactiveStateButtonStyle);
    ui->removeEdgesButton->setStyleSheet(inactiveStateButtonStyle);
    ui->removeNodesButton->setStyleSheet(inactiveStateButtonStyle);
    ui->moveNodesButton->setStyleSheet(inactiveStateButtonStyle);

    ui->genFGButton->setStyleSheet(clickedButtonStateStyle);
    ui->clearAllButton->setStyleSheet(clickedButtonStateStyle);
    ui->resetButton->setStyleSheet(clickedButtonStateStyle);
}

void MainWindow::updateState()
{
    QPushButton *clickedButton = reinterpret_cast<QPushButton*>(sender());

    ui->addEdgesButton->setStyleSheet(inactiveStateButtonStyle);
    ui->addNodesButton->setStyleSheet(inactiveStateButtonStyle);
    ui->removeEdgesButton->setStyleSheet(inactiveStateButtonStyle);
    ui->removeNodesButton->setStyleSheet(inactiveStateButtonStyle);
    ui->moveNodesButton->setStyleSheet(inactiveStateButtonStyle);

    clickedButton->setStyleSheet(activeStateButtonStyle);

    if(clickedButton == ui->addEdgesButton)
        m_graph.setState(Graph::State::AddArc);

    else if(clickedButton == ui->addNodesButton)
        m_graph.setState(Graph::State::AddNode);

    else if(clickedButton == ui->removeEdgesButton)
        m_graph.setState(Graph::State::RemoveArc);

    else if(clickedButton == ui->removeNodesButton)
        m_graph.setState(Graph::State::RemoveNode);
    else if(clickedButton == ui->moveNodesButton)
        m_graph.setState(Graph::State::MoveNode);
}


void MainWindow::setSpeed(qint32 speed)
{
    m_graphBulider->setSpeed(speed);
}

void MainWindow::action()
{
    if(m_graph.listNode().isEmpty())
        return;
    blockElements();
    m_graph.setState(Graph::State::None);
    ui->minimumWeight->setText("0");
    m_buildingMSTree = !m_buildingMSTree;
    if(m_buildingMSTree){
        ui->actionButton->setText("Pause");
        m_graphBulider->play();
    }
    else{
        ui->actionButton->setText("Play");
        m_graphBulider->pause();
    }
}

void MainWindow::reset()
{
    m_buildingMSTree = false;
    ui->minimumWeight->setText("0");
    ui->actionButton->setText("Play");
    m_graphBulider->reset();
    m_graph.setState(Graph::State::None);
    unblockElements();
}

void MainWindow::generateGraph()
{
    m_graphBulider->buildGraph(ui->countNodes->value());
}

void MainWindow::clearAll()
{
    m_graph.clearAll();
}

void MainWindow::complitedBuildingMSTree(qint32 weight)
{
    m_buildingMSTree = false;
    ui->minimumWeight->setText(QString::number(weight));
    m_graph.setState(Graph::State::None);
    unblockElements();
    ui->actionButton->setText("Play");
}

