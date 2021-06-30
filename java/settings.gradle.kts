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

rootProject.name = "jomiel.client.demos"

startParameter.setExcludedTaskNames(listOf(
    // ":verifyGoogleJavaFormat",
    // ":pmdMain",
    // ":pmdTest",
    // ":distTar",
    ":distZip",
    // ":jar",
    ":extractIncludeTestProto",
    ":processTestResources",
    ":extractTestProto",
    ":compileTestJava",
    ":testClasses",
    ":test"
))
