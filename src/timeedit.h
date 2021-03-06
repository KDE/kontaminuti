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
#ifndef TIMEEDIT_H
#define TIMEEDIT_H

#include "ui_timeedit.h"

class TopLevel;
class TimeEditUI;


/**
 * @short the timeedit dialog.
 *
 * @author Ruurd Pels <ruurd@tiscali.nl>
 */
class TimeEditDialog : public KDialog
{
    Q_OBJECT

public:
    TimeEditDialog(TopLevel *toplevel);
    ~TimeEditDialog();

private slots:
    void checkOkButtonState();
    void accept();

private:
    TimeEditUI *ui;
    TopLevel *m_toplevel;
};

#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
