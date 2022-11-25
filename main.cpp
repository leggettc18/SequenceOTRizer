#include "Sequence.h"
#include "Utils.h"
#include <iostream>

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
            std::vector<std::string> splitPath = StringUtils::Split(item.path().generic_string(), ".");
            // Check if file has an extension and extract it.
            if (splitPath.size() >= 2) {
                std::string extension = splitPath.at(splitPath.size() - 1);
                splitPath.pop_back();
                std::string afterPath = std::accumulate(
                    splitPath.begin(), splitPath.end(), std::string(),
                    [](std::string lhs, const std::string& rhs) { return lhs.empty() ? rhs : lhs + '.' + rhs; });
                // Proceed if the extension is .seq and a .meta file of the same name also exists.
                if (extension == "seq") {
                    if (!std::filesystem::exists(afterPath + ".meta")) {
                        std::cerr << item.path().generic_string()
                                  << " does not have a corresponding .meta file! Skipping.";
                        return 1;
                    }
                    ZeldaOTRizer::Sequence sequence = ZeldaOTRizer::Sequence::FromSeqFile(otrFile, item.path());
                    // Output the sequence resource to the OTR File.
                    sequence.OTRize();
                    printf("musicArchive->AddFile(%s)\n", sequence.outPath.c_str());
                }
            }
        }
    } else {
        std::cout << customMusicPath.generic_string() << " cannot be found." << std::endl;
        return 1;
    }
    return 0;
}