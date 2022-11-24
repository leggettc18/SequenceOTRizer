#include "Sequence.h"

ZeldaOTRizer::Sequence::Sequence(std::shared_ptr<Ship::Archive> otrFile, const char *outPath)
    : OTRizer::Resource(otrFile, outPath, 0x4F534551, 2, 0) {}

void ZeldaOTRizer::Sequence::WriteResourceData() {
  writer->Write((uint32_t)this->size);
  writer->Write(rawBinary.data(), this->size);
  writer->Write(sequenceNum);
  writer->Write(medium);
  writer->Write(cachePolicy);
  writer->Write(numFonts);
  for (uint32_t i = 0; i < numFonts; i++) {
    writer->Write(fontIndices[i]);
  }
}