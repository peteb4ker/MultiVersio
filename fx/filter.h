#pragma once

#include "libDaisy/src/daisy_versio.h"

#include "IEffect.h"
#include "core/IMultiVersioCommon.h"

/**
 * @brief Implements a filter effect.
 */
class Filter : public IEffect
{
  public:
    Filter(IMultiVersioCommon &mv);

    float filter_mode_l;
    float filter_mode_r;
    float filter_path;
    float filter_target_l_freq;
    float filter_target_r_freq;
    float filter_current_l_freq;
    float filter_current_r_freq;

    void processSample(float &outl, float &outr, float inl, float inr);
    void postProcess(float       outl[],
                     float       outr[],
                     const float inl[],
                     const float inr[],
                     size_t      size);
    void run(float blend,
             float regen,
             float tone,
             float speed,
             float size,
             float index,
             float dense,
             bool  gate);

  private:
    daisy::Parameter filter_cutoff_l_par;
    daisy::Parameter filter_cutoff_r_par;

    IMultiVersioCommon &mv;
};
