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

import supertest from "supertest";

import app from "../app";
import helper from "./test-helper";

const api = supertest(app);

describe("POST /api/by-url", () => {
  it("should fail when input uri is missing", async () => {
    await helper.testHttpPost({
      api,
      path: "/api/by-url",
      payload: {},
      expectCode: 400,
      expectStatus: "error",
      expectMessage: "missing inputUri",
    });
  });

  it("should fail when input uri is not supported by jomiel", async () => {
    await helper.testHttpPost({
      api,
      path: "/api/by-url",
      payload: { inputUri: "https://foo.bar/baz" },
      expectCode: 500,
      expectStatus: "error",
      expectMessage:
        "Unable to find a matching parser for URI <https://foo.bar/baz>",
    });
  });

  it("should return video details", async () => {
    await helper.testHttpPost({
      api,
      path: "/api/by-url",
      payload: { inputUri: "https://youtu.be/PRdcZSuCpNo" },
      expectCode: 200,
      expectStatus: "ok",
      expectData: {
        title: "7 Days of Artificial Intelligence",
      },
    });
  });
});
