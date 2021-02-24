/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <QApplication>
#include <QCloseEvent>
#include <QMainWindow>
#include <QSettings>

namespace demo {

static const char *SETTINGS_GROUP = "MainWindow";
static const char *SETTINGS_GEOMETRY = "geometry";
static const char *SETTINGS_STATE = "state";

struct DemoMainWindowOptions {
  struct {
    QString appVersion;
    QString appName;
    QString orgName;
    QString orgDomain;
  } properties;
  bool disableMinMaxHints = false;
  QSize minSize = QSize(600, 370);
};

class DemoMainWindow : public QMainWindow {
  Q_OBJECT

protected:
  inline int run(QApplication const &app,
                 DemoMainWindowOptions const &opts) {
    initialize(opts);
    return app.exec();
  }
  inline void initialize(DemoMainWindowOptions const &opts) {
    QCoreApplication::setApplicationVersion(opts.properties.appVersion);
    QCoreApplication::setApplicationName(opts.properties.appName);

    QCoreApplication::setOrganizationName(opts.properties.orgName);
    QCoreApplication::setOrganizationDomain(opts.properties.orgDomain);

    setWindowTitle(opts.properties.appName);
    setMinimumSize(opts.minSize);

    if (opts.disableMinMaxHints)
      noMinMaxHints();

    restoreSettings();
    show();
  }
  virtual inline void restoreSettings() {
    QSettings conf;
    conf.beginGroup(SETTINGS_GROUP);
    restoreGeometry(conf.value(SETTINGS_GEOMETRY).toByteArray());
    restoreState(conf.value(SETTINGS_STATE).toByteArray());
    conf.endGroup();
  }
  virtual inline void saveSettings() const {
    QSettings conf;
    conf.beginGroup(SETTINGS_GROUP);
    conf.setValue(SETTINGS_GEOMETRY, saveGeometry());
    conf.setValue(SETTINGS_STATE, saveState());
    conf.endGroup();
  }

private:
  inline void noMinMaxHints() {
    Qt::WindowFlags flags = windowFlags();
    flags &= ~Qt::WindowMinimizeButtonHint;
    flags &= ~Qt::WindowMaximizeButtonHint;
    setWindowFlags(flags);
  }

protected:
  inline void closeEvent(QCloseEvent *e) override {
    saveSettings();
    e->accept();
  }
};

} // namespace demo

// vim: set ts=2 sw=2 tw=72 expandtab:
