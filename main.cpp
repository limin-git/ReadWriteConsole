// ReadWriteConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// 㘵
// Unicode                      1200	utf-16	    3536
// Unicode (Big-Endian)	        1201	utf-16BE	3635
// Unicode (UTF-32)	            12000   utf-32	    35360000
// Unicode (UTF-32 Big-Endian)	12001	utf-32BE	00003635
// 简体中文(GB18030)            54936	GB18030	    8230A839
// Unicode (UTF-7)	            65000	utf-7	    2B4E6A552D
// Unicode (UTF-8)	            65001	utf-8	    E398B5


std::wstring to_wstring( const std::string& s, int code_page )
{
    std::wstring ws;
    int wsize = MultiByteToWideChar( code_page, 0, s.c_str(), s.size(), 0, 0 );
    ws.resize( wsize );
    MultiByteToWideChar( code_page, 0, s.c_str(), s.size(), const_cast<wchar_t*>(ws.c_str()), wsize );
    return ws;
}


std::string to_string( const std::wstring& ws, int code_page )
{
    std::string s;
    int size = WideCharToMultiByte( code_page, 0, ws.c_str(), ws.size(), 0, 0, 0, 0 );
    s.resize( size );
    WideCharToMultiByte( code_page, 0, ws.c_str(), ws.length(), const_cast<char*>(s.c_str()), size, 0, 0 );
    return s;
}


// ReadConsole: it can read in either Unicode (wide-character) or ANSI mode
//              can not read in other mode like GB18030(54936)
std::wstring read_string()
{
    wchar_t s[1024] = { 0 };
    unsigned long size = 0;
    ReadConsoleW( GetStdHandle(STD_INPUT_HANDLE), s, 1024, &size, NULL );
    return std::wstring( s );
}


void output_string( const std::wstring& ws )
{
    unsigned long size = 0;
    WriteConsoleW( GetStdHandle(STD_OUTPUT_HANDLE), ws.c_str(), ws.size(), &size, NULL );
}


void output_string( const std::string& s, int code_page )
{
    std::wstring ws = to_wstring( s, code_page );
    output_string( ws );
}


// CharNextExA only dealing with East Asian, DBCS code page, not support UTF-8, GB18030
void output_encoding( const std::string& s, int code_page )
{
    const char* p = s.c_str();
    const char* n = CharNextExA( code_page, p, 0 );

    while( *p )
    {
        for ( ; p != n; ++p )
        {
            std::cout << std::setiosflags(std::ios::uppercase) << std::setw(2) << std::setfill('0') << std::hex << (unsigned short)(*(unsigned char*)p);
        }

        if ( *p == 0 )
        {
            break;
        }

        std::cout << " " << std::flush;

        p = n;
        n = CharNextExA( code_page, p, 0 );
    }
}


void output_encoding2( const std::string& s, int code_page )
{
    std::wstring ws = to_wstring( s, code_page );

    for ( size_t i = 0; i < ws.size(); ++i )
    {
        std::wstring tws( 1, ws[i] );
        std::string ts = to_string( tws, code_page );

        for ( size_t j = 0; j < ts.size(); ++j )
        {
             std::cout << std::setiosflags(std::ios::uppercase) << std::setw(2) << std::setfill('0') << std::hex << (unsigned short)((unsigned char)ts[j]);
        }

        std::cout << " ";
    }
}


int _tmain(int argc, _TCHAR* argv[])
{
    std::wstring ws = read_string();
    output_string( ws );

    std::cout << "GBK    : ";
    output_encoding2( to_string( ws, 936 ), 936 );
    std::cout << std::endl;

    std::cout << "UTF-8  : ";
    output_encoding2( to_string( ws, CP_UTF8 ), CP_UTF8 );
    std::cout << std::endl;

    std::cout << "GB18030: ";
    output_encoding2( to_string( ws, 54936 ), 54936 );
    std::cout << std::endl;

	return 0;
}
