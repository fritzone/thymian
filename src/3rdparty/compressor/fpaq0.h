#ifndef FPAQ0
#define FPAQ0

#include <vector>
#include <string>
#include <stdint.h>

#include <string.h>

class Archive
{
public:

    Archive(const std::vector<unsigned char>& s) : content(s.begin(), s.end()), it(content.begin()) {}
    Archive() : content(), it(content.begin()) {}

    int getc() const
    {
        if(it == content.end())
        {
            return EOF;
        }
        int v = static_cast<int> (*it);
        ++ it;
        return v;
    }

    void putc(char c)
    {
        content.push_back(c);
    }

    std::vector<unsigned char> data() const
    {
        return content;
    }

private:
    std::vector<unsigned char> content;         // Compressed data file
    mutable std::vector<unsigned char>::iterator it;
};

//////////////////////////// Predictor /////////////////////////

/* A Predictor estimates the probability that the next bit of
   uncompressed data is 1.  Methods:
   p() returns P(1) as a 12 bit number (0-4095).
   update(y) trains the predictor with the actual bit (0 or 1).
*/

class Predictor
{
    int cxt;  // Context: last 0-8 bits with a leading 1
    int ct[512][2];  // 0 and 1 counts in context cxt
public:
    Predictor(): cxt(1)
    {
        memset(ct, 0, sizeof(ct));
    }

    // Assume a stationary order 0 stream of 9-bit symbols
    int p() const;

    void update(int y);
};


//////////////////////////// Encoder ////////////////////////////

/* An Encoder does arithmetic encoding.  Methods:
   Encoder(COMPRESS, f) creates encoder for compression to archive f, which
     must be open past any header for writing in binary mode
   Encoder(DECOMPRESS, f) creates encoder for decompression from archive f,
     which must be open past any header for reading in binary mode
   encode(bit) in COMPRESS mode compresses bit to file f.
   decode() in DECOMPRESS mode returns the next decompressed bit from file f.
   flush() should be called when there is no more to compress.
*/

typedef enum {COMPRESS, DECOMPRESS} Mode;
class Encoder
{
public:
    Encoder(Mode m, Archive& archive);
    void encode(int y);    // Compress bit y
    int decode();          // Uncompress and return bit y
    void flush();          // Call when done compressing
private:
    Predictor predictor;
    const Mode mode;       // Compress or decompress?
    Archive& archive;
    uint32_t x1, x2;            // Range, initially [0, 1), scaled by 2^32
    uint32_t x;                 // Last 4 input bytes of archive.
};

std::vector<unsigned char> compress(const std::string& s);

std::string decompress(const std::vector<unsigned char>& v);

#endif // FPAQ0

