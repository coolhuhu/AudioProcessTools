#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <iomanip>
#include <sys/stat.h>


extern "C" {
    #include "libavformat/avformat.h"
}


using namespace std;

// 获取单个文件的时长
double getDuration(const string& filename) {
    AVFormatContext* formatContext = nullptr;
    if (avformat_open_input(&formatContext, filename.c_str(), nullptr, nullptr) != 0) {
        cerr << "Could not open file: " << filename << endl;
        return -1;
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        cerr << "Could not find stream information: " << filename << endl;
        avformat_close_input(&formatContext);
        return -1;
    }

    double duration = formatContext->duration / static_cast<double>(AV_TIME_BASE);
    avformat_close_input(&formatContext);
    return duration;
}

// 递归遍历目录并统计时长
void traverseDirectory(const string& path, vector<string>& files) {
    DIR* dir = opendir(path.c_str());
    if (!dir) {
        cerr << "Could not open directory: " << path << endl;
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        string name = entry->d_name;
        if (name == "." || name == "..") continue;

        string fullPath = path + "/" + name;
        struct stat statbuf;
        if (stat(fullPath.c_str(), &statbuf) != 0) {
            cerr << "Could not stat file: " << fullPath << endl;
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            traverseDirectory(fullPath, files);
        } else {
            files.push_back(fullPath);
        }
    }

    closedir(dir);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <file_or_directory>" << endl;
        return 1;
    }

    string input = argv[1];
    struct stat statbuf;
    if (stat(input.c_str(), &statbuf) != 0) {
        cerr << "Could not stat input: " << input << endl;
        return 1;
    }

    vector<string> files;
    if (S_ISDIR(statbuf.st_mode)) {
        traverseDirectory(input, files);
    } else {
        files.push_back(input);
    }

    double totalDuration = 0;
    long count_wav = 0;
    for (const string& file : files) {
        double duration = getDuration(file);
        if (duration >= 0) {
            cout << "File: " << file << ", Duration: " 
                 << fixed << setprecision(2) 
                 << duration << " seconds" << endl;
            totalDuration += duration;
            ++count_wav;
        }
    }

    cout << "Total " << count_wav << " audio files" << endl;
    cout << "Total Duration: " << fixed << setprecision(2) 
         << totalDuration << " seconds" << endl;
    if (!files.empty()) {
        cout << "Average Duration: " << fixed << setprecision(2) 
             << totalDuration / files.size() << " seconds" << endl;
    }

    return 0;
}