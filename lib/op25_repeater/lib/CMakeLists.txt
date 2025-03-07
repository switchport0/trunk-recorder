# Copyright 2011,2012 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
cmake_minimum_required(VERSION 3.2)
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND op25_repeater_sources
    ambe_encoder_sb_impl.cc
    dmr_bs_tx_bb_impl.cc
    ysf_tx_sb_impl.cc
    dstar_tx_sb_impl.cc
    vocoder_impl.cc
    gardner_costas_cc_impl.cc
    p25_frame_assembler_impl.cc
    fsk4_demod_ff_impl.cc
    fsk4_slicer_fb_impl.cc )

list(APPEND op25_repeater_sources
    bch.cc
    rs.cc
    p25_framer.cc
    p25p1_fdma.cc
    p25p1_voice_encode.cc
    p25p1_voice_decode.cc
    p25p2_framer.cc
    p25p2_isch.cc
    p25p2_duid.cc
    p25p2_sync.cc
    p25p2_tdma.cc
    p25p2_vf.cc
    imbe_decoder.cc
    software_imbe_decoder.cc
    ambe.c
    mbelib.c
    ambe_encoder.cc
    rx_sync.cc
    hamming.cc
    golay2087.cc
    bptc19696.cc
    trellis.cc
    dmr_cai.cc
    dmr_slot.cc
    op25_audio.cc
    op25_timer.cc
    CCITTChecksumReverse.cpp
)

list(APPEND op25_repeater_sources
    imbe_vocoder/aux_sub.cc
    imbe_vocoder/basicop2.cc
    imbe_vocoder/ch_decode.cc
    imbe_vocoder/ch_encode.cc
    imbe_vocoder/dc_rmv.cc
    imbe_vocoder/decode.cc
    imbe_vocoder/dsp_sub.cc
    imbe_vocoder/encode.cc
    imbe_vocoder/imbe_vocoder.cc
    imbe_vocoder/math_sub.cc
    imbe_vocoder/pe_lpf.cc
    imbe_vocoder/pitch_est.cc
    imbe_vocoder/pitch_ref.cc
    imbe_vocoder/qnt_sub.cc
    imbe_vocoder/rand_gen.cc
    imbe_vocoder/sa_decode.cc
    imbe_vocoder/sa_encode.cc
    imbe_vocoder/sa_enh.cc
    imbe_vocoder/tbls.cc
    imbe_vocoder/uv_synt.cc
    imbe_vocoder/v_synt.cc
    imbe_vocoder/v_uv_det.cc
)

add_library(gnuradio-op25_repeater SHARED ${op25_repeater_sources})
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    find_library(GR_FILTER_LIBRARY libgnuradio-filter.dylib )
    find_library(GR_PMT_LIBRARY libgnuradio-pmt.dylib )
else(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    find_library(GR_FILTER_LIBRARY libgnuradio-filter.so )
endif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
set(GR_FILTER_LIBRARIES ${GR_FILTER_LIBRARY})
set(GR_PMT_LIBRARIES ${GR_PMT_LIBRARY})

target_link_libraries(gnuradio-op25_repeater ${Boost_LIBRARIES} ${GNURADIO_RUNTIME_LIBRARIES} ${GR_PMT_LIBRARIES} ${GR_FILTER_LIBRARIES} )
if(NOT Gnuradio_VERSION VERSION_LESS "3.8")
    target_link_libraries(gnuradio-op25_repeater
    gnuradio::gnuradio-runtime
    gnuradio::gnuradio-filter
    gnuradio::gnuradio-pmt
    )  
endif()

set_target_properties(gnuradio-op25_repeater PROPERTIES DEFINE_SYMBOL "gnuradio_op25_repeater_EXPORTS")

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-op25_repeater LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/trunk-recorder)



