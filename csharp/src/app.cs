/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

using System;

namespace Demo {

    internal class Demo {

        internal static int Main (string [] args)
        {
            var exit_status = 1;
            try {
                new Runner (args).main ();
                exit_status = 0;
            } catch (Exception exc) {
                // Print full stack:
                Console.Error.WriteLine ("error: {0}", exc.ToString ());
                // Print the last exception message:
                // Console.Error.WriteLine("error: {0}", exc.Message);
                exit_status = 1;
            }
            return exit_status;
        }

    } // class Demo

} // namespace Demo
