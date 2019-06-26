/*
 * Copyright (C) 2018-2019 Rinigus https://github.com/rinigus
 *
 * This file is part of QML Runner.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <QDir>
#include <QFileInfo>
#include <QApplication>
#include <QIcon>
#include <QStringList>
#include <QTranslator>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QtGlobal>

#include <iostream>

int main(int argc, char *argv[])
{
  // parse options that have to be applied before creation of QApplication
  for (int i = 1; i < argc; ++i)
    {
      const char *arg = argv[i];
      if (!qstrcmp(arg, "-scale"))
        QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
      else if (!qstrcmp(arg, "-noscale"))
        QGuiApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    }

  QApplication app(argc, argv);

  const QStringList arguments = QCoreApplication::arguments();
  QString appName;
  QString prefix = "/usr/share";
  QStringList paths;
  for (int i = 1, size = arguments.size(); i < size; ++i)
    if (arguments.at(i) == "--")
      break; // following options are options for the application
    else if (!arguments.at(i).startsWith(QLatin1Char('-')))
      {
        if (appName.isEmpty())
          appName = arguments.at(i);
        else
          {
            std::cerr << "Application name specified multiple times\n";
            return -2;
          }
      }
    else if (arguments.at(i) == "-P" && i+1 < size)
      {
        prefix = arguments.at(i+1);
        ++i;
      }
    else if (arguments.at(i) == "-path" && i+1 < size)
      {
        paths.push_back(arguments.at(i+1));
        ++i;
      }
    else if (arguments.at(i) == "-noscale" || arguments.at(i) == "-scale")
      {
        // have been processed above
      }
    else
      {
        std::cerr << "Unknown option: " << arguments.at(i).toStdString() << "\n";
        return -3;
      }

  if (appName.isEmpty())
    {
      std::cerr << "Usage: " << argv[0] << " [-P prefix] [-path path] [-scale] [-noscale] appname [--] [application_options]\n";
      std::cerr << "Loaded QML is determined by prefix and appname as follows:\n"
                << " prefix/appname/qml/appname.qml\n"
                << "by default, prefix is /usr/share\n"
                << "scale/noscale either applies AA_EnableHighDpiScaling or AA_DisableHighDpiScaling\n";
      return -1;
    }
  
  QDir dir(prefix);
  QString qmlpath = dir.absoluteFilePath(appName + "/qml/" + appName + ".qml");
  if (!QFileInfo::exists(qmlpath))
    {
      std::cerr << "Cannot find " << qmlpath.toStdString() << "\n";
      return -2;
    }

  app.setApplicationName(appName);
  app.setOrganizationName(appName);
  app.setApplicationVersion("1.0");
  app.setDesktopFileName(appName + ".desktop");

  // add translations
  QString transpath = dir.absoluteFilePath(appName + "/translations");
  QTranslator translator;
  std::cout << "Current locale: " << QLocale().name().toStdString() << "\n";
  if (translator.load(QLocale(), appName, QLatin1String("-"), transpath))
    {
      std::cout << "Loaded translation\n";
      app.installTranslator(&translator);
    }
  else
    std::cout << "Translation not found\n";

#if (QT_VERSION >= QT_VERSION_CHECK(5, 11, 0))
  // add fallback icon path
  QString icons_extra_path = dir.absoluteFilePath(appName + "/icons");
  if (QFileInfo::exists(icons_extra_path)) {
      QIcon::setFallbackSearchPaths(QIcon::fallbackSearchPaths() << icons_extra_path);
    }
#endif
  
  QQmlApplicationEngine engine;
  for (const auto &p: paths)
    engine.addImportPath(p);

  engine.load(qmlpath);
  if (engine.rootObjects().isEmpty())
    {
      std::cerr << "Error loading " << qmlpath.toStdString() << "\n";
      return -3;
    }

  return app.exec();
}
