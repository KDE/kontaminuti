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
#include "tomato.h"
#include <klocalizedstring.h>

Tomato::Tomato(const QString &task, const unsigned time)
  : m_time( 0 )
{
    setTask( task );
    setTime( time );
}


QString Tomato::task() const
{
    return m_task;
}


void Tomato::setTask(const QString &task)
{
    if( task.isEmpty() ) {
        m_task = i18n( "Undescribed task" );
    }
    else {
        m_task = task;
    }
}


unsigned Tomato::time() const
{
    return m_time;
}


void Tomato::setTime(const unsigned time)
{
    m_time = time;
}


QString Tomato::timeToString(const bool longdesc) const
{
    return int2time( m_time, longdesc );
}


QString Tomato::int2time(const int time, const bool longdesc)
{
    QString str;
    const unsigned hour = 60;
    const unsigned day = 24 * hour;
    const unsigned year = 365 * day;

    if( time / year ) {
        if( longdesc ) {
            str.append( i18np( "%1 year", "%1 years", time/year ) );
        }
        else {
            str.append( i18np( "%1 y", "%1 y", time/year ) );
        }
    }

    if( (time % year) / day ) {
        if( !str.isEmpty() ) {
            str.append( QLatin1Char( ' ' ) );
        }

        if( longdesc ) {
            str.append( i18np( "%1 day", "%1 days", (time % year) / day ) );
        }
        else {
            str.append( i18np( "%1 d", "%1 d", (time % year) / day ) );
        }
    }

    if( (time % day) / hour ) {
        if( !str.isEmpty() ) {
            str.append( QLatin1Char( ' ' ) );
        }

        if( longdesc ) {
            str.append( i18np( "%1 hour", "%1 hours", ( (time%day ) / hour ) ) );
        }
        else {
            str.append( i18np( "%1 h", "%1 h", ( (time % day) / hour) ) );
        }
    }

    if( time % hour ) {
        if( !str.isEmpty() ) {
            str.append( QLatin1Char( ' ' ) );
        }

        if( longdesc ) {
            str.append( i18np( "%1 minute", "%1 minutes", ( time % hour ) ) );
        }
        else {
            str.append( i18np( "%1 m", "%1 m", ( time % hour ) ) );
        }
    }

    return str;
}

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:


