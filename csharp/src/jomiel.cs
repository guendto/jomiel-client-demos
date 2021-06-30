/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-client-demos
 *
 * Copyright
 *  2019-2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

using System.Collections.Generic;
using System.IO;
using System;

using Newtonsoft.Json.Serialization;
using Newtonsoft.Json;
using Google.Protobuf;
using DocoptNet;
using ZeroMQ;

using Jomiel.ProtoBuf.V1Beta1;

namespace Demo {

    using Options = IDictionary<string, DocoptNet.ValueObject>;

    internal class Jomiel {
        private Options _opts;
        private ZContext _ctx;
        private ZSocket _sck;
        private ZPollItem _poller;
        private JsonSerializerSettings _jsonSettings;

        internal Jomiel (Options opts)
        {
            _ctx = new ZContext ();
            _sck = new ZSocket (_ctx, ZSocketType.REQ);
            _poller = ZPollItem.CreateReceiver ();

            _jsonSettings = new JsonSerializerSettings {
                DefaultValueHandling = DefaultValueHandling.Ignore,
                ContractResolver = new DynamicContractResolver (),
            };

            _sck.Linger = TimeSpan.FromMilliseconds (0);
            _opts = opts;
        }

        internal void inquire ()
        {
            if (_opts ["--output-json"].IsTrue) {
                printStatus (
                    "`--output-json' ignored, the output is always JSON"
                );
            }
            var uri = _opts ["URI"].AsList;
            if (uri.Count > 0) {
                connect ();
                foreach (var _ in uri) {
                    sendInquiry (_.ToString ());
                    receiveResponse ();
                }
            } else {
                throw new InvalidOperationException ("no input URI given");
            }
        }

        private void connect ()
        {
            var re = _opts ["--router-endpoint"].ToString ();
            var to = _opts ["--connect-timeout"].AsInt;
            var st = String.Format ("<connect> {0} (timeout={1})", re, to);
            printStatus (st);
            _sck.Connect (re);
        }

        private void sendInquiry (string uri)
        {
            Inquiry msg = new Inquiry {
                Media = new MediaInquiry { InputUri = uri }
            };

            if (!_opts ["--be-terse"].IsTrue) {
                printMessage ("<send>", msg);
            }

            using (MemoryStream stream = new MemoryStream ()) {
                msg.WriteTo (stream);
                var bytes = stream.ToArray ();
                var frame = new ZFrame (bytes);
                _sck.Send (frame);
            }
        }

        private void receiveResponse ()
        {
            var to = _opts ["--connect-timeout"].AsInt;
            var timeout = TimeSpan.FromMilliseconds (to * 1000);

            ZMessage zmsg;
            ZError zerr;

            if (_sck.PollIn (_poller, out zmsg, out zerr, timeout)) {
                using (var stream = zmsg.Pop ()) {
                    Response msg = Response.Parser.ParseFrom (stream);
                    dumpResponse (msg);
                }
            } else {
                if (zerr == ZError.EAGAIN) {
                    throw new IOException ("connection timed out");
                }
                throw new ZException (zerr);
            }
        }

        private void printStatus (string status)
        {
            if (!_opts ["--be-terse"].IsTrue) {
                Console.Error.WriteLine ("status: {0}", status);
            }
        }

        private void dumpResponse (Response msg)
        {
            var status = "<recv>";
            if (msg.Status.Code == StatusCode.Ok) {
                if (_opts ["--be-terse"].IsTrue) dumpTerseResponse (msg.Media);
                else printMessage (status, msg.Media);
            } else {
                printMessage (status, msg);
            }
        }

        private void dumpTerseResponse (MediaResponse msg)
        {
            Console.WriteLine ("---\ntitle: {0}\nquality:", msg.Title);
            foreach (var stream in msg.Stream) {
                var qty = stream.Quality;
                Console.WriteLine (
                    "  profile: {0}\n    width: {1}\n    height: {2}",
                    stream.Quality.Profile, stream.Quality.Width,
                    stream.Quality.Height);
            }
        }

        private string toJSON (IMessage msg)
        {
            var fmtn = (_opts ["--compact-json"].IsTrue)
                ? Formatting.None
                : Formatting.Indented;
            return JsonConvert.SerializeObject (msg, fmtn, _jsonSettings);
        }

        private void printMessage (string status, IMessage msg)
        {
            printStatus (status);
            // Google.Protobuf: IMessage.ToString() returns JSON.
            // We'll use Newtonsoft.Json instead, for:
            // - compact and human-readable presentation of JSON
            // - better control over the serialized fields
            Console.WriteLine (toJSON (msg));
            //
            // if (_opts["--output-json"].IsTrue) {
            //    Console.WriteLine(toJSON(msg));
            // } else {
            //    Console.WriteLine(msg.ToString());
            // }
        }

    } // class Jomiel

    internal class DynamicContractResolver : DefaultContractResolver {
        // Used to weed out "InquiryCase" and friends from the JSON.
        //

        private readonly List<string> fieldNames =
                                        new List<string> { "inquiryCase" };

        protected override IList<JsonProperty> CreateProperties (Type type,
                            MemberSerialization memberSerialization)
        {
            // Return all property names, except the ones we specify in
            // `fieldNames`.
            //
            var props = base.CreateProperties (type, memberSerialization);
            var result = new List<JsonProperty> ();

            foreach (var p in props) {
                if (!fieldNames.Contains (p.PropertyName)) {
                    result.Add (p);
                }
            }
            return result;
        }

        protected override string ResolvePropertyName (string key)
        {
            // Convert field names to use camelCase.
            //
            return Char.ToLowerInvariant (key [0]) + key.Substring (1);
        }

    } // class DynamicContractResolver

} // namespace Demo
