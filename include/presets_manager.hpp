#ifndef PRESETS_MANAGER_HPP
#define PRESETS_MANAGER_HPP

#include <giomm/settings.h>
#include <boost/filesystem.hpp>
#include <memory>
#include <vector>
#include "autogain_preset.hpp"
#include "bass_enhancer_preset.hpp"
#include "compressor_preset.hpp"
#include "convolver_preset.hpp"
#include "crossfeed_preset.hpp"
#include "crystalizer_preset.hpp"
#include "deesser_preset.hpp"
#include "equalizer_preset.hpp"
#include "exciter_preset.hpp"
#include "filter_preset.hpp"
#include "gate_preset.hpp"
#include "limiter_preset.hpp"
#include "loudness_preset.hpp"
#include "maximizer_preset.hpp"
#include "multiband_compressor_preset.hpp"
#include "multiband_gate_preset.hpp"
#include "pitch_preset.hpp"
#include "reverb_preset.hpp"
#include "stereo_tools_preset.hpp"
#include "webrtc_preset.hpp"

class PresetsManager {
 public:
  PresetsManager();
  virtual ~PresetsManager();

  std::vector<std::string> get_names();
  void add(const std::string& name);
  void save(const std::string& name);
  void remove(const std::string& name);
  void load(const std::string& name);
  void import(const std::string& file_path);

 private:
  std::string log_tag = "presets_manager: ";

  boost::filesystem::path presets_dir;

  Glib::RefPtr<Gio::Settings> settings, sie_settings, soe_settings;

  std::unique_ptr<LimiterPreset> limiter;
  std::unique_ptr<BassEnhancerPreset> bass_enhancer;
  std::unique_ptr<CompressorPreset> compressor;
  std::unique_ptr<CrossfeedPreset> crossfeed;
  std::unique_ptr<DeesserPreset> deesser;
  std::unique_ptr<EqualizerPreset> equalizer;
  std::unique_ptr<ExciterPreset> exciter;
  std::unique_ptr<FilterPreset> filter;
  std::unique_ptr<GatePreset> gate;
  std::unique_ptr<MaximizerPreset> maximizer;
  std::unique_ptr<PitchPreset> pitch;
  std::unique_ptr<ReverbPreset> reverb;
  std::unique_ptr<WebrtcPreset> webrtc;
  std::unique_ptr<MultibandCompressorPreset> multiband_compressor;
  std::unique_ptr<LoudnessPreset> loudness;
  std::unique_ptr<MultibandGatePreset> multiband_gate;
  std::unique_ptr<StereoToolsPreset> stereo_tools;
  std::unique_ptr<ConvolverPreset> convolver;
  std::unique_ptr<CrystalizerPreset> crystalizer;
  std::unique_ptr<AutoGainPreset> autogain;

  template <typename T>
  T get_default(const Glib::RefPtr<Gio::Settings>& settings,
                const std::string& key) {
    Glib::Variant<T> value;

    settings->get_default_value(key, value);

    return value.get();
  }

  template <typename T>
  void update_key(boost::property_tree::ptree& root,
                  const Glib::RefPtr<Gio::Settings>& settings,
                  const std::string& key,
                  const std::string& json_key) {
    Glib::Variant<T> aux;

    settings->get_value(key, aux);

    T current_value = aux.get();

    T new_value = root.get<T>(json_key, get_default<T>(settings, key));

    if (is_different(current_value, new_value)) {
      auto v = Glib::Variant<T>::create(new_value);

      settings->set_value(key, v);
    }
  }

  void update_string_key(boost::property_tree::ptree& root,
                         const Glib::RefPtr<Gio::Settings>& settings,
                         const std::string& key,
                         const std::string& json_key) {
    std::string current_value = settings->get_string(key);

    std::string new_value = root.get<std::string>(
        json_key, get_default<std::string>(settings, key));

    if (current_value != new_value) {
      settings->set_string(key, new_value);
    }
  }

  template <typename T>
  bool is_different(const T& a, const T& b) {
    return a != b;
  }

  void save_general_settings(boost::property_tree::ptree& root);

  void load_general_settings(boost::property_tree::ptree& root);
};

#endif
