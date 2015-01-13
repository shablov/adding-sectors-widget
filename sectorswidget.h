#ifndef SECTORSWIDGET_H
#define SECTORSWIDGET_H

#include <QWidget>

#include "sectorsmodel.h"

class GraphicsSectorsWidget;
class TableSectorsWidget;
class QAbstractTableModel;
class QTabWidget;

class SectorsWidget : public QWidget
{
	Q_OBJECT
public:
	explicit SectorsWidget(QWidget *parent = 0);
	void setModel(SectorsModel *model);

	QList<Sector> sectors();

signals:

public slots:

private slots:
	void onSectorIntersected(Sector newSector, QList<Sector> intersectedSectors);

private:
	GraphicsSectorsWidget *graphicsWidget;
	TableSectorsWidget *tableWidget;
	QTabWidget *tabWidget;

	SectorsModel *pModel;
};

#endif // SECTORSWIDGET_H
