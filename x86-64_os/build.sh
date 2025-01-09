docker build buildenv -t myos-buildenv
docker run --rm -it -v "$(pwd)":/root/env myos-buildenv

# make build-x86_64
# qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso