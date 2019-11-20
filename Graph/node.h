#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>

class Arc;

class Node : public QGraphicsItem
{
public:
	Node(const QSize  &sizeNode,
		 const qint32 &widthLine,
		 QFont		  &fontNode,
		 qint32		   indexNode = -1);
	Node(const Node &node);
	~Node() override;

	QRectF boundingRect() const Q_DECL_OVERRIDE;
	void paint(QPainter *painter,
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget) Q_DECL_OVERRIDE;

	qint32 index() const;
	qint32 &atIndex();

	QList<Arc *> listArc() const;
	void setArcs(QList<Arc *> listArcs);
	void addArc(Arc *arc);
	void removeArc(Arc *arc);

	QColor currentColor() const;
	void setColor(QColor newColor);

	// Устанавливает нужный размер шрифта под размер узла.
	void refreshFont();

	QPointF center() const;

private:
	qint32		  m_index;

	QList<Arc *>  m_listArcs;

	const QSize  &m_sizeNode;
	QFont		 &m_font;
	const qint32 &m_widthLine;
	QColor		  m_color;
};

#endif
