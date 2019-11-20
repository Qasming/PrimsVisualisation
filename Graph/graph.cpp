#include "graph.h"
#include "graphscene.h"
#include "node.h"

#include <QMouseEvent>

#include <QDebug>

Graph::Graph(QWidget *parent)
	: QGraphicsView(parent),
	  m_oldPos(QPoint(0, 0)),
	  m_standardCursor(Qt::PointingHandCursor),
	  m_movableCursor(Qt::SizeAllCursor),
	  m_clickedItem(nullptr),
	  m_pressedItem(nullptr),
	  m_sceneIsMove(false),
	  m_sceneIsMovable(true),
	  m_itemIsMove(false),
	  m_weightForNextLine(-1),
	  m_weightLine(-1),
	  m_currentState(None)
{
	m_scene = new GraphScene(this);
	m_scene->setSceneRect(rect());
	setScene(m_scene);

	setCursor(m_standardCursor);

	setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
}

Graph::~Graph()
{

}

qint32 Graph::addNode(QPoint pos)
{
	return m_scene->addNode(pos);
}

bool Graph::removeNode(qint32 indexNode)
{
	if (indexNode > listNode().size()) {
		m_strError = "Переданный узел не существует! "
					 "[indexNode > listItems.size; method: removeNode]";
		return false;
	}
	if (m_scene->removeNode(getItemOnIndex(indexNode))) {
		return true;
	}
	else {
		m_strError = "Был передан неизвестный указатель! [method: removeNode]";
		return false;
	}
}

bool Graph::moveNode(qint32 indexNode, qint32 xBy, qint32 yBy)
{
	if (indexNode > listNode().size()) {
		m_strError = "Переданный узел не существует! "
					 "[indexNode > listItems.size; method: moveNode]";
		return false;
	}
	if (m_scene->moveNode(getItemOnIndex(indexNode), xBy, yBy)) {
		return true;
	}
	else {
		m_strError = "Был передан неизвестный указатель! [method: moveNode]";
		return false;
	}
}

bool Graph::setWeight(qint32 weight)
{
	if (weight < -1) {
		m_strError = "";
		return false;
	}

	m_weightForNextLine = weight;
	return true;
}

bool Graph::addArc(qint32 indexNode_1, qint32 indexNode_2, qint32 weight)
{
	if (weight < -1) {
		m_strError = "Переданный вес меньше -1! "
					 "[weight < -1; method: addArc]";
		return false;
	}
	if (indexNode_1 == indexNode_2) {
		m_strError = "Переданные узлы одинаковы! "
					 "[indexNode_1 = indexNode_2; method: addArc]";
		return false;
	}
	if (indexNode_1 > listNode().size()) {
		m_strError = "Переданный узел (1) не существует! "
					 "[indexNode_1 > listItems.size; method: addArc]";
		return false;
	}
	if (indexNode_2 > listNode().size()) {
		m_strError = "Переданный узел (2) не существует! "
					 "[indexNode_2 > listItems.size; method: addArc]";
		return false;
	}

	if (m_scene->addArc(getItemOnIndex(indexNode_1),
						 getItemOnIndex(indexNode_2),
						 weight)) {
		return true;
	}
	else {
		m_strError = "Между узлами уже существует связь! [method: addArc]";
		return false;
	}
}

bool Graph::setWeight(qint32 indexNode_1, qint32 indexNode_2, qint32 weight)
{
	if (weight < -1) {
		m_strError = "Переданный вес меньше -1! "
					 "[weight < -1; method: setWeight]";
		return false;
	}
	if (indexNode_1 == indexNode_2) {
		m_strError = "Переданные узлы одинаковы! "
					 "[indexNode_1 = indexNode_2; method: setWeight]";
		return false;
	}
	if (indexNode_1 > listNode().size()) {
		m_strError = "Переданный узел (1) не существует! "
					 "[indexNode_1 > listItems.size; method: setWeight]";
		return false;
	}
	if (indexNode_2 > listNode().size()) {
		m_strError = "Переданный узел (2) не существует! "
					 "[indexNode_2 > listItems.size; method: setWeight]";
		return false;
	}

    if (m_scene->setArcWeight(getItemOnIndex(indexNode_1),
							   getItemOnIndex(indexNode_2),
							   weight)) {
		return true;
	}
	else {
		m_strError = "Между узлами связь не найдена! [method: setWeight]";
		return false;
	}
}

bool Graph::removeArc(qint32 indexNode_1, qint32 indexNode_2)
{
	if (indexNode_1 == indexNode_2) {
		m_strError = "Переданные узлы одинаковы! "
					 "[indexNode_1 = indexNode_2; method: removeArc]";
		return false;
	}
	if (indexNode_1 > listNode().size()) {
		m_strError = "Переданный узел (1) не существует! "
					 "[indexNode_1 > listItems.size; method: removeArc]";
		return false;
	}
	if (indexNode_2 > listNode().size()) {
		m_strError = "Переданный узел (2) не существует! "
					 "[indexNode_2 > listItems.size; method: removeArc]";
		return false;
	}

	if (m_scene->removeArc(getItemOnIndex(indexNode_1),
							getItemOnIndex(indexNode_2))) {
		return true;
	}
	else {
		m_strError = "Между узлами связь не найдена! [method: removeArc]";
		return false;
	}
}

void Graph::clearAll()
{
	m_scene->removeAll();
}

bool Graph::sceneIsMovable() const
{
	return m_sceneIsMovable;
}

Node *Graph::getItemOnIndex(qint32 indexItem)
{
	return listNode()[indexItem - 1];
}

const QString &Graph::error() const
{
	return m_strError;
}

QList<Node *> Graph::listNode()
{
	return m_scene->listNodes();
}

QList<Arc *> Graph::listArc()
{
    return m_scene->listArcs();
}

Arc *Graph::getArc(Node *node1, Node *node2)
{
    return m_scene->getArc(node1,node2);
}

bool Graph::resizeItems(QSize size)
{
	if (size.width() < 0 || size.height() < 0) {
		m_strError = "Параметр переданного размера отрицателен! [method: resizeItems]";
		return false;
	}
	m_scene->setSizeNodes(size);
	return true;
}

bool Graph::resizeWeight(qint32 pointSize)
{
	m_scene->setSizeWeightArcs(pointSize);
	return true;
}

bool Graph::setWidthLineWeights(qint32 width)
{
	m_scene->setWidthLineWeightArcs(width);
	return true;
}

bool Graph::setWidthLineItems(qint32 width)
{
	if (width < 0) {
		m_strError = "Переданный параметр отрицателен! [method: setWidthLineItems]";
		return false;
	}
	m_scene->setWidthLineNodes(width);
	return true;
}

bool Graph::setWidthLines(qint32 width)
{
	if (width < 0) {
		m_strError = "Переданный параметр отрицателен! [method: setWidthLines]";
		return false;
	}
	m_scene->setWidthLineArcs(width);
	return true;
}

void Graph::setSceneMovable(bool b)
{
	m_sceneIsMovable = b;
}

void Graph::changeState(Graph::State st)
{
	m_currentState = st;
}

void Graph::mousePressEvent(QMouseEvent *event)
{
	// Перемещение сцены
	if (m_sceneIsMovable && event->button() == Qt::RightButton) {
		m_sceneIsMove = true;
		m_oldPos = event->pos();

		setCursor(m_movableCursor);
	}
	// Перемещение узла
	else if (event->button() == Qt::LeftButton) {
		if (m_currentState == MoveItem) {
			Node *item = m_scene->nodeOnPos(event->pos());
			if (item) {
				item->setColor(QColor(50, 50, 150));
				item->update();

				m_pressedItem = item;
				m_oldPos = event->pos();
			}
		}
	}
	QGraphicsView::mousePressEvent(event);
}

void Graph::mouseReleaseEvent(QMouseEvent *event)
{
	// Перемещение сцены
	if (m_sceneIsMovable && event->button() == Qt::RightButton){
		m_sceneIsMove = false;

		setCursor(m_standardCursor);
	}
	// Управление узлами
	else if (event->button() == Qt::LeftButton) {
		if (m_currentState == MoveItem) {
			if (m_pressedItem) {
				m_pressedItem->setColor(QColor(0, 0, 0));
				m_pressedItem->update();
				m_pressedItem = nullptr;
			}
		}
		else if (m_currentState == AddItem) {
			m_scene->addNode(event->pos());
		}
		else if (m_currentState == RemoveItem) {
			Node *item = m_scene->nodeOnPos(event->pos());
			if (item) {
				m_scene->removeNode(item);
			}
		}
		else if (m_currentState == AddLine ||
				 m_currentState == RemoveLine ||
				 m_currentState == ChangeLineWeight) {
			Node *item = m_scene->nodeOnPos(event->pos());
			if (item) {
				if (m_clickedItem) { // Если уже был выделен какой-то элемент...
					// Если кликнутый ранее узел равен этому же узлу
					if (m_clickedItem == item) {
						// Отменяем выделение узла
						item->setColor(QColor(0, 0, 0));
						item->update();
						m_clickedItem = nullptr;
					}
					else {
						m_clickedItem->setColor(QColor(0, 0, 0));
						m_clickedItem->update();

						item->setColor(QColor(0, 0, 0));
						item->update();

						if (m_currentState == AddLine)
                            m_scene->addArc(m_clickedItem, item/*, m_weightForNextLine*/);
						else if (m_currentState == RemoveLine)
							m_scene->removeArc(m_clickedItem, item);
						else
                            m_scene->setArcWeight(m_clickedItem, item, m_weightForNextLine);

						m_clickedItem = nullptr;
					}
				}
				else { // Если выделяемый элемент первый...
					if (m_currentState == AddLine)
						item->setColor(QColor(0, 190, 190));
					else if (m_currentState == RemoveLine)
						item->setColor(QColor(255, 128, 128));
					else if (m_currentState == ChangeLineWeight)
						item->setColor(QColor(190, 190, 0));
					item->update();

					m_clickedItem = item;
				}
			}
		}
	}
	QGraphicsView::mouseReleaseEvent(event);
}

void Graph::mouseMoveEvent(QMouseEvent *event)
{
	// Перемещение сцены
	if (m_sceneIsMove) {
		qint32 moveX = -(m_oldPos.x() - event->x());
		qint32 moveY = -(m_oldPos.y() - event->y());
		m_scene->moveAllObject(moveX, moveY);
		m_oldPos = event->pos();
	}
	// Перемещение узла
	else if (m_pressedItem) {
		m_itemIsMove = true;
		qint32 moveX = -(m_oldPos.x() - event->x());
		qint32 moveY = -(m_oldPos.y() - event->y());
		m_scene->moveNode(m_pressedItem, moveX, moveY);
		m_oldPos = event->pos();
	}
	QGraphicsView::mouseMoveEvent(event);
}

void Graph::resizeEvent(QResizeEvent *event)
{
	m_scene->setSceneRect(rect());
	QGraphicsView::resizeEvent(event);
}
