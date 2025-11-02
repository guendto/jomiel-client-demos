/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-client-demos
 *
 * Copyright
 *  2019-2021,2025 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */
use prost::Message;
use std::convert::TryFrom;
use zmq::{Context, REQ, Socket};

use super::options::Options;
use crate::proto::jomiel::protobuf::v1beta1::{
    Inquiry, MediaInquiry, MediaResponse, Response, StatusCode,
    inquiry, response,
};

pub struct Jomiel {
    opts: Options,
    sck: Socket,
}

impl Jomiel {
    pub fn new(options: Options) -> Self {
        let r: Jomiel = Jomiel {
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
                self.send_inquiry(uri);
                self.receive_response();
            }
        } else {
            eprintln!("error: input URI not given");
            std::process::exit(1);
        }
    }

    fn connect(&self) {
        let re: &String = &self.opts.router_endpoint;
        let to: &i64 = &self.opts.connect_timeout;
        self.print_status(&format!(
            "<connect> {} (timeout={})",
            re, to
        ));
        self.sck.connect(re).unwrap();
    }

    fn send_inquiry(&self, uri: &str) {
        let mut msg: Inquiry = Inquiry::default();

        msg.inquiry = Some(inquiry::Inquiry::Media(MediaInquiry {
            input_uri: uri.to_string(),
        }));

        let mut buf: Vec<u8> = Vec::new();
        msg.encode(&mut buf).unwrap();

        if !self.opts.be_terse {
            self.print_message("<send>", &msg);
        }
        self.sck.send(&buf, 0).unwrap();
    }

    fn receive_response(&self) {
        let mut poll_items = [self.sck.as_poll_item(zmq::POLLIN)];
        let timeout: i64 = self.opts.connect_timeout * 1000;

        zmq::poll(&mut poll_items, timeout).unwrap();

        if poll_items[0].is_readable() {
            let bytes: Vec<u8> = self.sck.recv_bytes(0).unwrap();
            let msg: Response = Response::decode(&*bytes).unwrap();
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
        let status: &str = "<recv>";

        if let Some(st) = &msg.status {
            if let Ok(code) = StatusCode::try_from(st.code) {
                if code == StatusCode::Ok {
                    if let Some(response::Response::Media(media)) =
                        &msg.response
                    {
                        if self.opts.be_terse {
                            self.dump_terse_response(media);
                        } else {
                            self.print_message(status, msg);
                        }
                    }
                } else {
                    self.print_message(status, msg);
                }
            } else {
                eprintln!("Unknown status code: {}", st.code);
            }
        } else {
            eprintln!("Response missing status");
        }
    }

    fn dump_terse_response(&self, msg: &MediaResponse) {
        println!("---\ntitle: {}\nquality:", msg.title);
        for stream in &msg.stream {
            if let Some(qty) = &stream.quality {
                println!(
                    "  profile: {}\n    width: {}\n    height: {}",
                    qty.profile, qty.width, qty.height
                );
            }
        }
    }

    fn print_message<T: serde::Serialize + std::fmt::Debug>(
        &self,
        label: &str,
        msg: &T,
    ) {
        self.print_status(label);
        if self.opts.output_json {
            println!("{}", serde_json::to_string_pretty(msg).unwrap());
        } else {
            println!("{:#?}", msg);
        }
    }
}
