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
    implementation("com.thesamet.scalapb:scalapb-runtime_2.12:0.10.10")
    implementation("com.thesamet.scalapb:scalapb-json4s_2.12:0.10.3")
    implementation("org.tinylog:tinylog-api-scala_2.12:2.2.1")
    implementation("org.tinylog:tinylog-impl:2.2.1")
    implementation("info.picocli:picocli:4.6.1")
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
    protoc {
        artifact = "com.google.protobuf:protoc:3.15.6"
    }
    plugins {
        id("scalapb") {
            artifact = "com.thesamet.scalapb:protoc-gen-scala:0.10.10:unix@sh"
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
