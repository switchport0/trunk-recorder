// P25 TDMA Decoder (C) Copyright 2013, 2014 Max H. Parke KA1RBI
// 
// This file is part of OP25
// 
// OP25 is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3, or (at your option)
// any later version.
// 
// OP25 is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
// License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with OP25; see the file COPYING. If not, write to the Free
// Software Foundation, Inc., 51 Franklin Street, Boston, MA
// 02110-1301, USA.

#ifndef INCLUDED_P25P2_TDMA_H
#define INCLUDED_P25P2_TDMA_H

#include <stdint.h>
#include <deque>
#include <vector>
#include <gnuradio/msg_queue.h>
#include "mbelib.h"
#include "imbe_decoder.h"
#include "software_imbe_decoder.h"
#include "p25p2_duid.h"
#include "p25p2_sync.h"
#include "p25p2_vf.h"
#include "p25p2_framer.h"
#include "op25_audio.h"
#include "log_ts.h"

#include "ezpwd/rs"

//class p25p2_tdma;
class p25p2_tdma
{
public:
	p25p2_tdma(const op25_audio& udp, int slotid, int debug, bool do_msgq, gr::msg_queue::sptr queue, std::deque<int16_t> &qptr, bool do_audio_output, bool do_nocrypt) ;	// constructor
	int handle_packet(const uint8_t dibits[]) ;
	void set_slotid(int slotid);
	uint8_t* tdma_xormask;
	uint32_t symbols_received;
	uint32_t packets;
	~p25p2_tdma();	// destructor
	void set_xormask(const char*p);
	bool rx_sym(uint8_t sym);
	int handle_frame(void) ;
  	bool get_call_terminated();
private:
	p25p2_sync sync;
	p25p2_duid duid;
	p25p2_vf vf;
	int write_bufp;
	char write_buf[512];
	int d_slotid;
	mbe_parms cur_mp;
	mbe_parms prev_mp;
	mbe_parms enh_mp;
	mbe_tone tone_mp;
	int mbe_err_cnt;
	bool tone_frame;
	software_imbe_decoder software_decoder;
	gr::msg_queue::sptr d_msg_queue;
	std::deque<int16_t> &output_queue_decode;
	bool d_do_msgq;
	bool d_do_audio_output;
        bool d_do_nocrypt;
		bool terminate_call;
        const op25_audio& op25audio;
	log_ts logts;

	int d_debug;
	unsigned long int crc_errors;

        int burst_id;
        inline int track_vb(int burst_type) { return burst_id = (burst_type == 0) ? (++burst_id % 5) : 4; }
        inline void reset_vb(void) { burst_id = -1; }

        ezpwd::RS<63,35> rs28;      // Reed-Solomon decoder object
        std::vector<uint8_t> ESS_A; // ESS_A and ESS_B are hexbits vectors
        std::vector<uint8_t> ESS_B;

        uint16_t ess_keyid;
        uint8_t ess_algid;
	uint8_t ess_mi[9] = {0};

	p25p2_framer p2framer;

	int handle_acch_frame(const uint8_t dibits[], bool fast) ;
	void handle_voice_frame(const uint8_t dibits[]) ;
	int process_mac_pdu(const uint8_t byte_buf[], const unsigned int len, const int rs_errs) ;
        void handle_mac_ptt(const uint8_t byte_buf[], const unsigned int len, const int rs_errs) ;
        void handle_mac_end_ptt(const uint8_t byte_buf[], const unsigned int len, const int rs_errs) ;
        void handle_mac_idle(const uint8_t byte_buf[], const unsigned int len, const int rs_errs) ;
        void handle_mac_active(const uint8_t byte_buf[], const unsigned int len, const int rs_errs) ;
        void handle_mac_hangtime(const uint8_t byte_buf[], const unsigned int len, const int rs_errs) ;
        void decode_mac_msg(const uint8_t byte_buf[], const unsigned int len) ;
        void handle_4V2V_ess(const uint8_t dibits[]);
        inline bool encrypted() { return (ess_algid != 0x80); }

	void send_msg(const std::string msg_str, long msg_type);
};
#endif /* INCLUDED_P25P2_TDMA_H */
