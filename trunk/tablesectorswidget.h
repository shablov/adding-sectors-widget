#ifndef TABLESECTORSWIDGET_H
#define TABLESECTORSWIDGET_H

#include <QWidget>

#include "sectorsmodel.h"

class QTableView;

class TableSectorsWidget : public QWidget
{
	Q_OBJECT

public:
	TableSectorsWidget(QWidget *parent = 0);

	void setModel(QAbstractTableModel *model);

private slots:
	void addRow();
	void removeRow();

private:
	QTableView *tableView;
	SectorsModel *pModel;
};

#endif // TABLESECTORSWIDGET_H
