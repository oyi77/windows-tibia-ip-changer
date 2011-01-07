#include <sstream>
#include <cstdlib>
#include "change.h"
#include "clients.h"

HANDLE Change::getTibiaProcess(){
    DWORD processId;
    HWND hWnd = FindWindow("TibiaClient", 0);
    GetWindowThreadProcessId(hWnd, &processId);
    return OpenProcess(PROCESS_ALL_ACCESS, false, processId);
}

bool Change::writeMemory(unsigned addr, const char *data){
    int i;
	const int len = strlen(data);
	for(i = 0; i < len; i++){
		if(!writeMemoryByte(addr + i, (int)data[i]))
			return false;
	}
	writeMemoryByte(addr + i, 0x00);
	return true;
}

bool Change::writeMemoryByte(unsigned addr, int data){
    HANDLE tibia = getTibiaProcess();
    if(!WriteProcessMemory(tibia, (LPVOID)(addr), &data, sizeof(data), 0))
        return false;

    return true;
}

bool Change::setRSA(unsigned addr){
    HANDLE tibia = getTibiaProcess();
    if(addr != 0){
        DWORD protection;
    	if (!VirtualProtectEx(tibia, reinterpret_cast<void*>(addr), strlen(RSA_KEY), PAGE_EXECUTE_READWRITE, &protection)) {
    		VirtualProtectEx(tibia, reinterpret_cast<void*>(addr), strlen(RSA_KEY), protection, &protection);
    		CloseHandle(tibia);
            return false;
    	}
    	if (WriteProcessMemory(tibia, reinterpret_cast<void*>(addr), RSA_KEY, strlen(RSA_KEY), NULL)) {
    		VirtualProtectEx(tibia, reinterpret_cast<void*>(addr), strlen(RSA_KEY), protection, &protection);
    		return true;
    	}
    }
    return true;
}

int Change::readMemory(unsigned addr){
    HANDLE tibia = getTibiaProcess();
	int value;
	if (!ReadProcessMemory(tibia, reinterpret_cast<void*>(addr), &value, sizeof(int), NULL))
		return -1;

	return value;
}

std::string Change::getClientVersion(){
	switch(readMemory(readMemory(0x40003C) + 0x400050)){
		case 2101248: return "760";
		case 3510272: return "780";
		case 3575808: return "792";
		case 3678208: return "800";
		case 3698688: return "810";
		case 3756032: return "820";
		case 3768320: return "821";
		case 3780608: return "822";
		case 3796992: return "830";
		case 3817472: return "840";
		case 3821568: return "841";
		case 3842048: return "842";
		case 3846144: return "850";
		case 3850240: return "852";
		case 4128768: return "854";
		case 4157440: return "855";
		case 4169728: return "857";
		case 4124672: return "861";
		case 4313088: return "862";
		case 4349952: return "870";
		default: return "";
	}
}

bool Change::changeIP(std::string IP, int port, std::string v){
	int LoginServersStart, RSA, ServerCount = 0;
	if(v == "Auto")
	   v = getClientVersion();

	for(int i = 0; i < int(sizeof(adresses)/sizeof(adresses[1])); i++){
		if(adresses[i][0] == v){
			std::stringstream ilss;
            ilss<<adresses[i][1];
           	ilss >> std::setbase(0) >> LoginServersStart;
			std::stringstream irsa;
            irsa<<adresses[i][2];
			irsa >> std::setbase(0) >> RSA;
			ServerCount = atoi(adresses[i][3].c_str());	
			break;
		}
	}
	
	if(!setRSA(RSA))
	   return false;

    HANDLE tibia = getTibiaProcess();
	for(int i = 0; i < ServerCount; i++) {
		if(!writeMemory(LoginServersStart, IP.c_str())){
            CloseHandle(tibia);
			return false;
        }
		if(!writeMemoryByte(LoginServersStart + PORT_DISTANCE, port)){
            CloseHandle(tibia);
			return false;
        }
		LoginServersStart += IP_DISTANCE;
	}
	std::stringstream info;
    info<<"Tibia on host: "<<IP<<":"<<port;
    HWND hwnd = FindWindow("TibiaClient", 0);
	SetWindowTextA(hwnd, info.str().c_str());
	SetForegroundWindow(hwnd);
	CloseHandle(tibia);
    return true;
}
