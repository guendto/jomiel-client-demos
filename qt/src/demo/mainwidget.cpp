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

#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>

#include "DemoAppWindow"

namespace jp = jomiel::protobuf::v1beta1;

namespace demo {

MainWidget::MainWidget(QWidget *parent) : QWidget(parent) { setupUi(); }

void MainWidget::setupUi() {
  details = new Details(tr("Details"));
  addressbar = new QLineEdit;
  endpoint = new QLineEdit;

  QHBoxLayout *endpointBox = new QHBoxLayout;
  endpointBox->addWidget(new QLabel(tr("Endpoint:")));
  endpointBox->addWidget(endpoint);
  // endpointBox->addStretch(0);

  QVBoxLayout *box = new QVBoxLayout;
  box->addWidget(addressbar);
  box->addLayout(endpointBox);
  box->addWidget(details);
  // box->addStretch(0);
  setLayout(box);

  addressbar->setText("https://youtu.be/K81-SLUFo9c");
  endpoint->setText("tcp://localhost:5514");
}

using resp_t = jp::MediaResponse;
using stream_t = resp_t::Stream;

static inline void add_streams(QTreeWidget *tree,
                               resp_t const &mediaResponse) {
  // add_child_item() -- a nested (lambda) function.
  //
  auto const add_child_item = [](QTreeWidgetItem *root,
                                 QString const &itemName,
                                 QString const &value) {
    auto const child = new QTreeWidgetItem;
    child->setText(0, itemName);
    child->setText(1, value);
    root->addChild(child);
  };

  // add_root_item() -- a nested (lambda) function.
  //
  auto const add_root_item = [&](stream_t const &stream) {
    auto const root = new QTreeWidgetItem(tree);
    auto const &quality = stream.quality();

    root->setText(0, QString::fromStdString(quality.profile()));

    auto const w = quality.width();
    auto const h = quality.height();

    add_child_item(root, "Width", QString("%1").arg(w));
    add_child_item(root, "Height", QString("%1").arg(h));
  };

  const bool sortingEnabled = tree->isSortingEnabled();
  tree->setSortingEnabled(false);

  for (auto const &stream : mediaResponse.stream())
    add_root_item(stream);

  tree->topLevelItem(0)->setExpanded(true);
  tree->resizeColumnToContents(0);
  tree->setSortingEnabled(sortingEnabled);
}

void MainWidget::parseResponse(QList<QByteArray> const &data) {
  // Convert serialized data into jomiel::Response.
  //
  auto const &size = data[0].length();
  auto const ptr = data[0].constData();

  const std::string serialized(ptr, size);

  jp::Response response;
  response.ParseFromString(serialized);

  // Parse the returned jomiel::Response.
  //  - If status is not OK, display an error
  //  - Otherwise update the details
  //
  auto const &responseStatus = response.status();
  auto const &mediaResponse = response.media();

  // set_title() -- a nested (lambda) function.
  //
  auto const set_title = [&](resp_t const &mediaResponse) {
    auto const text = QString::fromStdString(mediaResponse.title());
    details->title->setText(text);
  };

  // show_error() -- a nested (lambda) function.
  //
  auto const show_error = [&](QWidget *parent) {
    auto const message = responseStatus.message();
    auto const text = QString::fromStdString(message);
    QMessageBox::critical(parent, qApp->applicationName(), text);
  };

  if (responseStatus.code() == jp::STATUS_CODE_OK) {
    set_title(mediaResponse);
    add_streams(details->tree, mediaResponse);
  } else {
    show_error(this);
  }
  addressbar->setDisabled(false);
  endpoint->setDisabled(false);
}

} // namespace demo

// vim: set ts=2 sw=2 tw=72 expandtab:
