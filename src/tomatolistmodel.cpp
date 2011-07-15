/*
 * Copyright 2011 by Ruurd Pels <ruurd@tiscali.nl>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tomatolistmodel.h"
#include "tomato.h"

#include <klocalizedstring.h>



TomatoListModel::TomatoListModel(const QList<Tomato> &tomatolist, QObject *parent)
  : QAbstractTableModel(parent), m_tomatolist(tomatolist)
{
}


QModelIndex TomatoListModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED( parent );

    return createIndex(row, column);
}


int TomatoListModel::rowCount(const QModelIndex &parent) const
{
    if( parent.isValid() ) {
        return 0;
    }

    return m_tomatolist.size();
}


int TomatoListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED( parent );

    return 2;
}


QVariant TomatoListModel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() ) {
        return QVariant();
    }

    if( role == Qt::DisplayRole ) {
        if( index.column() == 0 ) {
            return m_tomatolist.at( index.row() ).name();
        }

        return m_tomatolist.at( index.row() ).timeToString();
    }

    if( role == Qt::EditRole ) {
        if( index.column() == 0 ) {
            return m_tomatolist.at( index.row() ).name();
        }
        return m_tomatolist.at( index.row() ).time();
    }

    if( role == Qt::ToolTipRole ) {
        QString s( m_tomatolist.at( index.row() ).name() );
        s.append( i18n( " (" ) );
        s.append( m_tomatolist.at( index.row() ).timeToString( true ) );
        s.append( i18n( ")" ) );

        return s;
    }

    return QVariant();
}



bool TomatoListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( index.isValid() && ( role == Qt::EditRole || role == Qt::DisplayRole ) ) {
        if( index.column() == 0 ) {
            m_tomatolist[ index.row() ].setName( value.toString() );
        } else if( index.column() == 1 ) {
            m_tomatolist[ index.row() ].setTime( value.toUInt() );
        }
        dataChanged( index, index );
        return true;
    }

    return false;
}


QVariant TomatoListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( orientation == Qt::Horizontal ) {
        if( role == Qt::DisplayRole )
            return section == 0 ? i18n("Name") : i18n("Time");
    }

    return QAbstractTableModel::headerData( section, orientation, role );
}


bool TomatoListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED( parent );
    beginInsertRows( QModelIndex(), row, row+count-1 );

    for(int i = 0; i < count; ++i) {
        m_tomatolist.insert( row, Tomato( i18n( "Unnamed Tomato" ), 180 ) );
    }

    endInsertRows();
    return true;
}


bool TomatoListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if( row-count > m_tomatolist.size() ) {
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row+count-1);
    for(int i=0; i < count; ++i) {
        m_tomatolist.removeAt( row );
    }

    endRemoveRows();
    return true;
}


const QList<Tomato> TomatoListModel::getTomatoList() const
{
    return m_tomatolist;
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:

