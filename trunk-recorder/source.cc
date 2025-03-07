#include "source.h"
#include "formatter.h"

static int src_counter = 0;

void Source::set_antenna(std::string ant) {
  antenna = ant;

  if (driver == "osmosdr") {
    cast_to_osmo_sptr(source_block)->set_antenna(antenna, 0);
    BOOST_LOG_TRIVIAL(info) << "Setting antenna to [" << cast_to_osmo_sptr(source_block)->get_antenna() << "]";
  }

  if (driver == "usrp") {
    BOOST_LOG_TRIVIAL(info) << "Setting antenna to [" << antenna << "]";
    cast_to_usrp_sptr(source_block)->set_antenna(antenna, 0);
  }
}

std::string Source::get_antenna() {
  return antenna;
}

void Source::set_silence_frames(int m) {
  silence_frames = m;
}

int Source::get_silence_frames() {
  return silence_frames;
}

double Source::get_min_hz() {
  return min_hz;
}

double Source::get_max_hz() {
  return max_hz;
}

double Source::get_center() {
  return center;
}

double Source::get_rate() {
  return rate;
}

std::string Source::get_driver() {
  return driver;
}

std::string Source::get_device() {
  return device;
}

void Source::set_error(double e) {
  error = e;
}

double Source::get_error() {
  return error;
}

void Source::set_mix_gain(int b) {
  if (driver == "osmosdr") {
    mix_gain = b;
    cast_to_osmo_sptr(source_block)->set_gain(mix_gain, "MIX", 0);
    BOOST_LOG_TRIVIAL(info) << "MIX Gain set to: " << cast_to_osmo_sptr(source_block)->get_gain("MIX");
  }
}

int Source::get_mix_gain() {
  if (driver == "osmosdr") {
    try {
      mix_gain = cast_to_osmo_sptr(source_block)->get_gain("MIX", 0);
    } catch (std::exception &e) {
      BOOST_LOG_TRIVIAL(error) << "MIX Gain unsupported or other error: " << e.what();
    }
  }
  return mix_gain;
}

void Source::set_lna_gain(int b) {
  if (driver == "osmosdr") {
    lna_gain = b;
    cast_to_osmo_sptr(source_block)->set_gain(lna_gain, "LNA", 0);
    BOOST_LOG_TRIVIAL(info) << "LNA Gain set to: " << cast_to_osmo_sptr(source_block)->get_gain("LNA");
  }
}

int Source::get_lna_gain() {
  if (driver == "osmosdr") {
    try {
      lna_gain = cast_to_osmo_sptr(source_block)->get_gain("LNA", 0);
    } catch (std::exception &e) {
      BOOST_LOG_TRIVIAL(error) << "LNA Gain unsupported or other error: " << e.what();
    }
  }
  return lna_gain;
}

void Source::set_tia_gain(int b) {
  if (driver == "osmosdr") {
    tia_gain = b;
    cast_to_osmo_sptr(source_block)->set_gain(tia_gain, "TIA", 0);
    BOOST_LOG_TRIVIAL(info) << "TIA Gain set to: " << cast_to_osmo_sptr(source_block)->get_gain("TIA");
  }
}

int Source::get_tia_gain() {
  if (driver == "osmosdr") {
    try {
      tia_gain = cast_to_osmo_sptr(source_block)->get_gain("TIA", 0);
    } catch (std::exception &e) {
      BOOST_LOG_TRIVIAL(error) << "TIA Gain unsupported or other error: " << e.what();
    }
  }
  return tia_gain;
}

void Source::set_pga_gain(int b) {
  if (driver == "osmosdr") {
    pga_gain = b;
    cast_to_osmo_sptr(source_block)->set_gain(pga_gain, "PGA", 0);
    BOOST_LOG_TRIVIAL(info) << "PGA Gain set to: " << cast_to_osmo_sptr(source_block)->get_gain("PGA");
  }
}

int Source::get_pga_gain() {
  if (driver == "osmosdr") {
    try {
      pga_gain = cast_to_osmo_sptr(source_block)->get_gain("PGA", 0);
    } catch (std::exception &e) {
      BOOST_LOG_TRIVIAL(error) << "PGA Gain unsupported or other error: " << e.what();
    }
  }
  return pga_gain;
}

void Source::set_vga1_gain(int b) {
  if (driver == "osmosdr") {
    vga1_gain = b;
    cast_to_osmo_sptr(source_block)->set_gain(vga1_gain, "VGA1", 0);
    BOOST_LOG_TRIVIAL(info) << "VGA1 Gain set to: " << cast_to_osmo_sptr(source_block)->get_gain("VGA1");
  }
}

int Source::get_vga1_gain() {
  if (driver == "osmosdr") {
    try {
      vga1_gain = cast_to_osmo_sptr(source_block)->get_gain("VGA1", 0);
    } catch (std::exception &e) {
      BOOST_LOG_TRIVIAL(error) << "VGA1 Gain unsupported or other error: " << e.what();
    }
  }
  return vga1_gain;
}

void Source::set_vga2_gain(int b) {
  if (driver == "osmosdr") {
    vga2_gain = b;
    cast_to_osmo_sptr(source_block)->set_gain(vga2_gain, "VGA2", 0);
    BOOST_LOG_TRIVIAL(info) << "VGA2 Gain set to: " << cast_to_osmo_sptr(source_block)->get_gain("VGA2");
  }
}

int Source::get_vga2_gain() {
  if (driver == "osmosdr") {
    try {
      vga2_gain = cast_to_osmo_sptr(source_block)->get_gain("VGA2", 0);
    } catch (std::exception &e) {
      BOOST_LOG_TRIVIAL(error) << "VGA2 Gain unsupported or other error: " << e.what();
    }
  }
  return vga2_gain;
}

void Source::set_bb_gain(int b) {
  if (driver == "osmosdr") {
    bb_gain = b;
    cast_to_osmo_sptr(source_block)->set_bb_gain(bb_gain);
    BOOST_LOG_TRIVIAL(info) << "BB Gain set to: " << cast_to_osmo_sptr(source_block)->get_gain("BB");
  }
}

int Source::get_bb_gain() {
  return bb_gain;
}

void Source::set_gain(int r) {
  if (driver == "osmosdr") {
    gain = r;
    cast_to_osmo_sptr(source_block)->set_gain(gain);
    BOOST_LOG_TRIVIAL(info) << "Gain set to: " << cast_to_osmo_sptr(source_block)->get_gain();
  }

  if (driver == "usrp") {
    gain = r;
    cast_to_usrp_sptr(source_block)->set_gain(gain);
  }
}

int Source::get_gain() {
  return gain;
}

void Source::set_gain_mode(bool m) {
  if (driver == "osmosdr") {
    gain_mode = m;
    cast_to_osmo_sptr(source_block)->set_gain_mode(gain_mode);
    if (cast_to_osmo_sptr(source_block)->get_gain_mode()) {
      BOOST_LOG_TRIVIAL(info) << "Auto gain control is ON";
    } else {
      BOOST_LOG_TRIVIAL(info) << "Auto gain control is OFF";
    }
  }
}

bool Source::get_gain_mode() {
  return gain_mode;
}

void Source::set_if_gain(int i) {
  if (driver == "osmosdr") {
    if_gain = i;
    cast_to_osmo_sptr(source_block)->set_if_gain(if_gain);
    BOOST_LOG_TRIVIAL(info) << "IF Gain set to: " << cast_to_osmo_sptr(source_block)->get_gain("IF");
  }
}

void Source::set_freq_corr(double p) {
  ppm = p;

  if (driver == "osmosdr") {
    cast_to_osmo_sptr(source_block)->set_freq_corr(ppm);
    BOOST_LOG_TRIVIAL(info) << "PPM set to: " << cast_to_osmo_sptr(source_block)->get_freq_corr();
  }
}

int Source::get_if_gain() {
  return if_gain;
}

analog_recorder_sptr Source::create_conventional_recorder(gr::top_block_sptr tb) {

  analog_recorder_sptr log = make_analog_recorder(this);

  analog_recorders.push_back(log);
  tb->connect(source_block, 0, log, 0);
  return log;
}

void Source::create_analog_recorders(gr::top_block_sptr tb, int r) {
  max_analog_recorders = r;

  for (int i = 0; i < max_analog_recorders; i++) {
    analog_recorder_sptr log = make_analog_recorder(this);
    analog_recorders.push_back(log);
    tb->connect(source_block, 0, log, 0);
  }
}

Recorder *Source::get_analog_recorder(Talkgroup *talkgroup) {
  int num_available_recorders = get_num_available_analog_recorders();

  if (talkgroup && talkgroup->get_priority() > num_available_recorders) { // a low priority is bad. You need atleast the number of availalbe recorders to your priority
    BOOST_LOG_TRIVIAL(info) << "\t\tNot recording talkgroup " << talkgroup->number << " (" << talkgroup->alpha_tag << ")" << ", priority is " <<
      talkgroup->get_priority() << " but only " << num_available_recorders << " recorders available";
    return NULL;
  }

  return get_analog_recorder();
}

Recorder *Source::get_analog_recorder() {
  for (std::vector<analog_recorder_sptr>::iterator it = analog_recorders.begin();
       it != analog_recorders.end(); it++) {
    analog_recorder_sptr rx = *it;

    if (rx->get_state() == AVAILABLE) {
      return (Recorder *)rx.get();

      break;
    }
  }
  BOOST_LOG_TRIVIAL(info) << "[ " << device << " ] No Analog Recorders Available";
  return NULL;
}

void Source::create_digital_recorders(gr::top_block_sptr tb, int r) {
  max_digital_recorders = r;

  for (int i = 0; i < max_digital_recorders; i++) {
    p25_recorder_sptr log = make_p25_recorder(this);
    digital_recorders.push_back(log);
    tb->connect(source_block, 0, log, 0);
  }
}

p25_recorder_sptr Source::create_digital_conventional_recorder(gr::top_block_sptr tb) {
  // Not adding it to the vector of digital_recorders. We don't want it to be available for trunk recording.
  p25_recorder_sptr log = make_p25_recorder(this);
  tb->connect(source_block, 0, log, 0);
  return log;
}

void Source::create_debug_recorder(gr::top_block_sptr tb, int source_num) {
  max_debug_recorders = 1;
  debug_recorder_port = config->debug_recorder_port + source_num;
  debug_recorder_sptr log = make_debug_recorder(this, config->debug_recorder_address, debug_recorder_port);
  debug_recorders.push_back(log);
  tb->connect(source_block, 0, log, 0);
}

Recorder *Source::get_debug_recorder() {
  for (std::vector<debug_recorder_sptr>::iterator it = debug_recorders.begin();
       it != debug_recorders.end(); it++) {
    debug_recorder_sptr rx = *it;

    if (rx->get_state() == INACTIVE) {
      return (Recorder *)rx.get();

      break;
    }
  }
  return NULL;
}

int Source::get_debug_recorder_port() {
  return debug_recorder_port;
}

void Source::create_sigmf_recorders(gr::top_block_sptr tb, int r) {
  max_sigmf_recorders = r;

  for (int i = 0; i < max_sigmf_recorders; i++) {
    sigmf_recorder_sptr log = make_sigmf_recorder(this);

    sigmf_recorders.push_back(log);
    tb->connect(source_block, 0, log, 0);
  }
}

Recorder *Source::get_sigmf_recorder() {
  for (std::vector<sigmf_recorder_sptr>::iterator it = sigmf_recorders.begin();
       it != sigmf_recorders.end(); it++) {
    sigmf_recorder_sptr rx = *it;

    if (rx->get_state() == INACTIVE) {
      return (Recorder *)rx.get();

      break;
    }
  }
  return NULL;
}

void Source::print_recorders() {
  BOOST_LOG_TRIVIAL(info) << "[ " << device << " ]  ";

  for (std::vector<p25_recorder_sptr>::iterator it = digital_recorders.begin();
       it != digital_recorders.end(); it++) {
    p25_recorder_sptr rx = *it;

    BOOST_LOG_TRIVIAL(info) << "[ D" << rx->get_num() << " ] State: " << format_state(rx->get_state());
  }

  for (std::vector<analog_recorder_sptr>::iterator it = analog_recorders.begin();
       it != analog_recorders.end(); it++) {
    analog_recorder_sptr rx = *it;

    BOOST_LOG_TRIVIAL(info) << "[ A" << rx->get_num() << " ] State: " << format_state(rx->get_state());
  }
}

void Source::tune_digital_recorders() {
  for (std::vector<p25_recorder_sptr>::iterator it = digital_recorders.begin(); it != digital_recorders.end(); it++) {
    p25_recorder_sptr rx = *it;

    if (rx->get_state() == ACTIVE) {
      rx->autotune();
    }
  }
}

int Source::digital_recorder_count() {
  return digital_recorders.size();
}

int Source::analog_recorder_count() {
  return analog_recorders.size();
}

int Source::debug_recorder_count() {
  return debug_recorders.size();
}

int Source::sigmf_recorder_count() {
  return sigmf_recorders.size();
}

int Source::get_num() {
  return src_num;
};

int Source::get_num_available_digital_recorders() {
  int num_available_recorders = 0;

  for (std::vector<p25_recorder_sptr>::iterator it = digital_recorders.begin();
       it != digital_recorders.end(); it++) {
    p25_recorder_sptr rx = *it;

    if (rx->get_state() == AVAILABLE) {
      num_available_recorders++;
    }
  }
  return num_available_recorders;
}

int Source::get_num_available_analog_recorders() {
  int num_available_recorders = 0;

  for (std::vector<analog_recorder_sptr>::iterator it = analog_recorders.begin(); it != analog_recorders.end(); it++) {
    analog_recorder_sptr rx = *it;

    if (rx->get_state() == AVAILABLE) {
      num_available_recorders++;
    }
  }
  return num_available_recorders;
}

Recorder *Source::get_digital_recorder(Talkgroup *talkgroup) {
  int num_available_recorders = get_num_available_digital_recorders();

  if (talkgroup && talkgroup->get_priority() > num_available_recorders) { // a low priority is bad. You need atleast the number of availalbe recorders to your priority
    BOOST_LOG_TRIVIAL(info) << "\t\tNot recording talkgroup " << talkgroup->number << " (" << talkgroup->alpha_tag << ")" << ", priority is " <<
      talkgroup->get_priority() << " but only " << num_available_recorders << " recorders available";
    return NULL;
  }

  return get_digital_recorder();
}

Recorder *Source::get_digital_recorder() {
  for (std::vector<p25_recorder_sptr>::iterator it = digital_recorders.begin();
       it != digital_recorders.end(); it++) {
    p25_recorder_sptr rx = *it;

    if (rx->get_state() == AVAILABLE) {
      return (Recorder *)rx.get();

      break;
    }
  }
  BOOST_LOG_TRIVIAL(info) << "[ " << device << " ] No Digital Recorders Available";

  for (std::vector<p25_recorder_sptr>::iterator it = digital_recorders.begin();
       it != digital_recorders.end(); it++) {
    p25_recorder_sptr rx = *it;
    BOOST_LOG_TRIVIAL(info) << "[ " << rx->get_num() << " ] State: " << format_state(rx->get_state()) << " Freq: " << rx->get_freq();
  }
  return NULL;
}

gr::basic_block_sptr Source::get_src_block() {
  return source_block;
}

Config *Source::get_config() {
  return config;
}

void Source::set_min_max() {
  long s = rate;
  long if_freqs[] = {24000, 25000, 32000};
  long decim = 24000;
  for (int i = 0; i < 3; i++) {
    long if_freq = if_freqs[i];
    if (s % if_freq != 0) {
      continue;
    }
    long q = s / if_freq;
    if (q & 1) {
      continue;
    }

    if ((q >= 40) && ((q & 3) == 0)) {
      decim = q / 4;
    } else {
      decim = q / 2;
    }
  }
  long if1 = rate / decim;
  min_hz = center - ((rate / 2) - (if1 / 2));
  max_hz = center + ((rate / 2) - (if1 / 2));
}

Source::Source(double c, double r, double e, std::string drv, std::string dev, Config *cfg) {
  rate = r;
  center = c;
  error = e;
  set_min_max();
  driver = drv;
  device = dev;
  config = cfg;
  gain = 0;
  lna_gain = 0;
  tia_gain = 0;
  pga_gain = 0;
  mix_gain = 0;
  if_gain = 0;
  src_num = src_counter++;
  max_digital_recorders = 0;
  max_debug_recorders = 0;
  max_sigmf_recorders = 0;
  max_analog_recorders = 0;
  debug_recorder_port = 0;

  if (driver == "osmosdr") {
    osmosdr::source::sptr osmo_src;
    std::vector<std::string> gain_names;
    if (dev == "") {
      BOOST_LOG_TRIVIAL(info) << "Source Device not specified";
      osmo_src = osmosdr::source::make();
    } else {
      std::ostringstream msg;

      if (isdigit(dev[0])) {  // Assume this is a serial number and fail back
                              // to using rtl as default
        msg << "rtl=" << dev; // <<  ",buflen=32764,buffers=8";
        BOOST_LOG_TRIVIAL(info) << "Source device name missing, defaulting to rtl device";
      } else {
        msg << dev; // << ",buflen=32764,buffers=8";
      }
      BOOST_LOG_TRIVIAL(info) << "Source Device: " << msg.str();
      osmo_src = osmosdr::source::make(msg.str());
    }
    BOOST_LOG_TRIVIAL(info) << "SOURCE TYPE OSMOSDR (osmosdr)";
    BOOST_LOG_TRIVIAL(info) << "Setting sample rate to: " << FormatSamplingRate(rate);
    osmo_src->set_sample_rate(rate);
    actual_rate = osmo_src->get_sample_rate();
    BOOST_LOG_TRIVIAL(info) << "Actual sample rate: " << FormatSamplingRate(actual_rate);
    BOOST_LOG_TRIVIAL(info) << "Tuning to " << format_freq(center + error);
    osmo_src->set_center_freq(center + error, 0);
    gain_names = osmo_src->get_gain_names();
    std::string gain_list;
    for (std::vector<std::string>::iterator it = gain_names.begin(); it != gain_names.end(); it++) {
      std::string gain_name = *it;
      osmosdr::gain_range_t range = osmo_src->get_gain_range(gain_name);
      std::vector<double> gains = range.values();
      std::string gain_opt_str;
      for (std::vector<double>::iterator gain_it = gains.begin(); gain_it != gains.end(); gain_it++) {
        double gain_opt = *gain_it;
        std::ostringstream ss;
        //gain_opt = floor(gain_opt * 10) / 10;
        ss << gain_opt << " ";

        gain_opt_str += ss.str();
      }
      BOOST_LOG_TRIVIAL(info) << "Gain Stage: " << gain_name << " supported values: " << gain_opt_str;
    }

    source_block = osmo_src;
  }

  if (driver == "usrp") {
    gr::uhd::usrp_source::sptr usrp_src;
    usrp_src = gr::uhd::usrp_source::make(device, uhd::stream_args_t("fc32"));

    BOOST_LOG_TRIVIAL(info) << "SOURCE TYPE USRP (UHD)";

    BOOST_LOG_TRIVIAL(info) << "Setting sample rate to: " << FormatSamplingRate(rate);
    usrp_src->set_samp_rate(rate);
    actual_rate = usrp_src->get_samp_rate();
    BOOST_LOG_TRIVIAL(info) << "Actual sample rate: " << FormatSamplingRate(actual_rate);
    BOOST_LOG_TRIVIAL(info) << "Tuning to " << format_freq(center + error);
    usrp_src->set_center_freq(center + error, 0);

    source_block = usrp_src;
  }
}

std::vector<Recorder *> Source::get_recorders() {

  std::vector<Recorder *> recorders;

  for (std::vector<p25_recorder_sptr>::iterator it = digital_recorders.begin(); it != digital_recorders.end(); it++) {
    p25_recorder_sptr rx = *it;
    recorders.push_back((Recorder *)rx.get());
  }

  for (std::vector<analog_recorder_sptr>::iterator it = analog_recorders.begin(); it != analog_recorders.end(); it++) {
    analog_recorder_sptr rx = *it;
    recorders.push_back((Recorder *)rx.get());
  }

  for (std::vector<debug_recorder_sptr>::iterator it = debug_recorders.begin(); it != debug_recorders.end(); it++) {
    debug_recorder_sptr rx = *it;
    recorders.push_back((Recorder *)rx.get());
  }

  for (std::vector<sigmf_recorder_sptr>::iterator it = sigmf_recorders.begin(); it != sigmf_recorders.end(); it++) {
    sigmf_recorder_sptr rx = *it;
    recorders.push_back((Recorder *)rx.get());
  }
  return recorders;
}
