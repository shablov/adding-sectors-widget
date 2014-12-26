#include "graphicssectorswidget.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QVBoxLayout>

#include <math.h>


GraphicsSectorsWidget::GraphicsSectorsWidget(QWidget *parent)
	: QWidget(parent), padding(40), scalePadding(5),
	  standardOffset(90), scaleCount(8)
{
	setDefaultSettings();
	createGraphics();
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(view);
}

void GraphicsSectorsWidget::setDefaultSettings()
{

	sectorsBrush = QBrush(Qt::red);
	sectorsPen = QPen();

	backgroundBrush = QBrush(Qt::white);

	circleBrush = QBrush(Qt::white);
	scalePen = QPen(Qt::black, 2);

	mCCW = 1;
	mOffset = 0;
	minimum = 0;
	maximum = 360;
}

void GraphicsSectorsWidget::createGraphics()
{
	createGraphicsScene();
	createGraphicsView();
}

void GraphicsSectorsWidget::createGraphicsScene()
{
	scene = new QGraphicsScene;
	scene->installEventFilter(this);
}

void GraphicsSectorsWidget::createGraphicsView()
{
	view = new QGraphicsView;
	view->setRenderHint(QPainter::Antialiasing);
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setScene(scene);
}

void GraphicsSectorsWidget::setModel(SectorsModel *model)
{
	pModel = model;
	connect(pModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(onDataChanged(QModelIndex, QModelIndex)));
}

void GraphicsSectorsWidget::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
	redraw();
}


void GraphicsSectorsWidget::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	redraw();
}


void GraphicsSectorsWidget::redraw()
{
	clearScene();
	calculateDrawRect();
	drawScale();
	drawSectors();
}

void GraphicsSectorsWidget::clearScene()
{
	scene->clear();
	scene->setSceneRect(view->rect());
	scene->setBackgroundBrush(backgroundBrush);
}

void GraphicsSectorsWidget::calculateDrawRect()
{
	QRectF sceneRect = scene->sceneRect();
	qreal side = qMin(sceneRect.width(), sceneRect.height());
	side -= (padding + scalePadding)* 2;
	QPointF centerPoint = sceneRect.center();

	drawRect = QRectF();
	drawRect.setWidth(side);
	drawRect.setHeight(side);
	drawRect.moveCenter(centerPoint);
}

void GraphicsSectorsWidget::drawScale()
{
	QPointF centerPoint = drawRect.center();
	QLineF scaleLine(centerPoint, QPointF(drawRect.right() + scalePadding, centerPoint.y()));
	QLineF textLine(scaleLine);
	textLine.setLength(textLine.length() + 20);

	scaleLine.setAngle(generalOffset());
	textLine.setAngle(generalOffset());
	qreal scaleStep = 360 / scaleCount * mCCW;
	qreal scaleTextStep = range() / scaleCount;
	for (int i = 0; i < scaleCount; i++)
	{
		scene->addLine(scaleLine, scalePen);
		QString scaleText = mPrefix + QString::number(minimum + scaleTextStep * i) + mSuffix;
		QGraphicsTextItem *textItem = scene->addText(scaleText);

		QRectF boundRect = textItem->boundingRect();
		boundRect.moveCenter(textLine.p2());
		textItem->setPos(boundRect.topLeft());

		scaleLine.setAngle(scaleLine.angle() + scaleStep);
		textLine.setAngle(textLine.angle() + scaleStep);
	}

	scene->addEllipse(drawRect, QPen(), circleBrush);
}

void GraphicsSectorsWidget::drawSectors()
{
	QListIterator<Sector> i(sectors());
	while(i.hasNext())
	{
		Sector sector = i.next();
		QGraphicsEllipseItem *ellipse = scene->addEllipse(drawRect, sectorsPen, sectorsBrush);

		qreal firstAngle = angleFromValue(sector.begin);
		qreal secondAngle = angleFromValue(sector.end);
		int startAngle = (firstAngle * mCCW + generalOffset()) * 16;
		int spanAngle = fmod(secondAngle - firstAngle + 360, 360) * 16 * mCCW;
		ellipse->setStartAngle(startAngle);
		ellipse->setSpanAngle(spanAngle);
	}
}

qreal GraphicsSectorsWidget::range()
{
	return maximum - minimum;
}

qreal GraphicsSectorsWidget::generalOffset()
{
	return mOffset + standardOffset;
}

QList<Sector> GraphicsSectorsWidget::sectors()
{
	return pModel->sectors();
}

qreal GraphicsSectorsWidget::angleFromValue(const qreal &value)
{
	return ((value - minimum) * 360 / (range()));
}

qreal GraphicsSectorsWidget::valueFromAngle(const qreal &angle)
{
	return angle / 360 * range() + minimum;
}
