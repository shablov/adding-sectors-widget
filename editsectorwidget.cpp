#include "editsectorwidget.h"

#include "tools.h"

#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>

#include <QFormLayout>
#include <QLayout>

EditSectorWidget::EditSectorWidget(QWidget *parent = 0) :
	QDialog(parent), degreeChar(0x00B0)
{
	setLayout(new QVBoxLayout());
	createCentralWidgets();
	createButtons();
}

void EditSectorWidget::createCentralWidgets()
{
	QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(layout());
	if (!mainLayout)
	{
		return;
	}

	QFormLayout *widgetsLayout = new QFormLayout;
	mainLayout->addLayout(widgetsLayout);

	beginSectorSB = new QDoubleSpinBox;
	beginSectorSB->setRange(0, 360);
	beginSectorSB->setDecimals(2);
	beginSectorSB->setSuffix(degreeChar);


	endSectorSB = new QDoubleSpinBox;
	endSectorSB->setRange(0, 360);
	endSectorSB->setDecimals(2);
	endSectorSB->setSuffix(degreeChar);

	widgetsLayout->addRow("Begin sector: ", beginSectorSB);
	widgetsLayout->addRow("End sector: ", endSectorSB);
}

void EditSectorWidget::createButtons()
{
	QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(layout());
	if (!mainLayout)
	{
		return;
	}

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	mainLayout->addLayout(buttonsLayout);

	QPushButton *okButton = new QPushButton(tr("Ok"));
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	QPushButton *cancelButton = new QPushButton(tr("Cancel"));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	buttonsLayout->addStretch();
	buttonsLayout->addWidget(okButton);
	buttonsLayout->addWidget(cancelButton);
}

void EditSectorWidget::setSector(const Sector &sector)
{
	mSector = sector;
	beginSectorSB->setValue(sector.begin);
	endSectorSB->setValue(sector.end);
}

Sector EditSectorWidget::sector()
{
	mSector.begin = beginSectorSB->value();
	mSector.end = endSectorSB->value();
	return mSector;
}

void EditSectorWidget::accept()
{
	if (beginSectorSB->value() == endSectorSB->value())
	{
		Tools::information(this, "Information",
						   "Beginning and ending of sector are equal!", "Correct this");
		return;
	}
	QDialog::accept();
}

