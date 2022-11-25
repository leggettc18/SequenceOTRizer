#include "Sequence.h"
#include <fstream>
#include <iostream>

std::vector<char> ReadAllBytes(const std::filesystem::path& filePath) {
    std::ifstream file(filePath, std::ios::in | std::ios::binary | std::ios::ate);

    if (!file) {
        return std::vector<char>();
    }

    int32_t fileSize = (int32_t)file.tellg();
    file.seekg(0);
    char* data = new char[fileSize];
    file.read(data, fileSize);
    std::vector<char> result = std::vector<char>(data, data + fileSize);
    delete[] data;

    return result;
};

void ReplaceOriginal(std::string& str, const std::string& from, const std::string& to) {
    size_t startPos = str.find(from);

    while (startPos != std::string::npos) {
        str.replace(startPos, from.length(), to);
        startPos = str.find(from);
    }
}

std::vector<std::string> Split(std::string s, const std::string& delimiter) {
    size_t posStart = 0, posEnd, delimLen = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((posEnd = s.find(delimiter, posStart)) != std::string::npos) {
        token = s.substr(posStart, posEnd - posStart);
        posStart = posEnd + delimLen;
        res.push_back(token);
    }

    res.push_back(s.substr(posStart));
    return res;
}

void PrintUsage(std::string fileName) {
    printf("Usage: %s --seq-path <seq-path> --game-path <game-path>\n\n"
           "<seq-path> - path to custom sequences folder\n"
           "<game-path> - path to the game's executable folder\n",
           fileName.c_str());
}

int main(int argc, char* argv[]) {
    std::filesystem::path customMusicPath;
    std::filesystem::path gamePath;

    // Parse arguments.
    if (argc < 5) {
        PrintUsage(argv[0]);
    }

    if (argc == 5) {
        int i = 1; // skip the first argument which is the filename.
        while (i < argc) {
            if (strcmp(argv[i], "--seq-path") == 0) {
                i += 1; // next argument (should be the actual path).
                customMusicPath = argv[i];
                i += 1;   // check next argument on next iteration.
                continue; // go to next loop iteration.
            }

            if (strcmp(argv[i], "--game-path") == 0) {
                i += 1;
                gamePath = argv[i];
                i += 1;
                continue;
            }

            std::cout << "Invalid Arguments" << std::endl;
            PrintUsage(argv[0]);
            return 1;
        }
    }

    // Create mods folder if it doesn't already exist.
    if (!std::filesystem::exists(gamePath / "mods")) {
        try {
            std::filesystem::create_directory(gamePath / "mods");
        } catch (std::filesystem::filesystem_error e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
    }

    // Ask to delete custom-music.otr if it already exists.
    std::filesystem::path outputPath = gamePath / "mods" / "custom-music.otr";
    if (std::filesystem::exists(outputPath)) {
        std::cout << outputPath.generic_string() << " already exists. Would you like to overwrite it? (y/n): ";
        for (std::string answer; std::getline(std::cin, answer);) {
            if (answer == "y") {
                std::filesystem::remove(outputPath);
                break;
            }
            if (answer == "n") {
                return 0;
            }
            printf("Invalid response: \"%s\". Please enter \"y\" for yes or \"n\" for no: \n", answer.c_str());
        }
    }

    // Create sequence resources and write them to the OTR file if the path to the custom sequences exists.
    if (std::filesystem::exists(customMusicPath)) {
        // Create the archive file.
        std::shared_ptr<Ship::Archive> otrFile = Ship::Archive::CreateArchive(outputPath.generic_string(), 40000);
        auto list = std::filesystem::recursive_directory_iterator(customMusicPath);
        // Ensure directory is not empty.
        if (std::filesystem::begin(list) == std::filesystem::end(list)) {
            std::cerr << customMusicPath << " is empty!" << std::endl;
            return 1;
        }
        // Iterate over all files in the custom sequences path.
        for (auto item : list) {
            // Use the existing path and the sequence's corresponding .meta file to construct the resource
            // and OTR Database Name.
            std::vector<std::string> splitPath = Split(item.path().generic_string(), ".");
            // Check if file has an extension and extract it.
            if (splitPath.size() >= 2) {
                std::string extension = splitPath.at(splitPath.size() - 1);
                splitPath.pop_back();
                std::string afterPath = std::accumulate(splitPath.begin(), splitPath.end(), std::string(""));
                // Proceed if the extension is .seq and a .meta file of the same name also exists.
                if (extension == "seq") {
                    if (!std::filesystem::exists(afterPath + ".meta")) {
                        std::cerr << item.path().generic_string()
                                  << " does not have a corresponding .meta file! Skipping.";
                        continue;
                    }
                    std::string metaName;
                    uint8_t fontIdx;
                    // Parse the .meta file.
                    std::ifstream metaFile(afterPath + ".meta");
                    // Replace the file name with the name from the .meta file.
                    if (std::getline(metaFile, metaName)) {
                        auto tmp = Split(afterPath, "/");
                        ReplaceOriginal(metaName, "/", "|");
                        tmp[tmp.size() - 1] = metaName;
                        afterPath = std::accumulate(tmp.begin(), tmp.end(), std::string(),
                                                    [](std::string lhs, const std::string& rhs) {
                                                        return lhs.empty() ? rhs : lhs + '/' + rhs;
                                                    });
                    }
                    // Get the font(s) from the .meta file eventually this should support multiple,
                    // but for now only one.
                    std::string metaFontIdx;
                    if (std::getline(metaFile, metaFontIdx)) {
                        fontIdx = stoi(metaFontIdx, nullptr, 16);
                    }
                    // Get the sequence type (for now it will be either bgm or fanfare).
                    std::string type;
                    if (!std::getline(metaFile, type)) {
                        type = "bgm";
                    }
                    // append the type to the database name for later extraction by the game.
                    std::locale loc;
                    for (int i = 0; i < type.length(); i++) {
                        type[i] = std::tolower(type[i], loc);
                    }
                    afterPath += ("_" + type);
                    // Create the Sequence Resource.
                    ZeldaOTRizer::Sequence sequence(otrFile, afterPath.c_str());
                    std::vector<char> binaryData = ReadAllBytes(item);
                    sequence.size = binaryData.size();
                    sequence.rawBinary = std::vector<char>(binaryData.size());
                    memcpy(sequence.rawBinary.data(), binaryData.data(), binaryData.size());
                    sequence.cachePolicy = (type == "bgm" ? 2 : 1);
                    sequence.medium = 2;
                    sequence.sequenceNum = 0;
                    sequence.numFonts = 1;
                    sequence.fontIndices = { fontIdx };
                    // Output the sequence resource to the OTR File.
                    sequence.OTRize();
                    printf("musicArchive->AddFile(%s)\n", afterPath.c_str());
                }
            }
        }
    } else {
        std::cout << customMusicPath.generic_string() << " cannot be found." << std::endl;
        return 1;
    }
    return 0;
}