#include "fileInfo.h"
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

typedef struct _FILE_INFO {
    WCHAR wszFileName[MAX_PATH];
    WCHAR wszFullPathName[MAX_PATH];
    WCHAR wszFileExtension[MAX_PATH];
    WCHAR wszOpenWithAppName[MAX_PATH];
    WCHAR wszFileTypeName[MAX_PATH];
    DWORD dwBinaryType;
}FILE_INFO, *LPFILE_INFO;

typedef struct FILE_ASSOC {
    TCHAR wszCommand[MAX_PATH];         //�t�@�C�����J���A�v���̎��s�R�}���h ()
    TCHAR wszExecutable[MAX_PATH];      //���s�t�@�C���p�X ()
    TCHAR wszDocName[MAX_PATH];         //�t�@�C���̎�� (Microsoft Word ����)
    TCHAR wszAppName[MAX_PATH];         //�A�v���P�[�V�����̖��O (Word)
    TCHAR wszNoOpen[MAX_PATH];
    TCHAR wszShellNewValue[MAX_PATH];
    TCHAR wszDdeCommand[MAX_PATH];
    TCHAR wszDdeIfExec[MAX_PATH];
    TCHAR wszDdeApp[MAX_PATH];
    TCHAR wszDdeTopic[MAX_PATH];
    TCHAR wszInfoTip[MAX_PATH];
    TCHAR wszQuickTip[MAX_PATH];
    TCHAR wszTileInfo[MAX_PATH];
    TCHAR wszContentType[MAX_PATH];
    TCHAR wszDefaultIcon[MAX_PATH];     //�t�@�C���A�C�R���̃p�X
    TCHAR wszShellExtension[MAX_PATH];
    TCHAR wszDropTarget[MAX_PATH];
    TCHAR wszDelegateExecute[MAX_PATH];
    TCHAR wszSupportedUriProtocols[MAX_PATH];
    TCHAR wszProgId[MAX_PATH];          //Programment ID
    TCHAR wszAppId[MAX_PATH];           //App ID
    TCHAR wszAppPublisher[MAX_PATH];    //���s�A�v���P�[�V�����̔��s��
    TCHAR wszAppIconReference[MAX_PATH];
    TCHAR wszMax[MAX_PATH];
}FILE_ASSOC, * LPFILE_ASSOC;




int main(void) {
    setlocale(LC_ALL, "");
    WCHAR wszFileName[MAX_PATH] = L"C:\\Users\\T31068068\\Desktop\\word doc.docx";
    wprintf(L"===================================================================================================================\n");
    wprintf(L"file information of \"%s\"\n\n", wszFileName);

    FILE_INFO fi;
    GetFileInfo(wszFileName, &fi);
    return 0;
}


DWORD GetFileInfo(LPWSTR lpFileName, LPFILE_INFO pfi) {
    //���S�p�X�E�t�@�C�����̎擾
    WCHAR wszFullPathName[MAX_PATH];
    WCHAR* wszFileName;
    GetFullPathNameW(lpFileName, MAX_PATH, wszFullPathName, &wszFileName);
    wcscpy(pfi->wszFullPathName, wszFullPathName);
    wcscpy(pfi->wszFileName, wszFileName);

    wcscpy(pfi->wszFileExtension, PathFindExtensionW(pfi->wszFullPathName));

    //���s�t�@�C�����ǂ����E���s�t�@�C���̎��
    GetBinaryTypeW(wszFileName, &pfi->dwBinaryType);

    //�g���q���
    LPFILE_ASSOC fa = (LPFILE_ASSOC)malloc(sizeof(FILE_ASSOC));
    GetAssoc(pfi->wszFileExtension, fa);
    wprintf(L"%s\n\n", fa->wszDocName);


    //�t�@�C���̈�ʏ��擾
}


BOOL GetAssoc(LPCTSTR pszExtension, LPFILE_ASSOC pfa) {
    TCHAR wszFile[MAX_PATH * 2]; DWORD dwFileSize = MAX_PATH * 2; HRESULT hr;

    for (int i = 1; i <= 23; i++) {
        hr = AssocQueryStringW(ASSOCF_NOTRUNCATE, i, pszExtension, NULL, wszFile, &dwFileSize);
        switch (i) {
        case 1:
            wcscpy(pfa->wszCommand, wszFile);
        case 2:
            wcscpy(pfa->wszExecutable, wszFile);
        case 3:
            wcscpy(pfa->wszDocName, wszFile);
        case 4:
            wcscpy(pfa->wszAppName, wszFile);
        case 5:
            wcscpy(pfa->wszNoOpen, wszFile);
        case 6:
            wcscpy(pfa->wszShellNewValue, wszFile);
        case 7:
            wcscpy(pfa->wszDdeCommand, wszFile);
        case 8:
            wcscpy(pfa->wszDdeIfExec, wszFile);
        case 9:
            wcscpy(pfa->wszDdeApp, wszFile);
        case 10:
            wcscpy(pfa->wszDdeTopic, wszFile);
        case 11:
            wcscpy(pfa->wszInfoTip, wszFile);
        case 12:
            wcscpy(pfa->wszQuickTip, wszFile);
        case 13:
            wcscpy(pfa->wszTileInfo, wszFile);
        case 14:
            wcscpy(pfa->wszContentType, wszFile);
        case 15:
            wcscpy(pfa->wszDefaultIcon, wszFile);
        case 16:
            wcscpy(pfa->wszShellExtension, wszFile);
        case 17:
            wcscpy(pfa->wszDropTarget, wszFile);
        case 18:
            wcscpy(pfa->wszDelegateExecute, wszFile);
        case 19:
            wcscpy(pfa->wszSupportedUriProtocols, wszFile);
        case 20:
            wcscpy(pfa->wszProgId, wszFile);
        case 21:
            wcscpy(pfa->wszAppId, wszFile);
        case 22:
            wcscpy(pfa->wszAppPublisher, wszFile);
        case 23:
            wcscpy(pfa->wszAppIconReference, wszFile);
        }
    }
    if (FAILED(hr) || dwFileSize == 0) return TRUE;
    else return FALSE;
}
