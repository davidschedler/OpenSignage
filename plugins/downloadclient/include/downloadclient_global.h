/* Copyright (C) 2020  David Schedler (https://github.com/davidschedler/)
 *
 * This file is part of OpenSigange
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DOWNLOADCLIENT_GLOBAL_H
#define DOWNLOADCLIENT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DOWNLOADCLIENT_LIBRARY)
#  define DOWNLOADCLIENTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DOWNLOADCLIENTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DOWNLOADCLIENT_GLOBAL_H
