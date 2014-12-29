#ifndef SECTORSMODEL_H
#define SECTORSMODEL_H

#include <QAbstractItemModel>

struct Sector
{
	Sector();
	Sector(const Sector &sector);
	qreal begin;
	qreal end;
	qint32 id;

	void normalize();
	bool isEmpty();
	bool isValid();
	bool intersects(const Sector &sector) const;
	Sector united(const Sector &sector);

	friend bool operator == (const Sector &left, const Sector &right);
};

class SectorsModel : public QAbstractTableModel
{
	Q_OBJECT
	enum SectorsColumn
	{
		BeginSector,
		EndSector,
		Size
	};
public:
	explicit SectorsModel(QObject *parent = 0);

	// QAbstractItemModel interface
	virtual int rowCount(const QModelIndex & = QModelIndex()) const;
	virtual int columnCount(const QModelIndex & = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	virtual bool insertRows(int row, int count, const QModelIndex &parent);
	virtual bool removeRows(int row, int count, const QModelIndex &parent);
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;

	void uniteSectors(Sector &first, QList<Sector> intersectedSectors);
	void appendSector(const Sector &sector);
	QList<Sector> sectors();

signals:
	void sectorIntersected(Sector sector, QList<Sector> intersectedSector);

public slots:

private:
	bool isOutRange(const QModelIndex &index) const;
	/**
	 * @brief toDegrees - converted QVariant value to degree 0 - 359.99
	 * @param value - QVariant value
	 * @return - degrees, if the conversion is successful, otherwise -1
	 */
	qreal toDegrees(const QVariant &value);
	bool degreesOutOfRange(qreal degrees);
	QList<Sector> intersectsSectors(const Sector &sector);

	QList<Sector> mSectors;
	quint32 idQueueSectors;
};

#endif // SECTORSMODEL_H
