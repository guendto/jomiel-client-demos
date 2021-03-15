/*
 * -*- coding: utf-8 -*-
 *
 * Copyright
 *  2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

plugins {
    id("com.github.sherter.google-java-format") version "0.9"
    id("com.github.johnrengelman.shadow") version "6.1.0"
    id("com.google.protobuf") version "0.8.15"
    id("application")
    id("java")
    id("pmd")
}

application {
    mainClass.set("jomiel.examples.App")
    applicationName = "demo"
    // <https://github.com/johnrengelman/shadow/issues/609>
    // <https://github.com/johnrengelman/shadow/issues/645>
    mainClassName = application.mainClass.get()
}

java {
    sourceCompatibility = JavaVersion.VERSION_11
    targetCompatibility = JavaVersion.VERSION_11
}

repositories {
    mavenCentral()
}

dependencies {
    implementation("com.google.protobuf:protobuf-java-util:3.15.2")
    implementation("com.google.protobuf:protobuf-java:3.15.2")
    implementation("org.tinylog:tinylog-impl:2.2.1")
    implementation("org.tinylog:tinylog-api:2.2.1")
    implementation("info.picocli:picocli:4.6.1")
    implementation("org.zeromq:jeromq:0.5.2")
}

sourceSets.main {
    java.srcDirs("build/generated/source/proto/main/java")
    // 'com.google.protobuf' plugin.
    proto.srcDirs("../proto")
}

// println("srcDirs=" + sourceSets["main"].java.srcDirs)
