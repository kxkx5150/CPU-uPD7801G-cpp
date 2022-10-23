#include "upd7801.h"
#include <cstdio>


#define ZHC_ADD(a, b, c)                                                                                               \
    {                                                                                                                  \
        if (a)                                                                                                         \
            PSW &= ~F_Z;                                                                                               \
        else                                                                                                           \
            PSW |= F_Z;                                                                                                \
        if (a == b)                                                                                                    \
            PSW = (PSW & ~F_CY) | (c);                                                                                 \
        else if (a < b)                                                                                                \
            PSW |= F_CY;                                                                                               \
        else                                                                                                           \
            PSW &= ~F_CY;                                                                                              \
        if ((a & 15) < (b & 15))                                                                                       \
            PSW |= F_HC;                                                                                               \
        else                                                                                                           \
            PSW &= ~F_HC;                                                                                              \
    }
#define ZHC_SUB(a, b, c)                                                                                               \
    {                                                                                                                  \
        if (a)                                                                                                         \
            PSW &= ~F_Z;                                                                                               \
        else                                                                                                           \
            PSW |= F_Z;                                                                                                \
        if (a == b)                                                                                                    \
            PSW = (PSW & ~F_CY) | (c);                                                                                 \
        else if (a > b)                                                                                                \
            PSW |= F_CY;                                                                                               \
        else                                                                                                           \
            PSW &= ~F_CY;                                                                                              \
        if ((a & 15) > (b & 15))                                                                                       \
            PSW |= F_HC;                                                                                               \
        else                                                                                                           \
            PSW &= ~F_HC;                                                                                              \
    }
#define SET_Z(n)                                                                                                       \
    {                                                                                                                  \
        if (n)                                                                                                         \
            PSW &= ~F_Z;                                                                                               \
        else                                                                                                           \
            PSW |= F_Z;                                                                                                \
    }
#define SKIP_CY                                                                                                        \
    {                                                                                                                  \
        if (PSW & F_CY)                                                                                                \
            PSW |= F_SK;                                                                                               \
    }
#define SKIP_NC                                                                                                        \
    {                                                                                                                  \
        if (!(PSW & F_CY))                                                                                             \
            PSW |= F_SK;                                                                                               \
    }
#define SKIP_Z                                                                                                         \
    {                                                                                                                  \
        if (PSW & F_Z)                                                                                                 \
            PSW |= F_SK;                                                                                               \
    }
#define SKIP_NZ                                                                                                        \
    {                                                                                                                  \
        if (!(PSW & F_Z))                                                                                              \
            PSW |= F_SK;                                                                                               \
    }
#define ACI(r)                                                                                                         \
    {                                                                                                                  \
        uint8 tmp = r + FETCH8() + (PSW & F_CY);                                                                       \
        ZHC_ADD(tmp, r, (PSW & F_CY));                                                                                 \
        r = tmp;                                                                                                       \
    }
#define ACI_IO(p)                                                                                                      \
    {                                                                                                                  \
        uint8 old = IN8(p);                                                                                            \
        uint8 tmp = old + FETCH8() + (PSW & F_CY);                                                                     \
        ZHC_ADD(tmp, old, (PSW & F_CY));                                                                               \
        OUT8(p, tmp);                                                                                                  \
    }
#define ADC(r, n)                                                                                                      \
    {                                                                                                                  \
        uint8 tmp = r + n + (PSW & F_CY);                                                                              \
        ZHC_ADD(tmp, r, (PSW & F_CY));                                                                                 \
        r = tmp;                                                                                                       \
    }
#define ADCW()                                                                                                         \
    {                                                                                                                  \
        uint8 tmp = _A + RM8(FETCHWA()) + (PSW & F_CY);                                                                \
        ZHC_ADD(tmp, _A, (PSW & F_CY));                                                                                \
        _A = tmp;                                                                                                      \
    }
#define ADCX(r)                                                                                                        \
    {                                                                                                                  \
        uint8 tmp = _A + RM8(r) + (PSW & F_CY);                                                                        \
        ZHC_ADD(tmp, _A, (PSW & F_CY));                                                                                \
        _A = tmp;                                                                                                      \
    }
#define ADD(r, n)                                                                                                      \
    {                                                                                                                  \
        uint8 tmp = r + n;                                                                                             \
        ZHC_ADD(tmp, r, 0);                                                                                            \
        r = tmp;                                                                                                       \
    }
#define ADDNC(r, n)                                                                                                    \
    {                                                                                                                  \
        uint8 tmp = r + n;                                                                                             \
        ZHC_ADD(tmp, r, 0);                                                                                            \
        r = tmp;                                                                                                       \
        SKIP_NC;                                                                                                       \
    }
#define ADDNCW()                                                                                                       \
    {                                                                                                                  \
        uint8 tmp = _A + RM8(FETCHWA());                                                                               \
        ZHC_ADD(tmp, _A, 0);                                                                                           \
        _A = tmp;                                                                                                      \
        SKIP_NC;                                                                                                       \
    }
#define ADDNCX(r)                                                                                                      \
    {                                                                                                                  \
        uint8 tmp = _A + RM8(r);                                                                                       \
        ZHC_ADD(tmp, _A, 0);                                                                                           \
        _A = tmp;                                                                                                      \
        SKIP_NC;                                                                                                       \
    }
#define ADDW()                                                                                                         \
    {                                                                                                                  \
        uint8 tmp = _A + RM8(FETCHWA());                                                                               \
        ZHC_ADD(tmp, _A, 0);                                                                                           \
        _A = tmp;                                                                                                      \
    }
#define ADDX(r)                                                                                                        \
    {                                                                                                                  \
        uint8 tmp = _A + RM8(r);                                                                                       \
        ZHC_ADD(tmp, _A, 0);                                                                                           \
        _A = tmp;                                                                                                      \
    }
#define ADI(r)                                                                                                         \
    {                                                                                                                  \
        uint8 tmp = r + FETCH8();                                                                                      \
        ZHC_ADD(tmp, r, 0);                                                                                            \
        r = tmp;                                                                                                       \
    }
#define ADI_IO(p)                                                                                                      \
    {                                                                                                                  \
        uint8 old = IN8(p);                                                                                            \
        uint8 tmp = old + FETCH8();                                                                                    \
        ZHC_ADD(tmp, old, 0);                                                                                          \
        OUT8(p, tmp);                                                                                                  \
    }
#define ADINC(r)                                                                                                       \
    {                                                                                                                  \
        uint8 tmp = r + FETCH8();                                                                                      \
        ZHC_ADD(tmp, r, 0);                                                                                            \
        r = tmp;                                                                                                       \
        SKIP_NC;                                                                                                       \
    }
#define ADINC_IO(p)                                                                                                    \
    {                                                                                                                  \
        uint8 old = IN8(p);                                                                                            \
        uint8 tmp = old + FETCH8();                                                                                    \
        ZHC_ADD(tmp, old, 0);                                                                                          \
        OUT8(p, tmp);                                                                                                  \
        SKIP_NC;                                                                                                       \
    }
#define ANA(r, n)                                                                                                      \
    {                                                                                                                  \
        r &= n;                                                                                                        \
        SET_Z(r);                                                                                                      \
    }
#define ANAW()                                                                                                         \
    {                                                                                                                  \
        _A &= RM8(FETCHWA());                                                                                          \
        SET_Z(_A);                                                                                                     \
    }
#define ANAX(r)                                                                                                        \
    {                                                                                                                  \
        _A &= RM8(r);                                                                                                  \
        SET_Z(_A);                                                                                                     \
    }
#define ANI(r)                                                                                                         \
    {                                                                                                                  \
        r &= FETCH8();                                                                                                 \
        SET_Z(r);                                                                                                      \
    }
#define ANI_IO(p)                                                                                                      \
    {                                                                                                                  \
        uint8 tmp = IN8(p) & FETCH8();                                                                                 \
        OUT8(p, tmp);                                                                                                  \
        SET_Z(tmp);                                                                                                    \
    }
#define ANIW()                                                                                                         \
    {                                                                                                                  \
        uint16 dst = FETCHWA();                                                                                        \
        uint8  tmp = RM8(dst) & FETCH8();                                                                              \
        WM8(dst, tmp);                                                                                                 \
        SET_Z(tmp);                                                                                                    \
    }
#define BIT(b)                                                                                                         \
    {                                                                                                                  \
        if (RM8(FETCHWA()) & (1 << b))                                                                                 \
            PSW |= F_SK;                                                                                               \
    }
#define BLOCK()                                                                                                        \
    {                                                                                                                  \
        WM8(DE++, RM8(HL++));                                                                                          \
        if (_C--) {                                                                                                    \
            PSW &= ~F_CY;                                                                                              \
            PC--;                                                                                                      \
        } else                                                                                                         \
            PSW |= F_CY;                                                                                               \
    }
#define CALF(o)                                                                                                        \
    {                                                                                                                  \
        uint16 dst = 0x800 + ((o & 7) << 8) + FETCH8();                                                                \
        PUSH16(PC);                                                                                                    \
        PC = dst;                                                                                                      \
    }
#define CALL()                                                                                                         \
    {                                                                                                                  \
        uint16 dst = FETCH16();                                                                                        \
        PUSH16(PC);                                                                                                    \
        PC = dst;                                                                                                      \
    }
#define CALT(o)                                                                                                        \
    {                                                                                                                  \
        uint16 dst = RM16(0x80 + ((o & 0x3f) << 1));                                                                   \
        PUSH16(PC);                                                                                                    \
        PC = dst;                                                                                                      \
    }
#define DAA()                                                                                                          \
    {                                                                                                                  \
        uint8 lo = _A & 0xf, hi = _A >> 4, diff = 0;                                                                   \
        if (lo <= 9 && !(PSW & F_HC))                                                                                  \
            diff = (hi >= 10 || (PSW & F_CY)) ? 0x60 : 0x00;                                                           \
        else if (lo >= 10 && !(PSW & F_HC))                                                                            \
            diff = (hi >= 9 || (PSW & F_CY)) ? 0x66 : 0x06;                                                            \
        else if (lo <= 2 && (PSW & F_HC))                                                                              \
            diff = (hi >= 10 || (PSW & F_CY)) ? 0x66 : 0x06;                                                           \
        _A += diff;                                                                                                    \
        if (_A)                                                                                                        \
            PSW &= ~F_Z;                                                                                               \
        else                                                                                                           \
            PSW |= F_Z;                                                                                                \
        if ((PSW & F_CY) || (lo <= 9 ? hi >= 10 : hi >= 9))                                                            \
            PSW |= F_CY;                                                                                               \
        else                                                                                                           \
            PSW &= ~F_CY;                                                                                              \
        if (lo >= 10)                                                                                                  \
            PSW |= F_HC;                                                                                               \
        else                                                                                                           \
            PSW &= ~F_HC;                                                                                              \
    }
#define DCR(r)                                                                                                         \
    {                                                                                                                  \
        uint8 carry = PSW & F_CY;                                                                                      \
        uint8 tmp   = r - 1;                                                                                           \
        ZHC_SUB(tmp, r, 0);                                                                                            \
        r = tmp;                                                                                                       \
        SKIP_CY;                                                                                                       \
        PSW = (PSW & ~F_CY) | carry;                                                                                   \
    }
#define DCRW()                                                                                                         \
    {                                                                                                                  \
        uint8  carry = PSW & F_CY;                                                                                     \
        uint16 dst   = FETCHWA();                                                                                      \
        uint8  old   = RM8(dst);                                                                                       \
        uint8  tmp   = old - 1;                                                                                        \
        ZHC_SUB(tmp, old, 0);                                                                                          \
        WM8(dst, tmp);                                                                                                 \
        SKIP_CY;                                                                                                       \
        PSW = (PSW & ~F_CY) | carry;                                                                                   \
    }
#define EQA(r, n)                                                                                                      \
    {                                                                                                                  \
        uint8 tmp = r - n;                                                                                             \
        ZHC_SUB(tmp, r, 0);                                                                                            \
        SKIP_Z;                                                                                                        \
    }
#define EQAW()                                                                                                         \
    {                                                                                                                  \
        uint8 tmp = _A - RM8(FETCHWA());                                                                               \
        ZHC_SUB(tmp, _A, 0);                                                                                           \
        SKIP_Z;                                                                                                        \
    }
#define EQAX(r)                                                                                                        \
    {                                                                                                                  \
        uint8 tmp = _A - RM8(r);                                                                                       \
        ZHC_SUB(tmp, _A, 0);                                                                                           \
        SKIP_Z;                                                                                                        \
    }
#define EQI(r)                                                                                                         \
    {                                                                                                                  \
        uint8 tmp = r - FETCH8();                                                                                      \
        ZHC_SUB(tmp, r, 0);                                                                                            \
        SKIP_Z;                                                                                                        \
    }
#define EQI_IO(p)                                                                                                      \
    {                                                                                                                  \
        uint8 old = IN8(p);                                                                                            \
        uint8 tmp = old - FETCH8();                                                                                    \
        ZHC_SUB(tmp, old, 0);                                                                                          \
        SKIP_Z;                                                                                                        \
    }
#define EQIW()                                                                                                         \
    {                                                                                                                  \
        uint8 old = RM8(FETCHWA());                                                                                    \
        uint8 tmp = old - FETCH8();                                                                                    \
        ZHC_SUB(tmp, old, 0);                                                                                          \
        SKIP_Z;                                                                                                        \
    }
#define EX()                                                                                                           \
    {                                                                                                                  \
        uint16 tmp;                                                                                                    \
        tmp   = VA;                                                                                                    \
        VA    = altVA;                                                                                                 \
        altVA = tmp;                                                                                                   \
    }
#define EXX()                                                                                                          \
    {                                                                                                                  \
        uint16 tmp;                                                                                                    \
        tmp   = BC;                                                                                                    \
        BC    = altBC;                                                                                                 \
        altBC = tmp;                                                                                                   \
        tmp   = DE;                                                                                                    \
        DE    = altDE;                                                                                                 \
        altDE = tmp;                                                                                                   \
        tmp   = HL;                                                                                                    \
        HL    = altHL;                                                                                                 \
        altHL = tmp;                                                                                                   \
    }
#define GTA(r, n)                                                                                                      \
    {                                                                                                                  \
        uint8 tmp = r - n - 1;                                                                                         \
        ZHC_SUB(tmp, r, 1);                                                                                            \
        SKIP_NC;                                                                                                       \
    }
#define GTAW()                                                                                                         \
    {                                                                                                                  \
        uint8 tmp = _A - RM8(FETCHWA()) - 1;                                                                           \
        ZHC_SUB(tmp, _A, 1);                                                                                           \
        SKIP_NC;                                                                                                       \
    }
#define GTAX(r)                                                                                                        \
    {                                                                                                                  \
        uint8 tmp = _A - RM8(r) - 1;                                                                                   \
        ZHC_SUB(tmp, _A, 1);                                                                                           \
        SKIP_NC;                                                                                                       \
    }
#define GTI(r)                                                                                                         \
    {                                                                                                                  \
        uint8 tmp = r - FETCH8() - 1;                                                                                  \
        ZHC_SUB(tmp, r, 1);                                                                                            \
        SKIP_NC;                                                                                                       \
    }
#define GTI_IO(p)                                                                                                      \
    {                                                                                                                  \
        uint8 old = IN8(p);                                                                                            \
        uint8 tmp = old - FETCH8() - 1;                                                                                \
        ZHC_SUB(tmp, old, 1);                                                                                          \
        SKIP_NC;                                                                                                       \
    }
#define GTIW()                                                                                                         \
    {                                                                                                                  \
        uint8 old = RM8(FETCHWA());                                                                                    \
        uint8 tmp = old - FETCH8() - 1;                                                                                \
        ZHC_SUB(tmp, old, 1);                                                                                          \
        SKIP_NC;                                                                                                       \
    }
#define INR(r)                                                                                                         \
    {                                                                                                                  \
        uint8 carry = PSW & F_CY;                                                                                      \
        uint8 tmp   = r + 1;                                                                                           \
        ZHC_ADD(tmp, r, 0);                                                                                            \
        r = tmp;                                                                                                       \
        SKIP_CY;                                                                                                       \
        PSW = (PSW & ~F_CY) | carry;                                                                                   \
    }
#define INRW()                                                                                                         \
    {                                                                                                                  \
        uint8  carry = PSW & F_CY;                                                                                     \
        uint16 dst   = FETCHWA();                                                                                      \
        uint8  old   = RM8(dst);                                                                                       \
        uint8  tmp   = old + 1;                                                                                        \
        ZHC_ADD(tmp, old, 0);                                                                                          \
        WM8(dst, tmp);                                                                                                 \
        SKIP_CY;                                                                                                       \
        PSW = (PSW & ~F_CY) | carry;                                                                                   \
    }
#define JRE(o)                                                                                                         \
    {                                                                                                                  \
        uint8 tmp = FETCH8();                                                                                          \
        if (o & 1)                                                                                                     \
            PC -= 256 - tmp;                                                                                           \
        else                                                                                                           \
            PC += tmp;                                                                                                 \
    }
#define LTA(r, n)                                                                                                      \
    {                                                                                                                  \
        uint8 tmp = r - n;                                                                                             \
        ZHC_SUB(tmp, r, 0);                                                                                            \
        SKIP_CY;                                                                                                       \
    }
#define LTAW()                                                                                                         \
    {                                                                                                                  \
        uint8 tmp = _A - RM8(FETCHWA());                                                                               \
        ZHC_SUB(tmp, _A, 0);                                                                                           \
        SKIP_CY;                                                                                                       \
    }
#define LTAX(r)                                                                                                        \
    {                                                                                                                  \
        uint8 tmp = _A - RM8(r);                                                                                       \
        ZHC_SUB(tmp, _A, 0);                                                                                           \
        SKIP_CY;                                                                                                       \
    }
#define LTI(r)                                                                                                         \
    {                                                                                                                  \
        uint8 tmp = r - FETCH8();                                                                                      \
        ZHC_SUB(tmp, r, 0);                                                                                            \
        SKIP_CY;                                                                                                       \
    }
#define LTI_IO(p)                                                                                                      \
    {                                                                                                                  \
        uint8 old = IN8(p);                                                                                            \
        uint8 tmp = old - FETCH8();                                                                                    \
        ZHC_SUB(tmp, old, 0);                                                                                          \
        SKIP_CY;                                                                                                       \
    }
#define LTIW()                                                                                                         \
    {                                                                                                                  \
        uint8 old = RM8(FETCHWA());                                                                                    \
        uint8 tmp = old - FETCH8();                                                                                    \
        ZHC_SUB(tmp, old, 0);                                                                                          \
        SKIP_CY;                                                                                                       \
    }
#define MVIW()                                                                                                         \
    {                                                                                                                  \
        uint16 dst = FETCHWA();                                                                                        \
        WM8(dst, FETCH8());                                                                                            \
    }
#define NEA(r, n)                                                                                                      \
    {                                                                                                                  \
        uint8 tmp = r - n;                                                                                             \
        ZHC_SUB(tmp, r, 0);                                                                                            \
        SKIP_NZ;                                                                                                       \
    }
#define NEAW()                                                                                                         \
    {                                                                                                                  \
        uint8 tmp = _A - RM8(FETCHWA());                                                                               \
        ZHC_SUB(tmp, _A, 0);                                                                                           \
        SKIP_NZ;                                                                                                       \
    }
#define NEAX(r)                                                                                                        \
    {                                                                                                                  \
        uint8 tmp = _A - RM8(r);                                                                                       \
        ZHC_SUB(tmp, _A, 0);                                                                                           \
        SKIP_NZ;                                                                                                       \
    }
#define NEI(r)                                                                                                         \
    {                                                                                                                  \
        uint8 tmp = r - FETCH8();                                                                                      \
        ZHC_SUB(tmp, r, 0);                                                                                            \
        SKIP_NZ;                                                                                                       \
    }
#define NEI_IO(p)                                                                                                      \
    {                                                                                                                  \
        uint8 old = IN8(p);                                                                                            \
        uint8 tmp = old - FETCH8();                                                                                    \
        ZHC_SUB(tmp, old, 0);                                                                                          \
        SKIP_NZ;                                                                                                       \
    }
#define NEIW()                                                                                                         \
    {                                                                                                                  \
        uint8 old = RM8(FETCHWA());                                                                                    \
        uint8 tmp = old - FETCH8();                                                                                    \
        ZHC_SUB(tmp, old, 0);                                                                                          \
        SKIP_NZ;                                                                                                       \
    }
#define OFFA(r, n)                                                                                                     \
    {                                                                                                                  \
        if (r & n)                                                                                                     \
            PSW &= ~F_Z;                                                                                               \
        else                                                                                                           \
            PSW |= F_Z | F_SK;                                                                                         \
    }
#define OFFAW()                                                                                                        \
    {                                                                                                                  \
        if (_A & RM8(FETCHWA()))                                                                                       \
            PSW &= ~F_Z;                                                                                               \
        else                                                                                                           \
            PSW |= F_Z | F_SK;                                                                                         \
    }
#define OFFAX(r)                                                                                                       \
    {                                                                                                                  \
        if (_A & RM8(r))                                                                                               \
            PSW &= ~F_Z;                                                                                               \
        else                                                                                                           \
            PSW |= F_Z | F_SK;                                                                                         \
    }
#define OFFI(r)                                                                                                        \
    {                                                                                                                  \
        if (r & FETCH8())                                                                                              \
            PSW &= ~F_Z;                                                                                               \
        else                                                                                                           \
            PSW |= F_Z | F_SK;                                                                                         \
    }
#define OFFI_IO(p)                                                                                                     \
    {                                                                                                                  \
        if (IN8(p) & FETCH8())                                                                                         \
            PSW &= ~F_Z;                                                                                               \
        else                                                                                                           \
            PSW |= F_Z | F_SK;                                                                                         \
    }
#define OFFIW()                                                                                                        \
    {                                                                                                                  \
        uint8 tmp = RM8(FETCHWA());                                                                                    \
        if (tmp & FETCH8())                                                                                            \
            PSW &= ~F_Z;                                                                                               \
        else                                                                                                           \
            PSW |= F_Z | F_SK;                                                                                         \
    }
#define ONA(r, n)                                                                                                      \
    {                                                                                                                  \
        if (r & n)                                                                                                     \
            PSW = (PSW & ~F_Z) | F_SK;                                                                                 \
        else                                                                                                           \
            PSW |= F_Z;                                                                                                \
    }
#define ONAW()                                                                                                         \
    {                                                                                                                  \
        if (_A & RM8(FETCHWA()))                                                                                       \
            PSW = (PSW & ~F_Z) | F_SK;                                                                                 \
        else                                                                                                           \
            PSW |= F_Z;                                                                                                \
    }
#define ONAX(r)                                                                                                        \
    {                                                                                                                  \
        if (_A & RM8(r))                                                                                               \
            PSW = (PSW & ~F_Z) | F_SK;                                                                                 \
        else                                                                                                           \
            PSW |= F_Z;                                                                                                \
    }
#define ONI(r)                                                                                                         \
    {                                                                                                                  \
        if (r & FETCH8())                                                                                              \
            PSW = (PSW & ~F_Z) | F_SK;                                                                                 \
        else                                                                                                           \
            PSW |= F_Z;                                                                                                \
    }
#define ONI_IO(p)                                                                                                      \
    {                                                                                                                  \
        if (IN8(p) & FETCH8())                                                                                         \
            PSW = (PSW & ~F_Z) | F_SK;                                                                                 \
        else                                                                                                           \
            PSW |= F_Z;                                                                                                \
    }
#define ONIW()                                                                                                         \
    {                                                                                                                  \
        uint8 tmp = RM8(FETCHWA());                                                                                    \
        if (tmp & FETCH8())                                                                                            \
            PSW = (PSW & ~F_Z) | F_SK;                                                                                 \
        else                                                                                                           \
            PSW |= F_Z;                                                                                                \
    }
#define ORA(r, n)                                                                                                      \
    {                                                                                                                  \
        r |= n;                                                                                                        \
        SET_Z(r);                                                                                                      \
    }
#define ORAW()                                                                                                         \
    {                                                                                                                  \
        _A |= RM8(FETCHWA());                                                                                          \
        SET_Z(_A);                                                                                                     \
    }
#define ORAX(r)                                                                                                        \
    {                                                                                                                  \
        _A |= RM8(r);                                                                                                  \
        SET_Z(_A);                                                                                                     \
    }
#define ORI(r)                                                                                                         \
    {                                                                                                                  \
        r |= FETCH8();                                                                                                 \
        SET_Z(r);                                                                                                      \
    }
#define ORI_IO(p)                                                                                                      \
    {                                                                                                                  \
        uint8 tmp = IN8(p) | FETCH8();                                                                                 \
        OUT8(p, tmp);                                                                                                  \
        SET_Z(tmp);                                                                                                    \
    }
#define ORIW()                                                                                                         \
    {                                                                                                                  \
        uint16 dst = FETCHWA();                                                                                        \
        uint8  tmp = RM8(dst) | FETCH8();                                                                              \
        WM8(dst, tmp);                                                                                                 \
        SET_Z(tmp);                                                                                                    \
    }
#define PEN()                                                                                                          \
    {                                                                                                                  \
    }
#define PER()                                                                                                          \
    {                                                                                                                  \
    }
#define PEX()                                                                                                          \
    {                                                                                                                  \
    }
#define RLD()                                                                                                          \
    {                                                                                                                  \
        uint8 old = RM8(HL);                                                                                           \
        uint8 tmp = (old << 4) | (_A & 0x0f);                                                                          \
        _A        = (_A & 0xf0) | (old >> 4);                                                                          \
        WM8(HL, tmp);                                                                                                  \
    }
#define RLL(r)                                                                                                         \
    {                                                                                                                  \
        uint8 carry = PSW & F_CY;                                                                                      \
        PSW         = (PSW & ~F_CY) | ((r >> 7) & F_CY);                                                               \
        r           = (r << 1) | carry;                                                                                \
    }
#define RLR(r)                                                                                                         \
    {                                                                                                                  \
        uint8 carry = (PSW & F_CY) << 7;                                                                               \
        PSW         = (PSW & ~F_CY) | (r & F_CY);                                                                      \
        r           = (r >> 1) | carry;                                                                                \
    }
#define RRD()                                                                                                          \
    {                                                                                                                  \
        uint8 old = RM8(HL);                                                                                           \
        uint8 tmp = (_A << 4) | (old >> 4);                                                                            \
        _A        = (_A & 0xf0) | (old & 0x0f);                                                                        \
        WM8(HL, tmp);                                                                                                  \
    }
#define SBB(r, n)                                                                                                      \
    {                                                                                                                  \
        uint8 tmp = r - n - (PSW & F_CY);                                                                              \
        ZHC_SUB(tmp, r, (PSW & F_CY));                                                                                 \
        r = tmp;                                                                                                       \
    }
#define SBBW()                                                                                                         \
    {                                                                                                                  \
        uint8 tmp = _A - RM8(FETCHWA()) - (PSW & F_CY);                                                                \
        ZHC_SUB(tmp, _A, (PSW & F_CY));                                                                                \
        _A = tmp;                                                                                                      \
    }
#define SBBX(r)                                                                                                        \
    {                                                                                                                  \
        uint8 tmp = _A - RM8(r) - (PSW & F_CY);                                                                        \
        ZHC_SUB(tmp, _A, (PSW & F_CY));                                                                                \
        _A = tmp;                                                                                                      \
    }
#define SBI(r)                                                                                                         \
    {                                                                                                                  \
        uint8 tmp = r - FETCH8() - (PSW & F_CY);                                                                       \
        ZHC_SUB(tmp, r, (PSW & F_CY));                                                                                 \
        r = tmp;                                                                                                       \
    }
#define SBI_IO(p)                                                                                                      \
    {                                                                                                                  \
        uint8 old = IN8(p);                                                                                            \
        uint8 tmp = old - FETCH8() - (PSW & F_CY);                                                                     \
        ZHC_SUB(tmp, old, (PSW & F_CY));                                                                               \
        OUT8(p, tmp);                                                                                                  \
    }
#define SIO()                                                                                                          \
    {                                                                                                                  \
        scount = 32 + 4;                                                                                               \
    }
#define SK(f)                                                                                                          \
    {                                                                                                                  \
        if (PSW & f)                                                                                                   \
            PSW |= F_SK;                                                                                               \
    }
#define SKIT(f)                                                                                                        \
    {                                                                                                                  \
        if (IRR & f)                                                                                                   \
            PSW |= F_SK;                                                                                               \
        IRR &= ~f;                                                                                                     \
    }
#define SKN(f)                                                                                                         \
    {                                                                                                                  \
        if (!(PSW & f))                                                                                                \
            PSW |= F_SK;                                                                                               \
    }
#define SKNIT(f)                                                                                                       \
    {                                                                                                                  \
        if (!(IRR & f))                                                                                                \
            PSW |= F_SK;                                                                                               \
        IRR &= ~f;                                                                                                     \
    }
#define SLL(r)                                                                                                         \
    {                                                                                                                  \
        PSW = (PSW & ~F_CY) | ((r >> 7) & F_CY);                                                                       \
        r <<= 1;                                                                                                       \
    }
#define SLR(r)                                                                                                         \
    {                                                                                                                  \
        PSW = (PSW & ~F_CY) | (r & F_CY);                                                                              \
        r >>= 1;                                                                                                       \
    }
#define STM()                                                                                                          \
    {                                                                                                                  \
        tcount = (((TM0 | (TM1 << 8)) & 0xfff) + 1) * PRESCALER;                                                       \
    }
#define SUB(r, n)                                                                                                      \
    {                                                                                                                  \
        uint8 tmp = r - n;                                                                                             \
        ZHC_SUB(tmp, r, 0);                                                                                            \
        r = tmp;                                                                                                       \
    }
#define SUBNB(r, n)                                                                                                    \
    {                                                                                                                  \
        uint8 tmp = r - n;                                                                                             \
        ZHC_SUB(tmp, r, 0);                                                                                            \
        r = tmp;                                                                                                       \
        SKIP_NC;                                                                                                       \
    }
#define SUBNBW()                                                                                                       \
    {                                                                                                                  \
        uint8 tmp = _A - RM8(FETCHWA());                                                                               \
        ZHC_SUB(tmp, _A, 0);                                                                                           \
        _A = tmp;                                                                                                      \
        SKIP_NC;                                                                                                       \
    }
#define SUBNBX(r)                                                                                                      \
    {                                                                                                                  \
        uint8 tmp = _A - RM8(r);                                                                                       \
        ZHC_SUB(tmp, _A, 0);                                                                                           \
        _A = tmp;                                                                                                      \
        SKIP_NC;                                                                                                       \
    }
#define SUBW()                                                                                                         \
    {                                                                                                                  \
        uint8 tmp = _A - RM8(FETCHWA());                                                                               \
        ZHC_SUB(tmp, _A, 0);                                                                                           \
        _A = tmp;                                                                                                      \
    }
#define SUBX(r)                                                                                                        \
    {                                                                                                                  \
        uint8 tmp = _A - RM8(r);                                                                                       \
        ZHC_SUB(tmp, _A, 0);                                                                                           \
        _A = tmp;                                                                                                      \
    }
#define SUI(r)                                                                                                         \
    {                                                                                                                  \
        uint8 tmp = r - FETCH8();                                                                                      \
        ZHC_SUB(tmp, r, 0);                                                                                            \
        r = tmp;                                                                                                       \
    }
#define SUI_IO(p)                                                                                                      \
    {                                                                                                                  \
        uint8 old = IN8(p);                                                                                            \
        uint8 tmp = old - FETCH8();                                                                                    \
        ZHC_SUB(tmp, old, 0);                                                                                          \
        OUT8(p, tmp);                                                                                                  \
    }
#define SUINB(r)                                                                                                       \
    {                                                                                                                  \
        uint8 tmp = r - FETCH8();                                                                                      \
        ZHC_SUB(tmp, r, 0);                                                                                            \
        r = tmp;                                                                                                       \
        SKIP_NC;                                                                                                       \
    }
#define SUINB_IO(p)                                                                                                    \
    {                                                                                                                  \
        uint8 old = IN8(p);                                                                                            \
        uint8 tmp = old - FETCH8();                                                                                    \
        ZHC_SUB(tmp, old, 0);                                                                                          \
        OUT8(p, tmp);                                                                                                  \
        SKIP_NC;                                                                                                       \
    }
#define XRA(r, n)                                                                                                      \
    {                                                                                                                  \
        r ^= n;                                                                                                        \
        SET_Z(r);                                                                                                      \
    }
#define XRAW()                                                                                                         \
    {                                                                                                                  \
        _A ^= RM8(FETCHWA());                                                                                          \
        SET_Z(_A);                                                                                                     \
    }
#define XRAX(r)                                                                                                        \
    {                                                                                                                  \
        _A ^= RM8(r);                                                                                                  \
        SET_Z(_A);                                                                                                     \
    }
#define XRI(r)                                                                                                         \
    {                                                                                                                  \
        r ^= FETCH8();                                                                                                 \
        SET_Z(r);                                                                                                      \
    }
#define XRI_IO(p)                                                                                                      \
    {                                                                                                                  \
        uint8 tmp = IN8(p) ^ FETCH8();                                                                                 \
        OUT8(p, tmp);                                                                                                  \
        SET_Z(tmp);                                                                                                    \
    }

//


UPD7801::UPD7801(VM *parent_vm, EMU *parent_emu) : DEVICE(parent_vm, parent_emu)
{
    reset();
}

UPD7801::~UPD7801()
{
}
void UPD7801::reset()
{
    PC = SP = 0;
    PSW = IRR = IFF = SIRQ = HALT = 0;
    _V = MB = MC = TM0 = TM1 = SR = 0xff;
    MK                            = 0x1f;
    PORTC = TO = SAK = 0;
    count = scount = tcount = 0;
}
void UPD7801::run(int clock)
{
    count += clock;
    while (count > 0) {
        if (IFF & 2)
            IFF--;

        prvPC = PC;
        OP();
        count -= period;

        if (scount && (scount -= period) <= 0) {
            scount = 0;
            IRR |= INTFS;
            OUT8(P_SO, SR);
            SR = IN8(P_SI);
            if (SAK) {
                SAK = 0;
                UPDATE_PORTC(0);
            }
        }

        if (tcount && (tcount -= period) <= 0) {
            tcount += (((TM0 | (TM1 << 8)) & 0xfff) + 1) * PRESCALER;
            IRR |= INTFT;
            if (TO) {
                TO = 0;
                UPDATE_PORTC(0);
            }
        }

        if (IFF == 1 && !SIRQ) {
            for (int i = 0; i < 5; i++) {
                uint8 bit = irq_bits[i];
                if ((IRR & bit) && !(MK & bit)) {
                    if (HALT) {
                        HALT = 0;
                        PC++;
                    }
                    PUSH8(PSW);
                    PUSH16(PC);
                    PC = irq_addr[i];
                    PSW &= ~(F_SK | F_L0 | F_L1);
                    IFF = 0;
                    IRR &= ~bit;
                    break;
                }
            }
        }
    }
}
void UPD7801::write_signal(int id, uint32 data, uint32 mask)
{
    if (id == SIG_UPD7801_INTF0) {
        if (data & mask)
            IRR |= INTF0;
        else
            IRR &= ~INTF0;
    } else if (id == SIG_UPD7801_INTF1) {
        if (data & mask)
            IRR |= INTF1;
        else
            IRR &= ~INTF1;
    } else if (id == SIG_UPD7801_INTF2) {
        if ((data & mask) && (MK & 0x20))
            IRR |= INTF2;
        else if (!(data & mask) && !(MK & 0x20))
            IRR |= INTF2;
    }
}
void UPD7801::OP()
{
    uint8 ope = FETCH8();

    // printf("op: %x\n", ope);

    if ((PSW & F_SK) && ope != 0x72) {
        switch (ope) {
            case 0x48:
                PSW &= ~(F_SK | F_L0 | F_L1);
                ope = FETCH8();
                PC += op48[ope].oplen - 2;
                period = op48[ope].clock;
                break;
            case 0x4c:
                PSW &= ~(F_SK | F_L0 | F_L1);
                ope = FETCH8();
                PC += op4c[ope].oplen - 2;
                period = op4c[ope].clock;
                break;
            case 0x4d:
                PSW &= ~(F_SK | F_L0 | F_L1);
                ope = FETCH8();
                PC += op4d[ope].oplen - 2;
                period = op4d[ope].clock;
                break;
            case 0x60:
                PSW &= ~(F_SK | F_L0 | F_L1);
                ope = FETCH8();
                PC += op60[ope].oplen - 2;
                period = op60[ope].clock;
                break;
            case 0x64:
                PSW &= ~(F_SK | F_L0 | F_L1);
                ope = FETCH8();
                PC += op64[ope].oplen - 2;
                period = op64[ope].clock;
                break;
            case 0x70:
                PSW &= ~(F_SK | F_L0 | F_L1);
                ope = FETCH8();
                PC += op70[ope].oplen - 2;
                period = op70[ope].clock;
                break;
            case 0x74:
                PSW &= ~(F_SK | F_L0 | F_L1);
                ope = FETCH8();
                PC += op74[ope].oplen - 2;
                period = op74[ope].clock;
                break;
            case 0x34:
                PSW &= ~(F_SK | F_L1);
                PC += op[ope].oplen - 1;
                period = op[ope].clock;
                break;
            case 0x69:
                PSW &= ~(F_SK | F_L0);
                PC += op[ope].oplen - 1;
                period = op[ope].clock;
                break;
            case 0x6f:
                PSW &= ~(F_SK | F_L1);
                PC += op[ope].oplen - 1;
                period = op[ope].clock;
                break;
            default:
                PSW &= ~(F_SK | F_L0 | F_L1);
                PC += op[ope].oplen - 1;
                period = op[ope].clock;
                break;
        }
        return;
    }
    period = op[ope].clock;
    switch (ope) {
        case 0x00:
            break;
        case 0x01:
            HALT = 1;
            PC--;
            break;
        case 0x02:
            SP++;
            break;
        case 0x03:
            SP--;
            break;
        case 0x04:
            SP = FETCH16();
            break;
        case 0x05:
            ANIW();
            break;
        case 0x07:
            ANI(_A);
            break;
        case 0x08:
            PC = POP16();
            break;
        case 0x09:
            SIO();
            break;
        case 0x0a:
            _A = _B;
            break;
        case 0x0b:
            _A = _C;
            break;
        case 0x0c:
            _A = _D;
            break;
        case 0x0d:
            _A = _E;
            break;
        case 0x0e:
            _A = _H;
            break;
        case 0x0f:
            _A = _L;
            break;
        case 0x10:
            EX();
            break;
        case 0x11:
            EXX();
            break;
        case 0x12:
            BC++;
            break;
        case 0x13:
            BC--;
            break;
        case 0x14:
            BC = FETCH16();
            break;
        case 0x15:
            ORIW();
            break;
        case 0x16:
            XRI(_A);
            break;
        case 0x17:
            ORI(_A);
            break;
        case 0x18:
            PC = POP16();
            PSW |= F_SK;
            break;
        case 0x19:
            if (!TO) {
                TO = 0x10;
                UPDATE_PORTC(0);
            }
            STM();
            break;
        case 0x1a:
            _B = _A;
            break;
        case 0x1b:
            _C = _A;
            break;
        case 0x1c:
            _D = _A;
            break;
        case 0x1d:
            _E = _A;
            break;
        case 0x1e:
            _H = _A;
            break;
        case 0x1f:
            _L = _A;
            break;
        case 0x20:
            INRW();
            break;
        case 0x21:
            BC = RM16(PC + _A + 1);
            break;
        case 0x22:
            DE++;
            break;
        case 0x23:
            DE--;
            break;
        case 0x24:
            DE = FETCH16();
            break;
        case 0x25:
            GTIW();
            break;
        case 0x26:
            ADINC(_A);
            break;
        case 0x27:
            GTI(_A);
            break;
        case 0x28:
            _A = RM8(FETCHWA());
            break;
        case 0x29:
            _A = RM8(BC);
            break;
        case 0x2a:
            _A = RM8(DE);
            break;
        case 0x2b:
            _A = RM8(HL);
            break;
        case 0x2c:
            _A = RM8(DE++);
            ;
            break;
        case 0x2d:
            _A = RM8(HL++);
            break;
        case 0x2e:
            _A = RM8(DE--);
            break;
        case 0x2f:
            _A = RM8(HL--);
            break;
        case 0x30:
            DCRW();
            break;
        case 0x31:
            BLOCK();
            break;
        case 0x32:
            HL++;
            break;
        case 0x33:
            HL--;
            break;
        case 0x34:
            if (PSW & F_L0)
                PC += 2;
            else
                HL = FETCH16();
            PSW = (PSW & ~F_L1) | F_L0;
            return;
        case 0x35:
            LTIW();
            break;
        case 0x36:
            SUINB(_A);
            break;
        case 0x37:
            LTI(_A);
            break;
        case 0x38:
            WM8(FETCHWA(), _A);
            break;
        case 0x39:
            WM8(BC, _A);
            break;
        case 0x3a:
            WM8(DE, _A);
            break;
        case 0x3b:
            WM8(HL, _A);
            break;
        case 0x3c:
            WM8(DE++, _A);
            break;
        case 0x3d:
            WM8(HL++, _A);
            break;
        case 0x3e:
            WM8(DE--, _A);
            break;
        case 0x3f:
            WM8(HL--, _A);
            break;
        case 0x41:
            INR(_A);
            break;
        case 0x42:
            INR(_B);
            break;
        case 0x43:
            INR(_C);
            break;
        case 0x44:
            CALL();
            break;
        case 0x45:
            ONIW();
            break;
        case 0x46:
            ADI(_A);
            break;
        case 0x47:
            ONI(_A);
            break;
        case 0x48:
            OP48();
            break;
        case 0x49:
            WM8(BC, FETCH8());
            break;
        case 0x4a:
            WM8(DE, FETCH8());
            break;
        case 0x4b:
            WM8(HL, FETCH8());
            break;
        case 0x4c:
            OP4C();
            break;
        case 0x4d:
            OP4D();
            break;
        case 0x4e:
        case 0x4f:
            JRE(ope);
            break;
        case 0x51:
            DCR(_A);
            break;
        case 0x52:
            DCR(_B);
            break;
        case 0x53:
            DCR(_C);
            break;
        case 0x54:
            PC = FETCH16();
            break;
        case 0x55:
            OFFIW();
            break;
        case 0x56:
            ACI(_A);
            break;
        case 0x57:
            OFFI(_A);
            break;
        case 0x58:
            BIT(0);
            break;
        case 0x59:
            BIT(1);
            break;
        case 0x5a:
            BIT(2);
            break;
        case 0x5b:
            BIT(3);
            break;
        case 0x5c:
            BIT(4);
            break;
        case 0x5d:
            BIT(5);
            break;
        case 0x5e:
            BIT(6);
            break;
        case 0x5f:
            BIT(7);
            break;
        case 0x60:
            OP60();
            break;
        case 0x61:
            DAA();
            break;
        case 0x62:
            PC   = POP16();
            PSW  = POP8();
            SIRQ = 0;
            return;
        case 0x63:
            PUSH16(PC);
            PC = BC;
            break;
        case 0x64:
            OP64();
            break;
        case 0x65:
            NEIW();
            break;
        case 0x66:
            SUI(_A);
            break;
        case 0x67:
            NEI(_A);
            break;
        case 0x68:
            _V = FETCH8();
            break;
        case 0x69:
            if (PSW & F_L1)
                PC++;
            else
                _A = FETCH8();
            PSW = (PSW & ~F_L0) | F_L1;
            return;
        case 0x6a:
            _B = FETCH8();
            break;
        case 0x6b:
            _C = FETCH8();
            break;
        case 0x6c:
            _D = FETCH8();
            break;
        case 0x6d:
            _E = FETCH8();
            break;
        case 0x6e:
            _H = FETCH8();
            break;
        case 0x6f:
            if (PSW & F_L0)
                PC++;
            else
                _L = FETCH8();
            PSW = (PSW & ~F_L1) | F_L0;
            return;
        case 0x70:
            OP70();
            break;
        case 0x71:
            MVIW();
            break;
        case 0x72:
            PUSH8(PSW);
            PUSH16(PC);
            PSW &= ~F_SK;
            SIRQ = 1;
            PC   = 0x0060;
            break;
        case 0x73:
            PC = BC;
            break;
        case 0x74:
            OP74();
            break;
        case 0x75:
            EQIW();
            break;
        case 0x76:
            SBI(_A);
            break;
        case 0x77:
            EQI(_A);
            break;
        case 0x78:
        case 0x79:
        case 0x7a:
        case 0x7b:
        case 0x7c:
        case 0x7d:
        case 0x7e:
        case 0x7f:
            CALF(ope);
            break;
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x86:
        case 0x87:
        case 0x88:
        case 0x89:
        case 0x8a:
        case 0x8b:
        case 0x8c:
        case 0x8d:
        case 0x8e:
        case 0x8f:
        case 0x90:
        case 0x91:
        case 0x92:
        case 0x93:
        case 0x94:
        case 0x95:
        case 0x96:
        case 0x97:
        case 0x98:
        case 0x99:
        case 0x9a:
        case 0x9b:
        case 0x9c:
        case 0x9d:
        case 0x9e:
        case 0x9f:
        case 0xa0:
        case 0xa1:
        case 0xa2:
        case 0xa3:
        case 0xa4:
        case 0xa5:
        case 0xa6:
        case 0xa7:
        case 0xa8:
        case 0xa9:
        case 0xaa:
        case 0xab:
        case 0xac:
        case 0xad:
        case 0xae:
        case 0xaf:
        case 0xb0:
        case 0xb1:
        case 0xb2:
        case 0xb3:
        case 0xb4:
        case 0xb5:
        case 0xb6:
        case 0xb7:
        case 0xb8:
        case 0xb9:
        case 0xba:
        case 0xbb:
        case 0xbc:
        case 0xbd:
        case 0xbe:
        case 0xbf:
            CALT(ope);
            break;
        case 0xc0:
        case 0xc1:
        case 0xc2:
        case 0xc3:
        case 0xc4:
        case 0xc5:
        case 0xc6:
        case 0xc7:
        case 0xc8:
        case 0xc9:
        case 0xca:
        case 0xcb:
        case 0xcc:
        case 0xcd:
        case 0xce:
        case 0xcf:
        case 0xd0:
        case 0xd1:
        case 0xd2:
        case 0xd3:
        case 0xd4:
        case 0xd5:
        case 0xd6:
        case 0xd7:
        case 0xd8:
        case 0xd9:
        case 0xda:
        case 0xdb:
        case 0xdc:
        case 0xdd:
        case 0xde:
        case 0xdf:
            PC += ope & 0x1f;
            break;
        case 0xe0:
        case 0xe1:
        case 0xe2:
        case 0xe3:
        case 0xe4:
        case 0xe5:
        case 0xe6:
        case 0xe7:
        case 0xe8:
        case 0xe9:
        case 0xea:
        case 0xeb:
        case 0xec:
        case 0xed:
        case 0xee:
        case 0xef:
        case 0xf0:
        case 0xf1:
        case 0xf2:
        case 0xf3:
        case 0xf4:
        case 0xf5:
        case 0xf6:
        case 0xf7:
        case 0xf8:
        case 0xf9:
        case 0xfa:
        case 0xfb:
        case 0xfc:
        case 0xfd:
        case 0xfe:
        case 0xff:
            PC -= 0x20 - (ope & 0x1f);
            break;
        default:
            printf(("PC=%4x\tCPU\tUNKNOWN OP : %2x\n"), prvPC, ope);
    }
    PSW &= ~(F_L0 | F_L1);
}
void UPD7801::OP48()
{
    uint8 ope = FETCH8();
    period    = op48[ope].clock;
    switch (ope) {
        case 0x00:
            SKIT(INTF0);
            break;
        case 0x01:
            SKIT(INTFT);
            break;
        case 0x02:
            SKIT(INTF1);
            break;
        case 0x03:
            SKIT(INTF2);
            break;
        case 0x04:
            SKIT(INTFS);
            break;
        case 0x0a:
            SK(F_CY);
            break;
        case 0x0c:
            SK(F_Z);
            break;
        case 0x0e:
            PUSH16(VA);
            break;
        case 0x0f:
            VA = POP16();
            break;
        case 0x10:
            SKNIT(INTF0);
            break;
        case 0x11:
            SKNIT(INTFT);
            break;
        case 0x12:
            SKNIT(INTF1);
            break;
        case 0x13:
            SKNIT(INTF2);
            break;
        case 0x14:
            SKNIT(INTFS);
            break;
        case 0x1a:
            SKN(F_CY);
            break;
        case 0x1c:
            SKN(F_Z);
            break;
        case 0x1e:
            PUSH16(BC);
            break;
        case 0x1f:
            BC = POP16();
            break;
        case 0x20:
            IFF = 3;
            break;
        case 0x24:
            IFF = 0;
            break;
        case 0x2a:
            PSW &= ~F_CY;
            break;
        case 0x2b:
            PSW |= F_CY;
            break;
        case 0x2c:
            PEN();
            break;
        case 0x2d:
            PEX();
            break;
        case 0x2e:
            PUSH16(DE);
            break;
        case 0x2f:
            DE = POP16();
            break;
        case 0x30:
            RLL(_A);
            break;
        case 0x31:
            RLR(_A);
            break;
        case 0x32:
            RLL(_C);
            break;
        case 0x33:
            RLR(_C);
            break;
        case 0x34:
            SLL(_A);
            break;
        case 0x35:
            SLR(_A);
            break;
        case 0x36:
            SLL(_C);
            break;
        case 0x37:
            SLR(_C);
            break;
        case 0x38:
            RLD();
            break;
        case 0x39:
            RRD();
            break;
        case 0x3c:
            PER();
            break;
        case 0x3e:
            PUSH16(HL);
            break;
        case 0x3f:
            HL = POP16();
            break;
        default:
            printf(("PC=%4x\tCPU\tUNKNOWN OP : 48 %2x\n"), prvPC, ope);
    }
}
void UPD7801::OP4C()
{
    uint8 ope = FETCH8();
    period    = op4c[ope].clock;
    switch (ope) {
        case 0xc0:
            _A = IN8(P_A);
            break;
        case 0xc1:
            _A = IN8(P_B);
            break;
        case 0xc2:
            _A = IN8(P_C);
            break;
        case 0xc3:
            _A = MK;
            break;
        case 0xc4:
            _A = MB;
            break;
        case 0xc5:
            _A = MC;
            break;
        case 0xc6:
            _A = TM0;
            break;
        case 0xc7:
            _A = TM1;
            break;
        case 0xc8:
            if (!SAK) {
                SAK = 8;
                UPDATE_PORTC(0);
            }
            _A = SR;
            break;
        default:
            if (ope < 0xc0) {
                UPDATE_PORTC(0x20);
                _A = RM8((_B << 8) | ope);
                UPDATE_PORTC(0);
            } else
                printf(("PC=%4x\tCPU\tUNKNOWN OP : 4c %2x\n"), prvPC, ope);
    }
}
void UPD7801::OP4D()
{
    uint8 ope = FETCH8();
    period    = op4d[ope].clock;
    switch (ope) {
        case 0xc0:
            OUT8(P_A, _A);
            break;
        case 0xc1:
            OUT8(P_B, _A);
            break;
        case 0xc2:
            OUT8(P_C, _A);
            break;
        case 0xc3:
            MK = _A;
            break;
        case 0xc4:
            MB = _A;
            break;
        case 0xc5:
            if (MC != _A) {
                MC = _A;
                UPDATE_PORTC(0);
                break;
            }
            MC = _A;
            break;
        case 0xc6:
            TM0 = _A;
            break;
        case 0xc7:
            TM1 = _A;
            break;
        case 0xc8:
            if (!SAK) {
                SAK = 8;
                UPDATE_PORTC(0);
            }
            SR = _A;
            break;
        default:
            if (ope < 0xc0) {
                UPDATE_PORTC(0x20);
                WM8((_B << 8) | ope, _A);
                UPDATE_PORTC(0);
            } else
                printf(("PC=%4x\tCPU\tUNKNOWN OP : 4d %2x\n"), prvPC, ope);
    }
}
void UPD7801::OP60()
{
    uint8 ope = FETCH8();
    period    = op60[ope].clock;
    switch (ope) {
        case 0x08:
            ANA(_V, _A);
            break;
        case 0x09:
            ANA(_A, _A);
            break;
        case 0x0a:
            ANA(_B, _A);
            break;
        case 0x0b:
            ANA(_C, _A);
            break;
        case 0x0c:
            ANA(_D, _A);
            break;
        case 0x0d:
            ANA(_E, _A);
            break;
        case 0x0e:
            ANA(_H, _A);
            break;
        case 0x0f:
            ANA(_L, _A);
            break;
        case 0x10:
            XRA(_V, _A);
            break;
        case 0x11:
            XRA(_A, _A);
            break;
        case 0x12:
            XRA(_B, _A);
            break;
        case 0x13:
            XRA(_C, _A);
            break;
        case 0x14:
            XRA(_D, _A);
            break;
        case 0x15:
            XRA(_E, _A);
            break;
        case 0x16:
            XRA(_H, _A);
            break;
        case 0x17:
            XRA(_L, _A);
            break;
        case 0x18:
            ORA(_V, _A);
            break;
        case 0x19:
            ORA(_A, _A);
            break;
        case 0x1a:
            ORA(_B, _A);
            break;
        case 0x1b:
            ORA(_C, _A);
            break;
        case 0x1c:
            ORA(_D, _A);
            break;
        case 0x1d:
            ORA(_E, _A);
            break;
        case 0x1e:
            ORA(_H, _A);
            break;
        case 0x1f:
            ORA(_L, _A);
            break;
        case 0x20:
            ADDNC(_V, _A);
            break;
        case 0x21:
            ADDNC(_A, _A);
            break;
        case 0x22:
            ADDNC(_B, _A);
            break;
        case 0x23:
            ADDNC(_C, _A);
            break;
        case 0x24:
            ADDNC(_D, _A);
            break;
        case 0x25:
            ADDNC(_E, _A);
            break;
        case 0x26:
            ADDNC(_H, _A);
            break;
        case 0x27:
            ADDNC(_L, _A);
            break;
        case 0x28:
            GTA(_V, _A);
            break;
        case 0x29:
            GTA(_A, _A);
            break;
        case 0x2a:
            GTA(_B, _A);
            break;
        case 0x2b:
            GTA(_C, _A);
            break;
        case 0x2c:
            GTA(_D, _A);
            break;
        case 0x2d:
            GTA(_E, _A);
            break;
        case 0x2e:
            GTA(_H, _A);
            break;
        case 0x2f:
            GTA(_L, _A);
            break;
        case 0x30:
            SUBNB(_V, _A);
            break;
        case 0x31:
            SUBNB(_A, _A);
            break;
        case 0x32:
            SUBNB(_B, _A);
            break;
        case 0x33:
            SUBNB(_C, _A);
            break;
        case 0x34:
            SUBNB(_D, _A);
            break;
        case 0x35:
            SUBNB(_E, _A);
            break;
        case 0x36:
            SUBNB(_H, _A);
            break;
        case 0x37:
            SUBNB(_L, _A);
            break;
        case 0x38:
            LTA(_V, _A);
            break;
        case 0x39:
            LTA(_A, _A);
            break;
        case 0x3a:
            LTA(_B, _A);
            break;
        case 0x3b:
            LTA(_C, _A);
            break;
        case 0x3c:
            LTA(_D, _A);
            break;
        case 0x3d:
            LTA(_E, _A);
            break;
        case 0x3e:
            LTA(_H, _A);
            break;
        case 0x3f:
            LTA(_L, _A);
            break;
        case 0x40:
            ADD(_V, _A);
            break;
        case 0x41:
            ADD(_A, _A);
            break;
        case 0x42:
            ADD(_B, _A);
            break;
        case 0x43:
            ADD(_C, _A);
            break;
        case 0x44:
            ADD(_D, _A);
            break;
        case 0x45:
            ADD(_E, _A);
            break;
        case 0x46:
            ADD(_H, _A);
            break;
        case 0x47:
            ADD(_L, _A);
            break;
        case 0x50:
            ADC(_V, _A);
            break;
        case 0x51:
            ADC(_A, _A);
            break;
        case 0x52:
            ADC(_B, _A);
            break;
        case 0x53:
            ADC(_C, _A);
            break;
        case 0x54:
            ADC(_D, _A);
            break;
        case 0x55:
            ADC(_E, _A);
            break;
        case 0x56:
            ADC(_H, _A);
            break;
        case 0x57:
            ADC(_L, _A);
            break;
        case 0x60:
            SUB(_V, _A);
            break;
        case 0x61:
            SUB(_A, _A);
            break;
        case 0x62:
            SUB(_B, _A);
            break;
        case 0x63:
            SUB(_C, _A);
            break;
        case 0x64:
            SUB(_D, _A);
            break;
        case 0x65:
            SUB(_E, _A);
            break;
        case 0x66:
            SUB(_H, _A);
            break;
        case 0x67:
            SUB(_L, _A);
            break;
        case 0x68:
            NEA(_V, _A);
            break;
        case 0x69:
            NEA(_A, _A);
            break;
        case 0x6a:
            NEA(_B, _A);
            break;
        case 0x6b:
            NEA(_C, _A);
            break;
        case 0x6c:
            NEA(_D, _A);
            break;
        case 0x6d:
            NEA(_E, _A);
            break;
        case 0x6e:
            NEA(_H, _A);
            break;
        case 0x6f:
            NEA(_L, _A);
            break;
        case 0x70:
            SBB(_V, _A);
            break;
        case 0x71:
            SBB(_A, _A);
            break;
        case 0x72:
            SBB(_B, _A);
            break;
        case 0x73:
            SBB(_C, _A);
            break;
        case 0x74:
            SBB(_D, _A);
            break;
        case 0x75:
            SBB(_E, _A);
            break;
        case 0x76:
            SBB(_H, _A);
            break;
        case 0x77:
            SBB(_L, _A);
            break;
        case 0x78:
            EQA(_V, _A);
            break;
        case 0x79:
            EQA(_A, _A);
            break;
        case 0x7a:
            EQA(_B, _A);
            break;
        case 0x7b:
            EQA(_C, _A);
            break;
        case 0x7c:
            EQA(_D, _A);
            break;
        case 0x7d:
            EQA(_E, _A);
            break;
        case 0x7e:
            EQA(_H, _A);
            break;
        case 0x7f:
            EQA(_L, _A);
            break;
        case 0x88:
            ANA(_A, _V);
            break;
        case 0x89:
            ANA(_A, _A);
            break;
        case 0x8a:
            ANA(_A, _B);
            break;
        case 0x8b:
            ANA(_A, _C);
            break;
        case 0x8c:
            ANA(_A, _D);
            break;
        case 0x8d:
            ANA(_A, _E);
            break;
        case 0x8e:
            ANA(_A, _H);
            break;
        case 0x8f:
            ANA(_A, _L);
            break;
        case 0x90:
            XRA(_A, _V);
            break;
        case 0x91:
            XRA(_A, _A);
            break;
        case 0x92:
            XRA(_A, _B);
            break;
        case 0x93:
            XRA(_A, _C);
            break;
        case 0x94:
            XRA(_A, _D);
            break;
        case 0x95:
            XRA(_A, _E);
            break;
        case 0x96:
            XRA(_A, _H);
            break;
        case 0x97:
            XRA(_A, _L);
            break;
        case 0x98:
            ORA(_A, _V);
            break;
        case 0x99:
            ORA(_A, _A);
            break;
        case 0x9a:
            ORA(_A, _B);
            break;
        case 0x9b:
            ORA(_A, _C);
            break;
        case 0x9c:
            ORA(_A, _D);
            break;
        case 0x9d:
            ORA(_A, _E);
            break;
        case 0x9e:
            ORA(_A, _H);
            break;
        case 0x9f:
            ORA(_A, _L);
            break;
        case 0xa0:
            ADDNC(_A, _V);
            break;
        case 0xa1:
            ADDNC(_A, _A);
            break;
        case 0xa2:
            ADDNC(_A, _B);
            break;
        case 0xa3:
            ADDNC(_A, _C);
            break;
        case 0xa4:
            ADDNC(_A, _D);
            break;
        case 0xa5:
            ADDNC(_A, _E);
            break;
        case 0xa6:
            ADDNC(_A, _H);
            break;
        case 0xa7:
            ADDNC(_A, _L);
            break;
        case 0xa8:
            GTA(_A, _V);
            break;
        case 0xa9:
            GTA(_A, _A);
            break;
        case 0xaa:
            GTA(_A, _B);
            break;
        case 0xab:
            GTA(_A, _C);
            break;
        case 0xac:
            GTA(_A, _D);
            break;
        case 0xad:
            GTA(_A, _E);
            break;
        case 0xae:
            GTA(_A, _H);
            break;
        case 0xaf:
            GTA(_A, _L);
            break;
        case 0xb0:
            SUBNB(_A, _V);
            break;
        case 0xb1:
            SUBNB(_A, _A);
            break;
        case 0xb2:
            SUBNB(_A, _B);
            break;
        case 0xb3:
            SUBNB(_A, _C);
            break;
        case 0xb4:
            SUBNB(_A, _D);
            break;
        case 0xb5:
            SUBNB(_A, _E);
            break;
        case 0xb6:
            SUBNB(_A, _H);
            break;
        case 0xb7:
            SUBNB(_A, _L);
            break;
        case 0xb8:
            LTA(_A, _V);
            break;
        case 0xb9:
            LTA(_A, _A);
            break;
        case 0xba:
            LTA(_A, _B);
            break;
        case 0xbb:
            LTA(_A, _C);
            break;
        case 0xbc:
            LTA(_A, _D);
            break;
        case 0xbd:
            LTA(_A, _E);
            break;
        case 0xbe:
            LTA(_A, _H);
            break;
        case 0xbf:
            LTA(_A, _L);
            break;
        case 0xc0:
            ADD(_A, _V);
            break;
        case 0xc1:
            ADD(_A, _A);
            break;
        case 0xc2:
            ADD(_A, _B);
            break;
        case 0xc3:
            ADD(_A, _C);
            break;
        case 0xc4:
            ADD(_A, _D);
            break;
        case 0xc5:
            ADD(_A, _E);
            break;
        case 0xc6:
            ADD(_A, _H);
            break;
        case 0xc7:
            ADD(_A, _L);
            break;
        case 0xc8:
            ONA(_A, _V);
            break;
        case 0xc9:
            ONA(_A, _A);
            break;
        case 0xca:
            ONA(_A, _B);
            break;
        case 0xcb:
            ONA(_A, _C);
            break;
        case 0xcc:
            ONA(_A, _D);
            break;
        case 0xcd:
            ONA(_A, _E);
            break;
        case 0xce:
            ONA(_A, _H);
            break;
        case 0xcf:
            ONA(_A, _L);
            break;
        case 0xd0:
            ADC(_A, _V);
            break;
        case 0xd1:
            ADC(_A, _A);
            break;
        case 0xd2:
            ADC(_A, _B);
            break;
        case 0xd3:
            ADC(_A, _C);
            break;
        case 0xd4:
            ADC(_A, _D);
            break;
        case 0xd5:
            ADC(_A, _E);
            break;
        case 0xd6:
            ADC(_A, _H);
            break;
        case 0xd7:
            ADC(_A, _L);
            break;
        case 0xd8:
            OFFA(_A, _V);
            break;
        case 0xd9:
            OFFA(_A, _A);
            break;
        case 0xda:
            OFFA(_A, _B);
            break;
        case 0xdb:
            OFFA(_A, _C);
            break;
        case 0xdc:
            OFFA(_A, _D);
            break;
        case 0xdd:
            OFFA(_A, _E);
            break;
        case 0xde:
            OFFA(_A, _H);
            break;
        case 0xdf:
            OFFA(_A, _L);
            break;
        case 0xe0:
            SUB(_A, _V);
            break;
        case 0xe1:
            SUB(_A, _A);
            break;
        case 0xe2:
            SUB(_A, _B);
            break;
        case 0xe3:
            SUB(_A, _C);
            break;
        case 0xe4:
            SUB(_A, _D);
            break;
        case 0xe5:
            SUB(_A, _E);
            break;
        case 0xe6:
            SUB(_A, _H);
            break;
        case 0xe7:
            SUB(_A, _L);
            break;
        case 0xe8:
            NEA(_A, _V);
            break;
        case 0xe9:
            NEA(_A, _A);
            break;
        case 0xea:
            NEA(_A, _B);
            break;
        case 0xeb:
            NEA(_A, _C);
            break;
        case 0xec:
            NEA(_A, _D);
            break;
        case 0xed:
            NEA(_A, _E);
            break;
        case 0xee:
            NEA(_A, _H);
            break;
        case 0xef:
            NEA(_A, _L);
            break;
        case 0xf0:
            SBB(_A, _V);
            break;
        case 0xf1:
            SBB(_A, _A);
            break;
        case 0xf2:
            SBB(_A, _B);
            break;
        case 0xf3:
            SBB(_A, _C);
            break;
        case 0xf4:
            SBB(_A, _D);
            break;
        case 0xf5:
            SBB(_A, _E);
            break;
        case 0xf6:
            SBB(_A, _H);
            break;
        case 0xf7:
            SBB(_A, _L);
            break;
        case 0xf8:
            EQA(_A, _V);
            break;
        case 0xf9:
            EQA(_A, _A);
            break;
        case 0xfa:
            EQA(_A, _B);
            break;
        case 0xfb:
            EQA(_A, _C);
            break;
        case 0xfc:
            EQA(_A, _D);
            break;
        case 0xfd:
            EQA(_A, _E);
            break;
        case 0xfe:
            EQA(_A, _H);
            break;
        case 0xff:
            EQA(_A, _L);
            break;
        default:
            printf(("PC=%4x\tCPU\tUNKNOWN OP : 60 %2x\n"), prvPC, ope);
    }
}
void UPD7801::OP64()
{
    uint8 ope = FETCH8();
    period    = op64[ope].clock;
    switch (ope) {
        case 0x08:
            ANI(_V);
            break;
        case 0x09:
            ANI(_A);
            break;
        case 0x0a:
            ANI(_B);
            break;
        case 0x0b:
            ANI(_C);
            break;
        case 0x0c:
            ANI(_D);
            break;
        case 0x0d:
            ANI(_E);
            break;
        case 0x0e:
            ANI(_H);
            break;
        case 0x0f:
            ANI(_L);
            break;
        case 0x10:
            XRI(_V);
            break;
        case 0x11:
            XRI(_A);
            break;
        case 0x12:
            XRI(_B);
            break;
        case 0x13:
            XRI(_C);
            break;
        case 0x14:
            XRI(_D);
            break;
        case 0x15:
            XRI(_E);
            break;
        case 0x16:
            XRI(_H);
            break;
        case 0x17:
            XRI(_L);
            break;
        case 0x18:
            ORI(_V);
            break;
        case 0x19:
            ORI(_A);
            break;
        case 0x1a:
            ORI(_B);
            break;
        case 0x1b:
            ORI(_C);
            break;
        case 0x1c:
            ORI(_D);
            break;
        case 0x1d:
            ORI(_E);
            break;
        case 0x1e:
            ORI(_H);
            break;
        case 0x1f:
            ORI(_L);
            break;
        case 0x20:
            ADINC(_V);
            break;
        case 0x21:
            ADINC(_A);
            break;
        case 0x22:
            ADINC(_B);
            break;
        case 0x23:
            ADINC(_C);
            break;
        case 0x24:
            ADINC(_D);
            break;
        case 0x25:
            ADINC(_E);
            break;
        case 0x26:
            ADINC(_H);
            break;
        case 0x27:
            ADINC(_L);
            break;
        case 0x28:
            GTI(_V);
            break;
        case 0x29:
            GTI(_A);
            break;
        case 0x2a:
            GTI(_B);
            break;
        case 0x2b:
            GTI(_C);
            break;
        case 0x2c:
            GTI(_D);
            break;
        case 0x2d:
            GTI(_E);
            break;
        case 0x2e:
            GTI(_H);
            break;
        case 0x2f:
            GTI(_L);
            break;
        case 0x30:
            SUINB(_V);
            break;
        case 0x31:
            SUINB(_A);
            break;
        case 0x32:
            SUINB(_B);
            break;
        case 0x33:
            SUINB(_C);
            break;
        case 0x34:
            SUINB(_D);
            break;
        case 0x35:
            SUINB(_E);
            break;
        case 0x36:
            SUINB(_H);
            break;
        case 0x37:
            SUINB(_L);
            break;
        case 0x38:
            LTI(_V);
            break;
        case 0x39:
            LTI(_A);
            break;
        case 0x3a:
            LTI(_B);
            break;
        case 0x3b:
            LTI(_C);
            break;
        case 0x3c:
            LTI(_D);
            break;
        case 0x3d:
            LTI(_E);
            break;
        case 0x3e:
            LTI(_H);
            break;
        case 0x3f:
            LTI(_L);
            break;
        case 0x40:
            ADI(_V);
            break;
        case 0x41:
            ADI(_A);
            break;
        case 0x42:
            ADI(_B);
            break;
        case 0x43:
            ADI(_C);
            break;
        case 0x44:
            ADI(_D);
            break;
        case 0x45:
            ADI(_E);
            break;
        case 0x46:
            ADI(_H);
            break;
        case 0x47:
            ADI(_L);
            break;
        case 0x48:
            ONI(_V);
            break;
        case 0x49:
            ONI(_A);
            break;
        case 0x4a:
            ONI(_B);
            break;
        case 0x4b:
            ONI(_C);
            break;
        case 0x4c:
            ONI(_D);
            break;
        case 0x4d:
            ONI(_E);
            break;
        case 0x4e:
            ONI(_H);
            break;
        case 0x4f:
            ONI(_L);
            break;
        case 0x50:
            ACI(_V);
            break;
        case 0x51:
            ACI(_A);
            break;
        case 0x52:
            ACI(_B);
            break;
        case 0x53:
            ACI(_C);
            break;
        case 0x54:
            ACI(_D);
            break;
        case 0x55:
            ACI(_E);
            break;
        case 0x56:
            ACI(_H);
            break;
        case 0x57:
            ACI(_L);
            break;
        case 0x58:
            OFFI(_V);
            break;
        case 0x59:
            OFFI(_A);
            break;
        case 0x5a:
            OFFI(_B);
            break;
        case 0x5b:
            OFFI(_C);
            break;
        case 0x5c:
            OFFI(_D);
            break;
        case 0x5d:
            OFFI(_E);
            break;
        case 0x5e:
            OFFI(_H);
            break;
        case 0x5f:
            OFFI(_L);
            break;
        case 0x60:
            SUI(_V);
            break;
        case 0x61:
            SUI(_A);
            break;
        case 0x62:
            SUI(_B);
            break;
        case 0x63:
            SUI(_C);
            break;
        case 0x64:
            SUI(_D);
            break;
        case 0x65:
            SUI(_E);
            break;
        case 0x66:
            SUI(_H);
            break;
        case 0x67:
            SUI(_L);
            break;
        case 0x68:
            NEI(_V);
            break;
        case 0x69:
            NEI(_A);
            break;
        case 0x6a:
            NEI(_B);
            break;
        case 0x6b:
            NEI(_C);
            break;
        case 0x6c:
            NEI(_D);
            break;
        case 0x6d:
            NEI(_E);
            break;
        case 0x6e:
            NEI(_H);
            break;
        case 0x6f:
            NEI(_L);
            break;
        case 0x70:
            SBI(_V);
            break;
        case 0x71:
            SBI(_A);
            break;
        case 0x72:
            SBI(_B);
            break;
        case 0x73:
            SBI(_C);
            break;
        case 0x74:
            SBI(_D);
            break;
        case 0x75:
            SBI(_E);
            break;
        case 0x76:
            SBI(_H);
            break;
        case 0x77:
            SBI(_L);
            break;
        case 0x78:
            EQI(_V);
            break;
        case 0x79:
            EQI(_A);
            break;
        case 0x7a:
            EQI(_B);
            break;
        case 0x7b:
            EQI(_C);
            break;
        case 0x7c:
            EQI(_D);
            break;
        case 0x7d:
            EQI(_E);
            break;
        case 0x7e:
            EQI(_H);
            break;
        case 0x7f:
            EQI(_L);
            break;
        case 0x88:
            ANI_IO(P_A);
            break;
        case 0x89:
            ANI_IO(P_B);
            break;
        case 0x8a:
            ANI_IO(P_C);
            break;
        case 0x8b:
            ANI(MK);
            break;
        case 0x90:
            XRI_IO(P_A);
            break;
        case 0x91:
            XRI_IO(P_B);
            break;
        case 0x92:
            XRI_IO(P_C);
            break;
        case 0x93:
            XRI(MK);
            break;
        case 0x98:
            ORI_IO(P_A);
            break;
        case 0x99:
            ORI_IO(P_B);
            break;
        case 0x9a:
            ORI_IO(P_C);
            break;
        case 0x9b:
            ORI(MK);
            break;
        case 0xa0:
            ADINC_IO(P_A);
            break;
        case 0xa1:
            ADINC_IO(P_B);
            break;
        case 0xa2:
            ADINC_IO(P_C);
            break;
        case 0xa3:
            ADINC(MK);
            break;
        case 0xa8:
            GTI_IO(P_A);
            break;
        case 0xa9:
            GTI_IO(P_B);
            break;
        case 0xaa:
            GTI_IO(P_C);
            break;
        case 0xab:
            GTI(MK);
            break;
        case 0xb0:
            SUINB_IO(P_A);
            break;
        case 0xb1:
            SUINB_IO(P_B);
            break;
        case 0xb2:
            SUINB_IO(P_C);
            break;
        case 0xb3:
            SUINB(MK);
            break;
        case 0xb8:
            LTI_IO(P_A);
            break;
        case 0xb9:
            LTI_IO(P_B);
            break;
        case 0xba:
            LTI_IO(P_C);
            break;
        case 0xbb:
            LTI(MK);
            break;
        case 0xc0:
            ADI_IO(P_A);
            break;
        case 0xc1:
            ADI_IO(P_B);
            break;
        case 0xc2:
            ADI_IO(P_C);
            break;
        case 0xc3:
            ADI(MK);
            break;
        case 0xc8:
            ONI_IO(P_A);
            break;
        case 0xc9:
            ONI_IO(P_B);
            break;
        case 0xca:
            ONI_IO(P_C);
            break;
        case 0xcb:
            ONI(MK);
            break;
        case 0xd0:
            ACI_IO(P_A);
            break;
        case 0xd1:
            ACI_IO(P_B);
            break;
        case 0xd2:
            ACI_IO(P_C);
            break;
        case 0xd3:
            ACI(MK);
            break;
        case 0xd8:
            OFFI_IO(P_A);
            break;
        case 0xd9:
            OFFI_IO(P_B);
            break;
        case 0xda:
            OFFI_IO(P_C);
            break;
        case 0xdb:
            OFFI(MK);
            break;
        case 0xe0:
            SUI_IO(P_A);
            break;
        case 0xe1:
            SUI_IO(P_B);
            break;
        case 0xe2:
            SUI_IO(P_C);
            break;
        case 0xe3:
            SUI(MK);
            break;
        case 0xe8:
            NEI_IO(P_A);
            break;
        case 0xe9:
            NEI_IO(P_B);
            break;
        case 0xea:
            NEI_IO(P_C);
            break;
        case 0xeb:
            NEI(MK);
            break;
        case 0xf0:
            SBI_IO(P_A);
            break;
        case 0xf1:
            SBI_IO(P_B);
            break;
        case 0xf2:
            SBI_IO(P_C);
            break;
        case 0xf3:
            SBI(MK);
            break;
        case 0xf8:
            EQI_IO(P_A);
            break;
        case 0xf9:
            EQI_IO(P_B);
            break;
        case 0xfa:
            EQI_IO(P_C);
            break;
        case 0xfb:
            EQI(MK);
            break;
        default:
            printf(("PC=%4x\tCPU\tUNKNOWN OP : 64 %2x\n"), prvPC, ope);
    }
}
void UPD7801::OP70()
{
    uint8 ope = FETCH8();
    period    = op70[ope].clock;
    switch (ope) {
        case 0x0e:
            WM16(FETCH16(), SP);
            break;
        case 0x0f:
            SP = RM16(FETCH16());
            break;
        case 0x1e:
            WM16(FETCH16(), BC);
            break;
        case 0x1f:
            BC = RM16(FETCH16());
            break;
        case 0x2e:
            WM16(FETCH16(), DE);
            break;
        case 0x2f:
            DE = RM16(FETCH16());
            break;
        case 0x3e:
            WM16(FETCH16(), HL);
            break;
        case 0x3f:
            HL = RM16(FETCH16());
            break;
        case 0x68:
            _V = RM8(FETCH16());
            break;
        case 0x69:
            _A = RM8(FETCH16());
            break;
        case 0x6a:
            _B = RM8(FETCH16());
            break;
        case 0x6b:
            _C = RM8(FETCH16());
            break;
        case 0x6c:
            _D = RM8(FETCH16());
            break;
        case 0x6d:
            _E = RM8(FETCH16());
            break;
        case 0x6e:
            _H = RM8(FETCH16());
            break;
        case 0x6f:
            _L = RM8(FETCH16());
            break;
        case 0x78:
            WM8(FETCH16(), _V);
            break;
        case 0x79:
            WM8(FETCH16(), _A);
            break;
        case 0x7a:
            WM8(FETCH16(), _B);
            break;
        case 0x7b:
            WM8(FETCH16(), _C);
            break;
        case 0x7c:
            WM8(FETCH16(), _D);
            break;
        case 0x7d:
            WM8(FETCH16(), _E);
            break;
        case 0x7e:
            WM8(FETCH16(), _H);
            break;
        case 0x7f:
            WM8(FETCH16(), _L);
            break;
        case 0x89:
            ANAX(BC);
            break;
        case 0x8a:
            ANAX(DE);
            break;
        case 0x8b:
            ANAX(HL);
            break;
        case 0x8c:
            ANAX(DE++);
            break;
        case 0x8d:
            ANAX(HL++);
            break;
        case 0x8e:
            ANAX(DE--);
            break;
        case 0x8f:
            ANAX(HL--);
            break;
        case 0x91:
            XRAX(BC);
            break;
        case 0x92:
            XRAX(DE);
            break;
        case 0x93:
            XRAX(HL);
            break;
        case 0x94:
            XRAX(DE++);
            break;
        case 0x95:
            XRAX(HL++);
            break;
        case 0x96:
            XRAX(DE--);
            break;
        case 0x97:
            XRAX(HL--);
            break;
        case 0x99:
            ORAX(BC);
            break;
        case 0x9a:
            ORAX(DE);
            break;
        case 0x9b:
            ORAX(HL);
            break;
        case 0x9c:
            ORAX(DE++);
            break;
        case 0x9d:
            ORAX(HL++);
            break;
        case 0x9e:
            ORAX(DE--);
            break;
        case 0x9f:
            ORAX(HL--);
            break;
        case 0xa1:
            ADDNCX(BC);
            break;
        case 0xa2:
            ADDNCX(DE);
            break;
        case 0xa3:
            ADDNCX(HL);
            break;
        case 0xa4:
            ADDNCX(DE++);
            break;
        case 0xa5:
            ADDNCX(HL++);
            break;
        case 0xa6:
            ADDNCX(DE--);
            break;
        case 0xa7:
            ADDNCX(HL--);
            break;
        case 0xa9:
            GTAX(BC);
            break;
        case 0xaa:
            GTAX(DE);
            break;
        case 0xab:
            GTAX(HL);
            break;
        case 0xac:
            GTAX(DE++);
            break;
        case 0xad:
            GTAX(HL++);
            break;
        case 0xae:
            GTAX(DE--);
            break;
        case 0xaf:
            GTAX(HL--);
            break;
        case 0xb1:
            SUBNBX(BC);
            break;
        case 0xb2:
            SUBNBX(DE);
            break;
        case 0xb3:
            SUBNBX(HL);
            break;
        case 0xb4:
            SUBNBX(DE++);
            break;
        case 0xb5:
            SUBNBX(HL++);
            break;
        case 0xb6:
            SUBNBX(DE--);
            break;
        case 0xb7:
            SUBNBX(HL--);
            break;
        case 0xb9:
            LTAX(BC);
            break;
        case 0xba:
            LTAX(DE);
            break;
        case 0xbb:
            LTAX(HL);
            break;
        case 0xbc:
            LTAX(DE++);
            break;
        case 0xbd:
            LTAX(HL++);
            break;
        case 0xbe:
            LTAX(DE--);
            break;
        case 0xbf:
            LTAX(HL--);
            break;
        case 0xc1:
            ADDX(BC);
            break;
        case 0xc2:
            ADDX(DE);
            break;
        case 0xc3:
            ADDX(HL);
            break;
        case 0xc4:
            ADDX(DE++);
            break;
        case 0xc5:
            ADDX(HL++);
            break;
        case 0xc6:
            ADDX(DE--);
            break;
        case 0xc7:
            ADDX(HL--);
            break;
        case 0xc9:
            ONAX(BC);
            break;
        case 0xca:
            ONAX(DE);
            break;
        case 0xcb:
            ONAX(HL);
            break;
        case 0xcc:
            ONAX(DE++);
            break;
        case 0xcd:
            ONAX(HL++);
            break;
        case 0xce:
            ONAX(DE--);
            break;
        case 0xcf:
            ONAX(HL--);
            break;
        case 0xd1:
            ADCX(BC);
            break;
        case 0xd2:
            ADCX(DE);
            break;
        case 0xd3:
            ADCX(HL);
            break;
        case 0xd4:
            ADCX(DE++);
            break;
        case 0xd5:
            ADCX(HL++);
            break;
        case 0xd6:
            ADCX(DE--);
            break;
        case 0xd7:
            ADCX(HL--);
            break;
        case 0xd9:
            OFFAX(BC);
            break;
        case 0xda:
            OFFAX(DE);
            break;
        case 0xdb:
            OFFAX(HL);
            break;
        case 0xdc:
            OFFAX(DE++);
            break;
        case 0xdd:
            OFFAX(HL++);
            break;
        case 0xde:
            OFFAX(DE--);
            break;
        case 0xdf:
            OFFAX(HL--);
            break;
        case 0xe1:
            SUBX(BC);
            break;
        case 0xe2:
            SUBX(DE);
            break;
        case 0xe3:
            SUBX(HL);
            break;
        case 0xe4:
            SUBX(DE++);
            break;
        case 0xe5:
            SUBX(HL++);
            break;
        case 0xe6:
            SUBX(DE--);
            break;
        case 0xe7:
            SUBX(HL--);
            break;
        case 0xe9:
            NEAX(BC);
            break;
        case 0xea:
            NEAX(DE);
            break;
        case 0xeb:
            NEAX(HL);
            break;
        case 0xec:
            NEAX(DE++);
            break;
        case 0xed:
            NEAX(HL++);
            break;
        case 0xee:
            NEAX(DE--);
            break;
        case 0xef:
            NEAX(HL--);
            break;
        case 0xf1:
            SBBX(BC);
            break;
        case 0xf2:
            SBBX(DE);
            break;
        case 0xf3:
            SBBX(HL);
            break;
        case 0xf4:
            SBBX(DE++);
            break;
        case 0xf5:
            SBBX(HL++);
            break;
        case 0xf6:
            SBBX(DE--);
            break;
        case 0xf7:
            SBBX(HL--);
            break;
        case 0xf9:
            EQAX(BC);
            break;
        case 0xfa:
            EQAX(DE);
            break;
        case 0xfb:
            EQAX(HL);
            break;
        case 0xfc:
            EQAX(DE++);
            break;
        case 0xfd:
            EQAX(HL++);
            break;
        case 0xfe:
            EQAX(DE--);
            break;
        case 0xff:
            EQAX(HL--);
            break;
        default:
            printf(("PC=%4x\tCPU\tUNKNOWN OP : 70 %2x\n"), prvPC, ope);
    }
}
void UPD7801::OP74()
{
    uint8 ope = FETCH8();
    period    = op74[ope].clock;
    switch (ope) {
        case 0x88:
            ANAW();
            break;
        case 0x90:
            XRAW();
            break;
        case 0x98:
            ORAW();
            break;
        case 0xa0:
            ADDNCW();
            break;
        case 0xa8:
            GTAW();
            break;
        case 0xb0:
            SUBNBW();
            break;
        case 0xb8:
            LTAW();
            break;
        case 0xc0:
            ADDW();
            break;
        case 0xc8:
            ONAW();
            break;
        case 0xd0:
            ADCW();
            break;
        case 0xd8:
            OFFAW();
            break;
        case 0xe0:
            SUBW();
            break;
        case 0xe8:
            NEAW();
            break;
        case 0xf0:
            SBBW();
            break;
        case 0xf8:
            EQAW();
            break;
        default:
            printf(("PC=%4x\tCPU\tUNKNOWN OP : 74 %2x\n"), prvPC, ope);
    }
}
