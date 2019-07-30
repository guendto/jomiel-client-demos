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

use protobuf::Message;
use structopt::StructOpt;
use zmq::{Context, Socket, REQ};

use options::Options;
use proto::Media::MediaResponse;
use proto::Message::{Inquiry, Response};
use proto::Status::StatusCode;

#[allow(dead_code)]
pub struct Jomiel {
    pub opts: Options,
    context: Context,
    sock: Socket,
}

impl Jomiel {
    pub fn new() -> Self {
        let arg = Options::from_args();
        let ctx = Context::new();
        let sck = ctx.socket(REQ).unwrap();
        sck.set_linger(0).unwrap();
        Jomiel {
            context: ctx,
            sock: sck,
            opts: arg,
        }
    }

    pub fn connect(&self) {
        let re = &self.opts.router_endpoint;
        let to = &self.opts.connect_timeout;

        self.print_status(&format!("<connect> {} <timeout={}>", re, to));
        self.sock.connect(&re).unwrap();
    }

    pub fn inquire(&self, uri: &str) {
        self.send(uri);
        self.recv();
    }

    fn send(&self, uri: &str) {
        let mut inquiry = Inquiry::new();
        inquiry.mut_media().set_input_uri(uri.into());

        if !self.opts.be_terse {
            self.print_message("<send>", &inquiry);
        }

        let bytes = inquiry.write_to_bytes().unwrap();
        //println!("{:X?}", bytes);

        self.sock.send(&bytes, 0).unwrap();
    }

    fn recv(&self) {
        let mut items = [self.sock.as_poll_item(zmq::POLLIN)];
        let timeout = self.opts.connect_timeout * 1000;

        zmq::poll(&mut items, timeout).unwrap();

        if items[0].is_readable() {
            let bytes = self.sock.recv_bytes(0).unwrap();
            //println!("{:X?}", bytes);
            let response: Response = protobuf::parse_from_bytes(&bytes).unwrap();
            self.dump_response(&response);
        } else {
            println!("error: connection timed out");
            std::process::exit(1);
        }
    }

    pub fn print_status(&self, message: &str) {
        if !self.opts.be_terse {
            eprintln!("status: {}", message);
        }
    }

    pub fn dump_response(&self, response: &Response) {
        let response_status = response.get_status();
        let media_response = response.get_media();
        if response_status.get_code() == StatusCode::OK {
            if self.opts.be_terse {
                self.dump_terse_response(media_response);
            } else {
                self.print_message("<recv>", media_response);
            }
        } else {
            self.print_message("<recv>", response);
        }
    }

    pub fn dump_terse_response(&self, response: &MediaResponse) {
        println!("---\ntitle: {}", response.get_title());
        println!("quality:");
        for stream in response.get_stream() {
            let quality = stream.get_quality();
            println!(
                "  profile: {}\n    width: {}\n    height: {}",
                quality.get_profile(),
                quality.get_width(),
                quality.get_height()
            );
        }
    }

    pub fn print_message(&self, status: &str, response: &Message) {
        self.print_status(status);
        /*
        if self.opts.output_json {
            let json = serde_json::to_string(&response).unwrap();
        } else {
            print!("{:#?}", response);
        }
        */
        print!("{:#?}", response);
    }
}
