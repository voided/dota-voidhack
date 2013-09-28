
// CByteScanner
// by: christopher thorne


#pragma once


class CByteScanner
{

public:
        CByteScanner();
        CByteScanner( const void *lpAddress );
        CByteScanner( const char *lpModuleName );

        bool FindCodeSegment( const void *lpAddress );
        bool FindCodePattern( const void *lpBytes, const char *lpMask, void **lppOut );

private:
        size_t m_ulBaseOfCode; // Base address of code segment
        size_t m_ulSizeOfCode; // Size (in bytes) of code segment

};
