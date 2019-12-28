#include "graph.h"
#include "graphscene.h"
#include "node.h"

#include <QMouseEvent>

#include <QDebug>

Graph::Graph(QWidget *parent)
    : QGraphicsView(parent)
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

bool Graph::addNode(QPoint pos)
{
    return m_scene->addNode(pos);
}

bool Graph::removeNode(Node *node)
{
   return m_scene->removeNode(node);
}

bool Graph::setWeight(qint32 weight)
{
	if (weight < -1) {
		return false;
	}

	m_weightForNextLine = weight;
	return true;
}


bool Graph::addArc(Node *node1, Node *node2, qint32 weight)
{
    return m_scene->addArc(node1,node2,weight);
}


bool Graph::setWeight(Node *node1, Node *node2, qint32 weight)
{
    Arc* arc = getArc(node1,node2);
    return m_scene->setArcWeight(arc,weight);
}


void Graph::clearAll()
{
	m_scene->removeAll();
}

bool Graph::sceneIsMovable() const
{
	return m_sceneIsMovable;
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

void Graph::update()
{
    m_scene->update();
}

bool Graph::resizeItems(QSize size)
{
	if (size.width() < 0 || size.height() < 0) {
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
    m_scene->setBorderWidthWeightArcs(width);
	return true;
}

bool Graph::setWidthLineItems(qint32 width)
{
	if (width < 0) {
		return false;
	}
    m_scene->setBorderWidthNodes(width);
	return true;
}

bool Graph::setWidthLines(qint32 width)
{
	if (width < 0) {
		return false;
	}
    m_scene->setBorderWidthArcs(width);
	return true;
}

void Graph::setSceneMovable(bool b)
{
	m_sceneIsMovable = b;
}

void Graph::setState(Graph::State st)
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
        if (m_currentState == MoveNode) {
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
        if (m_currentState == MoveNode) {
			if (m_pressedItem) {
				m_pressedItem->setColor(QColor(0, 0, 0));
				m_pressedItem->update();
				m_pressedItem = nullptr;
			}
		}
        else if (m_currentState == AddNode) {
			m_scene->addNode(event->pos());
		}
        else if (m_currentState == RemoveNode) {
			Node *item = m_scene->nodeOnPos(event->pos());
			if (item) {
				m_scene->removeNode(item);
			}
		}
        else if (m_currentState == AddArc ||
                 m_currentState == RemoveArc ||
                 m_currentState == ChangeArcWeight) {
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

                        if (m_currentState == AddArc)
                            m_scene->addArc(m_clickedItem, item);
                        else if (m_currentState == RemoveArc)
							m_scene->removeArc(m_clickedItem, item);
						else
                            m_scene->setArcWeight(m_clickedItem,
                                                  item,
                                                  m_weightForNextLine);

						m_clickedItem = nullptr;
					}
				}
				else { // Если выделяемый элемент первый...
                    if (m_currentState == AddArc)
						item->setColor(QColor(0, 190, 190));
                    else if (m_currentState == RemoveArc)
						item->setColor(QColor(255, 128, 128));
                    else if (m_currentState == ChangeArcWeight)
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
