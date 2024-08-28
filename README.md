# MiniOS
 Minimal bootable binary for x86 architecture

# Currently implemented
- Interrupt seting
- Ability to set keyboard events
- Harddrive driver
- Filesystem (to be reworked)
- Screen driver
- Speaker driver (PC Speaker)
- Library for memory managment

 # My Terrible Filesystem
Files consist of set size pages linked together
 File page structure:
 - 1 byte - filetype [D-directory, F-File]
 - 1 byte - flags [... , isFirstSegment, isLastSegment]
 - 2 bytes- address of next segment
 - 60 bytes left for future metadata
 In case of file rest is space for raw data
 For directories data is stored in the format of:
 - 61 bytes- file name
 - 1 null byte
 - 2 bytes - file address


# Todo:
- rework filesystem

# Planned to include
- basic tree style file system handling
- minimalistic shell with basic commands

# Resources used for this project:
- https://github.com/cfenollosa/os-tutorial
- https://wiki.osdev.org/