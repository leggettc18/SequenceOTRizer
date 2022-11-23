#pragma once
#include <filesystem>
#include "libultraship/libultraship.h"

namespace OTRizer {
class Resource {
    public:
        Resource(const std::filesystem::path& outPath, int resType, int gameVersion = 0, int resVersion = 0);
        Resource(const std::filesystem::path& outPath, int resType);
        std::vector<char> ToVector();
    protected:
        void WriteHeader();
        virtual void WriteResourceData() = 0;
    private:
        const std::filesystem::path outPath;
        int resType;
        int gameVersion;
        int resVersion;
        std::shared_ptr<Ship::BinaryWriter> writer;
};
};