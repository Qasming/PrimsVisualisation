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

	QList<Node *> listNodes();
	QList<Arc *> listArcs();

	// Узлы
	qint32 addNode(QPoint point);
	bool removeNode(Node *item);
	bool setSizeNodes(const QSize &size);
	bool setWidthLineNodes(qint32 width);
	bool moveNode(Node *item, qint32 xBy, qint32 yBy);
	Node *nodeOnPos(QPoint point) const;

	// Грани
    bool addArc(Node *item1, Node *item2);
    bool addArc(Node *item1, Node *item2, qint32 weight);
    bool setArcWeight(Arc* arc,qint32 weight);
    bool setArcWeight(Node *item1, Node *item2, qint32 weight);
	bool removeArc(Node *item1, Node *item2);
	bool setWidthLineArcs(qint32 width);
    Arc *getArc(Node *node1, Node *node2);

	// Вес граней
	bool setSizeWeightArcs(qint32 pointSize);
	void setFontWeightArc(QFont font);
	bool setWidthLineWeightArcs(qint32 width);

private:
	QSize m_sizeNodes;

	qint32 m_widthLineNodes;
	qint32 m_widthLineWeightArcs;
	qint32 m_widthLineArcs;

	QFont m_fontWeightArcs;
	QFont m_fontNodes;

	QList<Node *> m_listNodes;
	QList<Arc *> m_listArcs;
};

#endif
