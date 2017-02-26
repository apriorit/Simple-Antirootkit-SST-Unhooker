#ifndef PE_UTILS_H
#define PE_UTILS_H

#include <ntddk.h>
#include <ntimage.h>
#include "drv_ntdefinitions.h"


typedef NTSTATUS (__stdcall *drv_SectionHandlerType)(PIMAGE_SECTION_HEADER pSection, 
                                                 void * pContext,
                                                 int * bContinueSearch); // out


NTSTATUS drv_EnumSections(char * pModule, 
                                drv_SectionHandlerType pSectionHandler, 
                                void * pContext);
NTSTATUS Drv_ResolveSectionAddress(PVOID pModuleAddress, PVOID pAddress, IMAGE_SECTION_HEADER ** ppSection);

NTSTATUS Drv_FindSection(PVOID pModuleAddress, 
                          UCHAR Name[IMAGE_SIZEOF_SHORT_NAME],
                          IMAGE_SECTION_HEADER ** ppSection);

#endif