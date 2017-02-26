#include "cmnUtils.h"
#include "cmnDriver.h"
#include "drvCommonPortableDefs.h"
#include "iomanip"

void outHex(size_t value)
{
    int iSize = sizeof(value)*2;
    std::cout<< std::hex << std::setw(iSize)
        << std::setfill('0')
        << value;
}

int do_stat(int argc, wchar_t * argv[])
{
    cmn::CDriver driver(DRV_UNHOOKER_USERMODE_SYMBOLIC_LINK);
    DWORD dwRes = 0;

    std::vector<char> data(1024*1024);

    driver.SendIoCtl(DRV_UNHOOKER_GET_STATUS, &data.front(), (DWORD)data.size(), 0, &dwRes);

    DRV_REPORT * pReport = (DRV_REPORT * )&data.front();
    if (pReport->m_countOfModifiedEntries)
    {
        std::cout<<"Result: SST IS PATCHED!\n"<<pReport->m_countOfModifiedEntries<<" entries modified\n--------- ";
        std::cout<<pReport->m_countOfReportEntries<<" entries reported:\n";

        DRV_REPORT_ENTRY * pEntryBegin =  (DRV_REPORT_ENTRY *)((char *)pReport + pReport->m_reportEntryOffsets);
        DRV_REPORT_ENTRY * pCurrentEntry = pEntryBegin;
        for(int i = 0; i< pReport->m_countOfReportEntries; ++i)
        {
            char * pName = (char*)pCurrentEntry + pCurrentEntry->m_nameOffset;
            std::string str(pName,  pName + pCurrentEntry->m_nameSize);
            outHex(pCurrentEntry->m_sstIndex);
            std::cout<<"   module: "<<str<<"   module address: ";
            outHex((size_t)pCurrentEntry->m_moduleAddress);
            std::cout<<"   hooked address: ";
            outHex((size_t)pCurrentEntry->m_newAddress);
            std::cout<<"\n";

            pCurrentEntry = (DRV_REPORT_ENTRY *)((char*)pCurrentEntry + pCurrentEntry->m_sizeOfEntry);
        }
        std::cout<<"\n";
    }
    else
    {
        std::cout<<"Result: SST is clean \n\n";
    }
    return 0;
}

int do_unhook(int argc, wchar_t * argv[])
{
    cmn::CDriver driver(DRV_UNHOOKER_USERMODE_SYMBOLIC_LINK);
    DWORD dwRes = 0;
    driver.SendIoCtl(DRV_UNHOOKER_UNHOOK, 0,0,0, &dwRes);

    std::cout<<"Success\n\n";
    return 0;
}

static void PrintUsage()
{
    std::cout<<"Usage: [stat] or [unhook]\n";
}

int wmain(int argc, wchar_t * argv[])
{
    try
    {
        if (argc != 2)
        {
            PrintUsage();
            return 0;
        }
        if (wcscmp(argv[1], L"stat") == 0)
        {
            return do_stat(argc, argv);
        }
        if (wcscmp(argv[1], L"unhook") == 0)
        {
            return do_unhook(argc, argv);
        }
        PrintUsage();
        return 0;
    }
    catch(std::exception & e)
    {
        std::cout<<e.what()<<"\n";
    }
    return 0;
}