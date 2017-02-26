# Simple antirootkit 

## About

There are a number of ways for malware to intrude into the system. This project represents a simple software solution that helps to remediate one aspect of possible rootkit intrusions – System Service Table violations performed to hide files, services, or processes. This antirootkit restores original SST.

## Implementation

The implementation of rootkit detection is based on the idea of comparing the current version of SST with the one stored in ntoskernel.exe. The techniques of work with the memory-mapped files in kernel mode are used.

You can find more explanations on how to develop an anti-rootkit, details and step-by-step programming tutorial in the [related article](https://www.apriorit.com/dev-blog/180-simple-antirootkit). 

## License

Licensed under the MIT license. © Apriorit.
