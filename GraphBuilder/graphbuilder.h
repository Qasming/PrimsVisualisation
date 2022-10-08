#ifndef GRAPHBUILDER_H
#define GRAPHBUILDER_H

#include <QObject>
#include <QVector>
#include <QColor>

class Graph;
class Arc;
class Node;

class GraphBuilder : public QObject
{
    Q_OBJECT

    enum State{
        Build,
        Pause,
        Reset,
        None
    };

public:
    explicit GraphBuilder(Graph &graph,QObject *parent = nullptr);

public:
    State getSate();
signals:
    //!Сигнал о завершении построения MSTree
    void buildingCompleted(qint32);
public slots:
    //!Генерация графа
    void buildGraph(qint32 countNode);
    //!Запуск/Продолжение построения MSTree
    void play();
    //!Пауза при построении MSTree
    void pause();
    //!Запуск построения MSTree
    void start();
    //!Сброс, возврат к начальному графу
    void reset();
    //!Установка скорости поиска
    void setSpeed(qint32 speed);

private:
    //!Обновление дерева
    static void updateColors(Graph *graph,
                              QList<Node*>nodesInTree,
                              QList<Arc*> arcsInTree,
                              QList<Arc *> arcsForCheck,
                              Arc *arcAtStep = nullptr,
                              Arc *minArc = nullptr,
                              Node *noneAtStepOfFilling = nullptr,
                              Arc  *arcAtStepOfFilling = nullptr);

    //!Создание дерева
    static void drawMSTree(GraphBuilder *graphBuilder);
    //!Случайная генерация значений от 1 до 999
    qint32 rand();
private:
    Graph *m_graph;
    //!Состояние выполения
    State m_state{State::None};
    //!Скорость отрисовки
    qint32 m_speed{1};
    //!Максимальная задержка между итерациями
    qint32 msMax{500};

};

#endif // GRAPHBUILDER_H
