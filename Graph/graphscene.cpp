#include "graphscene.h"
#include "node.h"
#include "arc.h"

#include <QInputDialog>
#include <QDebug>

GraphScene::GraphScene(QObject *parent) :
	QGraphicsScene(parent),
	m_sizeNodes(QSize(40, 40)),
	m_widthLineNodes(2),
	m_widthLineWeightArcs(2),
	m_widthLineArcs(2),
	m_fontWeightArcs(QFont("Arial", 8, QFont::Normal)),
	m_fontNodes(QFont("Arial", 0, QFont::Normal))
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
qint32 GraphScene::addNode(QPoint point)
{
	point.setX(point.x() - m_sizeNodes.width() / 2 /* i don't know why 4 */);
	point.setY(point.y() - m_sizeNodes.height() / 2 /* i don't know why 4 */);

	Node *node = new Node(m_sizeNodes, m_widthLineNodes, m_fontNodes, m_listNodes.size() + 1);

	// Перед добавлением нового итема в лист делаем перерисовку всех итомов,
	// т.к. мог измениться размер шрифта для индексов итемов.
	foreach (Node *node, m_listNodes) {
		node->update();
	}

	m_listNodes.append(node);
	node->setPos(point);

	QGraphicsScene::addItem(node);

	return node->index();
}

bool GraphScene::removeNode(Node *node)
{
	if (!node)
		return false;

	foreach (Arc *arc, node->listArc()) {
		arc->node1() == node ? arc->node2()->removeArc(arc) :
							   arc->node1()->removeArc(arc);
		QGraphicsScene::removeItem(arc);
		m_listArcs.removeOne(arc);
		delete arc;
	}
	QGraphicsScene::removeItem(node);
	m_listNodes.removeOne(node);

    for (int i = node->index() - 1; i < m_listNodes.size(); i++) {
        m_listNodes[i]->atIndex()--;
    }

	if (!m_listNodes.isEmpty()) {
        m_listNodes.last()->refreshFont();
		// Перерисовываем всем итемы, т.к. индексы изменились,
		// и, возможно, изменился размер шрифта.
		foreach (Node *node, m_listNodes) {
			node->update();
		}
	}

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

bool GraphScene::setWidthLineNodes(qint32 width)
{
	if (width < 1)
		return false;

	m_widthLineNodes = width;
	update();
	return true;
}

bool GraphScene::moveNode(Node *node, qint32 xBy, qint32 yBy)
{
	if (!node)
		return false;

	node->moveBy(xBy, yBy);

	foreach (Arc *arc, node->listArc()) {
		arc->node1() == node ? arc->setLine(QLineF(node->center(), arc->node2()->center())):
							   arc->setLine(QLineF(arc->node1()->center(), node->center()));
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
    qint32 weight = QInputDialog::getInt(nullptr,"Вес дуги","Введите вес (1-500):",1,1,500,1,&ok);
    if(!ok)
        return false;
    if (!node1 || !node2)
        return false;

    // Проверка на "Существование ребра между двумя вершинами, проверка веса найденного ребра".
    Arc* arc = getArc(node1,node2);
    if(arc != nullptr){
        if(arc->weight() != weight){
            setArcWeight(arc,weight);
            return false;
        }
    }
    //Если связь не найена, то создаем новую
    return addArc(node1,node2,weight);
}

bool GraphScene::addArc(Node *node1, Node *node2, qint32 weight)
{
    if (!node1 || !node2)
        return false;

    if(getArc(node1,node2) != nullptr)
        return false;
//	// Проверка на "существует ли линия между этими узлами".
//	foreach (Arc *arc, node1->listArc()) {
//		if ((arc->node1() == node1 && arc->node2() == node2) ||
//				(arc->node1() == node2 && arc->node2() == node1)) {
//            if(arc->weight() != weight)

//			return false;
//		}
//	}

	Node *newNode_1 = new Node(*node1);
	Node *newNode_2 = new Node(*node2);

	m_listNodes.last()->refreshFont();

	m_listNodes.insert(node1->index() - 1, newNode_1);
	m_listNodes.removeOne(node1);
	m_listNodes.insert(node2->index() - 1, newNode_2);
	m_listNodes.removeOne(node2);

	QGraphicsScene::removeItem(node1);
	QGraphicsScene::removeItem(node2);
	delete node1;
	delete node2;

	Arc *arc = new Arc(newNode_1,
					   newNode_2,
					   m_widthLineArcs,
					   m_fontWeightArcs,
					   m_widthLineWeightArcs,
					   weight);
	m_listArcs.append(arc);
	newNode_1->addArc(arc);
	newNode_2->addArc(arc);

	update();

	QGraphicsScene::addItem(arc);
	QGraphicsScene::addItem(newNode_1);
	QGraphicsScene::addItem(newNode_2);

	return true;
}

bool GraphScene::setArcWeight(Node *node1, Node *node2, qint32 weight)
{
    // Поиск нужной линии.
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
//    foreach (Arc *arc, node1->listArc()) {
//        if ((arc->node1() == node1 && arc->node2() == node2) ||
//                (arc->node1() == node2 && arc->node2() == node1)) {
//            node1->removeArc(arc);
//            node2->removeArc(arc);
//            QGraphicsScene::removeItem(arc);
//            m_listArcs.removeOne(arc);
//            delete arc;
//            return true;
//        }
//    }
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

bool GraphScene::setWidthLineArcs(qint32 width)
{
	if (width < 1)
		return false;

	m_widthLineArcs = width;
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

bool GraphScene::setWidthLineWeightArcs(qint32 width)
{
	if (width < 1)
		return false;

	m_widthLineWeightArcs = width;
	update();
	return true;
}
