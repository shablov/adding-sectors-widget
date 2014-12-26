#ifndef TOOLS_H
#define TOOLS_H

class QWidget;

#include <QString>

class Tools
{
public:
	static void information(QWidget *parent, const QString &title,
			const QString &text, const QString &detailedText=QString());
	static void warning(QWidget *parent, const QString &title,
			const QString &text, const QString &detailedText=QString());
	static bool question(QWidget *parent, const QString &title,
			const QString &text, const QString &detailedText=QString());
};

#endif // TOOLS_H
