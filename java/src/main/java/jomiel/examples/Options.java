/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2019-2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package jomiel.examples;

import static picocli.CommandLine.Command;

import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.List;
import picocli.CommandLine.Option;
import picocli.CommandLine.Parameters;

@Command(name = "demo")
public class Options {

  @Option(
      names = {"-D", "--print-config"},
      description = "Print configuration values and exit")
  private boolean printConfig;

  @Option(
      names = {"-h", "--help"},
      usageHelp = true,
      description = "Display this help message and exit")
  private boolean showHelp;

  @Option(
      names = {"-r", "--router-endpoint"},
      paramLabel = "<addr>",
      description = "Specify the router endpoint address (default: ${DEFAULT-VALUE})")
  private String routerEndpoint = "tcp://localhost:5514";

  @Option(
      names = {"-t", "--connect-timeout"},
      paramLabel = "<timeout>",
      description =
          "Specify maximum time in seconds for the connection allowed to take (default: ${DEFAULT-VALUE})")
  private int connectTimeout = 30;

  @Option(
      names = {"-V", "--version-zmq"},
      description = "Display ZeroMQ version and exit")
  private boolean showZmqVersion;

  @Option(
      names = {"-j", "--output-json"},
      description = "Print dumped messages in JSON")
  private boolean outputJson;

  @Option(
      names = {"-q", "--be-terse"},
      description = "Be brief and to the point; dump interesting details only")
  private boolean beTerse;

  @Parameters(arity = "0..*", paramLabel = "<uri>", description = "the URIs to parse")
  private List<String> uri = new ArrayList<String>();

  public final boolean printConfig() {
    return printConfig;
  }

  public final boolean showHelp() {
    return showHelp;
  }

  public final String getRouterEndpoint() {
    return routerEndpoint;
  }

  public final int getConnectTimeout() {
    return connectTimeout * 1000;
  }

  public final boolean showZmqVersion() {
    return showZmqVersion;
  }

  public final boolean outputAsJson() {
    return outputJson;
  }

  public final boolean beTerse() {
    return beTerse;
  }

  public final List<String> getUri() {
    return uri;
  }

  public final void dump() {
    System.out.println("---");
    for (final Field f : getClass().getDeclaredFields()) {
      try {
        System.out.println(f.getName() + ": " + f.get(this));
      } catch (final IllegalAccessException exc) {
        // Pass.
      }
    }
  }
}

// vim: set ts=2 sw=2 tw=72 expandtab:
