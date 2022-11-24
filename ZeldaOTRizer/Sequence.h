#include "OTRizer/Resource.h"

namespace ZeldaOTRizer {
class Sequence : public OTRizer::Resource {
    protected:
        void WriteResourceData();
    public:
      Sequence(std::shared_ptr<Ship::Archive> otrFile, const char* outPath);
      /**
        * @brief the size of the binary sequence data.
        */
      size_t size;
      /**
       * @brief the binary sequence data.
       */
      std::vector<char> rawBinary;
      /**
       * @brief the ID of the sequence in-game.
       *
       * Ignored in-game for custom sequences.
       */
      uint8_t sequenceNum;
      /**
       * @brief the medium to stream from in-game.
       *
       * Should be set to two for custom sequences representing the MEDIUM_CART
       * enum.
       */
      uint8_t medium;
      /**
       * @brief the cache policy to tell the game how to cache the sequence.
       *
       * This should be 2 (`CACHE_TEMPORARY`) for BGM sequences and 1
       * (`CACHE_PERSISTENT`) for Fanfare Sequences.
       */
      uint8_t cachePolicy;
      /**
       * @brief the number of soundfonts used by this sequence.
       */
      uint32_t numFonts;
      /**
       * @brief the indices of the soundfonts used by this sequence.
       *
       * See [the instrument sets
       * here](https://sites.google.com/site/deathbasketslair/zelda/ocarina-of-time/instrument-lists#TOC-Percussion-maps)
       * for more info on which indices to use.
       */
      std::vector<uint8_t> fontIndices;
};
};