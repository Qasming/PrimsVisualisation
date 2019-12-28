#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>

class Arc;

class Node : public QGraphicsItem
{
public:
    Node(const QSize  &sizeNode,
         const qint32 &borderWidht,
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

    //!Список инцидентных ребер
	QList<Arc *> listArc() const;
    //!Добавить инцидентное
	void addArc(Arc *arc);
    //!Удаляет инцидентное
	void removeArc(Arc *arc);   
    //!Возвращает смежную вершину
    Node* adjacentNode(Arc* arc);

    //!Цвет вершины
    QColor currentColor() const;
    //!Установить цвет вершины
    void setColor(QColor newColor);
    //!Устанавливает нужный размер шрифта под размер узла.
	void refreshFont();
    //!Центр вершины в координатах
	QPointF center() const;

private:
    //!Список инцидентных ребер
	QList<Arc *>  m_listArcs;
    //!Индекс вершины (виден пользователю)
    qint32		  m_index;
    //!Размер вершины
	const QSize  &m_sizeNode;
    //!Шрифт индекса
	QFont		 &m_font;
    //!Ширина стенки
    const qint32 &m_borderWidth;
    //!Цвет вершины
    QColor		  m_color{QColor(0,0,0)};
};

#endif
