#ifndef ARC_H
#define ARC_H

#include <QGraphicsLineItem>

class Node;

class Arc : public QGraphicsLineItem
{
public:
	Arc(Node		 *node1,
		Node		 *node2,
		qint32		  widthLine,
		const QFont	 &fontWeight,
		const qint32 &widthLineWeight,
		qint32		  weight = -1);
	~Arc() Q_DECL_OVERRIDE;

	Node *node1() const;
	Node *node2() const;
	void setNode1(Node *node);
	void setNode2(Node *node);

	void setLine(QLineF line);

	void paint(QPainter *painter,
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget) Q_DECL_OVERRIDE;

	QPointF center() const;

	void refreshRectForWeight();

	qint32 weight() const;
	void setWeight(const qint32 &weight);
    void setColor(const QColor color);
private:
	Node		 *m_node1;
	Node		 *m_node2;

    const QFont	 &m_fontWeight;
	const qint32 &m_widthLineWeight;
	qint32		  m_weight;
	QRectF		  m_rectForWeight;

	QPointF m_center;
    QColor m_color{QColor(0,0,0)};
};

#endif
