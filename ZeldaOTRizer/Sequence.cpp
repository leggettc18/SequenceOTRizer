#include "Sequence.h"

void ZeldaOTRizer::Sequence::WriteResourceData() {
  writer->Write((uint32_t)size);
  writer->Write(rawBinary.data(), size);
  writer->Write(sequenceNum);
  writer->Write(medium);
  writer->Write(cachePolicy);
  writer->Write(numFonts);
  for (uint32_t i = 0; i < numFonts; i++) {
    writer->Write(fontIndices[i]);
  }
}