#include "arc.h"
#include "node.h"

#include <QPainter>
#include <QDebug>

Arc::Arc(Node *node1,
		 Node *node2,
		 qint32 widthLine,
		 const QFont &fontWeight,
         const qint32 &borderWidthWeight,
		 qint32 weight) :
	QGraphicsLineItem(),
	m_fontWeight(fontWeight),
    m_borderWidthWeight(borderWidthWeight),
	m_weight(weight)
{	
	m_node1 = node1;
	m_node2 = node2;

	setPen(QPen(QColor(0, 0, 0), widthLine));
	setLine(QLineF(node1->center(), node2->center()));
}

Arc::~Arc()
{

}

Node *Arc::node1() const
{
	return m_node1;
}

Node *Arc::node2() const
{
	return m_node2;
}

void Arc::setNode1(Node *node)
{
	m_node1 = node;
}

void Arc::setNode2(Node *node)
{
	m_node2 = node;
}

void Arc::setLine(QLineF line)
{
	QRectF rect(line.p1(), line.p2());
	m_center = rect.center();
	// Перемещаем вес в новый центр.
	refreshRectForWeight();
    QGraphicsLineItem::setLine(line);
}

void Arc::paint(QPainter *painter,
                const QStyleOptionGraphicsItem *option,
                QWidget *widget)
{
	painter->setRenderHint(QPainter::Antialiasing);
	QGraphicsLineItem::paint(painter, option, widget);
    QGraphicsLineItem::setPen(QPen(QColor(m_color),3));

	if (m_weight != -1) {
        painter->setPen(QPen(m_color, m_borderWidthWeight));
        painter->setBrush(m_weightBrush);
		painter->drawRect(m_rectForWeight);

		painter->setFont(m_fontWeight);
		painter->drawText(m_rectForWeight,
						  Qt::AlignCenter | Qt::AlignHCenter,
						  QString::number(m_weight));
	}
}

QPointF Arc::center() const
{
	return m_center;
}

void Arc::refreshRectForWeight()
{
	QFontMetrics fm(m_fontWeight);
	qreal width = fm.width(QString::number(m_weight)) + 5;
	qreal height = fm.height() + 5;
	m_rectForWeight = QRectF(m_center.x() - width / 2,
							 m_center.y() - height / 2,
							 width,
							 height);
}

qint32 Arc::weight() const
{
	return m_weight;
}

void Arc::setWeight(const qint32 &weight)
{
	m_weight = weight;
    refreshRectForWeight();
}

void Arc::setBrushWeight(const QColor &color)
{
    m_weightBrush = color;
}

void Arc::setColor(const QColor &color)
{
    m_color = color;
}
