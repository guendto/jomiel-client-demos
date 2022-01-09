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

import com.google.protobuf.gradle.*
import org.jetbrains.kotlin.gradle.plugin.KotlinSourceSet
import org.jetbrains.kotlin.gradle.tasks.KotlinCompile

val pbandkVersion by extra("0.13.0")

plugins {
    // <https://plugins.gradle.org/plugin/org.jetbrains.kotlin.jvm>
    id("org.jetbrains.kotlin.jvm") version "1.6.10"
    // <https://plugins.gradle.org/plugin/com.google.protobuf>
    id("com.google.protobuf") version "0.8.18"
    application
}

repositories {
    mavenCentral()
}

application {
    mainClass.set("jomiel.client.demos.AppKt")
    applicationName = "demo"
}

dependencies {
    implementation("pro.streem.pbandk:pbandk-runtime-jvm:$pbandkVersion")
    implementation("org.jetbrains.kotlin:kotlin-reflect:1.6.10")
    implementation("org.tinylog:tinylog-api-kotlin:2.2.1")
    implementation("org.tinylog:tinylog-impl:2.2.1")
    implementation("info.picocli:picocli:4.6.1")
    implementation("org.zeromq:jeromq:0.5.2")
}

sourceSets {
    main {
        withConvention(KotlinSourceSet::class) {
            kotlin.srcDirs(
                "build/generated/source/proto/main/kotlin"
            )
        }
        proto {
            srcDirs("../proto")
        }
    }
}

protobuf {
    protoc {
        artifact = "com.google.protobuf:protoc:3.16.0"
    }
    plugins {
        id("kotlin") {
            artifact =
                "pro.streem.pbandk:protoc-gen-pbandk-jvm:$pbandkVersion:jvm8@jar"
        }
    }
    generateProtoTasks {
        ofSourceSet("main").forEach { task ->
            task.builtins {
                remove("java")
            }
            task.plugins {
                id("kotlin")
            }
        }
    }
}

val jvmCompilerArgs = listOfNotNull(
    "-Xopt-in=kotlin.RequiresOptIn"
)

tasks {
    compileJava {
        enabled = false
    }
    withType<KotlinCompile>().configureEach {
        kotlinOptions {
            freeCompilerArgs = jvmCompilerArgs
            jvmTarget= "11"
        }
    }
}
