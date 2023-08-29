# sacsnr

Get SNR of SAC files in a specified time window.

$S N R=10 \log _{10}\left(\frac{A_s}{A_N}\right)^2$

Calculation of SNR of radial and tangential seismic records before estimating Receiver Functions(RFs) 

## SAC I/O functions

- `sacio.h`: Head file for SAC file format, and prototype for SAC IO functions.
- `sacio.c`: Definitions of several SAC IO functions.
  - `read_sac_head`: read SAC header
  - `read_sac`: read SAC binary data
  - `read_sac_xy`: read SAC binary XY data
  - `read_sac_pdw`: read SAC data in a partial data window (cut option)
  - `write_sac`: write SAC binary data
  - `write_sac_xy`: write SAC binary XY data
  - `new_sac_head`: create a minimal SAC header
  - `sac_head_index`: return the index of a SAC head field
  - `issac`: Check if a file in in SAC format

## SAC Utilities

- [sacsnr](#sacsnr): Get SNR of SAC files in a specified time window.

### `sacsnr`
```
Usage:
  sacsnr [-Ttmark/s0/s1/n0/n1] sacfiles
  tmark  sac hearder t0~9 
  s0-s1  signal time window 
  n0-n1  noise time window  
  eg:  sacsnr -T7/0/50/-50/0 CD2.BHN.2007.294.210257.SAC
Options:
  -h    show usage.
```

## Compile and Use
Use the command in this folder:
```
gcc -o sacsnr sacsnr.c sacio.c sacio.h -lm
```
Place the tool directly into the $SACHOME/bin.

SACHOME refers to the folder where the SAC is installed