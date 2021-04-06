# frozen_string_literal: true

#
# jomiel-examples
#
# Copyright
#  2021 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#
require 'docopt'

require 'jomiel'
require 'usage'

module Runner
  module_function

  def main
    opts = Docopt.docopt(Usage::USAGE)
    if opts['--print-config']
      Runner.dump_config(opts)
    elsif opts['--version-zmq']
      Runner.print_zmq_version
    else
      Jomiel::Jomiel.new(opts).inquire
    end
  rescue Docopt::Exit => e
    warn e.message
  end

  def dump_config(opts)
    puts '---'
    opts.each { |key, value| puts "#{key.sub('--', '')}: #{value}" }
  end

  def print_zmq_version
    require 'ffi-rzmq'
    puts "ZeroMQ version #{LibZMQ.version.values.join('.')}"
  end
end
