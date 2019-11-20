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
				 AddItem, RemoveItem, MoveItem,
				 AddLine, RemoveLine, ChangeLineWeight };

	Graph(QWidget *parent = nullptr);
	~Graph() Q_DECL_OVERRIDE;

	//! Возвратит индекс созданного узла.
	qint32 addNode(QPoint pos);
	//! Удалит узел по его индексу.
	bool removeNode(qint32 indexNode);
	//! Переместит узел на xBy и yBy позиций.
	bool moveNode(qint32 indexNode, qint32 xBy, qint32 yBy);

	//! Устанавливает вес для последующих созданных дуг.
	bool setWeight(qint32 weight);
	//! Добавляет дугу между двумя узлами с переданной индексацией узлов.
	bool addArc(qint32 indexNode_1, qint32 indexNode_2, qint32 weight = -1);
	//! Изменяет вес между двумя узлами.
	bool setWeight(qint32 indexNode_1, qint32 indexNode_2, qint32 weight);
	//! Удаляет дугу между двумя узлами с переданной индексацией узлов.
	bool removeArc(qint32 indexNode_1, qint32 indexNode_2);

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

	//! Эта функция возвращает корректный указатель на элемент из листа,
	//! т.е. если узел имеет индекс 2, то из листа вернется объект с индексом 1.
	//! Потому что индексы узлов начинаются с 1, а у листа с 0.
	Node *getItemOnIndex(qint32 indexItem);

	//! Возвращает текст ошибки, если один из методов с типом bool вернет false.
	const QString &error() const;

	//! Лист узлов.
	QList<Node *> listNode();
	//! Лист дуг.
	QList<Arc *> listArc();

private:
	GraphScene *m_scene;

	QPoint m_oldPos;

	QCursor m_standardCursor;
	QCursor m_movableCursor;

	Node *m_clickedItem;
	Node *m_pressedItem;

	bool m_sceneIsMove;
	bool m_sceneIsMovable;

	bool m_itemIsMove;

	qint32 m_weightForNextLine;
	qint32 m_weightLine;

	State m_currentState;

	QString m_strError;

protected:
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

public slots:
	void changeState(State st);
};
#endif
