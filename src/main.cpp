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

#include "toplevel.h"

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>

int main(int argc, char* argv[])
{
    KAboutData aboutData
    (
        "kontaminuti",
        0,
        ki18n("Kontaminuti"),
        "0.0.0",
        ki18n("KDE utility for the Pomodoro Technique."),
        KAboutData::License_GPL,
        ki18n("2011, Ruurd Pels")
    );

    aboutData.addAuthor
    (
        ki18n("Ruurd Pels"),
        ki18n("Current maintainer"),
        "ruurd@tiscali.nl",
        "http://home.tiscali.nl/ruurd",
        "ruurd"
    );

    KCmdLineArgs::init(argc, argv, &aboutData);
    KCmdLineOptions options;
    options.add("t");
    options.add("time <minutes>", ki18n("Start a new task with this time."));
    options.add("n");
    options.add("name <name>", ki18n("Use this name for the task started."));
    KCmdLineArgs::addCmdLineOptions(options);

    KApplication app;
    QApplication::setQuitOnLastWindowClosed(false);
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    TopLevel *toplevel = new TopLevel(&aboutData);

    if (args->isSet("time"))
    {
        const int time = args->getOption("time").toInt();
        if (time > 0)
        {
            const Tomato tomato(args->isSet("name") ? args->getOption("name") : i18n("Anonymous Tomato"), time);
            toplevel->runTomato(tomato);
        }
    }
    args->clear();
    return app.exec();
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
