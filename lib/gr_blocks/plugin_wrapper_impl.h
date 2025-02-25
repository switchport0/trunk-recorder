/* -*- c++ -*- */
/*
 * Copyright 2020 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_GR_PLUGIN_WRAPPER_IMPL_H
#define INCLUDED_GR_PLUGIN_WRAPPER_IMPL_H

#include "plugin_wrapper.h"
#include <boost/log/trivial.hpp>

namespace gr {
	namespace blocks {

		class plugin_wrapper_impl : public plugin_wrapper
		{
		private:
			plugin_callback d_callback;

        protected:

			boost::mutex d_mutex;
			virtual int dowork(int noutput_items, gr_vector_const_void_star& input_items, gr_vector_void_star& output_items);

		public:

			
	#if GNURADIO_VERSION < 0x030900
	typedef boost::shared_ptr <plugin_wrapper_impl> sptr;
	#else
	typedef std::shared_ptr <plugin_wrapper_impl> sptr;
	#endif


			static sptr make(plugin_callback callback);

			plugin_wrapper_impl(plugin_callback callback);
			
            virtual int work(int noutput_items,
				gr_vector_const_void_star& input_items,
				gr_vector_void_star& output_items);
		};

	} /* namespace blocks */
} /* namespace gr */

#endif /* INCLUDED_GR_PLUGIN_WRAPPER_IMPL_H */
