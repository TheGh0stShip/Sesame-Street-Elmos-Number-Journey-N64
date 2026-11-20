# g1: level gzip streams
Headers of all 16 members: method 8, FNAME set (original .bif names), mtime 0, XFL 0, OS byte 0 (FAT/DOS). The system gzip (1.14) at levels 1-9 and zlib at every setting do not reproduce the deflate streams. Hypothesis not yet tested: the 16-bit DOS build of GNU gzip 1.2.4 (smaller buffers change block splitting). Building and running the public 1.2.4 source in the lab was blocked by the permission system; needs the user to allow it.
Tool: tools/gzprobe.py (reads headers, tests system gzip levels).
