#include <windows.h>
#include <windns.h>
#include <iostream>
#include <string>

#pragma comment(lib, "dnsapi.lib")
#pragma comment(lib, "Ws2_32.lib")

int main()
{
    std::string domain = "";

    std::cout << "Insira o dominio: ";
    std::getline(std::cin, domain);

    PDNS_RECORD pDnsRecord = nullptr;
    DNS_STATUS status;

    status = DnsQuery_A(
        domain.c_str(),
        DNS_TYPE_A,
        DNS_QUERY_STANDARD,
        nullptr,
        &pDnsRecord,
        nullptr
    );

    if (status != 0)
    {
        std::cerr << "Erro ao consultar DNS para " << domain
            << ". Código de status: " << status << std::endl;
        return 1;
    }

    PDNS_RECORD pRecord = pDnsRecord;
    bool foundAny = false;

    while (pRecord != nullptr)
    {
        if (pRecord->wType == DNS_TYPE_A)
        {
            IN_ADDR ipAddr;
            ipAddr.S_un.S_addr = pRecord->Data.A.IpAddress;

            char* ipStr = inet_ntoa(ipAddr);
            if (ipStr != nullptr)
            {
                std::cout << "IP encontrado: " << ipStr << std::endl;
                foundAny = true;
            }
        }
        pRecord = pRecord->pNext;
    }

    if (!foundAny)
    {
        std::cout << "Nenhum registro do tipo A foi encontrado para "
            << domain << std::endl;
    }

    if (pDnsRecord != nullptr)
    {
        DnsRecordListFree(pDnsRecord, DnsFreeRecordList);
    }

    return 0;
}
