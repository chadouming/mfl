/*
 * Copyright 2016-2017 MFL Project
 *
 * This is a placeholder until i choose a license
 * that fits my need.
 *
 * Chad Cormier Roussel <chadcormierroussel@gmail.com>
 * Christophe-Andre Gassman <Christo-Chibougamo@hotmail.com>
 *
 */

#include <QUrl>
#include <QString>
#include <QDir>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#ifndef HELPER_H
#define HELPER_H

bool copy_dir_recursive(QString from_dir, QString to_dir, bool replace_on_conflit);

#endif // HELPER_H
