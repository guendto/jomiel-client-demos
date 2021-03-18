/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2019-2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package app

import (
	"fmt"
	"log"
	"time"

	"github.com/pebbe/zmq4"
	"github.com/spf13/pflag"
	"google.golang.org/protobuf/encoding/protojson"
	"google.golang.org/protobuf/encoding/prototext"
	"google.golang.org/protobuf/proto"

	msgs "internal/messages"
)

type jomiel struct {
	opts    options
	poller  *zmq4.Poller
	sock    *zmq4.Socket
	timeout time.Duration
}

func newJomiel(opts options) jomiel {
	sck, err := zmq4.NewSocket(zmq4.REQ)
	if err != nil {
		log.Fatalln("error: failed to create a socket: ", err)
	}
	sck.SetLinger(0)

	poller := zmq4.NewPoller()
	poller.Add(sck, zmq4.POLLIN)

	timeout := time.Duration(opts.ConnectTimeout)
	timeout *= time.Second

	return jomiel{
		timeout: timeout,
		poller:  poller,
		opts:    opts,
		sock:    sck,
	}
}

func (j jomiel) inquire() {
	if pflag.NArg() > 0 {
		j.connect()
		for i := 0; i < pflag.NArg(); i++ {
			uri := pflag.Arg(i)
			j.sendInquiry(uri)
			j.receiveResponse()
		}
	} else {
		log.Fatalln("error: no input URI given")
	}
}

func (j *jomiel) connect() {
	re := j.opts.RouterEndpoint
	to := j.opts.ConnectTimeout
	j.printStatus(fmt.Sprintf("<connect> %s (timeout=%d)", re, to))
	j.sock.Connect(re)
}

func (j *jomiel) sendInquiry(uri string) {
	msg := newInquiry(uri)
	if !opts.BeTerse {
		j.printMessage("<send>", msg)
	}
	bytes := marshalInquiry(msg)
	j.sock.SendMessage(bytes)
}

func (j *jomiel) receiveResponse() {
	polled, err := j.poller.Poll(j.timeout)
	if err != nil {
		log.Fatalln("error: failed to pollin an event: ", err)
	}

	if len(polled) == 0 {
		log.Fatalln("error: connection timed out")
	}

	bytes, err := j.sock.RecvBytes(0)
	if err != nil {
		log.Fatalln("error: failed to receive message: ", err)
	}

	msg := unmarshalResponse(bytes)
	j.dumpResponse(&msg)
}

func (j *jomiel) printStatus(status string) {
	if !j.opts.BeTerse {
		log.Println("status: " + status)
	}
}

func (j *jomiel) toJson(response *msgs.Response) string {
	json, err := json.MarshalIndent(response, "", "  ")
	if err != nil {
		log.Fatalln("error: ", err)
	}
	return string(json)
}

func (j *jomiel) printMessage(status string, message *msgs.Response) {
	var result string
	if j.opts.OutputJson {
		result = j.toJson(message)
	} else {
		result = proto.MarshalTextString(message)
	}
	j.printStatus(status)
	fmt.Printf(result)
}

func (j *jomiel) dumpResponse(response *msgs.Response) {
	responseStatus := response.GetStatus()
	mediaResponse := response.GetMedia()

	if responseStatus.GetCode() == msgs.StatusCode_STATUS_CODE_OK {
		if j.opts.BeTerse {
			j.dumpTerseResponse(mediaResponse)
		} else {
			j.printMessage("<recv>", response)
		}
	} else {
		j.printMessage("<recv>", response)
	}
}

func (j *jomiel) dumpTerseResponse(response *msgs.MediaResponse) {
	fmt.Println("---\ntitle: " + response.GetTitle())
	fmt.Println("quality:")

	stream := response.GetStream()

	for i := 0; i < len(stream); i++ {
		quality := stream[i].GetQuality()
		fmt.Printf("  profile: %s\n    width: %d\n    height: %d\n",
			quality.GetProfile(), quality.GetWidth(), quality.GetHeight())
	}
}

func newInquiry(uri string) *msgs.Inquiry {
	return &msgs.Inquiry{
		Inquiry: &msgs.Inquiry_Media{
			Media: &msgs.MediaInquiry{
				InputUri: uri,
			},
		},
	}
}

func marshalInquiry(msg proto.Message) []byte {
	bytes, err := proto.Marshal(msg)
	if err != nil {
		log.Fatalln("error: failed to encode inquiry: ", err)
	}
	return bytes
}

func unmarshalResponse(bytes []byte) msgs.Response {
	msg := msgs.Response{}
	if err := proto.Unmarshal(bytes, &msg); err != nil {
		log.Fatalln("error: failed to decode response: ", err)
	}
	return msg
}
