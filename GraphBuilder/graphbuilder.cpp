#include "graphbuilder.h"
#include "./Graph/graph.h"
#include "./Graph/arc.h"
#include "./Graph/node.h"

#include <QDebug>
#include <QThread>
#include <QVector>



GraphBuilder::GraphBuilder(Graph &graph,QObject *parent)
    : QObject(parent)
{
    m_graph = &graph;
    theard = QThread::create(drawMCSTree,this,m_graph);

}

void GraphBuilder::buildGraph(qint32 countNode, qint32 graphComplexity)
{
    _init_graphTable(countNode);
    fillGraphTable(countNode,graphComplexity);
}

void GraphBuilder::buildMCStree()
{
    //_init_graphTable(m_graph->listNode().length());
    //fillGraphTable();

    //theard->exit();
    theard->start();
    //drawMCStree();

}

GraphBuilder::State GraphBuilder::getSate()
{
    return m_state;
}

void GraphBuilder::start(){
    buildMCStree();
}

void GraphBuilder::play()
{
    m_state = State::Build;
}

void GraphBuilder::pause(){
    m_state = State::Pause;
}

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
    theard->start();
//    QList<Node*> _nodes = m_graph->listNode();
//    QList<Arc*> _arcs = m_graph->listArc();

//    qint32 **graphMatrix = new qint32*[_nodes.length()];
//    for(qint32 i = 0; i < _nodes.length(); i++){
//        graphMatrix[i] = new qint32[_nodes.length()];
//        for(qint32 j = 0; j < _nodes.length(); j++){
//            graphMatrix[i][j] = 0;
//        }
//    }

//    foreach (Node* node, _nodes) {
//        QList<Arc*> nodeArcs  = node->listArc();
//        foreach(Arc* arc, nodeArcs){
//            if(node != arc->node1()){
//                qint32 indexOfNode1 = _nodes.indexOf(node);
//                qint32 indexOfNode2 = _nodes.indexOf(arc->node1());

//                graphMatrix[indexOfNode1][indexOfNode2] = arc->weight();
//                graphMatrix[indexOfNode2][indexOfNode1] = arc->weight();
//            }
//            else if(node != arc->node2()){
//                qint32 indexOfNode1 = _nodes.indexOf(node);
//                qint32 indexOfNode2 = _nodes.indexOf(arc->node2());

//                graphMatrix[indexOfNode1][indexOfNode2] = arc->weight();
//                graphMatrix[indexOfNode2][indexOfNode1] = arc->weight();
//            }
//        }
//    }

//    QVector<Node*> nodes;
//    QVector<Arc*>  arcs;

//    m_state = State::Build;

//    foreach(Node* node, m_graph->listNode()){
//        nodes.push_back(node);
//    }
//    foreach(Arc* arc, m_graph->listArc()){
//        arcs.push_back(arc);
//    }

//    bool* nodesInTree = new bool[m_graph->listNode().length()];
//    for(int i = 0; i < m_graph->listNode().length(); i++){
//        if(i == 0)
//            nodesInTree[i] = true;
//        else
//            nodesInTree[i] = false;
//    }

//    //удаляем все ребра
//    foreach(Arc *arc, arcs){
//        arc->setColor(m_inactiveColor);
//    }

//    //Инициализируем переменные, нужные для постройки дерева
//    bool   complited = false;

//    QList<Node*> _nodesInTree;
//    _nodesInTree.append(nodes[0]);
//    QList<Arc*>  arcInTree;

//    Arc* arcOnStep = nullptr;
//    Node* nodeOnStep = nullptr;

//    qint32 indexNode1 = 0;
//    qint32 indexNode2 = 0;
//    Arc* minArc = nullptr;

//    qint32 count = 1;

//    //(QGraphBuilder *graph, Graph *graph, QList<Node*> &_nodesInTree, QList<Arc*> &_arcInTree, bool* nodesInTree, bool &complited)
//    while(complited == false){
//        if(m_state == State::Build){ //Строим деревe
//            if(indexNode1 < _nodesInTree.size()){
//                nodeOnStep = nodes[indexNode1];
//                if(indexNode2 < nodes.size()){
//                    if(graphMatrix[indexNode1][indexNode2] > 0)
//                    {
//                        if(minArc == nullptr) {
//                            if(!nodesInTree[indexNode1] || !nodesInTree[indexNode2]){
//                                minArc = m_graph->getArc(nodes[indexNode1],nodes[indexNode2]);
//                                arcOnStep = minArc;
//                            }
//                        }
//                        else {
//                            arcOnStep = m_graph->getArc(nodes[indexNode1],nodes[indexNode2]);
//                            if(minArc->weight() > graphMatrix[indexNode1][indexNode2]){
//                                qDebug() << "Index1 " << nodesInTree[indexNode1];
//                                qDebug() << "Index2 " << nodesInTree[indexNode2];
//                                if((nodesInTree[indexNode1] && !nodesInTree[indexNode2]) ||
//                                   (!nodesInTree[indexNode1] && nodesInTree[indexNode2])){
//                                    minArc = m_graph->getArc(nodes[indexNode1],nodes[indexNode2]);
//                                }
//                        }
//                        }
//                    }
//                    indexNode2++;
//                }
//                else{
//                    ++indexNode1;
//                    indexNode2 = 0;
//                }
//            } else if(minArc != nullptr){
//                minArc->setColor(m_MCSTreeColor);
//                Node *node1 = minArc->node1();
//                Node *node2 = minArc->node2();
//                qint32 index1 = nodes.indexOf(node1);
//                qint32 index2 = nodes.indexOf(node2);
//                arcInTree.append(minArc);
//                if(!nodesInTree[index1]){
//                    nodesInTree[index1] = true;
//                   _nodesInTree.append(node1);
//                   count++;
//                }
//                if(!nodesInTree[index2]){
//                    nodesInTree[index2] = true;
//                    _nodesInTree.append(node2);
//                    count++;
//                }
//                minArc = nullptr;
//                indexNode1 = 0;
//                indexNode2 = 0;
//            }

//            if(count == nodes.length())
//                complited = true;

//            updateMCStree(m_graph,_nodesInTree,arcInTree,nodeOnStep,arcOnStep, minArc);
//        }
//        else if(m_state == State::Pause){ //Пропускаем итерацию
//            continue;
//        }
//        else if(m_state == State::Reset){ //Сбрасываем
//            foreach(Arc *arc,m_graph->listArc()){
//                arc->setColor(m_defaultColor);
//            }
//            foreach(Node *node, m_graph->listNode()){
//                node->setColor(m_defaultColor);
//            }
//            delete[] nodesInTree;
//            break;
//        }
//        QThread::msleep(100);

//    }

//    for(int i = 0; i < count; i++){
//        delete [] graphMatrix[i];
//    }
//    delete [] graphMatrix;
//    updateMCStree(m_graph,_nodesInTree,arcInTree,nullptr,nullptr, nullptr);
//    emit buildingCompleted();
}

void GraphBuilder::clearGraphTable(qint32 count)
{
    for(int i = 0; i < count; i++){
        delete [] m_graphTable[i];
    }
    delete [] m_graphTable;
}

void GraphBuilder::updateMCStree(Graph *graph,
                                 QList<Node *> nodesInTree,
                                 QList<Arc *> arcsInTree,
                                 Node* nodeOnStep,
                                 Arc *arcOnStep,
                                 Arc *minArc)
{
    QList<Node*> nodes = graph->listNode();
    QList<Arc*>  arcs  = graph->listArc();

    foreach(Node* item, nodes){
        item->setColor(QColor(0,0,0,255));
    }
    foreach(Arc* item, arcs){
        item->setColor(QColor(196, 196, 196));
    }

    if(minArc != nullptr) minArc->setColor(QColor(0, 227, 68,255));

    if(arcsInTree.size() > 0){
        foreach(Arc* item, arcsInTree){
            item->setColor(QColor(52, 119, 235,255));
        }
    }

    if(nodesInTree.size() > 0){
        foreach(Node* item, nodesInTree){
            item->setColor(QColor(52, 119, 235,255));
        }
    }

    if(arcOnStep != nullptr) arcOnStep->setColor(QColor(255, 56, 38,255));
    if(nodeOnStep != nullptr)nodeOnStep->setColor(QColor(245, 236, 66 ,255));
    graph->update();
}

void GraphBuilder::drawMCSTree(GraphBuilder *graphBuilder,
                               Graph *graph
/*                               QList<Node *>&_nodesInTree,
                               QList<Arc *>&_arcInTree,
                               qint32 **graphMatrix,
                               bool *nodesInTree,
                               bool &complited*/)
{
    QList<Node*> _nodes = graph->listNode();
    QList<Arc*> _arcs = graph->listArc();

    qint32 **graphMatrix = new qint32*[_nodes.length()];
    for(qint32 i = 0; i < _nodes.length(); i++){
        graphMatrix[i] = new qint32[_nodes.length()];
        for(qint32 j = 0; j < _nodes.length(); j++){
            graphMatrix[i][j] = 0;
        }
    }

    foreach (Node* node, _nodes) {
        QList<Arc*> nodeArcs  = node->listArc();
        foreach(Arc* arc, nodeArcs){
            if(node != arc->node1()){
                qint32 indexOfNode1 = _nodes.indexOf(node);
                qint32 indexOfNode2 = _nodes.indexOf(arc->node1());

                graphMatrix[indexOfNode1][indexOfNode2] = arc->weight();
                graphMatrix[indexOfNode2][indexOfNode1] = arc->weight();
            }
            else if(node != arc->node2()){
                qint32 indexOfNode1 = _nodes.indexOf(node);
                qint32 indexOfNode2 = _nodes.indexOf(arc->node2());

                graphMatrix[indexOfNode1][indexOfNode2] = arc->weight();
                graphMatrix[indexOfNode2][indexOfNode1] = arc->weight();
            }
        }
    }

    QVector<Node*> nodes;
    QVector<Arc*>  arcs;

    graphBuilder->play();

    foreach(Node* node, graph->listNode()){
        nodes.push_back(node);
    }
    foreach(Arc* arc, graph->listArc()){
        arcs.push_back(arc);
    }

    bool* nodesInTree = new bool[graph->listNode().length()];
    for(int i = 0; i < graph->listNode().length(); i++){
        if(i == 0)
            nodesInTree[i] = true;
        else
            nodesInTree[i] = false;
    }

    //удаляем все ребра
    foreach(Arc *arc, arcs){
        arc->setColor(QColor(196, 196, 196));
    }

    //Инициализируем переменные, нужные для постройки дерева
    bool   complited = false;

    QList<Node*> _nodesInTree;
    _nodesInTree.append(nodes[0]);
    QList<Arc*>  arcInTree;

    Arc* arcOnStep = nullptr;
    Node* nodeOnStep = nullptr;

    qint32 indexNode1 = 0;
    qint32 indexNode2 = 0;
    Arc* minArc = nullptr;

    qint32 count = 1;

    //(QGraphBuilder *graph, Graph *graph, QList<Node*> &_nodesInTree, QList<Arc*> &_arcInTree, bool* nodesInTree, bool &complited)
    while(complited == false){
        if(graphBuilder->getSate() == State::Build) qDebug() << "build";
        if(graphBuilder->getSate() == State::Pause) qDebug() << "pause";
        if(graphBuilder->getSate() == State::Reset) qDebug() << "resete";
        ////////////////////////////////////////////////////////////////////
        if(graphBuilder->getSate() == State::Build){ //Строим деревe
            if(indexNode1 < _nodesInTree.size()){
                nodeOnStep =  _nodesInTree[indexNode1];
                if(indexNode2 < nodes.size()){
                    if(graphMatrix[indexNode1][indexNode2] > 0)
                    {
                        if(minArc == nullptr) {
                            if(!nodesInTree[nodes.indexOf(_nodesInTree[indexNode1])] || !nodesInTree[indexNode2]){
                                minArc = graph->getArc(_nodesInTree[indexNode1],nodes[indexNode2]);
                                arcOnStep = minArc;
                            }
                        }
                        else {
                            arcOnStep = graph->getArc(_nodesInTree[indexNode1],nodes[indexNode2]);
                            if(minArc->weight() > graphMatrix[indexNode1][indexNode2]){
//                                qDebug() << "Index1 " << nodesInTree[indexNode1];
//                                qDebug() << "Index2 " << nodesInTree[indexNode2];
                                if((nodesInTree[indexNode1] && !nodesInTree[indexNode2]) ||
                                   (!nodesInTree[indexNode1] && nodesInTree[indexNode2])){
                                    minArc = graph->getArc(_nodesInTree[indexNode1],nodes[indexNode2]);
                                }
                        }
                        }
                    }
                    indexNode2++;
                }
                else{
                    ++indexNode1;
                    indexNode2 = 0;
                }
            } else if(minArc != nullptr){
                minArc->setColor(QColor(52, 119, 235,255));
                Node *node1 = minArc->node1();
                Node *node2 = minArc->node2();
                qint32 index1 = nodes.indexOf(node1);
                qint32 index2 = nodes.indexOf(node2);
                arcInTree.append(minArc);
                if(!nodesInTree[index1]){
                    nodesInTree[index1] = true;
                   _nodesInTree.append(node1);
                   count++;
                }
                if(!nodesInTree[index2]){
                    nodesInTree[index2] = true;
                    _nodesInTree.append(node2);
                    count++;
                }
                minArc = nullptr;
                indexNode1 = 0;
                indexNode2 = 0;
            }

            if(count == nodes.length())
                complited = true;

            updateMCStree(graph,_nodesInTree,arcInTree,nodeOnStep,arcOnStep, minArc);
        }
        else if(graphBuilder->getSate() == State::Pause){ //Пропускаем итерацию
            continue;
        }
        else if(graphBuilder->getSate() == State::Reset){ //Сбрасываем
            foreach(Arc *arc,graph->listArc()){
                arc->setColor(QColor{0,0,0,255});
            }
            foreach(Node *node, graph->listNode()){
                node->setColor(QColor{0,0,0,255});
            }
            delete[] nodesInTree;
            break;
        }
        QThread::msleep(300);

    }
    for(int i = 0; i < count; i++){
        delete [] graphMatrix[i];
    }
    delete [] graphMatrix;
    updateMCStree(graph,_nodesInTree,arcInTree,nullptr,nullptr, nullptr);
    //emit buildingCompleted();
}

void GraphBuilder::printGraphTable(qint32 count)
{
    for(int i = 0; i < count; i++){
        for(int j = 0; j < count; j++){
           // qDebug() << m_graphTable[i][j] << ' ';
            Arc *arc = m_graph->getArc(m_graph->listNode()[i],m_graph->listNode()[j]);
            if(arc != nullptr)
                qDebug() << arc->weight();
            else
                qDebug() << 0;
        }
        qDebug() << endl;
    }
}


