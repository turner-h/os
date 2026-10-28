# OS
![GitHub top language](https://img.shields.io/github/languages/top/turner-h/os)
![CI](https://github.com/turner-h/os/actions/workflows/ci.yml/badge.svg)
![Repo Size](https://img.shields.io/github/repo-size/turner-h/os)

A hobby operating system written in C


### Features
- Custom two-stage bootloader
- Paging and virtual memory
- Page frame allocator
- Heap memory allocator
- Interrupt handling
- Unit testing

### Next Steps:
- [ ] finish fs implementation
- [ ] load user programs

### Usage
```bash
make - runs the os in QEMU
make test - runs unit tests
```

### Dependencies
- i386-elf cross compiler
- mtools
