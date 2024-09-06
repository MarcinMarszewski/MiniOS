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

 # My (a little bit less) Terrible Filesystem
Files consist of multiple segments, each segment following the structure:

[rawData:508b] [previousSegmentNumber:2b] [nextSegmentNumber:2b]

In case of direcotires raw data segment contains uninterrupted stream of file descriptors with the structore of:

[totalDescriptorLength:1b] [fileName:variable lenght] [dataBytesInLastSegment:1b] [firstSegmentNumber:2b] [lastSegmentNumber:2] [filetype(1-directory 2-datafile):1b]


# Todo:
- improvements and fixes to filesystem

# Planned to include
- basic tree style file system handling
- minimalistic shell with basic commands

# Resources used for this project:
- https://github.com/cfenollosa/os-tutorial
- https://wiki.osdev.org/