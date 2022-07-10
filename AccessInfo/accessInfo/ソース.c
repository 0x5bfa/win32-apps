#include <windows.h>
#include <aclapi.h>
#include <stdio.h>
#include <locale.h>
#pragma warning (disable: 4996)

int main() {
    _wsetlocale(LC_ALL, L"");
    GetFileAcl();
}



HRESULT GetFileAcl() {
    PSECURITY_DESCRIPTOR pSD;    DWORD nLengthNeeded = 0;
    PACL pDacl;    ACL_SIZE_INFORMATION aclSize;
    ACCESS_ALLOWED_ACE* pAce;    TCHAR szAccountName[256];
    DWORD dwAccountNameSize;    TCHAR szDomainName[256];
    DWORD dwDomainNameSize;    SID_NAME_USE snu;
    BOOL bDaclDefault;    BOOL bDaclPresent;
    LPCTSTR lpFileName = L"C:\\Windows\\regedit.exe";

    GetFileSecurityW(lpFileName, DACL_SECURITY_INFORMATION, NULL, 0, &nLengthNeeded);

    pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, nLengthNeeded);
    if (pSD == NULL) { return 1; }

    GetFileSecurityW(lpFileName, DACL_SECURITY_INFORMATION, pSD, nLengthNeeded, &nLengthNeeded);

    GetSecurityDescriptorDacl(pSD, &bDaclPresent, &pDacl, &bDaclDefault);

    if (!bDaclPresent) {
        printf("No DACL");
    }
    else {
        GetAclInformation(pDacl, &aclSize, sizeof(aclSize), AclSizeInformation);

        wprintf(L"==============================================================================================================\n");
        wprintf(L"ACL of \"%s\"\n\n", lpFileName);
        wprintf(L"Type     Principal                                           Access Control                        Inheritance\n");
        wprintf(L"-------- --------------------------------------------------- ------------------------------------- -----------\n");

        for (DWORD i = 0; i < aclSize.AceCount; i++) {

            GetAce(pDacl, i, (LPVOID*)&pAce);

            dwAccountNameSize = sizeof(szAccountName) / sizeof(szAccountName[0]);
            dwDomainNameSize = sizeof(szDomainName) / sizeof(szDomainName[0]);

            LookupAccountSidW(NULL, &pAce->SidStart, szAccountName, &dwAccountNameSize, szDomainName, &dwDomainNameSize, &snu);

            //ACE タイプ
            switch (pAce->Header.AceType) {
            case ACCESS_ALLOWED_ACE_TYPE:
                printf("ALLOWED  ");
                break;
            case ACCESS_DENIED_ACE_TYPE:
                printf("DENIED   ");
                break;
            case SYSTEM_AUDIT_ACE_TYPE:
                printf("AUDIT    ");
                break;
            default:
                printf("Unknown");
                break;
            }


            //プリンシパル名
            TCHAR computerName[16];
            DWORD cchConputerName = 16;
            GetComputerNameW(computerName, &cchConputerName);
            if (!wcscmp(szDomainName, L"BUILTIN")
                || !wcscmp(szDomainName, computerName)) {
                TCHAR szFullName[512];
                wcscpy(szFullName, computerName);
                wcscat(szFullName, L"\\");
                wcscat(szFullName, szAccountName);
                if (wcslen(szFullName) <= 51) {
                    wprintf(L"%-*s ", 51 - GetConsoleStringWidth(szFullName), szFullName);
                }
                else {
                    wprintf(L"%-*s ", 51 - GetConsoleStringWidth(szAccountName), szAccountName);
                }

            }
            else {
                wprintf(L"%-*s ", 51 - GetConsoleStringWidth(szAccountName), szAccountName);

            }

            TCHAR accessCtrl[256] = L"";

            //フルコントロール
            wcscat(accessCtrl, L"");
            if ((pAce->Mask & FILE_ALL_ACCESS) == FILE_ALL_ACCESS)
                wcscpy(accessCtrl, L"Full control");
            //アクセスコントロールなし
            else if (pAce->Mask == 0) {
                wcscpy(accessCtrl, L"No control");
            }
            else {
                //変更
                if ((pAce->Mask & FILE_GENERIC_READ) == FILE_GENERIC_READ
                    && (pAce->Mask & FILE_GENERIC_WRITE) == FILE_GENERIC_WRITE
                    && (pAce->Mask & FILE_GENERIC_EXECUTE) == FILE_GENERIC_EXECUTE
                    && (pAce->Mask & DELETE) == DELETE) {
                    wcscat(accessCtrl, L"M ");
                }
                else {
                    //読み取りと実行
                    if ((pAce->Mask & FILE_GENERIC_READ) == FILE_GENERIC_READ
                        && (pAce->Mask & FILE_GENERIC_EXECUTE) == FILE_GENERIC_EXECUTE) {
                        wcscat(accessCtrl, L"RX ");
                    }
                    else {
                        //読み取り
                        if ((pAce->Mask & FILE_GENERIC_READ) == FILE_GENERIC_READ)
                            wcscat(accessCtrl, L"R ");
                        else {
                            //データの読み取り/フォルダの一覧
                            if ((pAce->Mask & FILE_READ_DATA) == FILE_READ_DATA)
                                wcscat(accessCtrl, L"RD ");
                            //属性の読み取り
                            if ((pAce->Mask & FILE_READ_ATTRIBUTES) == FILE_READ_ATTRIBUTES)
                                wcscat(accessCtrl, L"RA ");

                            //拡張属性の読み取り
                            if ((pAce->Mask & FILE_READ_EA) == FILE_READ_EA)
                                wcscat(accessCtrl, L"REA ");

                            if ((pAce->Mask & SYNCHRONIZE) == SYNCHRONIZE)
                                wcscat(accessCtrl, L"S ");
                        }
                        //フォルダスキャン/ファイルの実行
                        if ((pAce->Mask & FILE_EXECUTE) == FILE_EXECUTE)
                            wcscat(accessCtrl, L"X ");

                    }
                    //書き込み
                    if ((pAce->Mask & FILE_GENERIC_WRITE) == FILE_GENERIC_WRITE) {
                        wcscat(accessCtrl, L"GW ");
                    }
                    else {
                        //データの書き込み
                        if ((pAce->Mask & FILE_WRITE_DATA) == FILE_WRITE_DATA)
                            wcscat(accessCtrl, L"WD ");
                        //データの追加
                        if ((pAce->Mask & FILE_APPEND_DATA) == FILE_APPEND_DATA)
                            wcscat(accessCtrl, L"AD ");
                        //属性の書き込み
                        if ((pAce->Mask & FILE_WRITE_ATTRIBUTES) == FILE_WRITE_ATTRIBUTES)
                            wcscat(accessCtrl, L"WA ");
                        //属性の書き込み
                        if ((pAce->Mask & FILE_WRITE_EA) == FILE_WRITE_EA)
                            wcscat(accessCtrl, L"WEA ");
                    }
                    if ((pAce->Mask & DELETE) == DELETE)
                        wcscat(accessCtrl, L"D ");
                }
                if ((pAce->Mask & WRITE_DAC) == WRITE_DAC)
                    wcscat(accessCtrl, L"WDAC ");
                if ((pAce->Mask & WRITE_OWNER) == WRITE_OWNER)
                    wcscat(accessCtrl, L"WO ");
            }
            wprintf(L"%-37s ", accessCtrl);

            //継承
            switch (pAce->Header.AceFlags) {
            case OBJECT_INHERIT_ACE:
                wprintf(L"OI");
                break;
            case INHERIT_ONLY_ACE:
                wprintf(L"IO");
                break;
            case CONTAINER_INHERIT_ACE:
                wprintf(L"CI");
                break;
            default:
                wprintf(L"None");
            }

            printf("\n");
        }
    }

    LocalFree(pSD);

    return 0;

}

DWORD GetConsoleStringWidth(LPCWSTR lpString) {
    DWORD dwWidth = 0;
    for (int i = 0; lpString[i] != NULL; i++) {
        if (0x200000 <= lpString[i] && lpString[i] <= 0x2fa1f) {
            dwWidth++;
        }
        else if (0x3040 <= lpString[i] && lpString[i] <= 0x9fff) {
            dwWidth++;
        }
    }
    return dwWidth;
}