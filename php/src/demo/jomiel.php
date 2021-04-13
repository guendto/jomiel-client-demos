<?php
/* -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2019-2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

namespace Demo;

use \Exception;

use Docopt\Response as DocoptResponse;

use \ZMQPollException;
use \ZMQContext;
use \ZMQSocket;
use \ZMQPoll;
use \ZMQ;

use Jomiel\ProtoBuf\V1beta1\{
    MediaInquiry,
    Inquiry,
    Response,
    StatusCode
};

use function Demo\Util\pretty_dump;

class Jomiel
{
    private DocoptResponse $opts;
    private ZMQPoll $poller;
    private ZMQSocket $sck;

    function __construct(DocoptResponse $options)
    {
        $this->sck = new ZMQSocket(new ZMQContext(), ZMQ::SOCKET_REQ);
        $this->poller = new ZMQPoll();
        $this->opts = $options;

        $this->sck->setSockOpt(ZMQ::SOCKOPT_LINGER, 0);
        $this->poller->add($this->sck, ZMQ::POLL_IN);
    }

    function inquire(): void
    {
        $input = $this->opts["URI"];

        if (!empty($input)) {
            if ($this->opts["--output-json"]) {
                $this->printStatus(
                    "<note> redundant use of `--output-json`, output is always in JSON"
                );
            }

            $this->connect();

            foreach ($input as $uri) {
                $this->sendInquiry($uri);
                $this->receiveResponse();
            }
        } else {
            fwrite(STDERR, "error: no input URI given\n");
            die(1);
        }
    }

    private function connect(): void
    {
        $re = $this->opts["--router-endpoint"];
        $to = $this->opts["--connect-timeout"];
        $this->printStatus(
            sprintf("<connect> %s (timeout=%d)", $re, $to)
        );
        $this->sck->connect($re);
    }

    private function sendInquiry(string $uri): void
    {
        $msg = (new Inquiry())->setMedia(new MediaInquiry());
        $msg->getMedia()->setInputUri($uri);

        if (!$this->opts["--be-terse"]) {
            $this->printMessage("<send>", $msg);
        }

        $bytes = $msg->serializeToString();
        $this->sck->send($bytes);
    }

    private function receiveResponse(): void
    {
        $timeout = $this->opts["--connect-timeout"] * 1000; # to msec
        $writeable = [];
        $readable = [];
        $events = 0;

        try {
            $events = $this->poller->poll(
                $readable,
                $writeable,
                $timeout
            );
            $errors = $this->poller->getLastErrors();

            if (count($errors) > 0) {
                foreach ($errors as $e) {
                    fwrite(STDERR, "$e\n");
                }
                die(1);
            }
        } catch (ZMQPollException $e) {
            fwrite(STDERR, $e->getMessage() . "\n");
            die(1);
        }

        if ($events > 0) {
            foreach ($readable as $r) {
                try {
                    $bytes = $r->recv();
                    $msg = new Response();
                    $msg->mergeFromString($bytes);
                    $this->dumpResponse($msg);
                } catch (Exception $e) {
                    fwrite(STDERR, $e->getMessage() . "\n");
                    die(1);
                }
            }
        } else {
            fwrite(STDERR, "error: connection timed out\n");
            die(1);
        }
    }

    private function dumpResponse(Response $msg): void
    {
        $status = "<recv>";

        if (
            $msg->getStatus()->getCode() == StatusCode::STATUS_CODE_OK
        ) {
            if ($this->opts["--be-terse"]) {
                $this->dumpTerseResponse($msg->getMedia());
            } else {
                $this->printMessage($status, $msg->getMedia());
            }
        } else {
            $this->printMessage($status, $msg);
        }
    }

    private function dumpTerseResponse(
        \Jomiel\ProtoBuf\V1beta1\MediaResponse $msg
    ): void {
        echo sprintf("---\ntitle: %s\nquality\n", $msg->getTitle());
        foreach ($msg->getStream() as $stream) {
            $qty = $stream->getQuality();
            echo sprintf(
                "  profile: %s\n    width: %d\n    height: %d\n",
                $qty->getProfile(),
                $qty->getWidth(),
                $qty->getHeight()
            );
        }
    }

    private function printStatus(string $status): void
    {
        if (!$this->opts["--be-terse"]) {
            fwrite(STDERR, "$status\n");
        }
    }

    /**
     * @param Inquiry|Response|\Jomiel\ProtoBuf\V1beta1\MediaResponse $msg
     */
    private function printMessage(string $status, $msg): void
    {
        $this->printStatus($status);

        $result = $msg->serializeToJsonString();

        if (!$this->opts["--compact-json"]) {
            $json = json_decode($result);
            $result = pretty_dump($json);
        }
        echo "$result\n";
    }
}

?>
