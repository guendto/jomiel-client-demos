/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-client-demos
 *
 * Copyright
 *  2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

import com.google.protobuf.gradle.*

plugins {
    id("com.google.protobuf") version "0.8.18"
    scala
    application
}

repositories {
    mavenCentral()
}

application {
    mainClass.set("jomiel.client.demos.App")
    applicationName = "demo"
}

dependencies {
    implementation("com.thesamet.scalapb:scalapb-runtime_2.12:0.11.8")
    implementation("com.thesamet.scalapb:scalapb-json4s_2.12:0.12.0")
    implementation("org.tinylog:tinylog-api-scala_2.12:2.4.1")
    implementation("org.tinylog:tinylog-impl:2.4.1")
    implementation("info.picocli:picocli:4.6.2")
    implementation("org.zeromq:jeromq:0.5.2")
}

sourceSets {
    main {
        withConvention(ScalaSourceSet::class) {
            scala.setSrcDirs(listOf(
                "src/main/scala",
                "build/generated/source/proto/main/scalapb"
            ))
        }
        proto {
            srcDirs("../proto")
        }
    }
}

protobuf {
    // Configure the protoc executable
    protoc {
        // Download from repositories (pre-compl. protoc, MavenCentral):
        // - <https://github.com/google/protobuf-gradle-plugin#customizing-protobuf-compilation>
        // artifact = "com.google.protobuf:protoc:3.15.6"
        //
        // Or, specify a local path:
        // - e.g. ideal for building on alpine/linux
        path = "/usr/bin/protoc"
    }
    plugins {
        id("scalapb") {
            artifact = "com.thesamet.scalapb:protoc-gen-scala:0.11.8:unix@sh"
        }
    }
    generateProtoTasks {
        ofSourceSet("main").forEach { task ->
            task.builtins {
                remove("java")
            }
            task.plugins {
                id("scalapb")
            }
        }
    }
}
