#include "graphscene.h"
#include "node.h"
#include "arc.h"

#include <QInputDialog>
#include <QDebug>

GraphScene::GraphScene(QObject *parent) :
    QGraphicsScene(parent)
{

}

void GraphScene::moveAllObject(qint32 xBy, qint32 yBy)
{
	foreach (Node *node, m_listNodes) {
		moveNode(node, xBy, yBy);
	}
}

void GraphScene::removeAll()
{
	foreach (Node *node, m_listNodes) {
		removeNode(node);
	}
    update();
}

QList<Node *> GraphScene::listNodes()
{
	return m_listNodes;
}

QList<Arc *> GraphScene::listArcs()
{
	return m_listArcs;
}


// ******************** NODES ********************
bool GraphScene::addNode(QPoint point)
{
    point.setX(point.x() - m_sizeNodes.width() / 2);
    point.setY(point.y() - m_sizeNodes.height() / 2);

    Node *node = new Node(m_sizeNodes, m_borderWidthNodes,
                          m_fontNodes, m_listNodes.size() + 1);

    // Перед добавлением нового итема в лист делаем перерисовку всех итомов,
    // т.к. мог измениться размер шрифта для индексов итемов.
    update();
    //Добавляем вершину в список
    m_listNodes.append(node);
    node->setPos(point);

    //Добавляем на сцену
    QGraphicsScene::addItem(node);

    return true;
}

bool GraphScene::removeNode(Node *node)
{
	if (!node)
		return false;

    //Удаление всех инцидентных ребер
	foreach (Arc *arc, node->listArc()) {
		arc->node1() == node ? arc->node2()->removeArc(arc) :
							   arc->node1()->removeArc(arc);
		QGraphicsScene::removeItem(arc);
		m_listArcs.removeOne(arc);
		delete arc;
	}
    //Удаление вершины со сцены
	QGraphicsScene::removeItem(node);
	m_listNodes.removeOne(node);

    for (int i = node->index() - 1; i < m_listNodes.size(); i++) {
        m_listNodes[i]->atIndex()--;
    }

	if (!m_listNodes.isEmpty()) {
        m_listNodes.last()->refreshFont();
		// Перерисовываем всем итемы, т.к. индексы изменились,
		// и, возможно, изменился размер шрифта.
        update();

	}

    //Отчщение памяти
	delete node;

	return true;
}

bool GraphScene::setSizeNodes(const QSize &size)
{
    if (size.width() < 0 || size.height() < 0)
        return false;

    m_sizeNodes = size;
    // Т.к. были изменены размеры узлов, изменяем положение линий.
    foreach (Arc *arc, m_listArcs) {
        arc->setLine(QLineF(arc->node1()->center(), arc->node2()->center()));
    }
    if (!m_listNodes.isEmpty()) m_listNodes.last()->refreshFont();
    update();

    return true;
}

bool GraphScene::setBorderWidthNodes(qint32 width)
{
    if (width < 1)
        return false;

    m_borderWidthNodes = width;
    update();
    return true;
}

bool GraphScene::moveNode(Node *node, qint32 xBy, qint32 yBy)
{
	if (!node)
		return false;

    //Задаем новую позицию вершине
	node->moveBy(xBy, yBy);
    //Обновляем все ребра
	foreach (Arc *arc, node->listArc()) {
        arc->node1() == node ? arc->setLine(QLineF(node->center(),
                                                   arc->node2()->center())):
                               arc->setLine(QLineF(arc->node1()->center(),
                                                   node->center()));
	}
	update();
	return true;
}

Node *GraphScene::nodeOnPos(QPoint point) const
{
	foreach (Node *node, m_listNodes) {
		QRectF rect(node->pos(), m_sizeNodes);
		if (rect.contains(point)) {
			return node;
		}
	}
    return nullptr;
}

// ******************** ARCS ********************
bool GraphScene::addArc(Node *node1, Node *node2)
{
    bool ok;
    qint32 weight = QInputDialog::getInt(nullptr,
                                         "Weight: ","Enter weight (1-999):",
                                         1,1,999,1,
                                         &ok);
    if(!ok)
        return false;
    if (!node1 || !node2)
        return false;

    // Проверка на "Существование ребра между двумя вершинами,
    // проверка веса найденного ребра".
    Arc* arc = getArc(node1,node2);
    if(arc != nullptr){
        if(arc->weight() != weight){
            return setArcWeight(arc,weight);
        }
    }
    //Если связь не найена, то создаем новую
    return addArc(node1,node2,weight);
}

bool GraphScene::addArc(Node *node1, Node *node2, qint32 weight)
{
    if (!node1 || !node2)
        return false;

    if(getArc(node1,node2) != nullptr){
        return false;
    }else{

        QGraphicsScene::removeItem(node1);
        QGraphicsScene::removeItem(node2);

        Arc *arc = new Arc(node1,
                           node2,
                           m_lineWidthArcs,
                           m_fontWeightArcs,
                           m_borderWidthWeightArcs,
                           weight);
        m_listArcs.append(arc);
        node1->addArc(arc);
        node2->addArc(arc);


        QGraphicsScene::addItem(arc);
        QGraphicsScene::addItem(node1);
        QGraphicsScene::addItem(node2);

        update();

        return true;
    }
}

bool GraphScene::setArcWeight(Node *node1, Node *node2, qint32 weight)
{
    // Поиск нужного ребра.
    Arc *arc = getArc(node1,node2);
    if(arc != nullptr){

        return setArcWeight(arc,weight);
    }
    // Линия не найдена.
    return false;
}

bool GraphScene::setArcWeight(Arc* arc, qint32 weight){
    arc->setWeight(weight);
    arc->update();
    return true;
}

bool GraphScene::removeArc(Node *node1, Node *node2)
{
    Arc* arc = getArc(node1,node2);
    if(arc != nullptr){
        node1->removeArc(arc);
        node2->removeArc(arc);
        QGraphicsScene::removeItem(arc);
        m_listArcs.removeOne(arc);
        delete arc;
        return true;
    }
	return false;
}

bool GraphScene::setBorderWidthArcs(qint32 width)
{
	if (width < 1)
		return false;

    m_lineWidthArcs = width;
	foreach (Arc *arc, m_listArcs) {
		arc->setPen(QPen(QColor(0, 0, 0), width));
	}
	update();
    return true;
}

Arc *GraphScene::getArc(Node *node1, Node *node2)
{
    foreach (Arc *arc, node1->listArc()) {
        if ((arc->node1() == node1 && arc->node2() == node2) ||
                (arc->node1() == node2 && arc->node2() == node1)) {
            return arc;
        }
    }
    return nullptr;
}

// ******************** WEIGHT ********************
bool GraphScene::setSizeWeightArcs(qint32 pointSize)
{
	if (pointSize <= 0)
		return false;


	m_fontWeightArcs.setPointSize(pointSize);
	foreach (Arc *arc, m_listArcs) {
		arc->refreshRectForWeight();
	}
	update();
	return true;
}

void GraphScene::setFontWeightArc(QFont font)
{	
	m_fontWeightArcs = font;
	foreach (Arc *arc, m_listArcs) {
		arc->refreshRectForWeight();
	}
	update();
}

bool GraphScene::setBorderWidthWeightArcs(qint32 width)
{
	if (width < 1)
		return false;

    m_borderWidthWeightArcs = width;
	update();
	return true;
}
