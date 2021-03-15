/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

rootProject.name = "jomiel.examples"

startParameter.setExcludedTaskNames(listOf(
    // ":verifyGoogleJavaFormat",
    // ":shadowJar",
    // ":shadowDistTar",
    ":shadowDistZip",
    // ":pmdMain",
    // ":pmdTest",
    ":distTar",
    ":distZip",
    ":jar",
    ":extractIncludeTestProto",
    ":processTestResources",
    ":extractTestProto",
    ":compileTestJava",
    ":testClasses",
    ":test"
))
