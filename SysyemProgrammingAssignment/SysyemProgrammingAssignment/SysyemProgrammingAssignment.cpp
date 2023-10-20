#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <locale>
#include <codecvt>

using namespace std;

int main()
{
    // 콘솔 출력을 UTF-8로 설정
    // 유니코드 문자열을 콘솔에 직접 출력하면 제대로 표시되지 않을 수 있기 때문
    SetConsoleOutputCP(CP_UTF8); 

    TCHAR test[] = _T("안녕하세요 HELLO");
    char test2[] = "안녕하세요 HELLO";
    char test3[] = u8"안녕하세요 HELLO";

    // UTF-8로 인코딩된 문자열을 직접 유니코드로 변환
    wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    wstring unicode_string = converter.from_bytes(test3);

    wcout.imbue(locale("ko_KR.UTF-8")); // 유니코드 출력을 위한 locale 설정
    wcout << unicode_string << ", " << sizeof(test3) << endl;
}
