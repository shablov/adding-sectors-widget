#ifndef GRAPHICSSECTORSWIDGET_H
#define GRAPHICSSECTORSWIDGET_H

#include <QWidget>

#include <QPen>

#include "sectorsmodel.h"

class SectorsModel;

class QGraphicsScene;
class QGraphicsView;
class QGraphicsEllipseItem;

class QModelIndex;

class QMenu;

class GraphicsSectorsWidget : public QWidget
{
	Q_OBJECT
public:
	GraphicsSectorsWidget(QWidget *parent = 0);

	void setOffset(qreal offset);
	void setDirection(bool directionCW);
	void setScaleParameters(qreal offset, bool directionCW = true);

	void setSectorsBrush(const QBrush &brush);
	void setSectorsPen(const QPen &pen);
	void setBackgroundBrush(const QBrush &brush);
	void setCircleBrush(const QBrush &brush);
	void setScalePen(const QPen &pen);

	void setMinimum(const qreal &min);
	void setMaximum(const qreal &max);
	void setRange(const qreal &min, const qreal &max);
	void setPrefix(const QString &prefix);
	void setSuffix(const QString &suffix);

	void setModel(SectorsModel *model);

protected:
	void resizeEvent(QResizeEvent *event);
	bool eventFilter(QObject *obj, QEvent *event);

private slots:
	void onDataChanged(const QModelIndex &, const QModelIndex &);

	void editSector();
	void removeSector();

private:
	void setDefaultSettings();

	void createSceneContextMenu();
	void createGraphics();
	void createGraphicsScene();
	void createGraphicsView();

	void redraw();
	void clearScene();
	void calculateDrawRect();
	void drawScale();
	void drawSectors();
	QGraphicsEllipseItem *drawSector(const Sector &sector);

	void drawNewSector(qreal value);
	void saveSectorIfPossible();
	void addSector(const Sector &sector);
	Sector mAddedSector;
	QGraphicsEllipseItem *pAddedEllipseSector;

	void handleMouseEvent(QEvent *event);
	void sceneContextMenuEvent(QEvent *event);
	qreal calculateValue(const QPointF &point);
	bool leftButtonWasPressed();
	Qt::MouseButton buttonPressed;
	QMenu *sceneContextMenu;

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
