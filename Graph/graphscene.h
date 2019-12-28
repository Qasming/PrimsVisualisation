#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>

class Node;
class Arc;

class GraphScene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit GraphScene(QObject *parent = nullptr);

	void moveAllObject(qint32 xBy, qint32 yBy);
	void removeAll();

    //!Возвращает все вершины
	QList<Node *> listNodes();
    //!Возвращает все ребра
	QList<Arc *> listArcs();

    //Вершины
    //!Добавление узла на сцену
    bool addNode(QPoint point);
    //!Удаление узла со сцены
	bool removeNode(Node *item);

    // Графика
    bool setSizeNodes(const QSize &size);
    bool setBorderWidthNodes(qint32 width);
	bool moveNode(Node *item, qint32 xBy, qint32 yBy);
	Node *nodeOnPos(QPoint point) const;

    // Ребра
    //!Добавление ребра
    bool addArc(Node *item1, Node *item2);
    //!Добавление ребра с весом
    bool addArc(Node *item1, Node *item2, qint32 weight);
    //!Получение ребра по его вершинам
    Arc *getArc(Node *node1, Node *node2);
    //!Удаление ребра
    bool removeArc(Node *item1, Node *item2);
    //!Установить вер ребра
    bool setArcWeight(Arc* arc,qint32 weight);
    //!Устнаовить вес ребра
    bool setArcWeight(Node *item1, Node *item2, qint32 weight);

    // Графика
    bool setBorderWidthArcs(qint32 width);


    // Вес ребер
	bool setSizeWeightArcs(qint32 pointSize);
	void setFontWeightArc(QFont font);
    bool setBorderWidthWeightArcs(qint32 width);

private:
    //!Ращмер вершин
    QSize m_sizeNodes{QSize(40, 40)};

    //!Ширина стенок у вершин
    qint32 m_borderWidthNodes{2};
    //!Ширина стенок у рамок для веса
    qint32 m_borderWidthWeightArcs{2};
    //!Ширина лини ребра
    qint32 m_lineWidthArcs{2};
    //!Шрифт веса
    QFont m_fontWeightArcs{QFont("Arial", 8, QFont::Normal)};
    //!Шрифт индексов вершин
    QFont m_fontNodes{QFont("Arial", 0, QFont::Normal)};

    //!Список вершин
	QList<Node *> m_listNodes;
    //!Список ребер
	QList<Arc *> m_listArcs;
};

#endif
