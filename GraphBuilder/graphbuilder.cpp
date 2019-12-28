#include "graphbuilder.h"
#include "./Graph/graph.h"
#include "./Graph/arc.h"
#include "./Graph/node.h"

#include <QDebug>
#include <QThread>
#include <QtMath>

GraphBuilder::GraphBuilder(Graph &graph,QObject *parent)
    : QObject(parent)
{
    m_graph = &graph;
}

void GraphBuilder::buildGraph(qint32 countNodes)
{
    if(countNodes <= 0) return;

    //Создание матрицы смежности, по которой будет заполняться граф
    qint32 **graphMatrix = new qint32*[static_cast<quint32>(countNodes)];
    for(qint32 i = 0; i < countNodes; i++){
        graphMatrix[i] = new qint32[static_cast<quint32>(countNodes)];
        for(qint32 j = 0; j < countNodes; j++){
            if(i == j) graphMatrix[i][j] = 0;
            else graphMatrix[i][j] = this->rand();
        }
    }

    //Отчищаем поле
    m_graph->clearAll();

    //Добавление вершин на поле
    if(countNodes == 1){
        m_graph->addNode(QPoint(m_graph->width() / 2,
                                m_graph->height()/2));
    }

    else if(countNodes == 2){
        m_graph->addNode(QPoint(m_graph->width() / 2,
                                m_graph->height() / 2 - 70));
        m_graph->addNode(QPoint(m_graph->width() / 2,
                                m_graph->height() / 2 + 70));
    }

    else if(countNodes == 3){
        m_graph->addNode(QPoint(m_graph->width() / 2,
                                m_graph->height() / 2 - 70));
        m_graph->addNode(QPoint(m_graph->width() / 2 - 70,
                                m_graph->height() / 2 + 70));
        m_graph->addNode(QPoint(m_graph->width() / 2 + 70,
                                m_graph->height() / 2 + 70));
    }

    //Круговое добавление
    else if(countNodes % 2 == 0){
        qint32 _centerX = m_graph->width() / 2;
        qint32 _centerY = m_graph->height() / 2;
        //Количество радиан между вершинами
        double k = (360.0 / (countNodes))*M_PI/180;
        qint32 radius = 30;

        for(qint32 i = 0; i < countNodes; i++){
            double  pointY  = _centerY + (sin(k*i) * ((countNodes)*radius));
            double  pointX  = _centerX + (cos(k*i) * ((countNodes)*radius));

            m_graph->addNode(QPoint(static_cast<qint32>(pointX),
                                    static_cast<qint32>(pointY))
                             );
        }
    }
    else {
        qint32 _centerX = m_graph->width() / 2;
        qint32 _centerY = m_graph->height() / 2;
        //Количество радиан между вершинами
        double k = (360.0 / (countNodes - 1))*M_PI/180;
        qint32 radius = 30;

        for(qint32 i = 0; i < countNodes - 1; i++){
            double  pointY  = _centerY + (sin(k*i) * ((countNodes-1)*radius));
            double  pointX  = _centerX + (cos(k*i) * ((countNodes-1)*radius));

            m_graph->addNode(QPoint(static_cast<qint32>(pointX),
                                    static_cast<qint32>(pointY))
                             );
        }
        m_graph->addNode(QPoint(_centerX,
                                _centerY)
                         );
    }
    //Создание ребер по матрице
    for(qint32 i = 0; i < m_graph->listNode().length(); i++){
        for(qint32 j = 0; j < m_graph->listNode().length(); j++){
            Node *node1 = m_graph->listNode()[i];
            Node *node2 = m_graph->listNode()[j];
            if(m_graph->listNode()[i] == m_graph->listNode()[j]) {continue;}
            m_graph->addArc(node1,node2, graphMatrix[i][j]);
        }
    }
    //Удаление матрицы
    for(int i = 0; i < countNodes; i++){
        delete [] graphMatrix[i];
    }
    delete [] graphMatrix;
    //Обновление поля после создания графа
    m_graph->update();
}

GraphBuilder::State GraphBuilder::getSate()
{
    return m_state;
}

void GraphBuilder::start()
{
    //Провека(не является ли поле пустым)
    if(m_graph->listArc().size() < 1)
        return;
    //Запуск поиска в отдельном потоке
    QThread::create(drawMSTree,this)->start();
}

void GraphBuilder::play()
{
    //Запуск поиска дерева
    if(m_state == State::None)
    {
        m_state = State::Build;
        start();
    }
    m_state = State::Build;
}

void GraphBuilder::pause(){
    m_state = State::Pause;
}

void GraphBuilder::reset(){
    if(m_state == State::Build || m_state == State::Pause){
        m_state = State::Reset;
    }
    else
    {
        foreach(Arc *arc,m_graph->listArc()){
            arc->setColor(QColor{0,0,0,255});
            arc->setBrushWeight(QColor(255,255,255,255));
        }
        foreach(Node *node, m_graph->listNode()){
            node->setColor(QColor{0,0,0,255});
        }
        m_graph->update();
    }

}

void GraphBuilder::setSpeed(qint32 speed)
{
    m_speed = speed;
}

qint32 GraphBuilder::rand()
{
     //Генерирует число от 1 до 999

     qint32 n = qrand();
     if(n == 0)
         return this->rand();
     if(n > 999)
         n = n / 100;
     return n;
}

void GraphBuilder::updateColors(Graph *graph,
                                 QList<Node *> nodesInTree,
                                 QList<Arc *> arcsInTree,
                                 QList<Arc *> arcsForCheck,
                                 Arc *arcAtStep,
                                 Arc *minArc,
                                 Node *nodeAtStepOfFilling,
                                 Arc *arcAtStepOfFilling)
{

    foreach(Node* item, graph->listNode()){
        item->setColor(QColor(0,0,0,255));
    }

    if(!nodesInTree.isEmpty()){
        foreach(Arc* item, graph->listArc()){
            item->setColor(QColor(196, 196, 196,30));
            item->setBrushWeight(QColor(196, 196, 196,30));
        }
    }
    else{
        foreach(Arc* item, graph->listArc()){
            item->setColor(QColor(0, 0, 0,255));
            item->setBrushWeight(QColor(255, 255, 255 ,255));
        }
    }


    if(!nodesInTree.isEmpty()){
        foreach(Node* item, nodesInTree){
            item->setColor(QColor(52, 119, 235,255));
        }
    }

    if(!arcsInTree.isEmpty()){
        foreach(Arc* item, arcsInTree){
            item->setColor(QColor(52, 119, 235,255));
            item->setBrushWeight(QColor(255, 255, 255,255));
        }
    }

    if(!arcsForCheck.isEmpty()){
        foreach(Arc* item, arcsForCheck){
            item->setColor(QColor(248, 255, 145,255));
            item->setBrushWeight(QColor(255, 255, 255,90));
        }
    }

    if(minArc != nullptr) {
        minArc->setColor(QColor(0, 227, 68,255));
        minArc->setBrushWeight(QColor(255,255,255,255));
    }

    if(arcAtStep != nullptr) {
        arcAtStep->setColor(QColor(255, 56, 38,255));
        arcAtStep->setBrushWeight(QColor(255,255,255,255));
    }

    if(arcAtStepOfFilling){
        arcAtStepOfFilling->setColor(QColor(133, 38, 171,255));
        arcAtStepOfFilling->setBrushWeight(QColor(255,255,255,255));
    }
    if(nodeAtStepOfFilling){
        nodeAtStepOfFilling->setColor(QColor(133, 38, 171,255));
    }

}

void GraphBuilder::drawMSTree(GraphBuilder *graphBuilder)
{
    //Инициализируем переменные, нужные для постройки дерева

    //Флаг завершенности поиска дерева
    bool   complited = false;
    //Вершины в дереве
    QList<Node*> nodesInTree;
    //Ребра в дереве
    QList<Arc*> arcsInTree;
    //Множество ребер для поиска минимального (разрез)
    QList<Arc*> cut;

    //Ребро на этапе поиска минимального
    Arc* arcAtStep = nullptr;
    //Возможное минимальное ребро
    Arc* minArc = nullptr;

    //Ребро на этапе заполнения множества ребер для поиска минимального
    Arc*  arcAtStepOfFilling  = nullptr;
    //Вершина на этапе заполнения множества ребер для поиска минимального
    Node* nodeAtStep = nullptr;

    //Флаг "Формирование разреза"
    bool cutMode = true;
    //Флаг "Поиск минимального ребра"
    bool searchMode   = false;

    //Индекс текущего ребра
    qint32 indexArc =  0;

    //Иницализация множества вершин в дереве
    //Добавление вершины index = 0 в дерево в качестве корня
    nodesInTree.append(graphBuilder->m_graph->listNode()[0]);

    while(complited == false){
        if(graphBuilder->getSate() == State::Build){ //Строим деревe
            if(arcsInTree.size() < graphBuilder->
                    m_graph->listNode().size()-1)
            {
                if(searchMode) //Поиск минимального ребра
                {
                    //Провека(не все ли ребра в множестве проверены)
                    if(indexArc < cut.size()){
                        //Ребро на текущем шаге
                        arcAtStep = cut[indexArc];
                        //если минимальное ребро отсутсвоет,
                        //то присваивается ему текущее
                        if(minArc == nullptr){
                            minArc = arcAtStep;
                        }
                        else {
                            //Если вес текущего ребра меньше минимального
                            //То в минимальное ребро записывается текущее
                            if(minArc->weight() > arcAtStep->weight()){
                                minArc = arcAtStep;
                            }
                        }
                        indexArc++;
                    }
                    else{
                        if(minArc){
                            //удаляем ребро из множества
                            cut.removeOne(minArc);
                            //Добавляем ребро в дерево
                            arcsInTree.append(minArc);

                           //Проверяются инцидентные вершины минимального
                           // ребра

                            //Первая вершина
                           if(nodesInTree.indexOf(minArc->node1())
                                   == -1){
                               Node* node = minArc->node1();
                               nodesInTree.append(node);
                           }
                           //Вторая вершина
                           else if(nodesInTree.indexOf(minArc->node2())
                                   == -1){
                               Node* node = minArc->node2();
                               nodesInTree.append(node);
                           }

                           //Смена режима
                           //В качестве вершины для заполнения множества
                           //ребер, берется только-что добавленная
                           //вершина (последния)
                           nodeAtStep= nodesInTree.last();
                           minArc = nullptr;
                           cutMode = true;
                           arcAtStep = nullptr;
                           searchMode = false;
                           indexArc = 0;
                        }
                    }
                }
                else if(cutMode) //Заполнение разреза
                    {
                        //Вершина на данном шаге
                        nodeAtStep = nodesInTree.last();
                        if(indexArc < nodeAtStep->listArc().size()){
                            //Получение ребро на данном шаге
                            arcAtStepOfFilling =
                                    nodeAtStep->listArc()[indexArc];
                            //Поверка(не находится ли смежное ребро в дереве)
                            if(nodesInTree.indexOf(
                                        nodeAtStep->
                                        adjacentNode(arcAtStepOfFilling))==-1)
                            {
                                cut.append(arcAtStepOfFilling);
                            }
                            //Проверка(входит ли текущее ребро в разрез
                            else if(cut.indexOf(arcAtStepOfFilling)
                                    != -1)
                            {
                                cut.removeOne(arcAtStepOfFilling);
                            }
                            indexArc++;
                        }
                    else{
                        //Смена режима
                        arcAtStepOfFilling  = nullptr;
                        nodeAtStep= nullptr;
                        cutMode = false;
                        searchMode = true;
                        indexArc = 0;
                    }
                }
            }
            else
            {

                //Завершение поиска
                complited = true;
                cut.clear();
                arcAtStep = nullptr;
                arcAtStepOfFilling = nullptr;
                nodeAtStep = nullptr;
                minArc = nullptr;

//                updateColors(graphBuilder->m_graph,
//                              nodesInTree,
//                              arcsInTree,
//                              cut,
//                              arcAtStep,
//                              minArc,
//                              nodeAtStep,
//                              arcAtStepOfFilling);
            }
//            //Обновляем цвета
            updateColors(graphBuilder->m_graph,
                          nodesInTree,
                          arcsInTree,
                          cut,
                          arcAtStep,
                          minArc,
                          nodeAtStep,
                          arcAtStepOfFilling);
            //Задержка между итерациями
            QThread::msleep(graphBuilder->msMax/graphBuilder->m_speed);
        }
        else if(graphBuilder->getSate() == State::Pause){ //Пропуск итерации
            continue;
        }
        else if(graphBuilder->getSate() == State::Reset){
            //Сброс до начального состояния
            nodesInTree.clear();
            arcsInTree.clear();
            cut.clear();
            complited = true;
            graphBuilder->reset();
        }
        graphBuilder->m_graph->update();
        //graph->update();
    }

    //Установка текущего состояние в None
    graphBuilder->m_state = State::None;
    qint32 sum = 0;
    //Нахождение веса получившегося дерева
    foreach(Arc* arc, arcsInTree){
        sum += arc->weight();
    }
    //отправка сигнала о завершении поиска. Отправка веса
    emit graphBuilder->buildingCompleted(sum);
    graphBuilder->m_graph->update();
}



