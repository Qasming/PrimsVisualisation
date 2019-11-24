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
    void buildGraph(qint32 countNode, qint32 graphComplexity);
    void buildMCStree();

signals:
    void buildingCompleted();
    void buildingBegun();

public slots:
    void start();
    void pause();
    void reset();
    void setSpeed(qint32 speed);

private:
    //!Генерирует нуливую квадратную матрицу (Количесво вершин)
    void _init_graphTable(qint32 count);
    //!Заполняет нулевую квадратную матриц случайными значениями(сложность 1-5)
    void fillGraphTable(qint32 count,qint32 complexity);
    //!Запоняет матрицу по готовому графу
    void fillGraphTable();
    //!Выводим основной граф
    void drawGraph();
    //!Отрисовка дерева
    void drawMCStree();
    //!Отчищает таблицу связей
    void clearGraphTable(qint32 count);

    //!Обновляем дерево
    static void updateMCStree(Graph *graph,
                              QList<Node*>nodesInTree,
                              QList<Arc*> arcsInTree,
                              Node* nodeOnStep,
                              Arc *arcOnStep, Arc *minArc);

//!Тестировочные функции
private:
    void printGraphTable(qint32 count);
/////////////////////////////////
private:
    Graph *m_graph;
    //!Состояние выполения
    State m_state{State::None};
    //!Скорость отрисовки
    qint32 m_speed{1};
    //!Таблица связей
    qint32** m_graphTable;
private:
    //!Ширина
    qint32 m_nodeWidth{45};
    qint32 m_nodeBorderWidth{5};
    qint32 m_arcWidth{5};

private: //!Цвета
    //!Цвет вершины на шаге
    QColor m_stepColorNode{QColor(245, 236, 66 ,255)};
    //!Цвет дуги на шаге
    QColor m_stepColorArc{QColor(255, 56, 38,255)};
    //!Цвет неактивных элементов
    QColor m_inactiveColor{QColor(196, 196, 196)};
    //!Цвет остового дерева
    QColor m_MCSTreeColor{QColor(52, 119, 235,255)};
    //!Цвет по умолчанию
    QColor m_defaultColor{QColor(0,0,0,255)};

};

#endif // GRAPHBUILDER_H
