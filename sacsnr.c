/*
 *  Get SNR of SAC files in a specified time window
 *  
 *  SNR=10 * log_10⁡((A_S/A_N )^2)
 *
 *  Author: JandyZ
 *  
 *  Revision:
 *    2023-3-28  JandyZ   Initial coding.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <float.h>
#include <math.h>
#include "sacio.h"


void usage(void);

void usage() {
    fprintf(stderr, "Get SNR of SAC files in a specified time window.\n");
    fprintf(stderr, "                                                          \n");
    fprintf(stderr, "Usage:                                                    \n");
    fprintf(stderr, "  sacsnr [-Ttmark/s0/s1/n0/n1] sacfiles                   \n");
    fprintf(stderr, "                                                          \n");
    fprintf(stderr, "  tmark  SAC header t0~9                                  \n");
    fprintf(stderr, "  s0-s1  signal time window                               \n");
    fprintf(stderr, "  n0-n1  noise time window                                \n"); 
    fprintf(stderr, "                                                          \n"); 
    fprintf(stderr, "  eg:  sacsnr -T7/0/50/-50/0 CD2.BHN.2007.294.210257.SAC  \n");
    fprintf(stderr, "Options:                                                  \n");
    fprintf(stderr, "  -h    show usage.                                       \n");
}

int main(int argc, char* argv[])
{
    int c;
    //int mode;
    int error;
    int cut = 0;   /* cut a time window or not */
    int tmark;
    float s0, s1;
    float n0, n1;
    int i;
    
    error = 0;
    while ((c = getopt(argc, argv, "T:h")) != -1) { /*argc 参数数量 argv 字符串变量数组*/
        switch (c) {
        case 'T':
            if (sscanf(optarg, "%d/%f/%f/%f/%f", &tmark, &s0, &s1, &n0, &n1) != 5) 
            {
                error++;
            }
            else {
                cut = 1;
            }
            break;
        case 'h':
            usage();
            return -1;
        }
    }

    if (argc - optind < 1 || error) {
        usage();
        exit(-1);
    }


    for (i = optind; i < argc; i++) {  /* loop over files */
        float* data1, * data2;
        SACHEAD hd1, hd2;
        int j;
        int k;

        if (cut) data1 = read_sac_pdw(argv[i], &hd1, tmark, s0, s1);
        else     data1 = read_sac(argv[i], &hd1);
        if (cut) data2 = read_sac_pdw(argv[i], &hd2, tmark, n0, n1);
        else     data2 = read_sac(argv[i], &hd2);

        /* do SNR_cal in time domain */
        float snr_value;
        float ssuma, nsuma;
        int sn, nn;
        float As, An; /*RMS*/

        /* initialization */
        snr_value = 0,As = 0 , An = 0; 
        ssuma = 0, nsuma = 0;
        sn = 0, nn = 0;

        for (j = 0; j < hd1.npts; j++) {
            ssuma = ssuma + (data1[j] * data1[j]);
            sn = sn + 1;
        }
        As = sqrt((ssuma / sn));
        for (k = 0; k < hd2.npts; k++) {
            nsuma = nsuma + (data2[k] * data2[k]);
            nn = nn + 1;
        }
        An = sqrt((nsuma / nn));
        if (An != 0) snr_value = 10 * log10((As * As) / (An * An));
        else    snr_value = 0;
        printf("%s %g\n", argv[i], snr_value);
    }

    return 0;
}
