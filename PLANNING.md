# OS Planning

### Next Steps:
- [ ] finish fs implementation
- [ ] load user programs
- [ ] ELF parser
- [ ] user privileges
- [ ] process scheduler
- [x] PIT
- [ ] syscalls

### Process Context
- code, data, heap, and stack in RAM
- system resources (files)
- virtual memory mapping
- process id

### Scheduler
- round robin queue
- no priority for simplicity, probably easy to add though
- context switching done in software
- triggered by timer (PIT, APIC)

### Syscalls
- int 0x80 software interupts
- process syscalls
    - exec
    - fork
    - wait
    - exit
    - kill
- file i/o syscalls
