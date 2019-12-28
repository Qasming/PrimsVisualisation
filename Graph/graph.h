#ifndef GRAPH_H
#define GRAPH_H

#include <QGraphicsView>

class GraphScene;
class Node;
class Arc;

class Graph : public QGraphicsView
{
    Q_OBJECT

public:
	enum State { None,
                 AddNode, RemoveNode, MoveNode,
                 AddArc, RemoveArc, ChangeArcWeight };

	Graph(QWidget *parent = nullptr);
	~Graph() Q_DECL_OVERRIDE;

    bool addNode(QPoint pos);
    //! Удалить узел
    bool removeNode(Node* node);
	//! Переместит узел на xBy и yBy позиций.
    bool moveNode(Node* node, qint32 xBy, qint32 yBy);
	//! Устанавливает вес для последующих созданных дуг.
	bool setWeight(qint32 weight);

    bool addArc(Node* node1, Node*node2,qint32 weight =-1);
	//! Изменяет вес между двумя узлами.
    bool setWeight(Node* node1, Node*node2, qint32 weight);
    //! Удаляет дугу между двумя узлами с переданной индексацией узлов.
    bool removeArc(Node* node1, Node* node2);

	//! Очищает всю сцену.
	void clearAll();

	//! Изменяет размеры элементов.
	bool resizeItems(QSize size);
	//! Изменяет размеры веса дуг. Параметр — размер шрифта.
	bool resizeWeight(qint32 pointSize);
	//! Изменяет ширину линии у веса дуг.
	bool setWidthLineWeights(qint32 width);
	//! Устанавливает ширину линий внутри элементов.
	bool setWidthLineItems(qint32 width);
	//! Устанавливает ширину дуг.
	bool setWidthLines(qint32 width);

	//! Делает возможность перемещения сцены на ПКМ.
	void setSceneMovable(bool b);
	//! Возвращает возможность перемещения сцены на ПКМ.
	bool sceneIsMovable() const;

	//! Лист узлов.
	QList<Node *> listNode();
	//! Лист дуг.
	QList<Arc *> listArc();

    //! Возвращает дугу между вершинами
    Arc* getArc(Node *node1, Node *node2);

    void update();
private:
	GraphScene *m_scene;

    QPoint m_oldPos{QPoint(0, 0)};

    QCursor m_standardCursor{Qt::PointingHandCursor};
    QCursor m_movableCursor{Qt::SizeAllCursor};

    Node *m_clickedItem{nullptr};
    Node *m_pressedItem{nullptr};

    bool m_sceneIsMove{false};
    bool m_sceneIsMovable{true};

    bool m_itemIsMove{false};

    qint32 m_weightForNextLine{-1};
    qint32 m_weightLine{State::None};

	State m_currentState;

protected:
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

public slots:
    void setState(State st);
};
#endif
