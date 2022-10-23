#ifndef _SOUND_TBL_H_
#define _SOUND_TBL_H_

static int timbre_table[8][256] = {
    {48,   167,  212,  220,  231,  239,  245,  247,  250,  253,  253,  256,  256,  256,  256,  256,  256,  256,  253,
     253,  253,  250,  242,  229,  218,  177,  129,  94,   64,   45,   29,   18,   8,    2,    0,    -4,   -7,   -7,
     -9,   -9,   -9,   -12,  -12,  -12,  -12,  -12,  -12,  -12,  -12,  -9,   -12,  -9,   -9,   -9,   -9,   -9,   -9,
     -9,   -9,   -9,   -9,   -9,   -9,   -9,   -7,   -7,   -7,   -7,   -7,   -7,   -7,   -7,   -7,   -7,   -4,   -4,
     -4,   -4,   -4,   -7,   -4,   -4,   -4,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   0,    -1,
     0,    0,    -1,   -1,   0,    18,   75,   161,  207,  215,  226,  231,  237,  239,  242,  245,  247,  247,  247,
     247,  247,  247,  247,  247,  247,  247,  247,  247,  245,  245,  239,  226,  215,  172,  115,  75,   43,   18,
     2,    -9,   -20,  -28,  -34,  -36,  -39,  -42,  -42,  -44,  -44,  -44,  -47,  -44,  -44,  -47,  -47,  -44,  -44,
     -44,  -44,  -44,  -44,  -44,  -44,  -44,  -42,  -42,  -42,  -42,  -39,  -42,  -39,  -39,  -39,  -39,  -39,  -39,
     -36,  -39,  -36,  -36,  -36,  -36,  -36,  -36,  -34,  -34,  -34,  -34,  -34,  -34,  -34,  -31,  -31,  -28,  5,
     78,   161,  204,  212,  220,  226,  231,  234,  237,  239,  239,  239,  242,  242,  242,  239,  234,  229,  220,
     215,  210,  204,  185,  153,  113,  72,   35,   10,   -12,  -28,  -47,  -122, -176, -190, -209, -219, -230, -238,
     -244, -249, -252, -252, -252, -252, -252, -249, -249, -249, -246, -244, -241, -238, -238, -236, -236, -233, -230,
     -228, -225, -225, -219, -222, -219, -214, -190, -74},

    {3,    93,   139,  174,  197,  213,  220,  220,  220,  217,  217,  213,  213,  213,  213,  217,  220,  223,  226,
     226,  226,  220,  210,  194,  178,  168,  158,  152,  142,  116,  61,   0,    -54,  -99,  -138, -164, -186, -180,
     -141, -96,  -54,  -15,  12,   35,   51,   42,   -2,   -50,  -102, -141, -173, -196, -206, -199, -151, -92,  -41,
     3,    35,   64,   84,   97,   100,  103,  103,  100,  100,  100,  100,  100,  103,  103,  103,  106,  110,  110,
     110,  97,   77,   55,   35,   19,   6,    0,    -5,   3,    22,   45,   68,   87,   100,  110,  113,  68,   -15,
     -102, -177, -206, -212, -219, -225, -225, -216, -180, -105, -50,  -5,   29,   55,   87,   123,  158,  184,  207,
     223,  236,  233,  178,  84,   -11,  -96,  -167, -206, -206, -216, -219, -225, -232, -238, -242, -248, -248, -251,
     -248, -235, -222, -164, -86,  -31,  16,   55,   93,   139,  178,  207,  233,  246,  249,  252,  252,  252,  252,
     252,  252,  252,  252,  252,  256,  246,  207,  158,  123,  90,   68,   51,   42,   51,   71,   90,   110,  123,
     136,  145,  145,  136,  119,  103,  87,   77,   68,   61,   64,   87,   119,  149,  174,  197,  213,  223,  223,
     204,  174,  142,  116,  97,   81,   68,   51,   25,   -2,   -31,  -54,  -70,  -83,  -92,  -83,  -37,  16,   68,
     113,  152,  178,  197,  213,  223,  233,  239,  243,  243,  243,  246,  236,  207,  171,  139,  110,  87,   71,
     58,   55,   64,   77,   90,   103,  113,  119,  123,  106,  64,   16,   -31,  -70,  -96,  -122, -138, -148, -154,
     -157, -157, -157, -157, -157, -157, -154, -125, -63},

    {14,   35,   50,   53,   59,   62,   62,   62,   65,   92,   154,  199,  211,  217,  226,  229,  232,  235,  238,
     241,  244,  247,  250,  250,  250,  250,  244,  232,  223,  214,  199,  169,  142,  122,  98,   80,   65,   53,
     44,   38,   35,   35,   38,   44,   50,   53,   56,   62,   59,   29,   -22,  -82,  -132, -171, -204, -216, -219,
     -216, -177, -129, -85,  -49,  -22,  -1,   14,   26,   38,   50,   56,   62,   65,   71,   71,   71,   74,   77,
     77,   77,   77,   77,   62,   8,    -67,  -153, -210, -222, -228, -234, -237, -240, -243, -246, -246, -249, -249,
     -249, -249, -246, -246, -162, 0,    142,  205,  214,  226,  235,  232,  226,  175,  80,   2,    -55,  -100, -144,
     -198, -216, -225, -234, -240, -243, -252, -249, -249, -204, -70,  53,   151,  205,  211,  220,  226,  235,  241,
     244,  250,  253,  253,  256,  247,  235,  211,  130,  50,   -4,   -52,  -100, -174, -216, -222, -234, -240, -246,
     -249, -255, -249, -240, -213, -144, -82,  -34,  2,    8,    -16,  -58,  -103, -141, -168, -192, -210, -204, -168,
     -118, -70,  -31,  0,    26,   44,   59,   71,   83,   92,   98,   101,  104,  107,  110,  110,  110,  113,  113,
     113,  110,  110,  110,  107,  104,  101,  101,  98,   95,   92,   92,   95,   95,   98,   101,  101,  104,  104,
     92,   71,   47,   23,   8,    -4,   -16,  -22,  -19,  -1,   17,   35,   50,   62,   71,   77,   95,   116,  142,
     163,  181,  193,  202,  205,  184,  136,  83,   35,   -4,   -34,  -58,  -79,  -129, -198, -219, -225, -234, -240,
     -246, -249, -249, -240, -228, -168, -94,  -43,  -1},

    {70,   182,  208,  220,  235,  241,  247,  244,  229,  220,  194,  135,  88,   52,   32,   32,   38,   47,   52,
     61,   64,   70,   73,   70,   67,   67,   67,   64,   64,   64,   64,   67,   70,   73,   76,   76,   76,   79,
     76,   67,   61,   55,   47,   44,   38,   35,   14,   -25,  -69,  -110, -143, -169, -187, -204, -210, -216, -225,
     -231, -237, -243, -249, -252, -252, -249, -240, -234, -228, -222, -216, -213, -213, -216, -222, -231, -237, -240,
     -243, -246, -246, -237, -228, -216, -184, -137, -104, -78,  -63,  -54,  -46,  -43,  -43,  -40,  -37,  -43,  -66,
     -104, -146, -184, -204, -207, -210, -210, -210, -213, -196, -146, -104, -72,  -43,  -22,  -4,   17,   44,   67,
     88,   105,  117,  126,  126,  94,   41,   -10,  -57,  -96,  -125, -146, -157, -137, -101, -60,  -25,  2,    23,
     41,   55,   70,   82,   97,   105,  111,  117,  123,  123,  123,  117,  111,  105,  102,  97,   94,   94,   97,
     102,  108,  114,  117,  120,  120,  120,  117,  111,  105,  100,  97,   91,   88,   88,   91,   97,   102,  105,
     111,  114,  114,  114,  111,  105,  100,  94,   91,   85,   82,   82,   85,   91,   97,   102,  105,  108,  108,
     108,  105,  100,  94,   88,   82,   79,   76,   76,   82,   88,   91,   97,   100,  102,  105,  105,  102,  100,
     94,   94,   91,   88,   88,   88,   91,   94,   100,  102,  102,  105,  105,  144,  197,  211,  220,  232,  241,
     247,  253,  256,  250,  235,  223,  203,  150,  102,  70,   52,   52,   58,   67,   76,   82,   85,   88,   64,
     14,   -40,  -90,  -131, -163, -190, -204, -152, -34},

    {5,    26,   43,   56,   80,   107,  134,  158,  177,  185,  188,  188,  188,  188,  188,  188,  185,  185,  180,
     177,  172,  164,  153,  148,  140,  134,  132,  129,  129,  129,  129,  126,  129,  129,  129,  123,  107,  83,
     59,   40,   24,   10,   0,    -9,   -23,  -42,  -60,  -77,  -87,  -98,  -106, -112, -122, -136, -149, -157, -166,
     -174, -176, -182, -184, -190, -195, -195, -195, -195, -195, -195, -195, -190, -184, -179, -174, -171, -166, -160,
     -152, -141, -131, -122, -114, -109, -106, -98,  -87,  -79,  -66,  -58,  -50,  -44,  -39,  -34,  -20,  -7,   5,
     16,   26,   32,   37,   45,   59,   72,   86,   97,   105,  110,  115,  123,  142,  164,  180,  188,  188,  191,
     191,  194,  194,  199,  202,  204,  207,  210,  210,  212,  215,  220,  226,  231,  234,  237,  237,  239,  242,
     245,  247,  250,  250,  253,  253,  253,  253,  253,  256,  256,  256,  256,  253,  253,  253,  250,  250,  247,
     245,  245,  245,  242,  237,  231,  229,  223,  218,  215,  212,  210,  207,  199,  194,  188,  185,  183,  180,
     175,  148,  110,  80,   56,   35,   21,   8,    -4,   -23,  -39,  -55,  -66,  -74,  -82,  -90,  -109, -141, -174,
     -201, -209, -209, -211, -214, -217, -219, -222, -228, -230, -233, -233, -236, -236, -238, -241, -244, -246, -246,
     -249, -249, -249, -249, -252, -252, -252, -252, -252, -252, -252, -252, -252, -252, -252, -252, -252, -249, -249,
     -249, -246, -246, -244, -244, -241, -241, -241, -238, -236, -233, -230, -228, -225, -222, -222, -219, -214, -209,
     -203, -201, -193, -179, -163, -128, -87,  -50,  -17},

    {2,    16,   24,   38,   68,   107,  143,  167,  173,  176,  176,  178,  173,  121,  55,   2,    -43,  -76,  -103,
     -122, -142, -158, -172, -183, -191, -197, -199, -202, -202, -202, -197, -197, -194, -191, -191, -188, -177, -169,
     -161, -153, -144, -142, -136, -142, -175, -208, -216, -219, -224, -227, -230, -230, -227, -224, -213, -183, -153,
     -133, -114, -92,  -59,  -23,  5,    33,   52,   68,   79,   88,   90,   90,   90,   90,   90,   88,   88,   85,
     77,   71,   66,   57,   55,   52,   49,   46,   41,   33,   27,   24,   19,   16,   16,   8,    0,    -4,   -12,
     -18,  -21,  -23,  -26,  -29,  -29,  -32,  -32,  -32,  -32,  -34,  -32,  -32,  -34,  -34,  -34,  -34,  -34,  -34,
     -34,  -37,  -34,  -34,  -34,  -34,  -34,  -34,  -34,  -32,  -4,   44,   93,   140,  170,  176,  178,  184,  187,
     192,  200,  209,  214,  220,  222,  225,  228,  233,  239,  244,  247,  253,  253,  256,  256,  250,  242,  236,
     228,  225,  220,  217,  211,  203,  198,  189,  181,  176,  173,  167,  132,  68,   8,    -40,  -81,  -114, -139,
     -158, -188, -216, -224, -227, -232, -232, -235, -238, -238, -241, -246, -249, -252, -255, -255, -255, -255, -246,
     -235, -227, -197, -158, -133, -109, -89,  -73,  -56,  -45,  -34,  -29,  -23,  -21,  -21,  -21,  -23,  -26,  -26,
     -29,  -29,  -32,  -37,  -48,  -56,  -62,  -67,  -73,  -78,  -81,  -84,  -89,  -92,  -98,  -100, -100, -103, -103,
     -103, -103, -103, -103, -103, -103, -103, -103, -103, -106, -109, -109, -111, -111, -111, -111, -111, -111, -111,
     -111, -111, -109, -109, -100, -78,  -54,  -29,  -12},

    {2,    10,   21,   32,   53,   83,   115,  142,  164,  180,  194,  202,  202,  202,  202,  202,  202,  202,  202,
     202,  202,  204,  207,  210,  212,  212,  215,  215,  218,  220,  223,  226,  226,  229,  229,  231,  231,  234,
     237,  239,  239,  242,  242,  242,  245,  245,  247,  250,  250,  253,  253,  253,  253,  256,  256,  256,  256,
     256,  256,  256,  256,  256,  256,  256,  256,  256,  256,  256,  256,  256,  256,  253,  253,  253,  253,  253,
     253,  253,  250,  250,  250,  247,  247,  247,  247,  245,  245,  242,  242,  239,  239,  239,  237,  234,  234,
     231,  229,  229,  229,  226,  223,  223,  220,  218,  215,  212,  212,  210,  210,  207,  204,  202,  199,  196,
     196,  194,  194,  191,  188,  185,  183,  180,  180,  169,  145,  115,  91,   72,   53,   40,   32,   18,   -1,
     -23,  -42,  -58,  -71,  -79,  -87,  -96,  -104, -114, -122, -128, -133, -139, -141, -152, -176, -198, -203, -206,
     -209, -209, -211, -211, -214, -217, -219, -222, -225, -225, -225, -228, -228, -230, -233, -236, -238, -238, -238,
     -241, -241, -244, -244, -246, -246, -249, -249, -249, -249, -252, -252, -252, -252, -252, -252, -252, -252, -252,
     -252, -252, -252, -252, -252, -252, -252, -252, -252, -252, -249, -249, -249, -249, -249, -246, -246, -246, -246,
     -246, -244, -244, -244, -241, -241, -241, -238, -238, -236, -236, -236, -233, -230, -230, -228, -228, -225, -225,
     -222, -219, -217, -217, -214, -211, -209, -209, -206, -203, -201, -198, -195, -193, -193, -190, -187, -184, -179,
     -163, -147, -133, -125, -112, -90,  -63,  -36,  -15},

    {0,    60,   118,  128,  140,  152,  161,  167,  170,  176,  185,  195,  204,  210,  216,  219,  222,  225,  231,
     237,  240,  246,  249,  249,  252,  252,  256,  256,  256,  256,  256,  256,  256,  256,  252,  249,  246,  243,
     240,  240,  237,  234,  228,  219,  213,  207,  201,  198,  195,  188,  179,  170,  161,  155,  149,  146,  143,
     140,  134,  128,  124,  121,  118,  115,  112,  82,   48,   21,   -5,   -26,  -41,  -50,  -66,  -87,  -111, -133,
     -148, -163, -172, -181, -187, -197, -203, -206, -212, -215, -218, -218, -218, -215, -212, -206, -203, -200, -200,
     -197, -194, -191, -184, -181, -178, -175, -172, -169, -169, -166, -166, -166, -166, -166, -166, -163, -166, -172,
     -175, -181, -184, -187, -191, -191, -197, -200, -206, -209, -215, -218, -218, -221, -224, -233, -242, -245, -245,
     -248, -248, -248, -248, -248, -251, -251, -251, -251, -255, -255, -255, -255, -255, -255, -255, -255, -255, -255,
     -255, -255, -255, -255, -255, -255, -255, -255, -255, -255, -255, -255, -255, -255, -255, -255, -255, -251, -251,
     -251, -251, -251, -251, -251, -248, -248, -248, -248, -248, -248, -248, -248, -248, -245, -245, -245, -245, -245,
     -245, -245, -245, -242, -242, -242, -242, -242, -242, -242, -242, -242, -242, -242, -242, -242, -242, -242, -242,
     -242, -242, -239, -239, -239, -239, -239, -239, -239, -239, -239, -239, -236, -236, -236, -236, -236, -236, -236,
     -236, -236, -236, -236, -233, -233, -233, -233, -233, -233, -233, -233, -230, -227, -221, -215, -212, -206, -203,
     -200, -191, -163, -130, -105, -78,  -53,  -35,  -20}};

static int noise0_table[256] = {
    -229, -47,  92,   240,  256,  201,  227,  176,  211,  163,  70,   220,  240,  147,  160,  115,  192,  224,  131,
    112,  80,   214,  112,  86,   64,   -213, -159, 0,    163,  211,  204,  73,   -37,  -210, -242, -245, -239, -184,
    -66,  76,   227,  224,  124,  -53,  -210, -239, -248, -226, -66,  51,   224,  240,  121,  150,  80,   -229, -117,
    22,   217,  236,  102,  140,  83,   -50,  -63,  217,  227,  204,  9,    -127, -235, -242, -235, -155, 3,    195,
    188,  -229, -232, -226, -15,  115,  236,  243,  233,  176,  60,   -149, -232, -232, -216, -59,  16,   208,  252,
    240,  160,  22,   -139, -223, -235, -203, 9,    160,  243,  246,  185,  204,  57,   -226, -120, 12,   80,   243,
    243,  236,  70,   -56,  -223, -235, -219, -56,  83,   236,  252,  179,  204,  160,  22,   227,  236,  227,  70,
    -69,  -219, -232, -232, -120, 35,   224,  243,  195,  182,  147,  112,  147,  144,  -171, -235, -223, -75,  3,
    182,  236,  236,  150,  -8,   -194, -232, -229, -203, -31,  115,  233,  243,  147,  179,  99,   -50,  57,   227,
    233,  112,  150,  64,   -226, -242, -213, -226, -235, -213, 41,   156,  236,  246,  236,  150,  57,   -181, -226,
    -226, -155, 9,    35,   240,  246,  236,  128,  -8,   -210, -229, -219, -95,  67,   230,  246,  240,  195,  172,
    233,  233,  153,  0,    -171, -223, -235, -184, 9,    163,  233,  236,  182,  176,  192,  233,  233,  214,  86,
    -15,  -216, -239, -229, -162, -11,  22,   236,  243,  224,  51,   -75,  -219, -229, -229, -79,  57,   230,  243,
    179,  179,  144,  12,   227,  230,  220,  57,   -75};

static int lupin3_table[256] = {
    12,   -6,   -48,  -90,  -120, -144, -168, -180, -186, -192, -192, -198, -204, -204, -192, -198, -198, -198, -198,
    -192, -186, -186, -192, -192, -186, -186, -186, -174, -174, -174, -174, -174, -168, -126, -48,  30,   84,   138,
    168,  192,  216,  222,  228,  222,  204,  162,  126,  102,  84,   72,   66,   54,   48,   48,   42,   42,   42,
    42,   36,   42,   42,   42,   36,   36,   36,   36,   30,   24,   18,   12,   18,   12,   12,   12,   6,    6,
    6,    6,    6,    6,    12,   6,    6,    12,   6,    6,    0,    -42,  -78,  -108, -126, -144, -156, -162, -174,
    -174, -156, -102, -18,  48,   108,  150,  180,  204,  210,  216,  222,  228,  234,  234,  234,  228,  234,  234,
    228,  228,  222,  222,  228,  228,  222,  216,  216,  222,  216,  216,  216,  210,  198,  168,  114,  78,   48,
    24,   12,   0,    -6,   -12,  -12,  -12,  -18,  -18,  -12,  -18,  -18,  -12,  -18,  -18,  -18,  -18,  -18,  -24,
    -18,  -24,  -24,  -24,  -18,  -24,  -18,  -18,  -18,  -12,  -6,   0,    0,    0,    0,    0,    0,    0,    0,
    24,   60,   96,   120,  144,  162,  174,  174,  180,  180,  186,  192,  192,  186,  186,  186,  186,  186,  186,
    186,  180,  150,  78,   -6,   -66,  -120, -156, -180, -204, -210, -210, -222, -228, -234, -228, -228, -228, -228,
    -228, -228, -222, -222, -216, -186, -144, -102, -78,  -60,  -42,  -36,  -24,  -18,  -18,  -12,  -6,   0,    0,
    6,    6,    12,   12,   18,   18,   12,   12,   12,   12,   12,   6,    12,   24,   18,   18,   12,   12,   12,
    12,   12,   12,   12,   12,   12,   12,   12,   12};

static double detune_rate[32] = {100.0, 97.8, 91.7, 91.3, 88.1, 85.2, 82.1, 79.2, 75.5, 72.4, 69.0,
                                 66.3,  63.0, 60.0, 56.5, 53.6, 50.7, 47.4, 44.3, 41.4, 38.0, 34.8,
                                 31.6,  28.6, 25.3, 22.3, 19.0, 15.9, 12.6, 9.4,  6.3,  3.2};

#endif
