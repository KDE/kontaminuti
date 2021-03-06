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
#ifndef TOPLEVEL_H
#define TOPLEVEL_H

#include "tomato.h"

#include <QIcon>
#include <QPixmap>
#include <KStatusNotifierItem>


class QAction;
class QActionGroup;
class KHelpMenu;
class KPassivePopup;
class KAboutData;



/**
 * @short the main class for Kontaminuti
 *
 * @author Ruurd Pels <ruurd@tiscali.nl>
 */
class TopLevel : public KStatusNotifierItem
{
    Q_OBJECT

public:
    explicit TopLevel(const KAboutData *aboutData, const QString &icon = QLatin1String("kontaminuti"), QWidget *parent = 0);
    ~TopLevel();
    void setTomatoList(const QList<Tomato> &tomatolist);
    void runTomato(const Tomato &tomato);

private slots:
    void runTomato(QAction *a);
    void showSettingsDialog();
    void showTimeEditDialog();
    void tomatoTimeEvent();
    void cancelTomato();
    void showPopup(bool active, const QPoint& point);

private:
    void checkState();
    void loadConfig();
    void loadTomatoMenuItems();
    void repaintTrayIcon();
    void setTooltipText(const QString& content);

private:
    QString m_iconname;
    QList<Tomato> m_tomatolist;
    QAction *m_stopAct, *m_confAct, *m_anonAct, *m_exitAct;
    QActionGroup *m_tomatoActionGroup;

    KHelpMenu *m_helpMenu;
    QTimer *m_timer;
    KPassivePopup *m_popup;

    int m_runningTomatoTime;
    int m_nextNotificationTime;
    Tomato m_runningTomato;

    /** should we show a popup for events */
    bool m_usepopup;

    /** auto hide the popup? */
    bool m_autohide;

    /** time after the popup should be hide. */
    int m_autohidetime;

    /** remind us about a ready tomato? */
    bool m_usereminder;

    /** the time bedween remind events */
    int m_remindertime;

    /** use a visual effect in the system tray icon. */
    bool m_usevisualize;
};


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
