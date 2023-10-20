#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <filesystem>

using namespace std;
using filesystem::directory_iterator;

int countFilesWithExtension(const string& path, const string& extension) 
{
    int count = 0;
    for (const auto& entry : directory_iterator(path)) 
    {
        if (entry.path().extension() == extension)
        {
            cout << entry.path() << endl;
            count++;
        }
    }
    return count;
}

int main() 
{
    string path;
    string extension;

    cout << "디렉토리 경로를 입력하세요: ";
    cin >> path;

    cout << "확장자를 입력하세요 (예: .txt, .cpp, .jpg 등): ";
    cin >> extension;

    int fileCount = countFilesWithExtension(path, extension);
    cout << "디렉토리 내 " << extension << " 확장자를 가진 파일 수: " << fileCount << endl;

    return 0;
}