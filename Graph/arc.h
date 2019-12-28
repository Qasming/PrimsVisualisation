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
        const qint32 &borderWidthWeight,
		qint32		  weight = -1);
	~Arc() Q_DECL_OVERRIDE;

    //!Вершина 1
	Node *node1() const;
    //!Вершина 2
	Node *node2() const;
    //!Устанавливает вершину 1
	void setNode1(Node *node);
    //!Устанавливает вершину 2
	void setNode2(Node *node);
    //!Возвращает вес ребра
    qint32 weight() const;
    //!Устанавливает вес ребра
    void setWeight(const qint32 &weight);

    //!Устанавливает линию
	void setLine(QLineF line);
    //!Отрисовка
	void paint(QPainter *painter,
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget) Q_DECL_OVERRIDE;

	QPointF center() const;

	void refreshRectForWeight();
     void setColor(const QColor &color);
     void setBrushWeight(const QColor &color);
private:
    //! Первая вершина
    Node    *m_node1;
    //!Вторая вершина
    Node	*m_node2;

    //!Шриф веса
	const QFont	 &m_fontWeight;
    //!Ширина стенки у рамки
    const qint32 &m_borderWidthWeight;
    //!Рамка веса
	QRectF		  m_rectForWeight;
    //!Центр ребра
    QPointF m_center;
    //!Цвет ребра
    QColor m_color{QColor(0,0,0)};
    //!Заливка рамки
    QColor m_weightBrush{QColor(255,255,255,255)};

    //!Вес ребра
    qint32		  m_weight;
};

#endif
