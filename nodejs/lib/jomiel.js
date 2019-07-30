/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2019 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

'use strict';

const proto = require('protobufjs');
const zmq = require('zeromq');

class Jomiel {
  constructor(options) {
    this.sock = zmq.socket('req');
    //this.sock.setsockopt(zmq.ZMQ_LINGER, 0)
    this.options = options;
  }

  connect() {
    const re = this.options.routerEndpoint;
    this.printStatus(`<connect> ${re}`);
    this.sock.connect(re);
  }

  inquire(uri) {
    proto.load('../proto/Message.proto')
      .then(root => {
        const Inquiry = root.lookup('jomiel.Inquiry');
        const inquiry = Inquiry.create({
          media: {
            inputUri: uri,
          },
        });
        if (!this.options.beTerse)
          this.printMessage(`<send>`, inquiry);
        const serializedInquiry = Inquiry.encode(inquiry).finish();
        this.sock.send(serializedInquiry);
        this.recv(root);
      })
      .catch(what => {
        this.handleError(what);
      });
  }

  recv(messageRoot) {
    this.sock.on('message', data => {
      this.handleResponse(messageRoot, data);
      this.sock.close();
    });
  }

  handleResponse(messageRoot, data) {
    proto.load('../proto/Status.proto')
      .then(root => {
        this.dumpResponse({
          message: messageRoot,
          status: root,
        }, data);
      })
      .catch(what => {
        this.handleError(what);
      });
  }

  printStatus(message) {
    if (!this.options.beTerse) {
      console.error('status: ' + message);
    }
  }

  dumpTerseResponse(mediaResponse) {
    console.log(`---\ntitle: ` + mediaResponse.title);
    mediaResponse.stream.forEach(stream => {
      const quality = stream.quality;
      console.log(`  profile: ${quality.profile}`);
      console.log(`    width: ${quality.width}`);
      console.log(`    height: ${quality.height}`);
    });
  }

  dumpResponse(root, data) {
    const StatusCode = root.status.lookup('jomiel.status.StatusCode');
    const Response = root.message.lookup('jomiel.Response');
    const response = Response.decode(data);
    if (response.status.code == StatusCode.values.OK) {
      if (this.options.beTerse) {
        this.dumpTerseResponse(response.media);
      } else {
        this.printMessage(`<recv>`, response.media);
      }
    } else {
      this.printMessage(`<recv>`, response);
    }
  }

  printMessage(status, message) {
    let str;
    if (this.options.outputJson) {
      // 2=prettyprint with two-space indentation
      str = JSON.stringify(message, null, 2);
    } else {
      str = message;
    }

    this.printStatus(status);
    console.log(str);
  }

  handleError(what) {
    console.error(`error: ${what}`);
    process.exit(1);
  }

  static create(options) {
    return new Jomiel(options);
  }
}

module.exports = {
  Jomiel,
};

// vim: set ts=2 sw=2 tw=72 expandtab:
