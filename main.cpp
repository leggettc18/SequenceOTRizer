#include "Sequence.h"
#include "Utils.h"
#include "config.h"
#include <iostream>

void PrintVersion() {
    printf("v%d.%d\n", SequenceOTRizer_VERSION_MAJOR, SequenceOTRizer_VERSION_MINOR);
}

void PrintUsage(std::string fileName) {
    PrintVersion();
    printf("Usage: %s --seq-path <seq-path> [--game-path <game-path> --otr-name <otr-name>]\n\n"
           "<seq-path> - path to custom sequences folder\n"
           "<game-path> - (optional) path to the game's executable folder. Defaults to the current directory.\n"
           "<otr-name> - (optional) the filename of the output OTR file *without* the .otr extension. Defaults to \"custom-music\".\n",
           fileName.c_str());
}

int main(int argc, char* argv[]) {
    std::filesystem::path customMusicPath;
    std::filesystem::path gamePath = ".";
    std::string otrName = "custom-music";
    bool seqPathSet = false;

    // Parse arguments.
    if (argc == 1) {
        PrintUsage(argv[0]);
        return 0;
    }

    // Print version.
    if (argc == 2) {
        if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) {
            PrintVersion();
        } else {
            PrintUsage(argv[0]);
            if (strcmp(argv[1], "--help") != 0 && strcmp(argv[1], "-h") != 0) {
                return 1;
            }
        }
        return 0;
    }

    if (argc > 2) {
        int i = 1; // skip the first argument which is the filename.
        while (i < argc) {
            if (strcmp(argv[i], "--seq-path") == 0 && argc > i + 1) {
                i += 1; // next argument (should be the actual path).
                customMusicPath = argv[i];
                seqPathSet = true;
                i += 1;   // check next argument on next iteration.
                continue; // go to next loop iteration.
            }

            if (strcmp(argv[i], "--game-path") == 0 && argc > i + 1) {
                i += 1;
                gamePath = argv[i];
                i += 1;
                continue;
            }

            if (strcmp(argv[i], "--otr-name") == 0 && argc > i + 1) {
                i += 1;
                otrName = argv[i];
                i += 1;
                continue;
            }
        }
        if (!seqPathSet) {
            std::cout << "Missing --seqPath" << std::endl;
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

    // Ask to delete custom music otr if it already exists.
    std::filesystem::path outputPath = gamePath / "mods" / otrName += ".otr";
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
        std::vector<std::filesystem::path> folders;
        for (auto item : list) {
            if (std::filesystem::is_directory(item.path())) {
                folders.push_back(item.path());
            }
        }
        for (auto folder : folders) {
            std::filesystem::path seqPath;
            bool valid = false;
            for (auto file : std::filesystem::directory_iterator(folder)) {
                if (file.path().extension() == ".zbank") {
                    std::cerr << file.path().generic_string() << " is an unsupported soundbank mod! Skipping." << std::endl;
                    valid = false;
                    break;
                }
                if (file.path().extension() == ".seq" && std::filesystem::exists(file.path().parent_path() / file.path().stem() += ".meta")) {
                    seqPath = file.path();
                    valid = true;
                }
            }
            if (valid) {
                ZeldaOTRizer::Sequence sequence = ZeldaOTRizer::Sequence::FromSeqFile(otrFile, seqPath);
                // Output the sequence resource to the OTR File.
                sequence.OTRize();
                printf("musicArchive->AddFile(%s)\n", sequence.outPath.c_str());
            }
        }
    } else {
        std::cout << customMusicPath.generic_string() << " cannot be found." << std::endl;
        return 1;
    }
    return 0;
}
