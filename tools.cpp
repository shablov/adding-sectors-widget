#include "tools.h"

#include <QApplication>
#include <QScopedPointer>
#include <QSharedPointer>

#include <QMessageBox>
#include <QWidget>
#include <QPushButton>

void Tools::information(QWidget *parent, const QString &title, const QString &text,
						const QString &detailedText)
{
#if QT_VERSION >= 0x040600
	QScopedPointer<QMessageBox> messageBox(new QMessageBox(parent));
#else
	QSharedPointer<QMessageBox> messageBox(new QMessageBox(parent));
#endif
	if (parent)
	{
		messageBox->setWindowModality(Qt::WindowModal);
	}
	messageBox->setWindowTitle(title);
	messageBox->setText(text);
	if (!detailedText.isEmpty())
	{
		messageBox->setInformativeText(detailedText);
	}
	messageBox->setIcon(QMessageBox::Information);
	messageBox->addButton(QMessageBox::Ok);
	messageBox->exec();
}

void Tools::warning(QWidget *parent, const QString &title, const QString &text,
					const QString &detailedText)
{
#if QT_VERSION >= 0x040600
	QScopedPointer<QMessageBox> messageBox(new QMessageBox(parent));
#else
	QSharedPointer<QMessageBox> messageBox(new QMessageBox(parent));
#endif
	if (parent)
	{
		messageBox->setWindowModality(Qt::WindowModal);
	}
	messageBox->setWindowTitle(title);
	messageBox->setText(text);
	if (!detailedText.isEmpty())
	{
		messageBox->setInformativeText(detailedText);
	}
	messageBox->setIcon(QMessageBox::Warning);
	messageBox->addButton(QMessageBox::Ok);
	messageBox->exec();
}

bool Tools::question(QWidget *parent, const QString &title, const QString &text,
					 const QString &detailedText)
{
#if QT_VERSION >= 0x040600
	QScopedPointer<QMessageBox> messageBox(new QMessageBox(parent));
#else
	QSharedPointer<QMessageBox> messageBox(new QMessageBox(parent));
#endif
	if (parent)
	{
		messageBox->setWindowModality(Qt::WindowModal);
	}
	messageBox->setWindowTitle(title);
	messageBox->setText(text);
	if (!detailedText.isEmpty())
	{
		messageBox->setInformativeText(detailedText);
	}
	messageBox->setIcon(QMessageBox::Question);
	QString yesText = "Yes";
	QAbstractButton *yesButton = messageBox->addButton(yesText,
				QMessageBox::AcceptRole);

	QString noText = "No";
	messageBox->addButton(noText, QMessageBox::RejectRole);
	messageBox->setDefaultButton(qobject_cast<QPushButton*>(yesButton));
	messageBox->exec();
	return messageBox->clickedButton() == yesButton;
}

