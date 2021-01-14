# Simple Smart Contract on Unikraft

This is a simple smart contract-like program running on Unikraft.
It starts a server on port `12345` and waits for a integer (`4` bytes) to be received.
It then replies with the incredemented value of the integer.

Currently the C and C++ builds are available working.

For the C build you need to clone:
* [Unikraft](https://github.com/unikraft/unikraft)
* [lwip](https://github.com/unikraft/lib-lwip)

For the C++ build you need to clone:
* [Unikraft](https://github.com/unikraft/unikraft)
* [lwip](https://github.com/unikraft/lib-lwip)
* [lib-libunwind](https://github.com/unikraft/lib-libunwind)
* [lib-libcxx](https://github.com/unikraft/lib-libcxx)
* [lib-libcxxabi](https://github.com/unikraft/lib-libcxxabi)
* [lib-compiler-rt](https://github.com/unikraft/lib-compiler-rt)
* [lib-newlib](https://github.com/unikraft/lib-newlib)

Make sure that every repository is on the `staging` branch.

All libraries must be cloned in the same folder.
In each programming language subfolder (`c/`, `cpp/`) edit the `Makefile` file and update the variables to point to the correct location:
* Update the `UK_ROOT` variable to point to the location of the Unikraft clone.
* Update the `UK_LIBS` variable to point to the folder storing the library clones.

If you get a `Cannot find library` error at one of the following steps, you may need to add the `lib-` prefix to the libraries listed in the `LIBS` variable. For instance, `$(UK_LIBS)/lwip` becomes `$(UK_LIBS)/lib-lwip`

In each programming language folder, configure the builds by running:
```
make menuconfig
```
You don't need to do anything else here.
The configuration is loaded from the `Config.uk` file.
Just save the configuration.
As a result of this, a `.config` file is created.

Build the unikernel image by running:
```
make
```
The resulting image is in the `build/` subfolder, named `c_kvm-x86_64` or `cpp_kvm-x86_64`.

The resulting image is to be loaded as part of a KVM virtual machine with a software bridge allowing outside network access to the server application.
Start the image by using the `run_net_static` script (you require `sudo` privileges):
```
razvan@yggdrasil:~/.../unikraft-my/apps/app-smart-contract-simple$ ./run_net_static
Usage: ./run_net_static <kvm_image>

razvan@yggdrasil:~/.../unikraft-my/apps/app-smart-contract-simple$ ./run_net_static c/build/c_kvm-x86_64
[...]
0: Set IPv4 address 172.44.0.2 mask 255.255.255.0 gw 172.44.0.1
en0: Added
en0: Interface is up
Powered by
o.   .o       _ _               __ _
Oo   Oo  ___ (_) | __ __  __ _ ' _) :_
oO   oO ' _ `| | |/ /  _)' _` | |_|  _)
oOo oOO| | | | |   (| | | (_) |  _) :_
 OoOoO ._, ._:_:_,\_._,  .__,_:_, \___)
                     Rhea 0.4.0~478fdb5
Listening on port 12345...
[...]
```

The server applications waits for connection on IP address `172.44.0.2`, on TCP port `12345`.
Test it by running (on another terminal):
```
$ netcat 172.44.0.2 12345
a
b
```
Send `a` to the server and the server replies with an increment (`b`).
The server prints out information messages:
```
Received connection from 172.44.0.1:43464
Received: 2657
Sent: 2658
```

When done, close the server / KVM machine using `Ctrl+c`.
