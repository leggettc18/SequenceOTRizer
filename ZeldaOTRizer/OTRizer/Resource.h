#pragma once
#include <filesystem>
#include "libultraship/libultraship.h"

namespace OTRizer {
class Resource {
    public:
        Resource(const std::filesystem::path& outPath, int resType, int gameVersion = 0, int resVersion = 0);
        Resource(const std::filesystem::path& outPath, int resType);
    protected:
        void WriteHeader();
        virtual void WriteMetadata() = 0;
    private:
        const std::filesystem::path outPath;
        int resType;
        int gameVersion;
        int resVersion;
        std::shared_ptr<Ship::BinaryWriter> writer;
};
};