#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <filesystem>

using namespace std;
using filesystem::recursive_directory_iterator;

int main() 
{
    string path;
    string extension;

    while (true)
    {
        cout << "디렉토리의 상대경로 혹은 절대경로를 입력하세요: ";
        cin >> path;

        cout << "확장자를 입력하세요 (예: .cpp, .jpg 등): ";
        cin >> extension;

        try
        {
            int count = 0;
            for (const auto& entry : recursive_directory_iterator(path))
            {
                if (entry.path().extension() == extension)
                {
                    cout << entry.path() << endl;
                    count++;
                }
            }

            cout << "디렉토리 내 " << extension << " 확장자를 가진 파일 수: " << count << endl;
            cout << endl;
        }
        catch (const std::filesystem::filesystem_error& ex)
        {
            cerr << ex.what() << endl;
            cout << endl;
            continue;
        }
    }

    return 0;
}