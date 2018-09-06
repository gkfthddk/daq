#include <stdio.h>
#include <stdlib.h>
#include "NoticeUOSDAQ3.h"
#include <time.h>
//usage : getPedestal.exe sid duration(sec)

int main(int argc, char *argv[])
{
  double sec;
  int gain;
  int pol;
  int dec;
  int sid;
  char bfilename[256];
  char tfilename[256];
  int nread, nsample;
  int count[2];
  FILE *bfp;
  FILE *tfp;
  unsigned char data[16384];
  int adcX[100];
  int adcY[100];
  int i;
  int ch;
  double sum_pedestalX[100];
  double sum_pedestalY[100];
  int frame;
  int pedestal;

  // check # of argument
  if (argc < 3) {
    printf("insufficient # of argument, program is aborted.\n");
    return -1;
  }

  // get sid
  sid = atoi(argv[1]);

  // get # of read & # of samples
  sec = atof(argv[2]);

  // enter setting here
  // gain : 0 = 5.2 nA, 1 = 10 nA, 2 = 20 nA, 3 = 48 nA,
  //        4 = 96 nA, 5 = 192 nA, 6 = 288 nA, 7 = 384 nA
  // pol : 0 = negative current, 1 = positive current
  // dec : 1/2/4/8/16 : ADC decimation
  gain = 3;
  pol = 0;
  dec = 1;

  // open USB3
  UOSDAQ3init(0);

  // open UOSDAQ3  
  UOSDAQ3open(sid, 0);
  
  // write settings
  UOSDAQ3setup_DAQ(sid, gain, pol, dec);

  // make filename
  sprintf(bfilename, "uosdaq_pedestal_%d.dat", sid);
  sprintf(tfilename, "uosdaq_pedestal_%d.txt", sid);

  // get # of frames to take
  nread = UOSDAQ3get_NUM(sec, dec);
  nsample = nread * 32;
  count[0] = nsample;

  // open binary data file
  bfp = fopen(bfilename, "wb");

  // write # of samples
  fwrite(count, 4, 1, bfp);

  // reset DAQ
  UOSDAQ3reset(sid);

  // print start DAQ
  printf("Taking data for %lf seconds\n", sec);

  // start DAQ by software
  UOSDAQ3start_DAQ(sid);

  for (i = 0; i < nread; i++) {
    UOSDAQ3read_DATA(sid, data);
    fwrite(data, 1, 16384, bfp);
  }

  // close binary file
  fclose(bfp);

  // stop DAQ
  UOSDAQ3stop_DAQ(sid);

  // print stop DAQ
  printf("Finish taking data\n");

  // convert to text file
  printf("Converting binary data to text file\n");
  printf("Extracting pedestal\n");

  // open files
  bfp = fopen(bfilename, "rb");
  tfp = fopen(tfilename, "wt");

  // get # of samples
  fread(count, 4, 1, bfp);
  nsample = count[0];

  for (ch = 0; ch < 100; ch++) {
    sum_pedestalX[ch] = 0;
    sum_pedestalY[ch] = 0;
  }

  for (i = 0; i < nsample; i++) {
    fread(data, 1, 512, bfp);
    
    // convert to ADC value
    UOSDAQ3get_DATA(data, adcX, adcY, &frame);

    for (ch = 0; ch < 100; ch++) {
      sum_pedestalX[ch] += adcX[ch];
      sum_pedestalY[ch] += adcY[ch];
    }
  }
  
  //Get pedestal
  for (ch = 0; ch < 100; ch++) {
    pedestal = (int)(sum_pedestalX[ch] / nsample);
    fprintf(tfp, "%d\n", pedestal);
  }
  
  for (ch = 0; ch < 100; ch++) {
    pedestal = (int)(sum_pedestalY[ch] / nsample);
    fprintf(tfp, "%d\n", pedestal);
  }

  fclose(bfp);
  fclose(tfp);
 
  // close UOSDAQ3  
  UOSDAQ3close(sid);

  // close USB3
  UOSDAQ3exit(0);
  
  // print end
  printf("All done.\n");
  return 0;
}

