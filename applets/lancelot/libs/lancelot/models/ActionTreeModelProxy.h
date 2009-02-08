/*
 *   Copyright (C) 2007 Ivan Cukic <ivan.cukic+kde@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser/Library General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser/Library General Public License for more details
 *
 *   You should have received a copy of the GNU Lesser/Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef LANCELOT_ACTION_TREE_MODEL_PROXY_H
#define LANCELOT_ACTION_TREE_MODEL_PROXY_H

#include <lancelot/lancelot_export.h>
#include <lancelot/models/ActionTreeModel.h>
#include <lancelot/models/ActionListModel.h>

namespace Lancelot
{

class LANCELOT_EXPORT ActionTreeModelProxy: public ActionTreeModel {
    Q_OBJECT
public:
    explicit ActionTreeModelProxy(ActionListModel * model,
            QString title = QString(), QIcon icon = QIcon());

    ActionListModel * model() const;

    // ActionTreeModel
    L_Override virtual ActionTreeModel * child(int index);
    L_Override virtual QString modelTitle() const;
    L_Override virtual QIcon modelIcon()  const;

    // ActionListModel
    L_Override virtual QString title(int index) const;
    L_Override virtual QString description(int index) const;
    L_Override virtual QIcon icon(int index) const;
    L_Override virtual bool isCategory(int index) const;
    L_Override virtual bool hasContextActions(int index) const;
    L_Override virtual void setContextActions(int index, Lancelot::PopupMenu * menu);
    L_Override virtual void contextActivate(int index, QAction * context);
    L_Override virtual QMimeData * mimeData(int index) const;
    L_Override virtual void setDropActions(int index,
            Qt::DropActions & actions, Qt::DropAction & defaultAction);

    L_Override virtual int size() const;

Q_SIGNALS:
    void itemActivated(int index);

    void updated();
    void itemInserted(int index);
    void itemDeleted(int index);
    void itemAltered(int index);

protected:
    /** Models should reimplement this function. It is invoked when
     *  an item is activated, before the itemActivated signal is emitted */
    void activate(int index);

private:
    ActionListModel * m_model;
    QString m_modelTitle;
    QIcon m_modelIcon;
};

} // namespace Lancelot

#endif /* LANCELOT_ACTION_TREE_MODEL_PROXY_H */

