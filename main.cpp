#include "Sequence.h"
#include <fstream>
#include <iostream>


std::vector<char> ReadAllBytes(const std::filesystem::path &filePath) {
  std::ifstream file(filePath, std::ios::in | std::ios::binary | std::ios::ate);

  if (!file)
    return std::vector<char>();

  int32_t fileSize = (int32_t)file.tellg();
  file.seekg(0);
  char *data = new char[fileSize];
  file.read(data, fileSize);
  std::vector<char> result = std::vector<char>(data, data + fileSize);
  delete[] data;

  return result;
};

void ReplaceOriginal(std::string &str, const std::string &from,
                     const std::string &to) {
  size_t start_pos = str.find(from);

  while (start_pos != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos = str.find(from);
  }
}

std::vector<std::string> Split(std::string s, const std::string &delimiter) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
    token = s.substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back(token);
  }

  res.push_back(s.substr(pos_start));
  return res;
}

int main(int argc, char *argv[]) {
  std::filesystem::path customMusicPath = "Custom/Music";
  if (std::filesystem::exists(customMusicPath) &&
      !std::filesystem::exists("custom-music.otr")) {
    std::shared_ptr<Ship::Archive> otrFile =
        Ship::Archive::CreateArchive("custom-music.otr", 40000);
    auto list = std::filesystem::recursive_directory_iterator(customMusicPath);
    for (auto item : list) {
      std::vector<std::string> splitPath =
          Split(item.path().generic_string(), ".");
      if (splitPath.size() >= 2) {
        std::string extension = splitPath.at(splitPath.size() - 1);
        splitPath.pop_back();
        std::string afterPath = std::accumulate(
            splitPath.begin(), splitPath.end(), std::string(""));
        if (extension == "seq" &&
            std::filesystem::exists(afterPath + ".meta")) {
          uint8_t fontIdx;
          std::ifstream metaFile(afterPath + ".meta");
          std::string metaName;
          if (std::getline(metaFile, metaName)) {
            auto tmp = Split(afterPath, "/");
            ReplaceOriginal(metaName, "/", "|");
            tmp[tmp.size() - 1] = metaName;
            afterPath =
                std::accumulate(tmp.begin(), tmp.end(), std::string(),
                                [](std::string lhs, const std::string &rhs) {
                                  return lhs.empty() ? rhs : lhs + '/' + rhs;
                                });
          }
          std::string metaFontIdx;
          if (std::getline(metaFile, metaFontIdx)) {
            fontIdx = stoi(metaFontIdx, nullptr, 16);
          }
          std::string type;
          if (!std::getline(metaFile, type)) {
            type = "bgm";
          }
          afterPath += ("_" + type);
          std::locale loc;
          for (int i = 0; i < type.length(); i++) {
            type[i] = std::tolower(type[i], loc);
          }
          ZeldaOTRizer::Sequence sequence(otrFile, afterPath.c_str());
          std::vector<char> binaryData = ReadAllBytes(item);
          sequence.size = binaryData.size();
          sequence.rawBinary = std::vector<char>(binaryData.size());
          memcpy(sequence.rawBinary.data(), binaryData.data(),
                 binaryData.size());
          sequence.cachePolicy = (type == "bgm" ? 2 : 1);
          sequence.medium = 2;
          sequence.sequenceNum = 0;
          sequence.numFonts = 1;
          sequence.fontIndices = {fontIdx};
          sequence.OTRize();
          printf("musicArchive->AddFile(%s)\n", afterPath.c_str());
        }
      }
    }
  }
  return 0;
}