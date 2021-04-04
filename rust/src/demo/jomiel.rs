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

use std::fmt::Debug;

use protobuf::Message;
use zmq::{Context, Socket, REQ};

use super::options::Options;
use crate::proto::media::MediaResponse;
use crate::proto::message::{Inquiry, Response};
use crate::proto::status::StatusCode;

pub struct Jomiel {
    opts: Options,
    sck: Socket,
}

impl Jomiel {
    pub fn new(options: Options) -> Self {
        let r = Jomiel {
            sck: Context::new().socket(REQ).unwrap(),
            opts: options,
        };
        r.sck.set_linger(0).unwrap();
        r
    }

    pub fn inquire(&self) {
        if !self.opts.uri.is_empty() {
            self.connect();
            for uri in &self.opts.uri {
                self.send_inquiry(&uri);
                self.receive_response();
            }
        } else {
            eprintln!("error: input URI not given");
            std::process::exit(1);
        }
    }

    fn connect(&self) {
        let re = &self.opts.router_endpoint;
        let to = &self.opts.connect_timeout;
        self.print_status(&format!(
            "<connect> {} (timeout={})",
            re, to
        ));
        self.sck.connect(&re).unwrap();
    }

    fn send_inquiry(&self, uri: &str) {
        let mut msg = Inquiry::new();
        msg.mut_media().input_uri = uri.into();

        let bytes = msg.write_to_bytes().unwrap();
        // println!("{:X?}", bytes);

        if !self.opts.be_terse {
            self.print_message::<&Inquiry>("<send>", &msg);
        }
        self.sck.send(&bytes, 0).unwrap();
    }

    fn receive_response(&self) {
        let mut poll_items = [self.sck.as_poll_item(zmq::POLLIN)];
        let timeout = self.opts.connect_timeout * 1000; // to msec

        zmq::poll(&mut poll_items, timeout).unwrap();

        if poll_items[0].is_readable() {
            let bytes = self.sck.recv_bytes(0).unwrap();
            let msg = Response::parse_from_bytes(&bytes).unwrap();
            // println!("{:X?}", bytes);
            self.dump_response(&msg);
        } else {
            eprintln!("error: connection timed out");
            std::process::exit(1);
        }
    }

    fn print_status(&self, status: &str) {
        if !self.opts.be_terse {
            eprintln!("{}", status);
        }
    }

    fn dump_response(&self, msg: &Response) {
        let status = "<recv>";
        if msg.get_status().get_code() == StatusCode::STATUS_CODE_OK {
            let media = msg.get_media();
            if self.opts.be_terse {
                self.dump_terse_response(media);
            } else {
                self.print_message::<&MediaResponse>(status, media);
            }
        } else {
            self.print_message::<&Response>(status, msg);
        }
    }

    fn dump_terse_response(&self, msg: &MediaResponse) {
        println!("---\ntitle: {}\nquality:", msg.get_title());
        for stream in msg.get_stream() {
            let qty = stream.get_quality();
            println!(
                "  profile: {}\n    width: {}\n    height: {}",
                qty.get_profile(),
                qty.get_width(),
                qty.get_height()
            );
        }
    }

    fn print_message<T: serde::Serialize + Debug>(
        &self,
        status: &str,
        msg: T,
    ) {
        self.print_status(status);
        if self.opts.output_json {
            println!("{}", self.to_json(msg));
        } else {
            print!("{:#?}", msg);
        }
    }

    fn to_json<T: serde::Serialize>(&self, msg: T) -> String {
        return if self.opts.compact_json {
            serde_json::to_string(&msg).unwrap()
        } else {
            serde_json::to_string_pretty(&msg).unwrap()
        };
    }
}
