/*
 * AccessControlRuleListModel.cpp - data model for access control rules
 *
 * Copyright (c) 2016 Tobias Doerffel <tobydox/at/users/dot/sf/dot/net>
 *
 * This file is part of iTALC - http://italc.sourceforge.net
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#include <QIcon>

#include "AccessControlRuleListModel.h"
#include "ItalcConfiguration.h"
#include "ItalcCore.h"


AccessControlRuleListModel::AccessControlRuleListModel(QObject *parent)
    : QAbstractListModel(parent)
{
	reload();
}



void AccessControlRuleListModel::reload()
{
	beginResetModel();

	m_accessControlRules.clear();

	for( AccessControlRule accessControlRule : ItalcCore::config->accessControlRules() )
	{
		m_accessControlRules.append( accessControlRule );
	}

	endResetModel();
}



int AccessControlRuleListModel::rowCount(const QModelIndex &parent) const
{
	// For list models only the root node (an invalid parent) should return the list's size. For all
	// other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
	if (parent.isValid())
		return 0;

	return m_accessControlRules.count();
}



QVariant AccessControlRuleListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || index.row() > rowCount() )
	{
		return QVariant();
	}

	const AccessControlRule& rule = m_accessControlRules[index.row()];

	if( role == Qt::DecorationRole )
	{
		switch( rule.action() )
		{
		case AccessControlRule::ActionAllow: return QIcon( ":/Resources/vcs-normal.png" );
		case AccessControlRule::ActionDeny: return QIcon( ":/Resources/vcs-conflicting.png" );
		default: return QIcon( ":/Resources/vcs-removed.png" );
		}
	}
	else if( role == Qt::DisplayRole )
	{
		return rule.name();
	}
	else if( role == Qt::ToolTipRole )
	{
		return rule.description();
	}

	return QVariant();
}
