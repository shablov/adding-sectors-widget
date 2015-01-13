#ifndef EDITSECTORWIDGET_H
#define EDITSECTORWIDGET_H

#include <QDialog>

#include "sectorsmodel.h"

class QDoubleSpinBox;

class EditSectorWidget : public QDialog
{
public:
	EditSectorWidget(QWidget *parent);
	void setSector(const Sector &sector);
	Sector sector();

public slots:
	void accept();

private:
	void createCentralWidgets();
	void createButtons();

	const QChar degreeChar;

	QDoubleSpinBox *beginSectorSB;
	QDoubleSpinBox *endSectorSB;
	Sector mSector;
};

#endif // EDITSECTORWIDGET_H
