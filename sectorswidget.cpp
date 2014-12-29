#include "graphicssectorswidget.h"
#include "sectorswidget.h"
#include "tablesectorswidget.h"

#include "tools.h"

#include <QTabWidget>
#include <QVBoxLayout>

#include <QMessageBox>

#include <QDebug>

SectorsWidget::SectorsWidget(QWidget *parent) :
	QWidget(parent), graphicsWidget(0), tableWidget(0)
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
		graphicsWidget->setModel(model);
		graphicsWidget->setSuffix(QChar(0x00B0));
		tabWidget->addTab(graphicsWidget, tr("graphics"));
	}
	if (!tableWidget)
	{
		tableWidget = new TableSectorsWidget;
		tableWidget->setModel(model);
		tabWidget->addTab(tableWidget, tr("table"));
	}

	connect(model, SIGNAL(sectorIntersected(Sector, QList<Sector>)),
			this, SLOT(onSectorIntersected(Sector, QList<Sector>)));
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
