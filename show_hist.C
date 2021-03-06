{
  FILE *fp;
  char filename[256];
  int nrun;
  int sid;
  int sch;
  int nsample;
  int ch;
  int i;
  int frame;
  int adcX;
  int adcY;
  float py;

  printf("enter run number : ");
  scanf("%d", &nrun);

  printf("enter sid number : ");
  scanf("%d", &sid);

  sprintf(filename, "uosdaq_%d_%d.txt", nrun, sid);

  printf("enter channel number to show : ");
  scanf("%d", &sch);

  fp = fopen(filename, "rt");

  fscanf(fp, "%d", &nsample);
  
  TCanvas *c1 = new TCanvas("c1", "KIMS", 800, 800);
  c1->Divide(1,2);
  TH1F *hoscd1 = new TH1F("hoscd1", "histogram X channel", 4200, -124, 4096);
  TH1F *hoscd2 = new TH1F("hoscd2", "histogram Y channel", 4200, -124, 4096);

  hoscd1->Reset();
  hoscd2->Reset();
  
  //for (i = 0; i < nsample; i++) {
  for (i = 0; i < 100000; i++) {
    // read frame number
    fscanf(fp, "%d", &frame);
   
    // fill X channel
    for (ch = 0; ch <= sch; ch++)
      fscanf(fp, "%d", &adcX);
      py = adcX;
      hoscd1->Fill(py);
 
    for (ch = sch + 1; ch < 100; ch++)
      fscanf(fp, "%d", &adcX);

    // fill Y channel
    for (ch = 0; ch <= sch; ch++)
      fscanf(fp, "%d", &adcY);
      py = adcY;
      hoscd2->Fill(py);
 
    for (ch = sch + 1; ch < 100; ch++)
      fscanf(fp, "%d", &adcY);

    //printf("%d / %d filled, frame # = %d\n", i + 1, nsample, frame);
  }

  fclose(fp);

  c1->cd(1);
  hoscd1->Draw();
  c1->cd(2);
  hoscd2->Draw();
  c1->Modified();
  c1->Update();
}
    

