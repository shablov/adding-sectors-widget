#ifndef TABLESECTORSWIDGET_H
#define TABLESECTORSWIDGET_H

#include <QWidget>

#include "sectorsmodel.h"

class QTableView;

class QMenu;

class TableSectorsWidget : public QWidget
{
	Q_OBJECT

public:
	TableSectorsWidget(QWidget *parent = 0);

	void setModel(QAbstractTableModel *model);

private slots:
	void onContextMenuRequested(QPoint pos);

	void editRow();
	void addRow();
	void removeRow();
private:
	void createButtons();
	void createContextMenu();

	QTableView *tableView;
	SectorsModel *pModel;
	QMenu *contextMenu;
};

#endif // TABLESECTORSWIDGET_H
