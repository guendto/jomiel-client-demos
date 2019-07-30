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

package io.github.jomiel.examples;

import org.zeromq.ZMQ;
import picocli.CommandLine;
import picocli.CommandLine.ParameterException;

public final class Demo {
  public static void main(final String[] args) {
    final Options opts = new Options();
    try {
      CommandLine.populateCommand(opts, args);
      if (opts.showHelp()) {
        CommandLine.usage(opts, System.out);
        return;
      } else if (opts.showZmqVersion()) {
        printZmqVersion();
        return;
      } else if (opts.printConfig()) {
        opts.dump();
        return;
      }
    } catch (final ParameterException exc) {
      System.err.println(exc.getMessage());
      CommandLine.usage(opts, System.out);
      return;
    }

    try {
      final Jomiel jomiel = Jomiel.create(opts);
      jomiel.inquire();
    } catch (final Exception exc) {
      System.err.println("error: " + exc.getMessage());
      System.exit(1);
    }
  }

  private static void printZmqVersion() {
    final String version = ZMQ.getVersionString();
    final int fullVersion = ZMQ.getFullVersion();
    System.out.println(String.format("ZeroMQ version %s (%d)", version, fullVersion));
  }
}
