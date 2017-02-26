#ifndef DRV_COMMON_PORTABLE_DEFS_H
#define DRV_COMMON_PORTABLE_DEFS_H


// drvUnhookerDevice
#define DRV_UNHOOKER_DEVICE_TYPE 0x8100

#define DRV_UNHOOKER_UNHOOK CTL_CODE( \
	DRV_UNHOOKER_DEVICE_TYPE,0x08000, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define DRV_UNHOOKER_GET_STATUS CTL_CODE( \
	DRV_UNHOOKER_DEVICE_TYPE,0x08001, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define DRV_UNHOOKER_DEVICE_NAME             L"\\Device\\drvUnhooker"
#define DRV_UNHOOKER_DEVICE_SYMBOLIC_LINK    L"\\??\\drvUnhooker"
#define DRV_UNHOOKER_USERMODE_SYMBOLIC_LINK  L"\\\\.\\drvUnhooker"

#define SERVICE_NAME      L"drvUnhooker"
#define DRIVER_FILENAME   L"drvUnhooker.sys"

#pragma pack(push, 1)

typedef struct _DRV_REPORT
{
    __int32 m_countOfModifiedEntries;
    __int32 m_countOfReportEntries;
    __int32 m_reportEntryOffsets;
    __int32 m_sizeOfReportEntry;
}DRV_REPORT;

typedef struct _DRV_REPORT_ENTRY
{
    __int32  m_sizeOfEntry;
    unsigned __int64 m_moduleAddress;
    unsigned __int64 m_newAddress;
    __int32  m_nameOffset;
    __int32  m_nameSize;
    __int32  m_sstIndex;
}DRV_REPORT_ENTRY;

#pragma pack(pop)

#endif