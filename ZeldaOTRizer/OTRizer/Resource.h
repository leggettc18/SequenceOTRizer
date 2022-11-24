#pragma once
#include <filesystem>
#include "libultraship/libultraship.h"

namespace OTRizer {
class Resource {
    public:
        Resource(std::shared_ptr<Ship::Archive>, const std::string& outPath, int resType, int gameVersion, int resVersion);
        Resource(std::shared_ptr<Ship::Archive>, const std::string& outPath, int resType);

        /**
         * @brief Path of the file that will eventually be written to the OTR file.
         */
        const std::string outPath;
        /**
         * @brief An integer representing the type of the resource.
         * 
         * The mapping of integer to resource is defined by the game the
         * resource is intended for.
         */
        uint32_t resType;
        /**
         * @brief An integer representing the game version the resource was generated for.
         *
         * The mapping of integer to game version is defined by the game the
         * resource is intended for.
         */
        uint32_t gameVersion;
        /**
         * @brief An integer representing the version of the resource's binary format that is stored in the file.
         *
         * The mapping of integer to resource version is defined by the game the
         * resource is intended for.
         */
        uint32_t resVersion;

        /**
         * @brief Writes the header and resource data into the OTR File
         * the resource is pointing to.
         */
        void OTRize();
    protected:
      /**
       * @brief The Archive instance for the OTRFile that the resource will be written to.
       * 
       */
      std::shared_ptr<Ship::Archive> otrFile;
      std::shared_ptr<Ship::BinaryWriter> writer;

      /**
       * @brief Writes the Header data to the Resource's BinaryWriter stream.
       *
       * The header data includes the endianness of the binary data, the type of
       * the resource, the version of the game it was generated for, and the
       * binary format of the resource. It also includes some space reserved for
       * more data to be added in the future. The header uses the first 0x40
       * bytes of the stream.
       */
      void WriteHeader();
      /**
       * @brief Writes the resource's binary representation to the writer.
       *
       * This binary representation may include metadata as well as the raw
       * binary data of the resource itself.
       */
      virtual void WriteResourceData() = 0;
      /**
       * @brief Returns the raw binary data stored in the Resource's
       * BinaryWriter stream.
       *
       * @return std::vector<char>
       */
      std::vector<char> ToVector();;
};
};