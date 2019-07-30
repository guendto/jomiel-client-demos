/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2019 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package demo

import (
	"encoding/json"
	"fmt"
	"log"

	proto "github.com/golang/protobuf/proto"
	czmq "gopkg.in/zeromq/goczmq.v4"

	pb "jomiel.github.io/demo/examples/proto"
)

type Jomiel struct {
	sock *czmq.Sock
	opts *Options
}

func NewJomiel(opts *Options) *Jomiel {
	sck := czmq.NewSock(czmq.Req)
    sck.SetOption(czmq.SockSetLinger(0))
	return &Jomiel{
		opts: opts,
		sock: sck,
	}
}

func (j *Jomiel) Destroy() {
	defer j.sock.Destroy()
}

func (j *Jomiel) Connect() {
	re := j.opts.RouterEndpoint
	to := j.opts.ConnectTimeout

	status := fmt.Sprintf("<connect> %s (timeout=%d)", re, to)

	j.printStatus(status)
	j.sock.Connect(j.opts.RouterEndpoint)
}

func (j *Jomiel) Inquire(uri string) {
	j.send(uri)
	j.recv()
}

func (j *Jomiel) send(uri string) {
	inquiry := &pb.Inquiry{
		Inquiry: &pb.Inquiry_Media{
			Media: &pb.MediaInquiry{
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

func (j *Jomiel) recv() {
	poller, err := czmq.NewPoller(j.sock)
	if err != nil {
		log.Fatalln("failed to create a poller: ", err)
	}
	defer poller.Destroy()

	sck, err := poller.Wait(j.opts.ConnectTimeout * 1000)
	if err != nil {
        log.Fatalln("failed to pollin an event: ", err)
	} else {
        if sck == nil {
            log.Fatalln("error: connection timed out")
        }
    }

	data, err := j.sock.RecvMessage()
	if err != nil {
		log.Fatalln("failed to receive message: ", err)
	}

	response := &pb.Response{}
	err = proto.Unmarshal(data[0], response)
	if err != nil {
		log.Fatalln("failed to decode response: ", err)
	}

	j.dumpResponse(response)
}

func (j *Jomiel) printStatus(status string) {
	if !j.opts.BeTerse {
		log.Println("status: " + status)
	}
}

func (j *Jomiel) toJson(response *pb.Response) string {
	json, err := json.MarshalIndent(response, "", "  ")
	if err != nil {
		log.Fatalln("error: ", err)
	}
	return string(json)
}

func (j *Jomiel) printMessage(status string, message *pb.Response) {
	var result string
	if j.opts.OutputJson {
		result = j.toJson(message)
	} else {
		result = proto.MarshalTextString(message)
	}
	j.printStatus(status)
	fmt.Printf(result)
}

func (j *Jomiel) dumpResponse(response *pb.Response) {
	responseStatus := response.GetStatus()
	mediaResponse := response.GetMedia()

	if responseStatus.GetCode() == pb.StatusCode_OK {
		if j.opts.BeTerse {
			j.dumpTerseResponse(mediaResponse)
		} else {
			j.printMessage("<recv>", response)
		}
	} else {
		j.printMessage("<recv>", response)
	}
}

func (j *Jomiel) dumpTerseResponse(response *pb.MediaResponse) {
	fmt.Println("---\ntitle: " + response.GetTitle())
	fmt.Println("quality:")

	stream := response.GetStream()

	for i := 0; i < len(stream); i++ {
		quality := stream[i].GetQuality()
		fmt.Printf("  profile: %s\n    width: %d\n    height: %d\n",
			quality.GetProfile(), quality.GetWidth(), quality.GetHeight())
	}
}
