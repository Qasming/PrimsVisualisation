#include "node.h"
#include "arc.h"

#include <QPainter>

Node::Node(const QSize &sizeNode,
		   const qint32 &widthLine,
		   QFont &fontNode,
		   qint32 indexNode) :
	m_index(indexNode),
	m_sizeNode(sizeNode),
	m_font(fontNode),
	m_widthLine(widthLine),
	m_color(QColor(0, 0, 0))
{
	refreshFont();
}

Node::Node(const Node &node) :
	m_index(node.m_index),
	m_sizeNode(node.m_sizeNode),
	m_font(node.m_font),
	m_widthLine(node.m_widthLine),
	m_color(node.m_color)
{
	setPos(node.pos());

	setArcs(node.listArc());
	const Node * const pItem = &node;
	foreach (Arc *line, m_listArcs) {
		if (pItem == line->node1())
			line->setNode1(this);
		else
			line->setNode2(this);
	}
}

Node::~Node()
{

}

QRectF Node::boundingRect() const
{
	return QRectF(0, 0, m_sizeNode.width(), m_sizeNode.height());
}

void Node::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *)
{
	p->setRenderHint(QPainter::Antialiasing);

	p->setPen(QPen(m_color, m_widthLine));
	p->setBrush(QBrush(QColor(255, 255, 255)));

	p->drawRoundRect(boundingRect(), 90, 90);

	if (m_index != -1) {
		p->setPen(QPen(m_color, 1));
		p->setBrush(Qt::NoBrush);

		p->setFont(m_font);
		p->drawText(boundingRect(),
					Qt::AlignHCenter | Qt::AlignCenter,
					QString::number(m_index));
	}
}

qint32 Node::index() const
{
	return m_index;
}

qint32 &Node::atIndex()
{
	return m_index;
}

QList<Arc *> Node::listArc() const
{
	return m_listArcs;
}

void Node::setArcs(QList<Arc *> listArc)
{
	if (m_listArcs.isEmpty())
		m_listArcs = listArc;
}

void Node::addArc(Arc *arc)
{
	m_listArcs.append(arc);
}

void Node::removeArc(Arc *arc)
{
	m_listArcs.removeOne(arc);
}

QColor Node::currentColor() const
{
	return m_color;
}

void Node::setColor(QColor newColor)
{
	m_color = newColor;
}

void Node::refreshFont()
{
	m_font.setPointSize(1);
	QFontMetrics fm(m_font);

	int fontSize = -1;

	do {
		++fontSize;
		m_font.setPointSize(fontSize + 1);
		fm = QFontMetrics(m_font);
	} while(fm.width(QString::number(m_index)) <= m_sizeNode.width() / 2 &&
			fm.height() <= m_sizeNode.height() * 0.8);

	m_font.setPointSize(fontSize);
}

QPointF Node::center() const
{
	return QPointF(pos().x() + m_sizeNode.width() / 2,
				   pos().y() + m_sizeNode.height() / 2);
}
