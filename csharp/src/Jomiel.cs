/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2019-2020 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

using System.Collections.Generic;
using System.IO;
using System;

using Google.Protobuf;
using ZeroMQ;

using static Jomiel.ProtoBuf.V1Alpha1.MediaResponse.Types.Stream.Types; // StreamQuality
using Jomiel.ProtoBuf.V1Alpha1;

namespace Demo {

public class Jomiel {
    private static readonly log4net.ILog log =
	    log4net.LogManager.GetLogger(
            System.Reflection.MethodBase.GetCurrentMethod().DeclaringType
        );
    private IDictionary<string, DocoptNet.ValueObject> opts;
    private ZContext ctx;
    private ZSocket sck;
    private string addr;
    private int timeout;

    public Jomiel(IDictionary<string, DocoptNet.ValueObject> opts) {
        this.ctx = new ZContext();
        this.sck = new ZSocket(this.ctx, ZSocketType.REQ);

        this.sck.Linger = TimeSpan.FromMilliseconds(0);
        this.opts = opts;
    }

    ~Jomiel() {
        // this.sck.Disconnect(this.addr);
        // this.ctx.Terminate();
    }

    public void connect() {
        this.addr = this.opts["--router-endpoint"].ToString();
        this.timeout = this.opts["--connect-timeout"].AsInt;

        var status = String.Format("<connect> {0} (timeout={1})",
                        this.addr, this.timeout);

        this.printStatus(status);
        this.sck.Connect(this.addr);
    }

    public void inquire(string uri) {
        this.send(uri);
        this.recv();
    }

    private void send(string uri) {
        Inquiry inquiry = new Inquiry {
            Media = new MediaInquiry { InputUri = uri }
        };

        using (MemoryStream stream = new MemoryStream()) {
            inquiry.WriteTo(stream);
            byte[] bytes = stream.ToArray();
            this.sck.Send(new ZFrame(bytes));
        }
    }

    private void recv() {
        var timeout = TimeSpan.FromMilliseconds(this.timeout * 1000);
        var poll = ZPollItem.CreateReceiver();

        ZMessage msg;
        ZError error;

        if (this.sck.PollIn(poll, out msg, out error, timeout)) {
            using (var stream = msg.Pop()) {
                Response response = Response.Parser.ParseFrom(stream);
                this.dumpResponse(response);
                // Console.WriteLine(response.ToString());
            }
        } else {
            if (error == ZError.EAGAIN) {
                log.Error("connection timed out");
                System.Environment.Exit(1);
            } else {
                throw new ZException(error);
            }
        }
    }

    private string getQualityString(StreamQuality streamQuality) {
        return String.Format(
            "  profile: {0}\n    width: {1}\n    height: {2}",
                streamQuality.Profile,
                streamQuality.Width,
                streamQuality.Height
        );
    }

    private void dumpTerseResponse(MediaResponse mediaResponse) {
        Console.WriteLine("---\ntitle: {0}", mediaResponse.Title);
        Console.WriteLine("quality:");

        foreach (var stream in mediaResponse.Stream) {
            var streamQuality = stream.Quality;
            var qualityString = this.getQualityString(streamQuality);
            Console.WriteLine(qualityString);
        }
    }

    private void dumpResponse(Response response) {
        var status = response.Status;

        if (status.Code == StatusCode.Ok) {
            var mediaResponse = response.Media;
            if (this.opts["--be-terse"].IsTrue) {
                this.dumpTerseResponse(mediaResponse);
            } else {
                this.printMessage("<recv>", mediaResponse);
            }
        } else {
            this.printMessage("<recv>", response);
        }
    }

    private void printMessage(string status, IMessage message) {
        this.printStatus(status);
        Console.WriteLine(message.ToString());
    }

    private void printStatus(string status) {
        if (! this.opts["--be-terse"].IsTrue) {
            log.Info(status);
        }
    }
}

} // end of namespace
