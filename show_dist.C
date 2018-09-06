{
  FILE *fp;
  char filename[256];
  int nrun;
  int sid=1;
  int nsample;
  int ch;
  int i;
  int frame;
  int adcX;
  int adcY;
  float sumX[100];
  float sumY[100];
  float px;
 
  printf("enter run number : ");
  scanf("%d", &nrun);

  //printf("enter sid number : ");
  //scanf("%d", &sid);

  sprintf(filename, "uosdaq_%d_%d.txt", nrun, sid);
//  sprintf(filename, "uosdaq_pedestal_%d.txt", sid);

  fp = fopen(filename, "rt");

  fscanf(fp, "%d", &nsample);
  
  TCanvas *c1 = new TCanvas("c1", "KIMS", 800, 800);
  c1->Divide(1,2);
  TH1F *hoscd1 = new TH1F("hoscd1", "X distribution", 100, 0, 100);
  TH1F *hoscd2 = new TH1F("hoscd2", "Y distribution", 100, 0, 100);

  hoscd1->Reset();
  hoscd2->Reset();

  for (ch = 0; ch < 100; ch++) {
    sumX[ch] = 0.0;
    sumY[ch] = 0.0;
  }

  //for (i = 0; i < nsample; i++) {
  for (i = 0; i < nsample/8.; i++) {
    // read frame number
    fscanf(fp, "%d", &frame);
   
    // add ADC to get sum for X channels
    for (ch = 0; ch < 100; ch++) {
      fscanf(fp, "%d", &adcX);
      sumX[ch] = sumX[ch] + adcX;
    }
 
    // add ADC to get sum for Y channels
    for (ch = 0; ch < 100; ch++) {
      fscanf(fp, "%d", &adcY);
      sumY[ch] = sumY[ch] + adcY;
    }

    printf("%d / %d filled, frame # = %d\n", i + 1, nsample, frame);
  }

  fclose(fp);

  for (ch = 0; ch < 100; ch++) {
    px = ch + 0.5;			
    hoscd1->Fill(px, sumX[ch]);
    hoscd2->Fill(px, sumY[ch]);
  }

  c1->cd(1);
  hoscd1->Draw();
  c1->cd(2);
  hoscd2->Draw();
  c1->Modified();
  c1->Update();
}

