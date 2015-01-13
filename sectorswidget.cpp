#include "graphicssectorswidget.h"
#include "sectorswidget.h"
#include "tablesectorswidget.h"

#include "tools.h"

#include <QTabWidget>
#include <QVBoxLayout>

SectorsWidget::SectorsWidget(QWidget *parent) :
	QWidget(parent), graphicsWidget(0), tableWidget(0), pModel(0)
{
	QVBoxLayout * mainLayout = new QVBoxLayout(this);
	tabWidget = new QTabWidget;
	mainLayout->addWidget(tabWidget);
	setModel(new SectorsModel());
}

void SectorsWidget::setModel(SectorsModel *model)
{
	if (!graphicsWidget)
	{
		graphicsWidget = new GraphicsSectorsWidget;
		tabWidget->addTab(graphicsWidget, tr("graphics"));
	}
	graphicsWidget->setModel(model);
	graphicsWidget->setSuffix(QChar(0x00B0));

	if (!tableWidget)
	{
		tableWidget = new TableSectorsWidget;
		tabWidget->addTab(tableWidget, tr("table"));
	}
	tableWidget->setModel(model);

	if (pModel)
	{
		delete pModel;
		pModel = model;
	}

	connect(model, SIGNAL(sectorIntersected(Sector, QList<Sector>)),
			this, SLOT(onSectorIntersected(Sector, QList<Sector>)));
}

QList<Sector> SectorsWidget::sectors()
{
	if (pModel)
	{
		return pModel->sectors();
	}
	return QList<Sector>();
}

void SectorsWidget::onSectorIntersected(Sector newSector, QList<Sector> intersectedSectors)
{
	QStringList sectorsList;
	int i = 1;
	sectorsList.append(QString("%1)%2 - %3").arg(i).arg(newSector.begin).arg(newSector.end));
	foreach (Sector sector, intersectedSectors)
	{
		sectorsList.append(QString("%1)%2 - %3").arg(++i).arg(sector.begin).arg(sector.end));
	}

	QString title = "Intersected sectors";
	QString text = "You want to merge the sectors?";
	if (!Tools::question(this, title, text, sectorsList.join("\n")))
	{
		return;
	}

	SectorsModel *model = qobject_cast<SectorsModel*>(sender());
	if (!model)
	{
		return;
	}

	model->uniteSectors(newSector, intersectedSectors);


}
