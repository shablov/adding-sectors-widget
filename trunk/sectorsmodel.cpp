#include "sectorsmodel.h"

SectorsModel::SectorsModel(QObject *parent) :
	QAbstractTableModel(parent)
{
	idQueueSectors = 0;
}


int SectorsModel::rowCount(const QModelIndex &) const
{
	return mSectors.count();
}

int SectorsModel::columnCount(const QModelIndex &) const
{
	return Size;
}

QVariant SectorsModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || isOutRange(index))
	{
		return QVariant();
	}

	const Sector &sector = mSectors.at(index.row());

	if (role == Qt::EditRole || role == Qt::DisplayRole)
	{
		SectorsColumn column = static_cast<SectorsColumn>(index.column());
		switch (column)
		{
			case BeginSector: return sector.begin;
			case EndSector: return sector.end;
			case Size:
			default: break;
		}
	}
	return QVariant();
}

bool SectorsModel::isOutRange(const QModelIndex &index) const
{
	return index.row() < 0 || index.row() > rowCount() ||
			index.column() < 0 || index.column() > columnCount();
}

bool SectorsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid() || isOutRange(index))
	{
		return false;
	}

	Sector &sector = mSectors[index.row()];

	if (role == Qt::EditRole || role == Qt::DisplayRole)
	{
		SectorsColumn column = static_cast<SectorsColumn>(index.column());
		switch (column)
		{
			case BeginSector:
			{
				qreal begin = toDegrees(value);
				if (begin == -1)
				{
					return false;
				}
				qSwap(sector.begin, begin);
				QList<Sector> intersectedSectors = intersectsSectors(sector);
				if (!intersectedSectors.isEmpty())
				{
					Sector newSector(sector);
					qSwap(sector.begin, begin);
					emit sectorIntersected(newSector, intersectedSectors);
					return false;
				}
				break;
			}
			case EndSector:
			{
				qreal end = toDegrees(value);
				if (end == -1)
				{
					return false;
				}
				qSwap(sector.end, end);
				QList<Sector> intersectedSectors = intersectsSectors(sector);
				if (!intersectedSectors.isEmpty())
				{
					Sector tmp(sector);
					qSwap(sector.end, end);
					emit sectorIntersected(tmp, intersectedSectors);
					return false;
				}
				break;
			}
			case Size:
			default: return false;
		}
	}
	emit dataChanged(index, index);
	return true;
}

qreal SectorsModel::toDegrees(const QVariant &value)
{
	bool ok = false;
	qreal degree = value.toReal(&ok);
	if (!ok || (degreesOutOfRange(degree)))
	{
		degree = -1;
	}
	return degree;
}

bool SectorsModel::degreesOutOfRange(qreal degrees)
{
	return (degrees < 0) || (degrees > 360);
}

QList<Sector> SectorsModel::intersectsSectors(const Sector &sector)
{
	QList<Sector> result;
	QListIterator<Sector> i(mSectors);
	while (i.hasNext())
	{
		Sector probablyIntersectsSector = i.next();
		if (sector.intersects(probablyIntersectsSector))
		{
			result.append(probablyIntersectsSector);
		}
	}
	return result;
}

QVariant SectorsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Vertical)
	{
		return section + 1;
	}

	if (role == Qt::DisplayRole)
	{
		/// TODO: переделать на использование словаря
		switch (section)
		{
			case BeginSector: return tr("Begin");
			case EndSector: return tr("End");
			default: break;
		}
	}
	return QVariant();
}

bool SectorsModel::insertRows(int row, int count, const QModelIndex &parent)
{
	beginInsertRows(parent, row, row + count - 1);
	for (int i = row; i < row + count; i++)
	{
		Sector sector;
		sector.id = idQueueSectors++;

		mSectors.insert(i, sector);
	}
	endInsertRows();
	QAbstractItemModel::reset();
	return true;
}

bool SectorsModel::removeRows(int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(parent, row, row + count - 1);
	for (int i = row; i < row + count; i++)
	{
		mSectors.removeAt(i);
	}
	endRemoveRows();
	QAbstractItemModel::reset();
	return true;
}

Qt::ItemFlags SectorsModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags flags = QAbstractItemModel::flags(index);
	if (index.isValid() && !isOutRange(index))
	{
		flags |= Qt::ItemIsEditable | Qt::ItemIsEnabled;
	}
	return flags;
}

void SectorsModel::uniteSectors(Sector &first, QList<Sector> intersectedSectors)
{
	int firstId = first.id;
	foreach (Sector sector, intersectedSectors)
	{
		first = first.united(sector);
		removeRow(mSectors.indexOf(sector));
	}
	first.id = firstId;
	int indexFirstSector = mSectors.indexOf(first);
	mSectors.replace(indexFirstSector, first);
	reset();
}

void SectorsModel::appendSector(const Sector &sector)
{
	QList<Sector> intersectedSectors = intersectsSectors(sector);
	if (intersectedSectors.isEmpty())
	{
		insertRow(-1);
		mSectors.replace(mSectors.count(), sector);
		return;
	}
	emit sectorIntersected(sector, intersectedSectors);
}

QList<Sector> SectorsModel::sectors()
{
	return mSectors;
}

Sector::Sector()
{
	begin = 0;
	end = 0;
	id = -1;
}

Sector::Sector(const Sector &sector)
{
	begin = sector.begin;
	end = sector.end;
	id = sector.id;
}

bool Sector::isEmpty()
{
	return id == -1;
}

bool Sector::intersects(const Sector &sector) const
{
	if (sector.id == id)
	{
		return false;
	}

	if ((begin < end) && (sector.begin < sector.end))
	{
		return  ((begin <= sector.begin) && (sector.begin <= end)) ||
				((begin <= sector.end) && (sector.end <= end)) ||
				((sector.begin <= begin) && (begin <= sector.end)) ||
				((sector.begin <= begin) && (begin <= sector.end));
	}
	if ((begin < end) && (sector.begin > sector.end))
	{
		return (begin <= sector.end) || (sector.begin <= end);
	}
	if ((begin > end) && (sector.begin > sector.end))
	{
		return  true;
	}
	if ((begin > end) && (sector.begin < sector.end))
	{
		return (sector.begin <= end) || (begin <= sector.end);
	}
	return false;
}

Sector Sector::united(const Sector &sector)
{
	Sector resultingSector;
	if ((begin < end) && (sector.begin < sector.end))
	{
		resultingSector.begin = qMin(begin, sector.begin);
		resultingSector.end = qMax(end, sector.end);
	}
	if ((begin < end) && (sector.begin > sector.end))
	{
		if (sector.end >= begin)
		{
			resultingSector.end = qMin(sector.end, begin);
			resultingSector.begin = sector.begin;
		}
		if (sector.begin <= end)
		{
			resultingSector.begin = qMin(begin, sector.begin);
			resultingSector.end = sector.end;
		}
	}
	if ((begin > end) && (sector.begin > sector.end))
	{
		resultingSector.begin = qMin(begin, sector.begin);
		resultingSector.end = qMax(end, sector.end);
		if (resultingSector.begin < resultingSector.end)
		{
			resultingSector.begin = resultingSector.end;
		}
	}
	if ((begin > end) && (sector.begin < sector.end))
	{
		if (end >= sector.begin)
		{
			resultingSector.end = qMin(sector.end, begin);
			resultingSector.begin = begin;
		}
		if (begin <= sector.end)
		{
			resultingSector.begin = qMin(begin, sector.begin);
			resultingSector.end = end;
		}
	}
	return resultingSector;
}

bool operator ==(const Sector &left, const Sector &right)
{
	return left.id == right.id;
}
