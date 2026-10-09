Note
======

This is my changeset in order to use amdgpu module with my card - 
Radeon 780M, coming with an AMD Ryzen 7 8845HS. It's applied on top
of NetBSD 11.0. It's a minimal changeset, much of the stuff is disabled - e.g. AGP etc.

My card PCI ID is 0x1002 0x1900, but I suspect that this changeset
works with other newer Radeons. If you want to give it a try you
have to add your pci id in `sys/external/bsd/drm2/dist/drm/amd/amdgpu/amdgpu_drv.c`.
Unlike Linux, wildcard probing is disabled.

You will also need the newer amdgpu firmware from [here](https://gitlab.com/kernel-firmware/linux-firmware/-/tree/main/amdgpu?ref_type=heads).
Just drop all the files there in `/libdata/firmware/amdgpu/`

You will also need to load the amdgpu module on boot, this is a `boot.cfg` entry that I use:
`menu=Boot with amdgpu:rndseed /var/db/entropy-file;load drmkms_sched;load drmkms_ttm;load amdgpu;gop 0;boot`

Use this on your own risk, I am not responsible for anything.
License: I don't care, free to copy and free to use it as you find useful.
Just don't mention my name, I don't want to be associated with anything weird.

Below, the original README.


NetBSD
======

NetBSD is a free, fast, secure, and highly portable Unix-like Open
Source operating system.  It is available for a [wide range of
platforms](https://wiki.NetBSD.org/ports/), from large-scale servers
and powerful desktop systems to handheld and embedded devices.

Building
--------

You can cross-build NetBSD from most UNIX-like operating systems.
To build for amd64 (x86_64), in the src directory:

    ./build.sh -U -u -j4 -m amd64 -O ~/obj release

Additional build information available in the [BUILDING](BUILDING) file.

Binaries
--------

- [Daily builds](https://nycdn.NetBSD.org/pub/NetBSD-daily/HEAD/latest/)
- [Releases](https://cdn.NetBSD.org/pub/NetBSD/)

Testing
-------

On a running NetBSD system:

    cd /usr/tests; atf-run | atf-report

Troubleshooting
---------------

- Send bugs and patches [via web form](https://www.NetBSD.org/cgi-bin/sendpr.cgi?gndb=netbsd).
- Subscribe to the [mailing lists](https://www.NetBSD.org/mailinglists/).
  The [netbsd-users](https://www.NetBSD.org/mailinglists/#netbsd-users) list is a good choice for many problems; watch [current-users](https://www.NetBSD.org/mailinglists/#current-users) if you follow the bleeding edge of NetBSD-current.
- Join the community IRC channel [#netbsd @ libera.chat](https://web.libera.chat/#netbsd).

Latest sources
--------------

To fetch the main CVS repository:

    cvs -d anoncvs@anoncvs.NetBSD.org:/cvsroot checkout -P src

To work in the Git mirror, which is updated every few hours from CVS:

    git clone https://github.com/NetBSD/src.git

Additional Links
----------------

- [The NetBSD Guide](https://www.NetBSD.org/docs/guide/en/)
- [NetBSD manual pages](https://man.NetBSD.org/)
- [NetBSD Cross-Reference](https://nxr.NetBSD.org/)
