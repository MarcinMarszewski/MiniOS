# MiniOS
 Minimal bootable binary for x86 architecture

 Unfortunately I have to leave this project for now. I hope I'll be able to return to the idea eventually.

# Currently implemented
- Interrupt seting
- Ability to set keyboard events
- Harddrive driver
- Filesystem (to be reworked)
- Screen driver
- Speaker driver (PC Speaker)
- Library for memory managment
- Terminal (currently has issues)

 # My (a little bit less) Terrible Filesystem
Files consist of multiple segments, each segment following the structure:

[rawData:508b] [previousSegmentNumber:2b] [nextSegmentNumber:2b]

In case of direcotires raw data segment contains uninterrupted stream of file descriptors with the structore of:

[totalDescriptorLength:1b] [fileName:variable lenght <= 248b] [dataBytesInLastSegment:1b] [firstSegmentNumber:2b] [lastSegmentNumber:2] [filetype(1-directory 2-datafile):1b]


# Planned to include
- minimalistic shell with basic commands

# Resources used for this project:
- https://github.com/cfenollosa/os-tutorial
- https://wiki.osdev.org/