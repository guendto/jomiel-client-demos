/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-client-demos
 *
 * Copyright
 *  2020-2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <QGroupBox>
#include <QLineEdit>
#include <QTreeWidget>
#include <QVBoxLayout>

#include "DemoMainWindow"
#include "jomiel/protobuf/v1beta1/message.pb.h"
#include "nzmqt/nzmqt.hpp"

namespace demo {

class DemoAppWindow;

// Holds the video detail UI components in a groupbox.
class Details : public QGroupBox {
  Q_OBJECT

public:
  inline explicit Details(const QString &text = "",
                          QWidget *parent = Q_NULLPTR)
      : QGroupBox(text, parent) {
    title = new QLineEdit;
    tree = new QTreeWidget;

    QVBoxLayout *box = new QVBoxLayout;
    box->addWidget(title);
    box->addWidget(tree);
    box->addStretch(0);
    setLayout(box);

    title->setReadOnly(true);
    tree->setHeaderLabels(QStringList() << "Stream"
                                        << "");
  }
  inline void reset() {
    title->setText("");
    tree->clear();
  }

public:
  QTreeWidget *tree;
  QLineEdit *title;
};

// Holds the main window components.
// - This is the "central widget" for the main window.
//
class MainWidget : public QWidget {
  Q_OBJECT
public:
  explicit MainWidget(QWidget *parent = Q_NULLPTR);
  void parseResponse(QList<QByteArray> const &);

private:
  void setupUi();

private:
  QLineEdit *addressbar;
  QLineEdit *endpoint;
  Details *details;

  friend class DemoAppWindow;
};

// The main window for the application.
class DemoAppWindow : public DemoMainWindow {
  Q_OBJECT

public:
  DemoAppWindow();

  static inline int main(int argc, char **argv) {
    // Options for the main window.
    //
    DemoMainWindowOptions opts;

    opts.properties.orgDomain = "jomiel.github.io";
    opts.properties.orgName = "jomiel-client-demos";
    opts.properties.appVersion = "0.1.0";
    opts.properties.appName = "demo";

    // Instantiate the application.
    //
    QApplication const app(argc, argv);
    return DemoAppWindow().run(app, opts);
  }

private:
  void setupUi();

protected slots:
  void receiveResponse(QList<QByteArray> const &msg) const;
  void sendInquiry();

private:
  struct {
    nzmqt::ZMQContext *ctx;
    nzmqt::ZMQSocket *sck;
    QString endpoint;
  } zmq;
  MainWidget *mainWidget;
};

} // namespace demo

// vim: set ts=2 sw=2 tw=72 expandtab:
