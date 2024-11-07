#include "Synth.h"
#include "SynthDAC.h"
#include "SynthTimer.h"

#include <stdbool.h>
#include <math.h>

#define FIXED_POINT_COEF 10000

static OperationType pointOperation = floatingPoint;

static unsigned int sampleSizesOctave3[] =
{
    245, /* C3 */
    219, /* D3 */
    195, /* E3 */
    184, /* F3 */
    164, /* G3 */
    146, /* A3 */
    130, /* B3 */
    123 /* C4 */
};

static unsigned int sampleSizesOctave4[] =
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

static unsigned int * sampleSizes;

static float c3Samples[] =
{
    0.0937,
    0.1015,
    0.1117,
    0.1238,
    0.1371,
    0.1505,
    0.1629,
    0.1744,
    0.1841,
    0.1897,
    0.1911,
    0.1894,
    0.1836,
    0.1726,
    0.1577,
    0.1398,
    0.1189,
    0.0962,
    0.0731,
    0.0509,
    0.0316,
    0.0169,
    0.0075,
    0.0049,
    0.0098,
    0.0216,
    0.0392,
    0.0620,
    0.0886,
    0.1162,
    0.1432,
    0.1690,
    0.1922,
    0.2109,
    0.2249,
    0.2349,
    0.2407,
    0.2421,
    0.2400,
    0.2348,
    0.2277,
    0.2198,
    0.2105,
    0.2004,
    0.1916,
    0.1839,
    0.1761,
    0.1696,
    0.1650,
    0.1611,
    0.1581,
    0.1563,
    0.1548,
    0.1526,
    0.1493,
    0.1431,
    0.1332,
    0.1200,
    0.1021,
    0.0789,
    0.0523,
    0.0230,
   -0.0088,
   -0.0410,
   -0.0719,
   -0.1011,
   -0.1288,
   -0.1549,
   -0.1804,
   -0.2061,
   -0.2335,
   -0.2644,
   -0.2994,
   -0.3384,
   -0.3817,
   -0.4290,
   -0.4789,
   -0.5295,
   -0.5804,
   -0.6311,
   -0.6802,
   -0.7266,
   -0.7706,
   -0.8123,
   -0.8509,
   -0.8860,
   -0.9174,
   -0.9444,
   -0.9664,
   -0.9828,
   -0.9928,
   -0.9971,
   -0.9965,
   -0.9899,
   -0.9784,
   -0.9639,
   -0.9469,
   -0.9277,
   -0.9085,
   -0.8900,
   -0.8730,
   -0.8583,
   -0.8461,
   -0.8366,
   -0.8302,
   -0.8258,
   -0.8217,
   -0.8182,
   -0.8148,
   -0.8098,
   -0.8030,
   -0.7950,
   -0.7851,
   -0.7730,
   -0.7586,
   -0.7417,
   -0.7225,
   -0.7003,
   -0.6742,
   -0.6445,
   -0.6120,
   -0.5764,
   -0.5376,
   -0.4963,
   -0.4536,
   -0.4101,
   -0.3660,
   -0.3226,
   -0.2815,
   -0.2431,
   -0.2075,
   -0.1756,
   -0.1480,
   -0.1242,
   -0.1035,
   -0.0846,
   -0.0666,
   -0.0490,
   -0.0301,
   -0.0087,
    0.0146,
    0.0402,
    0.0696,
    0.1023,
    0.1375,
    0.1750,
    0.2142,
    0.2545,
    0.2955,
    0.3357,
    0.3746,
    0.4126,
    0.4483,
    0.4806,
    0.5107,
    0.5388,
    0.5638,
    0.5857,
    0.6048,
    0.6210,
    0.6350,
    0.6467,
    0.6566,
    0.6660,
    0.6754,
    0.6844,
    0.6939,
    0.7050,
    0.7171,
    0.7290,
    0.7398,
    0.7488,
    0.7545,
    0.7549,
    0.7490,
    0.7368,
    0.7179,
    0.6923,
    0.6612,
    0.6267,
    0.5908,
    0.5550,
    0.5200,
    0.4877,
    0.4593,
    0.4339,
    0.4111,
    0.3917,
    0.3749,
    0.3592,
    0.3446,
    0.3304,
    0.3162,
    0.3018,
    0.2861,
    0.2689,
    0.2516,
    0.2335,
    0.2142,
    0.1953,
    0.1775,
    0.1608,
    0.1457,
    0.1322,
    0.1212,
    0.1128,
    0.1067,
    0.1029,
    0.1023,
    0.1046,
    0.1090,
    0.1153,
    0.1236,
    0.1338,
    0.1446,
    0.1553,
    0.1664,
    0.1775,
    0.1874,
    0.1963,
    0.2044,
    0.2109,
    0.2156,
    0.2178,
    0.2171,
    0.2143,
    0.2093,
    0.2012,
    0.1913,
    0.1807,
    0.1685,
    0.1553,
    0.1421,
    0.1291,
    0.1164,
    0.1043,
    0.0929,
    0.0835,
    0.0770,
    0.0731,
    0.0728,
    0.0775,
    0.0867,
    0.0997,
    0.1160
};

static float d3Samples[] =
{
   -0.4068,
   -0.4177,
   -0.4270,
   -0.4367,
   -0.4482,
   -0.4615,
   -0.4786,
   -0.5010,
   -0.5282,
   -0.5601,
   -0.5961,
   -0.6346,
   -0.6730,
   -0.7089,
   -0.7417,
   -0.7701,
   -0.7937,
   -0.8142,
   -0.8334,
   -0.8523,
   -0.8718,
   -0.8927,
   -0.9147,
   -0.9356,
   -0.9537,
   -0.9682,
   -0.9770,
   -0.9789,
   -0.9741,
   -0.9635,
   -0.9479,
   -0.9282,
   -0.9069,
   -0.8854,
   -0.8640,
   -0.8434,
   -0.8244,
   -0.8058,
   -0.7866,
   -0.7657,
   -0.7425,
   -0.7156,
   -0.6835,
   -0.6470,
   -0.6059,
   -0.5594,
   -0.5086,
   -0.4543,
   -0.3968,
   -0.3363,
   -0.2734,
   -0.2094,
   -0.1443,
   -0.0786,
   -0.0148,
    0.0462,
    0.1033,
    0.1551,
    0.2007,
    0.2402,
    0.2755,
    0.3076,
    0.3374,
    0.3676,
    0.3989,
    0.4305,
    0.4621,
    0.4925,
    0.5198,
    0.5416,
    0.5561,
    0.5631,
    0.5609,
    0.5491,
    0.5292,
    0.5020,
    0.4686,
    0.4315,
    0.3931,
    0.3563,
    0.3225,
    0.2938,
    0.2724,
    0.2579,
    0.2503,
    0.2495,
    0.2539,
    0.2617,
    0.2707,
    0.2794,
    0.2860,
    0.2878,
    0.2843,
    0.2753,
    0.2599,
    0.2386,
    0.2128,
    0.1837,
    0.1525,
    0.1202,
    0.0893,
    0.0609,
    0.0356,
    0.0154,
    0.0014,
   -0.0066,
   -0.0085,
   -0.0051,
    0.0028,
    0.0124,
    0.0217,
    0.0297,
    0.0344,
    0.0348,
    0.0317,
    0.0259,
    0.0186,
    0.0108,
    0.0040,
   -0.0008,
   -0.0046,
   -0.0075,
   -0.0096,
   -0.0124,
   -0.0165,
   -0.0220,
   -0.0285,
   -0.0355,
   -0.0432,
   -0.0497,
   -0.0542,
   -0.0571,
   -0.0571,
   -0.0542,
   -0.0484,
   -0.0401,
   -0.0302,
   -0.0178,
   -0.0046,
    0.0082,
    0.0218,
    0.0347,
    0.0458,
    0.0560,
    0.0644,
    0.0712,
    0.0762,
    0.0791,
    0.0818,
    0.0839,
    0.0856,
    0.0894,
    0.0956,
    0.1044,
    0.1171,
    0.1338,
    0.1540,
    0.1760,
    0.1986,
    0.2212,
    0.2411,
    0.2576,
    0.2713,
    0.2825,
    0.2922,
    0.3017,
    0.3134,
    0.3289,
    0.3478,
    0.3717,
    0.4015,
    0.4350,
    0.4717,
    0.5108,
    0.5499,
    0.5868,
    0.6190,
    0.6453,
    0.6645,
    0.6744,
    0.6760,
    0.6709,
    0.6586,
    0.6411,
    0.6214,
    0.6008,
    0.5806,
    0.5626,
    0.5481,
    0.5372,
    0.5285,
    0.5212,
    0.5146,
    0.5062,
    0.4936,
    0.4779,
    0.4583,
    0.4331,
    0.4038,
    0.3711,
    0.3335,
    0.2913,
    0.2443,
    0.1936,
    0.1400,
    0.0837,
    0.0286,
   -0.0232,
   -0.0718,
   -0.1137,
   -0.1475,
   -0.1749,
   -0.1966,
   -0.2140,
   -0.2292,
   -0.2445,
   -0.2621,
   -0.2828,
   -0.3067,
};

static float e3Samples[] = 
{
    0.0093,
    0.0559,
    0.0923,
    0.1174,
    0.1328,
    0.1400,
    0.1408,
    0.1375,
    0.1333,
    0.1293,
    0.1274,
    0.1299,
    0.1366,
    0.1476,
    0.1631,
    0.1824,
    0.2036,
    0.2258,
    0.2479,
    0.2664,
    0.2801,
    0.2874,
    0.2865,
    0.2773,
    0.2615,
    0.2406,
    0.2158,
    0.1903,
    0.1648,
    0.1392,
    0.1141,
    0.0893,
    0.0637,
    0.0379,
    0.0135,
   -0.0095,
   -0.0298,
   -0.0455,
   -0.0567,
   -0.0636,
   -0.0660,
   -0.0645,
   -0.0612,
   -0.0562,
   -0.0503,
   -0.0459,
   -0.0439,
   -0.0458,
   -0.0533,
   -0.0680,
   -0.0886,
   -0.1140,
   -0.1425,
   -0.1696,
   -0.1928,
   -0.2103,
   -0.2207,
   -0.2243,
   -0.2228,
   -0.2181,
   -0.2103,
   -0.2016,
   -0.1924,
   -0.1817,
   -0.1701,
   -0.1574,
   -0.1431,
   -0.1263,
   -0.1081,
   -0.0876,
   -0.0638,
   -0.0376,
   -0.0079,
    0.0263,
    0.0652,
    0.1084,
    0.1565,
    0.2078,
    0.2585,
    0.3067,
    0.3481,
    0.3788,
    0.3973,
    0.4038,
    0.3998,
    0.3883,
    0.3759,
    0.3662,
    0.3634,
    0.3712,
    0.3897,
    0.4175,
    0.4519,
    0.4899,
    0.5261,
    0.5576,
    0.5828,
    0.5990,
    0.6070,
    0.6078,
    0.6028,
    0.5928,
    0.5800,
    0.5657,
    0.5497,
    0.5349,
    0.5223,
    0.5132,
    0.5097,
    0.5135,
    0.5248,
    0.5422,
    0.5655,
    0.5912,
    0.6161,
    0.6389,
    0.6573,
    0.6703,
    0.6782,
    0.6823,
    0.6824,
    0.6796,
    0.6753,
    0.6683,
    0.6591,
    0.6476,
    0.6332,
    0.6146,
    0.5918,
    0.5642,
    0.5297,
    0.4891,
    0.4424,
    0.3893,
    0.3318,
    0.2718,
    0.2103,
    0.1487,
    0.0877,
    0.0272,
   -0.0346,
   -0.0980,
   -0.1634,
   -0.2322,
   -0.3034,
   -0.3750,
   -0.4473,
   -0.5180,
   -0.5855,
   -0.6495,
   -0.7086,
   -0.7620,
   -0.8085,
   -0.8490,
   -0.8824,
   -0.9081,
   -0.9280,
   -0.9409,
   -0.9474,
   -0.9490,
   -0.9467,
   -0.9413,
   -0.9344,
   -0.9280,
   -0.9205,
   -0.9116,
   -0.9001,
   -0.8832,
   -0.8590,
   -0.8275,
   -0.7894,
   -0.7444,
   -0.6972,
   -0.6505,
   -0.6055,
   -0.5670,
   -0.5357,
   -0.5123,
   -0.4956,
   -0.4844,
   -0.4758,
   -0.4643,
   -0.4499,
   -0.4286,
   -0.3985,
   -0.3618,
   -0.3177,
   -0.2679,
   -0.2139,
   -0.1586,
   -0.1033,
   -0.0495,
   -0.0018,
    0.0393,
    0.0726
};

static float f3Samples[] =
{
   -0.7666,
   -0.7486,
   -0.7269,
   -0.7038,
   -0.6822,
   -0.6641,
   -0.6502,
   -0.6414,
   -0.6346,
   -0.6261,
   -0.6130,
   -0.5916,
   -0.5596,
   -0.5183,
   -0.4700,
   -0.4177,
   -0.3667,
   -0.3201,
   -0.2795,
   -0.2454,
   -0.2168,
   -0.1901,
   -0.1639,
   -0.1365,
   -0.1060,
   -0.0733,
   -0.0393,
   -0.0044,
    0.0298,
    0.0613,
    0.0900,
    0.1143,
    0.1330,
    0.1472,
    0.1568,
    0.1622,
    0.1646,
    0.1647,
    0.1616,
    0.1569,
    0.1507,
    0.1433,
    0.1363,
    0.1305,
    0.1252,
    0.1201,
    0.1150,
    0.1074,
    0.0980,
    0.0868,
    0.0736,
    0.0595,
    0.0456,
    0.0312,
    0.0162,
    0.0015,
   -0.0150,
   -0.0329,
   -0.0528,
   -0.0762,
   -0.1044,
   -0.1374,
   -0.1763,
   -0.2212,
   -0.2688,
   -0.3178,
   -0.3637,
   -0.4018,
   -0.4281,
   -0.4406,
   -0.4370,
   -0.4188,
   -0.3900,
   -0.3535,
   -0.3150,
   -0.2790,
   -0.2480,
   -0.2232,
   -0.2060,
   -0.1939,
   -0.1854,
   -0.1794,
   -0.1729,
   -0.1646,
   -0.1533,
   -0.1379,
   -0.1162,
   -0.0888,
   -0.0544,
   -0.0134,
    0.0316,
    0.0780,
    0.1225,
    0.1608,
    0.1898,
    0.2099,
    0.2207,
    0.2255,
    0.2283,
    0.2317,
    0.2385,
    0.2504,
    0.2672,
    0.2872,
    0.3098,
    0.3316,
    0.3505,
    0.3649,
    0.3732,
    0.3749,
    0.3719,
    0.3666,
    0.3610,
    0.3601,
    0.3654,
    0.3786,
    0.4004,
    0.4306,
    0.4671,
    0.5095,
    0.5564,
    0.6053,
    0.6559,
    0.7060,
    0.7535,
    0.7969,
    0.8358,
    0.8682,
    0.8946,
    0.9157,
    0.9305,
    0.9398,
    0.9437,
    0.9413,
    0.9328,
    0.9192,
    0.8996,
    0.8756,
    0.8479,
    0.8175,
    0.7848,
    0.7515,
    0.7184,
    0.6846,
    0.6525,
    0.6194,
    0.5847,
    0.5469,
    0.5031,
    0.4509,
    0.3890,
    0.3174,
    0.2356,
    0.1484,
    0.0592,
   -0.0274,
   -0.1062,
   -0.1739,
   -0.2299,
   -0.2742,
   -0.3081,
   -0.3360,
   -0.3589,
   -0.3798,
   -0.4004,
   -0.4216,
   -0.4434,
   -0.4674,
   -0.4946,
   -0.5239,
   -0.5585,
   -0.5960,
   -0.6352,
   -0.6740,
   -0.7084,
   -0.7344,
   -0.7496,
   -0.7550,
   -0.7496,
   -0.7387,
   -0.7261,
   -0.7137,
   -0.7054,
   -0.7016
};

static float g3Samples[] =
{
   -0.5025,
   -0.4966,
   -0.4913,
   -0.4886,
   -0.4913,
   -0.4991,
   -0.5122,
   -0.5308,
   -0.5527,
   -0.5775,
   -0.6038,
   -0.6284,
   -0.6506,
   -0.6678,
   -0.6779,
   -0.6818,
   -0.6789,
   -0.6710,
   -0.6607,
   -0.6479,
   -0.6343,
   -0.6194,
   -0.6002,
   -0.5749,
   -0.5393,
   -0.4900,
   -0.4276,
   -0.3532,
   -0.2734,
   -0.1970,
   -0.1303,
   -0.0788,
   -0.0424,
   -0.0150,
    0.0095,
    0.0393,
    0.0799,
    0.1325,
    0.1979,
    0.2735,
    0.3551,
    0.4403,
    0.5236,
    0.6003,
    0.6670,
    0.7183,
    0.7528,
    0.7700,
    0.7706,
    0.7596,
    0.7421,
    0.7241,
    0.7133,
    0.7130,
    0.7257,
    0.7519,
    0.7870,
    0.8277,
    0.8688,
    0.9046,
    0.9333,
    0.9531,
    0.9641,
    0.9700,
    0.9722,
    0.9729,
    0.9738,
    0.9734,
    0.9711,
    0.9654,
    0.9531,
    0.9340,
    0.9064,
    0.8696,
    0.8254,
    0.7737,
    0.7167,
    0.6566,
    0.5936,
    0.5300,
    0.4673,
    0.4056,
    0.3472,
    0.2914,
    0.2369,
    0.1836,
    0.1289,
    0.0725,
    0.0149,
   -0.0448,
   -0.1050,
   -0.1652,
   -0.2254,
   -0.2835,
   -0.3396,
   -0.3943,
   -0.4477,
   -0.5030,
   -0.5608,
   -0.6196,
   -0.6770,
   -0.7257,
   -0.7596,
   -0.7748,
   -0.7689,
   -0.7460,
   -0.7123,
   -0.6742,
   -0.6395,
   -0.6114,
   -0.5903,
   -0.5757,
   -0.5638,
   -0.5518,
   -0.5378,
   -0.5185,
   -0.4931,
   -0.4597,
   -0.4156,
   -0.3609,
   -0.2947,
   -0.2185,
   -0.1373,
   -0.0555,
    0.0200,
    0.0829,
    0.1304,
    0.1604,
    0.1745,
    0.1775,
    0.1728,
    0.1653,
    0.1586,
    0.1523,
    0.1482,
    0.1435,
    0.1363,
    0.1255,
    0.1083,
    0.0841,
    0.0533,
    0.0159,
   -0.0257,
   -0.0682,
   -0.1108,
   -0.1498,
   -0.1856,
   -0.2188,
   -0.2491,
   -0.2787,
   -0.3075,
   -0.3346,
   -0.3605,
   -0.3838,
   -0.4034,
   -0.4206,
   -0.4330,
   -0.4414,
   -0.4460,
   -0.4456,
   -0.4425,
   -0.4370,
   -0.4294,
   -0.4217
};

static float a3Samples[] =
{
    0.0564,
    0.1064,
    0.1579,
    0.2054,
    0.2470,
    0.2792,
    0.3018,
    0.3155,
    0.3217,
    0.3256,
    0.3316,
    0.3477,
    0.3800,
    0.4302,
    0.4958,
    0.5647,
    0.6236,
    0.6584,
    0.6616,
    0.6347,
    0.5831,
    0.5178,
    0.4459,
    0.3721,
    0.2978,
    0.2211,
    0.1424,
    0.0605,
   -0.0229,
   -0.1055,
   -0.1867,
   -0.2640,
   -0.3390,
   -0.4115,
   -0.4822,
   -0.5515,
   -0.6163,
   -0.6759,
   -0.7263,
   -0.7668,
   -0.7981,
   -0.8219,
   -0.8431,
   -0.8625,
   -0.8808,
   -0.8947,
   -0.8997,
   -0.8948,
   -0.8790,
   -0.8571,
   -0.8331,
   -0.8113,
   -0.7957,
   -0.7865,
   -0.7853,
   -0.7888,
   -0.7935,
   -0.7949,
   -0.7874,
   -0.7707,
   -0.7440,
   -0.7112,
   -0.6763,
   -0.6424,
   -0.6134,
   -0.5882,
   -0.5664,
   -0.5438,
   -0.5163,
   -0.4825,
   -0.4394,
   -0.3889,
   -0.3304,
   -0.2647,
   -0.1948,
   -0.1231,
   -0.0576,
   -0.0031,
    0.0363,
    0.0624,
    0.0837,
    0.1091,
    0.1497,
    0.2102,
    0.2903,
    0.3851,
    0.4844,
    0.5811,
    0.6669,
    0.7381,
    0.7933,
    0.8315,
    0.8564,
    0.8698,
    0.8771,
    0.8827,
    0.8893,
    0.9000,
    0.9135,
    0.9297,
    0.9459,
    0.9585,
    0.9649,
    0.9601,
    0.9436,
    0.9142,
    0.8732,
    0.8228,
    0.7624,
    0.6937,
    0.6155,
    0.5294,
    0.4387,
    0.3466,
    0.2596,
    0.1808,
    0.1146,
    0.0632,
    0.0265,
    0.0041,
   -0.0096,
   -0.0190,
   -0.0307,
   -0.0494,
   -0.0755,
   -0.1085,
   -0.1428,
   -0.1736,
   -0.1959,
   -0.2062,
   -0.2058,
   -0.1969,
   -0.1858,
   -0.1775,
   -0.1745,
   -0.1782,
   -0.1849,
   -0.1937,
   -0.2017,
   -0.2075,
   -0.2094,
   -0.2016,
   -0.1810,
   -0.1431,
   -0.0901,
   -0.0295
};

static float b3Samples[] = 
{
   -0.8949,
   -0.9139,
   -0.9230,
   -0.9189,
   -0.8992,
   -0.8673,
   -0.8246,
   -0.7742,
   -0.7188,
   -0.6631,
   -0.6131,
   -0.5720,
   -0.5418,
   -0.5170,
   -0.4922,
   -0.4577,
   -0.4099,
   -0.3474,
   -0.2773,
   -0.2078,
   -0.1482,
   -0.1034,
   -0.0732,
   -0.0546,
   -0.0409,
   -0.0278,
   -0.0088,
    0.0169,
    0.0508,
    0.0875,
    0.1235,
    0.1549,
    0.1850,
    0.2209,
    0.2702,
    0.3357,
    0.4102,
    0.4839,
    0.5442,
    0.5870,
    0.6103,
    0.6188,
    0.6154,
    0.6054,
    0.5922,
    0.5797,
    0.5697,
    0.5620,
    0.5551,
    0.5422,
    0.5171,
    0.4690,
    0.3933,
    0.2883,
    0.1635,
    0.0322,
   -0.0874,
   -0.1822,
   -0.2450,
   -0.2774,
   -0.2886,
   -0.2883,
   -0.2866,
   -0.2856,
   -0.2864,
   -0.2844,
   -0.2772,
   -0.2598,
   -0.2305,
   -0.1877,
   -0.1329,
   -0.0702,
   -0.0044,
    0.0579,
    0.1138,
    0.1609,
    0.2041,
    0.2468,
    0.2957,
    0.3512,
    0.4127,
    0.4758,
    0.5369,
    0.5916,
    0.6349,
    0.6642,
    0.6789,
    0.6857,
    0.6905,
    0.6996,
    0.7101,
    0.7183,
    0.7181,
    0.7104,
    0.6996,
    0.6923,
    0.6926,
    0.6988,
    0.7056,
    0.7033,
    0.6856,
    0.6465,
    0.5876,
    0.5100,
    0.4192,
    0.3183,
    0.2131,
    0.1096,
    0.0160,
   -0.0590,
   -0.1110,
   -0.1406,
   -0.1587,
   -0.1787,
   -0.2162,
   -0.2766,
   -0.3589,
   -0.4519,
   -0.5442,
   -0.6260,
   -0.6928,
   -0.7455,
   -0.7865,
   -0.8206,
   -0.8495,
   -0.8769,
   -0.9027,
   -0.9293,
   -0.9550,
   -0.9783
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

static float * noteSamplesOctave3[] =
{   c3Samples,
    d3Samples,
    e3Samples,
    f3Samples,
    g3Samples,
    a3Samples,
    b3Samples,
    c4Samples
};

static float * noteSamplesOctave4[] =
{   c4Samples,
    d4Samples,
    e4Samples,
    f4Samples,
    g4Samples,
    a4Samples,
    b4Samples,
    c5Samples
};

static float ** noteSamples;

static bool currentKeyType;

const unsigned char dacMidLevel = 128;
const float samplingFrequency  = 32000;

float notesFrequenciesOctave4[] = 
{   
    261.63, /*C4*/
    293.66, /*D4*/
    329.63, /*E4*/
    349.23, /*F4*/
    392.00, /*G4*/
    440.00, /*A4*/
    493.88, /*B4*/
    523.25  /*C5*/
}; 

float notesFrequenciesOctave3[] = 
{   
    130.81, /*C3*/ 
    146.83, /*D3*/
    164.81, /*E3*/
    174.61, /*F3*/
    196.00, /*G3*/
    220.00, /*A3*/
    246.94, /*B3*/
    261.63, /*C4*/
}; 

static float * notesFrequencies;

static const float attackTargetFloatingPoint = 1;
static const float attackGainFloatingPoint   = 0.01;
static const float decayTargetFloatingPoint  = 0;
static const float decayGainFloatingPoint    = 0.00006;

static const int32_t attackTarget  =(int32_t)(attackTargetFloatingPoint     * FIXED_POINT_COEF);
static const int32_t attackGain    =(int32_t)( attackGainFloatingPoint      * FIXED_POINT_COEF);

static const int32_t decayTarget   =(int32_t)( decayTargetFloatingPoint     * FIXED_POINT_COEF);
/* Trocar valor de 0.00006 por 0.0001 para entrar na conversão de ponto fixo */
static const int32_t decayGain     =(int32_t)( 0.0001            * FIXED_POINT_COEF);
/* Valor usado caso se queira usar um ganho de decaimento menor que 0.0001 */
/* Número de vezes pelo qual o valor ADSR durante decaimento é o mesmo */
/* Na média, o ganho de decaimento será decayGain / decaySkipCount     */
static const unsigned int decaySkipCount = 1;

static const float   attackTime = 0.02;         /* unidade em segundos */
static const float   ADSRDecayMinimum = 0.01;   /* Valor minimo do ADSR durante decaimento antes de desligar tecla */

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
        sampleKeys[i].active = false;
        sampleKeys[i].periodCounter = 0;
        sampleKeys[i].periodSize = sampleSizes[i];
        fixedPointConvert(sampleKeys[i].samples, noteSamples[i], sampleSizes[i]);
        sampleKeys[i].samplesFloatingPoint = noteSamples[i];
        sampleKeys[i].amplitude = 0;
        sampleKeys[i].amplitudeFloatingPoint =0;
        sampleKeys[i].tickCounter = 0;
        sampleKeys[i].ADSRGain = 0;
        sampleKeys[i].ADSRGainFloatingPoint = 0;
    }
}

void Synth_Open(void)
{
    notesFrequencies = notesFrequenciesOctave4;
    noteSamples = noteSamplesOctave4;
    sampleSizes = sampleSizesOctave4;
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

void Synth_ChangeOctave(void)
{
    if (noteSamples == noteSamplesOctave4)
    {
        noteSamples = noteSamplesOctave3;
        notesFrequencies = notesFrequenciesOctave3;
        sampleSizes = sampleSizesOctave3;
    }
    else
    {
        noteSamples = noteSamplesOctave4;
        notesFrequencies = notesFrequenciesOctave4;
        sampleSizes = sampleSizesOctave4;
    }

    initKeys();
}

void Synth_Press(Note note)
{
    squareKeys[note].pressed = true;

    sampleKeys[note].pressed = true;
    sampleKeys[note].active  = true;
}

void Synth_Release(Note note)
{
    squareKeys[note].pressed = false;
    sampleKeys[note].pressed = false;
}


static int processSquareKeys (void)
{
    int squareSignalSumBuffer = 0;

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

    for (int i = 0; i < KEYS_SIZE; i++)
    {
        squareSignalSumBuffer += squareKeys[i].amplitude;
    }

    return squareSignalSumBuffer;
}

static void processSampleADSRFixedPoint (SampleWaveKey * key)
{
    /* Processar proximo valor do ADSR usando duas curvas exponenciais */
    /* Uma para ataque e outra para queda */
    float keyTime = key->tickCounter / samplingFrequency;
    int32_t auxiliar, oldADSRGain, releaseGain;

    oldADSRGain = key->ADSRGain;
    if (key->pressed && keyTime < attackTime) /* Ataque */
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
    else if (key->pressed && keyTime >= attackTime)               /* Decaimento */
    {
        /* Equação original */
        /* key->ADSRGain = decayTarget*decayGain + (1.0 - decayGain)*key->ADSRGain; */
        if (decaySkipCount > 1 && (key->tickCounter % decaySkipCount) != 0)
        {
            key->tickCounter++;
            return;
        }

        key->ADSRGain = decayTarget*decayGain;
        key->ADSRGain = key->ADSRGain / FIXED_POINT_COEF;

        auxiliar = (1 * FIXED_POINT_COEF) - decayGain;
        auxiliar = auxiliar * oldADSRGain;
        auxiliar = auxiliar / FIXED_POINT_COEF;

        key->ADSRGain = key->ADSRGain + auxiliar;
    }
    else if (!key->pressed)
    {
        /* Equação original */
        /* key->ADSRGain = decayTarget*decayGain + (1.0 - decayGain)*key->ADSRGain; */
        if (decaySkipCount > 1 && (key->tickCounter % decaySkipCount) != 0)
        {
            key->tickCounter++;
            return;
        }

        releaseGain = decayGain*10;

        key->ADSRGain = decayTarget*releaseGain;
        key->ADSRGain = key->ADSRGain / FIXED_POINT_COEF;

        auxiliar = (1 * FIXED_POINT_COEF) - releaseGain;
        auxiliar = auxiliar * oldADSRGain;
        auxiliar = auxiliar / FIXED_POINT_COEF;

        key->ADSRGain = key->ADSRGain + auxiliar;
    }

    key->tickCounter++;
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

static void resetSampleKey(SampleWaveKey * key)
{
    key->periodCounter = 0;
    key->amplitude = 0;
    key->amplitudeFloatingPoint = 0;
    key->ADSRGain = 0;
    key->ADSRGainFloatingPoint = 0;
    key->tickCounter = 0;
}

static int processSampleKeysFixedPoint (void)
{
    int32_t SampleSignalSumBuffer = 0;
    float   floatSampleSignalSumBuffer = 0;
    float   compressedSignalSumBuffer = 0;
    float   compressionCoefficient = 1;
    float   keyTime;
    int32_t fixedPointADSRDecayMinimum;

    int i = 0;
    for ( i = 0; i < KEYS_SIZE; i++)
    {
        if(!sampleKeys[i].active)
        {
            resetSampleKey(&sampleKeys[i]);
            continue;
        } 

        processSampleADSRFixedPoint(&sampleKeys[i]);
        sampleKeys[i].amplitude = sampleKeys[i].samples[sampleKeys[i].periodCounter];
        sampleKeys[i].amplitude *= sampleKeys[i].ADSRGain;
        sampleKeys[i].amplitude = sampleKeys[i].amplitude/FIXED_POINT_COEF;

        sampleKeys[i].periodCounter++;
        sampleKeys[i].periodCounter = sampleKeys[i].periodCounter % sampleKeys[i].periodSize;
    }

    for (int i = 0; i < KEYS_SIZE; i++)
    {
        SampleSignalSumBuffer += sampleKeys[i].amplitude;

        keyTime = sampleKeys[i].tickCounter / samplingFrequency;
        fixedPointADSRDecayMinimum = (int32_t)(ADSRDecayMinimum * FIXED_POINT_COEF);
        if (keyTime > attackTime && (sampleKeys[i].ADSRGain <= fixedPointADSRDecayMinimum) )
        {
            /* Desligar notas cujo volume já está baixo durante decaimento */
            sampleKeys[i].pressed = false;
            sampleKeys[i].active = false;
            resetSampleKey(&sampleKeys[i]);
        }
    }

    floatSampleSignalSumBuffer = ((float)SampleSignalSumBuffer)/FIXED_POINT_COEF;
    /* Dividir 1 por N^(4/5) para manter qualidade e evitar clipping*/
    /* Valor 4/5 obtido experimentalmente por Matlab */
    compressionCoefficient = 1.0/(powf(getActiveSampleKeys(), 4.0/5.0) ); 
    compressedSignalSumBuffer = (floatSampleSignalSumBuffer * 127.0) * compressionCoefficient;
    return ((int)compressedSignalSumBuffer);
}

static void processSampleADSRFloatingPoint (SampleWaveKey * key)
{
    /* Processar proximo valor do ADSR usando duas curvas exponenciais */
    /* Uma para ataque e outra para queda */
    float releaseGain;
    float keyTime = key->tickCounter / samplingFrequency;

    if (key->pressed && keyTime < attackTime) /* Ataque */
    {   
        key->ADSRGainFloatingPoint = attackTargetFloatingPoint*attackGainFloatingPoint + (1.0 - attackGainFloatingPoint)*(key->ADSRGainFloatingPoint);
    }
    else if (key->pressed && keyTime >= attackTime)               /* Decaimento */
    {
        /* Equação original */
        key->ADSRGainFloatingPoint = decayTargetFloatingPoint*decayGainFloatingPoint + (1.0 - decayGainFloatingPoint)*key->ADSRGainFloatingPoint;
    }
    else if (!key->pressed)
    {
        releaseGain = decayGainFloatingPoint*10;
        key->ADSRGainFloatingPoint = decayTargetFloatingPoint*releaseGain + (1.0 - releaseGain)*key->ADSRGainFloatingPoint;
    }

    key->tickCounter++;
}

static int processSampleKeysFloatingPoint (void)
{
    float   SampleSignalSumBuffer = 0;
    float   floatSampleSignalSumBuffer = 0;
    float   compressedSignalSumBuffer = 0;
    float   compressionCoefficient = 1;
    float   keyTime;

    int i = 0;
    for ( i = 0; i < KEYS_SIZE; i++)
    {
        if(!sampleKeys[i].active)
        {
            resetSampleKey(&sampleKeys[i]);
            continue;
        } 

        processSampleADSRFloatingPoint(&sampleKeys[i]);
        sampleKeys[i].amplitudeFloatingPoint = sampleKeys[i].samplesFloatingPoint[sampleKeys[i].periodCounter];
        sampleKeys[i].amplitudeFloatingPoint *= sampleKeys[i].ADSRGainFloatingPoint;

        sampleKeys[i].periodCounter++;
        sampleKeys[i].periodCounter = sampleKeys[i].periodCounter % sampleKeys[i].periodSize;
    }

    for (int i = 0; i < KEYS_SIZE; i++)
    {
        SampleSignalSumBuffer += sampleKeys[i].amplitudeFloatingPoint;

        keyTime = sampleKeys[i].tickCounter / samplingFrequency;
        if (keyTime > attackTime && (sampleKeys[i].ADSRGainFloatingPoint <= ADSRDecayMinimum) )
        {
            /* Desligar notas cujo volume já está baixo durante decaimento */
            sampleKeys[i].pressed = false;
            sampleKeys[i].active  = false;
            resetSampleKey(&sampleKeys[i]);
        }
    }

    floatSampleSignalSumBuffer = SampleSignalSumBuffer;
    /* Dividir 1 por N^(4/5) para manter qualidade e evitar clipping*/
    /* Valor 4/5 obtido experimentalmente por Matlab */
    compressionCoefficient = 1.0/(powf(getActiveSampleKeys(), 4.0/5.0) ); 
    compressedSignalSumBuffer = (floatSampleSignalSumBuffer * 127.0) * compressionCoefficient;
    return ((int)compressedSignalSumBuffer);
}

void Synth_Run(void)
{
    int signalSumBuffer = 0;
    bool isTimeForProcess = false;

    SynthTimer_DisableInterrupt();
    isTimeForProcess = processNextDAC;
    SynthTimer_EnableInterrupt();

    if (!isTimeForProcess) return;

    if (currentKeyType == sampleSignal)
    {
        if (pointOperation == fixedPoint)
        {
            signalSumBuffer = processSampleKeysFixedPoint();
        }
        else signalSumBuffer = processSampleKeysFloatingPoint();
        
    }
    else if (currentKeyType == squareSignal)
    {
        signalSumBuffer = processSquareKeys();
    }

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

void Synth_SetOperationType(OperationType type)
{
    pointOperation = type;
}