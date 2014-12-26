#include "tablesectorswidget.h"

#include "sectorsmodel.h"

#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>

#include <QVBoxLayout>

#include <QDebug>

TableSectorsWidget::TableSectorsWidget(QWidget *parent)
	: QWidget(parent), pModel(0)
{
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	tableView = new QTableView;
	tableView->horizontalHeader()->setStretchLastSection(true);
	mainLayout->addWidget(tableView);

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	QPushButton *addButton = new QPushButton(tr("Add"));
	QPushButton *removeButton = new QPushButton(tr("Remove"));
	connect(addButton, SIGNAL(clicked()), this, SLOT(addRow()));
	connect(removeButton, SIGNAL(clicked()), this, SLOT(removeRow()));
	buttonLayout->addStretch();
	buttonLayout->addWidget(addButton);
	buttonLayout->addWidget(removeButton);
	mainLayout->addLayout(buttonLayout);
}

void TableSectorsWidget::setModel(QAbstractTableModel *model)
{
	pModel = qobject_cast<SectorsModel*>(model);
	if (pModel)
	{
		tableView->setModel(pModel);
	}
}

void TableSectorsWidget::addRow()
{
	if (pModel)
	{
		pModel->insertRow(pModel->rowCount());
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
