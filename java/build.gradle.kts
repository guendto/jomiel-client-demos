/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-client-demos
 *
 * Copyright
 *  2021-2022 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

val pbVersion by extra("3.19.2")

plugins {
    // <https://plugins.gradle.org/plugin/com.google.protobuf>
    id("com.google.protobuf") version "0.8.18"
    id("application")
    id("java")
    // id("com.github.sherter.google-java-format") version "0.9"
    // id("pmd")
}

application {
    mainClass.set("jomiel.client.demos.App")
    applicationName = "demo"
}

java {
    sourceCompatibility = JavaVersion.VERSION_11
    targetCompatibility = JavaVersion.VERSION_11
}

repositories {
    mavenCentral()
}

dependencies {
    implementation("com.google.protobuf:protobuf-java-util:$pbVersion")
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
