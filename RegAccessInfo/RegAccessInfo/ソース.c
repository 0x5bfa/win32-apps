#include <windows.h>
#include <aclapi.h>
#include <stdio.h>
#include <locale.h>
#include <winreg.h>
#pragma warning (disable: 4996)

int wmain(int argc, WCHAR* argv[]) {
    setlocale(LC_ALL, "");

    //if (argc == 1) {
    //    wprintf(L"ファイル名を指定してください\n");
    //    exit(1);
    //} else
    for (int i = 0; argv[i] != NULL; i++) {
        if (wcscmp(argv[i], L"")) {

        }
    }


    GetRegAccessInfo(HKEY_LOCAL_MACHINE, L"SOFTWARE");
}

int GetRegAccessInfo()
{
    PSECURITY_DESCRIPTOR pSD;
    DWORD nLengthNeeded = 0;
    PACL pDacl;
    ACL_SIZE_INFORMATION aclSize;
    ACCESS_ALLOWED_ACE* pAce;
    WCHAR szAccountName[256];
    DWORD dwAccountNameSize;
    WCHAR szDomainName[256];
    DWORD dwDomainNameSize;
    SID_NAME_USE snu;
    BOOL bDaclDefault;
    BOOL bDaclPresent;
    HKEY hKey = HKEY_LOCAL_MACHINE;

    RegGetKeySecurity(hKey, DACL_SECURITY_INFORMATION, NULL, &nLengthNeeded);

    pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, nLengthNeeded);
    if (pSD == NULL) {
        return 1;
    }

    RegGetKeySecurity(hKey, DACL_SECURITY_INFORMATION, pSD, &nLengthNeeded);

    GetSecurityDescriptorDacl(pSD, &bDaclPresent, &pDacl, &bDaclDefault);

    if (!bDaclPresent) {
        wprintf(L"No DACL");
    }
    else {
        GetAclInformation(pDacl, &aclSize, sizeof(aclSize), AclSizeInformation);

        for (DWORD i = 0; i < aclSize.AceCount; i++) {

            GetAce(pDacl, i, (LPVOID*)&pAce);


            dwAccountNameSize = sizeof(szAccountName) / sizeof(szAccountName[0]);
            dwDomainNameSize = sizeof(szDomainName) / sizeof(szDomainName[0]);

            LookupAccountSidW(NULL,
                &pAce->SidStart,
                szAccountName,
                &dwAccountNameSize,
                szDomainName,
                &dwDomainNameSize,
                &snu);

            wprintf(L"==========================================================\n");
            wprintf(L"AccountName:%s, DomainName:%s\n", szAccountName, szDomainName);

            wprintf(L"\n===AccessMask===\n");

            //固有のアクセス権
            wprintf(L"\n#Specific Access Rights\n");
            if ((pAce->Mask & KEY_QUERY_VALUE) == KEY_QUERY_VALUE)
                wprintf(L"KEY_QUERY_VALUE\n");
            if ((pAce->Mask & KEY_SET_VALUE) == KEY_SET_VALUE)
                wprintf(L"KEY_SET_VALUE\n");
            if ((pAce->Mask & KEY_CREATE_SUB_KEY) == KEY_CREATE_SUB_KEY)
                wprintf(L"KEY_CREATE_SUB_KEY\n");
            if ((pAce->Mask & KEY_ENUMERATE_SUB_KEYS) == KEY_ENUMERATE_SUB_KEYS)
                wprintf(L"KEY_ENUMERATE_SUB_KEYS\n");
            if ((pAce->Mask & KEY_NOTIFY) == KEY_NOTIFY)
                wprintf(L"KEY_NOTIFY\n");
            if ((pAce->Mask & KEY_CREATE_LINK) == KEY_CREATE_LINK)
                wprintf(L"KEY_CREATE_LINK\n");
            if ((pAce->Mask & KEY_READ) == KEY_READ)
                wprintf(L"KEY_READ\n");
            if ((pAce->Mask & KEY_WRITE) == KEY_WRITE)
                wprintf(L"KEY_WRITE\n");
            if ((pAce->Mask & KEY_EXECUTE) == KEY_EXECUTE)
                wprintf(L"KEY_EXECUTE\n");
            if ((pAce->Mask & KEY_ALL_ACCESS) == KEY_ALL_ACCESS)
                wprintf(L"KEY_ALL_ACCESS\n");

            //標準のアクセス権
            wprintf(L"\n#Standard Access Rights\n");
            if ((pAce->Mask & DELETE) == DELETE)
                wprintf(L"DELETE\n");
            if ((pAce->Mask & READ_CONTROL) == READ_CONTROL)
                wprintf(L"READ_CONTROL\n");
            if ((pAce->Mask & WRITE_DAC) == WRITE_DAC)
                wprintf(L"WRITE_DAC\n");
            if ((pAce->Mask & WRITE_OWNER) == WRITE_OWNER)
                wprintf(L"WRITE_OWNER\n");
            if ((pAce->Mask & SYNCHRONIZE) == SYNCHRONIZE)
                wprintf(L"SYNCHRONIZE\n");
            if ((pAce->Mask & STANDARD_RIGHTS_REQUIRED) == STANDARD_RIGHTS_REQUIRED)
                wprintf(L"STANDARD_RIGHTS_REQUIRED\n");
            if ((pAce->Mask & STANDARD_RIGHTS_READ) == STANDARD_RIGHTS_READ)
                wprintf(L"STANDARD_RIGHTS_READ\n");
            if ((pAce->Mask & STANDARD_RIGHTS_WRITE) == STANDARD_RIGHTS_WRITE)
                wprintf(L"STANDARD_RIGHTS_WRITE\n");
            if ((pAce->Mask & STANDARD_RIGHTS_EXECUTE) == STANDARD_RIGHTS_EXECUTE)
                wprintf(L"STANDARD_RIGHTS_EXECUTE\n");
            if ((pAce->Mask & STANDARD_RIGHTS_ALL) == STANDARD_RIGHTS_ALL)
                wprintf(L"STANDARD_RIGHTS_ALL\n");
            if ((pAce->Mask & SPECIFIC_RIGHTS_ALL) == SPECIFIC_RIGHTS_ALL)
                wprintf(L"SPECIFIC_RIGHTS_ALL\n");

            wprintf(L"\n===AceType===\n");
            switch (pAce->Header.AceType) {
            case ACCESS_ALLOWED_ACE_TYPE:
                wprintf(L"ACCESS_ALLOWED_ACE_TYPE\n");
                break;

            case ACCESS_DENIED_ACE_TYPE:
                wprintf(L"ACCESS_DENIED_ACE_TYPE\n");
                break;

            case SYSTEM_AUDIT_ACE_TYPE:
                wprintf(L"SYSTEM_AUDIT_ACE_TYPE\n");
                break;

            default:
                wprintf(L"Unknown ACE type\n");
                break;
            }
            wprintf(L"\n\n");
        }
    }

    LocalFree(pSD);

    return 0;
}

DWORD GetErrorMessage(DWORD errorCode) {
    LPVOID lpMsgBuf;
    DWORD dwRes = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS
        , NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
    wprintf(L"%s", (LPCTSTR)lpMsgBuf);
    return dwRes;
}