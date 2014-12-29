#include "graphicssectorswidget.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QVBoxLayout>
#include <QMenu>

#include <QEvent>
#include <QGraphicsSceneMouseEvent>

#include <math.h>

#include <QDebug>


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

void GraphicsSectorsWidget::setOffset(qreal offset)
{
	mOffset = offset;
	redraw();
}

void GraphicsSectorsWidget::setDirection(bool directionCW)
{
	mCCW = directionCW ? -1 : 1;
	mOffset *= mCCW;
	redraw();
}

void GraphicsSectorsWidget::setScaleParameters(qreal offset, bool directionCW)
{
	mCCW = directionCW ? -1 : 1;
	mOffset = offset;
	mOffset *= mCCW;
	redraw();
}

void GraphicsSectorsWidget::setSectorsBrush(const QBrush &brush)
{
	sectorsBrush = brush;
	redraw();
}

void GraphicsSectorsWidget::setSectorsPen(const QPen &pen)
{
	sectorsPen = pen;
	redraw();
}

void GraphicsSectorsWidget::setBackgroundBrush(const QBrush &brush)
{
	backgroundBrush = brush;
	redraw();
}

void GraphicsSectorsWidget::setCircleBrush(const QBrush &brush)
{
	circleBrush = brush;
	redraw();
}

void GraphicsSectorsWidget::setScalePen(const QPen &pen)
{
	scalePen = pen;
	redraw();
}

void GraphicsSectorsWidget::setMinimum(const qreal &min)
{
	minimum = min;
	redraw();
}

void GraphicsSectorsWidget::setMaximum(const qreal &max)
{
	maximum = max;
	redraw();
}

void GraphicsSectorsWidget::setRange(const qreal &min, const qreal &max)
{
	minimum = min;
	maximum = max;
	redraw();
}

void GraphicsSectorsWidget::setPrefix(const QString &prefix)
{
	mPrefix = prefix;
	redraw();
}

void GraphicsSectorsWidget::setSuffix(const QString &suffix)
{
	mSuffix = suffix;
	redraw();
}

void GraphicsSectorsWidget::setModel(SectorsModel *model)
{
	pModel = model;
	connect(pModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(onDataChanged(QModelIndex, QModelIndex)));
	redraw();
}

void GraphicsSectorsWidget::onDataChanged(const QModelIndex &/*topLeft*/, const QModelIndex &/*bottomRight*/)
{
	redraw();
}


void GraphicsSectorsWidget::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	redraw();
}

bool GraphicsSectorsWidget::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::GraphicsSceneMouseMove ||
		event->type() == QEvent::GraphicsSceneMousePress ||
		event->type() == QEvent::GraphicsSceneMouseRelease)
	{
		handleEvent(event);
	}
	return QWidget::eventFilter(obj, event);
}

void GraphicsSectorsWidget::handleEvent(QEvent *event)
{
	QGraphicsSceneMouseEvent *mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
	if (!mouseEvent)
	{
		return;
	}
	qreal value = calculateValue(mouseEvent->scenePos());
	switch (mouseEvent->type())
	{
		case QEvent::GraphicsSceneMouseMove:
		{
			if (!leftButtonWasPressed())
			{
				break;
			}
			drawNewSector(value);
			break;
		}
		case QEvent::GraphicsSceneMousePress:
		{
			buttonPressed = mouseEvent->button();
			if (!leftButtonWasPressed())
			{
				break;
			}
			mAddedSector.begin = value;
			mAddedSector.end = value;
			drawNewSector(value);
			break;
		}
		case QEvent::GraphicsSceneMouseRelease:
		{
			if (!leftButtonWasPressed())
			{
				break;
			}
			buttonPressed = Qt::NoButton;
			saveSectorIfPossible();
		}
		default: break;
	}
}

void GraphicsSectorsWidget::drawNewSector(qreal value)
{
	mAddedSector.end = value;
	if (!mAddedSector.isValid())
	{
		return;
	}
	if (pAddedEllipseSector)
	{
		scene->removeItem(pAddedEllipseSector);
	}
	pAddedEllipseSector = drawSector(mAddedSector);
}

void GraphicsSectorsWidget::saveSectorIfPossible()
{
	if (pAddedEllipseSector)
	{
		scene->removeItem(pAddedEllipseSector);
		pAddedEllipseSector = 0;
	}

	if (mAddedSector.isValid())
	{
		mAddedSector.normalize();
		addSector(Sector(mAddedSector));
	}
	mAddedSector = Sector();
}

void GraphicsSectorsWidget::addSector(const Sector &sector)
{
	pModel->appendSector(sector);
}

qreal GraphicsSectorsWidget::calculateValue(const QPointF &point)
{
	QLineF line(drawRect.center(), point);
	qreal angle = fmod(((line.angle() - generalOffset()) * mCCW + 720), 360);
	return valueFromAngle(angle);
}

bool GraphicsSectorsWidget::leftButtonWasPressed()
{
	return buttonPressed == Qt::LeftButton;
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
		drawSector(sector);
	}
}

QGraphicsEllipseItem *GraphicsSectorsWidget::drawSector(const Sector &sector)
{
	QGraphicsEllipseItem *ellipse = scene->addEllipse(drawRect, sectorsPen, sectorsBrush);
	ellipse->setFlags(ellipse->flags() | QGraphicsItem::ItemIsFocusable);

	qreal firstAngle = angleFromValue(sector.begin);
	qreal secondAngle = angleFromValue(sector.end);
	int startAngle = (firstAngle * mCCW + generalOffset()) * 16;
	int spanAngle = fmod(secondAngle - firstAngle + 360, 360) * 16 * mCCW;
	ellipse->setStartAngle(startAngle);
	ellipse->setSpanAngle(spanAngle);

	return ellipse;
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
