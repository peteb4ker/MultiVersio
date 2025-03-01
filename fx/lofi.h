#pragma once

#include "stmlib/fft/shy_fft.h"

#include "libDaisy/src/daisy_versio.h"

#include "IEffect.h"
#include "core/IMultiVersioCommon.h"
#include "core/helpers.h"
#include "filter.h"

/**
 * @brief Implements a lofi effect.
 */
class Lofi : public IEffect
{
  public:
    Lofi(IMultiVersioCommon &mv);

    void processSample(float &outl, float &outr, float inl, float inr);
    void run(float blend,
             float regen,
             float tone,
             float speed,
             float size,
             float index,
             float dense,
             bool  gate);
    bool usesReverb();

  private:
    float lofi_current_RMS, lofi_target_RMS;
    float lofi_damp_speed, lofi_depth;
    int   lofi_mod, lofi_rate_count;
    int   lofi_rmsCount;
    float lofi_cutoff, lofi_target_Lofi_LFO_Freq, lofi_current_Lofi_LFO_Freq;
    float lofi_previous_variable_compressor;
    float global_sample_rate;
    float lofi_previous_left_saturation, lofi_previous_right_saturation;
    float lofi_current_left_saturation, lofi_current_right_saturation;
    float lofi_drywet;
    float lofi_lpg_amount;
    float lofi_lpg_decay;

    IMultiVersioCommon &mv;

    Averager lofi_averager;

    daisy::Parameter lofi_tone_par, lofi_rate_par;
    daisy::Parameter lofi_reverb_tone_par, lofi_reverb_rate_par;
};
