#include <iostream>
#include <Windows.h>
#include <string>
#include <iphlpapi.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

using namespace std;

class WinApi {
private:
	HINSTANCE pHistance;

	WSAData wsData;
	WORD sockVersion;
	SOCKADDR_IN addrin;
	SOCKET sListen;
	SOCKET connection;
public:
	WinApi(HINSTANCE pHis = NULL) {
		pHistance = pHis;

	}
	LRESULT ShowMessageBox(LPCSTR header, LPCSTR text) {
		return MessageBox(NULL, header, text, MB_OK);
	}

	LRESULT ArpScaning(const char* fixedIpPart) {
		string startIp = fixedIpPart;
		string currIp;

		DWORD dwRetVal;
		char Mac[2];

		for (int i = 0; i < 255; ++i) {

			ULONG physAddrLen = 6;

			currIp = startIp + to_string(i);
			dwRetVal = SendARP(inet_addr(currIp.c_str()), NULL, &Mac, &physAddrLen);
			cout << currIp << endl;
			if (dwRetVal == NO_ERROR) {
				cout << "Finded   ";
				BYTE *bPhysAddr = (unsigned char*) Mac;
				for (i = 0; i < physAddrLen; i++) {
					if (i == (physAddrLen - 1))
						printf("%.2X\n", (int)bPhysAddr[i]);
					else
						printf("%.2X-", (int)bPhysAddr[i]);
				}
			}
			else {
				switch (dwRetVal) {
				case ERROR_GEN_FAILURE:
					cout << "ERROR_GEN_FAILURE" << endl;
					break;
				case ERROR_INVALID_PARAMETER:
					cout << "ERROR_INVALID_PARAMETER" << endl;
					break;
				case ERROR_INVALID_USER_BUFFER:
					cout << "ERROR_INVALID_USER_BUFFER" << endl;
					break;
				case ERROR_BAD_NET_NAME:
					cout << "ERROR_GEN_FAILURE" << endl;
					break;
				case ERROR_BUFFER_OVERFLOW:
					cout << "ERROR_BUFFER_OVERFLOW" << endl;;
					break;
				case ERROR_NOT_FOUND:
					cout << "ERROR_NOT_FOUND" << endl;
					break;
				default:
					cout << endl;
					break;
				}
			}
		}
		return 0;
	}

	LRESULT StartServer(const char* ipAddr, int portAddr) {
		sockVersion = MAKEWORD(2, 1);
		if (WSAStartup(sockVersion, &wsData) != 0) {
			cout << "Fail";
			exit(1);
		}

		addrin.sin_addr.s_addr = inet_addr(ipAddr);
		addrin.sin_port = htons(portAddr);
		addrin.sin_family = AF_INET;

		sListen = socket(AF_INET, SOCK_STREAM, NULL);
		bind(sListen, (SOCKADDR*)&addrin, sizeof(addrin));
		listen(sListen, 1);

		int arrSize = sizeof(addrin);
		connection = accept(sListen, (SOCKADDR*)& addrin, &arrSize);
		if (connection == 0) {
			cout << "Error of connection";
		} else {
			cout << "Connected client " << inet_ntoa(addrin.sin_addr) << endl;
			char bufHost[128];
			hostent *infoHost;
			if (gethostname(bufHost, sizeof(bufHost)) == 0) {
				if ((infoHost = gethostbyname(bufHost)) != NULL) {
					cout << bufHost << "  |  " << inet_ntoa(*(reinterpret_cast<in_addr*> (infoHost->h_addr_list))) << endl;
				}
			}
		}

		char bytesFromClient[1];
		int numBytesFromClient;

		do {
			numBytesFromClient = recv(connection, bytesFromClient, 1, 0);
			cout << bytesFromClient[0];
			if (bytesFromClient[0] == '`') {
				cout << endl;
				break;
			}
		} while (numBytesFromClient > 0);

		shutdown(connection, 2);
		closesocket(connection);
		WSACleanup();

		system("PAUSE");
		exit(0);

		return 0;
	}

};

int main() {
	WinApi api = WinApi();
	//api.StartServer("192.168.0.124", 44555);
	api.ArpScaning("192.168.0.");
	system("PAUSE");
}