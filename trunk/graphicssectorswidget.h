#ifndef GRAPHICSSECTORSWIDGET_H
#define GRAPHICSSECTORSWIDGET_H

#include <QWidget>

#include <QPen>

#include "sectorsmodel.h"

class SectorsModel;

class QGraphicsScene;
class QGraphicsView;

class QModelIndex;

class GraphicsSectorsWidget : public QWidget
{
	Q_OBJECT
public:
	GraphicsSectorsWidget(QWidget *parent = 0);

	void setModel(SectorsModel *model);

protected:
	void resizeEvent(QResizeEvent *event);

private slots:
	void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

private:
	void setDefaultSettings();

	void createGraphics();
	void createGraphicsScene();
	void createGraphicsView();

	void redraw();
	void clearScene();
	void calculateDrawRect();
	void drawScale();
	void drawSectors();


	QGraphicsView *view;
	QBrush backgroundBrush;
	QGraphicsScene *scene;

	QBrush circleBrush;
	QPen scalePen;
	QRectF drawRect;

	qreal range();
	qreal minimum;
	qreal maximum;

	QString mSuffix;
	QString mPrefix;

	/**
	 * @brief mCCW - направление шкалы: -1 по часовой, иначе 1
	 */
	int mCCW;

	qreal generalOffset();

	/**
	 * @brief mOffset - смещение шкалы
	 */
	qreal mOffset;

	QBrush sectorsBrush;
	QPen sectorsPen;

	SectorsModel *pModel;
	QList<Sector> sectors();

	qreal angleFromValue(const qreal &value);
	qreal valueFromAngle(const qreal &angle);

	/**
	 * @brief padding - учитывает отступы и длину, высоту текста надписи
	 */
	const qreal padding;
	/**
	 * @brief scalePadding - высота риски координатной шкалы
	 */
	const qreal scalePadding;
	/**
	 * @brief standartOffset - стандартный поворот координатной шкалы (0° вверху)
	 */
	const qreal standardOffset;
	/**
	 * @brief scaleStep - шаг координатной шкалы
	 */
	const qreal scaleCount;
};

#endif // GRAPHICSSECTORSWIDGET_H
