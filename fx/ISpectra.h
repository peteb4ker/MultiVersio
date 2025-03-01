#pragma once

#define MAX_SPECTRA_FREQUENCIES 6

/**
 * @class ISpectra
 * @brief Represents the interface for handling spectra data.
 *
 * This class provides member variables and functions for manipulating spectra data.
 * It includes arrays for different scales, variables for harmonics, octaves, spread, and other parameters.
 * The class also defines constants for the chromatic scale and other properties related to spectra.
 */
class ISpectra
{
  public:
    bool *spectra_selected_scale; /**< Pointer to the selected scale */
    bool  scale_1[12]
        = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; /**< Scale 1 array */
    bool scale_12[12]
        = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; /**< Scale 12 array */
    bool scale_2[12]
        = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}; /**< Scale 2 array */
    bool scale_3[12]
        = {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0}; /**< Scale 3 array */
    bool scale_4[12]
        = {1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0}; /**< Scale 4 array */
    bool scale_5[12]
        = {1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0}; /**< Scale 5 array */
    bool scale_6[12]
        = {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0}; /**< Scale 6 array */
    bool scale_7[12]
        = {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1}; /**< Scale 7 array */
    float spectra_lower_harmonics = 0.f;        /**< Lower harmonics value */
    float spectra_oct_mult;                     /**< Octave multiplier value */
    float spectra_spread = 1.0f;                /**< Spread value */
    int   CHRM_SCALE[128]
        = {8176,     8662,    9177,    9723,    10301,   10913,    11562,
           12250,    12978,   13750,   14568,   15434,   16352,    17324,
           18354,    19445,   20602,   21827,   23125,   24500,    25957,
           27500,    29135,   30868,   32703,   34648,   36708,    38891,
           41203,    43654,   46249,   48999,   51913,   55000,    58270,
           61735,    65406,   69296,   73416,   77782,   82407,    87307,
           92499,    97999,   103826,  110000,  116541,  123471,   130813,
           138591,   146832,  155563,  164814,  174614,  184997,   195998,
           207652,   220000,  233082,  246942,  261626,  277183,   293665,
           311127,   329628,  349228,  369994,  391995,  415305,   440000,
           466164,   493883,  523251,  554365,  587330,  622254,   659255,
           698456,   739989,  783991,  830609,  880000,  932328,   987767,
           1046502,  1108731, 1174659, 1244508, 1318510, 1396913,  1479978,
           1567982,  1661219, 1760000, 1864655, 1975533, 2093005,  2217461,
           2349318,  2489016, 2637020, 2793826, 2959955, 3135963,  3322438,
           3520000,  3729310, 3951066, 4186009, 4434922, 4698636,  4978032,
           5274041,  5587652, 5919911, 6271927, 6644875, 7040000,  7458620,
           7902133,  8372018, 8869844, 9397273, 9956063, 10548080, 11175300,
           11839820, 12543850}; /**< Chromatic scale array */
    int spectra_num_active
        = MAX_SPECTRA_FREQUENCIES; /**< Number of active spectra */
    int spectra_quantize  = 0;     /**< Quantize value */
    int spectra_transpose = 0;     /**< Transpose value */
};
