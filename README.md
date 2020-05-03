JSON parser from: https://github.com/udp/json-parser<br>
openpose from: https://github.com/CMU-Perceptual-Computing-Lab/openpose<br>

Functions
-
Fall detection using a slope algorithm supplementing OpenPose, inspiring(I hope) more work related to this module-like approach.<br>
Compared to other detection methods that employs machine learning out there, this requires far less usage on the already strained system that are running OpenPose.

Compiling
-
`gcc main.c json.c -lm`

Usage
-
Compile openpose to home dir<br >
Copy test to the folder named openpose<br >
Modify main.c to the correct folder pathname<br>
Modify and copy the contents under "run" <br >
`cd` to openpose<br >
Paste the command<br >

TODO
-
1. Bug checks<br >
2. Check caveats<br >
3. Remove hardcoded path<br >
4. Improvise algorithm, maybe replace entire slope method
