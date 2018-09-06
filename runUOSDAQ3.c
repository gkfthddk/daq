#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "NoticeUOSDAQ3.h"

int main(int argc, char *argv[])
{
  int nrun;
  double sec;
  int gain;
  int pol;
  int dec;
  int sid;
  char bfilename[256];
  char tfilename[256];
  char pfilename[256];
  int nread, nsample;
  int count[2];
  FILE *bfp;
  FILE *tfp;
  FILE *pfp;
  unsigned char data[16384];
  int adcX[100];
  int adcY[100];
  int pedX[100];
  int pedY[100];
  int i;
  int ch;
  int frame;
  time_t start_time, stop_time;
  double diff_time;

  // check # of argument
  if (argc < 4) {
    printf("insufficient # of argument, program is aborted.\n");
    return -1;
  }

  // get sid
  sid = atoi(argv[1]);

  // get run #
  nrun = atoi(argv[2]);

  // get # of read & # of samples
  sec = atof(argv[3]);

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
  sprintf(bfilename, "uosdaq_%d_%d.dat", nrun, sid);
  sprintf(tfilename, "uosdaq_%d_%d.txt", nrun, sid);
  sprintf(pfilename, "uosdaq_pedestal_%d.txt", sid);

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
  printf("Taking data : run # = %d for %lf seconds\n", nrun, sec);

  // start DAQ by software
  start_time = time(NULL);
  UOSDAQ3start_DAQ(sid);

  for (i = 0; i < nread; i++) {
    UOSDAQ3read_DATA(sid, data);
    fwrite(data, 1, 16384, bfp);
  }

  stop_time = time(NULL);
  diff_time = difftime(stop_time, start_time);

  // close binary file
  fclose(bfp);

  // stop DAQ
  UOSDAQ3stop_DAQ(sid);

  // print stop DAQ
  printf("DAQ time : %lf\n", diff_time);
  printf("Finish taking data\n");

  // convert to text file
  printf("Converting binary data to text file\n");

  // open files
  bfp = fopen(bfilename, "rb");
  tfp = fopen(tfilename, "wt");
  pfp = fopen(pfilename, "rt");

  //read in pedestal
  for (ch = 0; ch < 100; ch++) {
     fscanf(pfp, "%d\n", &pedX[ch]);
  }

  for (ch = 0; ch < 100; ch++) {
     fscanf(pfp, "%d\n", &pedY[ch]);
  }

  fclose(pfp);

  // get # of samples
  fread(count, 4, 1, bfp);
  nsample = count[0];
  fprintf(tfp, "%d\n", nsample);

  for (i = 0; i < nsample; i++) {
    fread(data, 1, 512, bfp);
    
    // convert to ADC value
    UOSDAQ3get_DATA(data, adcX, adcY, &frame);

    // write frame number
    fprintf(tfp, "%d\n", frame);    

    // write ADC value
    for (ch = 0; ch < 100; ch++)
      fprintf(tfp, "%d\n", adcX[ch] - pedX[ch]);

    for (ch = 0; ch < 100; ch++)
      fprintf(tfp, "%d\n", adcY[ch] - pedY[ch]);
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
