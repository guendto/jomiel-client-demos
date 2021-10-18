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

describe("GET /api/yt/by-id/:id", () => {
  it("should fail when video id is missing", async () => {
    await helper.testHttpGet({
      api,
      path: "/api/yt/by-id/-invalid-id-",
      expectCode: 404,
      expectStatus: "error",
      expectMessage: "not found",
    });
  });

  it("should fail with invalid youtube video id", async () => {
    await helper.testHttpGet({
      api,
      path: "/api/yt/by-id/PRdcZSuCpNq", // Note the last 'q', should be 'o'
      expectCode: 500,
      expectStatus: "error",
      expectMessage: "Video unavailable",
    });
  });

  it("should return video details", async () => {
    await helper.testHttpGet({
      api,
      path: "/api/yt/by-id/PRdcZSuCpNo",
      expectCode: 200,
      expectStatus: "ok",
      expectData: {
        title: "7 Days of Artificial Intelligence",
      },
    });
  });
});
