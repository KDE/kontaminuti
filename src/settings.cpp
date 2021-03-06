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

#include "settings.h"
#include "toplevel.h"
#include "tomatolistmodel.h"

#include <QHashIterator>
#include <QString>
#include <QDesktopWidget>

#include <knotifyconfigwidget.h>

class SettingsUI: public QWidget, public Ui::SettingsWidget
{
    Q_OBJECT

public:
    SettingsUI(QWidget *parent = 0)
            : QWidget(parent)
    {
        setupUi(this);
    }
};

SettingsDialog::SettingsDialog(TopLevel *toplevel, const QList<Tomato> &tomatos)
        : KDialog(),
        m_toplevel(toplevel)
{
    setCaption(i18n("Configure Kontaminuti"));

    setButtons(Ok | Cancel | Help | Reset);
    setButtonText(KDialog::Reset, i18n("Configure &amp;Notifications..."));
    setButtonToolTip(KDialog::Reset,  i18n("Configure notifications"));
    setButtonIcon(KDialog::Reset, KIcon(QLatin1String("preferences-desktop-notification")));

    setHelp(QLatin1String("configure"));

    setButtonWhatsThis(KDialog::Ok,     i18n("Save changes and close dialog."));
    setButtonWhatsThis(KDialog::Cancel, i18n("Close dialog without saving changes."));
    setButtonWhatsThis(KDialog::Help,   i18n("Show help page for this dialog."));
    setButtonWhatsThis(KDialog::Reset,  i18n("Configure notifications"));

    ui = new SettingsUI(this);
    setMainWidget(ui);

    KSharedConfigPtr config = KSharedConfig::openConfig();
    KConfigGroup group(config, "General");

    restoreDialogSize(group);

    QDesktopWidget desktop;
    int x = group.readEntry("SettingsDialogXPos", desktop.screenGeometry().width() / 2 - width() / 2);
    int y = group.readEntry("SettingsDialogYPos", desktop.screenGeometry().height() / 2 - height() / 2);

    x = qMin(qMax(0, x), desktop.screenGeometry().width() - width());
    x = qMin(qMax(0, y), desktop.screenGeometry().height() - height());
    move(QPoint(x, y));

    bool popup = group.readEntry("UsePopup", true);
    bool autohide = group.readEntry("PopupAutoHide", false);
    int autohidetime = group.readEntry("PopupAutoHideTime", 30);
    bool reminder = group.readEntry("UseReminder", false);
    int remindertime = group.readEntry("ReminderTime", 60);
    bool vis = group.readEntry("UseVisualize", true);

    ui->popupCheckBox->setChecked(popup);
    ui->autohideCheckBox->setChecked(autohide);
    ui->reminderCheckBox->setChecked(reminder);
    ui->visualizeCheckBox->setChecked(vis);

    ui->autohideSpinBox->setValue(autohidetime);
    ui->reminderSpinBox->setValue(remindertime);
    ui->autohideSpinBox->setSuffix(ki18ncp("Auto hide popup after", " second", " seconds"));
    ui->reminderSpinBox->setSuffix(ki18ncp("Reminder every", " second", " seconds"));

    ui->autohideCheckBox->setEnabled(popup);
    ui->autohideSpinBox->setEnabled(autohide);
    ui->reminderSpinBox->setEnabled(reminder);

    m_model = new TomatoListModel(tomatos, this);
    ui->tomatolistTreeView->setModel(m_model);

    connect
    (
        ui->tomatolistTreeView->selectionModel(),
        SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
        this,
        SLOT(updateSelection(QItemSelection, QItemSelection))
    );

    ui->removeButton->setEnabled(false);
    ui->upButton->setEnabled(false);
    ui->downButton->setEnabled(false);

    ui->newButton->setIcon(KIcon(QLatin1String("list-add")));
    ui->removeButton->setIcon(KIcon(QLatin1String("edit-delete")));
    ui->upButton->setIcon(KIcon(QLatin1String("arrow-up")));
    ui->downButton->setIcon(KIcon(QLatin1String("arrow-down")));

    connect(ui->popupCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkPopupButtonState(bool)));
    connect(this, SIGNAL(resetClicked()), this, SLOT(confButtonClicked()));
    connect(ui->newButton, SIGNAL(clicked()), this, SLOT(newButtonClicked()));
    connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(removeButtonClicked()));
    connect(ui->upButton, SIGNAL(clicked()), this, SLOT(upButtonClicked()));
    connect(ui->downButton, SIGNAL(clicked()), this, SLOT(downButtonClicked()));
    connect(ui->tomatoNameEdit, SIGNAL(textChanged(QString)), this, SLOT(nameValueChanged(QString)));
    connect(ui->minutesSpin, SIGNAL(valueChanged(int)), this, SLOT(timeValueChanged()));
}


SettingsDialog::~SettingsDialog()
{
    delete m_model;
    delete ui;
}


void SettingsDialog::accept()
{
    KSharedConfigPtr config = KSharedConfig::openConfig();
    KConfigGroup group(config, "General");
    group.writeEntry("SettingsDialogXPos", x());
    group.writeEntry("SettingsDialogYPos", y());

    hide();
    saveDialogSize(group);

    group.writeEntry("UsePopup",          ui->popupCheckBox->checkState() == Qt::Checked);
    group.writeEntry("PopupAutoHide",     ui->autohideCheckBox->checkState() == Qt::Checked);
    group.writeEntry("PopupAutoHideTime", ui->autohideSpinBox->value());
    group.writeEntry("UseReminder",       ui->reminderCheckBox->checkState() == Qt::Checked);
    group.writeEntry("ReminderTime",      ui->reminderSpinBox->value());
    group.writeEntry("UseVisualize",      ui->visualizeCheckBox->checkState() == Qt::Checked);

    config->sync();
    m_toplevel->setTomatoList(m_model->getTomatoList());
}


void SettingsDialog::checkPopupButtonState(bool b)
{
    ui->autohideCheckBox->setEnabled(b);

    if (b == false)
    {
        ui->autohideSpinBox->setEnabled(b);
    }
    else if (ui->autohideCheckBox->checkState() == 2)
    {
        ui->autohideSpinBox->setEnabled(b);
    }
}


void SettingsDialog::confButtonClicked()
{
    KNotifyConfigWidget::configure(this);
}


void SettingsDialog::newButtonClicked()
{
    int count = m_model->rowCount();
    m_model->insertRows(count, 1);

    QItemSelectionModel *sm = ui->tomatolistTreeView->selectionModel();
    QItemSelection selection(m_model->index(count, 0), m_model->index(count, 1));
    sm->select(selection, QItemSelectionModel::Clear | QItemSelectionModel::Select);
}


void SettingsDialog::removeButtonClicked()
{
    QModelIndexList indexes = ui->tomatolistTreeView->selectionModel()->selectedIndexes();

    foreach(const QModelIndex &index, indexes)
    {
        // Only delete a row when column==0, otherwise the row will be delete
        // multiple times (the loop iterate over every cell, not over rows).
        if (index.column() == 0)
        {
            m_model->removeRows(index.row(), 1);
        }
    }
}


void SettingsDialog::upButtonClicked()
{
    moveSelectedItem(true);
}


void SettingsDialog::downButtonClicked()
{
    moveSelectedItem(false);
}


void SettingsDialog::moveSelectedItem(bool moveup)
{
    QItemSelectionModel *sm = ui->tomatolistTreeView->selectionModel();
    QModelIndexList items = sm->selection().indexes();

    if (items.isEmpty() == false)
    {
        QString name = m_model->data(m_model->index(items.at(0).row(), 0), Qt::EditRole).toString();
        unsigned time = m_model->data(m_model->index(items.at(0).row(), 1), Qt::EditRole).toUInt();
        int pos = items.at(0).row();

        moveup ? --pos : ++pos;

        removeButtonClicked();

        m_model->insertRows(pos, 1);
        m_model->setData(m_model->index(pos, 0), name, Qt::EditRole);
        m_model->setData(m_model->index(pos, 1), time, Qt::EditRole);

        QItemSelection selection(m_model->index(pos, 0), m_model->index(pos, 1));
        sm->select(selection, QItemSelectionModel::Clear | QItemSelectionModel::Select);
    }
}


void SettingsDialog::updateSelection(const QItemSelection& selected, const QItemSelection& deselected)
{
    Q_UNUSED(deselected);
    QModelIndexList items = selected.indexes();

    QString name;
    unsigned time = 0;

    bool state = (items.isEmpty() == false);

    ui->tomatoPropertiesGroup->setEnabled(state);
    ui->tomatoNameEdit->setEnabled(state);
    ui->minutesSpin->setEnabled(state);
    ui->removeButton->setEnabled(state);

    if (state)
    {
        name = m_model->data(m_model->index(items.at(0).row(), 0), Qt::EditRole).toString();
        time = m_model->data(m_model->index(items.at(0).row(), 1), Qt::EditRole).toUInt();

        ui->upButton->setEnabled(items.at(0).row() > 0);
        ui->downButton->setEnabled(items.at(0).row() < (m_model->rowCount() - 1));
    }
    else
    {
        ui->upButton->setEnabled(false);
        ui->downButton->setEnabled(false);
    }

    ui->tomatoNameEdit->setText(name);
    ui->minutesSpin->setValue(time);
}


void SettingsDialog::timeValueChanged()
{
    QModelIndexList items = ui->tomatolistTreeView->selectionModel()->selection().indexes();

    if (items.isEmpty() == false)
    {
        int time = ui->minutesSpin->value();

        if (time <= 0)
        {
            time = 1;
        }
        m_model->setData(m_model->index(items.at(0).row(), 1), time, Qt::EditRole);
    }
}

void SettingsDialog::nameValueChanged(const QString &text)
{
    QModelIndexList items = ui->tomatolistTreeView->selectionModel()->selection().indexes();

    if (items.isEmpty() == false)
    {
        m_model->setData(m_model->index(items.at(0).row(), 0), text, Qt::EditRole);
    }
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:

#include "settings.moc"
#include "moc_settings.cpp"
