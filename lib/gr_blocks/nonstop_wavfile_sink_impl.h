/* -*- c++ -*- */
/*
 * Copyright 2008,2009,2013 Free Software Foundation, Inc.
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

#ifndef INCLUDED_GR_nonstop_wavfile_SINK_IMPL_H
#define INCLUDED_GR_nonstop_wavfile_SINK_IMPL_H

#include "nonstop_wavfile_sink.h"

#include <sys/time.h>
//#include "wavfile.h"
#include <boost/log/trivial.hpp>
#include <gr_blocks/wavfile_gr3.8.h>

namespace gr {
namespace blocks {

class nonstop_wavfile_sink_impl : public nonstop_wavfile_sink
{
private:

	unsigned d_sample_rate;
	int d_nchans;		
	int d_max_sample_val;
	int d_min_sample_val;
	int d_normalize_shift;
	int d_normalize_fac;
	bool d_use_float;
	bool d_conventional;
	bool d_first_work;
	bool d_termination_flag;
	time_t d_start_time;
	time_t d_stop_time;
	long curr_src_id;
	char current_filename[255];
	char current_base_filename[255];
	Call* d_current_call;
	long d_current_call_num;
	long d_current_call_recorder_num;
	std::string d_current_call_short_name;
	std::string d_current_call_capture_dir;
	double d_current_call_freq;
	double d_prior_transmission_length;
	long d_current_call_talkgroup;
	bool record_more_transmissions;
protected:
	
	unsigned d_sample_count;
	int d_bytes_per_sample;
	FILE *d_fp;
	boost::mutex d_mutex;
	virtual int dowork(int noutput_items,  gr_vector_const_void_star& input_items,  gr_vector_void_star& output_items);
	/*!
	 * \brief Convert a sample value within [-1;+1] to a corresponding
	 *  short integer value
	 */
	short convert_to_short(float sample);

	/*!
	 * \brief If any file changes have occurred, update now. This is called
	 * internally by work() and thus doesn't usually need to be called by
	 * hand.
	 */
	void do_update();

	/*!
	 * \brief Writes information to the WAV header which is not available
	 * a-priori (chunk size etc.) and closes the file. Not thread-safe and
	 * assumes d_fp is a valid file pointer, should thus only be called by
	 * other methods.
	 */
	void close_wav(bool close_call);
protected:
	bool stop();
	bool open_internal(const char *filename);
	std::vector<Transmission> transmission_list;
	State state;
public:

	
	#if GNURADIO_VERSION < 0x030900
	typedef boost::shared_ptr<nonstop_wavfile_sink_impl> sptr;
	#else
	typedef std::shared_ptr<nonstop_wavfile_sink_impl> sptr;
	#endif


	/*
	 * \param filename The .wav file to be opened
	 * \param n_channels Number of channels (2 = stereo or I/Q output)
	 * \param sample_rate Sample rate [S/s]
	 * \param bits_per_sample 16 or 8 bit, default is 16
	 */
	static sptr make(int n_channels,
	                 unsigned int sample_rate,
	                 int bits_per_sample = 16,
								 		bool use_float=true);

	nonstop_wavfile_sink_impl(int n_channels,
	                          unsigned int sample_rate,
	                          int bits_per_sample,
													bool use_float);
	virtual ~nonstop_wavfile_sink_impl();
	void create_base_filename();
	char *get_filename();
	bool start_recording(Call *call);
	void stop_recording();
	void end_transmission();
	void set_source(long src);
	void set_sample_rate(unsigned int sample_rate);
	void set_bits_per_sample(int bits_per_sample);
	void set_record_more_transmissions(bool more);
	void clear_transmission_list();
	std::vector<Transmission> get_transmission_list();
	void add_transmission(Transmission t);
	int bits_per_sample();
	unsigned int sample_rate();
	double total_length_in_seconds();
	double length_in_seconds();
	Call_Source * get_source_list();
	int get_source_count();
	virtual int work(int noutput_items,
	gr_vector_const_void_star &input_items,
	gr_vector_void_star &output_items);
	State get_state();
	time_t get_start_time();
	time_t get_stop_time();
};

} /* namespace blocks */
} /* namespace gr */

#endif /* INCLUDED_GR_nonstop_wavfile_SINK_IMPL_H */
