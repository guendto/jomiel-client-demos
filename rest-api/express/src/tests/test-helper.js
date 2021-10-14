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

const verifyResponse = ({
  response,
  expectStatus,
  expectMessage,
  expectData,
}) => {
  expect(response.body.status).toEqual(expectStatus);
  if (expectMessage) {
    expect(response.body.data.message).toMatch(expectMessage);
  }
  if (expectData) {
    expect(response.body.data.title).toEqual(expectData.title);
  }
};

const testHttpPost = async ({
  api,
  path,
  payload,
  expectCode,
  expectStatus,
  expectMessage,
  expectData,
}) => {
  const response = await api
    .post(path)
    .send(payload)
    .expect(expectCode)
    .expect("Content-Type", /application\/json/);
  verifyResponse({ response, expectStatus, expectMessage, expectData });
};

const testHttpGet = async ({
  api,
  path,
  expectCode,
  expectStatus,
  expectMessage,
  expectData,
}) => {
  const response = await api
    .get(path)
    .expect(expectCode)
    .expect("Content-Type", /application\/json/);
  verifyResponse({ response, expectStatus, expectMessage, expectData });
};

export default { testHttpPost, testHttpGet };
