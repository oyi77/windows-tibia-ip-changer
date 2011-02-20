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

/*int Change::readMemory(unsigned addr){
    HANDLE tibia = getTibiaProcess();
	int value;
	if (!ReadProcessMemory(tibia, reinterpret_cast<void*>(addr), &value, sizeof(int), NULL))
		return -1;

	return value;
}*/

std::string Change::getClientVersion()
{
    HANDLE tibia = getTibiaProcess();
	DWORD VALUE, oldProtect;
	VirtualProtectEx(tibia, (LPVOID)VERSION_ADDRESS, 4, PAGE_READWRITE, &oldProtect);
	ReadProcessMemory(tibia, (LPVOID)VERSION_ADDRESS, &VALUE, 4, NULL);
	VirtualProtectEx(tibia, (LPVOID)VERSION_ADDRESS, 4, oldProtect, NULL);
	CloseHandle(tibia);
	
	if(!VALUE) return 0;

	switch(std::abs(int(VALUE))){
        case 1947662723: return "760";
        case 800224000:  return "780";
        case 1793887061: return "792";
        case 1196488705: return "800";
        case 1056666274: return "810";
        case 1929356714: return "820";
        case 220:        return "821";
        case 1593835520: return "822";
        case 4559860:    return "830";
        case 286571:     return "840";
        case 265075:     return "841";
        case 1678442657: return "842";
        case 2293592:    return "850";
        case 737977104:  return "852";
        case 3152:       return "854";
        case 907476993:  return "855";
		case 620720247:  return "857";
		case 74056298:   return "860";
		case 103814912:  return "861";
        case 90375192:   return "862";
		case 41843:      return "870";
		case 1170669562: return "871";
		default:         return "";
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
