<?php
/* -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2019 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */
class Jomiel {
    private $ctx, $sck;
    private $opts, $lg;

    function __construct($logger, $opts) {
        $this->lg = $logger;
        $this->opts = $opts;

        $this->ctx = new ZMQContext;
        $this->sck = new ZMQSocket($this->ctx, ZMQ::SOCKET_REQ);

        $this->sck->setSockOpt(ZMQ::SOCKOPT_LINGER, 0);
    }

    function __destruct() {
        $this->sck->disconnect($this->opts->routerEndpoint);
    }

    function connect() {
        $addr = $this->opts->routerEndpoint;
        $time = $this->opts->connectTimeout;
        $status = sprintf("<connect> %s (timeout=%d)", $addr, $time);
        $this->printStatus($status);
        $this->sck->connect($addr);
    }

    function inquire($uri) {
        $this->send($uri);
        $this->recv();
    }

    private function send($uri) {
        $media_inquiry = new Jomiel\Media\MediaInquiry;
        $inquiry = new Jomiel\Inquiry;

        $media_inquiry->setInputUri($uri);
        $inquiry->setMedia($media_inquiry);

        if (! $this->opts->beTerse) {
            $this->printMessage("<send>", $inquiry);
        }

        $bytes = $inquiry->serializeToString();
        $this->sck->send($bytes);
    }

    private function recv() {
        $poll = new ZMQPoll;
        $poll->add($this->sck, ZMQ::POLL_IN);

        $timeout = $this->opts->connectTimeout * 1000; # to msec

        $writeable = array();
        $readable = array();
        $events = 0;

        try {
            $events = $poll->poll($readable, $writeable, $timeout);
            $errors = $poll->getLastErrors();

            if (count($errors) >0) {
                foreach($errors as $error) {
                    $this->lg->error($error);
                }
                die(1);
            }
        } catch (ZMQPollException $e) {
            $this->lg->error($e->getMessage());
            die(1);
        }

        if ($events >0) {
            foreach ($readable as $r) {
                try {
                    $bytes = $this->sck->recv();
                    $response = new Jomiel\Response;
                    $response->mergeFromString($bytes);
                    $this->dumpResponse($response);
                } catch (Exception $e) {
                    $this->lg->error($e->getMessage());
                    die(1);
                }
            }
        } else {
            $this->lg->error("connection timed out");
            die(1);
        }
    }

    private function getQualityString($stream_quality) {
        return sprintf("  profile: %s\n    width: %d\n    height: %d",
                        $stream_quality->getProfile(),
                        $stream_quality->getWidth(),
                        $stream_quality->getHeight());
    }

    private function dumpTerseResponse($media_response) {
        echo "---\ntitle: " . $media_response->getTitle() ."\n";
        echo "quality:\n";
        foreach ($media_response->getStream() as $stream) {
            $stream_quality = $stream->getQuality();
            $quality_string = $this->getQualityString($stream_quality);
            echo "$quality_string\n";
        }
    }

    private function dumpResponse($response) {

        $status = $response->getStatus();

        if ($status->getCode() == Jomiel\Status\StatusCode::OK) {
            $media_response = $response->getMedia();
            if ($this->opts->beTerse) {
                $this->dumpTerseResponse($media_response);
            } else {
                $this->printMessage("<recv>", $media_response);
            }
        } else {
            $this->printMessage("<recv>", $response);
        }
    }

    private function printMessage($status, $message) {
        $this->printStatus($status);
        echo $message->serializeToJsonString() . "\n";
    }

    private function printStatus($status) {
        if (! $this->opts->beTerse) {
            $this->lg->info($status);
        }
    }
}
?>
