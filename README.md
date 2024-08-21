# MiniOS
 Minimal bootable binary for x86 architecture

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
- interface over filesystem functions

# Planned to include
- basic tree style file system handling
- minimalistic shell with basic commands
- possibly a special shell language

Repository started after reading throug https://github.com/cfenollosa/os-tutorial repo