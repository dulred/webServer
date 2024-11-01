#include <windows.h>
#include <wincred.h>
#include <iostream>
#include <string>
#include <vector>

// windows 系统下凭据管理器的读取和写入
class CredentialManager {
public:
    static bool WriteCredential(const std::wstring& target, const std::wstring& username, const std::wstring& password) {
        CREDENTIALW cred = {};
        cred.Type = CRED_TYPE_GENERIC;
        cred.TargetName = const_cast<LPWSTR>(target.c_str());  // 修改这里
        cred.UserName = const_cast<LPWSTR>(username.c_str());

        // 将密码转换为字节数组
        std::vector<wchar_t> passwordBlob(password.begin(), password.end());
        passwordBlob.push_back(L'\0');  // 添加结束符

        cred.CredentialBlob = reinterpret_cast<LPBYTE>(passwordBlob.data());
        cred.CredentialBlobSize = static_cast<DWORD>(passwordBlob.size() * sizeof(wchar_t));
        cred.Persist = CRED_PERSIST_SESSION; // 存储凭据到会话中

        if (CredWriteW(&cred, 0)) {
            std::wcout << L"Credential saved successfully." << std::endl;
            return true;
        } else {
            std::wcerr << L"Error saving credential: " << GetLastError() << std::endl;
            return false;
        }
    }

    static bool ReadCredential(const std::wstring& target) {
        PCREDENTIALW cred = nullptr;

        if (CredReadW(target.c_str(), CRED_TYPE_GENERIC, 0, &cred)) {
            std::wcout << L"Username: " << cred->UserName << std::endl;
            std::wcout << L"Password: " << std::wstring(reinterpret_cast<wchar_t*>(cred->CredentialBlob), cred->CredentialBlobSize / sizeof(wchar_t)) << std::endl;
            CredFree(cred);
            return true;
        } else {
            std::wcerr << L"Error reading credential: " << GetLastError() << std::endl;
            return false;
        }
    }
};

int main() {
    const std::wstring target = L"MyApp";
    const std::wstring username = L"myUsername";
    const std::wstring password = L"myPassword";

    // 写入凭据
    CredentialManager::WriteCredential(target, username, password);

    // 读取凭据
    CredentialManager::ReadCredential(target);

    return 0;
}
