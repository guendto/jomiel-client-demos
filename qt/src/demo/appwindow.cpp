/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2020 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <QShortcut>

#include "DemoAppWindow"

namespace demo {

DemoAppWindow::DemoAppWindow() : DemoMainWindow() {
  setupUi();
}

void DemoAppWindow::setupUi() {
  // UI.
  //
  mainWidget = new MainWidget;
  setCentralWidget(mainWidget);

  connect(mainWidget->addressbar, &QLineEdit::returnPressed, this,
          &DemoAppWindow::sendInquiry);

  // ZMQ.
  //
  zmq.endpoint = mainWidget->endpoint->text();

  zmq.ctx = nzmqt::createDefaultContext(this);
  zmq.ctx->start();

  zmq.sck = zmq.ctx->createSocket(nzmqt::ZMQSocket::TYP_REQ, this);

  connect(zmq.sck, &nzmqt::ZMQSocket::messageReceived, this,
          &DemoAppWindow::receiveResponse);

  // Shortcuts.
  //
  new QShortcut(QKeySequence(tr("Ctrl+Q", "Quit")), this, SLOT(close()));
}

void DemoAppWindow::sendInquiry() {
  mainWidget->addressbar->setDisabled(true);
  mainWidget->endpoint->setDisabled(true);
  mainWidget->details->reset();

  auto const uri = mainWidget->addressbar->text().toStdString();

  jomiel::Inquiry inquiry;
  inquiry.mutable_media()->set_input_uri(uri);

  std::string serialized;
  inquiry.SerializeToString(&serialized);

  QList<QByteArray> request;
  request += QString::fromStdString(serialized).toLocal8Bit();

  zmq.sck->connectTo(zmq.endpoint);
  zmq.sck->sendMessage(request);
}

void DemoAppWindow::receiveResponse(
  QList<QByteArray> const& data) const {
  zmq.sck->disconnectFrom(zmq.endpoint);
  mainWidget->parseResponse(data);
}

}  // namespace demo

// vim: set ts=2 sw=2 tw=72 expandtab:
