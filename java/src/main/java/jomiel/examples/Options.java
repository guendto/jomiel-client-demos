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

import picocli.CommandLine.Command;
import picocli.CommandLine.Option;
import picocli.CommandLine.Parameters;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;

import static java.lang.String.format;
import static java.lang.System.exit;
import static org.tinylog.Logger.info;
import static org.zeromq.ZMQ.getFullVersion;
import static org.zeromq.ZMQ.getVersionString;
import static picocli.CommandLine.usage;

@SuppressWarnings({"CanBeFinal", "FieldMayBeFinal"})
@Command(name = "demo", usageHelpAutoWidth = true)
final class Options implements Callable<Integer> {

    @Option(names = {"-h", "--help"},
            usageHelp = true,
            description = "Display this help message and exit")
    private boolean help = false;

    @Option(names = {"-D", "--print-config"},
            description = "Print configuration values and exit")
    private boolean printConfig = false;

    @Option(names = {"-r", "--router-endpoint"},
            paramLabel = "<addr>",
            description = "Specify the router endpoint address "
                    + "(${DEFAULT-VALUE})")
    String routerEndpoint = "tcp://localhost:5514";

    @Option(names = {"-t", "--connect-timeout"},
            paramLabel = "<timeout>",
            description = "Specify maximum time in seconds for the "
                    + "connection allowed to take "
                    + "(${DEFAULT-VALUE})")
    long connectTimeout = 30;

    @Option(names = {"-V", "--version-zmq"},
            description = "Display ZeroMQ version and exit")
    private boolean zmqVersion = false;

    @Option(names = {"-j", "--output-json"},
            description = "Print dumped messages in JSON")
    boolean outputJson = false;

    @Option(names = {"-c", "--compact-json"},
            description = "Use more compact representation of JSON")
    boolean compactJson = false;

    @Option(names = {"-q", "--be-terse"},
            description = "Be brief and to the point; dump interesting "
                    + "details only")
    boolean beTerse = false;

    @Parameters(arity = "0..*",
            paramLabel = "<uri>",
            description = "the URIs to parse")
    List<String> uri = new ArrayList<>();

    @Override
    public final Integer call() throws Exception {
        if (help) usage(this, System.out);
        else if (printConfig) dumpConfig();
        else if (zmqVersion) printZmqVersion();
        else new Jomiel(this).run();
        return 0;
    }

    private void dumpConfig() throws IllegalAccessException {
        info("---");
        for (final var field : getClass().getDeclaredFields()) {
            field.setAccessible(true);
            info(format("%s: %s",
                    field.getName(),
                    field.get(this)));
        }
        exit(0);
    }

    private void printZmqVersion() {
        info(format("ZeroMQ version %s (%s)",
                getVersionString(),
                getFullVersion()));
        exit(0);
    }
}
