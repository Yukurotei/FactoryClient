#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <future>
#include <string>
#include <cstring>

#include "../../modules/module.h"
#include "../../settings/settings.h"

#pragma comment(lib, "ws2_32")

#define DEFAULT_PORT "6969"

bool stopFromDetaching = true;

void InitializeWinsock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return;
    }
}

void CleanupWinsock() {
    WSACleanup();
}

SOCKET CreateListenSocket() {
    struct addrinfo* result = nullptr, *ptr = nullptr, hints;
    int iResult;

    // Set up hints for addrinfo structure
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed: " << iResult << std::endl;
        return NULL;
    }

    // Create a socket for the server to listen on
    SOCKET listenSocket = INVALID_SOCKET;
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        return NULL;
    }

    // Bind the socket to the address and port
    iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        closesocket(listenSocket);
        return NULL;
    }

    freeaddrinfo(result);

    // Start listening for incoming connections
    iResult = listen(listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        return NULL;
    }

    return listenSocket;
}

SOCKET Accept(SOCKET listenSocket) {
    SOCKET clientSocket = INVALID_SOCKET;

    char buffer[1024] = {0};

    // Wait for a client connection
    std::cout << "(Socket)Waiting for a client connection..." << std::endl;
    clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "(Socket)Accept failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        return NULL;
    }

    std::cout << "Client connected!" << std::endl;

    // Send data to client
    /*
    const char* sendbuf = "fServer2Client-Accepted";
    int iResult = send(clientSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "(Socket)Send failed: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        closesocket(listenSocket);
        return NULL;
    }
    */
    //Perform handshake
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        std::cout << "Received: " << std::string(buffer, bytesReceived) << std::endl;

        if (std::string(buffer, bytesReceived) == "fClient2Server-RequestGUIConnect")
        {
            std::string response = "fServer2Client-AcceptedGUIConnection";
            send(clientSocket, response.c_str(), response.length(), 0);
            std::cout << "Accepted GUI connection" << std::endl;
        }
        
    } else {
        std::cerr << "Failed to receive data from client." << std::endl;
    }

    std::cout << "Performed handshake with GUI." << std::endl;

    return clientSocket;
}

void HandleClientConnection(SOCKET clientSocket, bool loop)
{
    char buffer[512];
    int bytesReceived;

    bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';  // Null-terminate the received data
        std::cout << "Received from client: " << buffer << std::endl;

        std::string response;
        //hardcoded socket method cuz I'm dumb
        if (std::string(buffer) == "getAllModuleNames") {
            for (int i = 0; i < modules.size(); i++)
            {
                response = response + modules[i]->name + ",";
            }
            response = response.replace(response.size() - 1, response.size(), ""); //Get rid of last letter which is comma
        } else if (std::string(buffer).rfind("toggleModule", 0) == 0) {
            std::string finallizedString = std::string(buffer).replace(0, 12, "");
            Module* module = GetModule(finallizedString);
            if (module == nullptr)
            {
                response = "unknownModule" + finallizedString;
            } else {
                module->toggle();
                response = "toggledModule" + module->name;
            }
        } else if (std::string(buffer).rfind("getAllSettingOfModule", 0) == 0) {
            std::string finallizedString = std::string(buffer).replace(0, 21, "");
            Module* module = GetModule(finallizedString);
            if (module == nullptr)
            {
                response = "unknownModule" + finallizedString;
            } else {
                std::map<std::string, Setting*> sMap = getSettingsMap();
                for (auto const& [key, val] : sMap)
                {
                    if (key.rfind(module->name, 0) == 0)
                    {
                        std::string fSettingName = val->name;
                        response += val->name + ",";
                    }
                }
                response = response.replace(response.size() - 1, response.size(), ""); //Get rid of last letter which is comma
            }
        } else if (std::string(buffer).rfind("setValueInSquareBracketsOfSettingOfModuleInBrackets", 0) == 0) {
            //[TYPE-value](module)setting
            //Find text inside bracket
            std::string input = std::string(buffer);
            std::string module = "";
            bool startEntering = false;
            int endI = 0;
            for (int i = 0; i < input.size(); i++)
            {
                if (input[i - 1] == '(' && !startEntering) //Avoid first bracket
                {
                    startEntering = true;
                }
                if (input[i] == ')' && startEntering)
                {
                    startEntering = false;
                    endI = i;
                    break;
                }
                if (startEntering)
                {
                    module = module + input[i];
                }
            }
            if (GetModule(module) == nullptr)
            {
                response = "NoModuleFound: " + module;
            } else {
                Module* mod = GetModule(module);
                bool startEnter = false;
                //Find setting
                std::string setting = "";
                for (int i = 0; i < input.size(); i++)
                {
                    if (i > endI)
                    {
                        startEnter = true;
                    }
                    if (startEnter)
                    {
                        setting = setting + input[i]; //The setting name should be at the end
                    }
                }
                if (GetSettingInModule(mod, setting) == nullptr)
                {
                    response = "NoSettingFound: " + setting;
                } else {
                    Setting* moduleSetting = GetSettingInModule(mod, setting);
                    std::string rawValue = "";
                    ConvertableValue value;
                    bool startEntering = false;
                    for (int i = 0; i < input.size(); i++)
                    {
                        if (input[i - 1] == '[' && !startEntering) //Avoid first bracket
                        {
                            startEntering = true;
                        }
                        if (input[i] == ']' && startEntering)
                        {
                            startEntering = false;
                            break;
                        }
                        if (startEntering)
                        {
                            rawValue = rawValue + input[i];
                        }
                    }
                    int rawValueType = rawValue[0];
                    if (rawValueType == 'F') //Float
                    {
                        value = ConvertableValue();
                        value.insertStringValue(rawValue.replace(0, 2, ""), false);
                        ((FloatSetting*)moduleSetting)->setCurrentConvertable(value);
                    } else if (rawValueType == 'I') //Int
                    {
                        value = ConvertableValue();
                        value.insertStringValue(rawValue.replace(0, 2, ""), false);
                        ((IntSetting*)moduleSetting)->setCurrentConvertable(value);
                    } else if (rawValueType == 'B') //Bool
                    {
                        value = ConvertableValue();
                        value.insertStringValue(rawValue.replace(0, 2, ""), true);
                        ((BoolSetting*)moduleSetting)->setCurrentConvertable(value);
                    } else if (rawValueType == 'M') //Mode
                    {
                        value = ConvertableValue();
                        value.insertStringValue(rawValue.replace(0, 2, ""), false);
                        ((ModeSetting*)moduleSetting)->setCurrentConvertable(value);
                    } else {
                        response = "Unknown value type: " + rawValueType;
                        std::cout << "Sending response: " << response << std::endl;
                        send(clientSocket, response.c_str(), response.length(), 0);
                        if (loop)
                        {
                            HandleClientConnection(clientSocket, loop);
                        }
                    }
                    response = "Set " + moduleSetting->name + " of " + mod->name + " to " + value.getStringValue();
                }
            }
        } else if (std::string(buffer).rfind("getValueOfSettingInModuleInBrackets", 0) == 0) {
            //(module)setting
            std::string providedString = std::string(buffer).replace(0, 35, "");
            bool startEntering = false;
            int endI = 0;
            std::string moduleName = "";
            for (int i = 0; i < providedString.size(); i++) {
                if (providedString[i - 1] == '(' && !startEntering) //Avoid first bracket
                {
                    startEntering = true;
                }
                if (providedString[i] == ')' && startEntering)
                {
                    startEntering = false;
                    endI = i;
                    break;
                }
                if (startEntering)
                {
                    moduleName = moduleName + providedString[i];
                }
            }
            if (GetModule(moduleName) == nullptr)
            {
                response = "NoModuleFound: " + moduleName;
            } else {
                bool startEnter = false;
                //Find setting
                std::string setting = "";
                for (int i = 0; i < providedString.size(); i++)
                {
                    if (i > endI)
                    {
                        startEnter = true;
                    }
                    if (startEnter)
                    {
                        setting = setting + providedString[i]; //The setting name should be at the end
                    }
                }
                if (GetSettingInModule(GetModule(moduleName), setting) == nullptr)
                {
                    response = "NoSettingFound: " + setting;
                } else {
                    Setting* moduleSetting = GetSettingInModule(GetModule(moduleName), setting);
                    std::string valueType = "";
                    std::string valueInString = "";
                    if (moduleSetting->value.originalValue == "str")
                    {
                        valueType = "[S]";
                    } else if (moduleSetting->value.originalValue == "int")
                    {
                        valueType = "[I]";
                    } else if (moduleSetting->value.originalValue == "flo")
                    {
                        valueType = "[F]";
                    } else if (moduleSetting->value.originalValue == "boo")
                    {
                        valueType = "[B]";
                    }
                    valueInString = moduleSetting->value.getStringValue();
                    response = valueType + valueInString;
                }
            }
        } else if (std::string(buffer).rfind("strMsg", 0) == 0) {
            std::string finallizedString = std::string(buffer).replace(0, 6, "");
            std::cout << "Recieved string from client: " << finallizedString << std::endl;
            if (loop)
            {
                HandleClientConnection(clientSocket, loop);
            }
        } else if (std::string(buffer).rfind("letServerReturn", 0) == 0) {
            std::string finallizedString = std::string(buffer).replace(0, 15, "");
            response = finallizedString;
        } else if (std::string(buffer) == "cooperativeEnd") {
            closesocket(clientSocket);
            stopFromDetaching = false;
            return;
        } else {
            response = "Unknown event";
        }
        std::cout << "Sending response: " << response << std::endl;

        // Send the response back to the client
        send(clientSocket, response.c_str(), response.length(), 0);
        if (loop)
        {
            HandleClientConnection(clientSocket, loop);
        }
    }
    else if (bytesReceived == 0) {
        std::cout << "Client disconnected." << std::endl;
        if (loop)
        {
            HandleClientConnection(clientSocket, loop);
        }
    }
    else {
        std::cerr << "recv() failed or connection closed unexpectedly." << std::endl;
        if (loop)
        {
            HandleClientConnection(clientSocket, loop);
        }
    }
}

auto HandleClientConnectionAsyncly(SOCKET clientSocket)
{
    std::future f = std::async(&HandleClientConnection, clientSocket, true);
    return f;
}

void SendDataToClient(std::string sendMsg, SOCKET listenSocket, SOCKET clientSocket)
{
    const char* sendBuf = sendMsg.c_str();
    int iResult = send(clientSocket, sendBuf, (int)strlen(sendBuf), 0);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "(Socket)Send failed: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        closesocket(listenSocket);
        return;
    }
}

void CloseSocket(SOCKET socket)
{
    closesocket(socket);
}
/*
int main() {
    InitializeWinsock();
    
    SOCKET listenSocket = CreateListenSocket();
    AcceptAndCommunicate(listenSocket);

    return 0;
}
*/
