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
#ifndef TEA_H
#define TEA_H

#include <QString>


/**
 * @short this class represent a tomato.
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */
class Tomato {
    public:
         /**
          * Constructs an tomato.
          * @param name the name of this tomato.
          * @param time the tomato time in seconds.
          */
        explicit Tomato(const QString &name = QString(), const unsigned time = 180);

        /**
         * Returns the name of this tomato.
         *
         * @return the name of this tomato.
         */
        QString name() const;

        /**
         * Set the name of this the.
         * @param name the new name for this tomato.
         */
        void setName(const QString &name);

        /**
         * Returns the time for this tomato in seconds.
         *
         * @return the time for this tomato in seconds.
         */
        unsigned time() const;

        /**
         * Set the time of this tomato.
         * @param time the new time for this tomato in seconds.
         */
        void setTime(const unsigned time);

        /**
         * Returns the time for this tomato as a @ref QString.
         * @param longdesc if true return long names like  "5 minutes 30 seconds", else returns a short form like "5 min 30 s".
         *
         * @return the time for this tomato as a @ref QString.
         */
        QString timeToString(const bool longdesc = false) const;


        /**
         * Returns a formatted @ref QString for the given time.
         * @param time the time in seconds.
         * @param longdesc if true return long names like  "5 minutes 30 seconds", else returns a short form like "5 min 30 s".
         *
         * @return the formatted @ref QString.
         */
        static QString int2time(const int time, const bool longdesc=false);

    private:
        QString m_name;
        unsigned m_time;
};

#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:

