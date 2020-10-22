// fpaq0 - Stationary order 0 file compressor.
// (C) 2004, Matt Mahoney under GPL, http://www.gnu.org/licenses/gpl.txt
// To compile: g++ -O fpaq0.cpp

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cassert>
#include <iostream>
#include "fpaq0.h"

using namespace std;


// Constructor
Encoder::Encoder(Mode m, Archive &parchive): predictor(), mode(m), archive(parchive), x1(0),
    x2(0xffffffff), x(0)
{

    // In DECOMPRESS mode, initialize x to the first 4 bytes of the archive
    if (mode==DECOMPRESS)
    {
        for (int i=0; i<4; ++i)
        {
            int c = archive.getc();
            if (c==EOF) c=0;
            x=(x<<8)+(c&0xff);
        }
    }
}

/* encode(y) -- Encode bit y by splitting the range [x1, x2] in proportion
to P(1) and P(0) as given by the predictor and narrowing to the appropriate
subrange.  Output leading bytes of the range as they become known. */

inline void Encoder::encode(int y)
{

    // Update the range
    const uint32_t xmid = x1 + ((x2-x1) >> 12) * predictor.p();
    assert(xmid >= x1 && xmid < x2);
    if (y)
        x2=xmid;
    else
        x1=xmid+1;
    predictor.update(y);

    // Shift equal MSB's out
    while ( ((x1^x2) & 0xff000000) == 0)
    {
        archive.putc(x2>>24);
        x1 <<= 8;
        x2 = (x2<<8) + 255;
    }
}

/* Decode one bit from the archive, splitting [x1, x2] as in the encoder
and returning 1 or 0 depending on which subrange the archive point x is in.
*/
inline int Encoder::decode()
{

    // Update the range
    const uint32_t xmid = x1 + ((x2-x1) >> 12) * predictor.p();
    assert(xmid >= x1 && xmid < x2);
    int y=0;
    if (x<=xmid)
    {
        y=1;
        x2=xmid;
    }
    else
        x1=xmid+1;
    predictor.update(y);

    // Shift equal MSB's out
    while (((x1^x2)&0xff000000)==0)
    {
        x1 <<= 8;
        x2=(x2<<8)+255;
        int c = archive.getc();
        if (c==EOF) c=0;
        x = (x<<8) + c;
    }
    return y;
}

// Should be called when there is no more to compress
void Encoder::flush()
{

    // In COMPRESS mode, write out the remaining bytes of x, x1 < x < x2
    if (mode==COMPRESS)
    {
        while (((x1^x2)&0xff000000)==0)
        {
            archive.putc(x2>>24);
            x1 <<= 8;
            x2 = (x2<<8) + 255;
        }
        archive.putc(x2>>24);  // First unequal byte
    }
}

std::vector<unsigned char> compress(const std::string& s)
{
    Archive a;
    Encoder e(COMPRESS, a);
    char c = 0;
    size_t idx = 0;
    while ( idx < s.length() )
    {
        c = s[idx];
        e.encode(0);
        for (int i=7; i>=0; --i)
        {
            e.encode((c>>i)&1);
        }
        idx ++;
    }
    e.encode(1);  // EOF code
    e.flush();

    return a.data();
}

std::string decompress(const std::vector<unsigned char>& v)
{
    Archive a(v);
    std::string result;
    Encoder e(DECOMPRESS, a);
    while (!e.decode())
    {
        int c=1;
        while (c<256)
            c += c + e.decode();

        char temp[2] = {0};
        temp[0] = c - 256;

        result += temp;
    }
    return result;
}

int Predictor::p() const
{
    return 4096*(ct[cxt][1]+1)/(ct[cxt][0]+ct[cxt][1]+2);
}

void Predictor::update(int y)
{
    if (++ct[cxt][y] > 65534)
    {
        ct[cxt][0] >>= 1;
        ct[cxt][1] >>= 1;
    }
    if ((cxt+=cxt+y) >= 512)
        cxt=1;
}
