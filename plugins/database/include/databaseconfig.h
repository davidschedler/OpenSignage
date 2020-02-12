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

#ifndef DATABASECONFIG_H
#define DATABASECONFIG_H

#include <QObject>

class DatabaseConfig : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseConfig(QObject *parent = nullptr);

    QString dbPath() const;
    void setDbPath(const QString &dbPath);

    QString connectionName() const;
    void setConnectionName(const QString &connectionName);

private:
    QString m_dbPath;
    QString m_connectionName;
};

#endif // DATABASECONFIG_H
