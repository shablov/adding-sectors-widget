#include "tablesectorswidget.h"
#include "editsectorwidget.h"

#include "sectorsmodel.h"

#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMenu>

#include <QEvent>
#include <QContextMenuEvent>

TableSectorsWidget::TableSectorsWidget(QWidget *parent)
	: QWidget(parent), pModel(0)
{
	tableView = new QTableView;
	tableView->horizontalHeader()->setStretchLastSection(true);
	tableView->setContextMenuPolicy(Qt::CustomContextMenu);
	tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	connect(tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onContextMenuRequested(QPoint)));

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(tableView);

	createButtons();
	createContextMenu();
}

void TableSectorsWidget::createButtons()
{
	QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(layout());

	QPushButton *editButton = new QPushButton(tr("Edit"));
	QPushButton *addButton = new QPushButton(tr("Add"));
	QPushButton *removeButton = new QPushButton(tr("Remove"));
	connect(editButton, SIGNAL(clicked()), this, SLOT(editRow()));
	connect(addButton, SIGNAL(clicked()), this, SLOT(addRow()));
	connect(removeButton, SIGNAL(clicked()), this, SLOT(removeRow()));

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(editButton);
	buttonLayout->addStretch();
	buttonLayout->addWidget(addButton);
	buttonLayout->addWidget(removeButton);
	mainLayout->addLayout(buttonLayout);
}

void TableSectorsWidget::createContextMenu()
{
	contextMenu = new QMenu;
	contextMenu->addAction(tr("Edit sector"), this, SLOT(editRow()));
	contextMenu->addAction(tr("Remove sector"), this, SLOT(removeRow()));
}

void TableSectorsWidget::setModel(QAbstractTableModel *model)
{
	pModel = qobject_cast<SectorsModel*>(model);
	if (pModel)
	{
		tableView->setModel(pModel);
	}
}

void TableSectorsWidget::onContextMenuRequested(QPoint pos)
{
	if (tableView->indexAt(pos).isValid())
	{
		contextMenu->exec(tableView->viewport()->mapToGlobal(pos));
	}
}

void TableSectorsWidget::editRow()
{
	if (!pModel)
	{
		return;
	}

	QModelIndex index = tableView->currentIndex();
	Sector sector = pModel->sector(index);
	if (sector.isEmpty())
	{
		return;
	}

	EditSectorWidget *editSectorWidget = new EditSectorWidget(this);
	editSectorWidget->setSector(sector);
	if (editSectorWidget->exec() == QDialog::Accepted)
	{
		pModel->appendSector(editSectorWidget->sector());
	}
}

void TableSectorsWidget::addRow()
{
	if (!pModel)
	{
		return;
	}

	EditSectorWidget *editSectorWidget = new EditSectorWidget(this);
	editSectorWidget->setSector(Sector());
	if (editSectorWidget->exec() == QDialog::Accepted)
	{
		pModel->appendSector(editSectorWidget->sector());
	}
}

void TableSectorsWidget::removeRow()
{
	if (pModel)
	{
		QModelIndex index = tableView->currentIndex();
		if (index.isValid())
		{
			pModel->removeRow(index.row());
		}
	}
}
