#include "pe_utils.h"
#include "ntddk_module.h"

typedef struct _drv_ResolveParams
{
    void * pModuleAddress;
    PVOID pAddress;
    IMAGE_SECTION_HEADER * pSection;
}drv_ResolveParams;
NTSTATUS __stdcall Drv_ResolveSectionAddressFnc(PIMAGE_SECTION_HEADER pSection, 
                                             void * pContext,
                                             int * bContinueSearch)
{
    drv_ResolveParams * pResolveParams = pContext;
    char * pStart = (char *)pResolveParams->pModuleAddress + pSection->VirtualAddress;
    char * pEnd = pStart + pSection->Misc.VirtualSize;

    *bContinueSearch = 1;

    // does pResolveParams->pAddress relate to this section?
    if (pResolveParams->pAddress >= pStart && pResolveParams->pAddress <= pEnd)
    {
        *bContinueSearch = 0;
        pResolveParams->pSection = pSection;
    }
    return STATUS_SUCCESS;
}


NTSTATUS drv_EnumSections(char * pModule, 
                                 drv_SectionHandlerType pSectionHandler, 
                                 void * pContext)
{
    NTSTATUS status = 0;
    PIMAGE_NT_HEADERS pPE = (PIMAGE_NT_HEADERS)(pModule+((PIMAGE_DOS_HEADER)pModule)->e_lfanew);
    short NumberOfSection = pPE->FileHeader.NumberOfSections;
    long SectionAlign=pPE->OptionalHeader.SectionAlignment;
   
    PIMAGE_SECTION_HEADER Section = (PIMAGE_SECTION_HEADER)((char*)&(pPE->FileHeader)+ 
                                                            pPE->FileHeader.SizeOfOptionalHeader+
                                                            sizeof(IMAGE_FILE_HEADER));

    {
        int i = 0, iContinueSearch = 0;
        for (i=0; i<NumberOfSection; ++i)
        {
            status = pSectionHandler(Section++, pContext, &iContinueSearch);
            if (!NT_SUCCESS(status))
                return status;

            if (!iContinueSearch)
                break;
        }
    }
    return status;
}

NTSTATUS Drv_ResolveSectionAddress(PVOID pModuleAddress, PVOID pAddress, IMAGE_SECTION_HEADER ** ppSection)
{
    drv_ResolveParams resolveParams;
    NTSTATUS status = 0;
    resolveParams.pModuleAddress = pModuleAddress;
    resolveParams.pSection = 0;
    resolveParams.pAddress = pAddress;

    status = drv_EnumSections(pModuleAddress, Drv_ResolveSectionAddressFnc, &resolveParams);
    if (!NT_SUCCESS(status))
        return status;

    if (!resolveParams.pSection)
        return STATUS_NOT_FOUND;
    *ppSection = resolveParams.pSection;
    return STATUS_SUCCESS;
}


// find section in file
NTSTATUS Drv_FindSection(PVOID pModuleAddress, 
                          UCHAR Name[IMAGE_SIZEOF_SHORT_NAME],
                          IMAGE_SECTION_HEADER ** ppSection)
{
    PIMAGE_DOS_HEADER dosHeader  =  (PIMAGE_DOS_HEADER)pModuleAddress;
    PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((char*)dosHeader + dosHeader->e_lfanew);
    PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(pNTHeader);

    unsigned i;

    for ( i=0; i < pNTHeader->FileHeader.NumberOfSections; i++, section++ ) 
    {
        int k, found = 1;
        for(k=0;k<IMAGE_SIZEOF_SHORT_NAME;++k)
        {
            if (Name[k] != section->Name[k])
            {
                found = 0;
                break;
            }
        }
        if (found)
        {
            *ppSection = section;
            return STATUS_SUCCESS;
        }
    }
    return STATUS_NOT_FOUND;
}

