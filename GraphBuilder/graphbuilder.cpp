#include "graphbuilder.h"
#include "./Graph/graph.h"
#include "./Graph/arc.h"
#include "./Graph/node.h"

#include <QDebug>

GraphBuilder::GraphBuilder(Graph &graph,QObject *parent)
    : QObject(parent)
{
    m_graph = &graph;
}

void GraphBuilder::buildGraph(qint32 countNode, qint32 graphComplexity)
{
    _init_graphTable(countNode);
    fillGraphTable(countNode,graphComplexity);
}

void GraphBuilder::buildMCStree()
{
    _init_graphTable(m_graph->listNode().length());
    fillGraphTable();
    printGraphTable(m_graph->listNode().length());
    clearGraphTable(m_graph->listNode().length());
}

void GraphBuilder::start(){
    buildMCStree();
}

void GraphBuilder::pause(){}

void GraphBuilder::reset(){}

void GraphBuilder::setSpeed(qint32 speed)
{
    m_speed = speed;
}

void GraphBuilder::_init_graphTable(qint32 count)
{
    m_graphTable = new qint32*[count];
    for(qint32 i = 0; i < count; i++){
        m_graphTable[i] = new qint32[count];
    }

    for(qint32 i = 0; i < count; i++)
        for(qint32 j = 0; j < count; j++){
            m_graphTable[i][j] = 0;
        }
}

void GraphBuilder::fillGraphTable(qint32 count,qint32 complexity)
{

    if(complexity == 1){
        for(qint32 i = 0; i < count - 1; i++){
            for(qint32 j = i + 1; j < count; j++){

            }
        }
    }

    if(complexity == 2){
        for(qint32 i = 0; i < count - 1; i++){
            for(qint32 j = i + 1; j < count; j++){

            }
        }
    }

    if(complexity == 3){
        for(qint32 i = 0; i < count - 1; i++){
            for(qint32 j = i + 1; j < count; j++){

            }
        }
    }

    if(complexity == 4){
        for(qint32 i = 0; i < count - 1; i++){
            for(qint32 j = i + 1; j < count; j++){

            }
        }
    }
}

void GraphBuilder::fillGraphTable()
{
    QList<Node*> nodes = m_graph->listNode();
    QList<Arc*> arcs = m_graph->listArc();

    foreach (Node* node, nodes) {
        QList<Arc*> nodeArcs  = node->listArc();
        foreach(Arc* arc, nodeArcs){
            if(node != arc->node1()){
                qint32 indexOfNode1 = nodes.indexOf(node);
                qint32 indexOfNode2 = nodes.indexOf(arc->node1());

                m_graphTable[indexOfNode1][indexOfNode2] = arc->weight();
                m_graphTable[indexOfNode2][indexOfNode1] = arc->weight();
            }
            else if(node != arc->node2()){
                qint32 indexOfNode1 = nodes.indexOf(node);
                qint32 indexOfNode2 = nodes.indexOf(arc->node2());

                m_graphTable[indexOfNode1][indexOfNode2] = arc->weight();
                m_graphTable[indexOfNode2][indexOfNode1] = arc->weight();
            }
        }
    }
}

void GraphBuilder::drawMCStree()
{

}

void GraphBuilder::clearGraphTable(qint32 count)
{
    for(int i = 0; i < count; i++){
        delete [] m_graphTable[i];
    }
    delete [] m_graphTable;
}

void GraphBuilder::printGraphTable(qint32 count)
{
    for(int i = 0; i < count; i++){
        for(int j = 0; j < count; j++){
            qDebug() << m_graphTable[i][j] << ' ';
        }
        qDebug() << endl;
    }
}


