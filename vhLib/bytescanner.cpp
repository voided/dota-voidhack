
// CByteScanner
// by: christopher thorne

#include "bytescanner.h"

#include <windows.h>


CByteScanner::CByteScanner()
{
    m_ulBaseOfCode = NULL;
    m_ulSizeOfCode = 0;
}

CByteScanner::CByteScanner( const void *lpAddress )
{
    m_ulBaseOfCode = NULL;
    m_ulSizeOfCode = 0;

    FindCodeSegment( lpAddress );
}

CByteScanner::CByteScanner( const char *lpModuleName )
{
    m_ulBaseOfCode = NULL;
    m_ulSizeOfCode = 0;

    FindCodeSegment( GetModuleHandle( lpModuleName ) );
}

bool CByteScanner::FindCodeSegment( const void *lpAddress )
{
    if ( !lpAddress )
            return false;

    MEMORY_BASIC_INFORMATION mbi;

    if ( VirtualQuery( lpAddress, &mbi, sizeof( mbi ) ) == 0 )
            return false;

    size_t ulAllocationBase = (size_t)mbi.AllocationBase;

    if ( !ulAllocationBase )
            return false;

    IMAGE_DOS_HEADER *idh = (IMAGE_DOS_HEADER *)(ulAllocationBase);
    IMAGE_NT_HEADERS *inh = (IMAGE_NT_HEADERS *)(ulAllocationBase+idh->e_lfanew);

    if ( !inh->OptionalHeader.BaseOfCode )
            return false;

    m_ulBaseOfCode = ulAllocationBase + inh->OptionalHeader.BaseOfCode;
    m_ulSizeOfCode = inh->OptionalHeader.SizeOfCode;

    return true;
}

bool CByteScanner::FindCodePattern( const void *lpBytes, const char *lpMask, void **lppOut )
{
    if ( !m_ulBaseOfCode )
            return false;

    unsigned int unLength = (unsigned int)strlen( lpMask );

    for ( unsigned int i = 0; i < m_ulSizeOfCode; i++ )
    {
            bool bMatch = true;

            for ( unsigned int j = 0; j < unLength; j++ )
            {
                    if ( *(char *)(lpMask+j) == 'x' && *(char *)(m_ulBaseOfCode+i+j) != *(((char *)lpBytes)+j) )
                    {
                            bMatch = false;

                            break;
                    }
            }

            if ( bMatch )
            {
                    *lppOut = (void *)(m_ulBaseOfCode+i);
                       
                    return true;
            }
    }

    *lppOut = NULL;

    return false;
}
