#include "Synth.h"
#include "SynthDAC.h"
#include "SynthTimer.h"

#include <stdbool.h>

#define FIXED_POINT_COEF 10000

static unsigned int sampleSizes[] =
{
    123, /* C4 */
    110, /* D4 */
    98,  /* E4 */
    93,  /* F4 */
    82,  /* G4 */
    74,  /* A4 */
    66,  /* B4 */
    62   /* C5 */
};

static float c4Samples[] =
{
    0.4257,
    0.3787,
    0.3213,
    0.2552,
    0.1830,
    0.1111,
    0.0474,
    0.0023,
   -0.0200,
   -0.0237,
   -0.0204,
   -0.0270,
   -0.0528,
   -0.0998,
   -0.1576,
   -0.2134,
   -0.2559,
   -0.2789,
   -0.2824,
   -0.2721,
   -0.2556,
   -0.2380,
   -0.2261,
   -0.2206,
   -0.2231,
   -0.2340,
   -0.2463,
   -0.2514,
   -0.2382,
   -0.1983,
   -0.1331,
   -0.0513,
    0.0324,
    0.1065,
    0.1676,
    0.2185,
    0.2678,
    0.3256,
    0.3969,
    0.4841,
    0.5816,
    0.6811,
    0.7739,
    0.8527,
    0.9127,
    0.9495,
    0.9653,
    0.9636,
    0.9552,
    0.9501,
    0.9522,
    0.9612,
    0.9719,
    0.9804,
    0.9808,
    0.9691,
    0.9402,
    0.8910,
    0.8224,
    0.7384,
    0.6419,
    0.5336,
    0.4126,
    0.2790,
    0.1412,
    0.0123,
   -0.0921,
   -0.1666,
   -0.2154,
   -0.2543,
   -0.2996,
   -0.3609,
   -0.4402,
   -0.5320,
   -0.6274,
   -0.7165,
   -0.7908,
   -0.8441,
   -0.8730,
   -0.8760,
   -0.8551,
   -0.8190,
   -0.7812,
   -0.7543,
   -0.7445,
   -0.7472,
   -0.7515,
   -0.7444,
   -0.7204,
   -0.6838,
   -0.6477,
   -0.6264,
   -0.6301,
   -0.6586,
   -0.6995,
   -0.7402,
   -0.7682,
   -0.7792,
   -0.7722,
   -0.7448,
   -0.6979,
   -0.6339,
   -0.5588,
   -0.4835,
   -0.4148,
   -0.3538,
   -0.2970,
   -0.2371,
   -0.1702,
   -0.0934,
   -0.0086,
    0.0793,
    0.1667,
    0.2461,
    0.3128,
    0.3647,
    0.4059,
    0.4420,
    0.4737,
    0.4970,
    0.5030,
    0.4829,
    0.4339
};

static float d4Samples[] =
{
    -0.1784,
   -0.1744,
   -0.1518,
   -0.1132,
   -0.0662,
   -0.0194,
    0.0230,
    0.0619,
    0.0984,
    0.1287,
    0.1444,
    0.1397,
    0.1161,
    0.0803,
    0.0388,
   -0.0049,
   -0.0479,
   -0.0865,
   -0.1177,
   -0.1428,
   -0.1682,
   -0.2042,
   -0.2605,
   -0.3384,
   -0.4275,
   -0.5106,
   -0.5753,
   -0.6214,
   -0.6579,
   -0.6946,
   -0.7356,
   -0.7778,
   -0.8118,
   -0.8251,
   -0.8079,
   -0.7574,
   -0.6798,
   -0.5868,
   -0.4908,
   -0.4020,
   -0.3259,
   -0.2626,
   -0.2058,
   -0.1443,
   -0.0670,
    0.0299,
    0.1406,
    0.2535,
    0.3573,
    0.4442,
    0.5107,
    0.5577,
    0.5925,
    0.6257,
    0.6643,
    0.7061,
    0.7430,
    0.7704,
    0.7932,
    0.8217,
    0.8614,
    0.9068,
    0.9452,
    0.9651,
    0.9628,
    0.9421,
    0.9090,
    0.8668,
    0.8157,
    0.7560,
    0.6924,
    0.6316,
    0.5753,
    0.5154,
    0.4376,
    0.3332,
    0.2074,
    0.0787,
   -0.0308,
   -0.1060,
   -0.1460,
   -0.1637,
   -0.1770,
   -0.1981,
   -0.2272,
   -0.2561,
   -0.2755,
   -0.2792,
   -0.2645,
   -0.2327,
   -0.1903,
   -0.1500,
   -0.1261,
   -0.1288,
   -0.1589,
   -0.2078,
   -0.2616,
   -0.3065,
   -0.3312,
   -0.3294,
   -0.3023,
   -0.2611,
   -0.2228,
   -0.1997,
   -0.1933,
   -0.1969,
   -0.2030,
   -0.2074,
   -0.2083,
   -0.2034 
};

static float e4Samples[] =
{
    0.3915,
    0.4470,
    0.5121,
    0.5681,
    0.5966,
    0.5904,
    0.5544,
    0.5009,
    0.4452,
    0.4024,
    0.3822,
    0.3807,
    0.3822,
    0.3718,
    0.3442,
    0.3006,
    0.2442,
    0.1818,
    0.1254,
    0.0842,
    0.0579,
    0.0395,
    0.0224,
    0.0054,
   -0.0055,
   -0.0020,
    0.0160,
    0.0353,
    0.0395,
    0.0261,
    0.0108,
    0.0148,
    0.0472,
    0.0991,
    0.1525,
    0.1947,
    0.2236,
    0.2424,
    0.2531,
    0.2573,
    0.2611,
    0.2740,
    0.2996,
    0.3269,
    0.3348,
    0.3101,
    0.2621,
    0.2166,
    0.1950,
    0.2013,
    0.2255,
    0.2517,
    0.2642,
    0.2538,
    0.2238,
    0.1891,
    0.1651,
    0.1579,
    0.1643,
    0.1757,
    0.1800,
    0.1603,
    0.1004,
   -0.0051,
   -0.1452,
   -0.2969,
   -0.4373,
   -0.5545,
   -0.6507,
   -0.7351,
   -0.8112,
   -0.8745,
   -0.9188,
   -0.9449,
   -0.9599,
   -0.9709,
   -0.9780,
   -0.9734,
   -0.9461,
   -0.8899,
   -0.8074,
   -0.7061,
   -0.5951,
   -0.4850,
   -0.3882,
   -0.3126,
   -0.2547,
   -0.1995,
   -0.1328,
   -0.0527,
    0.0293,
    0.1008,
    0.1580,
    0.2044,
    0.2467,
    0.2947,
    0.3584,
    0.4374
};

static float f4Samples[] =
{
    0.1640,
    0.1794,
    0.1862,
    0.1949,
    0.2220,
    0.2826,
    0.3753,
    0.4826,
    0.5834,
    0.6617,
    0.7093,
    0.7277,
    0.7258,
    0.7098,
    0.6797,
    0.6336,
    0.5725,
    0.5032,
    0.4371,
    0.3823,
    0.3324,
    0.2678,
    0.1747,
    0.0615,
   -0.0461,
   -0.1275,
   -0.1823,
   -0.2260,
   -0.2756,
   -0.3398,
   -0.4199,
   -0.5149,
   -0.6222,
   -0.7295,
   -0.8120,
   -0.8519,
   -0.8609,
   -0.8739,
   -0.9135,
   -0.9667,
   -0.9984,
   -0.9822,
   -0.9167,
   -0.8192,
   -0.7133,
   -0.6207,
   -0.5534,
   -0.5062,
   -0.4595,
   -0.3928,
   -0.2956,
   -0.1715,
   -0.0369,
    0.0857,
    0.1796,
    0.2396,
    0.2713,
    0.2860,
    0.2918,
    0.2870,
    0.2665,
    0.2359,
    0.2111,
    0.2019,
    0.2028,
    0.1968,
    0.1657,
    0.1017,
    0.0147,
   -0.0735,
   -0.1448,
   -0.1909,
   -0.2093,
   -0.2004,
   -0.1712,
   -0.1375,
   -0.1123,
   -0.0915,
   -0.0572,
   -0.0003,
    0.0665,
    0.1211,
    0.1501,
    0.1524,
    0.1368,
    0.1197,
    0.1185,
    0.1397,
    0.1764,
    0.2145,
    0.2403,
    0.2478,
    0.2433
};

static float g4Samples[] =
{
    0.9771,
    0.9166,
    0.8686,
    0.8503,
    0.8463,
    0.8197,
    0.7470,
    0.6392,
    0.5246,
    0.4190,
    0.3179,
    0.2154,
    0.1158,
    0.0208,
   -0.0758,
   -0.1703,
   -0.2391,
   -0.2676,
   -0.2760,
   -0.3017,
   -0.3604,
   -0.4346,
   -0.4965,
   -0.5374,
   -0.5675,
   -0.5871,
   -0.5739,
   -0.5095,
   -0.4037,
   -0.2822,
   -0.1623,
   -0.0539,
    0.0305,
    0.0885,
    0.1422,
    0.2171,
    0.3112,
    0.3922,
    0.4159,
    0.3524,
    0.2143,
    0.0547,
   -0.0707,
   -0.1409,
   -0.1811,
   -0.2364,
   -0.3247,
   -0.4240,
   -0.5053,
   -0.5574,
   -0.5805,
   -0.5810,
   -0.5735,
   -0.5739,
   -0.5859,
   -0.5961,
   -0.5864,
   -0.5536,
   -0.5159,
   -0.4966,
   -0.4996,
   -0.5024,
   -0.4837,
   -0.4461,
   -0.3977,
   -0.3333,
   -0.2498,
   -0.1551,
   -0.0545,
    0.0504,
    0.1491,
    0.2268,
    0.2811,
    0.3280,
    0.3929,
    0.4963,
    0.6378,
    0.7886,
    0.9031,
    0.9505,
    0.9422,
    0.9211
};

static float a4Samples[] =
{
    0.9354,
    0.9848,
    0.9303,
    0.7864,
    0.6347,
    0.5301,
    0.4540,
    0.3687,
    0.2610,
    0.1365,
    0.0108,
   -0.1001,
   -0.1884,
   -0.2549,
   -0.3118,
   -0.3750,
   -0.4508,
   -0.5321,
   -0.6178,
   -0.7150,
   -0.8144,
   -0.8921,
   -0.9267,
   -0.9110,
   -0.8672,
   -0.8236,
   -0.7753,
   -0.7028,
   -0.6075,
   -0.5055,
   -0.4096,
   -0.3299,
   -0.2699,
   -0.2140,
   -0.1294,
   -0.0089,
    0.0966,
    0.1326,
    0.1026,
    0.0570,
    0.0502,
    0.1091,
    0.2178,
    0.3280,
    0.3935,
    0.4050,
    0.3822,
    0.3514,
    0.3316,
    0.3145,
    0.2784,
    0.2188,
    0.1483,
    0.0836,
    0.0250,
   -0.0475,
   -0.1338,
   -0.2022,
   -0.2364,
   -0.2524,
   -0.2585,
   -0.2349,
   -0.1634,
   -0.0579,
    0.0571,
    0.1777,
    0.2951,
    0.3801,
    0.4347,
    0.5130,
    0.6509,
    0.8132,
    0.9274,
    0.9500
};

static float b4Samples[] =
{
   -0.1463,
   -0.2145,
   -0.2733,
   -0.2996,
   -0.2759,
   -0.1955,
   -0.0916,
   -0.0203,
    0.0059,
   -0.0077,
   -0.0589,
   -0.1011,
   -0.0965,
   -0.0806,
   -0.1193,
   -0.2312,
   -0.3826,
   -0.5393,
   -0.6629,
   -0.7216,
   -0.7343,
   -0.7445,
   -0.7538,
   -0.7241,
   -0.6497,
   -0.5733,
   -0.5278,
   -0.4942,
   -0.4333,
   -0.3285,
   -0.1789,
   -0.0048,
    0.1592,
    0.2903,
    0.3848,
    0.4728,
    0.5788,
    0.6833,
    0.7835,
    0.8887,
    0.9555,
    0.9336,
    0.8404,
    0.7424,
    0.6808,
    0.6450,
    0.6023,
    0.5163,
    0.3752,
    0.2189,
    0.1006,
    0.0264,
   -0.0436,
   -0.1302,
   -0.2012,
   -0.2054,
   -0.1433,
   -0.0784,
   -0.0636,
   -0.0897,
   -0.1055,
   -0.0971,
   -0.0899,
   -0.0932,
   -0.1255,
   -0.1978
};

static float c5Samples[] =
{
    0.0453,
    0.1165,
    0.2258,
    0.3231,
    0.3750,
    0.3906,
    0.4020,
    0.3956,
    0.3470,
    0.2864,
    0.2433,
    0.2116,
    0.1834,
    0.1457,
    0.0838,
    0.0038,
   -0.0592,
   -0.1104,
   -0.2089,
   -0.3458,
   -0.4544,
   -0.5319,
   -0.6335,
   -0.7767,
   -0.9163,
   -0.9859,
   -0.9450,
   -0.8201,
   -0.6884,
   -0.6000,
   -0.5365,
   -0.4338,
   -0.2647,
   -0.0817,
    0.0784,
    0.2291,
    0.3568,
    0.4374,
    0.4870,
    0.5199,
    0.5245,
    0.4986,
    0.4618,
    0.4139,
    0.3444,
    0.2838,
    0.2478,
    0.2034,
    0.1383,
    0.0952,
    0.1109,
    0.1655,
    0.2068,
    0.1746,
    0.0718,
   -0.0151,
   -0.0317,
   -0.0021,
    0.0237,
    0.0192,
    0.0199,
    0.0774
};

static float * noteSamples[] =
{   c4Samples,
    d4Samples,
    e4Samples,
    f4Samples,
    g4Samples,
    a4Samples,
    b4Samples,
    c5Samples
};


static bool currentKeyType;

const unsigned char dacMidLevel = 128;
const float samplingFrequency  = 32000;

const float notesFrequencies[KEYS_SIZE] = 
{   261.63, /*C4*/
    293.66, /*D4*/
    329.63, /*E4*/
    349.23, /*F4*/
    392.00, /*G4*/
    440.00, /*A4*/
    493.88, /*B4*/
    523.25  /*C5*/
}; 

static SquareWaveKey squareKeys[KEYS_SIZE];

static SampleWaveKey sampleKeys[KEYS_SIZE];

static volatile signed char signalSum = 0;
static volatile bool processNextDAC = false;

static void updateDAC (void)
{
    SynthDAC_Write(dacMidLevel + signalSum);
    processNextDAC = true;
}

static void fixedPointConvert (int32_t * fixedSamples ,float * floatSamples, unsigned int sampleSize)
{
    unsigned int index;
    for (index = 0; index < sampleSize; index++)
    {
        fixedSamples[index] = (int32_t)(floatSamples[index] * FIXED_POINT_COEF);
    }
}

static void initKeys(void)
{
    for (int i = 0; i < KEYS_SIZE; i++)
    {
        squareKeys[i].pressed = false;
        squareKeys[i].counter = 0;
        squareKeys[i].period = (unsigned int)(samplingFrequency/notesFrequencies[i]);
        squareKeys[i].amplitude = 0;

        sampleKeys[i].pressed = false;
        sampleKeys[i].periodCounter = 0;
        sampleKeys[i].periodSize = sampleSizes[i];
        fixedPointConvert(sampleKeys[i].samples, noteSamples[i], sampleSizes[i]);
        sampleKeys[i].amplitude = 0;
        sampleKeys[i].tickCounter = 0;
        sampleKeys[i].ADSRGain = 0;
    }
}

void Synth_Open(void)
{
    initKeys();
    signalSum = 0;
    currentKeyType = squareSignal;
    processNextDAC = false;

    SynthDAC_Open();
    SynthDAC_Write(dacMidLevel);

    SynthTimer_Open(updateDAC);
}


void Synth_Close(void)
{
    SynthTimer_Close();

    SynthDAC_Write(0);
    SynthDAC_Close();
}

void Synth_Press(Note note)
{
    squareKeys[note].pressed = true;
    sampleKeys[note].pressed = true;
}

void Synth_Release(Note note)
{
    squareKeys[note].pressed = false;
    sampleKeys[note].pressed = false;
}


static void processSquareKeys (void)
{
    for (int i = 0; i < KEYS_SIZE; i++)
    {
        if(!squareKeys[i].pressed)
        {
            squareKeys[i].counter = 0;
            squareKeys[i].amplitude = 0;
            continue;
        } 

        if (squareKeys[i].counter >= squareKeys[i].period/2)
        {
            squareKeys[i].amplitude = -12;
        }
        else squareKeys[i].amplitude = 12;

        squareKeys[i].counter++;
        squareKeys[i].counter = squareKeys[i].counter % squareKeys[i].period;
    }
}

static const int32_t attackTarget  =(int32_t)(1        * FIXED_POINT_COEF);
static const int32_t attackGain    =(int32_t)( 0.01    * FIXED_POINT_COEF);

static const int32_t decayTarget   =(int32_t)( 0       * FIXED_POINT_COEF);
// Trocar valor de 0.00006 por 0.0001 para entrar na conversão de ponto fixo
static const int32_t decayGain     =(int32_t)( 0.0001 * FIXED_POINT_COEF);   

static void processSampleADSR (SampleWaveKey * key)
{
    /* Processar proximo valor do ADSR usando duas curvas exponenciais */
    /* Uma para ataque e outra para queda */
    float keyTime = key->tickCounter / samplingFrequency;
    int32_t auxiliar, oldADSRGain;

    oldADSRGain = key->ADSRGain;
    if (keyTime < 0.02) /* Ataque */
    {   
        /* Função original */
        /* key->ADSRGain = attackTarget*attackGain + (1.0 - attackGain)*(key->ADSRGain); */

        key->ADSRGain = attackTarget*attackGain;
        key->ADSRGain = key->ADSRGain / FIXED_POINT_COEF;

        auxiliar = (1 * FIXED_POINT_COEF) - attackGain;
        auxiliar = auxiliar * oldADSRGain;
        auxiliar = auxiliar / FIXED_POINT_COEF;

        key->ADSRGain = key->ADSRGain + auxiliar;
    }
    else                /* Decaimento */
    {
        /* Equação original */
        /* key->ADSRGain = decayTarget*decayGain + (1.0 - decayGain)*key->ADSRGain; */
        key->ADSRGain = decayTarget*attackGain;
        key->ADSRGain = key->ADSRGain / FIXED_POINT_COEF;

        auxiliar = (1 * FIXED_POINT_COEF) - decayGain;
        auxiliar = auxiliar * oldADSRGain;
        auxiliar = auxiliar / FIXED_POINT_COEF;

        key->ADSRGain = key->ADSRGain + auxiliar;
    }

    key->tickCounter++;
}

static void processSampleKeys (void)
{
    int i = 0;
    for ( i = 0; i < KEYS_SIZE; i++)
    {
        if(!sampleKeys[i].pressed)
        {
            sampleKeys[i].periodCounter = 0;
            sampleKeys[i].amplitude = 0;
            sampleKeys[i].ADSRGain = 0;
            sampleKeys[i].tickCounter = 0;
            continue;
        } 

        processSampleADSR(&sampleKeys[i]);
        sampleKeys[i].amplitude = sampleKeys[i].samples[sampleKeys[i].periodCounter];
        sampleKeys[i].amplitude *= sampleKeys[i].ADSRGain;
        sampleKeys[i].amplitude = sampleKeys[i].amplitude/FIXED_POINT_COEF;

        sampleKeys[i].periodCounter++;
        sampleKeys[i].periodCounter = sampleKeys[i].periodCounter % sampleKeys[i].periodSize;
    }
}

static float getActiveSampleKeys (void)
{
    int i = 0;
    float totalActive = 0;
    for ( i = 0; i < KEYS_SIZE; i++)
    {
        if (sampleKeys[i].pressed ==  true) totalActive += 1;
    }

    if (totalActive == 0) return 1;
    return totalActive;
}

void Synth_Run(void)
{
    int signalSumBuffer = 0;

    int squareSignalSumBuffer = 0;

    int32_t SampleSignalSumBuffer = 0;
    float   floatSampleSignalSumBuffer = 0;

    float compressedSignalSumBuffer = 0;
    float compressionCoefficient = 1;
    bool isTimeForProcess = false;

    SynthTimer_DisableInterrupt();
    isTimeForProcess = processNextDAC;
    SynthTimer_EnableInterrupt();

    if (!isTimeForProcess) return;

    processSquareKeys();
    processSampleKeys();

    for (int i = 0; i < KEYS_SIZE; i++)
    {
        squareSignalSumBuffer += squareKeys[i].amplitude;
        SampleSignalSumBuffer += sampleKeys[i].amplitude;
    }

    if (currentKeyType == sampleSignal)
    {
        floatSampleSignalSumBuffer = ((float)SampleSignalSumBuffer)/FIXED_POINT_COEF;
        compressionCoefficient = 1.0/(getActiveSampleKeys()); /* Dividir 1 por N^(4/5) para ter melhor qualidade */
        compressedSignalSumBuffer = (floatSampleSignalSumBuffer * 127.0) * compressionCoefficient;
        signalSumBuffer = (int)compressedSignalSumBuffer;
    }
    else if (currentKeyType == squareSignal) signalSumBuffer = squareSignalSumBuffer;

    if (signalSumBuffer > 127) signalSumBuffer = 127;
    if (signalSumBuffer < -127)signalSumBuffer = -127;

    SynthTimer_DisableInterrupt();
    signalSum = (signed char) signalSumBuffer;
    processNextDAC = false;
    SynthTimer_EnableInterrupt();
}

void Synth_SetKeys(KeyType type)
{
    currentKeyType = type;
}

char Synth_GetNextDACValue(void)
{
    char signal;
    SynthTimer_DisableInterrupt();
    signal = signalSum;
    SynthTimer_EnableInterrupt();
    return signal;
}

SampleWaveKey  Synth_GetNoteSampleWaveKey(Note note)
{
    return sampleKeys[note];
}
