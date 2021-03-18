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

	proto "github.com/golang/protobuf/proto"
	zmq4 "github.com/pebbe/zmq4"
	"google.golang.org/protobuf/encoding/protojson"
	"google.golang.org/protobuf/encoding/prototext"
	"google.golang.org/protobuf/proto"

	msgs "internal/messages"
)

type jomiel struct {
	opts   options
	poller *zmq4.Poller
	sock   *zmq4.Socket
}

func newJomiel(opts *options) *jomiel {
	sck, err := zmq4.NewSocket(zmq4.REQ)
	if err != nil {
		log.Fatalln("failed to create a socket: ", err)
	}
	sck.SetLinger(0)
	return &jomiel{
		opts: opts,
		sock: sck,
	}
}

func (j *jomiel) Destroy() {
	j.sock.Close()
}

func (j *jomiel) connect() {
	re := j.opts.RouterEndpoint
	to := j.opts.ConnectTimeout

	status := fmt.Sprintf("<connect> %s (timeout=%d)", re, to)

	j.printStatus(status)
	j.sock.Connect(j.opts.RouterEndpoint)
}

func (j *jomiel) inquire(uri string) {
	j.send(uri)
	j.recv()
}

func (j *jomiel) send(uri string) {
	inquiry := &msgs.Inquiry{
		Inquiry: &msgs.Inquiry_Media{
			Media: &msgs.MediaInquiry{
				InputUri: uri,
			},
		},
	}

	out, err := proto.Marshal(inquiry)
	if err != nil {
		log.Fatalln("failed to encode inquiry: ", err)
	}

	j.printStatus("<send>")
	j.sock.SendMessage([][]byte{out})
}

func (j *jomiel) recv() {
	poller := zmq4.NewPoller()
	poller.Add(j.sock, zmq4.POLLIN)

	timeout := time.Duration(j.opts.ConnectTimeout)
	sck, err := poller.Poll(timeout * time.Second)
	if err != nil {
		log.Fatalln("failed to pollin an event: ", err)
	} else {
		if len(sck) == 0 {
			log.Fatalln("error: connection timed out")
		}
	}

	data, err := j.sock.Recv(0)
	if err != nil {
		log.Fatalln("failed to receive message: ", err)
	}

	response := &msgs.Response{}
	err = proto.Unmarshal([]byte(data), response)
	if err != nil {
		log.Fatalln("failed to decode response: ", err)
	}

	j.dumpResponse(response)
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
