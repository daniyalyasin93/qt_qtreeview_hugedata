/*
    Copyright (c) 2009-10 Qtrac Ltd. All rights reserved.

    This program or module is free software: you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version. It is provided
    for educational purposes and is distributed in the hope that it will
    be useful, but WITHOUT ANY WARRANTY; without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
    the GNU General Public License for more details.
*/

#include "treemodel.hpp"
#include <QFile>
#include <QMimeData>
#include <qdebug.h>


Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractItemModel::flags(index);
    if (index.isValid()) {
        theFlags |= Qt::ItemIsSelectable|Qt::ItemIsEnabled;
    }
    return theFlags;
}


QVariant TreeModel::data(const QModelIndex &index, int role) const
{
   if (Qt::SizeHintRole == role)
   {
      return QVariant();
   }
    if (!rootItem || !index.isValid() || index.column() < 0 )
        return QVariant();
    if (TaskItem *item = itemForIndex(index)) {
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            if (index.column() < item->columnTexts().length()) {
               return item->columnTexts().at(index.column());
            }
        }
        
    }
    return QVariant();
}


QVariant TreeModel::headerData(int section,
        Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
       if (section < m_lstHeaderData.length())
       {
          return m_lstHeaderData.at(section);
       }
    }
    return QVariant();
}


int TreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return 0;
    TaskItem *parentItem = itemForIndex(parent);
    return parentItem ? parentItem->childCount() : 0;
}


int TreeModel::columnCount(const QModelIndex &parent) const
{
   return m_lstHeaderData.size();
}


QModelIndex TreeModel::index(int row, int column,
                             const QModelIndex &parent) const
{
    if (!rootItem || row < 0 || column < 0
        || (parent.isValid() && parent.column() != 0))
        return QModelIndex();
    TaskItem *parentItem = itemForIndex(parent);
    Q_ASSERT(parentItem);
    if (TaskItem *item = parentItem->childAt(row))
        return createIndex(row, column, item);
    return QModelIndex();
}


TaskItem *TreeModel::itemForIndex(const QModelIndex &index) const
{
    if (index.isValid()) {
        if (TaskItem *item = static_cast<TaskItem*>(
                index.internalPointer()))
            return item;
    }
    return rootItem;
}


QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();
    if (TaskItem *childItem = itemForIndex(index)) {
        if (TaskItem *parentItem = childItem->parent()) {
            if (parentItem == rootItem)
                return QModelIndex();
            if (TaskItem *grandParentItem = parentItem->parent()) {
                int row = grandParentItem->rowOfChild(parentItem);
                return createIndex(row, 0, parentItem);
            }
        }
    }
    return QModelIndex();
}


bool TreeModel::setData(const QModelIndex &index,
                        const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    if (TaskItem *item = itemForIndex(index)) {
        if (role == Qt::EditRole)
            item->setName(value.toString());
        else
            return false;
        emit dataChanged(index, index);
        return true;
    }
    return false;
}


bool TreeModel::insertRows(int row, int count,
                           const QModelIndex &parent)
{
    if (!rootItem)
        rootItem = new TaskItem;
    TaskItem *parentItem = parent.isValid() ? itemForIndex(parent)
                                            : rootItem;
    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        TaskItem *item = new TaskItem(tr("New Task"), false);
        parentItem->insertChild(row, item);
    }
    endInsertRows();
    return true;
}

bool TreeModel::appendRow(const QModelIndex &parent, TaskItem* taskItem)
{
   if (!rootItem)
      rootItem = new TaskItem;
   TaskItem *parentItem = parent.isValid() ? itemForIndex(parent)
      : rootItem;
   
   beginInsertRows(parent, parentItem->childCount(), parentItem->childCount());
   parentItem->addChild(taskItem);
   endInsertRows();
   return true;
}

bool TreeModel::removeRows(int row, int count,
                           const QModelIndex &parent)
{
    if (!rootItem)
        return false;
    TaskItem *item = parent.isValid() ? itemForIndex(parent)
                                      : rootItem;
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
        delete item->takeChild(row);
    endRemoveRows();
    return true;
}


void TreeModel::clear()
{
    delete rootItem;
    rootItem = 0;
    reset();
}

QStringList TreeModel::pathForIndex(const QModelIndex &index) const
{
    QStringList path;
    QModelIndex thisIndex = index;
    while (thisIndex.isValid()) {
        path.prepend(data(thisIndex).toString());
        thisIndex = thisIndex.parent();
    }
    return path;
}


QModelIndex TreeModel::indexForPath(const QStringList &path) const
{
    return indexForPath(QModelIndex(), path);
}

bool TreeModel::hasChildren(const QModelIndex &parent) const
{
   TaskItem* item = itemForIndex(parent);
   if (item)
   {
      return item->childCount() > 0 ? true : false;
   }
   return false;
}


QModelIndex TreeModel::indexForPath(const QModelIndex &parent,
                                    const QStringList &path) const
{
    if (path.isEmpty())
        return QModelIndex();
    for (int row = 0; row < rowCount(parent); ++row) {
        QModelIndex thisIndex = index(row, 0, parent);
        if (data(thisIndex).toString() == path.at(0)) {
            if (path.count() == 1)
                return thisIndex;
            thisIndex = indexForPath(thisIndex, path.mid(1));
            if (thisIndex.isValid())
                return thisIndex;
        }
    }
    return QModelIndex();
}
