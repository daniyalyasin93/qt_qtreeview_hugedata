#ifndef TREEMODEL_HPP
#define TREEMODEL_HPP
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

#include "taskitem.hpp"
#include <QAbstractItemModel>
#include <QIcon>

#include <QtCore/QXmlStreamReader>
#include <QtCore/QXmlStreamWriter>


class QMimeData;


class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(QObject *parent=0)
        : QAbstractItemModel(parent), rootItem(0) {
       m_lstHeaderData << "C ID" << "M ID" << "V" << "U" << "Source";
    }
    ~TreeModel() { delete rootItem;  }

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index,
                  int role=Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role=Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent=QModelIndex()) const override;
    int columnCount(const QModelIndex &parent=QModelIndex()) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent=QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    bool setHeaderData(int, Qt::Orientation, const QVariant&,
                       int=Qt::EditRole) override { return false; }
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role=Qt::EditRole) override;
    bool insertRows(int row, int count,
                    const QModelIndex &parent=QModelIndex()) override;
    bool removeRows(int row, int count,
                    const QModelIndex &parent=QModelIndex()) override;
    bool appendRow(const QModelIndex &parent, TaskItem* taskItem);
   
    void clear();
    QStringList pathForIndex(const QModelIndex &index) const;
    QModelIndex indexForPath(const QStringList &path) const;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

signals:

private:
    TaskItem *itemForIndex(const QModelIndex &index) const;
    QModelIndex indexForPath(const QModelIndex &parent,
                             const QStringList &path) const;
   
    TaskItem *rootItem;
    QStringList m_lstHeaderData;
};
#endif // TREEMODEL_HPP
