
#include "spectrings.h"

#include "libDaisy/src/daisy_versio.h"

#include "core/IMultiVersioCommon.h"
#include "core/helpers.h"
#include "core/leds.h"
#include "spectra.h"

/**
 * @brief Construct a new Spectrings::Spectrings object
 *
 * Initializes the string voices.
 *
 * @param mv A reference to the IMultiVersioCommon interface
 * @param spectra A reference to the Spectra object
 * @param sample_rate The sample rate of the audio
 */
Spectrings::Spectrings(IMultiVersioCommon &mv,
                       Spectra            &spectra,
                       int                 sample_rate)
: mv(mv), spectra(spectra)
{
    for(int i = 0; i < NUM_OF_STRINGS; i++)
    {
        string_voice[i].Init(sample_rate);
    }

    spectrings_pan_spread         = 0.0f;
    spectrings_num_models         = 2;
    spectrings_active_voices      = 2;
    spectrings_current_voice      = 0;
    spectrings_trigger_next_cycle = false;
    spectrings_drywet             = 0.0f;
}

/**
 * @brief Gets the next sample from the spectrings effect.
 *
 * @param outl The left output sample.
 * @param outr The right output sample.
 * @param inl The left input sample.
 * @param inr The right input sample.
 */
void Spectrings::processSample(float &outl, float &outr, float inl, float inr)
{
    spectra.spectra_oscbank.updateFreqAndMagn();
    float rings_1 = string_voice[0].Process()
                    * (spectrings_accent_amount[0]
                           * this->mv.attack_lut[spectrings_attack_step[0]]
                       + (1 - spectrings_accent_amount[0]));
    float rings_2 = string_voice[1].Process()
                    * (spectrings_accent_amount[1]
                           * this->mv.attack_lut[spectrings_attack_step[1]]
                       + (1 - spectrings_accent_amount[1]));
    // float rings_3 = string_voice[2].Process()* (spectrings_accent_amount[2]*spectrings_attack_lut[spectrings_attack_step[2]] + (1-spectrings_accent_amount[2]) );
    // float rings_4 = string_voice[3].Process()* (spectrings_accent_amount[3]*spectrings_attack_lut[spectrings_attack_step[3]] + (1-spectrings_accent_amount[3]) );

    float spectrings_outl = (rings_1 + rings_2 * spectrings_pan_spread)
                            * (0.7 + (1 - spectrings_pan_spread) * 0.3);
    float spectrings_outr = (rings_2 + rings_1 * spectrings_pan_spread)
                            * (0.7 + (1 - spectrings_pan_spread) * 0.3);

    spectrings_attack_step[0] = clamp(spectrings_attack_step[0] + 1, 0, 299);
    spectrings_attack_step[1] = clamp(spectrings_attack_step[1] + 1, 0, 299);
    // spectrings_attack_step[2] = clamp(spectrings_attack_step[2]+1, 0, 299);
    // spectrings_attack_step[3] = clamp(spectrings_attack_step[3]+1, 0, 299);

    if(spectrings_drywet > 0.98f)
    {
        spectrings_drywet = 1.f;
    }
    outl = (sqrt(0.5f * (spectrings_drywet * 2.0f)) * spectrings_outl
            + sqrt(0.95f * (2.f - (spectrings_drywet * 2))) * inl)
           * 0.5f;
    outr = (sqrt(0.5f * (spectrings_drywet * 2.0f)) * spectrings_outr
            + sqrt(0.95f * (2.f - (spectrings_drywet * 2))) * inr)
           * 0.5f;
}

/**
 * @brief Runs the spectrings effect.
 *
 * @param blend The dry/wet blend of the effect.
 * @param regen The brightness of the effect.
 * @param tone The octave of the effect.
 * @param speed The reverb of the effect.
 * @param size The damping of the effect.
 * @param index The transpose of the effect.
 * @param dense The inharmonicity of the effect.
 * @param gate Effect gate from the FSU input.
 */
void Spectrings::run(float blend,
                     float regen,
                     float tone,
                     float speed,
                     float size,
                     float index,
                     float dense,
                     bool  gate)
{
    // blend = dry/wet
    // speed =  reverb
    // index = transpose when quantizing
    // tone = octave
    // size = damping
    // regen = brightness
    // dense = inharmonicity
    // gate = effect gate from gate input

    // tap = activate quantizer

    spectra.spectra_transpose = (int)std::round(index * 12.f);
    if(this->mv.versio.tap.RisingEdge())
    {
        spectra.spectra_quantize = (spectra.spectra_quantize + 1) % 9;
        if(spectra.spectra_quantize > 0)
        {
            this->mv.leds.SetBaseColor(0, 0, 1, 0);
            switch(spectra.spectra_quantize)
            {
                case 1:
                    spectra.spectra_selected_scale = scale_12;
                    this->mv.leds.SetBaseColor(3, 0, 0, 1);
                    break;
                case 2:
                    spectra.spectra_selected_scale = scale_7;
                    this->mv.leds.SetBaseColor(3, 0, 0, 0.8);
                    break;
                case 3:
                    spectra.spectra_selected_scale = scale_6;
                    this->mv.leds.SetBaseColor(3, 0, 0.3, 0.6);
                    break;
                case 4:
                    spectra.spectra_selected_scale = scale_5;
                    this->mv.leds.SetBaseColor(3, 0, 0.4, 0.4);
                    break;
                case 5:
                    this->mv.leds.SetBaseColor(3, 0, 0.6, 0.3);
                    spectra.spectra_selected_scale = scale_4;
                    break;
                case 6:
                    this->mv.leds.SetBaseColor(3, 0, 0.7, 0.2);
                    spectra.spectra_selected_scale = scale_3;
                    break;
                case 7:
                    this->mv.leds.SetBaseColor(3, 0, 0.4, 0.1);
                    spectra.spectra_selected_scale = scale_2;
                    break;
                case 8:
                    this->mv.leds.SetBaseColor(3, 0.4, 0.4, 0.0);
                    spectra.spectra_selected_scale = scale_1;
                    break;
                default: break;
            }
        }
        else
        {
            this->mv.leds.SetBaseColor(0, 0, 0, 0);
            this->mv.leds.SetBaseColor(3, 0, 0, 0);
        };
    };

    spectra.spectra_oscbank.calculatedSuggestedHop();
    // SelectSpectraQuality(0.8);
    for(int i = 0; i < NUM_OF_STRINGS; i++)
    {
        string_voice[i].SetBrightness(spectra.spectra_oscbank.getMagnitudo(i)
                                      * regen);

        string_voice[i].SetAccent(spectra.spectra_oscbank.getMagnitudo(i));

        string_voice[i].SetStructure(dense);
    }

    // spectra_waveform = ((int)(dense*9.f));
    // spectra_oscbank.SetAllWaveforms(spectra_waveform);

    spectra.spectra_num_active = NUM_OF_STRINGS;

    spectra.spectra_spread
        = 4.f; // clamp(map(size-0.5, 0.0f, 0.5f, 1.0f, 4.0f), 1.0f, 4.0f);
    spectra.spectra_lower_harmonics = 0.0f;
    spectra.spectra_oscbank.SetNumActive(spectra.spectra_num_active);

    spectra.SelectSpectraOctave(tone);

    if(spectrings_trigger_next_cycle)
    {
        string_voice[spectrings_current_voice].SetDamping(
            spectrings_decay_amount[spectrings_current_voice]);
        string_voice[spectrings_current_voice].Trig();
        spectrings_trigger_next_cycle = false;
    }

    if(gate)
    {
        spectra.spectra_do_analysis = true;
        spectrings_current_voice
            = (spectrings_current_voice + 1) % spectrings_active_voices;

        spectrings_trigger_next_cycle = true;
        spectrings_accent_amount[spectrings_current_voice]
            = spectra.spectra_oscbank.getMagnitudo(spectrings_current_voice);
        spectrings_decay_amount[spectrings_current_voice] = size;
        spectrings_attack_step[spectrings_current_voice]  = 0;

        if(spectrings_current_voice == 0)
        {
            this->mv.leds.SetForXCycles(1, 10, 1, 1, 1);
        }
        else
        {
            this->mv.leds.SetForXCycles(2, 10, 1, 1, 1);
        };
    }

    spectrings_drywet     = blend;
    spectrings_pan_spread = 1;
    this->mv.reverb_lowpass
        = this->mv.global_sample_rate * 0.4 * (1 - speed * speed * 0.6) / 2.f;
    this->mv.rev.SetLpFreq(this->mv.reverb_lowpass);
    this->mv.reverb_shimmer = 0.0f;
    this->mv.reverb_feedback
        = 0.7f + (std::log10(10 + speed * 90) - 1.000001f) * 0.299;
    this->mv.rev.SetFeedback(this->mv.reverb_feedback);

    this->mv.reverb_compression = 0.5f;
    spectra.spectra_drywet      = 1.0; // TODO what is this?. IMPLEMENT

    this->mv.reverb_drywet = clamp(
        map(clamp(speed * 1.1f, 0.0f, 1.0f) * 0.95, 0.0, 0.95, 0.7f, 0.95f),
        0.0f,
        0.95f);
}

/**
 * @brief Returns whether or not the Spectrings effect uses reverb.
 *
 * @return true
 */
bool Spectrings::usesReverb()
{
    return true;
}

/**
 * @brief Runs pre-processing on the Spectrings effect.
 *
 * Sets the frequency of the string voice.
 * Input samples and size are not used.
 * Overrides the IEffect::preProcess method.
 *
 * @param in1 The left input sample.
 * @param in2 The right input sample.
 * @param size The size of the input.
 */
void Spectrings::preProcess(const float *in1, const float *in2, size_t size)
{
    string_voice[spectrings_current_voice].SetFreq(
        spectra.spectra_oscbank.getFrequency(spectrings_current_voice));
}