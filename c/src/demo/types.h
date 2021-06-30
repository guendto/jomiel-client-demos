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

#pragma once

#include "jomiel/protobuf/v1beta1/message.pb-c.h"

// Structures.

typedef Jomiel__Protobuf__V1beta1__Inquiry JomielInquiry;
typedef Jomiel__Protobuf__V1beta1__Response JomielResponse;

typedef Jomiel__Protobuf__V1beta1__MediaInquiry JomielMediaInquiry;

typedef Jomiel__Protobuf__V1beta1__MediaResponse JomielMediaResponse;
typedef Jomiel__Protobuf__V1beta1__MediaResponse__Stream JomielStream;
typedef Jomiel__Protobuf__V1beta1__MediaResponse__Stream__StreamQuality JomielStreamQuality;

// Macros.

#define jomiel_media_inquiry_init                                      \
  JOMIEL__PROTOBUF__V1BETA1__MEDIA_INQUIRY__INIT

#define jomiel_inquiry_init                                            \
  JOMIEL__PROTOBUF__V1BETA1__INQUIRY__INIT

#define jomiel_inquiry_case_media                                      \
  JOMIEL__PROTOBUF__V1BETA1__INQUIRY__INQUIRY_MEDIA

// Enums.

#define JOMIEL_STATUS_CODE_ok                                          \
  JOMIEL__PROTOBUF__V1BETA1__STATUS_CODE__STATUS_CODE_OK

// Functions.

#define jomiel_inquiry_pack                                            \
  jomiel__protobuf__v1beta1__inquiry__pack

#define jomiel_inquiry_packed_size                                     \
  jomiel__protobuf__v1beta1__inquiry__get_packed_size

#define jomiel_response_unpack                                         \
  jomiel__protobuf__v1beta1__response__unpack

#define jomiel_response_free_unpacked                                  \
  jomiel__protobuf__v1beta1__response__free_unpacked
