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
#include "timeedit.h"
#include "toplevel.h"
#include "tomato.h"

#include <QDesktopWidget>


class TimeEditUI : public QWidget, public Ui::TimeEditWidget
{
    Q_OBJECT
public:
    TimeEditUI(QWidget *parent = 0)
            : QWidget(parent)
    {
        setupUi(this);
    }
};




TimeEditDialog::TimeEditDialog(TopLevel *toplevel)
        : KDialog(),
        m_toplevel(toplevel)
{
    setCaption(i18n("Anonymous Tomato"));

    setButtonWhatsThis(KDialog::Ok, i18n("Start a new anonymous tomato with the time configured in this dialog."));
    setButtonWhatsThis(KDialog::Cancel, i18n("Close this dialog without starting a new tomato."));

    ui = new TimeEditUI(this);
    setMainWidget(ui);

    KSharedConfigPtr config = KSharedConfig::openConfig();
    KConfigGroup group(config, "AnonymousTomatoDialog");

    int time = group.readEntry("AnonymousTomatoTime", 25);

    ui->minutes->setValue(time);

    ui->minutes->setFocus(Qt::ShortcutFocusReason);

    restoreDialogSize(group);

    QDesktopWidget desktop;
    int x = group.readEntry("AnonymousTomatoDialogXPos", desktop.screenGeometry().width() / 2 - width() / 2);
    int y = group.readEntry("AnonymousTomatoDialogYPos", desktop.screenGeometry().height() / 2 - height() / 2);

    x = qMin(qMax(0, x), desktop.screenGeometry().width() - width());
    x = qMin(qMax(0, y), desktop.screenGeometry().height() - height());
    move(QPoint(x, y));

    connect(ui->minutes, SIGNAL(valueChanged(int)), this, SLOT(checkOkButtonState()));
}


TimeEditDialog::~TimeEditDialog()
{
    delete ui;
}


void TimeEditDialog::checkOkButtonState()
{
    enableButtonOk(ui->minutes->value());
}


void TimeEditDialog::accept()
{
    hide();

    int time = ui->minutes->value();

    KSharedConfigPtr config = KSharedConfig::openConfig();
    KConfigGroup group(config, "AnonymousTomatoDialog");
    group.writeEntry("AnonymousTomatoTime", time);
    saveDialogSize(group);

    group.writeEntry("AnonymousTomatoDialogXPos", x());
    group.writeEntry("AnonymousTomatoDialogYPos", y());

    m_toplevel->runTomato(Tomato(i18n("Anonymous Tomato"), time));
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:

#include "timeedit.moc"
#include "moc_timeedit.cpp"

